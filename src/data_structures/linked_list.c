/**
 * @file linked_list.c
 * @brief Implementação de Lista Encadeada (Linked List) genérica
 *
 * Implementa três variantes:
 * - LIST_SINGLY: Lista simplesmente encadeada
 * - LIST_DOUBLY: Lista duplamente encadeada
 * - LIST_CIRCULAR: Lista circular (duplamente encadeada)
 *
 * Referências:
 * - Knuth TAOCP Vol 1, Section 2.2 - Linear Lists
 * - Cormen et al. (2009), Chapter 10.2 - Linked Lists
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA DA LINKED LIST
// ============================================================================

/**
 * @brief Nó da lista encadeada
 */
struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;  // NULL para LIST_SINGLY
};

/**
 * @brief Estrutura interna da lista
 */
struct LinkedList {
    ListType type;
    size_t element_size;
    size_t size;
    DestroyFn destroy;

    ListNode *head;          // Primeiro nó
    ListNode *tail;          // Último nó
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

/**
 * @brief Cria um novo nó
 */
static ListNode* create_node(size_t element_size, const void *data) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) {
        return NULL;
    }

    node->data = malloc(element_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, element_size);
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/**
 * @brief Libera um nó
 */
static void destroy_node(LinkedList *list, ListNode *node) {
    if (node == NULL) {
        return;
    }

    if (list->destroy != NULL && node->data != NULL) {
        list->destroy(node->data);
    }

    free(node->data);
    free(node);
}

/**
 * @brief Retorna o nó no índice especificado
 *
 * Complexidade: O(n)
 */
static ListNode* get_node_at(const LinkedList *list, size_t index) {
    if (index >= list->size) {
        return NULL;
    }

    ListNode *current = list->head;

    // Otimização: se doubly/circular e index > size/2, começar do tail
    if ((list->type == LIST_DOUBLY || list->type == LIST_CIRCULAR) &&
        index > list->size / 2) {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    } else {
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    }

    return current;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova lista encadeada
 *
 * Complexidade: O(1)
 */
LinkedList* list_create(size_t element_size, ListType type, DestroyFn destroy) {
    if (element_size == 0) {
        return NULL;
    }

    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }

    list->element_size = element_size;
    list->type = type;
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

/**
 * @brief Destrói a lista
 *
 * Complexidade: O(n)
 */
void list_destroy(LinkedList *list) {
    if (list == NULL) {
        return;
    }

    list_clear(list);
    free(list);
}

// ============================================================================
// INSERÇÃO
// ============================================================================

/**
 * @brief Insere no início
 *
 * Pseudocódigo (Cormen p. 238):
 * LIST-INSERT(L, x)
 *   x.next = L.head
 *   if L.head ≠ NIL
 *       L.head.prev = x
 *   L.head = x
 *   x.prev = NIL
 *
 * Complexidade: O(1)
 */
DataStructureError list_push_front(LinkedList *list, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *node = create_node(list->element_size, data);
    if (node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    if (list->head == NULL) {
        // Lista vazia
        list->head = list->tail = node;

        if (list->type == LIST_CIRCULAR) {
            node->next = node;
            node->prev = node;
        }
    } else {
        node->next = list->head;

        if (list->type == LIST_DOUBLY || list->type == LIST_CIRCULAR) {
            list->head->prev = node;
        }

        list->head = node;

        if (list->type == LIST_CIRCULAR) {
            node->prev = list->tail;
            list->tail->next = node;
        }
    }

    list->size++;
    return DS_SUCCESS;
}

/**
 * @brief Insere no final
 *
 * Complexidade: O(1)
 */
DataStructureError list_push_back(LinkedList *list, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *node = create_node(list->element_size, data);
    if (node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    if (list->tail == NULL) {
        // Lista vazia
        list->head = list->tail = node;

        if (list->type == LIST_CIRCULAR) {
            node->next = node;
            node->prev = node;
        }
    } else {
        if (list->type == LIST_DOUBLY || list->type == LIST_CIRCULAR) {
            node->prev = list->tail;
        }

        list->tail->next = node;
        list->tail = node;

        if (list->type == LIST_CIRCULAR) {
            node->next = list->head;
            list->head->prev = node;
        }
    }

    list->size++;
    return DS_SUCCESS;
}

/**
 * @brief Insere em posição específica
 *
 * Complexidade: O(n)
 */
DataStructureError list_insert_at(LinkedList *list, size_t index, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index > list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    if (index == 0) {
        return list_push_front(list, data);
    }

    if (index == list->size) {
        return list_push_back(list, data);
    }

    // Inserir no meio
    ListNode *current = get_node_at(list, index);
    if (current == NULL) {
        return DS_ERROR_INVALID_INDEX;
    }

    return list_insert_before(list, current, data);
}

/**
 * @brief Insere após um nó
 *
 * Complexidade: O(1)
 */
DataStructureError list_insert_after(LinkedList *list, ListNode *node, const void *data) {
    if (list == NULL || node == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *new_node = create_node(list->element_size, data);
    if (new_node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    new_node->next = node->next;
    node->next = new_node;

    if (list->type == LIST_DOUBLY || list->type == LIST_CIRCULAR) {
        new_node->prev = node;
        if (new_node->next != NULL && list->type != LIST_CIRCULAR) {
            new_node->next->prev = new_node;
        }
    }

    if (node == list->tail && list->type != LIST_CIRCULAR) {
        list->tail = new_node;
    }

    if (list->type == LIST_CIRCULAR && node == list->tail) {
        list->tail = new_node;
        list->head->prev = new_node;
    }

    list->size++;
    return DS_SUCCESS;
}

/**
 * @brief Insere antes de um nó
 *
 * Complexidade: O(1) para DOUBLY/CIRCULAR, O(n) para SINGLY
 */
DataStructureError list_insert_before(LinkedList *list, ListNode *node, const void *data) {
    if (list == NULL || node == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *new_node = create_node(list->element_size, data);
    if (new_node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    if (list->type == LIST_SINGLY) {
        // Para singly linked, precisamos percorrer para achar o prev
        if (node == list->head) {
            return list_push_front(list, data);
        }

        ListNode *prev = list->head;
        while (prev != NULL && prev->next != node) {
            prev = prev->next;
        }

        if (prev == NULL) {
            destroy_node(list, new_node);
            return DS_ERROR_NOT_FOUND;
        }

        new_node->next = node;
        prev->next = new_node;
    } else {
        // DOUBLY ou CIRCULAR
        new_node->next = node;
        new_node->prev = node->prev;

        if (node->prev != NULL) {
            node->prev->next = new_node;
        }

        node->prev = new_node;

        if (node == list->head) {
            list->head = new_node;
            if (list->type == LIST_CIRCULAR) {
                list->tail->next = new_node;
            }
        }
    }

    list->size++;
    return DS_SUCCESS;
}

// ============================================================================
// REMOÇÃO
// ============================================================================

/**
 * @brief Remove do início
 *
 * Complexidade: O(1)
 */
DataStructureError list_pop_front(LinkedList *list, void *output) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (list->size == 0) {
        return DS_ERROR_EMPTY;
    }

    ListNode *node = list->head;

    if (output != NULL) {
        memcpy(output, node->data, list->element_size);
    }

    if (list->size == 1) {
        // Único elemento
        list->head = list->tail = NULL;
    } else {
        list->head = node->next;

        if (list->type == LIST_DOUBLY || list->type == LIST_CIRCULAR) {
            list->head->prev = (list->type == LIST_CIRCULAR) ? list->tail : NULL;
        }

        if (list->type == LIST_CIRCULAR) {
            list->tail->next = list->head;
        }
    }

    destroy_node(list, node);
    list->size--;

    return DS_SUCCESS;
}

/**
 * @brief Remove do final
 *
 * Complexidade: O(1) para DOUBLY/CIRCULAR, O(n) para SINGLY
 */
DataStructureError list_pop_back(LinkedList *list, void *output) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (list->size == 0) {
        return DS_ERROR_EMPTY;
    }

    ListNode *node = list->tail;

    if (output != NULL) {
        memcpy(output, node->data, list->element_size);
    }

    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        if (list->type == LIST_SINGLY) {
            // Precisamos percorrer até o penúltimo
            ListNode *prev = list->head;
            while (prev->next != list->tail) {
                prev = prev->next;
            }
            prev->next = NULL;
            list->tail = prev;
        } else {
            // DOUBLY ou CIRCULAR
            list->tail = node->prev;
            list->tail->next = (list->type == LIST_CIRCULAR) ? list->head : NULL;

            if (list->type == LIST_CIRCULAR) {
                list->head->prev = list->tail;
            }
        }
    }

    destroy_node(list, node);
    list->size--;

    return DS_SUCCESS;
}

/**
 * @brief Remove em posição específica
 *
 * Complexidade: O(n)
 */
DataStructureError list_remove_at(LinkedList *list, size_t index, void *output) {
    if (list == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (index >= list->size) {
        return DS_ERROR_INVALID_INDEX;
    }

    if (index == 0) {
        return list_pop_front(list, output);
    }

    if (index == list->size - 1) {
        return list_pop_back(list, output);
    }

    ListNode *node = get_node_at(list, index);
    if (node == NULL) {
        return DS_ERROR_INVALID_INDEX;
    }

    return list_remove_node(list, node);
}

/**
 * @brief Remove um nó específico
 *
 * Pseudocódigo (Cormen p. 238):
 * LIST-DELETE(L, x)
 *   if x.prev ≠ NIL
 *       x.prev.next = x.next
 *   else L.head = x.next
 *   if x.next ≠ NIL
 *       x.next.prev = x.prev
 *
 * Complexidade: O(1) para DOUBLY/CIRCULAR, O(n) para SINGLY
 */
DataStructureError list_remove_node(LinkedList *list, ListNode *node) {
    if (list == NULL || node == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    // Casos especiais
    if (node == list->head) {
        return list_pop_front(list, NULL);
    }

    if (node == list->tail) {
        return list_pop_back(list, NULL);
    }

    // Remover do meio
    if (list->type == LIST_SINGLY) {
        // Precisamos encontrar o nó anterior
        ListNode *prev = list->head;
        while (prev != NULL && prev->next != node) {
            prev = prev->next;
        }

        if (prev == NULL) {
            return DS_ERROR_NOT_FOUND;
        }

        prev->next = node->next;
    } else {
        // DOUBLY ou CIRCULAR
        if (node->prev != NULL) {
            node->prev->next = node->next;
        }

        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
    }

    destroy_node(list, node);
    list->size--;

    return DS_SUCCESS;
}

/**
 * @brief Remove primeira ocorrência de um valor
 *
 * Complexidade: O(n)
 */
DataStructureError list_remove(LinkedList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *node = list_find(list, data, compare);
    if (node == NULL) {
        return DS_ERROR_NOT_FOUND;
    }

    return list_remove_node(list, node);
}

// ============================================================================
// ACESSO E BUSCA
// ============================================================================

DataStructureError list_get(const LinkedList *list, size_t index, void *output) {
    if (list == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *node = get_node_at(list, index);
    if (node == NULL) {
        return DS_ERROR_INVALID_INDEX;
    }

    memcpy(output, node->data, list->element_size);
    return DS_SUCCESS;
}

DataStructureError list_set(LinkedList *list, size_t index, const void *data) {
    if (list == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    ListNode *node = get_node_at(list, index);
    if (node == NULL) {
        return DS_ERROR_INVALID_INDEX;
    }

    // Destruir dado antigo se necessário
    if (list->destroy != NULL) {
        list->destroy(node->data);
    }

    memcpy(node->data, data, list->element_size);
    return DS_SUCCESS;
}

ListNode* list_find(const LinkedList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return NULL;
    }

    ListNode *current = list->head;
    size_t count = 0;

    while (current != NULL && count < list->size) {
        if (compare(current->data, data) == 0) {
            return current;
        }
        current = current->next;
        count++;

        // Para circular, evitar loop infinito
        if (list->type == LIST_CIRCULAR && count >= list->size) {
            break;
        }
    }

    return NULL;
}

ssize_t list_index_of(const LinkedList *list, const void *data, CompareFn compare) {
    if (list == NULL || data == NULL || compare == NULL) {
        return -1;
    }

    ListNode *current = list->head;
    for (size_t i = 0; i < list->size; i++) {
        if (compare(current->data, data) == 0) {
            return (ssize_t)i;
        }
        current = current->next;
    }

    return -1;
}

// ============================================================================
// CONSULTAS E UTILIDADES
// ============================================================================

bool list_is_empty(const LinkedList *list) {
    return (list == NULL || list->size == 0);
}

size_t list_size(const LinkedList *list) {
    return (list == NULL) ? 0 : list->size;
}

void list_clear(LinkedList *list) {
    if (list == NULL) {
        return;
    }

    while (!list_is_empty(list)) {
        list_pop_front(list, NULL);
    }
}

void list_print(const LinkedList *list, PrintFn print) {
    if (list == NULL || print == NULL) {
        return;
    }

    printf("LinkedList(%s, size=%zu) [",
           (list->type == LIST_SINGLY ? "SINGLY" :
            list->type == LIST_DOUBLY ? "DOUBLY" : "CIRCULAR"),
           list->size);

    ListNode *current = list->head;
    size_t count = 0;

    while (current != NULL && count < list->size) {
        if (count > 0) printf(" <-> ");
        print(current->data);
        current = current->next;
        count++;
    }

    if (list->type == LIST_CIRCULAR && list->size > 0) {
        printf(" -> (circular)");
    }

    printf("]\n");
}

void list_reverse(LinkedList *list) {
    if (list == NULL || list->size <= 1) {
        return;
    }

    ListNode *current = list->head;
    ListNode *temp = NULL;

    // Trocar next e prev de todos os nós
    for (size_t i = 0; i < list->size; i++) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;

        current = current->prev;  // Avançar (que agora é o prev)
    }

    // Trocar head e tail
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;

    // Para circular, ajustar conexões
    if (list->type == LIST_CIRCULAR) {
        list->head->prev = list->tail;
        list->tail->next = list->head;
    }
}

// ============================================================================
// ITERADORES
// ============================================================================

ListNode* list_begin(const LinkedList *list) {
    return (list == NULL) ? NULL : list->head;
}

ListNode* list_end(const LinkedList *list) {
    return (list == NULL) ? NULL : list->tail;
}

ListNode* list_next(const ListNode *node) {
    return (node == NULL) ? NULL : node->next;
}

ListNode* list_prev(const ListNode *node) {
    return (node == NULL) ? NULL : node->prev;
}

void* list_node_data(const ListNode *node) {
    return (node == NULL) ? NULL : node->data;
}

// TODO: Implementar operações avançadas quando necessário
LinkedList* list_clone(const LinkedList *list, CopyFn copy) {
    (void)list;
    (void)copy;
    return NULL;
}

void list_sort(LinkedList *list, CompareFn compare) {
    (void)list;
    (void)compare;
    // TODO: Implementar merge sort
}

DataStructureError list_concat(LinkedList *list1, LinkedList *list2) {
    (void)list1;
    (void)list2;
    return DS_ERROR_NOT_FOUND;
}
