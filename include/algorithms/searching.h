/**
 * @file searching.h
 * @brief Algoritmos de Busca (Searching Algorithms)
 *
 * Implementacao generica de algoritmos de busca em arrays ordenados
 * e nao-ordenados.
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 2
 * - Knuth (1998), TAOCP Vol 3, Chapter 6
 * - Sedgewick & Wayne (2011), Chapter 3
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include "data_structures/common.h"
#include <stddef.h>

#define SEARCH_NOT_FOUND ((size_t)-1)

/**
 * @brief Busca Linear - Verifica elemento por elemento
 *
 * @param arr Array de elementos
 * @param n Numero de elementos
 * @param elem_size Tamanho de cada elemento
 * @param target Elemento a buscar
 * @param cmp Funcao de comparacao
 * @return size_t Indice do elemento ou SEARCH_NOT_FOUND
 *
 * Complexidade: O(n)
 * Referencia: Cormen S2.1; Knuth TAOCP 3 S6.1
 */
size_t linear_search(const void *arr, size_t n, size_t elem_size,
                     const void *target, CompareFn cmp);

/**
 * @brief Busca Binaria - Divide conjunto ordenado ao meio
 *
 * Requer array ordenado. Retorna indice de qualquer ocorrencia.
 *
 * Complexidade: O(log n)
 * Referencia: Cormen S2.3 Exercise; Knuth TAOCP 3 S6.2.1
 */
size_t binary_search(const void *arr, size_t n, size_t elem_size,
                     const void *target, CompareFn cmp);

/**
 * @brief Busca por Interpolacao - Estima posicao por interpolacao linear
 *
 * Requer array ordenado de inteiros. Para dados uniformemente distribuidos,
 * alcanca O(log log n) em media.
 *
 * @param arr Array de inteiros ordenado
 * @param n Numero de elementos
 * @param target Valor a buscar
 * @return size_t Indice ou SEARCH_NOT_FOUND
 *
 * Complexidade: O(log log n) medio, O(n) pior caso
 * Referencia: Knuth TAOCP 3 S6.2.1; Sedgewick S3.1
 */
size_t interpolation_search(const int *arr, size_t n, int target);

/**
 * @brief Busca Ternaria - Divide conjunto ordenado em tercos
 *
 * Util para funcoes unimodais. Para arrays, similar a busca binaria
 * mas com fator log_3 em vez de log_2.
 *
 * Complexidade: O(log_3 n) = O(log n)
 * Referencia: Sedgewick S3.1
 */
size_t ternary_search(const void *arr, size_t n, size_t elem_size,
                      const void *target, CompareFn cmp);

/**
 * @brief Jump Search - Busca por saltos de tamanho sqrt(n)
 *
 * Salta blocos de sqrt(n) e faz busca linear no bloco.
 *
 * Complexidade: O(sqrt(n))
 * Referencia: Knuth TAOCP 3 S6.2.1
 */
size_t jump_search(const void *arr, size_t n, size_t elem_size,
                   const void *target, CompareFn cmp);

/**
 * @brief Exponential Search - Busca exponencial + binaria
 *
 * Encontra range exponencialmente, depois busca binaria no range.
 * Util para listas ilimitadas.
 *
 * Complexidade: O(log n)
 * Referencia: Knuth TAOCP 3 S6.2.1
 */
size_t exponential_search(const void *arr, size_t n, size_t elem_size,
                          const void *target, CompareFn cmp);

#endif // SEARCHING_H
