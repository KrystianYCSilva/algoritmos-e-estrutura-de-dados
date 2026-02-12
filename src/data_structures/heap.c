/**
 * @file heap.c
 * @brief Implementação de Heap Binário (Binary Heap) genérico
 *
 * Implementa heap binário como array implícito com suporte a Min-Heap e Max-Heap.
 * Para índice i: parent = (i-1)/2, left = 2i+1, right = 2i+2
 *
 * Referências:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 6 - Heapsort e Priority Queues
 * - Williams, J. W. J. (1964). "Algorithm 232: Heapsort",
 *   Communications of the ACM 7(6): 347–348
 * - Floyd, R. W. (1964). "Algorithm 245: Treesort 3",
 *   Communications of the ACM 7(12): 701
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA DO HEAP
// ============================================================================

struct Heap {
    void *array;
    size_t element_size;
    size_t size;
    size_t capacity;
    HeapType type;
    CompareFn compare;
    DestroyFn destroy;
};

#define HEAP_MIN_CAPACITY 16

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

static void heap_swap(void *a, void *b, size_t element_size) {
    unsigned char *pa = (unsigned char *)a;
    unsigned char *pb = (unsigned char *)b;
    unsigned char tmp;
    for (size_t i = 0; i < element_size; i++) {
        tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

static void *heap_element_at(const Heap *heap, size_t index) {
    return (char *)heap->array + (index * heap->element_size);
}

/**
 * Compara dois elementos respeitando o tipo de heap.
 * Retorna positivo se a tem "maior prioridade" que b.
 * Para HEAP_MAX: compare(a,b) > 0 significa a tem prioridade.
 * Para HEAP_MIN: compare(a,b) < 0 significa a tem prioridade, então invertemos.
 */
static int heap_compare(const Heap *heap, const void *a, const void *b) {
    int result = heap->compare(a, b);
    return (heap->type == HEAP_MAX) ? result : -result;
}

/**
 * @brief Bubble-up: restaura propriedade heap subindo
 *
 * Pseudocódigo (Cormen et al., 2009, p. 164):
 * while i > 0 and A[PARENT(i)] has less priority than A[i]
 *     exchange A[i] with A[PARENT(i)]
 *     i = PARENT(i)
 *
 * Complexidade: O(log n)
 */
static void heap_up(Heap *heap, size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap_compare(heap, heap_element_at(heap, index),
                         heap_element_at(heap, parent)) > 0) {
            heap_swap(heap_element_at(heap, index),
                      heap_element_at(heap, parent),
                      heap->element_size);
            index = parent;
        } else {
            break;
        }
    }
}

/**
 * @brief Bubble-down: restaura propriedade heap descendo (MAX-HEAPIFY)
 *
 * Pseudocódigo (Cormen et al., 2009, p. 154):
 * MAX-HEAPIFY(A, i)
 *   l = LEFT(i), r = RIGHT(i)
 *   largest = i
 *   if l ≤ heap-size and A[l] has more priority than A[largest]
 *       largest = l
 *   if r ≤ heap-size and A[r] has more priority than A[largest]
 *       largest = r
 *   if largest ≠ i
 *       exchange A[i] with A[largest]
 *       MAX-HEAPIFY(A, largest)
 *
 * Complexidade: O(log n)
 */
static void heap_down(Heap *heap, size_t index) {
    while (1) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t target = index;

        if (left < heap->size &&
            heap_compare(heap, heap_element_at(heap, left),
                         heap_element_at(heap, target)) > 0) {
            target = left;
        }

        if (right < heap->size &&
            heap_compare(heap, heap_element_at(heap, right),
                         heap_element_at(heap, target)) > 0) {
            target = right;
        }

        if (target == index) {
            break;
        }

        heap_swap(heap_element_at(heap, index),
                  heap_element_at(heap, target),
                  heap->element_size);
        index = target;
    }
}

static DataStructureError heap_ensure_capacity(Heap *heap) {
    if (heap->size < heap->capacity) {
        return DS_SUCCESS;
    }

    size_t new_capacity = heap->capacity * 2;
    if (new_capacity < HEAP_MIN_CAPACITY) {
        new_capacity = HEAP_MIN_CAPACITY;
    }

    void *new_array = realloc(heap->array, new_capacity * heap->element_size);
    if (new_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    heap->array = new_array;
    heap->capacity = new_capacity;

    return DS_SUCCESS;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

Heap* heap_create(size_t element_size, size_t initial_capacity,
                  HeapType type, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0 || compare == NULL) {
        return NULL;
    }

    Heap *heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL) {
        return NULL;
    }

    if (initial_capacity < HEAP_MIN_CAPACITY) {
        initial_capacity = HEAP_MIN_CAPACITY;
    }

    heap->array = malloc(initial_capacity * element_size);
    if (heap->array == NULL) {
        free(heap);
        return NULL;
    }

    heap->element_size = element_size;
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->type = type;
    heap->compare = compare;
    heap->destroy = destroy;

    return heap;
}

void heap_destroy(Heap *heap) {
    if (heap == NULL) {
        return;
    }

    if (heap->destroy != NULL) {
        for (size_t i = 0; i < heap->size; i++) {
            heap->destroy(heap_element_at(heap, i));
        }
    }

    free(heap->array);
    free(heap);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

DataStructureError heap_insert(Heap *heap, const void *data) {
    if (heap == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    DataStructureError err = heap_ensure_capacity(heap);
    if (err != DS_SUCCESS) {
        return err;
    }

    memcpy(heap_element_at(heap, heap->size), data, heap->element_size);
    heap->size++;
    heap_up(heap, heap->size - 1);

    return DS_SUCCESS;
}

DataStructureError heap_extract(Heap *heap, void *output) {
    if (heap == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (heap->size == 0) {
        return DS_ERROR_EMPTY;
    }

    if (output != NULL) {
        memcpy(output, heap_element_at(heap, 0), heap->element_size);
    }

    heap->size--;

    if (heap->size > 0) {
        memcpy(heap_element_at(heap, 0),
               heap_element_at(heap, heap->size),
               heap->element_size);
        heap_down(heap, 0);
    }

    return DS_SUCCESS;
}

DataStructureError heap_peek(const Heap *heap, void *output) {
    if (heap == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (heap->size == 0) {
        return DS_ERROR_EMPTY;
    }

    memcpy(output, heap_element_at(heap, 0), heap->element_size);

    return DS_SUCCESS;
}

DataStructureError heap_update(Heap *heap, size_t index, const void *new_data) {
    if (heap == NULL || new_data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index >= heap->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    int cmp = heap_compare(heap, new_data, heap_element_at(heap, index));
    memcpy(heap_element_at(heap, index), new_data, heap->element_size);

    if (cmp > 0) {
        heap_up(heap, index);
    } else if (cmp < 0) {
        heap_down(heap, index);
    }

    return DS_SUCCESS;
}

// ============================================================================
// HEAPIFY
// ============================================================================

void heap_heapify(Heap *heap, size_t index) {
    if (heap == NULL || index >= heap->size) {
        return;
    }

    heap_down(heap, index);
}

Heap* heap_build(size_t element_size, const void *array, size_t size,
                 HeapType type, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0 || array == NULL || compare == NULL) {
        return NULL;
    }

    size_t capacity = size;
    if (capacity < HEAP_MIN_CAPACITY) {
        capacity = HEAP_MIN_CAPACITY;
    }

    Heap *heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->array = malloc(capacity * element_size);
    if (heap->array == NULL) {
        free(heap);
        return NULL;
    }

    memcpy(heap->array, array, size * element_size);
    heap->element_size = element_size;
    heap->size = size;
    heap->capacity = capacity;
    heap->type = type;
    heap->compare = compare;
    heap->destroy = destroy;

    /* BUILD-HEAP (Cormen et al., 2009, p. 157):
     * for i = floor(n/2) - 1 downto 0
     *     HEAPIFY(A, i)
     * Complexidade: O(n) - análise tight */
    if (size > 1) {
        for (size_t i = size / 2; i > 0; i--) {
            heap_down(heap, i - 1);
        }
    }

    return heap;
}

// ============================================================================
// CONSULTAS
// ============================================================================

bool heap_is_empty(const Heap *heap) {
    return (heap == NULL || heap->size == 0);
}

size_t heap_size(const Heap *heap) {
    return (heap == NULL) ? 0 : heap->size;
}

size_t heap_capacity(const Heap *heap) {
    return (heap == NULL) ? 0 : heap->capacity;
}

void heap_clear(Heap *heap) {
    if (heap == NULL) {
        return;
    }

    if (heap->destroy != NULL) {
        for (size_t i = 0; i < heap->size; i++) {
            heap->destroy(heap_element_at(heap, i));
        }
    }

    heap->size = 0;
}

// ============================================================================
// HEAPSORT
// ============================================================================

void heap_sort(void *array, size_t element_size, size_t size, CompareFn compare) {
    if (array == NULL || element_size == 0 || size <= 1 || compare == NULL) {
        return;
    }

    /* Usamos um heap temporário no estilo struct para reaproveitar heap_down.
     * Construímos um MAX-HEAP e depois extraímos em ordem. */
    Heap temp;
    temp.array = array;
    temp.element_size = element_size;
    temp.size = size;
    temp.capacity = size;
    temp.type = HEAP_MAX;
    temp.compare = compare;
    temp.destroy = NULL;

    /* BUILD-MAX-HEAP (Cormen et al., 2009, p. 157) */
    for (size_t i = size / 2; i > 0; i--) {
        heap_down(&temp, i - 1);
    }

    /* HEAPSORT extract loop (Cormen et al., 2009, p. 160):
     * for i = n-1 downto 1
     *     exchange A[0] with A[i]
     *     heap-size = heap-size - 1
     *     MAX-HEAPIFY(A, 0) */
    for (size_t i = size - 1; i > 0; i--) {
        heap_swap((char *)array,
                  (char *)array + (i * element_size),
                  element_size);
        temp.size--;
        heap_down(&temp, 0);
    }
}

// ============================================================================
// CONVERSÕES
// ============================================================================

DataStructureError heap_to_array(const Heap *heap, void **array, size_t *size) {
    if (heap == NULL || array == NULL || size == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (heap->size == 0) {
        *array = NULL;
        *size = 0;
        return DS_SUCCESS;
    }

    Heap *clone = (Heap *)malloc(sizeof(Heap));
    if (clone == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    clone->array = malloc(heap->size * heap->element_size);
    if (clone->array == NULL) {
        free(clone);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    memcpy(clone->array, heap->array, heap->size * heap->element_size);
    clone->element_size = heap->element_size;
    clone->size = heap->size;
    clone->capacity = heap->size;
    clone->type = heap->type;
    clone->compare = heap->compare;
    clone->destroy = NULL;

    void *result = malloc(heap->size * heap->element_size);
    if (result == NULL) {
        free(clone->array);
        free(clone);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    *size = heap->size;

    for (size_t i = 0; i < *size; i++) {
        heap_extract(clone, (char *)result + (i * heap->element_size));
    }

    free(clone->array);
    free(clone);

    *array = result;
    return DS_SUCCESS;
}

// ============================================================================
// IMPRESSÃO
// ============================================================================

void heap_print(const Heap *heap, PrintFn print) {
    if (heap == NULL || print == NULL) {
        return;
    }

    const char *type_str = (heap->type == HEAP_MIN) ? "Min" : "Max";
    printf("Heap(%s, size=%zu, capacity=%zu) [", type_str, heap->size, heap->capacity);

    for (size_t i = 0; i < heap->size; i++) {
        if (i > 0) printf(", ");
        print(heap_element_at(heap, i));
    }

    printf("]\n");
}
