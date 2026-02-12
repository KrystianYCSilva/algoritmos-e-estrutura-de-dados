/**
 * @file array_list.c
 * @brief Implementação de ArrayList (Dynamic Array / Vector) genérico
 *
 * Array dinâmico com crescimento automático. Oferece acesso O(1) por índice
 * e inserção O(1) amortizada no final.
 *
 * Referências:
 * - Goodrich et al. (2011), Chapter 7 - Lists and Iterators
 * - Cormen et al. (2009), Chapter 17 - Amortized Analysis
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/array_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA DO ARRAYLIST
// ============================================================================

struct ArrayList {
    void *array;              // Array de elementos
    size_t element_size;      // Tamanho de cada elemento
    size_t size;              // Número de elementos
    size_t capacity;          // Capacidade atual
    GrowthStrategy growth;    // Estratégia de crescimento
    DestroyFn destroy;        // Função de destruição
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

/**
 * @brief Calcula nova capacidade baseada na estratégia
 */
static size_t calculate_new_capacity(size_t current, GrowthStrategy growth) {
    switch (growth) {
        case GROWTH_DOUBLE:
            return current * 2;

        case GROWTH_1_5:
            return current + (current / 2);

        case GROWTH_FIXED:
            return current + 16;  // Incremento fixo

        default:
            return current * 2;
    }
}

/**
 * @brief Redimensiona o array
 *
 * Análise Amortizada (Cormen Ch. 17):
 * Custo amortizado de n inserções com crescimento 2x = O(n)
 * Série geométrica: 1 + 2 + 4 + ... + n = 2n - 1 < 2n
 *
 * Complexidade: O(n)
 */
static DataStructureError arraylist_resize(ArrayList *list, size_t new_capacity) {
    if (new_capacity < list->size) {
        return DS_ERROR_INVALID_PARAM;
    }

    void *new_array = realloc(list->array, new_capacity * list->element_size);
    if (new_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    list->array = new_array;
    list->capacity = new_capacity;

    return DS_SUCCESS;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

ArrayList* arraylist_create(size_t element_size, size_t initial_capacity, DestroyFn destroy) {
    return arraylist_create_with_growth(element_size, initial_capacity, GROWTH_DOUBLE, destroy);
}

ArrayList* arraylist_create_with_growth(size_t element_size, size_t initial_capacity,
                                        GrowthStrategy growth, DestroyFn destroy) {
    if (element_size == 0) {
        return NULL;
    }

    if (initial_capacity == 0) initial_capacity = 16;

    ArrayList *list = (ArrayList*)malloc(sizeof(ArrayList));
    if (list == NULL) {
        return NULL;
    }

    list->array = malloc(initial_capacity * element_size);
    if (list->array == NULL) {
        free(list);
        return NULL;
    }

    list->element_size = element_size;
    list->size = 0;
    list->capacity = initial_capacity;
    list->growth = growth;
    list->destroy = destroy;

    return list;
}

void arraylist_destroy(ArrayList *list) {
    if (list == NULL) {
        return;
    }

    arraylist_clear(list);
    free(list->array);
    free(list);
}

// ============================================================================
// INSERÇÃO
// ============================================================================

/**
 * @brief Insere no final
 *
 * Análise Amortizada: O(1)
 * - Se não precisa realocar: O(1)
 * - Se precisa realocar: O(n), mas acontece raramente
 * - Amortizado: O(1)
 *
 * Complexidade: O(1) amortizado
 */
DataStructureError arraylist_push_back(ArrayList *list, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    // Verificar se precisa crescer
    if (list->size >= list->capacity) {
        size_t new_capacity = calculate_new_capacity(list->capacity, list->growth);
        DataStructureError err = arraylist_resize(list, new_capacity);
        if (err != DS_SUCCESS) {
            return err;
        }
    }

    // Inserir no final
    void *dest = (char*)list->array + (list->size * list->element_size);
    memcpy(dest, data, list->element_size);

    list->size++;
    return DS_SUCCESS;
}

/**
 * @brief Insere no início
 *
 * Requer deslocar todos os elementos.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_push_front(ArrayList *list, const void *data) {
    return arraylist_insert(list, 0, data);
}

/**
 * @brief Insere em posição específica
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_insert(ArrayList *list, size_t index, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index > list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    // Verificar se precisa crescer
    if (list->size >= list->capacity) {
        size_t new_capacity = calculate_new_capacity(list->capacity, list->growth);
        DataStructureError err = arraylist_resize(list, new_capacity);
        if (err != DS_SUCCESS) {
            return err;
        }
    }

    // Deslocar elementos à direita
    if (index < list->size) {
        void *src = (char*)list->array + (index * list->element_size);
        void *dest = (char*)list->array + ((index + 1) * list->element_size);
        size_t bytes_to_move = (list->size - index) * list->element_size;
        memmove(dest, src, bytes_to_move);
    }

    // Inserir elemento
    void *dest = (char*)list->array + (index * list->element_size);
    memcpy(dest, data, list->element_size);

    list->size++;
    return DS_SUCCESS;
}

// ============================================================================
// REMOÇÃO
// ============================================================================

DataStructureError arraylist_pop_back(ArrayList *list, void *output) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (list->size == 0) {
        return DS_ERROR_EMPTY;
    }

    list->size--;

    void *src = (char*)list->array + (list->size * list->element_size);

    if (output != NULL) {
        memcpy(output, src, list->element_size);
    }

    if (list->destroy != NULL) {
        list->destroy(src);
    }

    return DS_SUCCESS;
}

DataStructureError arraylist_pop_front(ArrayList *list, void *output) {
    return arraylist_remove_at(list, 0, output);
}

DataStructureError arraylist_remove_at(ArrayList *list, size_t index, void *output) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index >= list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    void *src = (char*)list->array + (index * list->element_size);

    if (output != NULL) {
        memcpy(output, src, list->element_size);
    }

    if (list->destroy != NULL) {
        list->destroy(src);
    }

    // Deslocar elementos à esquerda
    if (index < list->size - 1) {
        void *dest = src;
        void *next = (char*)list->array + ((index + 1) * list->element_size);
        size_t bytes_to_move = (list->size - index - 1) * list->element_size;
        memmove(dest, next, bytes_to_move);
    }

    list->size--;
    return DS_SUCCESS;
}

DataStructureError arraylist_remove(ArrayList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ssize_t index = arraylist_find(list, data, compare);
    if (index < 0) {
        return DS_ERROR_NOT_FOUND;
    }

    return arraylist_remove_at(list, (size_t)index, NULL);
}

// ============================================================================
// ACESSO E BUSCA
// ============================================================================

DataStructureError arraylist_get(const ArrayList *list, size_t index, void *output) {
    if (list == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index >= list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    void *src = (char*)list->array + (index * list->element_size);
    memcpy(output, src, list->element_size);

    return DS_SUCCESS;
}

void* arraylist_get_ptr(const ArrayList *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    return (char*)list->array + (index * list->element_size);
}

DataStructureError arraylist_set(ArrayList *list, size_t index, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index >= list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    void *dest = (char*)list->array + (index * list->element_size);

    if (list->destroy != NULL) {
        list->destroy(dest);
    }

    memcpy(dest, data, list->element_size);

    return DS_SUCCESS;
}

ssize_t arraylist_find(const ArrayList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return -1;
    }

    for (size_t i = 0; i < list->size; i++) {
        void *elem = (char*)list->array + (i * list->element_size);
        if (compare(elem, data) == 0) {
            return (ssize_t)i;
        }
    }

    return -1;
}

/**
 * @brief Busca binária (requer array ordenado)
 *
 * Pseudocódigo (Cormen p. 799):
 * BINARY-SEARCH(A, n, x)
 *   low = 1
 *   high = n
 *   while low <= high
 *       mid = ⌊(low + high)/2⌋
 *       if x == A[mid]
 *           return mid
 *       else if x < A[mid]
 *           high = mid - 1
 *       else low = mid + 1
 *   return NIL
 *
 * Complexidade: O(log n)
 */
ssize_t arraylist_binary_search(const ArrayList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return -1;
    }

    ssize_t low = 0;
    ssize_t high = (ssize_t)list->size - 1;

    while (low <= high) {
        ssize_t mid = low + (high - low) / 2;
        void *mid_elem = (char*)list->array + (mid * list->element_size);

        int cmp = compare(data, mid_elem);

        if (cmp == 0) {
            return mid;  // Encontrado
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;  // Não encontrado
}

// ============================================================================
// CONSULTAS E UTILIDADES
// ============================================================================

bool arraylist_is_empty(const ArrayList *list) {
    return (list == NULL || list->size == 0);
}

size_t arraylist_size(const ArrayList *list) {
    return (list == NULL) ? 0 : list->size;
}

size_t arraylist_capacity(const ArrayList *list) {
    return (list == NULL) ? 0 : list->capacity;
}

void arraylist_clear(ArrayList *list) {
    if (list == NULL) {
        return;
    }

    if (list->destroy != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            void *elem = (char*)list->array + (i * list->element_size);
            list->destroy(elem);
        }
    }

    list->size = 0;
}

DataStructureError arraylist_shrink_to_fit(ArrayList *list) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (list->size == list->capacity) {
        return DS_SUCCESS;  // Já está no tamanho ideal
    }

    size_t new_capacity = (list->size > 0) ? list->size : 1;
    return arraylist_resize(list, new_capacity);
}

DataStructureError arraylist_reserve(ArrayList *list, size_t new_capacity) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (new_capacity <= list->capacity) {
        return DS_SUCCESS;  // Já tem capacidade suficiente
    }

    return arraylist_resize(list, new_capacity);
}

void arraylist_print(const ArrayList *list, PrintFn print) {
    if (list == NULL || print == NULL) {
        return;
    }

    printf("ArrayList(size=%zu, capacity=%zu, growth=%s) [",
           list->size, list->capacity,
           list->growth == GROWTH_DOUBLE ? "2x" :
           list->growth == GROWTH_1_5 ? "1.5x" : "FIXED");

    for (size_t i = 0; i < list->size; i++) {
        if (i > 0) printf(", ");
        void *elem = (char*)list->array + (i * list->element_size);
        print(elem);
    }

    printf("]\n");
}

void arraylist_reverse(ArrayList *list) {
    if (list == NULL || list->size <= 1) {
        return;
    }

    void *temp = malloc(list->element_size);
    if (temp == NULL) {
        return;
    }

    size_t left = 0;
    size_t right = list->size - 1;

    while (left < right) {
        void *left_elem = (char*)list->array + (left * list->element_size);
        void *right_elem = (char*)list->array + (right * list->element_size);

        // Swap
        memcpy(temp, left_elem, list->element_size);
        memcpy(left_elem, right_elem, list->element_size);
        memcpy(right_elem, temp, list->element_size);

        left++;
        right--;
    }

    free(temp);
}

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

ArrayList* arraylist_clone(const ArrayList *list, CopyFn copy) {
    if (list == NULL) {
        return NULL;
    }

    ArrayList *new_list = arraylist_create_with_growth(
        list->element_size, list->capacity, list->growth, list->destroy);

    if (new_list == NULL) {
        return NULL;
    }

    // Copiar elementos
    for (size_t i = 0; i < list->size; i++) {
        void *src = (char*)list->array + (i * list->element_size);

        if (copy != NULL) {
            void *copied = copy(src);
            if (copied != NULL) {
                arraylist_push_back(new_list, copied);
                free(copied);  // copy retorna nova alocação
            }
        } else {
            arraylist_push_back(new_list, src);
        }
    }

    return new_list;
}

void arraylist_sort(ArrayList *list, CompareFn compare) {
    if (list == NULL || compare == NULL || list->size <= 1) {
        return;
    }

    // Usar qsort da stdlib
    qsort(list->array, list->size, list->element_size, compare);
}

void* arraylist_data(const ArrayList *list) {
    return (list == NULL) ? NULL : list->array;
}
