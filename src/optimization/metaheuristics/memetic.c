/**
 * @file memetic.c
 * @brief Implementacao do Algoritmo Memetico (MA)
 *
 * Referencias:
 * - Moscato, P. (1989). "On Evolution, Search, Optimization, Genetic
 *   Algorithms and Martial Arts: Towards Memetic Algorithms".
 * - Neri, F. & Cotta, C. (2012). "Memetic Algorithms and Memetic Computing
 *   Optimization: A Literature Review".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/memetic.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static int is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static void apply_local_search(void *solution, size_t element_size, size_t solution_size,
                               ObjectiveFn objective, NeighborFn neighbor,
                               const void *context, OptDirection direction,
                               size_t max_iter, size_t num_neighbors,
                               double *cost, size_t *evaluations,
                               MALearningType learning) {
    size_t data_size = element_size * solution_size;

    if (learning == MA_BALDWINIAN) {
        void *temp = malloc(data_size);
        if (temp == NULL) return;
        memcpy(temp, solution, data_size);

        void *candidate = malloc(data_size);
        void *best_n = malloc(data_size);
        if (candidate == NULL || best_n == NULL) {
            free(temp);
            free(candidate);
            free(best_n);
            return;
        }

        double temp_cost = *cost;
        for (size_t iter = 0; iter < max_iter; iter++) {
            double best_nc = (direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
            int improved = 0;

            for (size_t n = 0; n < num_neighbors; n++) {
                neighbor(temp, candidate, solution_size, context);
                double c = objective(candidate, solution_size, context);
                (*evaluations)++;
                if (is_better(c, best_nc, direction)) {
                    memcpy(best_n, candidate, data_size);
                    best_nc = c;
                }
            }
            if (is_better(best_nc, temp_cost, direction)) {
                memcpy(temp, best_n, data_size);
                temp_cost = best_nc;
                improved = 1;
            }
            if (!improved) break;
        }

        *cost = temp_cost;
        free(temp);
        free(candidate);
        free(best_n);
    } else {
        void *candidate = malloc(data_size);
        void *best_n = malloc(data_size);
        if (candidate == NULL || best_n == NULL) {
            free(candidate);
            free(best_n);
            return;
        }

        for (size_t iter = 0; iter < max_iter; iter++) {
            double best_nc = (direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
            int improved = 0;

            for (size_t n = 0; n < num_neighbors; n++) {
                neighbor(solution, candidate, solution_size, context);
                double c = objective(candidate, solution_size, context);
                (*evaluations)++;
                if (is_better(c, best_nc, direction)) {
                    memcpy(best_n, candidate, data_size);
                    best_nc = c;
                }
            }
            if (is_better(best_nc, *cost, direction)) {
                memcpy(solution, best_n, data_size);
                *cost = best_nc;
                improved = 1;
            }
            if (!improved) break;
        }

        free(candidate);
        free(best_n);
    }
}

static int tournament_select(const double *fitness, size_t pop_size,
                             size_t k, OptDirection direction) {
    int best = opt_random_int(0, (int)pop_size - 1);
    for (size_t t = 1; t < k; t++) {
        int r = opt_random_int(0, (int)pop_size - 1);
        if (is_better(fitness[r], fitness[best], direction)) {
            best = r;
        }
    }
    return best;
}

static int roulette_select(const double *fitness, size_t pop_size,
                           OptDirection direction) {
    double worst = fitness[0];
    for (size_t i = 1; i < pop_size; i++) {
        if (!is_better(fitness[i], worst, direction)) {
            worst = fitness[i];
        }
    }

    double offset = (direction == OPT_MINIMIZE) ? worst + 1.0 : 0.0;
    double total = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        if (direction == OPT_MINIMIZE) {
            total += (offset - fitness[i]);
        } else {
            total += fitness[i];
        }
    }

    if (total <= 0.0) return opt_random_int(0, (int)pop_size - 1);

    double r = opt_random_uniform() * total;
    double cumulative = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        if (direction == OPT_MINIMIZE) {
            cumulative += (offset - fitness[i]);
        } else {
            cumulative += fitness[i];
        }
        if (cumulative >= r) return (int)i;
    }
    return (int)pop_size - 1;
}

static int rank_select(size_t pop_size) {
    double total = (double)(pop_size * (pop_size + 1)) / 2.0;
    double r = opt_random_uniform() * total;
    double cumulative = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        cumulative += (double)(pop_size - i);
        if (cumulative >= r) return (int)i;
    }
    return (int)pop_size - 1;
}

static int select_parent(const MAConfig *config, const double *fitness,
                         const int *sorted_indices, size_t pop_size) {
    switch (config->selection) {
        case MA_SELECT_ROULETTE:
            return roulette_select(fitness, pop_size, config->direction);
        case MA_SELECT_RANK:
            return sorted_indices[rank_select(pop_size)];
        case MA_SELECT_TOURNAMENT:
        default:
            return tournament_select(fitness, pop_size, config->tournament_size, config->direction);
    }
}

static void sort_indices(int *indices, size_t n, const double *fitness) {
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (fitness[indices[j]] < fitness[indices[i]]) {
                int tmp = indices[i];
                indices[i] = indices[j];
                indices[j] = tmp;
            }
        }
    }
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

MAConfig ma_default_config(void) {
    MAConfig config;
    config.population_size = 50;
    config.max_generations = 200;
    config.crossover_rate = 0.8;
    config.mutation_rate = 0.05;
    config.elitism_count = 2;
    config.selection = MA_SELECT_TOURNAMENT;
    config.tournament_size = 3;
    config.learning = MA_LAMARCKIAN;
    config.ls_iterations = 50;
    config.ls_neighbors = 10;
    config.ls_probability = 1.0;
    config.ls_on_initial = true;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// ALGORITMO PRINCIPAL
// ============================================================================

OptResult ma_run(const MAConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 GenerateFn generate,
                 MACrossoverFn crossover,
                 MAMutationFn mutate,
                 NeighborFn neighbor,
                 const void *context) {
    if (config == NULL || objective == NULL || generate == NULL ||
        crossover == NULL || mutate == NULL || neighbor == NULL) {
        OptResult empty = {0};
        return empty;
    }

    size_t NP = config->population_size;
    if (NP < 4) NP = 4;
    size_t data_size = element_size * solution_size;

    opt_set_seed(config->seed);

    OptResult result = opt_result_create(config->max_generations);
    result.num_iterations = 0;
    result.num_evaluations = 0;

    void **pop = malloc(NP * sizeof(void *));
    double *fitness = malloc(NP * sizeof(double));
    int *sorted_idx = malloc(NP * sizeof(int));
    if (pop == NULL || fitness == NULL || sorted_idx == NULL) {
        free(pop);
        free(fitness);
        free(sorted_idx);
        return result;
    }

    for (size_t i = 0; i < NP; i++) {
        pop[i] = malloc(data_size);
        if (pop[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(pop[j]);
            free(pop);
            free(fitness);
            free(sorted_idx);
            return result;
        }
    }

    size_t best_idx = 0;
    double best_fitness = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    for (size_t i = 0; i < NP; i++) {
        generate(pop[i], solution_size, context);
        fitness[i] = objective(pop[i], solution_size, context);
        result.num_evaluations++;

        if (config->ls_on_initial) {
            apply_local_search(pop[i], element_size, solution_size, objective, neighbor,
                               context, config->direction, config->ls_iterations,
                               config->ls_neighbors, &fitness[i], &result.num_evaluations,
                               config->learning);
        }

        if (is_better(fitness[i], best_fitness, config->direction)) {
            best_fitness = fitness[i];
            best_idx = i;
        }
        sorted_idx[i] = (int)i;
    }

    sort_indices(sorted_idx, NP, fitness);

    void *child1 = malloc(data_size);
    void *child2 = malloc(data_size);
    void **new_pop = malloc(NP * sizeof(void *));
    double *new_fitness = malloc(NP * sizeof(double));
    if (child1 == NULL || child2 == NULL || new_pop == NULL || new_fitness == NULL) {
        free(child1);
        free(child2);
        free(new_pop);
        free(new_fitness);
        for (size_t i = 0; i < NP; i++) free(pop[i]);
        free(pop);
        free(fitness);
        free(sorted_idx);
        return result;
    }

    for (size_t i = 0; i < NP; i++) {
        new_pop[i] = malloc(data_size);
        if (new_pop[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(new_pop[j]);
            free(new_pop);
            free(new_fitness);
            free(child1);
            free(child2);
            for (size_t j = 0; j < NP; j++) free(pop[j]);
            free(pop);
            free(fitness);
            free(sorted_idx);
            return result;
        }
    }

    for (size_t gen = 0; gen < config->max_generations; gen++) {
        size_t new_count = 0;

        for (size_t e = 0; e < config->elitism_count && e < NP; e++) {
            memcpy(new_pop[new_count], pop[sorted_idx[e]], data_size);
            new_fitness[new_count] = fitness[sorted_idx[e]];
            new_count++;
        }

        while (new_count + 1 < NP) {
            int p1 = select_parent(config, fitness, sorted_idx, NP);
            int p2 = select_parent(config, fitness, sorted_idx, NP);

            if (opt_random_uniform() < config->crossover_rate) {
                crossover(pop[p1], pop[p2], child1, child2, solution_size, context);
            } else {
                memcpy(child1, pop[p1], data_size);
                memcpy(child2, pop[p2], data_size);
            }

            mutate(child1, solution_size, config->mutation_rate, context);
            mutate(child2, solution_size, config->mutation_rate, context);

            double c1_cost = objective(child1, solution_size, context);
            double c2_cost = objective(child2, solution_size, context);
            result.num_evaluations += 2;

            if (opt_random_uniform() < config->ls_probability) {
                apply_local_search(child1, element_size, solution_size, objective, neighbor,
                                   context, config->direction, config->ls_iterations,
                                   config->ls_neighbors, &c1_cost, &result.num_evaluations,
                                   config->learning);
            }
            if (opt_random_uniform() < config->ls_probability) {
                apply_local_search(child2, element_size, solution_size, objective, neighbor,
                                   context, config->direction, config->ls_iterations,
                                   config->ls_neighbors, &c2_cost, &result.num_evaluations,
                                   config->learning);
            }

            memcpy(new_pop[new_count], child1, data_size);
            new_fitness[new_count] = c1_cost;
            new_count++;

            if (new_count < NP) {
                memcpy(new_pop[new_count], child2, data_size);
                new_fitness[new_count] = c2_cost;
                new_count++;
            }
        }

        if (new_count < NP) {
            int p = select_parent(config, fitness, sorted_idx, NP);
            memcpy(new_pop[new_count], pop[p], data_size);
            new_fitness[new_count] = fitness[p];
            new_count++;
        }

        for (size_t i = 0; i < NP; i++) {
            memcpy(pop[i], new_pop[i], data_size);
            fitness[i] = new_fitness[i];
            sorted_idx[i] = (int)i;

            if (is_better(fitness[i], best_fitness, config->direction)) {
                best_fitness = fitness[i];
                best_idx = i;
            }
        }

        sort_indices(sorted_idx, NP, fitness);

        result.num_iterations = gen + 1;
        if (gen < result.convergence_size) {
            result.convergence[gen] = best_fitness;
        }
    }

    result.best = opt_solution_create(data_size);
    if (result.best.data != NULL) {
        memcpy(result.best.data, pop[best_idx], data_size);
        result.best.cost = best_fitness;
    }

    free(child1);
    free(child2);
    for (size_t i = 0; i < NP; i++) {
        free(pop[i]);
        free(new_pop[i]);
    }
    free(pop);
    free(new_pop);
    free(fitness);
    free(new_fitness);
    free(sorted_idx);

    return result;
}
