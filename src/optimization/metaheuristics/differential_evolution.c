/**
 * @file differential_evolution.c
 * @brief Implementacao do Differential Evolution (DE)
 *
 * Referencias:
 * - Storn, R. & Price, K. (1997). "Differential Evolution - A Simple and
 *   Efficient Heuristic for Global Optimization over Continuous Spaces".
 * - Price, K., Storn, R. & Lampinen, J. (2005). Differential Evolution:
 *   A Practical Approach to Global Optimization.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/differential_evolution.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static int is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static void clamp_vector(double *v, size_t D, double lb, double ub) {
    for (size_t i = 0; i < D; i++) {
        if (v[i] < lb) v[i] = lb;
        if (v[i] > ub) v[i] = ub;
    }
}

static int select_random_distinct(int *indices, int count, int NP, int exclude) {
    for (int c = 0; c < count; c++) {
        int r;
        int ok;
        do {
            r = opt_random_int(0, NP - 1);
            ok = (r != exclude);
            for (int j = 0; j < c && ok; j++) {
                if (indices[j] == r) ok = 0;
            }
        } while (!ok);
        indices[c] = r;
    }
    return 0;
}

// ============================================================================
// MUTACAO
// ============================================================================

static void mutation_rand_1(double *donor, const double *const *pop,
                            size_t D, double F, int NP, int i) {
    int idx[3];
    select_random_distinct(idx, 3, NP, i);
    for (size_t d = 0; d < D; d++) {
        donor[d] = pop[idx[0]][d] + F * (pop[idx[1]][d] - pop[idx[2]][d]);
    }
}

static void mutation_best_1(double *donor, const double *const *pop,
                            const double *best, size_t D, double F, int NP, int i) {
    int idx[2];
    select_random_distinct(idx, 2, NP, i);
    for (size_t d = 0; d < D; d++) {
        donor[d] = best[d] + F * (pop[idx[0]][d] - pop[idx[1]][d]);
    }
}

static void mutation_current_to_best_1(double *donor, const double *const *pop,
                                       const double *best, size_t D, double F,
                                       int NP, int i) {
    int idx[2];
    select_random_distinct(idx, 2, NP, i);
    for (size_t d = 0; d < D; d++) {
        donor[d] = pop[i][d] + F * (best[d] - pop[i][d])
                   + F * (pop[idx[0]][d] - pop[idx[1]][d]);
    }
}

static void mutation_rand_2(double *donor, const double *const *pop,
                            size_t D, double F, int NP, int i) {
    int idx[5];
    select_random_distinct(idx, 5, NP, i);
    for (size_t d = 0; d < D; d++) {
        donor[d] = pop[idx[0]][d]
                   + F * (pop[idx[1]][d] - pop[idx[2]][d])
                   + F * (pop[idx[3]][d] - pop[idx[4]][d]);
    }
}

static void mutation_best_2(double *donor, const double *const *pop,
                            const double *best, size_t D, double F, int NP, int i) {
    int idx[4];
    select_random_distinct(idx, 4, NP, i);
    for (size_t d = 0; d < D; d++) {
        donor[d] = best[d]
                   + F * (pop[idx[0]][d] - pop[idx[1]][d])
                   + F * (pop[idx[2]][d] - pop[idx[3]][d]);
    }
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

DEConfig de_default_config(void) {
    DEConfig config;
    config.population_size = 50;
    config.max_generations = 1000;
    config.F = 0.8;
    config.CR = 0.9;
    config.strategy = DE_RAND_1;
    config.lower_bound = -5.12;
    config.upper_bound = 5.12;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// ALGORITMO PRINCIPAL
// ============================================================================

OptResult de_run(const DEConfig *config,
                 size_t solution_size,
                 ObjectiveFn objective,
                 const void *context) {
    if (config == NULL || objective == NULL || solution_size == 0) {
        OptResult empty = {0};
        return empty;
    }

    size_t NP = config->population_size;
    size_t D = solution_size;
    double F = config->F;
    double CR = config->CR;
    double lb = config->lower_bound;
    double ub = config->upper_bound;

    if (NP < 4) NP = 4;

    opt_set_seed(config->seed);

    OptResult result = opt_result_create(config->max_generations);
    result.num_iterations = 0;
    result.num_evaluations = 0;

    double **pop = malloc(NP * sizeof(double *));
    double *fitness = malloc(NP * sizeof(double));
    if (pop == NULL || fitness == NULL) {
        free(pop);
        free(fitness);
        return result;
    }

    for (size_t i = 0; i < NP; i++) {
        pop[i] = malloc(D * sizeof(double));
        if (pop[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(pop[j]);
            free(pop);
            free(fitness);
            return result;
        }
    }

    size_t best_idx = 0;
    double best_fitness = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    for (size_t i = 0; i < NP; i++) {
        for (size_t d = 0; d < D; d++) {
            pop[i][d] = lb + opt_random_uniform() * (ub - lb);
        }
        fitness[i] = objective(pop[i], D, context);
        result.num_evaluations++;

        if (is_better(fitness[i], best_fitness, config->direction)) {
            best_fitness = fitness[i];
            best_idx = i;
        }
    }

    double *donor = malloc(D * sizeof(double));
    double *trial = malloc(D * sizeof(double));
    if (donor == NULL || trial == NULL) {
        for (size_t i = 0; i < NP; i++) free(pop[i]);
        free(pop);
        free(fitness);
        free(donor);
        free(trial);
        return result;
    }

    for (size_t gen = 0; gen < config->max_generations; gen++) {
        for (size_t i = 0; i < NP; i++) {
            switch (config->strategy) {
                case DE_RAND_1:
                    mutation_rand_1(donor, (const double *const *)pop, D, F, (int)NP, (int)i);
                    break;
                case DE_BEST_1:
                    mutation_best_1(donor, (const double *const *)pop, pop[best_idx], D, F, (int)NP, (int)i);
                    break;
                case DE_CURRENT_TO_BEST_1:
                    mutation_current_to_best_1(donor, (const double *const *)pop, pop[best_idx], D, F, (int)NP, (int)i);
                    break;
                case DE_RAND_2:
                    mutation_rand_2(donor, (const double *const *)pop, D, F, (int)NP, (int)i);
                    break;
                case DE_BEST_2:
                    mutation_best_2(donor, (const double *const *)pop, pop[best_idx], D, F, (int)NP, (int)i);
                    break;
            }

            clamp_vector(donor, D, lb, ub);

            int j_rand = opt_random_int(0, (int)D - 1);
            for (size_t d = 0; d < D; d++) {
                if (opt_random_uniform() < CR || (int)d == j_rand) {
                    trial[d] = donor[d];
                } else {
                    trial[d] = pop[i][d];
                }
            }

            double trial_fitness = objective(trial, D, context);
            result.num_evaluations++;

            if (is_better(trial_fitness, fitness[i], config->direction) ||
                trial_fitness == fitness[i]) {
                memcpy(pop[i], trial, D * sizeof(double));
                fitness[i] = trial_fitness;

                if (is_better(trial_fitness, best_fitness, config->direction)) {
                    best_fitness = trial_fitness;
                    best_idx = i;
                }
            }
        }

        result.num_iterations = gen + 1;
        if (gen < result.convergence_size) {
            result.convergence[gen] = best_fitness;
        }
    }

    result.best = opt_solution_create(D * sizeof(double));
    if (result.best.data != NULL) {
        memcpy(result.best.data, pop[best_idx], D * sizeof(double));
        result.best.cost = best_fitness;
    }

    free(donor);
    free(trial);
    for (size_t i = 0; i < NP; i++) free(pop[i]);
    free(pop);
    free(fitness);

    return result;
}
