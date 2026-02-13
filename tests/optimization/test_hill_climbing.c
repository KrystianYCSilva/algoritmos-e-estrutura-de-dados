/**
 * @file test_hill_climbing.c
 * @brief Testes para Hill Climbing e variantes (Wave 1)
 *
 * Valida steepest, first improvement, random restart e stochastic
 * em problemas TSP e funcoes continuas (Sphere, Rastrigin).
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/heuristics/hill_climbing.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(hc_default_config_values) {
    HCConfig cfg = hc_default_config();
    ASSERT_EQ((int)cfg.variant, (int)HC_STEEPEST);
    ASSERT_EQ(cfg.max_iterations, (size_t)1000);
    ASSERT_EQ(cfg.neighbors_per_iter, (size_t)20);
    ASSERT_EQ(cfg.num_restarts, (size_t)10);
    ASSERT_NEAR(cfg.stochastic_temperature, 1.0, 1e-9);
    ASSERT_EQ((int)cfg.direction, (int)OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42u);
}

// ============================================================================
// TESTES: STEEPEST DESCENT - TSP
// ============================================================================

TEST(hc_steepest_tsp_5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_STEEPEST;
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 10;
    cfg.seed = 42;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(int) * 5, 5,
                                   tsp_tour_cost,
                                   tsp_neighbor_swap,
                                   tsp_generate_random,
                                   inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost < 1e18);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_iterations > 0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(hc_steepest_tsp_improves) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.max_iterations = 200;
    cfg.neighbors_per_iter = 30;
    cfg.seed = 123;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(int) * 10, 10,
                                   tsp_tour_cost,
                                   tsp_neighbor_2opt,
                                   tsp_generate_random,
                                   inst);

    ASSERT_NOT_NULL(result.convergence);
    if (result.num_iterations > 1) {
        ASSERT_TRUE(result.convergence[result.num_iterations - 1] <=
                    result.convergence[0] + 1e-9);
    }

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: STEEPEST - CONTINUOUS
// ============================================================================

TEST(hc_steepest_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 20;
    cfg.seed = 42;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(double) * 3, 3,
                                   continuous_evaluate,
                                   continuous_neighbor_gaussian,
                                   continuous_generate_random,
                                   inst);

    ASSERT_TRUE(result.best.cost < 10.0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: FIRST IMPROVEMENT
// ============================================================================

TEST(hc_first_improvement_tsp) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_FIRST_IMPROVEMENT;
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 20;
    cfg.seed = 42;

    OptResult result = hc_first_improvement(&cfg,
                                            sizeof(int) * 5, 5,
                                            tsp_tour_cost,
                                            tsp_neighbor_swap,
                                            tsp_generate_random,
                                            inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(hc_first_improvement_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_FIRST_IMPROVEMENT;
    cfg.max_iterations = 1000;
    cfg.neighbors_per_iter = 10;
    cfg.seed = 77;

    OptResult result = hc_first_improvement(&cfg,
                                            sizeof(double) * 2, 2,
                                            continuous_evaluate,
                                            continuous_neighbor_gaussian,
                                            continuous_generate_random,
                                            inst);

    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: RANDOM RESTART
// ============================================================================

TEST(hc_random_restart_tsp) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_RANDOM_RESTART;
    cfg.max_iterations = 100;
    cfg.neighbors_per_iter = 10;
    cfg.num_restarts = 5;
    cfg.seed = 42;

    OptResult result = hc_random_restart(&cfg,
                                         sizeof(int) * 5, 5,
                                         tsp_tour_cost,
                                         tsp_neighbor_swap,
                                         tsp_generate_random,
                                         inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(hc_random_restart_better_than_single) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg_single = hc_default_config();
    cfg_single.max_iterations = 100;
    cfg_single.neighbors_per_iter = 10;
    cfg_single.seed = 42;

    OptResult single = hc_steepest(&cfg_single,
                                   sizeof(int) * 10, 10,
                                   tsp_tour_cost,
                                   tsp_neighbor_2opt,
                                   tsp_generate_random,
                                   inst);

    HCConfig cfg_restart = hc_default_config();
    cfg_restart.variant = HC_RANDOM_RESTART;
    cfg_restart.max_iterations = 100;
    cfg_restart.neighbors_per_iter = 10;
    cfg_restart.num_restarts = 10;
    cfg_restart.seed = 42;

    OptResult restart = hc_random_restart(&cfg_restart,
                                          sizeof(int) * 10, 10,
                                          tsp_tour_cost,
                                          tsp_neighbor_2opt,
                                          tsp_generate_random,
                                          inst);

    ASSERT_TRUE(restart.best.cost <= single.best.cost + 1e-9);

    opt_result_destroy(&single);
    opt_result_destroy(&restart);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: STOCHASTIC
// ============================================================================

TEST(hc_stochastic_tsp) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_STOCHASTIC;
    cfg.max_iterations = 500;
    cfg.stochastic_temperature = 5.0;
    cfg.seed = 42;

    OptResult result = hc_stochastic(&cfg,
                                     sizeof(int) * 5, 5,
                                     tsp_tour_cost,
                                     tsp_neighbor_swap,
                                     tsp_generate_random,
                                     inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_EQ(result.num_iterations, (size_t)500);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(hc_stochastic_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_STOCHASTIC;
    cfg.max_iterations = 2000;
    cfg.stochastic_temperature = 0.5;
    cfg.seed = 42;

    OptResult result = hc_stochastic(&cfg,
                                     sizeof(double) * 3, 3,
                                     continuous_evaluate,
                                     continuous_neighbor_gaussian,
                                     continuous_generate_random,
                                     inst);

    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: DISPATCHER hc_run
// ============================================================================

TEST(hc_run_dispatches_steepest) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_STEEPEST;
    cfg.max_iterations = 100;
    cfg.seed = 42;

    OptResult result = hc_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 100.0);
    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(hc_run_dispatches_first_improvement) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.variant = HC_FIRST_IMPROVEMENT;
    cfg.max_iterations = 100;
    cfg.seed = 42;

    OptResult result = hc_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 100.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(hc_steepest_zero_iterations) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(double) * 2, 2,
                                   continuous_evaluate,
                                   continuous_neighbor_gaussian,
                                   continuous_generate_random,
                                   inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_EQ(result.num_iterations, (size_t)0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(hc_convergence_monotonic) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.max_iterations = 200;
    cfg.neighbors_per_iter = 15;
    cfg.seed = 42;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(double) * 3, 3,
                                   continuous_evaluate,
                                   continuous_neighbor_gaussian,
                                   continuous_generate_random,
                                   inst);

    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(hc_rastrigin_finds_local_optimum) {
    ContinuousInstance *inst = continuous_create_rastrigin(2);
    ASSERT_NOT_NULL(inst);

    HCConfig cfg = hc_default_config();
    cfg.max_iterations = 500;
    cfg.neighbors_per_iter = 20;
    cfg.seed = 42;

    OptResult result = hc_steepest(&cfg,
                                   sizeof(double) * 2, 2,
                                   continuous_evaluate,
                                   continuous_neighbor_gaussian,
                                   continuous_generate_random,
                                   inst);

    ASSERT_TRUE(result.best.cost < 50.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Hill Climbing (Wave 1) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(hc_default_config_values);

    printf("\n[Steepest - TSP]\n");
    RUN_TEST(hc_steepest_tsp_5);
    RUN_TEST(hc_steepest_tsp_improves);

    printf("\n[Steepest - Continuous]\n");
    RUN_TEST(hc_steepest_sphere);

    printf("\n[First Improvement]\n");
    RUN_TEST(hc_first_improvement_tsp);
    RUN_TEST(hc_first_improvement_sphere);

    printf("\n[Random Restart]\n");
    RUN_TEST(hc_random_restart_tsp);
    RUN_TEST(hc_random_restart_better_than_single);

    printf("\n[Stochastic]\n");
    RUN_TEST(hc_stochastic_tsp);
    RUN_TEST(hc_stochastic_sphere);

    printf("\n[Dispatcher hc_run]\n");
    RUN_TEST(hc_run_dispatches_steepest);
    RUN_TEST(hc_run_dispatches_first_improvement);

    printf("\n[Edge Cases]\n");
    RUN_TEST(hc_steepest_zero_iterations);
    RUN_TEST(hc_convergence_monotonic);
    RUN_TEST(hc_rastrigin_finds_local_optimum);

    printf("\n=== Todos os %d testes passaram! ===\n", 16);
    return 0;
}
