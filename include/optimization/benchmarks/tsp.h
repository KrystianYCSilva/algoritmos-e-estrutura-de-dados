/**
 * @file tsp.h
 * @brief Benchmark TSP (Traveling Salesman Problem) para algoritmos de otimizacao
 *
 * Fornece instancias hardcoded e aleatorias do TSP, funcao objetivo
 * compativel com ObjectiveFn, vizinhancas (swap, 2-opt) e perturbacoes
 * (double-bridge) para uso com heuristicas e meta-heuristicas.
 *
 * Representacao: tour como array de int (permutacao de [0..n-1])
 * Funcao objetivo: custo total do ciclo hamiltoniano (distancia euclidiana)
 *
 * Referencias:
 * - Reinelt, G. (1991). "TSPLIB - A Traveling Salesman Problem Library"
 * - Croes, G. A. (1958). "A Method for Solving Traveling-Salesman Problems"
 * - Lin, S. & Kernighan, B. W. (1973). "An effective heuristic algorithm for the TSP"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_BENCHMARK_TSP_H
#define OPT_BENCHMARK_TSP_H

#include "optimization/common.h"
#include <stddef.h>

// ============================================================================
// INSTANCIA TSP
// ============================================================================

/**
 * @brief Instancia do Problema do Caixeiro Viajante
 */
typedef struct {
    double **dist_matrix;    /**< Matriz de distancias n x n */
    size_t n_cities;         /**< Numero de cidades */
    double *x;               /**< Coordenadas x das cidades */
    double *y;               /**< Coordenadas y das cidades */
    double known_optimum;    /**< Custo otimo conhecido (-1 se desconhecido) */
} TSPInstance;

// ============================================================================
// CRIACAO E DESTRUICAO DE INSTANCIAS
// ============================================================================

/**
 * @brief Cria instancia TSP com 5 cidades (pentagono, otimo conhecido)
 *
 * Cidades em pentagono regular inscrito em circulo de raio 10.
 * Otimo: tour sequencial ~29.39
 *
 * @return TSPInstance* Instancia alocada (caller libera com tsp_instance_destroy)
 */
TSPInstance* tsp_create_example_5(void);

/**
 * @brief Cria instancia TSP com 10 cidades (otimo estimado)
 *
 * @return TSPInstance* Instancia alocada
 */
TSPInstance* tsp_create_example_10(void);

/**
 * @brief Cria instancia TSP com 20 cidades
 *
 * @return TSPInstance* Instancia alocada
 */
TSPInstance* tsp_create_example_20(void);

/**
 * @brief Cria instancia TSP aleatoria com n cidades em [0,100]x[0,100]
 *
 * @param n Numero de cidades
 * @param seed Semente para reproducibilidade
 * @return TSPInstance* Instancia alocada
 */
TSPInstance* tsp_create_random(size_t n, unsigned seed);

/**
 * @brief Libera memoria de uma instancia TSP
 *
 * @param inst Instancia a destruir
 */
void tsp_instance_destroy(TSPInstance *inst);

// ============================================================================
// FUNCAO OBJETIVO (ObjectiveFn-compatible)
// ============================================================================

/**
 * @brief Calcula custo total de um tour TSP (ciclo fechado)
 *
 * Soma das distancias: tour[0]->tour[1]->...->tour[n-1]->tour[0]
 *
 * @param tour_data Array de int* (permutacao de [0..n-1])
 * @param n Numero de cidades
 * @param context TSPInstance* com dist_matrix
 * @return double Custo total do tour
 *
 * Complexidade: O(n)
 */
double tsp_tour_cost(const void *tour_data, size_t n, const void *context);

// ============================================================================
// VIZINHANCAS (NeighborFn-compatible)
// ============================================================================

/**
 * @brief Gera vizinho por swap de 2 cidades aleatorias
 *
 * Copia current para neighbor, troca posicoes i,j aleatorias.
 * Tamanho da vizinhanca: O(n^2)
 *
 * @param current Tour atual (int*)
 * @param neighbor Buffer para tour vizinho (int*, pre-alocado)
 * @param n Numero de cidades
 * @param context TSPInstance*
 */
void tsp_neighbor_swap(const void *current, void *neighbor, size_t n, const void *context);

/**
 * @brief Gera vizinho por 2-opt (inverte segmento aleatorio)
 *
 * Seleciona i < j aleatorios, inverte segmento [i..j] do tour.
 * Vizinhanca padrao para TSP. Tamanho: O(n^2)
 *
 * @param current Tour atual (int*)
 * @param neighbor Buffer para tour vizinho (int*, pre-alocado)
 * @param n Numero de cidades
 * @param context TSPInstance*
 *
 * Referencia: Croes (1958)
 */
void tsp_neighbor_2opt(const void *current, void *neighbor, size_t n, const void *context);

// ============================================================================
// PERTURBACAO (PerturbFn-compatible)
// ============================================================================

/**
 * @brief Perturbacao double-bridge para TSP
 *
 * Corta o tour em 4 segmentos e reconecta em ordem diferente.
 * Perturbacao forte usada em ILS para escapar de otimos locais 2-opt.
 *
 * @param current Tour atual (int*)
 * @param perturbed Buffer para tour perturbado (int*, pre-alocado)
 * @param n Numero de cidades
 * @param strength Nao utilizado (perturbacao tem forca fixa)
 * @param context TSPInstance*
 *
 * Referencia: Martin, Otto & Felten (1991)
 */
void tsp_perturb_double_bridge(const void *current, void *perturbed, size_t n,
                               int strength, const void *context);

// ============================================================================
// GERACAO (GenerateFn-compatible)
// ============================================================================

/**
 * @brief Gera tour aleatorio valido (permutacao de [0..n-1])
 *
 * Usa Fisher-Yates shuffle.
 *
 * @param tour_data Buffer int* pre-alocado para n inteiros
 * @param n Numero de cidades
 * @param context TSPInstance* (nao utilizado, pode ser NULL)
 */
void tsp_generate_random(void *tour_data, size_t n, const void *context);

// ============================================================================
// UTILIDADES
// ============================================================================

/**
 * @brief Verifica se um tour e uma permutacao valida de [0..n-1]
 *
 * @param tour Array de inteiros
 * @param n Tamanho
 * @return true se tour e permutacao valida
 */
bool tsp_is_valid_tour(const int *tour, size_t n);

/**
 * @brief Imprime tour e custo
 *
 * @param tour Array de inteiros
 * @param n Numero de cidades
 * @param cost Custo do tour
 */
void tsp_print_tour(const int *tour, size_t n, double cost);

#endif /* OPT_BENCHMARK_TSP_H */
