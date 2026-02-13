/**
 * @file queue.c
 * @brief Implementação de Queue (Fila FIFO) genérica
 *
 * Implementa duas variantes:
 * - QUEUE_ARRAY: Circular buffer (array dinâmico)
 * - QUEUE_LINKED: Lista encadeada
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 10.1 - Stacks and Queues
 * - Knuth TAOCP Vol 1, Section 2.2.1
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA DA QUEUE
// ============================================================================

/**
 * @brief Nó da fila encadeada (para QUEUE_LINKED)
 */
typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
} QueueNode;

/**
 * @brief Estrutura interna da fila
 */
struct Queue {
    QueueType type;           // Tipo de implementação
    size_t element_size;      // Tamanho de cada elemento
    size_t size;              // Número de elementos
    DestroyFn destroy;        // Função de destruição

    // Para QUEUE_ARRAY (circular buffer)
    void *array;              // Array de elementos
    size_t capacity;          // Capacidade do array
    size_t head;              // Índice do primeiro elemento
    size_t tail;              // Índice da próxima posição livre

    // Para QUEUE_LINKED
    QueueNode *front;         // Primeiro nó (para dequeue)
    QueueNode *rear;          // Último nó (para enqueue)
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

/**
 * @brief Redimensiona o array da fila (circular buffer)
 *
 * Dobra a capacidade e reorganiza os elementos.
 *
 * Complexidade: O(n)
 */
static DataStructureError queue_resize_array(Queue *queue) {
    size_t new_capacity = queue->capacity * 2;
    if (new_capacity < 4) new_capacity = 4;  // Capacidade mínima

    void *new_array = malloc(new_capacity * queue->element_size);
    if (new_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    // Copiar elementos do array antigo para o novo
    // Elementos podem estar "quebrados" no circular buffer
    size_t count = 0;
    for (size_t i = 0; i < queue->size; i++) {
        size_t idx = (queue->head + i) % queue->capacity;
        void *src = (char*)queue->array + (idx * queue->element_size);
        void *dest = (char*)new_array + (count * queue->element_size);
        memcpy(dest, src, queue->element_size);
        count++;
    }

    free(queue->array);
    queue->array = new_array;
    queue->capacity = new_capacity;
    queue->head = 0;
    queue->tail = queue->size;

    return DS_SUCCESS;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova fila
 *
 * Pseudocódigo (Cormen et al., 2009, p. 235):
 * Inicializa Q.head = Q.tail = 1
 *
 * Complexidade: O(1) para LINKED, O(capacity) para ARRAY
 */
Queue* queue_create(size_t element_size, QueueType type, size_t initial_capacity, DestroyFn destroy) {
    if (element_size == 0) {
        return NULL;
    }

    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }

    queue->element_size = element_size;
    queue->type = type;
    queue->size = 0;
    queue->destroy = destroy;

    if (type == QUEUE_ARRAY) {
        // Circular buffer
        if (initial_capacity == 0) initial_capacity = 16;  // Padrão

        queue->array = malloc(initial_capacity * element_size);
        if (queue->array == NULL) {
            free(queue);
            return NULL;
        }

        queue->capacity = initial_capacity;
        queue->head = 0;
        queue->tail = 0;
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        // Linked
        queue->array = NULL;
        queue->capacity = 0;
        queue->head = 0;
        queue->tail = 0;
        queue->front = NULL;
        queue->rear = NULL;
    }

    return queue;
}

/**
 * @brief Destrói a fila
 *
 * Complexidade: O(n)
 */
void queue_destroy(Queue *queue) {
    if (queue == NULL) {
        return;
    }

    queue_clear(queue);

    if (queue->type == QUEUE_ARRAY && queue->array != NULL) {
        free(queue->array);
    }

    free(queue);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS - ARRAY
// ============================================================================

/**
 * @brief Enqueue para QUEUE_ARRAY (circular buffer)
 *
 * Pseudocódigo (Cormen et al., 2009, p. 235):
 * ENQUEUE(Q, x)
 *   Q[Q.tail] = x
 *   if Q.tail == Q.length
 *       Q.tail = 1
 *   else Q.tail = Q.tail + 1
 *
 * Complexidade: O(1) amortizado
 */
static DataStructureError queue_enqueue_array(Queue *queue, const void *data) {
    // Verificar se precisa redimensionar
    if (queue->size >= queue->capacity) {
        DataStructureError err = queue_resize_array(queue);
        if (err != DS_SUCCESS) {
            return err;
        }
    }

    // Inserir na posição tail
    void *dest = (char*)queue->array + (queue->tail * queue->element_size);
    memcpy(dest, data, queue->element_size);

    // Atualizar tail (circular)
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;

    return DS_SUCCESS;
}

/**
 * @brief Dequeue para QUEUE_ARRAY
 *
 * Pseudocódigo (Cormen et al., 2009, p. 235):
 * DEQUEUE(Q)
 *   x = Q[Q.head]
 *   if Q.head == Q.length
 *       Q.head = 1
 *   else Q.head = Q.head + 1
 *   return x
 *
 * Complexidade: O(1)
 */
static DataStructureError queue_dequeue_array(Queue *queue, void *output) {
    if (queue->size == 0) {
        return DS_ERROR_EMPTY;
    }

    // Copiar elemento na posição head
    void *src = (char*)queue->array + (queue->head * queue->element_size);
    if (output != NULL) {
        memcpy(output, src, queue->element_size);
    } else {
        // Chamar destrutor se necessário
        if (queue->destroy != NULL) {
            queue->destroy(src);
        }
    }

    // Atualizar head (circular)
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;

    return DS_SUCCESS;
}

/**
 * @brief Front para QUEUE_ARRAY
 *
 * Complexidade: O(1)
 */
static DataStructureError queue_front_array(const Queue *queue, void *output) {
    if (queue->size == 0) {
        return DS_ERROR_EMPTY;
    }

    void *src = (char*)queue->array + (queue->head * queue->element_size);
    memcpy(output, src, queue->element_size);

    return DS_SUCCESS;
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS - LINKED
// ============================================================================

/**
 * @brief Enqueue para QUEUE_LINKED
 *
 * Complexidade: O(1)
 */
static DataStructureError queue_enqueue_linked(Queue *queue, const void *data) {
    // Criar novo nó
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if (node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    node->data = malloc(queue->element_size);
    if (node->data == NULL) {
        free(node);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    memcpy(node->data, data, queue->element_size);
    node->next = NULL;

    // Adicionar no final
    if (queue->rear == NULL) {
        // Fila vazia
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }

    queue->size++;
    return DS_SUCCESS;
}

/**
 * @brief Dequeue para QUEUE_LINKED
 *
 * Complexidade: O(1)
 */
static DataStructureError queue_dequeue_linked(Queue *queue, void *output) {
    if (queue->size == 0 || queue->front == NULL) {
        return DS_ERROR_EMPTY;
    }

    QueueNode *node = queue->front;

    // Copiar dados
    if (output != NULL) {
        memcpy(output, node->data, queue->element_size);
    } else {
        // Chamar destrutor se necessário
        if (queue->destroy != NULL) {
            queue->destroy(node->data);
        }
    }

    // Remover nó
    queue->front = node->next;
    if (queue->front == NULL) {
        queue->rear = NULL;  // Fila ficou vazia
    }

    free(node->data);
    free(node);
    queue->size--;

    return DS_SUCCESS;
}

/**
 * @brief Front para QUEUE_LINKED
 *
 * Complexidade: O(1)
 */
static DataStructureError queue_front_linked(const Queue *queue, void *output) {
    if (queue->size == 0 || queue->front == NULL) {
        return DS_ERROR_EMPTY;
    }

    memcpy(output, queue->front->data, queue->element_size);
    return DS_SUCCESS;
}

// ============================================================================
// INTERFACE PÚBLICA (dispatch para array ou linked)
// ============================================================================

DataStructureError queue_enqueue(Queue *queue, const void *data) {
    if (queue == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (queue->type == QUEUE_ARRAY) {
        return queue_enqueue_array(queue, data);
    } else {
        return queue_enqueue_linked(queue, data);
    }
}

DataStructureError queue_dequeue(Queue *queue, void *output) {
    if (queue == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (queue->type == QUEUE_ARRAY) {
        return queue_dequeue_array(queue, output);
    } else {
        return queue_dequeue_linked(queue, output);
    }
}

DataStructureError queue_front(const Queue *queue, void *output) {
    if (queue == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (queue->type == QUEUE_ARRAY) {
        return queue_front_array(queue, output);
    } else {
        return queue_front_linked(queue, output);
    }
}

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

bool queue_is_empty(const Queue *queue) {
    return (queue == NULL || queue->size == 0);
}

size_t queue_size(const Queue *queue) {
    return (queue == NULL) ? 0 : queue->size;
}

size_t queue_capacity(const Queue *queue) {
    if (queue == NULL || queue->type != QUEUE_ARRAY) {
        return 0;
    }
    return queue->capacity;
}

void queue_clear(Queue *queue) {
    if (queue == NULL) {
        return;
    }

    if (queue->type == QUEUE_ARRAY) {
        // Chamar destrutor para cada elemento se necessário
        if (queue->destroy != NULL) {
            for (size_t i = 0; i < queue->size; i++) {
                size_t idx = (queue->head + i) % queue->capacity;
                void *elem = (char*)queue->array + (idx * queue->element_size);
                queue->destroy(elem);
            }
        }
        queue->head = 0;
        queue->tail = 0;
        queue->size = 0;
    } else {
        // QUEUE_LINKED
        while (queue->front != NULL) {
            queue_dequeue_linked(queue, NULL);
        }
    }
}

void queue_print(const Queue *queue, PrintFn print) {
    if (queue == NULL || print == NULL) {
        return;
    }

    printf("Queue(size=%zu, capacity=%zu) [", queue->size, queue_capacity(queue));

    if (queue->type == QUEUE_ARRAY) {
        for (size_t i = 0; i < queue->size; i++) {
            if (i > 0) printf(", ");
            size_t idx = (queue->head + i) % queue->capacity;
            void *elem = (char*)queue->array + (idx * queue->element_size);
            print(elem);
        }
    } else {
        // QUEUE_LINKED
        QueueNode *node = queue->front;
        bool first = true;
        while (node != NULL) {
            if (!first) printf(", ");
            print(node->data);
            first = false;
            node = node->next;
        }
    }

    printf("]\n");
}

// TODO: Implementar queue_clone e queue_to_array quando necessário
Queue* queue_clone(const Queue *queue, CopyFn copy) {
    (void)queue;
    (void)copy;
    // TODO: Implementar
    return NULL;
}

DataStructureError queue_to_array(const Queue *queue, void **array, size_t *size) {
    (void)queue;
    (void)array;
    (void)size;
    // TODO: Implementar
    return DS_ERROR_NOT_FOUND;
}
