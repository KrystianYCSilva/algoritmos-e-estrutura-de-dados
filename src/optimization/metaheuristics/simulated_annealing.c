/**
 * @file simulated_annealing.c
 * @brief Implementacao do Simulated Annealing e variantes
 *
 * SA classico com 4 cooling schedules (geometric, linear, logarithmic, adaptive),
 * reheating opcional e auto-calibracao de T0.
 *
 * Referencias:
 * - Kirkpatrick, S., Gelatt, C. D. & Vecchi, M. P. (1983).
 *   "Optimization by Simulated Annealing". Science, 220(4598), 671-680.
 * - Hajek, B. (1988). "Cooling Schedules for Optimal Annealing"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/simulated_annealing.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool sa_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static double sa_cool(double T, double T0, double T_min, double alpha,
                      SACoolingSchedule cooling, size_t step,
                      double acceptance_rate,
                      double target_low, double target_high, double adapt_factor) {
    switch (cooling) {
        case SA_COOLING_GEOMETRIC:
            return T * alpha;

        case SA_COOLING_LINEAR: {
            double delta = T - (T0 - T_min) / 1000.0;
            return (delta > T_min) ? delta : T_min;
        }

        case SA_COOLING_LOGARITHMIC:
            return T0 / log(2.0 + (double)step);

        case SA_COOLING_ADAPTIVE:
            if (acceptance_rate < target_low) {
                return T * adapt_factor;
            } else if (acceptance_rate > target_high) {
                return T / adapt_factor;
            }
            return T;

        default:
            return T * alpha;
    }
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

SAConfig sa_default_config(void) {
    SAConfig config;
    config.initial_temp = 100.0;
    config.final_temp = 0.001;
    config.alpha = 0.95;
    config.cooling = SA_COOLING_GEOMETRIC;
    config.max_iterations = 10000;
    config.markov_chain_length = 50;

    config.enable_reheating = false;
    config.reheat_threshold = 0.01;
    config.reheat_factor = 2.0;

    config.auto_calibrate_t0 = false;
    config.calibration_samples = 100;
    config.target_acceptance = 0.8;

    config.adaptive_target_low = 0.2;
    config.adaptive_target_high = 0.5;
    config.adaptive_factor = 1.05;

    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// AUTO-CALIBRACAO T0
// ============================================================================

double sa_calibrate_t0(const SAConfig *config,
                       size_t element_size,
                       size_t solution_size,
                       ObjectiveFn objective,
                       NeighborFn neighbor,
                       GenerateFn generate,
                       const void *context) {
    opt_set_seed(config->seed);

    void *current = malloc(element_size);
    void *candidate = malloc(element_size);
    if (current == NULL || candidate == NULL) {
        free(current);
        free(candidate);
        return config->initial_temp;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);

    double sum_delta = 0.0;
    size_t count = 0;
    size_t samples = config->calibration_samples;
    if (samples == 0) samples = 100;

    for (size_t i = 0; i < samples; i++) {
        neighbor(current, candidate, solution_size, context);
        double cand_cost = objective(candidate, solution_size, context);
        double delta = fabs(cand_cost - current_cost);
        if (delta > 1e-15) {
            sum_delta += delta;
            count++;
        }
    }

    free(current);
    free(candidate);

    if (count == 0) return config->initial_temp;

    double avg_delta = sum_delta / (double)count;
    double target = config->target_acceptance;
    if (target <= 0.0 || target >= 1.0) target = 0.8;

    return -avg_delta / log(target);
}

// ============================================================================
// SA PRINCIPAL
// ============================================================================

OptResult sa_run(const SAConfig *config,
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

    double T;
    if (config->auto_calibrate_t0) {
        T = sa_calibrate_t0(config, element_size, solution_size,
                            objective, neighbor, generate, context);
        opt_set_seed(config->seed);
        generate(current, solution_size, context);
        current_cost = objective(current, solution_size, context);
        memcpy(result.best.data, current, element_size);
        result.best.cost = current_cost;
        result.num_evaluations = 1;
    } else {
        T = config->initial_temp;
    }

    double T0 = T;
    size_t global_iter = 0;
    size_t temp_step = 0;

    while (T > config->final_temp && global_iter < config->max_iterations) {
        size_t accepted = 0;
        size_t chain_len = config->markov_chain_length;
        if (chain_len == 0) chain_len = 1;

        for (size_t i = 0; i < chain_len && global_iter < config->max_iterations; i++) {
            neighbor(current, candidate, solution_size, context);
            double cand_cost = objective(candidate, solution_size, context);
            result.num_evaluations++;

            double delta;
            if (config->direction == OPT_MINIMIZE) {
                delta = cand_cost - current_cost;
            } else {
                delta = current_cost - cand_cost;
            }

            bool accept = false;
            if (delta < 0) {
                accept = true;
            } else if (T > 1e-15) {
                double prob = exp(-delta / T);
                if (opt_random_uniform() < prob) {
                    accept = true;
                }
            }

            if (accept) {
                memcpy(current, candidate, element_size);
                current_cost = cand_cost;
                accepted++;

                if (sa_is_better(current_cost, result.best.cost, config->direction)) {
                    memcpy(result.best.data, current, element_size);
                    result.best.cost = current_cost;
                }
            }

            if (result.convergence != NULL && global_iter < result.convergence_size) {
                result.convergence[global_iter] = result.best.cost;
            }
            global_iter++;
        }

        double acceptance_rate = (chain_len > 0) ?
                                 (double)accepted / (double)chain_len : 0.0;

        if (config->enable_reheating &&
            acceptance_rate < config->reheat_threshold &&
            T < T0 * 0.5) {
            T = T * config->reheat_factor;
            if (T > T0) T = T0;
        } else {
            temp_step++;
            T = sa_cool(T, T0, config->final_temp, config->alpha,
                        config->cooling, temp_step,
                        acceptance_rate,
                        config->adaptive_target_low,
                        config->adaptive_target_high,
                        config->adaptive_factor);
        }
    }

    result.num_iterations = global_iter;

    free(current);
    free(candidate);
    return result;
}
