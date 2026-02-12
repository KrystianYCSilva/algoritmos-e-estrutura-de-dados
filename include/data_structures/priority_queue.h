/**
 * @file priority_queue.h
 * @brief Fila de Prioridade (Priority Queue) implementada sobre Heap
 *
 * Priority Queue é uma estrutura onde cada elemento tem uma prioridade.
 * Elementos com maior prioridade são removidos primeiro.
 *
 * Implementação: Binary Heap (min-heap ou max-heap)
 *
 * Complexidade:
 * - Insert: O(log n)
 * - Extract (remove highest priority): O(log n)
 * - Peek (get highest priority): O(1)
 * - Update priority: O(log n)
 *
 * Aplicações:
 * - Dijkstra's algorithm
 * - A* pathfinding
 * - Huffman coding
 * - Event-driven simulation
 * - Operating system task scheduling
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 6 - Priority Queues
 * - Sedgewick & Wayne (2011), Section 2.4
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct PriorityQueue PriorityQueue;

typedef enum {
    PQ_MIN,  /**< Min priority queue (menor prioridade primeiro) */
    PQ_MAX   /**< Max priority queue (maior prioridade primeiro) */
} PriorityQueueType;

/**
 * @brief Cria uma priority queue
 *
 * @param element_size Tamanho dos elementos
 * @param initial_capacity Capacidade inicial
 * @param type PQ_MIN ou PQ_MAX
 * @param compare Função de comparação (define prioridade)
 * @param destroy Função de destruição
 */
PriorityQueue* pq_create(size_t element_size, size_t initial_capacity,
                         PriorityQueueType type, CompareFn compare, DestroyFn destroy);

void pq_destroy(PriorityQueue *pq);

/**
 * @brief Insere elemento com prioridade
 *
 * Complexidade: O(log n)
 */
DataStructureError pq_insert(PriorityQueue *pq, const void *data);

/**
 * @brief Remove e retorna elemento de maior prioridade
 *
 * Complexidade: O(log n)
 */
DataStructureError pq_extract(PriorityQueue *pq, void *output);

/**
 * @brief Retorna elemento de maior prioridade sem remover
 *
 * Complexidade: O(1)
 */
DataStructureError pq_peek(const PriorityQueue *pq, void *output);

/**
 * @brief Atualiza prioridade de um elemento
 *
 * Complexidade: O(n) para encontrar + O(log n) para atualizar = O(n)
 * Para O(log n), use indexed priority queue
 */
DataStructureError pq_update_priority(PriorityQueue *pq, const void *old_data,
                                      const void *new_data);

bool pq_is_empty(const PriorityQueue *pq);
size_t pq_size(const PriorityQueue *pq);
void pq_clear(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H
