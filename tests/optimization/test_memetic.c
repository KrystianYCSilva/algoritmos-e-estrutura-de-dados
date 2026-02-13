/**
 * @file test_memetic.c
 * @brief Testes do Algoritmo Memetico (Wave 11)
 */

#include "../test_macros.h"
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/memetic.h"
#include "optimization/metaheuristics/genetic_algorithm.h"

// ============================================================================
// WRAPPERS (GA operators -> MA operator signatures)
// ============================================================================

static void ma_crossover_ox(const void *p1, const void *p2,
                            void *c1, void *c2,
                            size_t size, const void *context) {
    ga_crossover_ox(p1, p2, c1, c2, size, context);
}

static void ma_mutation_swap(void *solution, size_t size,
                             double rate, const void *context) {
    ga_mutation_swap(solution, size, rate, context);
}

static void ma_crossover_blx(const void *p1, const void *p2,
                             void *c1, void *c2,
                             size_t size, const void *context) {
    ga_crossover_blx(p1, p2, c1, c2, size, context);
}

static void ma_mutation_gaussian(void *solution, size_t size,
                                 double rate, const void *context) {
    ga_mutation_gaussian(solution, size, rate, context);
}

// ============================================================================
// TESTES DE CONFIGURACAO
// ============================================================================

TEST(ma_default_config_values) {
    MAConfig cfg = ma_default_config();
    ASSERT_EQ(cfg.population_size, (size_t)50);
    ASSERT_EQ(cfg.max_generations, (size_t)200);
    ASSERT_NEAR(cfg.crossover_rate, 0.8, 1e-9);
    ASSERT_NEAR(cfg.mutation_rate, 0.05, 1e-9);
    ASSERT_EQ(cfg.elitism_count, (size_t)2);
    ASSERT_EQ(cfg.selection, MA_SELECT_TOURNAMENT);
    ASSERT_EQ(cfg.learning, MA_LAMARCKIAN);
    ASSERT_NEAR(cfg.ls_probability, 1.0, 1e-9);
    ASSERT_TRUE(cfg.ls_on_initial);
    ASSERT_EQ(cfg.direction, OPT_MINIMIZE);
    ASSERT_EQ(cfg.seed, (unsigned)42);
}

// ============================================================================
// LAMARCKIAN - TSP
// ============================================================================

TEST(ma_lamarck_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 30;
    cfg.ls_iterations = 30;
    cfg.ls_neighbors = 10;
    cfg.learning = MA_LAMARCKIAN;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(ma_lamarck_tsp10) {
    TSPInstance *inst = tsp_create_example_10();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 50;
    cfg.ls_iterations = 50;
    cfg.ls_neighbors = 10;
    cfg.learning = MA_LAMARCKIAN;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_2opt, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// BALDWINIAN
// ============================================================================

TEST(ma_baldwin_tsp5) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 30;
    cfg.ls_iterations = 20;
    cfg.ls_neighbors = 10;
    cfg.learning = MA_BALDWINIAN;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.best.cost, 0.0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// CONTINUOUS
// ============================================================================

TEST(ma_lamarck_sphere5) {
    ContinuousInstance *inst = continuous_create_sphere(5);
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 30;
    cfg.max_generations = 50;
    cfg.ls_iterations = 30;
    cfg.ls_neighbors = 15;
    cfg.learning = MA_LAMARCKIAN;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(double), inst->dimensions,
                           continuous_evaluate, continuous_generate_random,
                           ma_crossover_blx, ma_mutation_gaussian,
                           continuous_neighbor_gaussian, inst);
    ASSERT_LT(res.best.cost, 5.0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

// ============================================================================
// SELECTION VARIANTS
// ============================================================================

TEST(ma_roulette_selection) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 20;
    cfg.selection = MA_SELECT_ROULETTE;
    cfg.ls_iterations = 20;
    cfg.ls_neighbors = 10;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    ASSERT_NOT_NULL(res.best.data);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(ma_rank_selection) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 20;
    cfg.selection = MA_SELECT_RANK;
    cfg.ls_iterations = 20;
    cfg.ls_neighbors = 10;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    ASSERT_NOT_NULL(res.best.data);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(ma_zero_generations) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.max_generations = 0;
    cfg.population_size = 10;
    cfg.ls_on_initial = false;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_EQ(res.num_iterations, (size_t)0);

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

TEST(ma_partial_ls_probability) {
    ContinuousInstance *inst = continuous_create_sphere(3);
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 20;
    cfg.ls_probability = 0.5;
    cfg.ls_iterations = 20;
    cfg.ls_neighbors = 10;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(double), inst->dimensions,
                           continuous_evaluate, continuous_generate_random,
                           ma_crossover_blx, ma_mutation_gaussian,
                           continuous_neighbor_gaussian, inst);
    ASSERT_NOT_NULL(res.best.data);
    ASSERT_GT(res.num_evaluations, (size_t)0);

    opt_result_destroy(&res);
    continuous_instance_destroy(inst);
}

TEST(ma_convergence_monotonic) {
    TSPInstance *inst = tsp_create_example_5();
    ASSERT_NOT_NULL(inst);

    MAConfig cfg = ma_default_config();
    cfg.population_size = 20;
    cfg.max_generations = 30;
    cfg.ls_iterations = 20;
    cfg.ls_neighbors = 10;
    cfg.seed = 42;

    OptResult res = ma_run(&cfg, sizeof(int), inst->n_cities,
                           tsp_tour_cost, tsp_generate_random,
                           ma_crossover_ox, ma_mutation_swap,
                           tsp_neighbor_swap, inst);
    for (size_t i = 1; i < res.num_iterations; i++) {
        ASSERT(res.convergence[i] <= res.convergence[i - 1] + 1e-9);
    }

    opt_result_destroy(&res);
    tsp_instance_destroy(inst);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Testes: Memetic Algorithm (Wave 11) ===\n\n");

    printf("[Configuracao]\n");
    RUN_TEST(ma_default_config_values);

    printf("\n[Lamarckian - TSP]\n");
    RUN_TEST(ma_lamarck_tsp5);
    RUN_TEST(ma_lamarck_tsp10);

    printf("\n[Baldwinian]\n");
    RUN_TEST(ma_baldwin_tsp5);

    printf("\n[Continuous]\n");
    RUN_TEST(ma_lamarck_sphere5);

    printf("\n[Selection Variants]\n");
    RUN_TEST(ma_roulette_selection);
    RUN_TEST(ma_rank_selection);

    printf("\n[Edge Cases]\n");
    RUN_TEST(ma_zero_generations);
    RUN_TEST(ma_partial_ls_probability);
    RUN_TEST(ma_convergence_monotonic);

    printf("\n=== Todos os 10 testes passaram! ===\n");
    return 0;
}
