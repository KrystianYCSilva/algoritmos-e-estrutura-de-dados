/**
 * @file test_benchmarks.c
 * @brief Testes para infraestrutura de otimizacao (Wave 0)
 *
 * Valida common (OptSolution, OptResult, RNG), benchmarks TSP
 * (instancias, custo, vizinhancas, perturbacao, geracao) e
 * benchmarks continuos (funcoes, vizinhanca gaussiana, geracao).
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: OptSolution
// ============================================================================

TEST(solution_create) {
    OptSolution sol = opt_solution_create(sizeof(int) * 5);
    ASSERT_NOT_NULL(sol.data);
    ASSERT_EQ(sol.data_size, sizeof(int) * 5);
    ASSERT_NEAR(sol.cost, DBL_MAX, 1.0);
    opt_solution_destroy(&sol);
    ASSERT_NULL(sol.data);
}

TEST(solution_create_zero) {
    OptSolution sol = opt_solution_create(0);
    ASSERT_NULL(sol.data);
    ASSERT_EQ(sol.data_size, (size_t)0);
    opt_solution_destroy(&sol);
}

TEST(solution_clone) {
    OptSolution orig = opt_solution_create(sizeof(double) * 3);
    ASSERT_NOT_NULL(orig.data);
    double *d = (double*)orig.data;
    d[0] = 1.0; d[1] = 2.0; d[2] = 3.0;
    orig.cost = 42.0;

    OptSolution clone = opt_solution_clone(&orig);
    ASSERT_NOT_NULL(clone.data);
    ASSERT_NE(clone.data, orig.data);
    ASSERT_EQ(clone.data_size, orig.data_size);
    ASSERT_NEAR(clone.cost, 42.0, 1e-9);

    double *cd = (double*)clone.data;
    ASSERT_NEAR(cd[0], 1.0, 1e-9);
    ASSERT_NEAR(cd[1], 2.0, 1e-9);
    ASSERT_NEAR(cd[2], 3.0, 1e-9);

    opt_solution_destroy(&orig);
    opt_solution_destroy(&clone);
}

TEST(solution_clone_null) {
    OptSolution clone = opt_solution_clone(NULL);
    ASSERT_NULL(clone.data);
}

// ============================================================================
// TESTES: OptResult
// ============================================================================

TEST(result_create_destroy) {
    OptResult res = opt_result_create(100);
    ASSERT_NOT_NULL(res.convergence);
    ASSERT_EQ(res.convergence_size, (size_t)100);
    ASSERT_EQ(res.num_iterations, (size_t)0);
    ASSERT_EQ(res.num_evaluations, (size_t)0);
    opt_result_destroy(&res);
    ASSERT_NULL(res.convergence);
}

// ============================================================================
// TESTES: RNG
// ============================================================================

TEST(rng_uniform_range) {
    opt_set_seed(12345);
    for (int i = 0; i < 1000; i++) {
        double v = opt_random_uniform();
        ASSERT_TRUE(v >= 0.0);
        ASSERT_TRUE(v < 1.0);
    }
}

TEST(rng_int_range) {
    opt_set_seed(54321);
    for (int i = 0; i < 1000; i++) {
        int v = opt_random_int(5, 10);
        ASSERT_TRUE(v >= 5);
        ASSERT_TRUE(v <= 10);
    }
}

TEST(rng_gaussian_distribution) {
    opt_set_seed(99999);
    double sum = 0.0;
    int n = 10000;
    for (int i = 0; i < n; i++) {
        sum += opt_random_gaussian();
    }
    double mean = sum / n;
    ASSERT_NEAR(mean, 0.0, 0.1);
}

TEST(rng_deterministic_seed) {
    opt_set_seed(42);
    double v1 = opt_random_uniform();
    double v2 = opt_random_uniform();

    opt_set_seed(42);
    double v3 = opt_random_uniform();
    double v4 = opt_random_uniform();

    ASSERT_NEAR(v1, v3, 1e-15);
    ASSERT_NEAR(v2, v4, 1e-15);
}

// ============================================================================
// TESTES: TSP INSTANCIAS
// ============================================================================

TEST(tsp_example_5_create) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);
    ASSERT_EQ(inst->n_cities, (size_t)5);
    ASSERT_TRUE(inst->known_optimum > 0);

    for (size_t i = 0; i < 5; i++) {
        ASSERT_NEAR(inst->dist_matrix[i][i], 0.0, 1e-9);
        for (size_t j = 0; j < 5; j++) {
            ASSERT_NEAR(inst->dist_matrix[i][j], inst->dist_matrix[j][i], 1e-9);
        }
    }

    tsp_instance_destroy(inst);
}

TEST(tsp_example_10_create) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);
    ASSERT_EQ(inst->n_cities, (size_t)10);
    ASSERT_TRUE(inst->known_optimum > 0);
    tsp_instance_destroy(inst);
}

TEST(tsp_example_20_create) {
    TSPInstance *inst = tsp_create_example_20();
    ASSERT_NOT_NULL(inst);
    ASSERT_EQ(inst->n_cities, (size_t)20);
    ASSERT_TRUE(inst->known_optimum > 0);
    tsp_instance_destroy(inst);
}

TEST(tsp_random_create) {
    TSPInstance *inst = tsp_create_random(15, 42);
    ASSERT_NOT_NULL(inst);
    ASSERT_EQ(inst->n_cities, (size_t)15);
    ASSERT_NEAR(inst->known_optimum, -1.0, 1e-9);

    for (size_t i = 0; i < 15; i++) {
        ASSERT_TRUE(inst->x[i] >= 0.0);
        ASSERT_TRUE(inst->x[i] <= 100.0);
    }

    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: TSP CUSTO
// ============================================================================

TEST(tsp_tour_cost_sequential) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    int tour[] = {0, 1, 2, 3, 4};
    double cost = tsp_tour_cost(tour, 5, inst);
    ASSERT_NEAR(cost, inst->known_optimum, 0.01);

    tsp_instance_destroy(inst);
}

TEST(tsp_tour_cost_reversed) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    int tour_fwd[] = {0, 1, 2, 3, 4};
    int tour_rev[] = {4, 3, 2, 1, 0};
    double cost_fwd = tsp_tour_cost(tour_fwd, 5, inst);
    double cost_rev = tsp_tour_cost(tour_rev, 5, inst);
    ASSERT_NEAR(cost_fwd, cost_rev, 1e-9);

    tsp_instance_destroy(inst);
}

TEST(tsp_tour_cost_null_guard) {
    double cost = tsp_tour_cost(NULL, 5, NULL);
    ASSERT_TRUE(cost > 1e17);
}

// ============================================================================
// TESTES: TSP VIZINHANCAS
// ============================================================================

TEST(tsp_swap_neighbor) {
    opt_set_seed(100);
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    int tour[] = {0, 1, 2, 3, 4};
    int neighbor[5];

    tsp_neighbor_swap(tour, neighbor, 5, inst);
    ASSERT_TRUE(tsp_is_valid_tour(neighbor, 5));

    int diffs = 0;
    for (int i = 0; i < 5; i++) {
        if (tour[i] != neighbor[i]) diffs++;
    }
    ASSERT_EQ(diffs, 2);

    tsp_instance_destroy(inst);
}

TEST(tsp_2opt_neighbor) {
    opt_set_seed(200);
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    int tour[] = {0, 1, 2, 3, 4};
    int neighbor[5];

    tsp_neighbor_2opt(tour, neighbor, 5, inst);
    ASSERT_TRUE(tsp_is_valid_tour(neighbor, 5));

    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: TSP PERTURBACAO
// ============================================================================

TEST(tsp_double_bridge) {
    opt_set_seed(300);
    TSPInstance *inst = tsp_create_random(20, 42);
    ASSERT_NOT_NULL(inst);

    int tour[20];
    for (int i = 0; i < 20; i++) tour[i] = i;

    int perturbed[20];
    tsp_perturb_double_bridge(tour, perturbed, 20, 1, inst);
    ASSERT_TRUE(tsp_is_valid_tour(perturbed, 20));

    int diffs = 0;
    for (int i = 0; i < 20; i++) {
        if (tour[i] != perturbed[i]) diffs++;
    }
    ASSERT_TRUE(diffs > 0);

    tsp_instance_destroy(inst);
}

TEST(tsp_double_bridge_small_fallback) {
    int tour[] = {0, 1, 2, 3};
    int perturbed[4];
    tsp_perturb_double_bridge(tour, perturbed, 4, 1, NULL);

    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(tour[i], perturbed[i]);
    }
}

// ============================================================================
// TESTES: TSP GERACAO
// ============================================================================

TEST(tsp_generate_random_tour) {
    opt_set_seed(400);
    int tour[10];
    tsp_generate_random(tour, 10, NULL);
    ASSERT_TRUE(tsp_is_valid_tour(tour, 10));
}

TEST(tsp_is_valid_tour_checks) {
    int valid[] = {2, 0, 4, 1, 3};
    ASSERT_TRUE(tsp_is_valid_tour(valid, 5));

    int dup[] = {0, 1, 2, 2, 4};
    ASSERT_FALSE(tsp_is_valid_tour(dup, 5));

    int oob[] = {0, 1, 2, 3, 5};
    ASSERT_FALSE(tsp_is_valid_tour(oob, 5));

    ASSERT_FALSE(tsp_is_valid_tour(NULL, 5));
}

// ============================================================================
// TESTES: CONTINUOUS INSTANCIAS
// ============================================================================

TEST(continuous_sphere_create) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);
    ASSERT_EQ(inst->dimensions, (size_t)3);
    ASSERT_NEAR(inst->lower_bound, -5.12, 1e-9);
    ASSERT_NEAR(inst->upper_bound, 5.12, 1e-9);
    ASSERT_NEAR(inst->known_optimum, 0.0, 1e-9);
    continuous_instance_destroy(inst);
}

TEST(continuous_all_types_create) {
    ContinuousInstance *r = continuous_create_rastrigin(5);
    ContinuousInstance *ro = continuous_create_rosenbrock(5);
    ContinuousInstance *a = continuous_create_ackley(5);
    ContinuousInstance *s = continuous_create_schwefel(5);

    ASSERT_NOT_NULL(r);
    ASSERT_NOT_NULL(ro);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(s);

    ASSERT_EQ(r->dimensions, (size_t)5);
    ASSERT_EQ(ro->dimensions, (size_t)5);
    ASSERT_EQ(a->dimensions, (size_t)5);
    ASSERT_EQ(s->dimensions, (size_t)5);

    continuous_instance_destroy(r);
    continuous_instance_destroy(ro);
    continuous_instance_destroy(a);
    continuous_instance_destroy(s);
}

// ============================================================================
// TESTES: CONTINUOUS FUNCOES OBJETIVO
// ============================================================================

TEST(continuous_sphere_at_optimum) {
    double x[] = {0.0, 0.0, 0.0};
    ASSERT_NEAR(continuous_sphere(x, 3), 0.0, 1e-9);
}

TEST(continuous_sphere_nonzero) {
    double x[] = {1.0, 2.0, 3.0};
    ASSERT_NEAR(continuous_sphere(x, 3), 14.0, 1e-9);
}

TEST(continuous_rastrigin_at_optimum) {
    double x[] = {0.0, 0.0, 0.0};
    ASSERT_NEAR(continuous_rastrigin(x, 3), 0.0, 1e-9);
}

TEST(continuous_rosenbrock_at_optimum) {
    double x[] = {1.0, 1.0, 1.0};
    ASSERT_NEAR(continuous_rosenbrock(x, 3), 0.0, 1e-9);
}

TEST(continuous_ackley_at_optimum) {
    double x[] = {0.0, 0.0, 0.0};
    ASSERT_NEAR(continuous_ackley(x, 3), 0.0, 1e-6);
}

TEST(continuous_schwefel_at_optimum) {
    double x[] = {420.9687, 420.9687, 420.9687};
    ASSERT_NEAR(continuous_schwefel(x, 3), 0.0, 0.01);
}

TEST(continuous_evaluate_dispatch) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    double x[] = {1.0, 1.0};
    double val = continuous_evaluate(x, 2, inst);
    ASSERT_NEAR(val, 2.0, 1e-9);

    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: CONTINUOUS VIZINHANCA E GERACAO
// ============================================================================

TEST(continuous_gaussian_neighbor) {
    opt_set_seed(500);
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    double current[] = {0.0, 0.0, 0.0};
    double neighbor[3];

    continuous_neighbor_gaussian(current, neighbor, 3, inst);

    for (int i = 0; i < 3; i++) {
        ASSERT_TRUE(neighbor[i] >= inst->lower_bound);
        ASSERT_TRUE(neighbor[i] <= inst->upper_bound);
    }

    continuous_instance_destroy(inst);
}

TEST(continuous_gaussian_neighbor_clamping) {
    opt_set_seed(600);
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    double current[] = {5.12, -5.12};
    double neighbor[2];

    for (int trial = 0; trial < 100; trial++) {
        continuous_neighbor_gaussian(current, neighbor, 2, inst);
        ASSERT_TRUE(neighbor[0] >= inst->lower_bound);
        ASSERT_TRUE(neighbor[0] <= inst->upper_bound);
        ASSERT_TRUE(neighbor[1] >= inst->lower_bound);
        ASSERT_TRUE(neighbor[1] <= inst->upper_bound);
    }

    continuous_instance_destroy(inst);
}

TEST(continuous_generate_random_bounds) {
    opt_set_seed(700);
    ContinuousInstance *inst = continuous_create_ackley(5);
    ASSERT_NOT_NULL(inst);

    double x[5];
    continuous_generate_random(x, 5, inst);

    for (int i = 0; i < 5; i++) {
        ASSERT_TRUE(x[i] >= inst->lower_bound);
        ASSERT_TRUE(x[i] <= inst->upper_bound);
    }

    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: CONTINUOUS UTILIDADES
// ============================================================================

TEST(continuous_fn_name_strings) {
    ASSERT_TRUE(strcmp(continuous_fn_name(CONTINUOUS_SPHERE), "Sphere") == 0);
    ASSERT_TRUE(strcmp(continuous_fn_name(CONTINUOUS_RASTRIGIN), "Rastrigin") == 0);
    ASSERT_TRUE(strcmp(continuous_fn_name(CONTINUOUS_ROSENBROCK), "Rosenbrock") == 0);
    ASSERT_TRUE(strcmp(continuous_fn_name(CONTINUOUS_ACKLEY), "Ackley") == 0);
    ASSERT_TRUE(strcmp(continuous_fn_name(CONTINUOUS_SCHWEFEL), "Schwefel") == 0);
}

TEST(continuous_known_optimum_point_values) {
    double *sphere_opt = continuous_known_optimum_point(CONTINUOUS_SPHERE, 3);
    ASSERT_NOT_NULL(sphere_opt);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(sphere_opt[i], 0.0, 1e-9);
    }
    free(sphere_opt);

    double *rosen_opt = continuous_known_optimum_point(CONTINUOUS_ROSENBROCK, 3);
    ASSERT_NOT_NULL(rosen_opt);
    for (int i = 0; i < 3; i++) {
        ASSERT_NEAR(rosen_opt[i], 1.0, 1e-9);
    }
    free(rosen_opt);

    double *schwef_opt = continuous_known_optimum_point(CONTINUOUS_SCHWEFEL, 2);
    ASSERT_NOT_NULL(schwef_opt);
    for (int i = 0; i < 2; i++) {
        ASSERT_NEAR(schwef_opt[i], 420.9687, 1e-4);
    }
    free(schwef_opt);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Optimization Benchmarks (Wave 0) ===\n\n");

    printf("[OptSolution]\n");
    RUN_TEST(solution_create);
    RUN_TEST(solution_create_zero);
    RUN_TEST(solution_clone);
    RUN_TEST(solution_clone_null);

    printf("\n[OptResult]\n");
    RUN_TEST(result_create_destroy);

    printf("\n[RNG]\n");
    RUN_TEST(rng_uniform_range);
    RUN_TEST(rng_int_range);
    RUN_TEST(rng_gaussian_distribution);
    RUN_TEST(rng_deterministic_seed);

    printf("\n[TSP Instancias]\n");
    RUN_TEST(tsp_example_5_create);
    RUN_TEST(tsp_example_10_create);
    RUN_TEST(tsp_example_20_create);
    RUN_TEST(tsp_random_create);

    printf("\n[TSP Custo]\n");
    RUN_TEST(tsp_tour_cost_sequential);
    RUN_TEST(tsp_tour_cost_reversed);
    RUN_TEST(tsp_tour_cost_null_guard);

    printf("\n[TSP Vizinhancas]\n");
    RUN_TEST(tsp_swap_neighbor);
    RUN_TEST(tsp_2opt_neighbor);

    printf("\n[TSP Perturbacao]\n");
    RUN_TEST(tsp_double_bridge);
    RUN_TEST(tsp_double_bridge_small_fallback);

    printf("\n[TSP Geracao]\n");
    RUN_TEST(tsp_generate_random_tour);
    RUN_TEST(tsp_is_valid_tour_checks);

    printf("\n[Continuous Instancias]\n");
    RUN_TEST(continuous_sphere_create);
    RUN_TEST(continuous_all_types_create);

    printf("\n[Continuous Funcoes Objetivo]\n");
    RUN_TEST(continuous_sphere_at_optimum);
    RUN_TEST(continuous_sphere_nonzero);
    RUN_TEST(continuous_rastrigin_at_optimum);
    RUN_TEST(continuous_rosenbrock_at_optimum);
    RUN_TEST(continuous_ackley_at_optimum);
    RUN_TEST(continuous_schwefel_at_optimum);
    RUN_TEST(continuous_evaluate_dispatch);

    printf("\n[Continuous Vizinhanca/Geracao]\n");
    RUN_TEST(continuous_gaussian_neighbor);
    RUN_TEST(continuous_gaussian_neighbor_clamping);
    RUN_TEST(continuous_generate_random_bounds);

    printf("\n[Continuous Utilidades]\n");
    RUN_TEST(continuous_fn_name_strings);
    RUN_TEST(continuous_known_optimum_point_values);

    printf("\n=== Todos os %d testes passaram! ===\n", 32);
    return 0;
}
