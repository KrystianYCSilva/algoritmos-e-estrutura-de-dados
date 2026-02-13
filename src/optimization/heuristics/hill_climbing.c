/**
 * @file hill_climbing.c
 * @brief Implementacao do Hill Climbing e variantes
 *
 * Quatro variantes: Steepest, First Improvement, Random Restart, Stochastic.
 * Todas genericas via void* + ObjectiveFn + NeighborFn + GenerateFn.
 *
 * Referencias:
 * - Russell, S. & Norvig, P. (2010). Artificial Intelligence, Ch. 4.1
 * - Talbi, E.-G. (2009). Metaheuristics: From Design to Implementation, Ch. 3
 * - Luke, S. (2013). Essentials of Metaheuristics, Ch. 2
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/heuristics/hill_climbing.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

HCConfig hc_default_config(void) {
    HCConfig config;
    config.variant = HC_STEEPEST;
    config.max_iterations = 1000;
    config.neighbors_per_iter = 20;
    config.num_restarts = 10;
    config.stochastic_temperature = 1.0;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// STEEPEST
// ============================================================================

OptResult hc_steepest(const HCConfig *config,
                      size_t element_size,
                      size_t solution_size,
                      ObjectiveFn objective,
                      NeighborFn neighbor,
                      GenerateFn generate,
                      const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    void *candidate = malloc(element_size);
    void *best_neighbor_data = malloc(element_size);

    if (current == NULL || candidate == NULL || best_neighbor_data == NULL) {
        free(current);
        free(candidate);
        free(best_neighbor_data);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations = 1;

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, current, element_size);
    result.best.cost = current_cost;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        double best_nb_cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
        bool found_better = false;

        for (size_t k = 0; k < config->neighbors_per_iter; k++) {
            neighbor(current, candidate, solution_size, context);
            double cand_cost = objective(candidate, solution_size, context);
            result.num_evaluations++;

            if (is_better(cand_cost, best_nb_cost, config->direction)) {
                best_nb_cost = cand_cost;
                memcpy(best_neighbor_data, candidate, element_size);
                found_better = true;
            }
        }

        if (found_better && is_better(best_nb_cost, current_cost, config->direction)) {
            memcpy(current, best_neighbor_data, element_size);
            current_cost = best_nb_cost;

            if (is_better(current_cost, result.best.cost, config->direction)) {
                memcpy(result.best.data, current, element_size);
                result.best.cost = current_cost;
            }
        } else {
            if (result.convergence != NULL && iter < result.convergence_size) {
                result.convergence[iter] = result.best.cost;
            }
            result.num_iterations = iter + 1;
            break;
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(current);
    free(candidate);
    free(best_neighbor_data);
    return result;
}

// ============================================================================
// FIRST IMPROVEMENT
// ============================================================================

OptResult hc_first_improvement(const HCConfig *config,
                               size_t element_size,
                               size_t solution_size,
                               ObjectiveFn objective,
                               NeighborFn neighbor,
                               GenerateFn generate,
                               const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    void *candidate = malloc(element_size);

    if (current == NULL || candidate == NULL) {
        free(current);
        free(candidate);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations = 1;

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, current, element_size);
    result.best.cost = current_cost;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        bool improved = false;

        for (size_t k = 0; k < config->neighbors_per_iter; k++) {
            neighbor(current, candidate, solution_size, context);
            double cand_cost = objective(candidate, solution_size, context);
            result.num_evaluations++;

            if (is_better(cand_cost, current_cost, config->direction)) {
                memcpy(current, candidate, element_size);
                current_cost = cand_cost;
                improved = true;
                break;
            }
        }

        if (!improved) {
            if (result.convergence != NULL && iter < result.convergence_size) {
                result.convergence[iter] = result.best.cost;
            }
            result.num_iterations = iter + 1;
            break;
        }

        if (is_better(current_cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, current, element_size);
            result.best.cost = current_cost;
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(current);
    free(candidate);
    return result;
}

// ============================================================================
// RANDOM RESTART
// ============================================================================

OptResult hc_random_restart(const HCConfig *config,
                            size_t element_size,
                            size_t solution_size,
                            ObjectiveFn objective,
                            NeighborFn neighbor,
                            GenerateFn generate,
                            const void *context) {
    size_t total_max = config->max_iterations * config->num_restarts;
    OptResult result = opt_result_create(total_max);
    result.best = opt_solution_create(element_size);
    result.best.cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    size_t global_iter = 0;

    for (size_t r = 0; r < config->num_restarts; r++) {
        HCConfig sub_config = *config;
        sub_config.variant = HC_STEEPEST;
        sub_config.seed = config->seed + (unsigned)r;

        OptResult sub = hc_steepest(&sub_config, element_size, solution_size,
                                    objective, neighbor, generate, context);

        result.num_evaluations += sub.num_evaluations;

        for (size_t i = 0; i < sub.num_iterations && global_iter < total_max; i++) {
            if (result.convergence != NULL && global_iter < result.convergence_size) {
                result.convergence[global_iter] = result.best.cost;
                if (sub.convergence != NULL && i < sub.convergence_size) {
                    if (is_better(sub.convergence[i], result.convergence[global_iter],
                                  config->direction)) {
                        result.convergence[global_iter] = sub.convergence[i];
                    }
                }
            }
            global_iter++;
        }

        if (is_better(sub.best.cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, sub.best.data, element_size);
            result.best.cost = sub.best.cost;
        }

        opt_result_destroy(&sub);
    }

    result.num_iterations = global_iter;
    return result;
}

// ============================================================================
// STOCHASTIC
// ============================================================================

OptResult hc_stochastic(const HCConfig *config,
                        size_t element_size,
                        size_t solution_size,
                        ObjectiveFn objective,
                        NeighborFn neighbor,
                        GenerateFn generate,
                        const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    void *candidate = malloc(element_size);

    if (current == NULL || candidate == NULL) {
        free(current);
        free(candidate);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations = 1;

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, current, element_size);
    result.best.cost = current_cost;

    double temp = config->stochastic_temperature;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        neighbor(current, candidate, solution_size, context);
        double cand_cost = objective(candidate, solution_size, context);
        result.num_evaluations++;

        bool accept = false;
        if (is_better(cand_cost, current_cost, config->direction)) {
            accept = true;
        } else if (temp > 1e-15) {
            double delta = fabs(cand_cost - current_cost);
            double prob = exp(-delta / temp);
            if (opt_random_uniform() < prob) {
                accept = true;
            }
        }

        if (accept) {
            memcpy(current, candidate, element_size);
            current_cost = cand_cost;

            if (is_better(current_cost, result.best.cost, config->direction)) {
                memcpy(result.best.data, current, element_size);
                result.best.cost = current_cost;
            }
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(current);
    free(candidate);
    return result;
}

// ============================================================================
// DISPATCHER
// ============================================================================

OptResult hc_run(const HCConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 NeighborFn neighbor,
                 GenerateFn generate,
                 const void *context) {
    switch (config->variant) {
        case HC_STEEPEST:
            return hc_steepest(config, element_size, solution_size,
                               objective, neighbor, generate, context);
        case HC_FIRST_IMPROVEMENT:
            return hc_first_improvement(config, element_size, solution_size,
                                        objective, neighbor, generate, context);
        case HC_RANDOM_RESTART:
            return hc_random_restart(config, element_size, solution_size,
                                     objective, neighbor, generate, context);
        case HC_STOCHASTIC:
            return hc_stochastic(config, element_size, solution_size,
                                 objective, neighbor, generate, context);
        default:
            return hc_steepest(config, element_size, solution_size,
                               objective, neighbor, generate, context);
    }
}
