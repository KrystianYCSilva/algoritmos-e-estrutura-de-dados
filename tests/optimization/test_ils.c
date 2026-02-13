/**
 * @file test_ils.c
 * @brief Testes do Iterated Local Search (ILS) - Wave 5
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/ils.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(ils_default_config_values) {
    ILSConfig cfg = ils_default_config();
    ASSERT_EQ(cfg.max_iterations, 1000);
    ASSERT_EQ(cfg.local_search_iterations, 200);
    ASSERT_EQ(cfg.local_search_neighbors, 20);
    ASSERT_EQ(cfg.perturbation_strength, 1);
    ASSERT_EQ(cfg.acceptance, ILS_ACCEPT_BETTER);
    ASSERT_NEAR(cfg.sa_initial_temp, 10.0, 1e-9);
    ASSERT_NEAR(cfg.sa_alpha, 0.95, 1e-9);
    ASSERT_EQ(cfg.restart_threshold, 50);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42);
}

// ============================================================================
// TESTES: ILS ACCEPT BETTER - TSP
// ============================================================================

TEST(ils_better_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 100;
    cfg.local_search_iterations = 50;
    cfg.local_search_neighbors = 10;
    cfg.perturbation_strength = 1;
    cfg.acceptance = ILS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.best.cost < 200.0);
    ASSERT_TRUE(result.num_iterations > 0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(ils_better_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 200;
    cfg.local_search_iterations = 100;
    cfg.local_search_neighbors = 15;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_2opt,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_evaluations > result.num_iterations);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ILS ACCEPT ALWAYS - TSP
// ============================================================================

TEST(ils_always_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 80;
    cfg.local_search_iterations = 30;
    cfg.acceptance = ILS_ACCEPT_ALWAYS;
    cfg.seed = 123;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, 80);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ILS SA-LIKE ACCEPTANCE - TSP
// ============================================================================

TEST(ils_sa_like_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 150;
    cfg.local_search_iterations = 50;
    cfg.acceptance = ILS_ACCEPT_SA_LIKE;
    cfg.sa_initial_temp = 50.0;
    cfg.sa_alpha = 0.98;
    cfg.seed = 77;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_2opt,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ILS RESTART ACCEPTANCE - TSP
// ============================================================================

TEST(ils_restart_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 200;
    cfg.local_search_iterations = 50;
    cfg.acceptance = ILS_ACCEPT_RESTART;
    cfg.restart_threshold = 30;
    cfg.seed = 55;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, 200);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ILS - CONTINUOUS
// ============================================================================

TEST(ils_sphere_better) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 100;
    cfg.local_search_iterations = 100;
    cfg.local_search_neighbors = 20;
    cfg.perturbation_strength = 3;
    cfg.acceptance = ILS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(double) * inst->dimensions,
                               inst->dimensions,
                               continuous_evaluate,
                               continuous_neighbor_gaussian,
                               NULL,
                               continuous_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_TRUE(result.best.cost < 50.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(ils_rastrigin_sa_like) {
    ContinuousInstance *inst = continuous_create_rastrigin(3);
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 150;
    cfg.local_search_iterations = 80;
    cfg.local_search_neighbors = 15;
    cfg.perturbation_strength = 2;
    cfg.acceptance = ILS_ACCEPT_SA_LIKE;
    cfg.sa_initial_temp = 20.0;
    cfg.sa_alpha = 0.97;
    cfg.seed = 99;

    OptResult result = ils_run(&cfg,
                               sizeof(double) * inst->dimensions,
                               inst->dimensions,
                               continuous_evaluate,
                               continuous_neighbor_gaussian,
                               NULL,
                               continuous_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(ils_zero_iterations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(ils_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 50;
    cfg.local_search_iterations = 30;
    cfg.acceptance = ILS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    ASSERT_NOT_NULL(result.convergence);
    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(ils_valid_tour_output) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ILSConfig cfg = ils_default_config();
    cfg.max_iterations = 30;
    cfg.seed = 42;

    OptResult result = ils_run(&cfg,
                               sizeof(int) * inst->n_cities,
                               inst->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_swap,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               inst);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    double verify_cost = tsp_tour_cost(tour, inst->n_cities, inst);
    ASSERT_NEAR(result.best.cost, verify_cost, 1e-6);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Iterated Local Search (Wave 5) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(ils_default_config_values);

    printf("\n[ILS Accept Better - TSP]\n");
    RUN_TEST(ils_better_tsp5);
    RUN_TEST(ils_better_tsp10);

    printf("\n[ILS Accept Always - TSP]\n");
    RUN_TEST(ils_always_tsp5);

    printf("\n[ILS SA-like Acceptance - TSP]\n");
    RUN_TEST(ils_sa_like_tsp10);

    printf("\n[ILS Restart Acceptance - TSP]\n");
    RUN_TEST(ils_restart_tsp10);

    printf("\n[ILS Continuous]\n");
    RUN_TEST(ils_sphere_better);
    RUN_TEST(ils_rastrigin_sa_like);

    printf("\n[Edge Cases]\n");
    RUN_TEST(ils_zero_iterations);
    RUN_TEST(ils_convergence_monotonic);
    RUN_TEST(ils_valid_tour_output);

    printf("\n=== Todos os 11 testes passaram! ===\n");
    return 0;
}
