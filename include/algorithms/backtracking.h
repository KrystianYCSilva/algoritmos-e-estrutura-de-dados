/**
 * @file backtracking.h
 * @brief Algoritmos de Backtracking
 *
 * Implementacao de 4 problemas classicos de backtracking:
 * 1. N-Queens - posicionamento de N rainhas
 * 2. Subset Sum - soma de subconjunto
 * 3. Permutations - geracao de permutacoes
 * 4. Graph Coloring - coloracao de grafos
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 34 (NP-completeness context)
 * - Knuth (2011), TAOCP Vol 4A, Chapter 7 (Combinatorial Searching)
 * - Wirth (1976), "Algorithms + Data Structures = Programs"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// N-QUEENS
// ============================================================================

/**
 * @brief Resultado do N-Queens
 */
typedef struct {
    int **solutions;
    size_t count;
    size_t capacity;
    size_t n;
} NQueensResult;

/**
 * @brief Encontra todas as solucoes para o problema das N-Queens
 *
 * Posiciona N rainhas em um tabuleiro N x N tal que nenhuma
 * rainha ataca outra (mesma linha, coluna ou diagonal).
 *
 * @param n Tamanho do tabuleiro (e numero de rainhas)
 * @return NQueensResult com todas as solucoes
 *         Cada solucao e um array onde solution[i] = coluna da rainha na linha i
 *
 * Complexidade: O(N!) no pior caso
 * Espaco: O(N) por solucao
 *
 * Referencia: Knuth TAOCP 4A S7.2.2; Wirth (1976)
 */
NQueensResult nqueens_solve(size_t n);

/**
 * @brief Conta solucoes sem armazena-las
 *
 * @return size_t Numero de solucoes
 */
size_t nqueens_count(size_t n);

/**
 * @brief Libera memoria de NQueensResult
 */
void nqueens_result_destroy(NQueensResult *result);

// ============================================================================
// SUBSET SUM
// ============================================================================

/**
 * @brief Resultado do Subset Sum
 */
typedef struct {
    bool **subsets;
    size_t count;
    size_t capacity;
    size_t n;
} SubsetSumResult;

/**
 * @brief Encontra todos os subconjuntos que somam ao alvo
 *
 * @param set Array de inteiros (positivos)
 * @param n Tamanho do conjunto
 * @param target Soma alvo
 * @return SubsetSumResult com todos os subconjuntos validos
 *
 * Complexidade: O(2^n) pior caso
 * Referencia: Cormen S34.5.5 (SUBSET-SUM e NP-completude)
 */
SubsetSumResult subset_sum_all(const int *set, size_t n, int target);

/**
 * @brief Verifica se existe subconjunto que soma ao alvo
 *
 * @return bool true se existe
 */
bool subset_sum_exists(const int *set, size_t n, int target);

/**
 * @brief Libera memoria de SubsetSumResult
 */
void subset_sum_result_destroy(SubsetSumResult *result);

// ============================================================================
// PERMUTATIONS
// ============================================================================

/**
 * @brief Resultado das permutacoes
 */
typedef struct {
    int **perms;
    size_t count;
    size_t capacity;
    size_t n;
} PermutationResult;

/**
 * @brief Gera todas as permutacoes de arr[0..n-1]
 *
 * @param arr Array de inteiros
 * @param n Tamanho do array
 * @return PermutationResult com todas as n! permutacoes
 *
 * Complexidade: O(n * n!)
 * Referencia: Knuth TAOCP 4A S7.2.1.2 (Algorithm L - Heap's algorithm)
 */
PermutationResult permutations_generate(const int *arr, size_t n);

/**
 * @brief Conta permutacoes (n!)
 */
size_t permutations_count(size_t n);

/**
 * @brief Libera memoria de PermutationResult
 */
void permutation_result_destroy(PermutationResult *result);

// ============================================================================
// GRAPH COLORING
// ============================================================================

/**
 * @brief Resultado da coloracao de grafos
 */
typedef struct {
    int *colors;
    size_t n;
    bool solvable;
} GraphColoringResult;

/**
 * @brief Colore um grafo com no maximo m cores
 *
 * O grafo e representado como matriz de adjacencia (n x n).
 * adj[i*n + j] = 1 se existe aresta entre i e j.
 *
 * @param adj Matriz de adjacencia (n x n, row-major)
 * @param n Numero de vertices
 * @param m Numero maximo de cores
 * @return GraphColoringResult com coloracao valida ou solvable=false
 *
 * Complexidade: O(m^n) pior caso
 * Referencia: Cormen S34.4 (chromatic number); Welsh & Powell (1967)
 */
GraphColoringResult graph_coloring(const int *adj, size_t n, int m);

/**
 * @brief Libera memoria de GraphColoringResult
 */
void graph_coloring_result_destroy(GraphColoringResult *result);

#endif // BACKTRACKING_H
