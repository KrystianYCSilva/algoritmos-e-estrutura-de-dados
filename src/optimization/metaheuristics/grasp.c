/**
 * @file grasp.c
 * @brief Implementacao do GRASP (Greedy Randomized Adaptive Search Procedure)
 *
 * GRASP multi-start: cada iteracao constroi solucao com RCL
 * e aplica busca local. Variante reativa ajusta alpha dinamicamente.
 *
 * Referencias:
 * - Feo, T. A. & Resende, M. G. C. (1995). "Greedy Randomized Adaptive
 *   Search Procedures". J. Global Optimization, 6(2), 109-133.
 * - Resende, M. G. C. & Ribeiro, C. C. (2003). In Handbook of Metaheuristics.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/grasp.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/benchmarks/continuous.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool grasp_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static void grasp_local_search(void *solution, double *cost,
                               size_t element_size, size_t solution_size,
                               size_t max_iter, size_t neighbors_per_iter,
                               ObjectiveFn objective, NeighborFn neighbor,
                               OptDirection direction, const void *context,
                               size_t *eval_count) {
    void *candidate = malloc(element_size);
    void *best_neighbor = malloc(element_size);
    if (candidate == NULL || best_neighbor == NULL) {
        free(candidate);
        free(best_neighbor);
        return;
    }

    for (size_t iter = 0; iter < max_iter; iter++) {
        double best_neighbor_cost = (direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
        bool improved = false;

        for (size_t k = 0; k < neighbors_per_iter; k++) {
            neighbor(solution, candidate, solution_size, context);
            double cand_cost = objective(candidate, solution_size, context);
            (*eval_count)++;

            if (grasp_is_better(cand_cost, best_neighbor_cost, direction)) {
                memcpy(best_neighbor, candidate, element_size);
                best_neighbor_cost = cand_cost;
                improved = true;
            }
        }

        if (improved && grasp_is_better(best_neighbor_cost, *cost, direction)) {
            memcpy(solution, best_neighbor, element_size);
            *cost = best_neighbor_cost;
        } else {
            break;
        }
    }

    free(candidate);
    free(best_neighbor);
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

GRASPConfig grasp_default_config(void) {
    GRASPConfig config;
    config.max_iterations = 500;
    config.alpha = 0.3;
    config.local_search_iterations = 100;
    config.local_search_neighbors = 20;
    config.enable_reactive = false;
    config.reactive_num_alphas = 5;
    config.reactive_block_size = 50;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// GRASP PRINCIPAL
// ============================================================================

OptResult grasp_run(const GRASPConfig *config,
                    size_t element_size,
                    size_t solution_size,
                    ObjectiveFn objective,
                    GRASPConstructFn construct,
                    NeighborFn neighbor,
                    const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    if (current == NULL) return result;

    result.best = opt_solution_create(element_size);
    result.best.cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    double *alphas = NULL;
    double *alpha_scores = NULL;
    size_t *alpha_counts = NULL;
    size_t num_alphas = config->reactive_num_alphas;

    if (config->enable_reactive && num_alphas > 0) {
        alphas = malloc(num_alphas * sizeof(double));
        alpha_scores = calloc(num_alphas, sizeof(double));
        alpha_counts = calloc(num_alphas, sizeof(size_t));
        if (alphas == NULL || alpha_scores == NULL || alpha_counts == NULL) {
            free(alphas);
            free(alpha_scores);
            free(alpha_counts);
            free(current);
            return result;
        }
        for (size_t i = 0; i < num_alphas; i++) {
            alphas[i] = (double)(i + 1) / (double)(num_alphas + 1);
        }
    }

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        double alpha = config->alpha;
        size_t alpha_idx = 0;

        if (config->enable_reactive && alphas != NULL) {
            if (iter > 0 && iter % config->reactive_block_size == 0) {
                double total_score = 0;
                for (size_t i = 0; i < num_alphas; i++) {
                    if (alpha_counts[i] > 0) {
                        alpha_scores[i] = alpha_scores[i] / (double)alpha_counts[i];
                    }
                    total_score += alpha_scores[i];
                }
                if (total_score > 1e-12) {
                    for (size_t i = 0; i < num_alphas; i++) {
                        alpha_scores[i] /= total_score;
                    }
                }
                for (size_t i = 0; i < num_alphas; i++) {
                    alpha_scores[i] = 0;
                    alpha_counts[i] = 0;
                }
            }

            double r = opt_random_uniform();
            double cum = 0;
            alpha_idx = num_alphas - 1;
            double prob_each = 1.0 / (double)num_alphas;
            for (size_t i = 0; i < num_alphas; i++) {
                cum += prob_each;
                if (r < cum) {
                    alpha_idx = i;
                    break;
                }
            }
            alpha = alphas[alpha_idx];
        }

        construct(current, solution_size, alpha, context);
        double current_cost = objective(current, solution_size, context);
        result.num_evaluations++;

        grasp_local_search(current, &current_cost,
                           element_size, solution_size,
                           config->local_search_iterations,
                           config->local_search_neighbors,
                           objective, neighbor, config->direction, context,
                           &result.num_evaluations);

        if (config->enable_reactive && alpha_scores != NULL) {
            double score = (config->direction == OPT_MINIMIZE)
                           ? 1.0 / (1.0 + current_cost)
                           : current_cost;
            alpha_scores[alpha_idx] += score;
            alpha_counts[alpha_idx]++;
        }

        if (grasp_is_better(current_cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, current, element_size);
            result.best.cost = current_cost;
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(current);
    free(alphas);
    free(alpha_scores);
    free(alpha_counts);
    return result;
}

// ============================================================================
// CONSTRUTORES BUILTIN - TSP
// ============================================================================

void grasp_construct_tsp_nn(void *solution, size_t size,
                            double alpha, const void *context) {
    const TSPInstance *tsp = (const TSPInstance *)context;
    int *tour = (int *)solution;
    size_t n = size;

    bool *visited = calloc(n, sizeof(bool));
    if (visited == NULL) {
        for (size_t i = 0; i < n; i++) tour[i] = (int)i;
        return;
    }

    int start = opt_random_int(0, (int)(n - 1));
    tour[0] = start;
    visited[start] = true;

    for (size_t step = 1; step < n; step++) {
        int current = tour[step - 1];
        double d_min = DBL_MAX;
        double d_max = 0.0;

        for (size_t j = 0; j < n; j++) {
            if (!visited[j]) {
                double d = tsp->dist_matrix[current][j];
                if (d < d_min) d_min = d;
                if (d > d_max) d_max = d;
            }
        }

        double threshold = d_min + alpha * (d_max - d_min);

        int *rcl = malloc(n * sizeof(int));
        size_t rcl_size = 0;
        if (rcl == NULL) {
            for (size_t j = 0; j < n; j++) {
                if (!visited[j]) { tour[step] = (int)j; visited[j] = true; break; }
            }
            continue;
        }

        for (size_t j = 0; j < n; j++) {
            if (!visited[j] && tsp->dist_matrix[current][j] <= threshold + 1e-9) {
                rcl[rcl_size++] = (int)j;
            }
        }

        int chosen;
        if (rcl_size > 0) {
            chosen = rcl[opt_random_int(0, (int)(rcl_size - 1))];
        } else {
            for (size_t j = 0; j < n; j++) {
                if (!visited[j]) { chosen = (int)j; break; }
            }
        }

        tour[step] = chosen;
        visited[chosen] = true;
        free(rcl);
    }

    free(visited);
}

// ============================================================================
// CONSTRUTORES BUILTIN - CONTINUOUS
// ============================================================================

void grasp_construct_continuous(void *solution, size_t size,
                                double alpha, const void *context) {
    const ContinuousInstance *inst = (const ContinuousInstance *)context;
    double *x = (double *)solution;

    double center = (inst->lower_bound + inst->upper_bound) / 2.0;
    double range = inst->upper_bound - inst->lower_bound;

    for (size_t d = 0; d < size; d++) {
        double greedy = center;
        double random_val = inst->lower_bound + opt_random_uniform() * range;
        x[d] = greedy + alpha * (random_val - greedy);
    }
}
