/**
 * @file continuous.c
 * @brief Implementacao das funcoes benchmark continuas para otimizacao
 *
 * Implementa Sphere, Rastrigin, Rosenbrock, Ackley e Schwefel,
 * com vizinhanca gaussiana e geracao aleatoria uniforme no dominio.
 *
 * Referencias:
 * - Jamil, M. & Yang, X.-S. (2013). "A Literature Survey of Benchmark
 *   Functions for Global Optimization Problems"
 * - Molga, M. & Smutnicki, C. (2005). "Test functions for optimization needs"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/benchmarks/continuous.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// ============================================================================
// CRIACAO DE INSTANCIAS
// ============================================================================

static ContinuousInstance* continuous_alloc(ContinuousFnType fn_type, size_t dimensions,
                                            double lower, double upper,
                                            double optimum, double sigma) {
    ContinuousInstance *inst = calloc(1, sizeof(ContinuousInstance));
    if (inst == NULL) return NULL;

    inst->fn_type = fn_type;
    inst->dimensions = dimensions;
    inst->lower_bound = lower;
    inst->upper_bound = upper;
    inst->known_optimum = optimum;
    inst->neighbor_sigma = sigma;

    return inst;
}

ContinuousInstance* continuous_create_sphere(size_t dimensions) {
    return continuous_alloc(CONTINUOUS_SPHERE, dimensions, -5.12, 5.12, 0.0, 0.1);
}

ContinuousInstance* continuous_create_rastrigin(size_t dimensions) {
    return continuous_alloc(CONTINUOUS_RASTRIGIN, dimensions, -5.12, 5.12, 0.0, 0.1);
}

ContinuousInstance* continuous_create_rosenbrock(size_t dimensions) {
    return continuous_alloc(CONTINUOUS_ROSENBROCK, dimensions, -5.0, 10.0, 0.0, 0.1);
}

ContinuousInstance* continuous_create_ackley(size_t dimensions) {
    return continuous_alloc(CONTINUOUS_ACKLEY, dimensions, -32.768, 32.768, 0.0, 0.5);
}

ContinuousInstance* continuous_create_schwefel(size_t dimensions) {
    return continuous_alloc(CONTINUOUS_SCHWEFEL, dimensions, -500.0, 500.0, 0.0, 5.0);
}

void continuous_instance_destroy(ContinuousInstance *inst) {
    free(inst);
}

// ============================================================================
// FUNCOES OBJETIVO
// ============================================================================

double continuous_sphere(const double *x, size_t d) {
    double sum = 0.0;
    for (size_t i = 0; i < d; i++) {
        sum += x[i] * x[i];
    }
    return sum;
}

double continuous_rastrigin(const double *x, size_t d) {
    double sum = 10.0 * (double)d;
    for (size_t i = 0; i < d; i++) {
        sum += x[i] * x[i] - 10.0 * cos(2.0 * M_PI * x[i]);
    }
    return sum;
}

double continuous_rosenbrock(const double *x, size_t d) {
    double sum = 0.0;
    for (size_t i = 0; i < d - 1; i++) {
        double t1 = x[i + 1] - x[i] * x[i];
        double t2 = 1.0 - x[i];
        sum += 100.0 * t1 * t1 + t2 * t2;
    }
    return sum;
}

double continuous_ackley(const double *x, size_t d) {
    double sum_sq = 0.0;
    double sum_cos = 0.0;
    for (size_t i = 0; i < d; i++) {
        sum_sq += x[i] * x[i];
        sum_cos += cos(2.0 * M_PI * x[i]);
    }

    double dd = (double)d;
    return -20.0 * exp(-0.2 * sqrt(sum_sq / dd))
           - exp(sum_cos / dd)
           + 20.0 + M_E;
}

double continuous_schwefel(const double *x, size_t d) {
    double sum = 0.0;
    for (size_t i = 0; i < d; i++) {
        sum += x[i] * sin(sqrt(fabs(x[i])));
    }
    return 418.9829 * (double)d - sum;
}

double continuous_evaluate(const void *solution_data, size_t size, const void *context) {
    if (solution_data == NULL || context == NULL || size == 0) return 1e18;

    const double *x = (const double*)solution_data;
    const ContinuousInstance *inst = (const ContinuousInstance*)context;

    switch (inst->fn_type) {
        case CONTINUOUS_SPHERE:     return continuous_sphere(x, size);
        case CONTINUOUS_RASTRIGIN:  return continuous_rastrigin(x, size);
        case CONTINUOUS_ROSENBROCK: return continuous_rosenbrock(x, size);
        case CONTINUOUS_ACKLEY:     return continuous_ackley(x, size);
        case CONTINUOUS_SCHWEFEL:   return continuous_schwefel(x, size);
        default:                    return 1e18;
    }
}

// ============================================================================
// VIZINHANCA
// ============================================================================

void continuous_neighbor_gaussian(const void *current, void *neighbor,
                                  size_t size, const void *context) {
    if (current == NULL || neighbor == NULL || context == NULL || size == 0) return;

    const double *cur = (const double*)current;
    double *nb = (double*)neighbor;
    const ContinuousInstance *inst = (const ContinuousInstance*)context;

    for (size_t i = 0; i < size; i++) {
        nb[i] = cur[i] + inst->neighbor_sigma * opt_random_gaussian();

        if (nb[i] < inst->lower_bound) nb[i] = inst->lower_bound;
        if (nb[i] > inst->upper_bound) nb[i] = inst->upper_bound;
    }
}

// ============================================================================
// GERACAO
// ============================================================================

void continuous_generate_random(void *solution_data, size_t size, const void *context) {
    if (solution_data == NULL || context == NULL || size == 0) return;

    double *x = (double*)solution_data;
    const ContinuousInstance *inst = (const ContinuousInstance*)context;

    double range = inst->upper_bound - inst->lower_bound;
    for (size_t i = 0; i < size; i++) {
        x[i] = inst->lower_bound + opt_random_uniform() * range;
    }
}

// ============================================================================
// UTILIDADES
// ============================================================================

const char* continuous_fn_name(ContinuousFnType fn_type) {
    switch (fn_type) {
        case CONTINUOUS_SPHERE:     return "Sphere";
        case CONTINUOUS_RASTRIGIN:  return "Rastrigin";
        case CONTINUOUS_ROSENBROCK: return "Rosenbrock";
        case CONTINUOUS_ACKLEY:     return "Ackley";
        case CONTINUOUS_SCHWEFEL:   return "Schwefel";
        default:                    return "Unknown";
    }
}

double* continuous_known_optimum_point(ContinuousFnType fn_type, size_t dimensions) {
    if (dimensions == 0) return NULL;

    double *point = calloc(dimensions, sizeof(double));
    if (point == NULL) return NULL;

    switch (fn_type) {
        case CONTINUOUS_SPHERE:
        case CONTINUOUS_RASTRIGIN:
        case CONTINUOUS_ACKLEY:
            break;

        case CONTINUOUS_ROSENBROCK:
            for (size_t i = 0; i < dimensions; i++) {
                point[i] = 1.0;
            }
            break;

        case CONTINUOUS_SCHWEFEL:
            for (size_t i = 0; i < dimensions; i++) {
                point[i] = 420.9687;
            }
            break;
    }

    return point;
}
