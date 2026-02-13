/**
 * @file test_simulated_annealing.c
 * @brief Testes para Simulated Annealing e variantes (Wave 2)
 *
 * Valida SA classico com todos os cooling schedules, reheating,
 * auto-calibracao T0, em problemas TSP e funcoes continuas.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/simulated_annealing.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(sa_default_config_values) {
    SAConfig cfg = sa_default_config();
    ASSERT_NEAR(cfg.initial_temp, 100.0, 1e-9);
    ASSERT_NEAR(cfg.final_temp, 0.001, 1e-9);
    ASSERT_NEAR(cfg.alpha, 0.95, 1e-9);
    ASSERT_EQ((int)cfg.cooling, (int)SA_COOLING_GEOMETRIC);
    ASSERT_EQ(cfg.max_iterations, (size_t)10000);
    ASSERT_EQ(cfg.markov_chain_length, (size_t)50);
    ASSERT_FALSE(cfg.enable_reheating);
    ASSERT_FALSE(cfg.auto_calibrate_t0);
    ASSERT_EQ((int)cfg.direction, (int)OPT_MINIMIZE);
}

// ============================================================================
// TESTES: GEOMETRIC COOLING - TSP
// ============================================================================

TEST(sa_geometric_tsp_5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 100.0;
    cfg.final_temp = 0.01;
    cfg.alpha = 0.95;
    cfg.max_iterations = 5000;
    cfg.markov_chain_length = 20;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(int) * 5, 5,
                              tsp_tour_cost,
                              tsp_neighbor_swap,
                              tsp_generate_random,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.best.cost < 100.0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(sa_geometric_tsp_10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 200.0;
    cfg.final_temp = 0.001;
    cfg.alpha = 0.97;
    cfg.max_iterations = 10000;
    cfg.markov_chain_length = 30;
    cfg.seed = 123;

    OptResult result = sa_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost <= inst->known_optimum * 2.0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: GEOMETRIC COOLING - CONTINUOUS
// ============================================================================

TEST(sa_geometric_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 50.0;
    cfg.final_temp = 0.001;
    cfg.alpha = 0.95;
    cfg.max_iterations = 5000;
    cfg.markov_chain_length = 10;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 3, 3,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(sa_geometric_rastrigin) {
    ContinuousInstance *inst = continuous_create_rastrigin(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 100.0;
    cfg.final_temp = 0.001;
    cfg.alpha = 0.97;
    cfg.max_iterations = 10000;
    cfg.markov_chain_length = 20;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 20.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: LINEAR COOLING
// ============================================================================

TEST(sa_linear_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.cooling = SA_COOLING_LINEAR;
    cfg.initial_temp = 50.0;
    cfg.final_temp = 0.01;
    cfg.max_iterations = 5000;
    cfg.markov_chain_length = 10;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
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
// TESTES: LOGARITHMIC COOLING
// ============================================================================

TEST(sa_logarithmic_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.cooling = SA_COOLING_LOGARITHMIC;
    cfg.initial_temp = 50.0;
    cfg.final_temp = 0.01;
    cfg.max_iterations = 3000;
    cfg.markov_chain_length = 10;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 15.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: ADAPTIVE COOLING
// ============================================================================

TEST(sa_adaptive_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.cooling = SA_COOLING_ADAPTIVE;
    cfg.initial_temp = 50.0;
    cfg.final_temp = 0.001;
    cfg.max_iterations = 5000;
    cfg.markov_chain_length = 20;
    cfg.adaptive_target_low = 0.2;
    cfg.adaptive_target_high = 0.5;
    cfg.adaptive_factor = 1.05;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 3, 3,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 10.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: REHEATING
// ============================================================================

TEST(sa_reheating_tsp) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 100.0;
    cfg.final_temp = 0.01;
    cfg.alpha = 0.95;
    cfg.max_iterations = 5000;
    cfg.markov_chain_length = 20;
    cfg.enable_reheating = true;
    cfg.reheat_threshold = 0.05;
    cfg.reheat_factor = 2.0;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.num_evaluations > 0);

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: AUTO-CALIBRATE T0
// ============================================================================

TEST(sa_calibrate_t0_returns_positive) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.calibration_samples = 50;
    cfg.target_acceptance = 0.8;
    cfg.seed = 42;

    double t0 = sa_calibrate_t0(&cfg,
                                sizeof(double) * 3, 3,
                                continuous_evaluate,
                                continuous_neighbor_gaussian,
                                continuous_generate_random,
                                inst);

    ASSERT_TRUE(t0 > 0);

    continuous_instance_destroy(inst);
}

TEST(sa_auto_calibrate_run) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.auto_calibrate_t0 = true;
    cfg.calibration_samples = 50;
    cfg.target_acceptance = 0.8;
    cfg.final_temp = 0.001;
    cfg.alpha = 0.95;
    cfg.max_iterations = 3000;
    cfg.markov_chain_length = 10;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.best.cost < 10.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: SA vs HC
// ============================================================================

TEST(sa_beats_hc_on_rastrigin) {
    ContinuousInstance *inst = continuous_create_rastrigin(2);
    ASSERT_NOT_NULL(inst);

    SAConfig sa_cfg = sa_default_config();
    sa_cfg.initial_temp = 100.0;
    sa_cfg.final_temp = 0.001;
    sa_cfg.alpha = 0.97;
    sa_cfg.max_iterations = 10000;
    sa_cfg.markov_chain_length = 20;
    sa_cfg.seed = 42;

    OptResult sa_result = sa_run(&sa_cfg,
                                 sizeof(double) * 2, 2,
                                 continuous_evaluate,
                                 continuous_neighbor_gaussian,
                                 continuous_generate_random,
                                 inst);

    ASSERT_TRUE(sa_result.best.cost < 20.0);
    ASSERT_TRUE(sa_result.num_evaluations > 100);

    opt_result_destroy(&sa_result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: CONVERGENCE
// ============================================================================

TEST(sa_convergence_recorded) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.max_iterations = 1000;
    cfg.markov_chain_length = 5;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_NOT_NULL(result.convergence);
    ASSERT_TRUE(result.num_iterations > 0);

    double last = result.convergence[result.num_iterations - 1];
    ASSERT_TRUE(last <= result.convergence[0] + 1e-9);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(sa_zero_iterations) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.max_iterations = 0;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
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

TEST(sa_very_low_temp) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    SAConfig cfg = sa_default_config();
    cfg.initial_temp = 0.0001;
    cfg.final_temp = 0.00001;
    cfg.max_iterations = 500;
    cfg.markov_chain_length = 5;
    cfg.seed = 42;

    OptResult result = sa_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              inst);

    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Simulated Annealing (Wave 2) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(sa_default_config_values);

    printf("\n[Geometric - TSP]\n");
    RUN_TEST(sa_geometric_tsp_5);
    RUN_TEST(sa_geometric_tsp_10);

    printf("\n[Geometric - Continuous]\n");
    RUN_TEST(sa_geometric_sphere);
    RUN_TEST(sa_geometric_rastrigin);

    printf("\n[Linear Cooling]\n");
    RUN_TEST(sa_linear_sphere);

    printf("\n[Logarithmic Cooling]\n");
    RUN_TEST(sa_logarithmic_sphere);

    printf("\n[Adaptive Cooling]\n");
    RUN_TEST(sa_adaptive_sphere);

    printf("\n[Reheating]\n");
    RUN_TEST(sa_reheating_tsp);

    printf("\n[Auto-Calibrate T0]\n");
    RUN_TEST(sa_calibrate_t0_returns_positive);
    RUN_TEST(sa_auto_calibrate_run);

    printf("\n[SA vs HC]\n");
    RUN_TEST(sa_beats_hc_on_rastrigin);

    printf("\n[Convergence]\n");
    RUN_TEST(sa_convergence_recorded);

    printf("\n[Edge Cases]\n");
    RUN_TEST(sa_zero_iterations);
    RUN_TEST(sa_very_low_temp);

    printf("\n=== Todos os %d testes passaram! ===\n", 15);
    return 0;
}
