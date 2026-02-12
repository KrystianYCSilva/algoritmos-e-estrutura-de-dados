/**
 * @file heap.h
 * @brief Implementação genérica de Heap Binário (Binary Heap)
 *
 * Heap é uma árvore binária completa que satisfaz a propriedade heap:
 * - Max-Heap: pai ≥ filhos
 * - Min-Heap: pai ≤ filhos
 *
 * Implementado como array para eficiência (heap implícito).
 * Para índice i: parent = (i-1)/2, left = 2i+1, right = 2i+2
 *
 * Complexidade das Operações:
 * - Insert: O(log n)
 * - Extract-Min/Max: O(log n)
 * - Peek (get min/max): O(1)
 * - Heapify: O(log n)
 * - Build-Heap: O(n) - análise tight (não O(n log n))
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 6 - Heapsort e Priority Queues
 * - Williams, J. W. J. (1964). "Algorithm 232: Heapsort"
 * - Floyd, R. W. (1964). "Algorithm 245: Treesort 3"
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef HEAP_H
#define HEAP_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURA DO HEAP (OPACA)
// ============================================================================

typedef struct Heap Heap;

typedef enum {
    HEAP_MIN,  /**< Min-Heap: menor elemento no topo */
    HEAP_MAX   /**< Max-Heap: maior elemento no topo */
} HeapType;

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria um novo heap
 *
 * @param element_size Tamanho dos elementos
 * @param initial_capacity Capacidade inicial
 * @param type HEAP_MIN ou HEAP_MAX
 * @param compare Função de comparação
 * @param destroy Função de destruição (NULL se não necessário)
 * @return Heap* Heap criado
 *
 * Complexidade: O(capacity)
 */
Heap* heap_create(size_t element_size, size_t initial_capacity,
                  HeapType type, CompareFn compare, DestroyFn destroy);

/**
 * @brief Destrói o heap
 *
 * Complexidade: O(n)
 */
void heap_destroy(Heap *heap);

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * @brief Insere um elemento no heap
 *
 * @param heap Ponteiro para o heap
 * @param data Ponteiro para o dado
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 164):
 * MAX-HEAP-INSERT(A, key)
 *   A.heap-size = A.heap-size + 1
 *   A[A.heap-size] = -∞
 *   HEAP-INCREASE-KEY(A, A.heap-size, key)
 *
 * Algoritmo: Insere no final e "flutua" (heap-up/bubble-up) até posição correta.
 *
 * Complexidade: O(log n)
 */
DataStructureError heap_insert(Heap *heap, const void *data);

/**
 * @brief Remove e retorna o elemento de maior prioridade (min ou max)
 *
 * @param heap Ponteiro para o heap
 * @param output Buffer para armazenar o elemento (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 163):
 * HEAP-EXTRACT-MAX(A)
 *   if A.heap-size < 1
 *       error "heap underflow"
 *   max = A[1]
 *   A[1] = A[A.heap-size]
 *   A.heap-size = A.heap-size - 1
 *   MAX-HEAPIFY(A, 1)
 *   return max
 *
 * Algoritmo: Remove raiz, substitui pelo último, "afunda" (heap-down/bubble-down).
 *
 * Complexidade: O(log n)
 */
DataStructureError heap_extract(Heap *heap, void *output);

/**
 * @brief Retorna o elemento de maior prioridade sem remover
 *
 * @param heap Ponteiro para o heap
 * @param output Buffer para armazenar o elemento
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError heap_peek(const Heap *heap, void *output);

/**
 * @brief Atualiza a prioridade de um elemento (por índice)
 *
 * @param heap Ponteiro para o heap
 * @param index Índice do elemento
 * @param new_data Novo valor
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 164):
 * HEAP-INCREASE-KEY(A, i, key)
 *   if key < A[i]
 *       error "new key is smaller than current key"
 *   A[i] = key
 *   while i > 1 and A[PARENT(i)] < A[i]
 *       exchange A[i] with A[PARENT(i)]
 *       i = PARENT(i)
 *
 * Complexidade: O(log n)
 */
DataStructureError heap_update(Heap *heap, size_t index, const void *new_data);

// ============================================================================
// HEAPIFY
// ============================================================================

/**
 * @brief Restaura propriedade heap a partir de um índice
 *
 * @param heap Ponteiro para o heap
 * @param index Índice inicial
 *
 * Pseudocódigo (Cormen et al., 2009, p. 154):
 * MAX-HEAPIFY(A, i)
 *   l = LEFT(i)
 *   r = RIGHT(i)
 *   if l ≤ A.heap-size and A[l] > A[i]
 *       largest = l
 *   else largest = i
 *   if r ≤ A.heap-size and A[r] > A[largest]
 *       largest = r
 *   if largest ≠ i
 *       exchange A[i] with A[largest]
 *       MAX-HEAPIFY(A, largest)
 *
 * Complexidade: O(log n)
 */
void heap_heapify(Heap *heap, size_t index);

/**
 * @brief Constrói heap a partir de array
 *
 * @param element_size Tamanho dos elementos
 * @param array Array de entrada
 * @param size Tamanho do array
 * @param type HEAP_MIN ou HEAP_MAX
 * @param compare Função de comparação
 * @param destroy Função de destruição
 * @return Heap* Heap construído
 *
 * Pseudocódigo (Cormen et al., 2009, p. 157):
 * BUILD-MAX-HEAP(A)
 *   A.heap-size = A.length
 *   for i = ⌊A.length/2⌋ downto 1
 *       MAX-HEAPIFY(A, i)
 *
 * Análise: Σ(i=0 to h) (n/2^(i+1)) * O(i) = O(n) [não O(n log n)!]
 * Referência: Cormen p. 159, Theorem 6.3
 *
 * Complexidade: O(n) - análise tight
 */
Heap* heap_build(size_t element_size, const void *array, size_t size,
                 HeapType type, CompareFn compare, DestroyFn destroy);

// ============================================================================
// CONSULTAS
// ============================================================================

/**
 * @brief Verifica se heap está vazio
 */
bool heap_is_empty(const Heap *heap);

/**
 * @brief Retorna o número de elementos
 */
size_t heap_size(const Heap *heap);

/**
 * @brief Retorna a capacidade
 */
size_t heap_capacity(const Heap *heap);

/**
 * @brief Limpa o heap
 */
void heap_clear(Heap *heap);

// ============================================================================
// HEAPSORT
// ============================================================================

/**
 * @brief Ordena array in-place usando heapsort
 *
 * @param array Array a ordenar
 * @param element_size Tamanho dos elementos
 * @param size Número de elementos
 * @param compare Função de comparação
 *
 * Pseudocódigo (Cormen et al., 2009, p. 160):
 * HEAPSORT(A)
 *   BUILD-MAX-HEAP(A)
 *   for i = A.length downto 2
 *       exchange A[1] with A[i]
 *       A.heap-size = A.heap-size - 1
 *       MAX-HEAPIFY(A, 1)
 *
 * Complexidade: O(n log n) pior caso
 * Espaço: O(1) (in-place)
 */
void heap_sort(void *array, size_t element_size, size_t size, CompareFn compare);

// ============================================================================
// CONVERSÕES
// ============================================================================

/**
 * @brief Converte heap para array ordenado
 *
 * Complexidade: O(n log n) via extract sucessivos
 */
DataStructureError heap_to_array(const Heap *heap, void **array, size_t *size);

/**
 * @brief Imprime heap
 */
void heap_print(const Heap *heap, PrintFn print);

#endif // HEAP_H
