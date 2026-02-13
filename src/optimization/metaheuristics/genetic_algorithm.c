/**
 * @file genetic_algorithm.c
 * @brief Implementacao do Algoritmo Genetico e operadores builtin
 *
 * GA classico com selecao (tournament/roulette/rank), elitismo,
 * crossover e mutacao genericos, busca local opcional (memetico),
 * e taxas adaptativas.
 *
 * Referencias:
 * - Holland, J. H. (1975). Adaptation in Natural and Artificial Systems
 * - Goldberg, D. E. (1989). Genetic Algorithms in Search, Optimization, and ML
 * - Davis, L. (1985). "Applying Adaptive Algorithms to Epistatic Domains" (OX)
 * - Goldberg, D. E. & Lingle, R. (1985). "Alleles, Loci, and the TSP" (PMX)
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/genetic_algorithm.h"
#include "optimization/benchmarks/continuous.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool ga_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

typedef struct {
    void *data;
    double fitness;
} Individual;

static int cmp_fitness_asc(const void *a, const void *b) {
    double fa = ((const Individual*)a)->fitness;
    double fb = ((const Individual*)b)->fitness;
    if (fa < fb) return -1;
    if (fa > fb) return 1;
    return 0;
}

static int cmp_fitness_desc(const void *a, const void *b) {
    double fa = ((const Individual*)a)->fitness;
    double fb = ((const Individual*)b)->fitness;
    if (fa > fb) return -1;
    if (fa < fb) return 1;
    return 0;
}

// ============================================================================
// SELECTION
// ============================================================================

static size_t select_tournament(const Individual *pop, size_t pop_size,
                                size_t k, OptDirection dir) {
    size_t best = (size_t)opt_random_int(0, (int)pop_size - 1);
    for (size_t i = 1; i < k; i++) {
        size_t idx = (size_t)opt_random_int(0, (int)pop_size - 1);
        if (ga_is_better(pop[idx].fitness, pop[best].fitness, dir)) {
            best = idx;
        }
    }
    return best;
}

static size_t select_roulette(const Individual *pop, size_t pop_size,
                              OptDirection dir) {
    double worst = pop[0].fitness;
    for (size_t i = 1; i < pop_size; i++) {
        if (!ga_is_better(pop[i].fitness, worst, dir)) {
            worst = pop[i].fitness;
        }
    }

    double offset = (dir == OPT_MINIMIZE) ? worst + 1.0 : 0.0;
    double total = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        if (dir == OPT_MINIMIZE) {
            total += offset - pop[i].fitness;
        } else {
            total += pop[i].fitness - offset + 1.0;
        }
    }

    if (total <= 0.0) return (size_t)opt_random_int(0, (int)pop_size - 1);

    double r = opt_random_uniform() * total;
    double acc = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        if (dir == OPT_MINIMIZE) {
            acc += offset - pop[i].fitness;
        } else {
            acc += pop[i].fitness - offset + 1.0;
        }
        if (acc >= r) return i;
    }
    return pop_size - 1;
}

static size_t select_rank(const Individual *sorted_pop, size_t pop_size) {
    (void)sorted_pop;
    double total = (double)pop_size * ((double)pop_size + 1.0) / 2.0;
    double r = opt_random_uniform() * total;
    double acc = 0.0;
    for (size_t i = 0; i < pop_size; i++) {
        acc += (double)(pop_size - i);
        if (acc >= r) return i;
    }
    return pop_size - 1;
}

static size_t ga_select(const Individual *pop, size_t pop_size,
                        const GAConfig *config) {
    switch (config->selection) {
        case GA_SELECT_TOURNAMENT:
            return select_tournament(pop, pop_size, config->tournament_size,
                                     config->direction);
        case GA_SELECT_ROULETTE:
            return select_roulette(pop, pop_size, config->direction);
        case GA_SELECT_RANK:
            return select_rank(pop, pop_size);
        default:
            return select_tournament(pop, pop_size, 3, config->direction);
    }
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

GAConfig ga_default_config(void) {
    GAConfig config;
    config.population_size = 50;
    config.max_generations = 500;
    config.crossover_rate = 0.8;
    config.mutation_rate = 0.05;
    config.elitism_count = 2;

    config.selection = GA_SELECT_TOURNAMENT;
    config.tournament_size = 3;

    config.enable_local_search = false;

    config.enable_adaptive_rates = false;
    config.adaptive_min_mutation = 0.01;
    config.adaptive_max_mutation = 0.3;

    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// CROSSOVER: OX (Order Crossover)
// ============================================================================

void ga_crossover_ox(const void *p1, const void *p2,
                     void *c1, void *c2,
                     size_t size, const void *context) {
    (void)context;
    const int *par1 = (const int*)p1;
    const int *par2 = (const int*)p2;
    int *ch1 = (int*)c1;
    int *ch2 = (int*)c2;
    int n = (int)size;

    int i = opt_random_int(0, n - 2);
    int j = opt_random_int(i + 1, n - 1);

    memset(ch1, -1, n * sizeof(int));
    memset(ch2, -1, n * sizeof(int));

    for (int k = i; k <= j; k++) {
        ch1[k] = par1[k];
        ch2[k] = par2[k];
    }

    int pos1 = (j + 1) % n;
    int pos2 = (j + 1) % n;

    for (int k = 0; k < n; k++) {
        int idx = (j + 1 + k) % n;

        bool found1 = false;
        for (int m = i; m <= j; m++) {
            if (par2[idx] == ch1[m]) { found1 = true; break; }
        }
        if (!found1) {
            ch1[pos1] = par2[idx];
            pos1 = (pos1 + 1) % n;
        }

        bool found2 = false;
        for (int m = i; m <= j; m++) {
            if (par1[idx] == ch2[m]) { found2 = true; break; }
        }
        if (!found2) {
            ch2[pos2] = par1[idx];
            pos2 = (pos2 + 1) % n;
        }
    }
}

// ============================================================================
// CROSSOVER: PMX (Partially Mapped)
// ============================================================================

void ga_crossover_pmx(const void *p1, const void *p2,
                      void *c1, void *c2,
                      size_t size, const void *context) {
    (void)context;
    const int *par1 = (const int*)p1;
    const int *par2 = (const int*)p2;
    int *ch1 = (int*)c1;
    int *ch2 = (int*)c2;
    int n = (int)size;

    memcpy(ch1, par2, n * sizeof(int));
    memcpy(ch2, par1, n * sizeof(int));

    int i = opt_random_int(0, n - 2);
    int j = opt_random_int(i + 1, n - 1);

    for (int k = i; k <= j; k++) {
        int val1 = par1[k];
        int val2 = par2[k];
        if (val1 == val2) continue;

        int pos1 = -1, pos2 = -1;
        for (int m = 0; m < n; m++) {
            if (ch1[m] == val1) pos1 = m;
            if (ch1[m] == val2) pos2 = m;
        }
        if (pos1 >= 0 && pos2 >= 0) {
            ch1[pos1] = val2;
            ch1[pos2] = val1;
        }

        pos1 = -1; pos2 = -1;
        for (int m = 0; m < n; m++) {
            if (ch2[m] == val2) pos1 = m;
            if (ch2[m] == val1) pos2 = m;
        }
        if (pos1 >= 0 && pos2 >= 0) {
            ch2[pos1] = val1;
            ch2[pos2] = val2;
        }
    }
}

// ============================================================================
// CROSSOVER: BLX-alpha (Continuous)
// ============================================================================

void ga_crossover_blx(const void *p1, const void *p2,
                      void *c1, void *c2,
                      size_t size, const void *context) {
    const double *par1 = (const double*)p1;
    const double *par2 = (const double*)p2;
    double *ch1 = (double*)c1;
    double *ch2 = (double*)c2;

    double alpha = 0.5;
    double lower = -1e18, upper = 1e18;

    if (context != NULL) {
        const ContinuousInstance *inst = (const ContinuousInstance*)context;
        lower = inst->lower_bound;
        upper = inst->upper_bound;
    }

    for (size_t i = 0; i < size; i++) {
        double lo = (par1[i] < par2[i]) ? par1[i] : par2[i];
        double hi = (par1[i] > par2[i]) ? par1[i] : par2[i];
        double range = hi - lo;

        double low_bound = lo - alpha * range;
        double high_bound = hi + alpha * range;
        if (low_bound < lower) low_bound = lower;
        if (high_bound > upper) high_bound = upper;

        ch1[i] = low_bound + opt_random_uniform() * (high_bound - low_bound);
        ch2[i] = low_bound + opt_random_uniform() * (high_bound - low_bound);
    }
}

// ============================================================================
// MUTATION: Swap (Permutation)
// ============================================================================

void ga_mutation_swap(void *solution, size_t size,
                      double mutation_rate, const void *context) {
    (void)context;
    if (opt_random_uniform() >= mutation_rate || size < 2) return;

    int *s = (int*)solution;
    int i = opt_random_int(0, (int)size - 1);
    int j = opt_random_int(0, (int)size - 2);
    if (j >= i) j++;

    int tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
}

// ============================================================================
// MUTATION: Inversion (Permutation)
// ============================================================================

void ga_mutation_inversion(void *solution, size_t size,
                           double mutation_rate, const void *context) {
    (void)context;
    if (opt_random_uniform() >= mutation_rate || size < 3) return;

    int *s = (int*)solution;
    int i = opt_random_int(0, (int)size - 2);
    int j = opt_random_int(i + 1, (int)size - 1);

    while (i < j) {
        int tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

// ============================================================================
// MUTATION: Gaussian (Continuous)
// ============================================================================

void ga_mutation_gaussian(void *solution, size_t size,
                          double mutation_rate, const void *context) {
    double *s = (double*)solution;
    double sigma = 0.1;
    double lower = -1e18, upper = 1e18;

    if (context != NULL) {
        const ContinuousInstance *inst = (const ContinuousInstance*)context;
        sigma = inst->neighbor_sigma;
        lower = inst->lower_bound;
        upper = inst->upper_bound;
    }

    for (size_t i = 0; i < size; i++) {
        if (opt_random_uniform() < mutation_rate) {
            s[i] += sigma * opt_random_gaussian();
            if (s[i] < lower) s[i] = lower;
            if (s[i] > upper) s[i] = upper;
        }
    }
}

// ============================================================================
// GA PRINCIPAL
// ============================================================================

OptResult ga_run(const GAConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 GenerateFn generate,
                 CrossoverFn crossover,
                 MutationFn mutate,
                 LocalSearchFn local_search,
                 const void *context) {
    OptResult result = opt_result_create(config->max_generations);
    opt_set_seed(config->seed);

    size_t pop_size = config->population_size;
    if (pop_size < 4) pop_size = 4;
    if (pop_size % 2 != 0) pop_size++;

    Individual *pop = calloc(pop_size, sizeof(Individual));
    Individual *new_pop = calloc(pop_size, sizeof(Individual));

    if (pop == NULL || new_pop == NULL) {
        free(pop);
        free(new_pop);
        return result;
    }

    for (size_t i = 0; i < pop_size; i++) {
        pop[i].data = malloc(element_size);
        new_pop[i].data = malloc(element_size);
        if (pop[i].data == NULL || new_pop[i].data == NULL) {
            for (size_t j = 0; j <= i; j++) {
                free(pop[j].data);
                free(new_pop[j].data);
            }
            free(pop);
            free(new_pop);
            return result;
        }
    }

    for (size_t i = 0; i < pop_size; i++) {
        generate(pop[i].data, solution_size, context);
        pop[i].fitness = objective(pop[i].data, solution_size, context);
        result.num_evaluations++;

        if (config->enable_local_search && local_search != NULL) {
            pop[i].fitness = local_search(pop[i].data, solution_size,
                                          objective, context);
            result.num_evaluations++;
        }
    }

    if (config->direction == OPT_MINIMIZE) {
        qsort(pop, pop_size, sizeof(Individual), cmp_fitness_asc);
    } else {
        qsort(pop, pop_size, sizeof(Individual), cmp_fitness_desc);
    }

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, pop[0].data, element_size);
    result.best.cost = pop[0].fitness;

    void *child1_buf = malloc(element_size);
    void *child2_buf = malloc(element_size);

    double current_mutation = config->mutation_rate;

    for (size_t gen = 0; gen < config->max_generations; gen++) {
        size_t elite = config->elitism_count;
        if (elite > pop_size) elite = pop_size;

        for (size_t i = 0; i < elite; i++) {
            memcpy(new_pop[i].data, pop[i].data, element_size);
            new_pop[i].fitness = pop[i].fitness;
        }

        for (size_t i = elite; i < pop_size; i += 2) {
            size_t p1_idx = ga_select(pop, pop_size, config);
            size_t p2_idx = ga_select(pop, pop_size, config);

            if (opt_random_uniform() < config->crossover_rate) {
                crossover(pop[p1_idx].data, pop[p2_idx].data,
                          child1_buf, child2_buf,
                          solution_size, context);
            } else {
                memcpy(child1_buf, pop[p1_idx].data, element_size);
                memcpy(child2_buf, pop[p2_idx].data, element_size);
            }

            mutate(child1_buf, solution_size, current_mutation, context);
            mutate(child2_buf, solution_size, current_mutation, context);

            memcpy(new_pop[i].data, child1_buf, element_size);
            new_pop[i].fitness = objective(new_pop[i].data, solution_size, context);
            result.num_evaluations++;

            if (config->enable_local_search && local_search != NULL) {
                new_pop[i].fitness = local_search(new_pop[i].data, solution_size,
                                                  objective, context);
                result.num_evaluations++;
            }

            if (i + 1 < pop_size) {
                memcpy(new_pop[i + 1].data, child2_buf, element_size);
                new_pop[i + 1].fitness = objective(new_pop[i + 1].data,
                                                   solution_size, context);
                result.num_evaluations++;

                if (config->enable_local_search && local_search != NULL) {
                    new_pop[i + 1].fitness = local_search(new_pop[i + 1].data,
                                                          solution_size,
                                                          objective, context);
                    result.num_evaluations++;
                }
            }
        }

        Individual *tmp = pop;
        pop = new_pop;
        new_pop = tmp;

        if (config->direction == OPT_MINIMIZE) {
            qsort(pop, pop_size, sizeof(Individual), cmp_fitness_asc);
        } else {
            qsort(pop, pop_size, sizeof(Individual), cmp_fitness_desc);
        }

        if (ga_is_better(pop[0].fitness, result.best.cost, config->direction)) {
            memcpy(result.best.data, pop[0].data, element_size);
            result.best.cost = pop[0].fitness;
        }

        if (config->enable_adaptive_rates) {
            double diversity = 0.0;
            for (size_t i = 1; i < pop_size; i++) {
                diversity += fabs(pop[i].fitness - pop[0].fitness);
            }
            diversity /= (double)pop_size;

            if (diversity < 1e-6) {
                current_mutation = config->adaptive_max_mutation;
            } else {
                double ratio = diversity / (fabs(pop[0].fitness) + 1e-15);
                current_mutation = config->adaptive_min_mutation +
                    (config->adaptive_max_mutation - config->adaptive_min_mutation) *
                    (1.0 / (1.0 + ratio));
            }
        }

        if (result.convergence != NULL && gen < result.convergence_size) {
            result.convergence[gen] = result.best.cost;
        }
        result.num_iterations = gen + 1;
    }

    free(child1_buf);
    free(child2_buf);

    for (size_t i = 0; i < pop_size; i++) {
        free(pop[i].data);
        free(new_pop[i].data);
    }
    free(pop);
    free(new_pop);

    return result;
}
