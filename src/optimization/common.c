/**
 * @file common.c
 * @brief Implementacao da infraestrutura generica para otimizacao
 *
 * Implementa criacao/destruicao de OptSolution e OptResult,
 * e utilitarios de geracao de numeros aleatorios (RNG) usando
 * estado global com srand/rand e Box-Muller para gaussianos.
 *
 * Referencias:
 * - Box, G. E. P. & Muller, M. E. (1958). "A Note on the Generation
 *   of Random Normal Deviates"
 * - Knuth, D. E. (1997). TAOCP Vol. 2, Ch. 3 - Random Numbers
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/common.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================================
// SOLUCAO
// ============================================================================

OptSolution opt_solution_create(size_t data_size) {
    OptSolution sol;
    sol.data_size = data_size;
    sol.cost = DBL_MAX;
    sol.data = NULL;

    if (data_size > 0) {
        sol.data = calloc(1, data_size);
    }

    return sol;
}

OptSolution opt_solution_clone(const OptSolution *src) {
    OptSolution clone;
    clone.data = NULL;
    clone.data_size = 0;
    clone.cost = DBL_MAX;

    if (src == NULL) return clone;

    clone.data_size = src->data_size;
    clone.cost = src->cost;

    if (src->data_size > 0 && src->data != NULL) {
        clone.data = malloc(src->data_size);
        if (clone.data != NULL) {
            memcpy(clone.data, src->data, src->data_size);
        } else {
            clone.data_size = 0;
        }
    }

    return clone;
}

void opt_solution_destroy(OptSolution *sol) {
    if (sol == NULL) return;
    free(sol->data);
    sol->data = NULL;
    sol->data_size = 0;
    sol->cost = DBL_MAX;
}

// ============================================================================
// RESULTADO
// ============================================================================

OptResult opt_result_create(size_t max_iterations) {
    OptResult result;
    memset(&result, 0, sizeof(OptResult));

    result.convergence_size = max_iterations;
    if (max_iterations > 0) {
        result.convergence = calloc(max_iterations, sizeof(double));
    }

    return result;
}

void opt_result_destroy(OptResult *result) {
    if (result == NULL) return;
    opt_solution_destroy(&result->best);
    free(result->convergence);
    result->convergence = NULL;
    result->convergence_size = 0;
}

// ============================================================================
// RNG
// ============================================================================

static int g_rng_has_spare = 0;
static double g_rng_spare = 0.0;

void opt_set_seed(unsigned seed) {
    srand(seed);
    g_rng_has_spare = 0;
}

double opt_random_uniform(void) {
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

int opt_random_int(int min, int max) {
    if (min >= max) return min;
    return min + (int)(opt_random_uniform() * (max - min + 1));
}

double opt_random_gaussian(void) {
    if (g_rng_has_spare) {
        g_rng_has_spare = 0;
        return g_rng_spare;
    }

    double u, v, s;
    do {
        u = opt_random_uniform() * 2.0 - 1.0;
        v = opt_random_uniform() * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    double mul = sqrt(-2.0 * log(s) / s);
    g_rng_spare = v * mul;
    g_rng_has_spare = 1;
    return u * mul;
}
