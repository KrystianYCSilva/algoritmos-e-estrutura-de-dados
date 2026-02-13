/**
 * @file tsp.c
 * @brief Implementacao do benchmark TSP para algoritmos de otimizacao
 *
 * Instancias hardcoded (5, 10, 20 cidades) e aleatorias,
 * funcao de custo, vizinhancas swap/2-opt, perturbacao double-bridge,
 * e geracao aleatoria via Fisher-Yates shuffle.
 *
 * Referencias:
 * - Reinelt, G. (1991). "TSPLIB - A Traveling Salesman Problem Library"
 * - Croes, G. A. (1958). "A Method for Solving Traveling-Salesman Problems"
 * - Lin, S. & Kernighan, B. W. (1973). "An effective heuristic algorithm for the TSP"
 * - Martin, O., Otto, S. W. & Felten, E. W. (1991). "Large-step Markov chains
 *   for the traveling salesman problem"
 * - Fisher, R. A. & Yates, F. (1938). Statistical Tables
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/benchmarks/tsp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================================
// HELPERS INTERNOS
// ============================================================================

static TSPInstance* tsp_alloc_instance(size_t n) {
    TSPInstance *inst = calloc(1, sizeof(TSPInstance));
    if (inst == NULL) return NULL;

    inst->n_cities = n;
    inst->known_optimum = -1.0;

    inst->x = calloc(n, sizeof(double));
    inst->y = calloc(n, sizeof(double));
    inst->dist_matrix = calloc(n, sizeof(double*));

    if (inst->x == NULL || inst->y == NULL || inst->dist_matrix == NULL) {
        tsp_instance_destroy(inst);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        inst->dist_matrix[i] = calloc(n, sizeof(double));
        if (inst->dist_matrix[i] == NULL) {
            tsp_instance_destroy(inst);
            return NULL;
        }
    }

    return inst;
}

static void tsp_compute_distances(TSPInstance *inst) {
    size_t n = inst->n_cities;
    for (size_t i = 0; i < n; i++) {
        inst->dist_matrix[i][i] = 0.0;
        for (size_t j = i + 1; j < n; j++) {
            double dx = inst->x[i] - inst->x[j];
            double dy = inst->y[i] - inst->y[j];
            double d = sqrt(dx * dx + dy * dy);
            inst->dist_matrix[i][j] = d;
            inst->dist_matrix[j][i] = d;
        }
    }
}

// ============================================================================
// CRIACAO DE INSTANCIAS
// ============================================================================

TSPInstance* tsp_create_example_5(void) {
    TSPInstance *inst = tsp_alloc_instance(5);
    if (inst == NULL) return NULL;

    double radius = 10.0;
    for (size_t i = 0; i < 5; i++) {
        double angle = 2.0 * M_PI * (double)i / 5.0;
        inst->x[i] = radius * cos(angle);
        inst->y[i] = radius * sin(angle);
    }

    tsp_compute_distances(inst);

    double side = 2.0 * radius * sin(M_PI / 5.0);
    inst->known_optimum = 5.0 * side;

    return inst;
}

TSPInstance* tsp_create_example_10(void) {
    TSPInstance *inst = tsp_alloc_instance(10);
    if (inst == NULL) return NULL;

    double coords[][2] = {
        { 0.0,  0.0}, {10.0,  0.0}, {20.0,  0.0}, {30.0,  0.0}, {40.0,  0.0},
        {40.0, 10.0}, {30.0, 10.0}, {20.0, 10.0}, {10.0, 10.0}, { 0.0, 10.0}
    };

    for (size_t i = 0; i < 10; i++) {
        inst->x[i] = coords[i][0];
        inst->y[i] = coords[i][1];
    }

    tsp_compute_distances(inst);
    inst->known_optimum = 4.0 * 10.0 + 2.0 * sqrt(100.0 + 100.0);

    return inst;
}

TSPInstance* tsp_create_example_20(void) {
    TSPInstance *inst = tsp_alloc_instance(20);
    if (inst == NULL) return NULL;

    double coords[][2] = {
        { 0.0,  0.0}, {10.0,  0.0}, {20.0,  0.0}, {30.0,  0.0}, {40.0,  0.0},
        {50.0,  0.0}, {60.0,  0.0}, {70.0,  0.0}, {80.0,  0.0}, {90.0,  0.0},
        {90.0, 10.0}, {80.0, 10.0}, {70.0, 10.0}, {60.0, 10.0}, {50.0, 10.0},
        {40.0, 10.0}, {30.0, 10.0}, {20.0, 10.0}, {10.0, 10.0}, { 0.0, 10.0}
    };

    for (size_t i = 0; i < 20; i++) {
        inst->x[i] = coords[i][0];
        inst->y[i] = coords[i][1];
    }

    tsp_compute_distances(inst);
    inst->known_optimum = 9.0 * 10.0 * 2.0 + 2.0 * sqrt(100.0 + 100.0);

    return inst;
}

TSPInstance* tsp_create_random(size_t n, unsigned seed) {
    if (n < 2) return NULL;

    TSPInstance *inst = tsp_alloc_instance(n);
    if (inst == NULL) return NULL;

    unsigned saved_seed = seed;
    srand(saved_seed);

    for (size_t i = 0; i < n; i++) {
        inst->x[i] = (double)rand() / RAND_MAX * 100.0;
        inst->y[i] = (double)rand() / RAND_MAX * 100.0;
    }

    tsp_compute_distances(inst);
    inst->known_optimum = -1.0;

    return inst;
}

void tsp_instance_destroy(TSPInstance *inst) {
    if (inst == NULL) return;

    if (inst->dist_matrix != NULL) {
        for (size_t i = 0; i < inst->n_cities; i++) {
            free(inst->dist_matrix[i]);
        }
        free(inst->dist_matrix);
    }

    free(inst->x);
    free(inst->y);
    free(inst);
}

// ============================================================================
// FUNCAO OBJETIVO
// ============================================================================

double tsp_tour_cost(const void *tour_data, size_t n, const void *context) {
    if (tour_data == NULL || context == NULL || n < 2) return 1e18;

    const int *tour = (const int*)tour_data;
    const TSPInstance *inst = (const TSPInstance*)context;

    double cost = 0.0;
    for (size_t i = 0; i < n - 1; i++) {
        cost += inst->dist_matrix[tour[i]][tour[i + 1]];
    }
    cost += inst->dist_matrix[tour[n - 1]][tour[0]];

    return cost;
}

// ============================================================================
// VIZINHANCAS
// ============================================================================

void tsp_neighbor_swap(const void *current, void *neighbor, size_t n, const void *context) {
    (void)context;
    if (current == NULL || neighbor == NULL || n < 2) return;

    memcpy(neighbor, current, n * sizeof(int));

    int *nb = (int*)neighbor;
    int i = opt_random_int(0, (int)n - 1);
    int j = opt_random_int(0, (int)n - 2);
    if (j >= i) j++;

    int tmp = nb[i];
    nb[i] = nb[j];
    nb[j] = tmp;
}

void tsp_neighbor_2opt(const void *current, void *neighbor, size_t n, const void *context) {
    (void)context;
    if (current == NULL || neighbor == NULL || n < 3) return;

    memcpy(neighbor, current, n * sizeof(int));

    int *nb = (int*)neighbor;
    int i = opt_random_int(0, (int)n - 2);
    int j = opt_random_int(i + 1, (int)n - 1);

    while (i < j) {
        int tmp = nb[i];
        nb[i] = nb[j];
        nb[j] = tmp;
        i++;
        j--;
    }
}

// ============================================================================
// PERTURBACAO
// ============================================================================

void tsp_perturb_double_bridge(const void *current, void *perturbed, size_t n,
                               int strength, const void *context) {
    (void)strength;
    (void)context;
    if (current == NULL || perturbed == NULL || n < 8) {
        if (current != NULL && perturbed != NULL && n > 0) {
            memcpy(perturbed, current, n * sizeof(int));
        }
        return;
    }

    const int *src = (const int*)current;
    int *dst = (int*)perturbed;

    int positions[3];
    positions[0] = opt_random_int(1, (int)n / 4);
    positions[1] = opt_random_int(positions[0] + 1, (int)n / 2);
    positions[2] = opt_random_int(positions[1] + 1, (int)(3 * n / 4));

    int p1 = positions[0];
    int p2 = positions[1];
    int p3 = positions[2];

    int idx = 0;

    for (int i = 0; i < p1; i++)
        dst[idx++] = src[i];

    for (int i = p2; i < p3; i++)
        dst[idx++] = src[i];

    for (int i = p1; i < p2; i++)
        dst[idx++] = src[i];

    for (int i = p3; i < (int)n; i++)
        dst[idx++] = src[i];
}

// ============================================================================
// GERACAO
// ============================================================================

void tsp_generate_random(void *tour_data, size_t n, const void *context) {
    (void)context;
    if (tour_data == NULL || n == 0) return;

    int *tour = (int*)tour_data;
    for (size_t i = 0; i < n; i++) {
        tour[i] = (int)i;
    }

    for (size_t i = n - 1; i > 0; i--) {
        size_t j = (size_t)opt_random_int(0, (int)i);
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
    }
}

// ============================================================================
// UTILIDADES
// ============================================================================

bool tsp_is_valid_tour(const int *tour, size_t n) {
    if (tour == NULL || n == 0) return false;

    bool *seen = calloc(n, sizeof(bool));
    if (seen == NULL) return false;

    for (size_t i = 0; i < n; i++) {
        if (tour[i] < 0 || (size_t)tour[i] >= n) {
            free(seen);
            return false;
        }
        if (seen[tour[i]]) {
            free(seen);
            return false;
        }
        seen[tour[i]] = true;
    }

    free(seen);
    return true;
}

void tsp_print_tour(const int *tour, size_t n, double cost) {
    if (tour == NULL) {
        printf("(null tour)\n");
        return;
    }

    printf("Tour [cost=%.2f]: ", cost);
    for (size_t i = 0; i < n; i++) {
        printf("%d", tour[i]);
        if (i < n - 1) printf(" -> ");
    }
    printf(" -> %d\n", tour[0]);
}
