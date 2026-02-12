/**
 * @file queue.h
 * @brief Implementação genérica de Fila (Queue) - estrutura FIFO
 *
 * Fila é uma estrutura de dados que segue o princípio FIFO (First In, First Out).
 * Elementos são inseridos no final (enqueue) e removidos do início (dequeue).
 *
 * Complexidade das Operações:
 * - Enqueue: O(1) amortizado
 * - Dequeue: O(1)
 * - Front (peek): O(1)
 * - IsEmpty: O(1)
 * - Size: O(1)
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009).
 *   "Introduction to Algorithms" (3rd ed.), Chapter 10.1 - Stacks and Queues
 * - Knuth, D. E. (1997). "The Art of Computer Programming, Vol 1", Section 2.2.1
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURA DA FILA (OPACA)
// ============================================================================

/**
 * @brief Estrutura opaca da fila
 *
 * A implementação interna é escondida do usuário (padrão de design opaque pointer)
 * para permitir mudanças na implementação sem quebrar código cliente.
 */
typedef struct Queue Queue;

/**
 * @brief Tipos de implementação de fila
 */
typedef enum {
    QUEUE_ARRAY,    /**< Fila baseada em array circular (melhor performance) */
    QUEUE_LINKED    /**< Fila baseada em lista encadeada (tamanho dinâmico) */
} QueueType;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova fila genérica
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param type Tipo de implementação (QUEUE_ARRAY ou QUEUE_LINKED)
 * @param initial_capacity Capacidade inicial (apenas para QUEUE_ARRAY, ignorado para QUEUE_LINKED)
 * @param destroy Função de destruição customizada (NULL se não necessário)
 * @return Queue* Ponteiro para a fila criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * // Fila de inteiros
 * Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);
 *
 * // Fila de strings
 * Queue *q = queue_create(sizeof(char*), QUEUE_LINKED, 0, destroy_string);
 * @endcode
 *
 * Complexidade: O(1) para QUEUE_LINKED, O(capacity) para QUEUE_ARRAY
 */
Queue* queue_create(size_t element_size, QueueType type, size_t initial_capacity, DestroyFn destroy);

/**
 * @brief Destrói a fila e libera toda a memória associada
 *
 * @param queue Ponteiro para a fila
 *
 * Se uma função destroy foi fornecida na criação, ela será chamada para cada elemento.
 *
 * Complexidade: O(n) onde n é o número de elementos
 */
void queue_destroy(Queue *queue);

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * @brief Insere um elemento no final da fila (enqueue)
 *
 * @param queue Ponteiro para a fila
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso)
 *
 * O dado é copiado para dentro da fila. Para QUEUE_ARRAY, se a fila estiver
 * cheia, o array é redimensionado automaticamente (crescimento por fator de 2).
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
DataStructureError queue_enqueue(Queue *queue, const void *data);

/**
 * @brief Remove e retorna o elemento do início da fila (dequeue)
 *
 * @param queue Ponteiro para a fila
 * @param output Buffer para armazenar o elemento removido (deve ter tamanho >= element_size)
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso, DS_ERROR_EMPTY se vazia)
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
DataStructureError queue_dequeue(Queue *queue, void *output);

/**
 * @brief Retorna o elemento do início da fila sem removê-lo (peek/front)
 *
 * @param queue Ponteiro para a fila
 * @param output Buffer para armazenar o elemento (deve ter tamanho >= element_size)
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso, DS_ERROR_EMPTY se vazia)
 *
 * Complexidade: O(1)
 */
DataStructureError queue_front(const Queue *queue, void *output);

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

/**
 * @brief Verifica se a fila está vazia
 *
 * @param queue Ponteiro para a fila
 * @return true se vazia, false caso contrário
 *
 * Complexidade: O(1)
 */
bool queue_is_empty(const Queue *queue);

/**
 * @brief Retorna o número de elementos na fila
 *
 * @param queue Ponteiro para a fila
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t queue_size(const Queue *queue);

/**
 * @brief Retorna a capacidade atual da fila (apenas para QUEUE_ARRAY)
 *
 * @param queue Ponteiro para a fila
 * @return size_t Capacidade (0 para QUEUE_LINKED)
 *
 * Complexidade: O(1)
 */
size_t queue_capacity(const Queue *queue);

/**
 * @brief Remove todos os elementos da fila
 *
 * @param queue Ponteiro para a fila
 *
 * Se uma função destroy foi fornecida, ela será chamada para cada elemento.
 *
 * Complexidade: O(n)
 */
void queue_clear(Queue *queue);

/**
 * @brief Imprime a fila (para debugging)
 *
 * @param queue Ponteiro para a fila
 * @param print Função de impressão para cada elemento
 *
 * Complexidade: O(n)
 */
void queue_print(const Queue *queue, PrintFn print);

// ============================================================================
// OPERAÇÕES AVANÇADAS (OPCIONAIS)
// ============================================================================

/**
 * @brief Cria uma cópia da fila
 *
 * @param queue Ponteiro para a fila original
 * @param copy Função de cópia customizada (NULL para memcpy simples)
 * @return Queue* Ponteiro para a nova fila, ou NULL em caso de erro
 *
 * Complexidade: O(n)
 */
Queue* queue_clone(const Queue *queue, CopyFn copy);

/**
 * @brief Converte a fila para array
 *
 * @param queue Ponteiro para a fila
 * @param array Ponteiro para o array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho do array
 * @return DataStructureError Código de erro
 *
 * O array retornado deve ser liberado pelo caller com free().
 * Os elementos são copiados na ordem FIFO.
 *
 * Complexidade: O(n)
 */
DataStructureError queue_to_array(const Queue *queue, void **array, size_t *size);

#endif // QUEUE_H
