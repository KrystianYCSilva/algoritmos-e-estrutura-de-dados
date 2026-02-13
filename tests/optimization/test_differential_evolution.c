/**
 * @file test_differential_evolution.c
 * @brief Testes do Differential Evolution (Wave 9)
 */

#include "../test_macros.h"
#include "optimization/common.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/differential_evolution.h"
#include <math.h>

// ============================================================================
// TESTES DE CONFIGURACAO
// ============================================================================

TEST(de_default_config_values) {
    DEConfig cfg = de_default_config();
    ASSERT_EQ(cfg.population_size, 50);
    ASSERT_EQ(cfg.max_generations, 1000);
    ASSERT_NEAR(cfg.F, 0.8, 1e-9);
    ASSERT_NEAR(cfg.CR, 0.9, 1e-9);
    ASSERT_EQ(cfg.strategy, DE_RAND_1);
    ASSERT_NEAR(cfg.lower_bound, -5.12, 1e-9);
    ASSERT_NEAR(cfg.upper_bound, 5.12, 1e-9);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42);
}

// ============================================================================
// DE/RAND/1 - SPHERE
// ============================================================================

TEST(de_rand1_sphere5) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 500;
    cfg.strategy = DE_RAND_1;
    cfg.F = 0.8;
    cfg.CR = 0.9;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 1.0);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

TEST(de_rand1_sphere10) {
    ContinuousInstance *inst = continuous_create_sphere(10);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 50;
    cfg.max_generations = 1000;
    cfg.strategy = DE_RAND_1;
    cfg.seed = 123;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 1.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// DE/BEST/1 - RASTRIGIN
// ============================================================================

TEST(de_best1_rastrigin5) {
    ContinuousInstance *inst = continuous_create_rastrigin(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 60;
    cfg.max_generations = 800;
    cfg.strategy = DE_BEST_1;
    cfg.F = 0.6;
    cfg.CR = 0.9;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 20.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// DE/CURRENT-TO-BEST/1
// ============================================================================

TEST(de_current_to_best_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 40;
    cfg.max_generations = 500;
    cfg.strategy = DE_CURRENT_TO_BEST_1;
    cfg.F = 0.7;
    cfg.CR = 0.8;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 1.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// DE/RAND/2
// ============================================================================

TEST(de_rand2_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 40;
    cfg.max_generations = 500;
    cfg.strategy = DE_RAND_2;
    cfg.F = 0.5;
    cfg.CR = 0.9;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 5.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// DE/BEST/2
// ============================================================================

TEST(de_best2_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 40;
    cfg.max_generations = 500;
    cfg.strategy = DE_BEST_2;
    cfg.F = 0.7;
    cfg.CR = 0.9;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_LT(res.best.cost, 1.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(de_zero_generations) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.max_generations = 0;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_EQ(res.num_iterations, (size_t)0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

TEST(de_convergence_monotonic) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 100;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    for (size_t i = 1; i < res.num_iterations; i++) {
        ASSERT(res.convergence[i] <= res.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

TEST(de_small_population) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    DEConfig cfg = de_default_config();
    cfg.population_size = 4;
    cfg.max_generations = 50;
    cfg.seed = 42;

    OptResult res = de_run(&cfg, inst->dimensions, continuous_evaluate, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Differential Evolution (Wave 9) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(de_default_config_values);

    printf("\n[DE/rand/1 - Sphere]\n");
    RUN_TEST(de_rand1_sphere5);
    RUN_TEST(de_rand1_sphere10);

    printf("\n[DE/best/1 - Rastrigin]\n");
    RUN_TEST(de_best1_rastrigin5);

    printf("\n[DE/current-to-best/1]\n");
    RUN_TEST(de_current_to_best_sphere);

    printf("\n[DE/rand/2]\n");
    RUN_TEST(de_rand2_sphere);

    printf("\n[DE/best/2]\n");
    RUN_TEST(de_best2_sphere);

    printf("\n[Edge Cases]\n");
    RUN_TEST(de_zero_generations);
    RUN_TEST(de_convergence_monotonic);
    RUN_TEST(de_small_population);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
