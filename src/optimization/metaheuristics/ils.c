/**
 * @file ils.c
 * @brief Implementacao do Iterated Local Search (ILS)
 *
 * ILS combina busca local com perturbacao para explorar
 * multiplos otimos locais. A busca local interna usa
 * steepest descent, e a perturbacao eh delegada ao caller.
 *
 * Referencias:
 * - Lourenco, H. R., Martin, O. C. & Stutzle, T. (2003).
 *   "Iterated Local Search". In Handbook of Metaheuristics, Ch. 11.
 * - Talbi, E.-G. (2009). Metaheuristics, Ch. 3
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/ils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool ils_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static void local_search(void *solution, double *cost,
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

            if (ils_is_better(cand_cost, best_neighbor_cost, direction)) {
                memcpy(best_neighbor, candidate, element_size);
                best_neighbor_cost = cand_cost;
                improved = true;
            }
        }

        if (improved && ils_is_better(best_neighbor_cost, *cost, direction)) {
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

ILSConfig ils_default_config(void) {
    ILSConfig config;
    config.max_iterations = 1000;
    config.local_search_iterations = 200;
    config.local_search_neighbors = 20;
    config.perturbation_strength = 1;
    config.acceptance = ILS_ACCEPT_BETTER;
    config.sa_initial_temp = 10.0;
    config.sa_alpha = 0.95;
    config.restart_threshold = 50;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// ILS PRINCIPAL
// ============================================================================

OptResult ils_run(const ILSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  NeighborFn neighbor,
                  PerturbFn perturb,
                  GenerateFn generate,
                  const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    void *perturbed = malloc(element_size);
    void *ls_buffer = malloc(element_size);
    if (current == NULL || perturbed == NULL || ls_buffer == NULL) {
        free(current);
        free(perturbed);
        free(ls_buffer);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations = 1;

    local_search(current, &current_cost,
                 element_size, solution_size,
                 config->local_search_iterations,
                 config->local_search_neighbors,
                 objective, neighbor, config->direction, context,
                 &result.num_evaluations);

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, current, element_size);
    result.best.cost = current_cost;

    double sa_temp = config->sa_initial_temp;
    size_t no_improve_count = 0;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        if (perturb != NULL) {
            perturb(current, perturbed, solution_size,
                    config->perturbation_strength, context);
        } else {
            memcpy(perturbed, current, element_size);
            for (int s = 0; s < config->perturbation_strength; s++) {
                neighbor(perturbed, perturbed, solution_size, context);
            }
        }
        double perturbed_cost = objective(perturbed, solution_size, context);
        result.num_evaluations++;

        memcpy(ls_buffer, perturbed, element_size);
        double ls_cost = perturbed_cost;
        local_search(ls_buffer, &ls_cost,
                     element_size, solution_size,
                     config->local_search_iterations,
                     config->local_search_neighbors,
                     objective, neighbor, config->direction, context,
                     &result.num_evaluations);

        bool accept = false;
        switch (config->acceptance) {
            case ILS_ACCEPT_BETTER:
                accept = ils_is_better(ls_cost, current_cost, config->direction);
                break;
            case ILS_ACCEPT_ALWAYS:
                accept = true;
                break;
            case ILS_ACCEPT_SA_LIKE: {
                if (ils_is_better(ls_cost, current_cost, config->direction)) {
                    accept = true;
                } else {
                    double delta = (config->direction == OPT_MINIMIZE)
                                   ? (ls_cost - current_cost)
                                   : (current_cost - ls_cost);
                    if (sa_temp > 1e-12) {
                        double prob = exp(-delta / sa_temp);
                        accept = (opt_random_uniform() < prob);
                    }
                    sa_temp *= config->sa_alpha;
                }
                break;
            }
            case ILS_ACCEPT_RESTART:
                accept = ils_is_better(ls_cost, current_cost, config->direction);
                if (!accept) {
                    no_improve_count++;
                    if (no_improve_count >= config->restart_threshold) {
                        memcpy(current, result.best.data, element_size);
                        current_cost = result.best.cost;
                        no_improve_count = 0;
                    }
                } else {
                    no_improve_count = 0;
                }
                break;
        }

        if (accept) {
            memcpy(current, ls_buffer, element_size);
            current_cost = ls_cost;
        }

        if (ils_is_better(current_cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, current, element_size);
            result.best.cost = current_cost;
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(current);
    free(perturbed);
    free(ls_buffer);
    return result;
}
