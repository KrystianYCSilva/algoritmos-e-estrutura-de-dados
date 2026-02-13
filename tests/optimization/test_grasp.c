/**
 * @file test_grasp.c
 * @brief Testes do GRASP - Wave 6
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/grasp.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(grasp_default_config_values) {
    GRASPConfig cfg = grasp_default_config();
    ASSERT_EQ(cfg.max_iterations, 500);
    ASSERT_NEAR(cfg.alpha, 0.3, 1e-9);
    ASSERT_EQ(cfg.local_search_iterations, 100);
    ASSERT_EQ(cfg.local_search_neighbors, 20);
    ASSERT_FALSE(cfg.enable_reactive);
    ASSERT_EQ(cfg.reactive_num_alphas, 5);
    ASSERT_EQ(cfg.reactive_block_size, 50);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42);
}

// ============================================================================
// TESTES: GRASP TSP - GREEDY
// ============================================================================

TEST(grasp_tsp5_greedy) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 50;
    cfg.alpha = 0.0;
    cfg.local_search_iterations = 50;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_swap,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.best.cost < 200.0);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(grasp_tsp10_alpha03) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 100;
    cfg.alpha = 0.3;
    cfg.local_search_iterations = 80;
    cfg.local_search_neighbors = 15;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_2opt,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, 100);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(grasp_tsp10_random) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 80;
    cfg.alpha = 1.0;
    cfg.local_search_iterations = 50;
    cfg.seed = 77;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_swap,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: GRASP TSP - REACTIVE
// ============================================================================

TEST(grasp_tsp10_reactive) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 200;
    cfg.enable_reactive = true;
    cfg.reactive_num_alphas = 5;
    cfg.reactive_block_size = 40;
    cfg.local_search_iterations = 50;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_2opt,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, 200);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: GRASP CONTINUOUS
// ============================================================================

TEST(grasp_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 100;
    cfg.alpha = 0.3;
    cfg.local_search_iterations = 80;
    cfg.local_search_neighbors = 20;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(double) * inst->dimensions,
                                 inst->dimensions,
                                 continuous_evaluate,
                                 grasp_construct_continuous,
                                 continuous_neighbor_gaussian,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_TRUE(result.best.cost < 50.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(grasp_rastrigin) {
    ContinuousInstance *inst = continuous_create_rastrigin(3);
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 150;
    cfg.alpha = 0.2;
    cfg.local_search_iterations = 60;
    cfg.seed = 99;

    OptResult result = grasp_run(&cfg,
                                 sizeof(double) * inst->dimensions,
                                 inst->dimensions,
                                 continuous_evaluate,
                                 grasp_construct_continuous,
                                 continuous_neighbor_gaussian,
                                 inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(grasp_zero_iterations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_swap,
                                 inst);

    ASSERT_EQ(result.num_iterations, 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(grasp_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 50;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_swap,
                                 inst);

    ASSERT_NOT_NULL(result.convergence);
    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(grasp_valid_tour) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    GRASPConfig cfg = grasp_default_config();
    cfg.max_iterations = 20;
    cfg.seed = 42;

    OptResult result = grasp_run(&cfg,
                                 sizeof(int) * inst->n_cities,
                                 inst->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_swap,
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
    printf("=== Testes: GRASP (Wave 6) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(grasp_default_config_values);

    printf("\n[GRASP TSP - Greedy/Random]\n");
    RUN_TEST(grasp_tsp5_greedy);
    RUN_TEST(grasp_tsp10_alpha03);
    RUN_TEST(grasp_tsp10_random);

    printf("\n[GRASP TSP - Reactive]\n");
    RUN_TEST(grasp_tsp10_reactive);

    printf("\n[GRASP Continuous]\n");
    RUN_TEST(grasp_sphere);
    RUN_TEST(grasp_rastrigin);

    printf("\n[Edge Cases]\n");
    RUN_TEST(grasp_zero_iterations);
    RUN_TEST(grasp_convergence_monotonic);
    RUN_TEST(grasp_valid_tour);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
