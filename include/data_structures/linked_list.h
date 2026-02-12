/**
 * @file linked_list.h
 * @brief Implementação genérica de Lista Encadeada (Linked List)
 *
 * Lista encadeada é uma estrutura de dados linear onde cada elemento (nó) contém
 * dados e um ou mais ponteiros para outros nós. Suporta inserção e remoção
 * eficientes em qualquer posição quando se tem um ponteiro para o nó.
 *
 * Complexidade das Operações:
 * - Inserção/remoção com ponteiro: O(1)
 * - Inserção/remoção por índice: O(n)
 * - Busca: O(n)
 * - Acesso por índice: O(n)
 *
 * Referências Acadêmicas:
 * - Knuth, D. E. (1997). "The Art of Computer Programming, Vol 1", Section 2.2
 *   "Linear Lists" - Análise detalhada de listas encadeadas
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 10.2 - Linked Lists
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 1.3
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURAS OPACAS
// ============================================================================

/**
 * @brief Estrutura opaca da lista encadeada
 */
typedef struct LinkedList LinkedList;

/**
 * @brief Estrutura opaca do nó da lista
 *
 * Usado para iteração e acesso direto aos elementos.
 */
typedef struct ListNode ListNode;

/**
 * @brief Tipos de lista encadeada
 */
typedef enum {
    LIST_SINGLY,      /**< Lista simplesmente encadeada */
    LIST_DOUBLY,      /**< Lista duplamente encadeada */
    LIST_CIRCULAR     /**< Lista circular (duplamente encadeada) */
} ListType;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova lista encadeada genérica
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param type Tipo de lista (LIST_SINGLY, LIST_DOUBLY, LIST_CIRCULAR)
 * @param destroy Função de destruição customizada (NULL se não necessário)
 * @return LinkedList* Ponteiro para a lista criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * // Lista duplamente encadeada de inteiros
 * LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);
 *
 * // Lista simplesmente encadeada de strings
 * LinkedList *list = list_create(sizeof(char*), LIST_SINGLY, destroy_string);
 * @endcode
 *
 * Complexidade: O(1)
 */
LinkedList* list_create(size_t element_size, ListType type, DestroyFn destroy);

/**
 * @brief Destrói a lista e libera toda a memória associada
 *
 * @param list Ponteiro para a lista
 *
 * Se uma função destroy foi fornecida na criação, ela será chamada para cada elemento.
 *
 * Complexidade: O(n)
 */
void list_destroy(LinkedList *list);

// ============================================================================
// OPERAÇÕES DE INSERÇÃO
// ============================================================================

/**
 * @brief Insere um elemento no início da lista
 *
 * @param list Ponteiro para a lista
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 238):
 * LIST-INSERT(L, x)
 *   x.next = L.head
 *   if L.head ≠ NIL
 *       L.head.prev = x
 *   L.head = x
 *   x.prev = NIL
 *
 * Complexidade: O(1)
 */
DataStructureError list_push_front(LinkedList *list, const void *data);

/**
 * @brief Insere um elemento no final da lista
 *
 * @param list Ponteiro para a lista
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1) se mantiver ponteiro para tail, O(n) caso contrário
 * (nossa implementação mantém ponteiro para tail)
 */
DataStructureError list_push_back(LinkedList *list, const void *data);

/**
 * @brief Insere um elemento em uma posição específica
 *
 * @param list Ponteiro para a lista
 * @param index Índice onde inserir (0 = início, size = final)
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(n)
 */
DataStructureError list_insert_at(LinkedList *list, size_t index, const void *data);

/**
 * @brief Insere um elemento após um nó específico
 *
 * @param list Ponteiro para a lista
 * @param node Nó após o qual inserir
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError list_insert_after(LinkedList *list, ListNode *node, const void *data);

/**
 * @brief Insere um elemento antes de um nó específico (apenas LIST_DOUBLY e LIST_CIRCULAR)
 *
 * @param list Ponteiro para a lista
 * @param node Nó antes do qual inserir
 * @param data Ponteiro para o dado a ser inserido
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1) para LIST_DOUBLY/LIST_CIRCULAR, O(n) para LIST_SINGLY
 */
DataStructureError list_insert_before(LinkedList *list, ListNode *node, const void *data);

// ============================================================================
// OPERAÇÕES DE REMOÇÃO
// ============================================================================

/**
 * @brief Remove o primeiro elemento da lista
 *
 * @param list Ponteiro para a lista
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 238):
 * LIST-DELETE(L, x)
 *   if x.prev ≠ NIL
 *       x.prev.next = x.next
 *   else L.head = x.next
 *   if x.next ≠ NIL
 *       x.next.prev = x.prev
 *
 * Complexidade: O(1)
 */
DataStructureError list_pop_front(LinkedList *list, void *output);

/**
 * @brief Remove o último elemento da lista
 *
 * @param list Ponteiro para a lista
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1) para LIST_DOUBLY/LIST_CIRCULAR, O(n) para LIST_SINGLY
 */
DataStructureError list_pop_back(LinkedList *list, void *output);

/**
 * @brief Remove um elemento em uma posição específica
 *
 * @param list Ponteiro para a lista
 * @param index Índice do elemento a remover
 * @param output Buffer para armazenar o elemento removido (NULL para descartar)
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(n)
 */
DataStructureError list_remove_at(LinkedList *list, size_t index, void *output);

/**
 * @brief Remove um nó específico
 *
 * @param list Ponteiro para a lista
 * @param node Nó a ser removido
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1) para LIST_DOUBLY/LIST_CIRCULAR, O(n) para LIST_SINGLY
 */
DataStructureError list_remove_node(LinkedList *list, ListNode *node);

/**
 * @brief Remove a primeira ocorrência de um valor
 *
 * @param list Ponteiro para a lista
 * @param data Ponteiro para o dado a ser removido
 * @param compare Função de comparação
 * @return DataStructureError Código de erro (DS_ERROR_NOT_FOUND se não encontrado)
 *
 * Complexidade: O(n)
 */
DataStructureError list_remove(LinkedList *list, const void *data, CompareFn compare);

// ============================================================================
// OPERAÇÕES DE ACESSO E BUSCA
// ============================================================================

/**
 * @brief Retorna o elemento em uma posição específica
 *
 * @param list Ponteiro para a lista
 * @param index Índice do elemento
 * @param output Buffer para armazenar o elemento
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(n)
 */
DataStructureError list_get(const LinkedList *list, size_t index, void *output);

/**
 * @brief Define o valor de um elemento em uma posição específica
 *
 * @param list Ponteiro para a lista
 * @param index Índice do elemento
 * @param data Ponteiro para o novo dado
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(n)
 */
DataStructureError list_set(LinkedList *list, size_t index, const void *data);

/**
 * @brief Busca a primeira ocorrência de um valor
 *
 * @param list Ponteiro para a lista
 * @param data Ponteiro para o dado a ser buscado
 * @param compare Função de comparação
 * @return ListNode* Ponteiro para o nó encontrado, ou NULL se não encontrado
 *
 * Complexidade: O(n)
 */
ListNode* list_find(const LinkedList *list, const void *data, CompareFn compare);

/**
 * @brief Retorna o índice da primeira ocorrência de um valor
 *
 * @param list Ponteiro para a lista
 * @param data Ponteiro para o dado a ser buscado
 * @param compare Função de comparação
 * @return ssize_t Índice do elemento (-1 se não encontrado)
 *
 * Complexidade: O(n)
 */
ssize_t list_index_of(const LinkedList *list, const void *data, CompareFn compare);

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

/**
 * @brief Verifica se a lista está vazia
 *
 * @param list Ponteiro para a lista
 * @return true se vazia, false caso contrário
 *
 * Complexidade: O(1)
 */
bool list_is_empty(const LinkedList *list);

/**
 * @brief Retorna o número de elementos na lista
 *
 * @param list Ponteiro para a lista
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t list_size(const LinkedList *list);

/**
 * @brief Remove todos os elementos da lista
 *
 * @param list Ponteiro para a lista
 *
 * Complexidade: O(n)
 */
void list_clear(LinkedList *list);

/**
 * @brief Imprime a lista
 *
 * @param list Ponteiro para a lista
 * @param print Função de impressão para cada elemento
 *
 * Complexidade: O(n)
 */
void list_print(const LinkedList *list, PrintFn print);

/**
 * @brief Inverte a lista
 *
 * @param list Ponteiro para a lista
 *
 * Complexidade: O(n)
 */
void list_reverse(LinkedList *list);

// ============================================================================
// OPERAÇÕES DE ITERAÇÃO
// ============================================================================

/**
 * @brief Retorna o primeiro nó da lista
 *
 * @param list Ponteiro para a lista
 * @return ListNode* Ponteiro para o primeiro nó, ou NULL se vazia
 *
 * Complexidade: O(1)
 */
ListNode* list_begin(const LinkedList *list);

/**
 * @brief Retorna o último nó da lista
 *
 * @param list Ponteiro para a lista
 * @return ListNode* Ponteiro para o último nó, ou NULL se vazia
 *
 * Complexidade: O(1)
 */
ListNode* list_end(const LinkedList *list);

/**
 * @brief Retorna o próximo nó
 *
 * @param node Nó atual
 * @return ListNode* Próximo nó, ou NULL se final
 *
 * Complexidade: O(1)
 */
ListNode* list_next(const ListNode *node);

/**
 * @brief Retorna o nó anterior (apenas LIST_DOUBLY e LIST_CIRCULAR)
 *
 * @param node Nó atual
 * @return ListNode* Nó anterior, ou NULL se início
 *
 * Complexidade: O(1)
 */
ListNode* list_prev(const ListNode *node);

/**
 * @brief Retorna os dados de um nó
 *
 * @param node Nó
 * @return void* Ponteiro para os dados do nó
 *
 * Complexidade: O(1)
 */
void* list_node_data(const ListNode *node);

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Cria uma cópia da lista
 *
 * @param list Ponteiro para a lista original
 * @param copy Função de cópia customizada (NULL para memcpy simples)
 * @return LinkedList* Ponteiro para a nova lista, ou NULL em caso de erro
 *
 * Complexidade: O(n)
 */
LinkedList* list_clone(const LinkedList *list, CopyFn copy);

/**
 * @brief Ordena a lista
 *
 * @param list Ponteiro para a lista
 * @param compare Função de comparação
 *
 * Usa merge sort para manter estabilidade e O(n log n) no pior caso.
 * Referência: Knuth, Vol 3, Section 5.2.4 - Sorting by Merging
 *
 * Complexidade: O(n log n)
 */
void list_sort(LinkedList *list, CompareFn compare);

/**
 * @brief Concatena duas listas
 *
 * @param list1 Primeira lista (será modificada)
 * @param list2 Segunda lista (será esvaziada)
 * @return DataStructureError Código de erro
 *
 * Complexidade: O(1)
 */
DataStructureError list_concat(LinkedList *list1, LinkedList *list2);

#endif // LINKED_LIST_H
