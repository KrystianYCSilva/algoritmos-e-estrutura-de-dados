/**
 * @file tabu_search.c
 * @brief Implementacao do Tabu Search classico e variantes avancadas
 *
 * Lista tabu solution-based (hash FIFO circular), aspiracao,
 * diversificacao/intensificacao por frequencia, tenure reativo.
 *
 * Referencias:
 * - Glover, F. (1986). "Future Paths for Integer Programming and Links
 *   to Artificial Intelligence". Computers & OR, 13(5), 533-549.
 * - Glover, F. & Laguna, M. (1997). Tabu Search. Kluwer Academic.
 * - Battiti, R. & Tecchiolli, G. (1994). "The Reactive Tabu Search"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/tabu_search.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME        1099511628211ULL

// ============================================================================
// TABU LIST (FIFO circular de hashes)
// ============================================================================

typedef struct {
    uint64_t *hashes;
    size_t capacity;
    size_t head;
    size_t count;
} TabuList;

static TabuList tabu_list_create(size_t capacity) {
    TabuList tl;
    tl.capacity = (capacity > 0) ? capacity : 1;
    tl.hashes = calloc(tl.capacity, sizeof(uint64_t));
    tl.head = 0;
    tl.count = 0;
    return tl;
}

static void tabu_list_destroy(TabuList *tl) {
    free(tl->hashes);
    tl->hashes = NULL;
    tl->capacity = 0;
    tl->count = 0;
}

static void tabu_list_add(TabuList *tl, uint64_t hash) {
    tl->hashes[tl->head] = hash;
    tl->head = (tl->head + 1) % tl->capacity;
    if (tl->count < tl->capacity) tl->count++;
}

static bool tabu_list_contains(const TabuList *tl, uint64_t hash) {
    for (size_t i = 0; i < tl->count; i++) {
        if (tl->hashes[i] == hash) return true;
    }
    return false;
}

static void tabu_list_resize(TabuList *tl, size_t new_cap) {
    if (new_cap == 0) new_cap = 1;
    if (new_cap == tl->capacity) return;

    uint64_t *new_hashes = calloc(new_cap, sizeof(uint64_t));
    if (new_hashes == NULL) return;

    size_t copy_count = (tl->count < new_cap) ? tl->count : new_cap;
    size_t start = 0;
    if (tl->count > copy_count) {
        start = tl->count - copy_count;
    }

    for (size_t i = 0; i < copy_count; i++) {
        size_t idx = (tl->head + tl->capacity - tl->count + start + i) % tl->capacity;
        new_hashes[i] = tl->hashes[idx];
    }

    free(tl->hashes);
    tl->hashes = new_hashes;
    tl->capacity = new_cap;
    tl->head = copy_count % new_cap;
    tl->count = copy_count;
}

// ============================================================================
// FREQUENCY MEMORY (para diversificacao/intensificacao)
// ============================================================================

typedef struct {
    uint64_t *sol_hashes;
    size_t *frequencies;
    size_t capacity;
    size_t count;
} FrequencyMemory;

static FrequencyMemory freq_create(size_t capacity) {
    FrequencyMemory fm;
    fm.capacity = (capacity > 0) ? capacity : 256;
    fm.sol_hashes = calloc(fm.capacity, sizeof(uint64_t));
    fm.frequencies = calloc(fm.capacity, sizeof(size_t));
    fm.count = 0;
    return fm;
}

static void freq_destroy(FrequencyMemory *fm) {
    free(fm->sol_hashes);
    free(fm->frequencies);
    fm->sol_hashes = NULL;
    fm->frequencies = NULL;
    fm->count = 0;
}

static size_t freq_get(const FrequencyMemory *fm, uint64_t hash) {
    for (size_t i = 0; i < fm->count; i++) {
        if (fm->sol_hashes[i] == hash) return fm->frequencies[i];
    }
    return 0;
}

static void freq_increment(FrequencyMemory *fm, uint64_t hash) {
    for (size_t i = 0; i < fm->count; i++) {
        if (fm->sol_hashes[i] == hash) {
            fm->frequencies[i]++;
            return;
        }
    }

    if (fm->count < fm->capacity) {
        fm->sol_hashes[fm->count] = hash;
        fm->frequencies[fm->count] = 1;
        fm->count++;
    } else {
        size_t min_idx = 0;
        for (size_t i = 1; i < fm->capacity; i++) {
            if (fm->frequencies[i] < fm->frequencies[min_idx]) {
                min_idx = i;
            }
        }
        fm->sol_hashes[min_idx] = hash;
        fm->frequencies[min_idx] = 1;
    }
}

// ============================================================================
// HELPERS
// ============================================================================

static bool ts_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

TSConfig ts_default_config(void) {
    TSConfig config;
    config.max_iterations = 5000;
    config.neighbors_per_iter = 20;
    config.tabu_tenure = 15;

    config.enable_aspiration = true;

    config.enable_diversification = false;
    config.diversification_weight = 0.1;
    config.diversification_trigger = 100;

    config.enable_intensification = false;
    config.intensification_trigger = 50;

    config.enable_reactive_tenure = false;
    config.reactive_increase = 5;
    config.reactive_decrease = 1;
    config.min_tenure = 5;
    config.max_tenure = 50;

    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// HASH FUNCTIONS
// ============================================================================

uint64_t ts_hash_bytes(const void *solution_data, size_t size) {
    const unsigned char *bytes = (const unsigned char*)solution_data;
    uint64_t hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

uint64_t ts_hash_int_array(const void *solution_data, size_t size) {
    const int *arr = (const int*)solution_data;
    uint64_t hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < size; i++) {
        uint64_t val = (uint64_t)(unsigned int)arr[i];
        hash ^= val;
        hash *= FNV_PRIME;
    }
    return hash;
}

uint64_t ts_hash_double_array(const void *solution_data, size_t size) {
    const double *arr = (const double*)solution_data;
    uint64_t hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < size; i++) {
        int64_t discretized = (int64_t)(arr[i] * 10000.0);
        uint64_t val = (uint64_t)discretized;
        hash ^= val;
        hash *= FNV_PRIME;
    }
    return hash;
}

// ============================================================================
// TABU SEARCH PRINCIPAL
// ============================================================================

OptResult ts_run(const TSConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 NeighborFn neighbor,
                 GenerateFn generate,
                 TabuHashFn hash_fn,
                 const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    if (hash_fn == NULL) {
        hash_fn = ts_hash_bytes;
    }

    void *current = malloc(element_size);
    void *candidate = malloc(element_size);
    void *best_candidate = malloc(element_size);

    if (current == NULL || candidate == NULL || best_candidate == NULL) {
        free(current);
        free(candidate);
        free(best_candidate);
        return result;
    }

    generate(current, solution_size, context);
    double current_cost = objective(current, solution_size, context);
    result.num_evaluations = 1;

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, current, element_size);
    result.best.cost = current_cost;

    TabuList tabu = tabu_list_create(config->tabu_tenure);
    tabu_list_add(&tabu, hash_fn(current, solution_size));

    FrequencyMemory freq;
    bool use_freq = config->enable_diversification || config->enable_intensification;
    if (use_freq) {
        freq = freq_create(config->max_iterations < 4096 ? config->max_iterations : 4096);
        freq_increment(&freq, hash_fn(current, solution_size));
    }

    size_t iters_without_improvement = 0;
    size_t current_tenure = config->tabu_tenure;
    uint64_t prev_hash = hash_fn(current, solution_size);

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        double best_cand_cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;
        bool found_any = false;
        uint64_t best_cand_hash = 0;

        for (size_t k = 0; k < config->neighbors_per_iter; k++) {
            neighbor(current, candidate, solution_size, context);
            double cand_cost = objective(candidate, solution_size, context);
            result.num_evaluations++;

            uint64_t cand_hash = hash_fn(candidate, solution_size);
            bool is_tabu = tabu_list_contains(&tabu, cand_hash);

            bool aspiration = false;
            if (is_tabu && config->enable_aspiration) {
                if (ts_is_better(cand_cost, result.best.cost, config->direction)) {
                    aspiration = true;
                }
            }

            if (!is_tabu || aspiration) {
                double eval_cost = cand_cost;

                if (config->enable_diversification && use_freq) {
                    size_t f = freq_get(&freq, cand_hash);
                    if (config->direction == OPT_MINIMIZE) {
                        eval_cost += config->diversification_weight * (double)f;
                    } else {
                        eval_cost -= config->diversification_weight * (double)f;
                    }
                }

                if (!found_any || ts_is_better(eval_cost, best_cand_cost, config->direction)) {
                    best_cand_cost = eval_cost;
                    memcpy(best_candidate, candidate, element_size);
                    best_cand_hash = cand_hash;
                    found_any = true;
                }
            }
        }

        if (!found_any) {
            if (result.convergence != NULL && iter < result.convergence_size) {
                result.convergence[iter] = result.best.cost;
            }
            result.num_iterations = iter + 1;
            continue;
        }

        memcpy(current, best_candidate, element_size);
        current_cost = objective(current, solution_size, context);
        result.num_evaluations++;

        tabu_list_add(&tabu, best_cand_hash);
        if (use_freq) {
            freq_increment(&freq, best_cand_hash);
        }

        if (ts_is_better(current_cost, result.best.cost, config->direction)) {
            memcpy(result.best.data, current, element_size);
            result.best.cost = current_cost;
            iters_without_improvement = 0;
        } else {
            iters_without_improvement++;
        }

        if (config->enable_reactive_tenure) {
            if (best_cand_hash == prev_hash) {
                current_tenure += config->reactive_increase;
                if (current_tenure > config->max_tenure) {
                    current_tenure = config->max_tenure;
                }
            } else if (iters_without_improvement == 0) {
                if (current_tenure > config->reactive_decrease + config->min_tenure) {
                    current_tenure -= config->reactive_decrease;
                } else {
                    current_tenure = config->min_tenure;
                }
            }
            tabu_list_resize(&tabu, current_tenure);
        }

        prev_hash = best_cand_hash;

        if (config->enable_intensification &&
            iters_without_improvement == config->intensification_trigger) {
            memcpy(current, result.best.data, element_size);
            current_cost = result.best.cost;
        }

        if (config->enable_diversification &&
            iters_without_improvement == config->diversification_trigger) {
            generate(current, solution_size, context);
            current_cost = objective(current, solution_size, context);
            result.num_evaluations++;
            iters_without_improvement = 0;
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    tabu_list_destroy(&tabu);
    if (use_freq) {
        freq_destroy(&freq);
    }

    free(current);
    free(candidate);
    free(best_candidate);
    return result;
}
