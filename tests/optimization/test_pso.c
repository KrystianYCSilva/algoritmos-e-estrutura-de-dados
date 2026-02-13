/**
 * @file test_pso.c
 * @brief Testes do Particle Swarm Optimization (PSO) - Wave 7
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/pso.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(pso_default_config_values) {
    PSOConfig cfg = pso_default_config();
    ASSERT_EQ(cfg.num_particles, 30);
    ASSERT_EQ(cfg.max_iterations, 500);
    ASSERT_NEAR(cfg.w, 0.729, 1e-3);
    ASSERT_NEAR(cfg.w_min, 0.4, 1e-9);
    ASSERT_NEAR(cfg.c1, 1.49445, 1e-4);
    ASSERT_NEAR(cfg.c2, 1.49445, 1e-4);
    ASSERT_NEAR(cfg.v_max_ratio, 0.1, 1e-9);
    ASSERT_EQ(cfg.inertia_type, PSO_INERTIA_LINEAR_DECREASING);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, 42);
}

// ============================================================================
// TESTES: PSO SPHERE
// ============================================================================

TEST(pso_sphere_5d) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.num_particles = 30;
    cfg.max_iterations = 200;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_TRUE(result.best.cost < 1.0);
    ASSERT_EQ(result.num_iterations, 200);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(pso_sphere_10d) {
    ContinuousInstance *inst = continuous_create_sphere(10);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.num_particles = 40;
    cfg.max_iterations = 500;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: PSO RASTRIGIN
// ============================================================================

TEST(pso_rastrigin_3d) {
    ContinuousInstance *inst = continuous_create_rastrigin(3);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.num_particles = 50;
    cfg.max_iterations = 300;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_TRUE(result.best.cost < 30.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: PSO ACKLEY
// ============================================================================

TEST(pso_ackley_5d) {
    ContinuousInstance *inst = continuous_create_ackley(5);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.num_particles = 30;
    cfg.max_iterations = 300;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: INERTIA VARIANTS
// ============================================================================

TEST(pso_constant_inertia) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.inertia_type = PSO_INERTIA_CONSTANT;
    cfg.w = 0.7;
    cfg.num_particles = 20;
    cfg.max_iterations = 200;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(pso_constriction_factor) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.inertia_type = PSO_INERTIA_CONSTRICTION;
    cfg.c1 = 2.05;
    cfg.c2 = 2.05;
    cfg.num_particles = 30;
    cfg.max_iterations = 200;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost < 5.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(pso_zero_iterations) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.max_iterations = 0;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost >= 0.0);
    ASSERT_EQ(result.num_iterations, 0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(pso_convergence_monotonic) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.max_iterations = 100;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.convergence);
    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(pso_single_particle) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    PSOConfig cfg = pso_default_config();
    cfg.num_particles = 1;
    cfg.max_iterations = 50;
    cfg.lower_bound = inst->lower_bound;
    cfg.upper_bound = inst->upper_bound;
    cfg.seed = 42;

    OptResult result = pso_run(&cfg, inst->dimensions,
                               continuous_evaluate, inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_EQ(result.num_iterations, 50);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Particle Swarm Optimization (Wave 7) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(pso_default_config_values);

    printf("\n[PSO Sphere]\n");
    RUN_TEST(pso_sphere_5d);
    RUN_TEST(pso_sphere_10d);

    printf("\n[PSO Rastrigin]\n");
    RUN_TEST(pso_rastrigin_3d);

    printf("\n[PSO Ackley]\n");
    RUN_TEST(pso_ackley_5d);

    printf("\n[Inertia Variants]\n");
    RUN_TEST(pso_constant_inertia);
    RUN_TEST(pso_constriction_factor);

    printf("\n[Edge Cases]\n");
    RUN_TEST(pso_zero_iterations);
    RUN_TEST(pso_convergence_monotonic);
    RUN_TEST(pso_single_particle);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
