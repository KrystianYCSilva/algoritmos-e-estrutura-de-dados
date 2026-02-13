/**
 * @file test_genetic_algorithm.c
 * @brief Testes para Algoritmo Genetico e variantes (Wave 4)
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "../test_macros.h"
#include "optimization/metaheuristics/genetic_algorithm.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <math.h>
#include <float.h>

// ============================================================================
// TESTES: CONFIGURACAO
// ============================================================================

TEST(ga_default_config_values) {
    GAConfig cfg = ga_default_config();
    ASSERT_EQ(cfg.population_size, (size_t)50);
    ASSERT_EQ(cfg.max_generations, (size_t)500);
    ASSERT_NEAR(cfg.crossover_rate, 0.8, 1e-9);
    ASSERT_NEAR(cfg.mutation_rate, 0.05, 1e-9);
    ASSERT_EQ(cfg.elitism_count, (size_t)2);
    ASSERT_EQ((int)cfg.selection, (int)GA_SELECT_TOURNAMENT);
    ASSERT_EQ(cfg.tournament_size, (size_t)3);
    ASSERT_FALSE(cfg.enable_local_search);
    ASSERT_FALSE(cfg.enable_adaptive_rates);
    ASSERT_EQ((int)cfg.direction, (int)OPT_MINIMIZE);
}

// ============================================================================
// TESTES: GA - TSP com OX
// ============================================================================

TEST(ga_ox_tsp_5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 100;
    cfg.crossover_rate = 0.8;
    cfg.mutation_rate = 0.1;
    cfg.elitism_count = 2;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(int) * 5, 5,
                              tsp_tour_cost,
                              tsp_generate_random,
                              ga_crossover_ox,
                              ga_mutation_swap,
                              NULL,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_TRUE(result.best.cost > 0);
    ASSERT_TRUE(result.best.cost < 200.0);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, 5));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

TEST(ga_ox_tsp_10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 50;
    cfg.max_generations = 200;
    cfg.crossover_rate = 0.85;
    cfg.mutation_rate = 0.1;
    cfg.elitism_count = 2;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(int) * 10, 10,
                              tsp_tour_cost,
                              tsp_generate_random,
                              ga_crossover_ox,
                              ga_mutation_swap,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost <= inst->known_optimum * 3.0);

    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, 10));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: GA - TSP com PMX
// ============================================================================

TEST(ga_pmx_tsp_5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 100;
    cfg.mutation_rate = 0.1;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(int) * 5, 5,
                              tsp_tour_cost,
                              tsp_generate_random,
                              ga_crossover_pmx,
                              ga_mutation_inversion,
                              NULL,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    int *tour = (int*)result.best.data;
    ASSERT_TRUE(tsp_is_valid_tour(tour, 5));

    opt_result_destroy(&result);
    tsp_instance_destroy(inst);
}

// ============================================================================
// TESTES: GA - CONTINUOUS com BLX
// ============================================================================

TEST(ga_blx_sphere) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 40;
    cfg.max_generations = 200;
    cfg.crossover_rate = 0.8;
    cfg.mutation_rate = 0.1;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 3, 3,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 10.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(ga_blx_rastrigin) {
    ContinuousInstance *inst = continuous_create_rastrigin(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 60;
    cfg.max_generations = 300;
    cfg.crossover_rate = 0.8;
    cfg.mutation_rate = 0.15;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 30.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: SELECTION TYPES
// ============================================================================

TEST(ga_roulette_selection) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.selection = GA_SELECT_ROULETTE;
    cfg.population_size = 30;
    cfg.max_generations = 100;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 50.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(ga_rank_selection) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.selection = GA_SELECT_RANK;
    cfg.population_size = 30;
    cfg.max_generations = 100;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 50.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: ADAPTIVE RATES
// ============================================================================

TEST(ga_adaptive_rates) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 40;
    cfg.max_generations = 200;
    cfg.enable_adaptive_rates = true;
    cfg.adaptive_min_mutation = 0.01;
    cfg.adaptive_max_mutation = 0.3;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 3, 3,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.best.cost < 15.0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: CONVERGENCE
// ============================================================================

TEST(ga_convergence_monotonic) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 50;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    for (size_t i = 1; i < result.num_iterations; i++) {
        ASSERT_TRUE(result.convergence[i] <= result.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(ga_elitism_preserves_best) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 30;
    cfg.elitism_count = 2;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.convergence[result.num_iterations - 1] <=
                result.convergence[0] + 1e-9);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// TESTES: EDGE CASES
// ============================================================================

TEST(ga_zero_generations) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 10;
    cfg.max_generations = 0;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_NOT_NULL(result.best.data);
    ASSERT_EQ(result.num_iterations, (size_t)0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

TEST(ga_small_population) {
    ContinuousInstance *inst = continuous_create_sphere(2);
    ASSERT_NOT_NULL(inst);

    GAConfig cfg = ga_default_config();
    cfg.population_size = 4;
    cfg.max_generations = 20;
    cfg.elitism_count = 1;
    cfg.seed = 42;

    OptResult result = ga_run(&cfg,
                              sizeof(double) * 2, 2,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              inst);

    ASSERT_TRUE(result.num_iterations > 0);

    opt_result_destroy(&result);
    continuous_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Genetic Algorithm (Wave 4) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(ga_default_config_values);

    printf("\n[GA - TSP OX]\n");
    RUN_TEST(ga_ox_tsp_5);
    RUN_TEST(ga_ox_tsp_10);

    printf("\n[GA - TSP PMX]\n");
    RUN_TEST(ga_pmx_tsp_5);

    printf("\n[GA - Continuous BLX]\n");
    RUN_TEST(ga_blx_sphere);
    RUN_TEST(ga_blx_rastrigin);

    printf("\n[Selection Types]\n");
    RUN_TEST(ga_roulette_selection);
    RUN_TEST(ga_rank_selection);

    printf("\n[Adaptive Rates]\n");
    RUN_TEST(ga_adaptive_rates);

    printf("\n[Convergence]\n");
    RUN_TEST(ga_convergence_monotonic);
    RUN_TEST(ga_elitism_preserves_best);

    printf("\n[Edge Cases]\n");
    RUN_TEST(ga_zero_generations);
    RUN_TEST(ga_small_population);

    printf("\n=== Todos os %d testes passaram! ===\n", 13);
    return 0;
}
