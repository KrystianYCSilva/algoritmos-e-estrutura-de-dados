/**
 * @file sorting.h
 * @brief Algoritmos de Ordenacao (Sorting Algorithms)
 *
 * Implementacao generica de 10 algoritmos classicos de ordenacao,
 * cobrindo todas as categorias: comparativos quadraticos, comparativos
 * eficientes (divide-and-conquer), e nao-comparativos (lineares).
 *
 * Todos operam sobre arrays genericos (void*) com element_size e CompareFn,
 * seguindo o padrao da biblioteca.
 *
 * Complexidades:
 * - Quadraticos: Bubble O(n^2), Selection O(n^2), Insertion O(n^2)
 * - Sub-quadratico: Shell O(n^1.25) empirico
 * - Eficientes: Merge O(n log n), Quick O(n log n) avg, Heap O(n log n)
 * - Lineares: Counting O(n+k), Radix O(d*(n+k)), Bucket O(n) avg
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 2, 6, 7, 8
 * - Knuth (1998), TAOCP Vol 3, Chapter 5
 * - Sedgewick & Wayne (2011), Chapter 2
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef SORTING_H
#define SORTING_H

#include "data_structures/common.h"
#include <stddef.h>

/**
 * @brief Bubble Sort - Ordenacao por troca adjacente
 *
 * Compara pares adjacentes e troca se fora de ordem.
 * Otimizado com early exit se nenhuma troca ocorrer.
 *
 * @param arr Array de elementos
 * @param n Numero de elementos
 * @param elem_size Tamanho de cada elemento em bytes
 * @param cmp Funcao de comparacao
 *
 * Complexidade: O(n^2) pior/medio, O(n) melhor (ja ordenado)
 * Espaco: O(1)
 * Estavel: Sim
 *
 * Referencia: Cormen Problem 2-2; Knuth TAOCP 3 S5.2.2
 */
void bubble_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Selection Sort - Ordenacao por selecao do minimo
 *
 * Encontra o minimo do subarray nao-ordenado e coloca na posicao correta.
 *
 * Complexidade: O(n^2) todos os casos
 * Espaco: O(1)
 * Estavel: Nao
 * Trocas: Exatamente n-1
 *
 * Referencia: Cormen Exercise 2.2-2; Sedgewick S2.1
 */
void selection_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Insertion Sort - Ordenacao por insercao
 *
 * Insere cada elemento na posicao correta do subarray ja ordenado.
 * Melhor para arrays pequenos ou quase ordenados.
 *
 * Complexidade: O(n^2) pior, O(n) melhor (ja ordenado)
 * Espaco: O(1)
 * Estavel: Sim
 *
 * Referencia: Cormen S2.1 (pseudocodigo p. 18); Knuth TAOCP 3 S5.2.1
 */
void insertion_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Shell Sort - Ordenacao de Shell (generalizacao de Insertion Sort)
 *
 * Insertion sort com gap decrescente. Usa sequencia de Knuth: h = 3h+1.
 *
 * Complexidade: O(n^(3/2)) com sequencia de Knuth, O(n^1.25) empirico
 * Espaco: O(1)
 * Estavel: Nao
 *
 * Referencia: Knuth TAOCP 3 S5.2.1; Sedgewick S2.1
 */
void shell_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Merge Sort - Ordenacao por intercalacao (divide-and-conquer)
 *
 * Divide o array ao meio, ordena recursivamente, e intercala.
 * Garantia de O(n log n) em todos os casos.
 *
 * Complexidade: O(n log n) todos os casos
 * Espaco: O(n)
 * Estavel: Sim
 *
 * Referencia: Cormen S2.3 (pseudocodigo MERGE p. 31); Sedgewick S2.2
 */
void merge_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Quick Sort - Ordenacao rapida (divide-and-conquer)
 *
 * Particiona em torno de um pivo, ordena recursivamente as particoes.
 * Usa median-of-three para escolha do pivo.
 *
 * Complexidade: O(n log n) medio, O(n^2) pior caso
 * Espaco: O(log n) pilha de recursao
 * Estavel: Nao
 *
 * Referencia: Cormen S7 (pseudocodigo PARTITION p. 171); Sedgewick S2.3
 */
void quick_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Heap Sort - Ordenacao por heap
 *
 * Constroi max-heap e extrai maximo repetidamente.
 * In-place e O(n log n) garantido.
 *
 * Complexidade: O(n log n) todos os casos
 * Espaco: O(1)
 * Estavel: Nao
 *
 * Referencia: Cormen S6.4 (pseudocodigo HEAPSORT p. 160); Knuth TAOCP 3 S5.2.3
 */
void heap_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp);

/**
 * @brief Counting Sort - Ordenacao por contagem (nao-comparativo)
 *
 * Conta ocorrencias de cada valor. Requer mapeamento para inteiros.
 *
 * @param arr Array de inteiros
 * @param n Numero de elementos
 * @param max_val Valor maximo no array (range [0, max_val])
 *
 * Complexidade: O(n + k) onde k = max_val
 * Espaco: O(n + k)
 * Estavel: Sim
 *
 * Referencia: Cormen S8.2 (pseudocodigo COUNTING-SORT p. 195)
 */
void counting_sort(int *arr, size_t n, int max_val);

/**
 * @brief Radix Sort - Ordenacao por digitos (nao-comparativo)
 *
 * Ordena digito a digito do menos significativo ao mais significativo (LSD).
 *
 * @param arr Array de inteiros nao-negativos
 * @param n Numero de elementos
 *
 * Complexidade: O(d * (n + 10)) onde d = numero de digitos
 * Espaco: O(n + 10)
 * Estavel: Sim
 *
 * Referencia: Cormen S8.3 (pseudocodigo RADIX-SORT p. 198); Knuth TAOCP 3 S5.2.5
 */
void radix_sort(int *arr, size_t n);

/**
 * @brief Bucket Sort - Ordenacao por distribuicao em baldes
 *
 * Distribui elementos em baldes, ordena cada balde, concatena.
 * Requer dados uniformemente distribuidos em [0, max_val].
 *
 * @param arr Array de doubles em [0.0, 1.0)
 * @param n Numero de elementos
 *
 * Complexidade: O(n) medio (distribuicao uniforme), O(n^2) pior
 * Espaco: O(n)
 * Estavel: Sim (se sub-sort for estavel)
 *
 * Referencia: Cormen S8.4 (pseudocodigo BUCKET-SORT p. 201)
 */
void bucket_sort(double *arr, size_t n);

/**
 * @brief Verifica se um array esta ordenado
 *
 * @return true se arr[i] <= arr[i+1] para todo i
 */
bool is_sorted(const void *arr, size_t n, size_t elem_size, CompareFn cmp);

#endif // SORTING_H
