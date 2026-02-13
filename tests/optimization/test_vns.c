/**
 * @file test_vns.c
 * @brief Testes do Variable Neighborhood Search (Wave 10)
 */

#include "../test_macros.h"
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/vns.h"

// ============================================================================
// TESTES DE CONFIGURACAO
// ============================================================================

TEST(vns_default_config_values) {
    VNSConfig cfg = vns_default_config();
    ASSERT_EQ(cfg.max_iterations, (size_t)1000);
    ASSERT_EQ(cfg.k_max, 5);
    ASSERT_EQ(cfg.local_search_iterations, (size_t)200);
    ASSERT_EQ(cfg.local_search_neighbors, (size_t)20);
    ASSERT_EQ(cfg.variant, VNS_BASIC);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, (unsigned)42);
}

// ============================================================================
// BASIC VNS - TSP
// ============================================================================

TEST(vns_basic_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 50;
    cfg.k_max = 4;
    cfg.local_search_iterations = 100;
    cfg.local_search_neighbors = 10;
    cfg.variant = VNS_BASIC;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_swap, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(vns_basic_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 30;
    cfg.k_max = 5;
    cfg.local_search_iterations = 100;
    cfg.local_search_neighbors = 15;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_2opt, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// BASIC VNS - CONTINUOUS
// ============================================================================

TEST(vns_basic_sphere5) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 50;
    cfg.k_max = 5;
    cfg.local_search_iterations = 100;
    cfg.local_search_neighbors = 20;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(double), inst->dimensions,
                            continuous_evaluate, vns_shake_continuous,
                            continuous_neighbor_gaussian, continuous_generate_random, inst);
    ASSERT_LT(res.best.cost, 5.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// REDUCED VNS
// ============================================================================

TEST(vns_reduced_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 100;
    cfg.k_max = 5;
    cfg.variant = VNS_REDUCED;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            NULL, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// GENERAL VNS (VND)
// ============================================================================

TEST(vns_general_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 20;
    cfg.k_max = 4;
    cfg.local_search_iterations = 50;
    cfg.local_search_neighbors = 10;
    cfg.variant = VNS_GENERAL;
    cfg.vnd_num_neighborhoods = 3;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_2opt, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(vns_zero_iterations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_swap, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_EQ(res.num_iterations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(vns_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 30;
    cfg.k_max = 3;
    cfg.local_search_iterations = 50;
    cfg.local_search_neighbors = 10;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_swap, tsp_generate_random, inst);
    for (size_t i = 1; i < res.num_iterations; i++) {
        ASSERT(res.convergence[i] <= res.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(vns_single_k) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    VNSConfig cfg = vns_default_config();
    cfg.max_iterations = 20;
    cfg.k_max = 1;
    cfg.local_search_iterations = 50;
    cfg.local_search_neighbors = 10;
    cfg.seed = 42;

    OptResult res = vns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, vns_shake_tsp,
                            tsp_neighbor_swap, tsp_generate_random, inst);
    ASSERT_NOT_NULL(res.best.data);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Variable Neighborhood Search (Wave 10) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(vns_default_config_values);

    printf("\n[Basic VNS - TSP]\n");
    RUN_TEST(vns_basic_tsp5);
    RUN_TEST(vns_basic_tsp10);

    printf("\n[Basic VNS - Continuous]\n");
    RUN_TEST(vns_basic_sphere5);

    printf("\n[Reduced VNS]\n");
    RUN_TEST(vns_reduced_tsp10);

    printf("\n[General VNS (VND)]\n");
    RUN_TEST(vns_general_tsp10);

    printf("\n[Edge Cases]\n");
    RUN_TEST(vns_zero_iterations);
    RUN_TEST(vns_convergence_monotonic);
    RUN_TEST(vns_single_k);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
