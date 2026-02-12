/**
 * @file array_list.h
 * @brief Implementação genérica de ArrayList (Dynamic Array / Vector)
 *
 * ArrayList é um array dinâmico que cresce automaticamente quando necessário.
 * Oferece acesso O(1) por índice e inserção O(1) amortizada no final.
 *
 * Complexidade das Operações:
 * - Acesso por índice (get/set): O(1)
 * - Inserção no final (push_back): O(1) amortizado
 * - Inserção/remoção em posição arbitrária: O(n)
 * - Busca não ordenada: O(n)
 * - Busca ordenada (binary search): O(log n)
 *
 * Referências Acadêmicas:
 * - Goodrich, M. T., Tamassia, R., & Mount, D. M. (2011).
 *   "Data Structures and Algorithms in C++" (2nd ed.), Chapter 7 - Lists and Iterators
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 17 - Amortized Analysis (análise do crescimento dinâmico)
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 1.3
 *
 * Análise Amortizada do Crescimento:
 * - Fator de crescimento: 2x (dobra a capacidade)
 * - Custo amortizado de n inserções: O(n)
 * - Prova: série geométrica 1 + 2 + 4 + 8 + ... + n = 2n - 1 < 2n
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURA DO ARRAYLIST (OPACA)
// ============================================================================

/**
 * @brief Estrutura opaca do ArrayList
 */
typedef struct ArrayList ArrayList;

/**
 * @brief Configurações de crescimento do ArrayList
 */
typedef enum {
    GROWTH_DOUBLE,    /**< Crescimento 2x (padrão, melhor análise amortizada) */
    GROWTH_1_5,       /**< Crescimento 1.5x (menos desperdício de memória) */
    GROWTH_FIXED      /**< Crescimento fixo de N elementos */
} GrowthStrategy;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria um novo ArrayList genérico
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param initial_capacity Capacidade inicial (mínimo 1)
 * @param destroy Função de destruição customizada (NULL se não necessário)
 * @return ArrayList* Ponteiro para o ArrayList criado, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * // ArrayList de inteiros com capacidade inicial 10
 * ArrayList *arr = arraylist_create(sizeof(int), 10, NULL);
 *
 * // ArrayList de strings com capacidade inicial 5
 * ArrayList *arr = arraylist_create(sizeof(char*), 5, destroy_string);
 * @endcode
 *
 * Complexidade: O(capacity)
 */
ArrayList* arraylist_create(size_t element_size, size_t initial_capacity, DestroyFn destroy);

/**
 * @brief Cria um ArrayList com estratégia de crescimento customizada
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param initial_capacity Capacidade inicial
 * @param growth Estratégia de crescimento
 * @param destroy Função de destruição customizada
 * @return ArrayList* Ponteiro para o ArrayList criado, ou NULL em caso de erro
 *
 * Complexidade: O(capacity)
 */
ArrayList* arraylist_create_with_growth(size_t element_size, size_t initial_capacity,
                                        GrowthStrategy growth, DestroyFn destroy);

/**
 * @brief Destrói o ArrayList e libera toda a memória associada
 *
 * @param list Ponteiro para o ArrayList
 *
 * Se uma função destroy foi fornecida na criação, ela será chamada para cada elemento.
 *
 * Complexidade: O(n)
 */
void arraylist_destroy(ArrayList *list);

// ============================================================================
// OPERAÇÕES DE INSERÇÃO
// ============================================================================

/**
 * @brief Insere um elemento no final do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Se o ArrayList estiver cheio, a capacidade é aumentada automaticamente
 * de acordo com a estratégia de crescimento (padrão: 2x).
 *
 * Análise Amortizada (Cormen et al., 2009, Cap. 17):
 * Seja c_i o custo da i-ésima inserção:
 * - Se i-1 não é potência de 2: c_i = 1
 * - Se i-1 é potência de 2: c_i = i (copy todos elementos)
 * Custo total de n operações: sum(c_i) < 2n, logo amortizado = O(1)
 *
 * Complexidade: O(1) amortizado, O(n) pior caso (quando realloc necessário)
 */
DataStructureError arraylist_push_back(ArrayList *list, const void *data);

/**
 * @brief Insere um elemento no início do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Requer deslocar todos os elementos existentes uma posição à direita.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_push_front(ArrayList *list, const void *data);

/**
 * @brief Insere um elemento em uma posição específica
 *
 * @param list Ponteiro para o ArrayList
 * @param index Índice onde inserir (0 <= index <= size)
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Elementos de index em diante são deslocados uma posição à direita.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_insert(ArrayList *list, size_t index, const void *data);

// ============================================================================
// OPERAÇÕES DE REMOÇÃO
// ============================================================================

/**
 * @brief Remove o último elemento do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError arraylist_pop_back(ArrayList *list, void *output);

/**
 * @brief Remove o primeiro elemento do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Requer deslocar todos os elementos uma posição à esquerda.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_pop_front(ArrayList *list, void *output);

/**
 * @brief Remove um elemento em uma posição específica
 *
 * @param list Ponteiro para o ArrayList
 * @param index Índice do elemento a remover
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Elementos após index são deslocados uma posição à esquerda.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_remove_at(ArrayList *list, size_t index, void *output);

/**
 * @brief Remove a primeira ocorrência de um valor
 *
 * @param list Ponteiro para o ArrayList
 * @param data Ponteiro para o dado a ser removido
 * @param compare Função de comparação
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_remove(ArrayList *list, const void *data, CompareFn compare);

// ============================================================================
// OPERAÇÕES DE ACESSO E BUSCA
// ============================================================================

/**
 * @brief Retorna o elemento em uma posição específica
 *
 * @param list Ponteiro para o ArrayList
 * @param index Índice do elemento
 * @param output Buffer para armazenar o elemento
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError arraylist_get(const ArrayList *list, size_t index, void *output);

/**
 * @brief Retorna um ponteiro direto para o elemento (uso avançado)
 *
 * @param list Ponteiro para o ArrayList
 * @param index Índice do elemento
 * @return void* Ponteiro para o elemento, ou NULL se índice inválido
 *
 * AVISO: Ponteiro pode ser invalidado após operações que realocam o array.
 *
 * Complexidade: O(1)
 */
void* arraylist_get_ptr(const ArrayList *list, size_t index);

/**
 * @brief Define o valor de um elemento em uma posição específica
 *
 * @param list Ponteiro para o ArrayList
 * @param index Índice do elemento
 * @param data Ponteiro para o novo dado
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError arraylist_set(ArrayList *list, size_t index, const void *data);

/**
 * @brief Busca linear por um valor
 *
 * @param list Ponteiro para o ArrayList
 * @param data Ponteiro para o dado a ser buscado
 * @param compare Função de comparação
 * @return ssize_t Índice do elemento (-1 se não encontrado)
 *
 * Complexidade: O(n)
 */
ssize_t arraylist_find(const ArrayList *list, const void *data, CompareFn compare);

/**
 * @brief Busca binária por um valor (requer ArrayList ordenado)
 *
 * @param list Ponteiro para o ArrayList (deve estar ordenado)
 * @param data Ponteiro para o dado a ser buscado
 * @param compare Função de comparação
 * @return ssize_t Índice do elemento (-1 se não encontrado)
 *
 * Pseudocódigo (Cormen et al., 2009, p. 799):
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
ssize_t arraylist_binary_search(const ArrayList *list, const void *data, CompareFn compare);

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

/**
 * @brief Verifica se o ArrayList está vazio
 *
 * @param list Ponteiro para o ArrayList
 * @return true se vazio, false caso contrário
 *
 * Complexidade: O(1)
 */
bool arraylist_is_empty(const ArrayList *list);

/**
 * @brief Retorna o número de elementos no ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t arraylist_size(const ArrayList *list);

/**
 * @brief Retorna a capacidade atual do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @return size_t Capacidade
 *
 * Complexidade: O(1)
 */
size_t arraylist_capacity(const ArrayList *list);

/**
 * @brief Remove todos os elementos do ArrayList
 *
 * @param list Ponteiro para o ArrayList
 *
 * Não reduz a capacidade alocada.
 *
 * Complexidade: O(n)
 */
void arraylist_clear(ArrayList *list);

/**
 * @brief Reduz a capacidade para o tamanho atual (shrink-to-fit)
 *
 * @param list Ponteiro para o ArrayList
 * @return DataStructureError Código de erro
 *
 * Útil para economizar memória após muitas remoções.
 *
 * Complexidade: O(n)
 */
DataStructureError arraylist_shrink_to_fit(ArrayList *list);

/**
 * @brief Reserva capacidade mínima
 *
 * @param list Ponteiro para o ArrayList
 * @param new_capacity Nova capacidade mínima
 * @return DataStructureError Código de erro
 *
 * Se new_capacity > capacidade atual, realoca o array.
 *
 * Complexidade: O(n) se realocar, O(1) caso contrário
 */
DataStructureError arraylist_reserve(ArrayList *list, size_t new_capacity);

/**
 * @brief Imprime o ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param print Função de impressão para cada elemento
 *
 * Complexidade: O(n)
 */
void arraylist_print(const ArrayList *list, PrintFn print);

/**
 * @brief Inverte o ArrayList
 *
 * @param list Ponteiro para o ArrayList
 *
 * Complexidade: O(n)
 */
void arraylist_reverse(ArrayList *list);

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Cria uma cópia do ArrayList
 *
 * @param list Ponteiro para o ArrayList original
 * @param copy Função de cópia customizada (NULL para memcpy simples)
 * @return ArrayList* Ponteiro para o novo ArrayList, ou NULL em caso de erro
 *
 * Complexidade: O(n)
 */
ArrayList* arraylist_clone(const ArrayList *list, CopyFn copy);

/**
 * @brief Ordena o ArrayList
 *
 * @param list Ponteiro para o ArrayList
 * @param compare Função de comparação
 *
 * Usa QuickSort otimizado (stdlib qsort internamente).
 * Referência: Hoare, C. A. R. (1962). "Quicksort"
 *
 * Complexidade: O(n log n) esperado, O(n²) pior caso
 */
void arraylist_sort(ArrayList *list, CompareFn compare);

/**
 * @brief Retorna o array interno (uso avançado)
 *
 * @param list Ponteiro para o ArrayList
 * @return void* Ponteiro para o array interno
 *
 * AVISO: Ponteiro pode ser invalidado após operações que realocam.
 * Use com cautela e apenas quando necessário para interoperabilidade.
 *
 * Complexidade: O(1)
 */
void* arraylist_data(const ArrayList *list);

#endif // ARRAY_LIST_H
