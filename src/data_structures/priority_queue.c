/**
 * @file priority_queue.c
 * @brief Implementação de Fila de Prioridade sobre Heap Binário
 *
 * Priority Queue é um wrapper fino sobre o Heap, mapeando operações
 * de prioridade para operações do heap subjacente.
 *
 * Referências:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 6.5 - Priority Queues
 * - Sedgewick, R. & Wayne, K. (2011). "Algorithms" (4th ed.),
 *   Section 2.4 - Priority Queues
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/priority_queue.h"
#include "data_structures/heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA
// ============================================================================

struct PriorityQueue {
    Heap *heap;
    PriorityQueueType type;
    size_t element_size;
    CompareFn compare;
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

static HeapType pq_type_to_heap_type(PriorityQueueType type) {
    return (type == PQ_MIN) ? HEAP_MIN : HEAP_MAX;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

PriorityQueue* pq_create(size_t element_size, size_t initial_capacity,
                         PriorityQueueType type, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0 || compare == NULL) {
        return NULL;
    }

    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (pq == NULL) {
        return NULL;
    }

    pq->heap = heap_create(element_size, initial_capacity,
                           pq_type_to_heap_type(type), compare, destroy);
    if (pq->heap == NULL) {
        free(pq);
        return NULL;
    }

    pq->type = type;
    pq->element_size = element_size;
    pq->compare = compare;
    return pq;
}

void pq_destroy(PriorityQueue *pq) {
    if (pq == NULL) {
        return;
    }

    heap_destroy(pq->heap);
    free(pq);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

DataStructureError pq_insert(PriorityQueue *pq, const void *data) {
    if (pq == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    return heap_insert(pq->heap, data);
}

DataStructureError pq_extract(PriorityQueue *pq, void *output) {
    if (pq == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    return heap_extract(pq->heap, output);
}

DataStructureError pq_peek(const PriorityQueue *pq, void *output) {
    if (pq == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    return heap_peek(pq->heap, output);
}

/**
 * O(n) scan to find element + O(log n) heap_update.
 * Extracts all elements to find a match, then rebuilds with updated value.
 */
DataStructureError pq_update_priority(PriorityQueue *pq, const void *old_data,
                                      const void *new_data) {
    if (pq == NULL || old_data == NULL || new_data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    size_t n = heap_size(pq->heap);
    if (n == 0) {
        return DS_ERROR_NOT_FOUND;
    }

    void *buffer = malloc(n * pq->element_size);
    if (buffer == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t count = 0;
    bool found = false;

    for (size_t i = 0; i < n; i++) {
        heap_extract(pq->heap, (char *)buffer + i * pq->element_size);
        count++;
    }

    for (size_t i = 0; i < count; i++) {
        void *elem = (char *)buffer + i * pq->element_size;
        if (!found && pq->compare(elem, old_data) == 0) {
            memcpy(elem, new_data, pq->element_size);
            found = true;
        }
    }

    for (size_t i = 0; i < count; i++) {
        heap_insert(pq->heap, (char *)buffer + i * pq->element_size);
    }

    free(buffer);

    return found ? DS_SUCCESS : DS_ERROR_NOT_FOUND;
}

// ============================================================================
// CONSULTAS
// ============================================================================

bool pq_is_empty(const PriorityQueue *pq) {
    if (pq == NULL) {
        return true;
    }

    return heap_is_empty(pq->heap);
}

size_t pq_size(const PriorityQueue *pq) {
    if (pq == NULL) {
        return 0;
    }

    return heap_size(pq->heap);
}

void pq_clear(PriorityQueue *pq) {
    if (pq == NULL) {
        return;
    }

    heap_clear(pq->heap);
}
