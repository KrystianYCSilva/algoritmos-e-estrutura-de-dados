/**
 * @file test_tabu_search.c
 * @brief Testes para Tabu Search e variantes (Wave 3)
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/tabu_search.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(ts_default_config_values) {
    TSConfig cfg = ts_default_config();
    ASSERT_EQ(cfg.max_iterations, (size_t)5000);
    ASSERT_EQ(cfg.neighbors_per_iter, (size_t)20);
    ASSERT_EQ(cfg.tabu_tenure, (size_t)15);
    ASSERT_TRUE(cfg.enable_aspiration);
    ASSERT_FALSE(cfg.enable_diversification);
    ASSERT_FALSE(cfg.enable_intensification);
    ASSERT_FALSE(cfg.enable_reactive_tenure);
    ASSERT_EQ((int)cfg.direction, (int)OPT_MINIMIZE);
}

// ============================================================================
// TESTES: HASH FUNCTIONS
// ============================================================================

TEST(ts_hash_int_array_deterministic) {
    int a[] = {0, 1, 2, 3, 4};
    int b[] = {0, 1, 2, 3, 4};
    int c[] = {4, 3, 2, 1, 0};

    uint64_t ha = ts_hash_int_array(a, 5);
    uint64_t hb = ts_hash_int_array(b, 5);
    uint64_t hc = ts_hash_int_array(c, 5);

    ASSERT_EQ(ha, hb);
    ASSERT_NE(ha, hc);
}

TEST(ts_hash_double_array_similar) {
    double a[] = {1.0, 2.0, 3.0};
    double b[] = {1.0, 2.0, 3.0};
    double c[] = {1.0, 2.0, 3.1};

    uint64_t ha = ts_hash_double_array(a, 3);
    uint64_t hb = ts_hash_double_array(b, 3);
    uint64_t hc = ts_hash_double_array(c, 3);

    ASSERT_EQ(ha, hb);
    ASSERT_NE(ha, hc);
}

TEST(ts_hash_bytes_basic) {
    int data[] = {42, 99};
    uint64_t h = ts_hash_bytes(data, sizeof(data));
    ASSERT_NE(h, (uint64_t)0);
}

// ============================================================================
// TESTES: CLASSIC TS - TSP
// ============================================================================

TEST(ts_classic_tsp_5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 7;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 5, 5,
                              tsp_tour_cost,
                              tsp_neighbor_swap,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(ts_classic_tsp_10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 2000;
    cfg.neighbors_per_iter = 20;
    cfg.tabu_tenure = 10;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_TRUE(result.best.cost <= inst->known_optimum * 2.0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: CLASSIC TS - CONTINUOUS
// ============================================================================

TEST(ts_classic_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 2000;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 10;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(double) * 3, 3,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              ts_hash_double_array,
                              inst);

    ASSERT_TRUE(result.best.cost < 10.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: ASPIRATION
// ============================================================================

TEST(ts_aspiration_enabled) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 10;
    cfg.tabu_tenure = 20;
    cfg.enable_aspiration = true;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 5, 5,
                              tsp_tour_cost,
                              tsp_neighbor_swap,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_TRUE(result.best.cost > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: DIVERSIFICATION
// ============================================================================

TEST(ts_diversification_tsp) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 1000;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 10;
    cfg.enable_diversification = true;
    cfg.diversification_weight = 0.5;
    cfg.diversification_trigger = 50;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: INTENSIFICATION
// ============================================================================

TEST(ts_intensification_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 1000;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 8;
    cfg.enable_intensification = true;
    cfg.intensification_trigger = 30;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              ts_hash_double_array,
                              inst);

    ASSERT_TRUE(result.best.cost < 10.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: REACTIVE TENURE
// ============================================================================

TEST(ts_reactive_tenure_tsp) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 1000;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 10;
    cfg.enable_reactive_tenure = true;
    cfg.reactive_increase = 3;
    cfg.reactive_decrease = 1;
    cfg.min_tenure = 3;
    cfg.max_tenure = 30;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_TRUE(result.best.cost > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: NULL HASH (should use default)
// ============================================================================

TEST(ts_null_hash_fn) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 200;
    cfg.neighbors_per_iter = 10;
    cfg.tabu_tenure = 5;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 100.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: CONVERGENCE
// ============================================================================

TEST(ts_convergence_recorded) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 10;
    cfg.tabu_tenure = 7;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              ts_hash_double_array,
                              inst);

    ASSERT_NOT_NULL(result.convergence);
    ASSERT_TRUE(result.num_iterations > 0);

    double last = result.convergence[result.num_iterations - 1];
    ASSERT_TRUE(last <= result.convergence[0] + 1e-9);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: COMBINED FEATURES
// ============================================================================

TEST(ts_all_features_tsp) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 15;
    cfg.tabu_tenure = 10;
    cfg.enable_aspiration = true;
    cfg.enable_diversification = true;
    cfg.diversification_weight = 0.3;
    cfg.diversification_trigger = 100;
    cfg.enable_intensification = true;
    cfg.intensification_trigger = 50;
    cfg.enable_reactive_tenure = true;
    cfg.reactive_increase = 3;
    cfg.reactive_decrease = 1;
    cfg.min_tenure = 5;
    cfg.max_tenure = 25;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              ts_hash_int_array,
                              inst);

    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(ts_zero_iterations) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    TSConfig cfg = ts_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = ts_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              ts_hash_double_array,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_EQ(result.num_iterations, (size_t)0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Tabu Search (Wave 3) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(ts_default_config_values);

    printf("\n[Hash Functions]\n");
    RUN_TEST(ts_hash_int_array_deterministic);
    RUN_TEST(ts_hash_double_array_similar);
    RUN_TEST(ts_hash_bytes_basic);

    printf("\n[Classic TS - TSP]\n");
    RUN_TEST(ts_classic_tsp_5);
    RUN_TEST(ts_classic_tsp_10);

    printf("\n[Classic TS - Continuous]\n");
    RUN_TEST(ts_classic_sphere);

    printf("\n[Aspiration]\n");
    RUN_TEST(ts_aspiration_enabled);

    printf("\n[Diversification]\n");
    RUN_TEST(ts_diversification_tsp);

    printf("\n[Intensification]\n");
    RUN_TEST(ts_intensification_sphere);

    printf("\n[Reactive Tenure]\n");
    RUN_TEST(ts_reactive_tenure_tsp);

    printf("\n[Null Hash / Default]\n");
    RUN_TEST(ts_null_hash_fn);

    printf("\n[Convergence]\n");
    RUN_TEST(ts_convergence_recorded);

    printf("\n[Combined Features]\n");
    RUN_TEST(ts_all_features_tsp);

    printf("\n[Edge Cases]\n");
    RUN_TEST(ts_zero_iterations);

    printf("\n=== Todos os %d testes passaram! ===\n", 15);
    return 0;
}
