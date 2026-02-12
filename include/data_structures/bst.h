/**
 * @file bst.h
 * @brief Implementação genérica de Árvore Binária de Busca (Binary Search Tree)
 *
 * BST é uma árvore binária com a propriedade de ordenação:
 * Para todo nó x: esquerda(x) ≤ x < direita(x)
 *
 * Complexidade das Operações:
 * - Inserção: O(h) onde h é a altura
 * - Busca: O(h)
 * - Remoção: O(h)
 * - Min/Max: O(h)
 * - Árvore balanceada: h = O(log n)
 * - Árvore degenerada: h = O(n) [pior caso]
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 12 - Binary Search Trees
 * - Knuth, D. E. (1998). "The Art of Computer Programming, Vol 3", Section 6.2.2
 *   "Binary Tree Searching"
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 3.2
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef BST_H
#define BST_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURAS OPACAS
// ============================================================================

/**
 * @brief Estrutura opaca da BST
 */
typedef struct BST BST;

/**
 * @brief Estrutura opaca do nó da BST
 */
typedef struct BSTNode BSTNode;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova BST vazia
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param compare Função de comparação (OBRIGATÓRIA para BST)
 * @param destroy Função de destruição (NULL se não necessário)
 * @return BST* Ponteiro para a BST criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * BST *bst = bst_create(sizeof(int), compare_int, NULL);
 * @endcode
 *
 * Complexidade: O(1)
 */
BST* bst_create(size_t element_size, CompareFn compare, DestroyFn destroy);

/**
 * @brief Destrói a BST e libera memória
 *
 * @param bst Ponteiro para a BST
 *
 * Complexidade: O(n)
 */
void bst_destroy(BST *bst);

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * @brief Insere um elemento na BST
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado a inserir
 * @return DataStructureError Código de erro
 *
 * Mantém a propriedade BST: esquerda < nó < direita
 * Se elemento já existe, comportamento depende da implementação
 * (pode atualizar, ignorar ou inserir duplicata).
 *
 * Pseudocódigo (Cormen et al., 2009, p. 294):
 * TREE-INSERT(T, z)
 *   y = NIL
 *   x = T.root
 *   while x ≠ NIL
 *       y = x
 *       if z.key < x.key
 *           x = x.left
 *       else x = x.right
 *   z.p = y
 *   if y == NIL
 *       T.root = z
 *   else if z.key < y.key
 *       y.left = z
 *   else y.right = z
 *
 * Complexidade: O(h)
 */
DataStructureError bst_insert(BST *bst, const void *data);

/**
 * @brief Busca um elemento na BST
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado a buscar
 * @param output Buffer para armazenar o elemento encontrado
 * @return DataStructureError DS_SUCCESS ou DS_ERROR_NOT_FOUND
 *
 * Pseudocódigo Iterativo (Cormen et al., 2009, p. 291):
 * ITERATIVE-TREE-SEARCH(x, k)
 *   while x ≠ NIL and k ≠ x.key
 *       if k < x.key
 *           x = x.left
 *       else x = x.right
 *   return x
 *
 * Pseudocódigo Recursivo (Cormen et al., 2009, p. 290):
 * TREE-SEARCH(x, k)
 *   if x == NIL or k == x.key
 *       return x
 *   if k < x.key
 *       return TREE-SEARCH(x.left, k)
 *   else return TREE-SEARCH(x.right, k)
 *
 * Complexidade: O(h)
 */
DataStructureError bst_search(const BST *bst, const void *data, void *output);

/**
 * @brief Busca e retorna ponteiro para o nó
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado a buscar
 * @return BSTNode* Nó encontrado, ou NULL
 *
 * Complexidade: O(h)
 */
BSTNode* bst_find_node(const BST *bst, const void *data);

/**
 * @brief Verifica se um elemento existe na BST
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado
 * @return true se existe, false caso contrário
 *
 * Complexidade: O(h)
 */
bool bst_contains(const BST *bst, const void *data);

/**
 * @brief Remove um elemento da BST
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado a remover
 * @return DataStructureError Código de erro
 *
 * Três casos (Cormen et al., 2009, p. 296):
 * 1. Nó sem filhos: simplesmente remove
 * 2. Nó com um filho: substitui por esse filho
 * 3. Nó com dois filhos: substitui pelo sucessor (ou predecessor)
 *
 * Pseudocódigo (Cormen et al., 2009, p. 298):
 * TREE-DELETE(T, z)
 *   if z.left == NIL
 *       TRANSPLANT(T, z, z.right)
 *   else if z.right == NIL
 *       TRANSPLANT(T, z, z.left)
 *   else
 *       y = TREE-MINIMUM(z.right)
 *       if y.p ≠ z
 *           TRANSPLANT(T, y, y.right)
 *           y.right = z.right
 *           y.right.p = y
 *       TRANSPLANT(T, z, y)
 *       y.left = z.left
 *       y.left.p = y
 *
 * Complexidade: O(h)
 */
DataStructureError bst_remove(BST *bst, const void *data);

// ============================================================================
// OPERAÇÕES DE MIN/MAX
// ============================================================================

/**
 * @brief Retorna o menor elemento da BST
 *
 * @param bst Ponteiro para a BST
 * @param output Buffer para armazenar o mínimo
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 291):
 * TREE-MINIMUM(x)
 *   while x.left ≠ NIL
 *       x = x.left
 *   return x
 *
 * Complexidade: O(h)
 */
DataStructureError bst_min(const BST *bst, void *output);

/**
 * @brief Retorna o maior elemento da BST
 *
 * @param bst Ponteiro para a BST
 * @param output Buffer para armazenar o máximo
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 291):
 * TREE-MAXIMUM(x)
 *   while x.right ≠ NIL
 *       x = x.right
 *   return x
 *
 * Complexidade: O(h)
 */
DataStructureError bst_max(const BST *bst, void *output);

// ============================================================================
// OPERAÇÕES DE SUCESSOR/PREDECESSOR
// ============================================================================

/**
 * @brief Retorna o sucessor de um elemento (próximo maior)
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado
 * @param output Buffer para armazenar o sucessor
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo (Cormen et al., 2009, p. 292):
 * TREE-SUCCESSOR(x)
 *   if x.right ≠ NIL
 *       return TREE-MINIMUM(x.right)
 *   y = x.p
 *   while y ≠ NIL and x == y.right
 *       x = y
 *       y = y.p
 *   return y
 *
 * Complexidade: O(h)
 */
DataStructureError bst_successor(const BST *bst, const void *data, void *output);

/**
 * @brief Retorna o predecessor de um elemento (próximo menor)
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado
 * @param output Buffer para armazenar o predecessor
 * @return DataStructureError Código de erro
 *
 * Pseudocódigo: similar a TREE-SUCCESSOR, mas espelhado
 *
 * Complexidade: O(h)
 */
DataStructureError bst_predecessor(const BST *bst, const void *data, void *output);

// ============================================================================
// RANGE QUERIES
// ============================================================================

/**
 * @brief Busca todos os elementos em um intervalo [min, max]
 *
 * @param bst Ponteiro para a BST
 * @param min Valor mínimo do intervalo
 * @param max Valor máximo do intervalo
 * @param results Array de resultados (será alocado)
 * @param count Ponteiro para armazenar o número de resultados
 * @return DataStructureError Código de erro
 *
 * Retorna elementos ordenados no intervalo.
 * Array deve ser liberado com free().
 *
 * Complexidade: O(h + k) onde k é o número de elementos no intervalo
 */
DataStructureError bst_range_search(const BST *bst, const void *min, const void *max,
                                    void **results, size_t *count);

/**
 * @brief Conta elementos em um intervalo [min, max]
 *
 * @param bst Ponteiro para a BST
 * @param min Valor mínimo
 * @param max Valor máximo
 * @return size_t Número de elementos no intervalo
 *
 * Complexidade: O(h + k) onde k é o número de elementos no intervalo
 */
size_t bst_range_count(const BST *bst, const void *min, const void *max);

// ============================================================================
// TRAVESSIAS
// ============================================================================

/**
 * @brief Tipo de função callback para travessias
 */
typedef void (*BSTTraversalFn)(void *data, void *user_data);

/**
 * @brief Travessia em ordem (inorder) - retorna elementos ORDENADOS
 *
 * @param bst Ponteiro para a BST
 * @param callback Função callback
 * @param user_data Dados do usuário
 *
 * Para BST: visita elementos em ordem crescente
 *
 * Complexidade: O(n)
 */
void bst_inorder(const BST *bst, BSTTraversalFn callback, void *user_data);

/**
 * @brief Travessia pré-ordem (preorder)
 *
 * @param bst Ponteiro para a BST
 * @param callback Função callback
 * @param user_data Dados do usuário
 *
 * Complexidade: O(n)
 */
void bst_preorder(const BST *bst, BSTTraversalFn callback, void *user_data);

/**
 * @brief Travessia pós-ordem (postorder)
 *
 * @param bst Ponteiro para a BST
 * @param callback Função callback
 * @param user_data Dados do usuário
 *
 * Complexidade: O(n)
 */
void bst_postorder(const BST *bst, BSTTraversalFn callback, void *user_data);

/**
 * @brief Travessia em nível (levelorder)
 *
 * @param bst Ponteiro para a BST
 * @param callback Função callback
 * @param user_data Dados do usuário
 *
 * Complexidade: O(n)
 */
void bst_levelorder(const BST *bst, BSTTraversalFn callback, void *user_data);

// ============================================================================
// CONSULTAS E PROPRIEDADES
// ============================================================================

/**
 * @brief Verifica se a BST está vazia
 *
 * @param bst Ponteiro para a BST
 * @return true se vazia
 *
 * Complexidade: O(1)
 */
bool bst_is_empty(const BST *bst);

/**
 * @brief Retorna o número de elementos
 *
 * @param bst Ponteiro para a BST
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t bst_size(const BST *bst);

/**
 * @brief Retorna a altura da BST
 *
 * @param bst Ponteiro para a BST
 * @return int Altura
 *
 * Complexidade: O(n)
 */
int bst_height(const BST *bst);

/**
 * @brief Verifica se a árvore é uma BST válida
 *
 * @param bst Ponteiro para a BST
 * @return true se satisfaz a propriedade BST
 *
 * Útil para debugging e testes.
 * Verifica se para todo nó: esquerda < nó < direita
 *
 * Complexidade: O(n)
 */
bool bst_is_valid(const BST *bst);

/**
 * @brief Verifica se a BST está balanceada
 *
 * @param bst Ponteiro para a BST
 * @return true se balanceada (|altura_esq - altura_dir| ≤ 1 para todos os nós)
 *
 * Definição: Para cada nó, a diferença de altura entre subárvores
 * esquerda e direita é no máximo 1.
 *
 * Complexidade: O(n)
 */
bool bst_is_balanced(const BST *bst);

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Limpa a BST (remove todos os elementos)
 *
 * @param bst Ponteiro para a BST
 *
 * Complexidade: O(n)
 */
void bst_clear(BST *bst);

/**
 * @brief Cria uma cópia da BST
 *
 * @param bst Ponteiro para a BST original
 * @param copy_fn Função de cópia (NULL para memcpy)
 * @return BST* Nova BST
 *
 * Complexidade: O(n)
 */
BST* bst_clone(const BST *bst, CopyFn copy_fn);

/**
 * @brief Converte BST para array ordenado
 *
 * @param bst Ponteiro para a BST
 * @param array Ponteiro para array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho
 * @return DataStructureError Código de erro
 *
 * Usa inorder traversal, resultando em array ordenado.
 * Array deve ser liberado com free().
 *
 * Complexidade: O(n)
 */
DataStructureError bst_to_array(const BST *bst, void **array, size_t *size);

/**
 * @brief Cria BST a partir de array ordenado (balanceada)
 *
 * @param element_size Tamanho dos elementos
 * @param array Array ordenado de elementos
 * @param size Tamanho do array
 * @param compare Função de comparação
 * @param destroy Função de destruição
 * @return BST* BST balanceada criada
 *
 * Cria BST perfeitamente balanceada (altura O(log n)) a partir de array ordenado.
 * Útil para otimização de BST existente.
 *
 * Complexidade: O(n)
 */
BST* bst_from_sorted_array(size_t element_size, const void *array, size_t size,
                           CompareFn compare, DestroyFn destroy);

/**
 * @brief Balanceia a BST (reconstrução)
 *
 * @param bst Ponteiro para a BST
 * @return DataStructureError Código de erro
 *
 * Reconstrói a BST de forma balanceada.
 * Algoritmo: converte para array ordenado + reconstrói de forma balanceada.
 *
 * Complexidade: O(n)
 */
DataStructureError bst_balance(BST *bst);

/**
 * @brief Imprime a BST visualmente
 *
 * @param bst Ponteiro para a BST
 * @param print Função de impressão
 *
 * Complexidade: O(n)
 */
void bst_print(const BST *bst, PrintFn print);

/**
 * @brief Retorna o k-ésimo menor elemento (Order Statistics)
 *
 * @param bst Ponteiro para a BST
 * @param k Posição (1 = menor, size = maior)
 * @param output Buffer para armazenar o elemento
 * @return DataStructureError Código de erro
 *
 * Referência: Cormen et al., 2009, Chapter 14 (Order-Statistic Trees)
 *
 * Complexidade: O(h + k) para BST simples, O(h) se aumentada com size info
 */
DataStructureError bst_select(const BST *bst, size_t k, void *output);

/**
 * @brief Retorna o rank de um elemento (quantos são menores)
 *
 * @param bst Ponteiro para a BST
 * @param data Ponteiro para o dado
 * @return size_t Rank (posição em travessia inorder, começando de 0)
 *
 * Complexidade: O(h)
 */
size_t bst_rank(const BST *bst, const void *data);

#endif // BST_H
