/**
 * @file aco.c
 * @brief Implementacao do Ant Colony Optimization (ACO)
 *
 * ACO com variantes AS, Elitist AS e MAX-MIN AS.
 * Solucoes construidas probabilisticamente usando feromonio + heuristica.
 *
 * Referencias:
 * - Dorigo, M. & Stutzle, T. (2004). Ant Colony Optimization. MIT Press.
 * - Stutzle, T. & Hoos, H. H. (2000). "MAX-MIN Ant System".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/aco.h"
#include "optimization/benchmarks/tsp.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool aco_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static void construct_solution(int *tour, size_t n, double **tau,
                               ACOHeuristicFn heuristic, double alpha,
                               double beta, const void *context) {
    bool *visited = calloc(n, sizeof(bool));
    if (visited == NULL) {
        for (size_t i = 0; i < n; i++) tour[i] = (int)i;
        return;
    }

    int start = opt_random_int(0, (int)(n - 1));
    tour[0] = start;
    visited[start] = true;

    double *probs = malloc(n * sizeof(double));
    if (probs == NULL) {
        for (size_t i = 1; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (!visited[j]) { tour[i] = (int)j; visited[j] = true; break; }
            }
        }
        free(visited);
        return;
    }

    for (size_t step = 1; step < n; step++) {
        int current = tour[step - 1];
        double total = 0;

        for (size_t j = 0; j < n; j++) {
            if (visited[j]) {
                probs[j] = 0;
            } else {
                double t = pow(tau[current][j], alpha);
                double h = pow(heuristic((size_t)current, j, context), beta);
                probs[j] = t * h;
                total += probs[j];
            }
        }

        int chosen = -1;
        if (total > 1e-15) {
            double r = opt_random_uniform() * total;
            double cum = 0;
            for (size_t j = 0; j < n; j++) {
                if (probs[j] > 0) {
                    cum += probs[j];
                    if (cum >= r) { chosen = (int)j; break; }
                }
            }
        }

        if (chosen < 0) {
            for (size_t j = 0; j < n; j++) {
                if (!visited[j]) { chosen = (int)j; break; }
            }
        }

        tour[step] = chosen;
        visited[chosen] = true;
    }

    free(probs);
    free(visited);
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

ACOConfig aco_default_config(void) {
    ACOConfig config;
    config.n_ants = 20;
    config.max_iterations = 500;
    config.alpha = 1.0;
    config.beta = 3.0;
    config.rho = 0.1;
    config.q = 1.0;
    config.tau_0 = 0.1;
    config.variant = ACO_ANT_SYSTEM;
    config.elitist_weight = 2.0;
    config.tau_min = 0.001;
    config.tau_max = 10.0;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// ACO PRINCIPAL
// ============================================================================

OptResult aco_run(const ACOConfig *config,
                  size_t n_nodes,
                  ObjectiveFn objective,
                  ACOHeuristicFn heuristic,
                  const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    size_t n = n_nodes;
    size_t tour_bytes = n * sizeof(int);

    double **tau = malloc(n * sizeof(double*));
    if (tau == NULL) return result;
    for (size_t i = 0; i < n; i++) {
        tau[i] = malloc(n * sizeof(double));
        if (tau[i] == NULL) {
            for (size_t k = 0; k < i; k++) free(tau[k]);
            free(tau);
            return result;
        }
        for (size_t j = 0; j < n; j++) {
            tau[i][j] = config->tau_0;
        }
    }

    int **ant_tours = malloc(config->n_ants * sizeof(int*));
    double *ant_costs = malloc(config->n_ants * sizeof(double));
    if (ant_tours == NULL || ant_costs == NULL) {
        for (size_t i = 0; i < n; i++) free(tau[i]);
        free(tau);
        free(ant_tours);
        free(ant_costs);
        return result;
    }
    for (size_t k = 0; k < config->n_ants; k++) {
        ant_tours[k] = malloc(tour_bytes);
        if (ant_tours[k] == NULL) {
            for (size_t j = 0; j < k; j++) free(ant_tours[j]);
            for (size_t i = 0; i < n; i++) free(tau[i]);
            free(tau);
            free(ant_tours);
            free(ant_costs);
            return result;
        }
    }

    result.best = opt_solution_create(tour_bytes);
    result.best.cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        size_t best_ant = 0;
        double best_ant_cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

        for (size_t k = 0; k < config->n_ants; k++) {
            construct_solution(ant_tours[k], n, tau, heuristic,
                               config->alpha, config->beta, context);
            ant_costs[k] = objective(ant_tours[k], n, context);
            result.num_evaluations++;

            if (aco_is_better(ant_costs[k], best_ant_cost, config->direction)) {
                best_ant_cost = ant_costs[k];
                best_ant = k;
            }
        }

        if (aco_is_better(best_ant_cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, ant_tours[best_ant], tour_bytes);
            result.best.cost = best_ant_cost;
        }

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                tau[i][j] *= (1.0 - config->rho);
            }
        }

        switch (config->variant) {
            case ACO_ANT_SYSTEM:
                for (size_t k = 0; k < config->n_ants; k++) {
                    double deposit = config->q / (ant_costs[k] > 1e-15 ? ant_costs[k] : 1e-15);
                    for (size_t s = 0; s < n; s++) {
                        int from = ant_tours[k][s];
                        int to = ant_tours[k][(s + 1) % n];
                        tau[from][to] += deposit;
                        tau[to][from] += deposit;
                    }
                }
                break;

            case ACO_ELITIST: {
                for (size_t k = 0; k < config->n_ants; k++) {
                    double deposit = config->q / (ant_costs[k] > 1e-15 ? ant_costs[k] : 1e-15);
                    for (size_t s = 0; s < n; s++) {
                        int from = ant_tours[k][s];
                        int to = ant_tours[k][(s + 1) % n];
                        tau[from][to] += deposit;
                        tau[to][from] += deposit;
                    }
                }
                double elite_deposit = config->elitist_weight * config->q
                                     / (result.best.cost > 1e-15 ? result.best.cost : 1e-15);
                int *best_tour = (int*)result.best.data;
                for (size_t s = 0; s < n; s++) {
                    int from = best_tour[s];
                    int to = best_tour[(s + 1) % n];
                    tau[from][to] += elite_deposit;
                    tau[to][from] += elite_deposit;
                }
                break;
            }

            case ACO_MAX_MIN: {
                int *depositor;
                double dep_cost;
                if (iter % 5 == 0) {
                    depositor = (int*)result.best.data;
                    dep_cost = result.best.cost;
                } else {
                    depositor = ant_tours[best_ant];
                    dep_cost = best_ant_cost;
                }
                double deposit = config->q / (dep_cost > 1e-15 ? dep_cost : 1e-15);
                for (size_t s = 0; s < n; s++) {
                    int from = depositor[s];
                    int to = depositor[(s + 1) % n];
                    tau[from][to] += deposit;
                    tau[to][from] += deposit;
                }
                for (size_t i = 0; i < n; i++) {
                    for (size_t j = 0; j < n; j++) {
                        if (tau[i][j] < config->tau_min) tau[i][j] = config->tau_min;
                        if (tau[i][j] > config->tau_max) tau[i][j] = config->tau_max;
                    }
                }
                break;
            }
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    for (size_t k = 0; k < config->n_ants; k++) free(ant_tours[k]);
    free(ant_tours);
    free(ant_costs);
    for (size_t i = 0; i < n; i++) free(tau[i]);
    free(tau);
    return result;
}

// ============================================================================
// HEURISTICAS BUILTIN
// ============================================================================

double aco_heuristic_tsp(size_t i, size_t j, const void *context) {
    const TSPInstance *tsp = (const TSPInstance *)context;
    double d = tsp->dist_matrix[i][j];
    return (d > 1e-12) ? 1.0 / d : 1e12;
}
