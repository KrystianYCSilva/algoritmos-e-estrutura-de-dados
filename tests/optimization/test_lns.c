/**
 * @file test_lns.c
 * @brief Testes do Large Neighborhood Search / ALNS (Wave 12)
 */

#include "../test_macros.h"
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/lns.h"

// ============================================================================
// TESTES DE CONFIGURACAO
// ============================================================================

TEST(lns_default_config_values) {
    LNSConfig cfg = lns_default_config();
    ASSERT_EQ(cfg.max_iterations, (size_t)1000);
    ASSERT_NEAR(cfg.destroy_degree, 0.3, 1e-9);
    ASSERT_EQ(cfg.variant, LNS_BASIC);
    ASSERT_EQ(cfg.acceptance, LNS_ACCEPT_BETTER);
    ASSERT_NEAR(cfg.sa_initial_temp, 100.0, 1e-9);
    ASSERT_NEAR(cfg.sa_alpha, 0.99, 1e-9);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, (unsigned)42);
}

// ============================================================================
// LNS BASICO - TSP
// ============================================================================

TEST(lns_basic_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 200;
    cfg.destroy_degree = 0.3;
    cfg.acceptance = LNS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(lns_basic_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 500;
    cfg.destroy_degree = 0.3;
    cfg.acceptance = LNS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// LNS COM SA ACCEPTANCE
// ============================================================================

TEST(lns_sa_accept_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 300;
    cfg.destroy_degree = 0.3;
    cfg.acceptance = LNS_ACCEPT_SA_LIKE;
    cfg.sa_initial_temp = 50.0;
    cfg.sa_alpha = 0.995;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// WORST DESTROY + RANDOM REPAIR
// ============================================================================

TEST(lns_worst_destroy_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 300;
    cfg.destroy_degree = 0.3;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_worst, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(lns_random_repair_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 200;
    cfg.destroy_degree = 0.4;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_random, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// ALNS (ADAPTIVE)
// ============================================================================

TEST(alns_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 500;
    cfg.destroy_degree = 0.3;
    cfg.variant = LNS_ADAPTIVE;
    cfg.acceptance = LNS_ACCEPT_SA_LIKE;
    cfg.sa_initial_temp = 50.0;
    cfg.sa_alpha = 0.995;
    cfg.num_destroy_ops = 2;
    cfg.num_repair_ops = 2;
    cfg.weight_update_interval = 50;
    cfg.seed = 42;

    DestroyFn destroys[] = { lns_destroy_tsp_random, lns_destroy_tsp_worst };
    RepairFn repairs[] = { lns_repair_tsp_greedy, lns_repair_tsp_random };

    OptResult res = alns_run(&cfg, sizeof(int), inst->n_cities,
                             tsp_tour_cost, tsp_generate_random,
                             destroys, repairs, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(lns_zero_iterations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_EQ(res.num_iterations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(lns_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 100;
    cfg.destroy_degree = 0.3;
    cfg.acceptance = LNS_ACCEPT_BETTER;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    for (size_t i = 1; i < res.num_iterations; i++) {
        ASSERT(res.convergence[i] <= res.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(lns_valid_tour) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    LNSConfig cfg = lns_default_config();
    cfg.max_iterations = 100;
    cfg.seed = 42;

    OptResult res = lns_run(&cfg, sizeof(int), inst->n_cities,
                            tsp_tour_cost, tsp_generate_random,
                            lns_destroy_tsp_random, lns_repair_tsp_greedy, inst);
    ASSERT_NOT_NULL(res.best.data);

    int *tour = (int *)res.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, inst->n_cities));

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Large Neighborhood Search (Wave 12) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(lns_default_config_values);

    printf("\n[LNS Basico - TSP]\n");
    RUN_TEST(lns_basic_tsp5);
    RUN_TEST(lns_basic_tsp10);

    printf("\n[LNS SA Acceptance]\n");
    RUN_TEST(lns_sa_accept_tsp10);

    printf("\n[Worst Destroy / Random Repair]\n");
    RUN_TEST(lns_worst_destroy_tsp10);
    RUN_TEST(lns_random_repair_tsp5);

    printf("\n[ALNS (Adaptive)]\n");
    RUN_TEST(alns_tsp10);

    printf("\n[Edge Cases]\n");
    RUN_TEST(lns_zero_iterations);
    RUN_TEST(lns_convergence_monotonic);
    RUN_TEST(lns_valid_tour);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
