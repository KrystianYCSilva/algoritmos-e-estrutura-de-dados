/**
 * @file stack.h
 * @brief Implementação genérica de Pilha (Stack) - estrutura LIFO
 *
 * Pilha é uma estrutura de dados que segue o princípio LIFO (Last In, First Out).
 * Elementos são inseridos e removidos do topo (push e pop).
 *
 * Complexidade das Operações:
 * - Push: O(1) amortizado
 * - Pop: O(1)
 * - Top (peek): O(1)
 * - IsEmpty: O(1)
 * - Size: O(1)
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009).
 *   "Introduction to Algorithms" (3rd ed.), Chapter 10.1 - Stacks and Queues
 * - Knuth, D. E. (1997). "The Art of Computer Programming, Vol 1", Section 2.2.1
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 1.3
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef STACK_H
#define STACK_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURA DA PILHA (OPACA)
// ============================================================================

/**
 * @brief Estrutura opaca da pilha
 *
 * A implementação interna é escondida do usuário (padrão de design opaque pointer)
 * para permitir mudanças na implementação sem quebrar código cliente.
 */
typedef struct Stack Stack;

/**
 * @brief Tipos de implementação de pilha
 */
typedef enum {
    STACK_ARRAY,    /**< Pilha baseada em array dinâmico (melhor performance) */
    STACK_LINKED    /**< Pilha baseada em lista encadeada (tamanho ilimitado) */
} StackType;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova pilha genérica
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param type Tipo de implementação (STACK_ARRAY ou STACK_LINKED)
 * @param initial_capacity Capacidade inicial (apenas para STACK_ARRAY, ignorado para STACK_LINKED)
 * @param destroy Função de destruição customizada (NULL se não necessário)
 * @return Stack* Ponteiro para a pilha criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * // Pilha de inteiros
 * Stack *s = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);
 *
 * // Pilha de strings
 * Stack *s = stack_create(sizeof(char*), STACK_LINKED, 0, destroy_string);
 * @endcode
 *
 * Complexidade: O(1) para STACK_LINKED, O(capacity) para STACK_ARRAY
 */
Stack* stack_create(size_t element_size, StackType type, size_t initial_capacity, DestroyFn destroy);

/**
 * @brief Destrói a pilha e libera toda a memória associada
 *
 * @param stack Ponteiro para a pilha
 *
 * Se uma função destroy foi fornecida na criação, ela será chamada para cada elemento.
 *
 * Complexidade: O(n) onde n é o número de elementos
 */
void stack_destroy(Stack *stack);

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * @brief Insere um elemento no topo da pilha (push)
 *
 * @param stack Ponteiro para a pilha
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso)
 *
 * O dado é copiado para dentro da pilha. Para STACK_ARRAY, se a pilha estiver
 * cheia, o array é redimensionado automaticamente (crescimento por fator de 2).
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * PUSH(S, x)
 *   S.top = S.top + 1
 *   S[S.top] = x
 *
 * Complexidade: O(1) amortizado
 */
DataStructureError stack_push(Stack *stack, const void *data);

/**
 * @brief Remove e retorna o elemento do topo da pilha (pop)
 *
 * @param stack Ponteiro para a pilha
 * @param output Buffer para armazenar o elemento removido (deve ter tamanho >= element_size)
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso, DS_ERROR_EMPTY se vazia)
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * POP(S)
 *   if STACK-EMPTY(S)
 *       error "underflow"
 *   else S.top = S.top - 1
 *       return S[S.top + 1]
 *
 * Complexidade: O(1)
 */
DataStructureError stack_pop(Stack *stack, void *output);

/**
 * @brief Retorna o elemento do topo da pilha sem removê-lo (peek/top)
 *
 * @param stack Ponteiro para a pilha
 * @param output Buffer para armazenar o elemento (deve ter tamanho >= element_size)
 * @return DataStructureError Código de erro (DS_SUCCESS em caso de sucesso, DS_ERROR_EMPTY se vazia)
 *
 * Complexidade: O(1)
 */
DataStructureError stack_top(const Stack *stack, void *output);

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

/**
 * @brief Verifica se a pilha está vazia
 *
 * @param stack Ponteiro para a pilha
 * @return true se vazia, false caso contrário
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * STACK-EMPTY(S)
 *   if S.top == 0
 *       return TRUE
 *   else return FALSE
 *
 * Complexidade: O(1)
 */
bool stack_is_empty(const Stack *stack);

/**
 * @brief Retorna o número de elementos na pilha
 *
 * @param stack Ponteiro para a pilha
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t stack_size(const Stack *stack);

/**
 * @brief Retorna a capacidade atual da pilha (apenas para STACK_ARRAY)
 *
 * @param stack Ponteiro para a pilha
 * @return size_t Capacidade (0 para STACK_LINKED)
 *
 * Complexidade: O(1)
 */
size_t stack_capacity(const Stack *stack);

/**
 * @brief Remove todos os elementos da pilha
 *
 * @param stack Ponteiro para a pilha
 *
 * Se uma função destroy foi fornecida, ela será chamada para cada elemento.
 *
 * Complexidade: O(n)
 */
void stack_clear(Stack *stack);

/**
 * @brief Imprime a pilha (para debugging)
 *
 * @param stack Ponteiro para a pilha
 * @param print Função de impressão para cada elemento
 *
 * Imprime do topo para a base.
 *
 * Complexidade: O(n)
 */
void stack_print(const Stack *stack, PrintFn print);

// ============================================================================
// OPERAÇÕES AVANÇADAS (OPCIONAIS)
// ============================================================================

/**
 * @brief Cria uma cópia da pilha
 *
 * @param stack Ponteiro para a pilha original
 * @param copy Função de cópia customizada (NULL para memcpy simples)
 * @return Stack* Ponteiro para a nova pilha, ou NULL em caso de erro
 *
 * Complexidade: O(n)
 */
Stack* stack_clone(const Stack *stack, CopyFn copy);

/**
 * @brief Converte a pilha para array
 *
 * @param stack Ponteiro para a pilha
 * @param array Ponteiro para o array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho do array
 * @return DataStructureError Código de erro
 *
 * O array retornado deve ser liberado pelo caller com free().
 * Os elementos são copiados do topo para a base.
 *
 * Complexidade: O(n)
 */
DataStructureError stack_to_array(const Stack *stack, void **array, size_t *size);

/**
 * @brief Inverte a ordem dos elementos na pilha
 *
 * @param stack Ponteiro para a pilha
 *
 * Complexidade: O(n)
 */
void stack_reverse(Stack *stack);

#endif // STACK_H
