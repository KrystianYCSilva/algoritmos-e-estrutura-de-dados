/**
 * @file greedy.h
 * @brief Algoritmos Gulosos (Greedy Algorithms)
 *
 * Implementacao de 3 algoritmos gulosos classicos:
 * 1. Activity Selection - selecao de atividades
 * 2. Huffman Coding - codificacao de Huffman
 * 3. Fractional Knapsack - mochila fracionaria
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 16.1-16.3
 * - Huffman (1952), "A method for the construction of minimum-redundancy codes"
 * - Sedgewick & Wayne (2011), Chapter 5
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef GREEDY_H
#define GREEDY_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// ACTIVITY SELECTION
// ============================================================================

/**
 * @brief Resultado da selecao de atividades
 */
typedef struct {
    size_t *selected;
    size_t count;
} ActivityResult;

/**
 * @brief Selecao de atividades por tempo de termino
 *
 * Seleciona o maximo de atividades compativeis (sem sobreposicao).
 * Atividades devem ser fornecidas com tempos de inicio e fim.
 *
 * @param start Array de tempos de inicio
 * @param finish Array de tempos de termino
 * @param n Numero de atividades
 * @return ActivityResult com indices das atividades selecionadas
 *
 * Complexidade: O(n log n) (dominado pela ordenacao)
 * Espaco: O(n)
 *
 * Referencia: Cormen S16.1 (pseudocodigo GREEDY-ACTIVITY-SELECTOR p. 421)
 */
ActivityResult greedy_activity_selection(const int *start, const int *finish, size_t n);

/**
 * @brief Libera memoria de ActivityResult
 */
void activity_result_destroy(ActivityResult *result);

// ============================================================================
// HUFFMAN CODING
// ============================================================================

/**
 * @brief No da arvore de Huffman
 */
typedef struct HuffmanNode {
    char character;
    int frequency;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

/**
 * @brief Resultado da codificacao de Huffman
 */
typedef struct {
    char *codes[256];
    HuffmanNode *root;
} HuffmanResult;

/**
 * @brief Constroi arvore e tabela de codigos de Huffman
 *
 * @param chars Array de caracteres
 * @param freqs Array de frequencias correspondentes
 * @param n Numero de caracteres distintos
 * @return HuffmanResult com arvore e tabela de codigos
 *
 * Complexidade: O(n log n) com min-heap
 * Espaco: O(n)
 *
 * Referencia: Cormen S16.3 (pseudocodigo HUFFMAN p. 431)
 */
HuffmanResult greedy_huffman(const char *chars, const int *freqs, size_t n);

/**
 * @brief Libera memoria de HuffmanResult
 */
void huffman_result_destroy(HuffmanResult *result);

// ============================================================================
// FRACTIONAL KNAPSACK
// ============================================================================

/**
 * @brief Resultado da mochila fracionaria
 */
typedef struct {
    double max_value;
    double *fractions;
    size_t n;
} FractionalKnapsackResult;

/**
 * @brief Mochila fracionaria - valor maximo
 *
 * Ordena por valor/peso e pega fracao maxima de cada item.
 *
 * @param weights Array de pesos
 * @param values Array de valores
 * @param n Numero de itens
 * @param capacity Capacidade da mochila
 * @return double Valor maximo alcancavel
 *
 * Complexidade: O(n log n)
 * Espaco: O(n)
 *
 * Referencia: Cormen S16.2 (pseudocodigo implicito p. 427)
 */
double greedy_fractional_knapsack_value(const double *weights, const double *values,
                                        size_t n, double capacity);

/**
 * @brief Mochila fracionaria com fracoes de cada item
 *
 * @return FractionalKnapsackResult (caller deve liberar fractions)
 */
FractionalKnapsackResult greedy_fractional_knapsack(const double *weights, const double *values,
                                                     size_t n, double capacity);

/**
 * @brief Libera memoria de FractionalKnapsackResult
 */
void fractional_knapsack_result_destroy(FractionalKnapsackResult *result);

#endif // GREEDY_H
