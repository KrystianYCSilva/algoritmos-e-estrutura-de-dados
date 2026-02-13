/**
 * @file dynamic_programming.h
 * @brief Algoritmos de Programacao Dinamica (Dynamic Programming)
 *
 * Implementacao de 8 problemas classicos de programacao dinamica,
 * cobrindo otimizacao, sequencias, e contagem.
 *
 * Problemas:
 * 1. Fibonacci - sequencia numerica
 * 2. LCS - Longest Common Subsequence
 * 3. Knapsack 0/1 - problema da mochila
 * 4. Edit Distance - distancia de edicao (Levenshtein)
 * 5. LIS - Longest Increasing Subsequence
 * 6. Rod Cutting - corte de barras
 * 7. Matrix Chain Multiplication - multiplicacao de cadeia de matrizes
 * 8. Coin Change - troco com minimo de moedas
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 15.1-15.4
 * - Bellman (1957), "Dynamic Programming"
 * - Sedgewick & Wayne (2011), Chapter 5
 * - Levenshtein (1966), "Binary codes capable of correcting deletions"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// FIBONACCI
// ============================================================================

/**
 * @brief Fibonacci com memoizacao (top-down)
 *
 * @param n Indice na sequencia (0-indexed)
 * @return long long F(n)
 *
 * Complexidade: O(n) tempo, O(n) espaco
 * Referencia: Cormen S15.1 (rod cutting motivacao)
 */
long long dp_fibonacci_memo(int n);

/**
 * @brief Fibonacci com tabulacao (bottom-up)
 *
 * @param n Indice na sequencia (0-indexed)
 * @return long long F(n)
 *
 * Complexidade: O(n) tempo, O(1) espaco
 * Referencia: Cormen S15.1
 */
long long dp_fibonacci_tab(int n);

// ============================================================================
// LCS - Longest Common Subsequence
// ============================================================================

/**
 * @brief Resultado de LCS
 */
typedef struct {
    size_t length;
    char *sequence;
} LCSResult;

/**
 * @brief Calcula o comprimento da LCS de duas strings
 *
 * @param x Primeira string
 * @param y Segunda string
 * @return size_t Comprimento da LCS
 *
 * Complexidade: O(m*n) tempo e espaco
 * Referencia: Cormen S15.4 (pseudocodigo LCS-LENGTH p. 394)
 */
size_t dp_lcs_length(const char *x, const char *y);

/**
 * @brief Calcula a LCS com reconstrucao da subsequencia
 *
 * @param x Primeira string
 * @param y Segunda string
 * @return LCSResult com comprimento e string da LCS (caller deve liberar sequence)
 *
 * Complexidade: O(m*n) tempo e espaco
 * Referencia: Cormen S15.4 (pseudocodigo PRINT-LCS p. 395)
 */
LCSResult dp_lcs(const char *x, const char *y);

/**
 * @brief Libera memoria de LCSResult
 */
void dp_lcs_result_destroy(LCSResult *result);

// ============================================================================
// KNAPSACK 0/1
// ============================================================================

/**
 * @brief Resultado do Knapsack
 */
typedef struct {
    int max_value;
    bool *selected;
    size_t n;
} KnapsackResult;

/**
 * @brief Knapsack 0/1 - valor maximo
 *
 * @param weights Array de pesos dos itens
 * @param values Array de valores dos itens
 * @param n Numero de itens
 * @param capacity Capacidade da mochila
 * @return int Valor maximo alcancavel
 *
 * Complexidade: O(n*W) tempo e espaco, onde W = capacity
 * Referencia: Cormen S16.2 (fracionario); Kleinberg & Tardos (2005) S6.4
 */
int dp_knapsack_value(const int *weights, const int *values, size_t n, int capacity);

/**
 * @brief Knapsack 0/1 com reconstrucao dos itens selecionados
 *
 * @return KnapsackResult (caller deve liberar selected)
 */
KnapsackResult dp_knapsack(const int *weights, const int *values, size_t n, int capacity);

/**
 * @brief Libera memoria de KnapsackResult
 */
void dp_knapsack_result_destroy(KnapsackResult *result);

// ============================================================================
// EDIT DISTANCE (Levenshtein)
// ============================================================================

/**
 * @brief Calcula a distancia de edicao (Levenshtein) entre duas strings
 *
 * Operacoes: insercao, remocao, substituicao (custo 1 cada).
 *
 * @param s1 Primeira string
 * @param s2 Segunda string
 * @return size_t Numero minimo de operacoes
 *
 * Complexidade: O(m*n) tempo e espaco
 * Referencia: Cormen Problem 15-5; Levenshtein (1966)
 */
size_t dp_edit_distance(const char *s1, const char *s2);

// ============================================================================
// LIS - Longest Increasing Subsequence
// ============================================================================

/**
 * @brief Resultado de LIS
 */
typedef struct {
    size_t length;
    int *sequence;
    size_t seq_len;
} LISResult;

/**
 * @brief LIS - comprimento apenas (O(n log n) com busca binaria)
 *
 * @param arr Array de inteiros
 * @param n Tamanho do array
 * @return size_t Comprimento da LIS
 *
 * Complexidade: O(n log n) tempo, O(n) espaco
 * Referencia: Cormen Problem 15-4; Fredman (1975)
 */
size_t dp_lis_length(const int *arr, size_t n);

/**
 * @brief LIS com reconstrucao (O(n^2))
 *
 * @return LISResult (caller deve liberar sequence)
 */
LISResult dp_lis(const int *arr, size_t n);

/**
 * @brief Libera memoria de LISResult
 */
void dp_lis_result_destroy(LISResult *result);

// ============================================================================
// ROD CUTTING
// ============================================================================

/**
 * @brief Resultado do corte de barras
 */
typedef struct {
    int max_revenue;
    int *cuts;
    size_t num_cuts;
} RodCutResult;

/**
 * @brief Rod Cutting - receita maxima
 *
 * @param prices Array de precos (prices[i] = preco da barra de tamanho i+1)
 * @param n Comprimento da barra
 * @return int Receita maxima
 *
 * Complexidade: O(n^2) tempo, O(n) espaco
 * Referencia: Cormen S15.1 (pseudocodigo BOTTOM-UP-CUT-ROD p. 366)
 */
int dp_rod_cutting_value(const int *prices, size_t n);

/**
 * @brief Rod Cutting com reconstrucao dos cortes
 *
 * @return RodCutResult (caller deve liberar cuts)
 */
RodCutResult dp_rod_cutting(const int *prices, size_t n);

/**
 * @brief Libera memoria de RodCutResult
 */
void dp_rod_cut_result_destroy(RodCutResult *result);

// ============================================================================
// MATRIX CHAIN MULTIPLICATION
// ============================================================================

/**
 * @brief Resultado da multiplicacao de cadeia de matrizes
 */
typedef struct {
    long long min_operations;
    char *parenthesization;
} MatrixChainResult;

/**
 * @brief Matrix Chain - numero minimo de operacoes escalares
 *
 * @param dims Array de dimensoes (n+1 valores para n matrizes)
 *             Matriz i tem dimensoes dims[i] x dims[i+1]
 * @param n Numero de matrizes
 * @return long long Numero minimo de multiplicacoes escalares
 *
 * Complexidade: O(n^3) tempo, O(n^2) espaco
 * Referencia: Cormen S15.2 (pseudocodigo MATRIX-CHAIN-ORDER p. 375)
 */
long long dp_matrix_chain_value(const int *dims, size_t n);

/**
 * @brief Matrix Chain com parentetizacao otima
 *
 * @return MatrixChainResult (caller deve liberar parenthesization)
 */
MatrixChainResult dp_matrix_chain(const int *dims, size_t n);

/**
 * @brief Libera memoria de MatrixChainResult
 */
void dp_matrix_chain_result_destroy(MatrixChainResult *result);

// ============================================================================
// COIN CHANGE
// ============================================================================

/**
 * @brief Resultado do Coin Change
 */
typedef struct {
    int min_coins;
    int *coins_used;
    size_t num_coins_used;
} CoinChangeResult;

/**
 * @brief Coin Change - numero minimo de moedas
 *
 * @param coins Array de denominacoes disponiveis
 * @param num_coins Numero de denominacoes
 * @param amount Valor alvo
 * @return int Numero minimo de moedas, ou -1 se impossivel
 *
 * Complexidade: O(amount * num_coins) tempo, O(amount) espaco
 * Referencia: Cormen Problem 16-1; Bellman (1957)
 */
int dp_coin_change_value(const int *coins, size_t num_coins, int amount);

/**
 * @brief Coin Change com reconstrucao das moedas usadas
 *
 * @return CoinChangeResult (caller deve liberar coins_used)
 */
CoinChangeResult dp_coin_change(const int *coins, size_t num_coins, int amount);

/**
 * @brief Libera memoria de CoinChangeResult
 */
void dp_coin_change_result_destroy(CoinChangeResult *result);

#endif // DYNAMIC_PROGRAMMING_H
