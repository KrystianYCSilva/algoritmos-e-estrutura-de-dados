/**
 * @file lns.c
 * @brief Implementacao do Large Neighborhood Search (LNS) e ALNS
 *
 * Referencias:
 * - Shaw, P. (1998). "Using Constraint Programming and Local Search Methods
 *   to Solve Vehicle Routing Problems".
 * - Ropke, S. & Pisinger, D. (2006). "An Adaptive Large Neighborhood Search
 *   Heuristic for the Pickup and Delivery Problem with Time Windows".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/lns.h"
#include "optimization/benchmarks/tsp.h"
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

static int sa_accept(double current_cost, double new_cost, double temp,
                     OptDirection direction) {
    double delta;
    if (direction == OPT_MINIMIZE) {
        delta = new_cost - current_cost;
    } else {
        delta = current_cost - new_cost;
    }
    if (delta <= 0.0) return 1;
    if (temp <= 0.0) return 0;
    double prob = exp(-delta / temp);
    return opt_random_uniform() < prob;
}

static size_t roulette_select_weighted(const double *weights, size_t n) {
    double total = 0.0;
    for (size_t i = 0; i < n; i++) total += weights[i];
    if (total <= 0.0) return (size_t)opt_random_int(0, (int)n - 1);

    double r = opt_random_uniform() * total;
    double cum = 0.0;
    for (size_t i = 0; i < n; i++) {
        cum += weights[i];
        if (cum >= r) return i;
    }
    return n - 1;
}

// ============================================================================
// DESTROY/REPAIR BUILTIN - TSP
// ============================================================================

void lns_destroy_tsp_random(const void *solution, void *destroyed, size_t size,
                            double degree, const void *context) {
    (void)context;
    int n = (int)size;
    memcpy(destroyed, solution, (size_t)n * sizeof(int));
    int *d = (int *)destroyed;

    int to_remove = (int)(degree * n);
    if (to_remove < 1) to_remove = 1;
    if (to_remove >= n) to_remove = n - 1;

    for (int r = 0; r < to_remove; r++) {
        int idx = opt_random_int(0, n - 1);
        while (d[idx] == -1) {
            idx = (idx + 1) % n;
        }
        d[idx] = -1;
    }
}

void lns_destroy_tsp_worst(const void *solution, void *destroyed, size_t size,
                           double degree, const void *context) {
    const TSPInstance *tsp = (const TSPInstance *)context;
    int n = (int)size;
    memcpy(destroyed, solution, (size_t)n * sizeof(int));
    int *d = (int *)destroyed;
    const int *tour = (const int *)solution;

    int to_remove = (int)(degree * n);
    if (to_remove < 1) to_remove = 1;
    if (to_remove >= n) to_remove = n - 1;

    double *costs = malloc((size_t)n * sizeof(double));
    if (costs == NULL) return;

    for (int i = 0; i < n; i++) {
        int prev = tour[(i - 1 + n) % n];
        int curr = tour[i];
        int next = tour[(i + 1) % n];
        costs[i] = tsp->dist_matrix[prev][curr] + tsp->dist_matrix[curr][next];
    }

    for (int r = 0; r < to_remove; r++) {
        int worst = -1;
        double worst_cost = -1.0;
        for (int i = 0; i < n; i++) {
            if (d[i] != -1 && costs[i] > worst_cost) {
                worst_cost = costs[i];
                worst = i;
            }
        }
        if (worst >= 0) {
            d[worst] = -1;
            costs[worst] = -1.0;
        }
    }

    free(costs);
}

void lns_repair_tsp_greedy(const void *destroyed, void *repaired, size_t size,
                           const void *context) {
    const TSPInstance *tsp = (const TSPInstance *)context;
    int n = (int)size;
    const int *d = (const int *)destroyed;

    int *removed = malloc((size_t)n * sizeof(int));
    int *partial = malloc((size_t)n * sizeof(int));
    if (removed == NULL || partial == NULL) {
        free(removed);
        free(partial);
        memcpy(repaired, destroyed, (size_t)n * sizeof(int));
        return;
    }

    int n_removed = 0;
    int n_partial = 0;
    for (int i = 0; i < n; i++) {
        if (d[i] == -1) {
            removed[n_removed++] = i;
        } else {
            partial[n_partial++] = d[i];
        }
    }

    int *present = malloc((size_t)n * sizeof(int));
    if (present == NULL) {
        memcpy(repaired, destroyed, (size_t)n * sizeof(int));
        free(removed);
        free(partial);
        return;
    }

    memset(present, 0, (size_t)n * sizeof(int));
    for (int i = 0; i < n_partial; i++) {
        present[partial[i]] = 1;
    }

    int actual_removed = 0;
    for (int i = 0; i < n; i++) {
        if (!present[i]) {
            removed[actual_removed++] = i;
        }
    }

    for (int r = 0; r < actual_removed; r++) {
        int city = removed[r];
        int best_pos = n_partial;
        double best_cost_increase = DBL_MAX;

        if (n_partial == 0) {
            partial[0] = city;
            n_partial = 1;
            continue;
        }

        for (int p = 0; p <= n_partial; p++) {
            int prev_city, next_city;
            if (p == 0) {
                prev_city = partial[n_partial - 1];
                next_city = partial[0];
            } else if (p == n_partial) {
                prev_city = partial[n_partial - 1];
                next_city = partial[0];
            } else {
                prev_city = partial[p - 1];
                next_city = partial[p];
            }

            double old_cost = tsp->dist_matrix[prev_city][next_city];
            double new_cost = tsp->dist_matrix[prev_city][city] +
                              tsp->dist_matrix[city][next_city];
            double increase = new_cost - old_cost;

            if (increase < best_cost_increase) {
                best_cost_increase = increase;
                best_pos = p;
            }
        }

        for (int i = n_partial; i > best_pos; i--) {
            partial[i] = partial[i - 1];
        }
        partial[best_pos] = city;
        n_partial++;
    }

    memcpy(repaired, partial, (size_t)n * sizeof(int));

    free(removed);
    free(partial);
    free(present);
}

void lns_repair_tsp_random(const void *destroyed, void *repaired, size_t size,
                           const void *context) {
    (void)context;
    int n = (int)size;
    const int *d = (const int *)destroyed;

    int *partial = malloc((size_t)n * sizeof(int));
    if (partial == NULL) {
        memcpy(repaired, destroyed, (size_t)n * sizeof(int));
        return;
    }

    int *present = malloc((size_t)n * sizeof(int));
    int *missing = malloc((size_t)n * sizeof(int));
    if (present == NULL || missing == NULL) {
        free(partial);
        free(present);
        free(missing);
        memcpy(repaired, destroyed, (size_t)n * sizeof(int));
        return;
    }

    memset(present, 0, (size_t)n * sizeof(int));
    int n_partial = 0;
    for (int i = 0; i < n; i++) {
        if (d[i] != -1) {
            partial[n_partial++] = d[i];
            present[d[i]] = 1;
        }
    }

    int n_missing = 0;
    for (int i = 0; i < n; i++) {
        if (!present[i]) {
            missing[n_missing++] = i;
        }
    }

    for (int i = n_missing - 1; i > 0; i--) {
        int j = opt_random_int(0, i);
        int tmp = missing[i];
        missing[i] = missing[j];
        missing[j] = tmp;
    }

    for (int r = 0; r < n_missing; r++) {
        int pos = opt_random_int(0, n_partial);
        for (int i = n_partial; i > pos; i--) {
            partial[i] = partial[i - 1];
        }
        partial[pos] = missing[r];
        n_partial++;
    }

    memcpy(repaired, partial, (size_t)n * sizeof(int));

    free(partial);
    free(present);
    free(missing);
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

LNSConfig lns_default_config(void) {
    LNSConfig config;
    config.max_iterations = 1000;
    config.destroy_degree = 0.3;
    config.variant = LNS_BASIC;
    config.acceptance = LNS_ACCEPT_BETTER;
    config.sa_initial_temp = 100.0;
    config.sa_alpha = 0.99;
    config.num_destroy_ops = 0;
    config.num_repair_ops = 0;
    config.reward_best = 10.0;
    config.reward_better = 5.0;
    config.reward_accepted = 1.0;
    config.weight_update_interval = 50;
    config.weight_decay = 0.8;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// LNS BASICO
// ============================================================================

OptResult lns_run(const LNSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  GenerateFn generate,
                  DestroyFn destroy,
                  RepairFn repair,
                  const void *context) {
    if (config == NULL || objective == NULL || generate == NULL ||
        destroy == NULL || repair == NULL) {
        OptResult empty = {0};
        return empty;
    }

    opt_set_seed(config->seed);

    size_t data_size = element_size * solution_size;
    OptResult result = opt_result_create(config->max_iterations);
    result.num_iterations = 0;
    result.num_evaluations = 0;

    void *current = malloc(data_size);
    void *destroyed = malloc(data_size);
    void *repaired = malloc(data_size);
    void *best_data = malloc(data_size);
    if (current == NULL || destroyed == NULL || repaired == NULL || best_data == NULL) {
        free(current);
        free(destroyed);
        free(repaired);
        free(best_data);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations++;

    double best_cost = current_cost;
    memcpy(best_data, current, data_size);

    double temp = config->sa_initial_temp;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        destroy(current, destroyed, solution_size, config->destroy_degree, context);
        repair(destroyed, repaired, solution_size, context);

        double repaired_cost = objective(repaired, solution_size, context);
        result.num_evaluations++;

        int accepted = 0;
        if (config->acceptance == LNS_ACCEPT_SA_LIKE) {
            accepted = sa_accept(current_cost, repaired_cost, temp, config->direction);
            temp *= config->sa_alpha;
        } else {
            accepted = is_better(repaired_cost, current_cost, config->direction);
        }

        if (accepted) {
            memcpy(current, repaired, data_size);
            current_cost = repaired_cost;

            if (is_better(current_cost, best_cost, config->direction)) {
                memcpy(best_data, current, data_size);
                best_cost = current_cost;
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
    free(destroyed);
    free(repaired);
    free(best_data);

    return result;
}

// ============================================================================
// ALNS (ADAPTIVE)
// ============================================================================

OptResult alns_run(const LNSConfig *config,
                   size_t element_size,
                   size_t solution_size,
                   ObjectiveFn objective,
                   GenerateFn generate,
                   const DestroyFn *destroy_ops,
                   const RepairFn *repair_ops,
                   const void *context) {
    if (config == NULL || objective == NULL || generate == NULL ||
        destroy_ops == NULL || repair_ops == NULL ||
        config->num_destroy_ops == 0 || config->num_repair_ops == 0) {
        OptResult empty = {0};
        return empty;
    }

    opt_set_seed(config->seed);

    size_t nd = config->num_destroy_ops;
    size_t nr = config->num_repair_ops;
    size_t data_size = element_size * solution_size;

    OptResult result = opt_result_create(config->max_iterations);
    result.num_iterations = 0;
    result.num_evaluations = 0;

    double *weights_d = malloc(nd * sizeof(double));
    double *weights_r = malloc(nr * sizeof(double));
    double *scores_d = calloc(nd, sizeof(double));
    double *scores_r = calloc(nr, sizeof(double));
    size_t *usage_d = calloc(nd, sizeof(size_t));
    size_t *usage_r = calloc(nr, sizeof(size_t));
    if (weights_d == NULL || weights_r == NULL || scores_d == NULL ||
        scores_r == NULL || usage_d == NULL || usage_r == NULL) {
        free(weights_d); free(weights_r);
        free(scores_d); free(scores_r);
        free(usage_d); free(usage_r);
        return result;
    }

    for (size_t i = 0; i < nd; i++) weights_d[i] = 1.0;
    for (size_t i = 0; i < nr; i++) weights_r[i] = 1.0;

    void *current = malloc(data_size);
    void *destroyed = malloc(data_size);
    void *repaired = malloc(data_size);
    void *best_data = malloc(data_size);
    if (current == NULL || destroyed == NULL || repaired == NULL || best_data == NULL) {
        free(current); free(destroyed); free(repaired); free(best_data);
        free(weights_d); free(weights_r);
        free(scores_d); free(scores_r);
        free(usage_d); free(usage_r);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations++;

    double best_cost = current_cost;
    memcpy(best_data, current, data_size);

    double temp = config->sa_initial_temp;

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        size_t d_idx = roulette_select_weighted(weights_d, nd);
        size_t r_idx = roulette_select_weighted(weights_r, nr);

        destroy_ops[d_idx](current, destroyed, solution_size, config->destroy_degree, context);
        repair_ops[r_idx](destroyed, repaired, solution_size, context);

        double repaired_cost = objective(repaired, solution_size, context);
        result.num_evaluations++;

        usage_d[d_idx]++;
        usage_r[r_idx]++;

        int accepted = 0;
        if (config->acceptance == LNS_ACCEPT_SA_LIKE) {
            accepted = sa_accept(current_cost, repaired_cost, temp, config->direction);
            temp *= config->sa_alpha;
        } else {
            accepted = is_better(repaired_cost, current_cost, config->direction);
        }

        if (is_better(repaired_cost, best_cost, config->direction)) {
            scores_d[d_idx] += config->reward_best;
            scores_r[r_idx] += config->reward_best;
        } else if (is_better(repaired_cost, current_cost, config->direction)) {
            scores_d[d_idx] += config->reward_better;
            scores_r[r_idx] += config->reward_better;
        } else if (accepted) {
            scores_d[d_idx] += config->reward_accepted;
            scores_r[r_idx] += config->reward_accepted;
        }

        if (accepted) {
            memcpy(current, repaired, data_size);
            current_cost = repaired_cost;

            if (is_better(current_cost, best_cost, config->direction)) {
                memcpy(best_data, current, data_size);
                best_cost = current_cost;
            }
        }

        if (config->weight_update_interval > 0 &&
            (iter + 1) % config->weight_update_interval == 0) {
            for (size_t i = 0; i < nd; i++) {
                if (usage_d[i] > 0) {
                    weights_d[i] = config->weight_decay * weights_d[i] +
                                   (1.0 - config->weight_decay) * (scores_d[i] / (double)usage_d[i]);
                }
                if (weights_d[i] < 0.01) weights_d[i] = 0.01;
                scores_d[i] = 0.0;
                usage_d[i] = 0;
            }
            for (size_t i = 0; i < nr; i++) {
                if (usage_r[i] > 0) {
                    weights_r[i] = config->weight_decay * weights_r[i] +
                                   (1.0 - config->weight_decay) * (scores_r[i] / (double)usage_r[i]);
                }
                if (weights_r[i] < 0.01) weights_r[i] = 0.01;
                scores_r[i] = 0.0;
                usage_r[i] = 0;
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

    free(current); free(destroyed); free(repaired); free(best_data);
    free(weights_d); free(weights_r);
    free(scores_d); free(scores_r);
    free(usage_d); free(usage_r);

    return result;
}
