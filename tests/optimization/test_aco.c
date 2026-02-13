/**
 * @file test_aco.c
 * @brief Testes do Ant Colony Optimization (ACO) - Wave 8
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/aco.h"
#include "optimization/benchmarks/tsp.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(aco_default_config_values) {
    ACOConfig cfg = aco_default_config();
    ASSERT_EQ(cfg.n_ants, 20);
    ASSERT_EQ(cfg.max_iterations, 500);
    ASSERT_NEAR(cfg.alpha, 1.0, 1e-9);
    ASSERT_NEAR(cfg.beta, 3.0, 1e-9);
    ASSERT_NEAR(cfg.rho, 0.1, 1e-9);
    ASSERT_NEAR(cfg.q, 1.0, 1e-9);
    ASSERT_NEAR(cfg.tau_0, 0.1, 1e-9);
    ASSERT_EQ(cfg.variant, ACO_ANT_SYSTEM);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42);
}

// ============================================================================
// TESTES: ACO ANT SYSTEM - TSP
// ============================================================================

TEST(aco_as_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 10;
    cfg.max_iterations = 50;
    cfg.variant = ACO_ANT_SYSTEM;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.best.cost < 200.0);
    ASSERT_EQ(result.num_iterations, 50);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(aco_as_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 15;
    cfg.max_iterations = 100;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ACO ELITIST - TSP
// ============================================================================

TEST(aco_elitist_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 15;
    cfg.max_iterations = 80;
    cfg.variant = ACO_ELITIST;
    cfg.elitist_weight = 3.0;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: ACO MAX-MIN - TSP
// ============================================================================

TEST(aco_mmas_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 15;
    cfg.max_iterations = 100;
    cfg.variant = ACO_MAX_MIN;
    cfg.tau_min = 0.001;
    cfg.tau_max = 5.0;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(aco_mmas_tsp20) {
    TSPInstance *inst = tsp_create_example_20();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 20;
    cfg.max_iterations = 100;
    cfg.variant = ACO_MAX_MIN;
    cfg.tau_min = 0.001;
    cfg.tau_max = 5.0;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(aco_zero_iterations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_EQ(result.num_iterations, 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(aco_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 10;
    cfg.max_iterations = 50;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.convergence);
    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(aco_valid_tour_cost) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 5;
    cfg.max_iterations = 30;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    double verify_cost = tsp_tour_cost(tour, inst->n_cities, inst);
    ASSERT_NEAR(result.best.cost, verify_cost, 1e-6);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(aco_single_ant) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    ACOConfig cfg = aco_default_config();
    cfg.n_ants = 1;
    cfg.max_iterations = 20;
    cfg.seed = 42;

    OptResult result = aco_run(&cfg, inst->n_cities,
                               tsp_tour_cost, aco_heuristic_tsp, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_EQ(result.num_iterations, 20);
    ASSERT_EQ(result.num_evaluations, 20);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Ant Colony Optimization (Wave 8) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(aco_default_config_values);

    printf("\n[ACO Ant System - TSP]\n");
    RUN_TEST(aco_as_tsp5);
    RUN_TEST(aco_as_tsp10);

    printf("\n[ACO Elitist - TSP]\n");
    RUN_TEST(aco_elitist_tsp10);

    printf("\n[ACO MAX-MIN - TSP]\n");
    RUN_TEST(aco_mmas_tsp10);
    RUN_TEST(aco_mmas_tsp20);

    printf("\n[Edge Cases]\n");
    RUN_TEST(aco_zero_iterations);
    RUN_TEST(aco_convergence_monotonic);
    RUN_TEST(aco_valid_tour_cost);
    RUN_TEST(aco_single_ant);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
