/**
 * @file vns.c
 * @brief Implementacao do Variable Neighborhood Search (VNS)
 *
 * Referencias:
 * - Mladenovic, N. & Hansen, P. (1997). "Variable Neighborhood Search".
 * - Hansen, P. & Mladenovic, N. (2001). "Variable Neighborhood Search:
 *   Principles and Applications".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/vns.h"
#include "optimization/benchmarks/continuous.h"
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

static void local_search(void *solution, size_t element_size, size_t solution_size,
                         ObjectiveFn objective, NeighborFn neighbor,
                         const void *context, OptDirection direction,
                         size_t max_iter, size_t num_neighbors,
                         double *cost, size_t *evaluations) {
    size_t data_size = element_size * solution_size;
    void *candidate = malloc(data_size);
    void *best_neighbor = malloc(data_size);
    if (candidate == NULL || best_neighbor == NULL) {
        free(candidate);
        free(best_neighbor);
        return;
    }

    for (size_t iter = 0; iter < max_iter; iter++) {
        double best_neighbor_cost = (direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
        int improved = 0;

        for (size_t n = 0; n < num_neighbors; n++) {
            neighbor(solution, candidate, solution_size, context);
            double c = objective(candidate, solution_size, context);
            (*evaluations)++;

            if (is_better(c, best_neighbor_cost, direction)) {
                memcpy(best_neighbor, candidate, data_size);
                best_neighbor_cost = c;
            }
        }

        if (is_better(best_neighbor_cost, *cost, direction)) {
            memcpy(solution, best_neighbor, data_size);
            *cost = best_neighbor_cost;
            improved = 1;
        }

        if (!improved) break;
    }

    free(candidate);
    free(best_neighbor);
}

static void vnd_search(void *solution, size_t element_size, size_t solution_size,
                       ObjectiveFn objective, NeighborFn neighbor,
                       const void *context, OptDirection direction,
                       size_t max_iter, size_t num_neighbors,
                       int num_neighborhoods,
                       double *cost, size_t *evaluations) {
    size_t data_size = element_size * solution_size;
    void *candidate = malloc(data_size);
    if (candidate == NULL) return;

    for (int l = 1; l <= num_neighborhoods; l++) {
        int improved_in_neighborhood = 0;

        for (size_t iter = 0; iter < max_iter; iter++) {
            double best_c = (direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
            void *best_cand = malloc(data_size);
            if (best_cand == NULL) break;

            size_t neighbors_to_check = num_neighbors * (size_t)l;
            for (size_t n = 0; n < neighbors_to_check; n++) {
                neighbor(solution, candidate, solution_size, context);
                double c = objective(candidate, solution_size, context);
                (*evaluations)++;
                if (is_better(c, best_c, direction)) {
                    memcpy(best_cand, candidate, data_size);
                    best_c = c;
                }
            }

            if (is_better(best_c, *cost, direction)) {
                memcpy(solution, best_cand, data_size);
                *cost = best_c;
                improved_in_neighborhood = 1;
                free(best_cand);
            } else {
                free(best_cand);
                break;
            }
        }

        if (improved_in_neighborhood) {
            l = 0;
        }
    }

    free(candidate);
}

// ============================================================================
// SHAKE BUILTIN
// ============================================================================

void vns_shake_tsp(const void *current, void *shaken, size_t size,
                   int k, const void *context) {
    (void)context;
    memcpy(shaken, current, size * sizeof(int));
    int *tour = (int *)shaken;
    int n = (int)size;
    for (int s = 0; s < k; s++) {
        int i = opt_random_int(0, n - 1);
        int j = opt_random_int(0, n - 1);
        while (j == i) j = opt_random_int(0, n - 1);
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
    }
}

void vns_shake_continuous(const void *current, void *shaken, size_t size,
                          int k, const void *context) {
    const ContinuousInstance *inst = (const ContinuousInstance *)context;
    memcpy(shaken, current, size * sizeof(double));
    double *vec = (double *)shaken;
    double sigma = (double)k * 0.5;
    for (size_t d = 0; d < size; d++) {
        vec[d] += sigma * opt_random_gaussian();
        if (inst != NULL) {
            if (vec[d] < inst->lower_bound) vec[d] = inst->lower_bound;
            if (vec[d] > inst->upper_bound) vec[d] = inst->upper_bound;
        }
    }
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

VNSConfig vns_default_config(void) {
    VNSConfig config;
    config.max_iterations = 1000;
    config.k_max = 5;
    config.local_search_iterations = 200;
    config.local_search_neighbors = 20;
    config.variant = VNS_BASIC;
    config.vnd_num_neighborhoods = 3;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// ALGORITMO PRINCIPAL
// ============================================================================

OptResult vns_run(const VNSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  ShakeFn shake,
                  NeighborFn neighbor,
                  GenerateFn generate,
                  const void *context) {
    if (config == NULL || objective == NULL || shake == NULL || generate == NULL ||
        element_size == 0 || solution_size == 0) {
        OptResult empty = {0};
        return empty;
    }

    opt_set_seed(config->seed);

    size_t data_size = element_size * solution_size;
    OptResult result = opt_result_create(config->max_iterations);
    result.num_iterations = 0;
    result.num_evaluations = 0;

    void *current = malloc(data_size);
    void *shaken = malloc(data_size);
    void *ls_solution = malloc(data_size);
    if (current == NULL || shaken == NULL || ls_solution == NULL) {
        free(current);
        free(shaken);
        free(ls_solution);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations++;

    if (config->variant != VNS_REDUCED && neighbor != NULL) {
        if (config->variant == VNS_GENERAL) {
            vnd_search(current, element_size, solution_size, objective, neighbor,
                       context, config->direction,
                       config->local_search_iterations, config->local_search_neighbors,
                       config->vnd_num_neighborhoods,
                       &current_cost, &result.num_evaluations);
        } else {
            local_search(current, element_size, solution_size, objective, neighbor,
                         context, config->direction,
                         config->local_search_iterations, config->local_search_neighbors,
                         &current_cost, &result.num_evaluations);
        }
    }

    double best_cost = current_cost;
    void *best_data = malloc(data_size);
    if (best_data == NULL) {
        free(current);
        free(shaken);
        free(ls_solution);
        return result;
    }
    memcpy(best_data, current, data_size);

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        int k = 1;

        while (k <= config->k_max) {
            shake(current, shaken, solution_size, k, context);
            double shaken_cost = objective(shaken, solution_size, context);
            result.num_evaluations++;

            memcpy(ls_solution, shaken, data_size);
            double ls_cost = shaken_cost;

            if (config->variant != VNS_REDUCED && neighbor != NULL) {
                if (config->variant == VNS_GENERAL) {
                    vnd_search(ls_solution, element_size, solution_size, objective, neighbor,
                               context, config->direction,
                               config->local_search_iterations, config->local_search_neighbors,
                               config->vnd_num_neighborhoods,
                               &ls_cost, &result.num_evaluations);
                } else {
                    local_search(ls_solution, element_size, solution_size, objective, neighbor,
                                 context, config->direction,
                                 config->local_search_iterations, config->local_search_neighbors,
                                 &ls_cost, &result.num_evaluations);
                }
            }

            if (is_better(ls_cost, current_cost, config->direction)) {
                memcpy(current, ls_solution, data_size);
                current_cost = ls_cost;
                k = 1;

                if (is_better(current_cost, best_cost, config->direction)) {
                    memcpy(best_data, current, data_size);
                    best_cost = current_cost;
                }
            } else {
                k++;
            }
        }

        result.num_iterations = iter + 1;
        if (iter < result.convergence_size) {
            result.convergence[iter] = best_cost;
        }
    }

    result.best = opt_solution_create(data_size);
    if (result.best.data != NULL) {
        memcpy(result.best.data, best_data, data_size);
        result.best.cost = best_cost;
    }

    free(current);
    free(shaken);
    free(ls_solution);
    free(best_data);

    return result;
}
