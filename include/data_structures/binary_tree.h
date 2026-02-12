/**
 * @file binary_tree.h
 * @brief Implementação genérica de Árvore Binária (Binary Tree)
 *
 * Árvore binária é uma estrutura hierárquica onde cada nó tem no máximo dois filhos
 * (esquerdo e direito). Serve como base para BST, AVL, Red-Black, etc.
 *
 * Complexidade das Operações:
 * - Inserção: O(h) onde h é a altura
 * - Busca: O(h)
 * - Remoção: O(h)
 * - Travessia: O(n)
 * - Altura não balanceada: O(n) pior caso
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 12 - Binary Search Trees (generaliza para binary trees)
 * - Knuth, D. E. (1997). "The Art of Computer Programming, Vol 1", Section 2.3
 *   "Trees" - Análise matemática de árvores binárias
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 3.2
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURAS OPACAS
// ============================================================================

/**
 * @brief Estrutura opaca da árvore binária
 */
typedef struct BinaryTree BinaryTree;

/**
 * @brief Estrutura opaca do nó da árvore
 */
typedef struct TreeNode TreeNode;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova árvore binária vazia
 *
 * @param element_size Tamanho de cada elemento em bytes
 * @param compare Função de comparação (NULL se não usar ordenação)
 * @param destroy Função de destruição (NULL se não necessário)
 * @return BinaryTree* Ponteiro para a árvore criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);
 * @endcode
 *
 * Complexidade: O(1)
 */
BinaryTree* btree_create(size_t element_size, CompareFn compare, DestroyFn destroy);

/**
 * @brief Destrói a árvore e libera memória
 *
 * @param tree Ponteiro para a árvore
 *
 * Usa travessia pós-ordem para liberar todos os nós.
 *
 * Complexidade: O(n)
 */
void btree_destroy(BinaryTree *tree);

// ============================================================================
// OPERAÇÕES DE NÓS
// ============================================================================

/**
 * @brief Cria um novo nó com dados
 *
 * @param tree Ponteiro para a árvore
 * @param data Ponteiro para os dados
 * @return TreeNode* Ponteiro para o nó criado, ou NULL em caso de erro
 *
 * Complexidade: O(1)
 */
TreeNode* btree_create_node(BinaryTree *tree, const void *data);

/**
 * @brief Define a raiz da árvore
 *
 * @param tree Ponteiro para a árvore
 * @param root Ponteiro para o novo nó raiz
 *
 * Complexidade: O(1)
 */
void btree_set_root(BinaryTree *tree, TreeNode *root);

/**
 * @brief Retorna a raiz da árvore
 *
 * @param tree Ponteiro para a árvore
 * @return TreeNode* Ponteiro para a raiz, ou NULL se vazia
 *
 * Complexidade: O(1)
 */
TreeNode* btree_root(const BinaryTree *tree);

/**
 * @brief Define o filho esquerdo de um nó
 *
 * @param tree Ponteiro para a árvore
 * @param parent Nó pai
 * @param left Nó filho esquerdo
 *
 * Complexidade: O(1)
 */
void btree_set_left(BinaryTree *tree, TreeNode *parent, TreeNode *left);

/**
 * @brief Define o filho direito de um nó
 *
 * @param tree Ponteiro para a árvore
 * @param parent Nó pai
 * @param right Nó filho direito
 *
 * Complexidade: O(1)
 */
void btree_set_right(BinaryTree *tree, TreeNode *parent, TreeNode *right);

/**
 * @brief Retorna o filho esquerdo de um nó
 *
 * @param node Ponteiro para o nó
 * @return TreeNode* Filho esquerdo, ou NULL se não existe
 *
 * Complexidade: O(1)
 */
TreeNode* btree_left(const TreeNode *node);

/**
 * @brief Retorna o filho direito de um nó
 *
 * @param node Ponteiro para o nó
 * @return TreeNode* Filho direito, ou NULL se não existe
 *
 * Complexidade: O(1)
 */
TreeNode* btree_right(const TreeNode *node);

/**
 * @brief Retorna o pai de um nó
 *
 * @param node Ponteiro para o nó
 * @return TreeNode* Pai, ou NULL se raiz
 *
 * Complexidade: O(1)
 */
TreeNode* btree_parent(const TreeNode *node);

/**
 * @brief Retorna os dados de um nó
 *
 * @param node Ponteiro para o nó
 * @return void* Ponteiro para os dados
 *
 * Complexidade: O(1)
 */
void* btree_node_data(const TreeNode *node);

/**
 * @brief Verifica se um nó é folha (leaf)
 *
 * @param node Ponteiro para o nó
 * @return true se é folha (sem filhos), false caso contrário
 *
 * Complexidade: O(1)
 */
bool btree_is_leaf(const TreeNode *node);

// ============================================================================
// TRAVESSIAS (TREE TRAVERSALS)
// ============================================================================

/**
 * @brief Tipo de função callback para travessias
 *
 * @param data Ponteiro para os dados do nó
 * @param user_data Dados customizados do usuário
 */
typedef void (*TraversalFn)(void *data, void *user_data);

/**
 * @brief Travessia em ordem (Inorder): esquerda → raiz → direita
 *
 * @param tree Ponteiro para a árvore
 * @param callback Função a ser chamada para cada nó
 * @param user_data Dados customizados passados para o callback
 *
 * Pseudocódigo (Cormen et al., 2009, p. 288):
 * INORDER-TREE-WALK(x)
 *   if x ≠ NIL
 *       INORDER-TREE-WALK(x.left)
 *       print x.key
 *       INORDER-TREE-WALK(x.right)
 *
 * Para BST: visita nós em ordem crescente
 *
 * Complexidade: O(n)
 */
void btree_inorder(const BinaryTree *tree, TraversalFn callback, void *user_data);

/**
 * @brief Travessia pré-ordem (Preorder): raiz → esquerda → direita
 *
 * @param tree Ponteiro para a árvore
 * @param callback Função a ser chamada para cada nó
 * @param user_data Dados customizados
 *
 * Pseudocódigo:
 * PREORDER-TREE-WALK(x)
 *   if x ≠ NIL
 *       print x.key
 *       PREORDER-TREE-WALK(x.left)
 *       PREORDER-TREE-WALK(x.right)
 *
 * Útil para: copiar árvore, serialização
 *
 * Complexidade: O(n)
 */
void btree_preorder(const BinaryTree *tree, TraversalFn callback, void *user_data);

/**
 * @brief Travessia pós-ordem (Postorder): esquerda → direita → raiz
 *
 * @param tree Ponteiro para a árvore
 * @param callback Função a ser chamada para cada nó
 * @param user_data Dados customizados
 *
 * Pseudocódigo:
 * POSTORDER-TREE-WALK(x)
 *   if x ≠ NIL
 *       POSTORDER-TREE-WALK(x.left)
 *       POSTORDER-TREE-WALK(x.right)
 *       print x.key
 *
 * Útil para: deletar árvore, avaliar expressões
 *
 * Complexidade: O(n)
 */
void btree_postorder(const BinaryTree *tree, TraversalFn callback, void *user_data);

/**
 * @brief Travessia em nível (Level-order / BFS)
 *
 * @param tree Ponteiro para a árvore
 * @param callback Função a ser chamada para cada nó
 * @param user_data Dados customizados
 *
 * Visita nós nível por nível, da esquerda para direita.
 * Implementação usa uma fila (Queue).
 *
 * Complexidade: O(n)
 */
void btree_levelorder(const BinaryTree *tree, TraversalFn callback, void *user_data);

// ============================================================================
// CONSULTAS E PROPRIEDADES
// ============================================================================

/**
 * @brief Verifica se a árvore está vazia
 *
 * @param tree Ponteiro para a árvore
 * @return true se vazia, false caso contrário
 *
 * Complexidade: O(1)
 */
bool btree_is_empty(const BinaryTree *tree);

/**
 * @brief Retorna o número de nós na árvore
 *
 * @param tree Ponteiro para a árvore
 * @return size_t Número de nós
 *
 * Complexidade: O(1) se mantém contador, O(n) se contar recursivamente
 */
size_t btree_size(const BinaryTree *tree);

/**
 * @brief Retorna a altura da árvore
 *
 * @param tree Ponteiro para a árvore
 * @return int Altura (folha = 0, vazia = -1)
 *
 * Definição: altura = max(altura_esquerda, altura_direita) + 1
 *
 * Complexidade: O(n)
 */
int btree_height(const BinaryTree *tree);

/**
 * @brief Retorna o número de folhas
 *
 * @param tree Ponteiro para a árvore
 * @return size_t Número de folhas
 *
 * Complexidade: O(n)
 */
size_t btree_leaf_count(const BinaryTree *tree);

/**
 * @brief Verifica se a árvore é completa (complete)
 *
 * @param tree Ponteiro para a árvore
 * @return true se completa (todos os níveis cheios exceto último)
 *
 * Definição: Todos os níveis completamente preenchidos exceto possivelmente
 * o último, que está preenchido da esquerda para direita.
 *
 * Complexidade: O(n)
 */
bool btree_is_complete(const BinaryTree *tree);

/**
 * @brief Verifica se a árvore é cheia (full)
 *
 * @param tree Ponteiro para a árvore
 * @return true se cheia (cada nó tem 0 ou 2 filhos)
 *
 * Complexidade: O(n)
 */
bool btree_is_full(const BinaryTree *tree);

/**
 * @brief Verifica se a árvore é perfeita (perfect)
 *
 * @param tree Ponteiro para a árvore
 * @return true se perfeita (completa e cheia, todos níveis preenchidos)
 *
 * Definição: Todos os nós internos têm 2 filhos e todas as folhas
 * estão no mesmo nível.
 * Propriedade: n = 2^(h+1) - 1 onde h é a altura
 *
 * Complexidade: O(n)
 */
bool btree_is_perfect(const BinaryTree *tree);

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Busca um elemento na árvore (linear search)
 *
 * @param tree Ponteiro para a árvore
 * @param data Ponteiro para o dado a buscar
 * @return TreeNode* Nó encontrado, ou NULL
 *
 * Usa travessia para buscar (não assume ordenação).
 * Para busca eficiente em árvore ordenada, use BST.
 *
 * Complexidade: O(n)
 */
TreeNode* btree_find(const BinaryTree *tree, const void *data);

/**
 * @brief Remove um nó da árvore
 *
 * @param tree Ponteiro para a árvore
 * @param node Nó a ser removido
 * @return DataStructureError Código de erro
 *
 * Remove o nó e reconecta filhos (estratégia depende da implementação).
 *
 * Complexidade: O(h)
 */
DataStructureError btree_remove_node(BinaryTree *tree, TreeNode *node);

/**
 * @brief Limpa a árvore (remove todos os nós)
 *
 * @param tree Ponteiro para a árvore
 *
 * Complexidade: O(n)
 */
void btree_clear(BinaryTree *tree);

/**
 * @brief Cria uma cópia da árvore
 *
 * @param tree Ponteiro para a árvore original
 * @param copy_fn Função de cópia (NULL para memcpy)
 * @return BinaryTree* Nova árvore, ou NULL em caso de erro
 *
 * Complexidade: O(n)
 */
BinaryTree* btree_clone(const BinaryTree *tree, CopyFn copy_fn);

/**
 * @brief Imprime a árvore visualmente (para debugging)
 *
 * @param tree Ponteiro para a árvore
 * @param print Função de impressão dos dados
 *
 * Imprime a árvore em formato hierárquico.
 *
 * Complexidade: O(n)
 */
void btree_print(const BinaryTree *tree, PrintFn print);

/**
 * @brief Converte árvore para array (via travessia)
 *
 * @param tree Ponteiro para a árvore
 * @param array Ponteiro para array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho
 * @param inorder Se true usa inorder, se false usa levelorder
 * @return DataStructureError Código de erro
 *
 * Array deve ser liberado com free().
 *
 * Complexidade: O(n)
 */
DataStructureError btree_to_array(const BinaryTree *tree, void **array, size_t *size, bool inorder);

// ============================================================================
// OPERAÇÕES DE ANÁLISE
// ============================================================================

/**
 * @brief Calcula o menor ancestral comum (LCA - Lowest Common Ancestor)
 *
 * @param tree Ponteiro para a árvore
 * @param node1 Primeiro nó
 * @param node2 Segundo nó
 * @return TreeNode* LCA, ou NULL se não encontrado
 *
 * Referência: Cormen et al., Exercise 12-3
 *
 * Complexidade: O(h)
 */
TreeNode* btree_lca(const BinaryTree *tree, TreeNode *node1, TreeNode *node2);

/**
 * @brief Calcula a distância entre dois nós
 *
 * @param tree Ponteiro para a árvore
 * @param node1 Primeiro nó
 * @param node2 Segundo nó
 * @return int Distância (número de arestas), -1 se não conectados
 *
 * Usa LCA para calcular: dist(n1, n2) = dist(n1, LCA) + dist(n2, LCA)
 *
 * Complexidade: O(h)
 */
int btree_distance(const BinaryTree *tree, TreeNode *node1, TreeNode *node2);

/**
 * @brief Calcula o diâmetro da árvore
 *
 * @param tree Ponteiro para a árvore
 * @return int Diâmetro (maior distância entre duas folhas)
 *
 * Definição: O maior caminho entre duas folhas quaisquer.
 *
 * Complexidade: O(n)
 */
int btree_diameter(const BinaryTree *tree);

#endif // BINARY_TREE_H
