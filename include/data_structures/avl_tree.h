/**
 * @file avl_tree.h
 * @brief Árvore AVL - BST auto-balanceada
 *
 * AVL Tree garante altura O(log n) através de rotações após inserção/remoção.
 * Mantém fator de balanceamento |altura_esq - altura_dir| ≤ 1 para todos os nós.
 *
 * Complexidade GARANTIDA (não amortizada):
 * - Insert: O(log n)
 * - Search: O(log n)
 * - Remove: O(log n)
 * - Min/Max: O(log n)
 *
 * vs BST simples: pior caso O(n) quando desbalanceada
 * vs Red-Black: AVL é mais rígida (mais balanceada), melhor para buscas
 *
 * Referências:
 * - Adelson-Velsky, G., & Landis, E. M. (1962). "An algorithm for the
 *   organization of information"
 * - Cormen et al. (2009), Chapter 13 (Red-Black) - conceitos aplicam a AVL
 * - Knuth TAOCP Vol 3, Section 6.2.3
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct AVLTree AVLTree;
typedef struct AVLNode AVLNode;

/**
 * @brief Cria AVL tree
 */
AVLTree* avl_create(size_t element_size, CompareFn compare, DestroyFn destroy);
void avl_destroy(AVLTree *tree);

/**
 * @brief Insere elemento mantendo balanceamento
 *
 * Algoritmo:
 * 1. Inserção BST normal
 * 2. Atualizar altura dos ancestrais
 * 3. Verificar fator de balanceamento
 * 4. Rotacionar se necessário (LL, RR, LR, RL)
 *
 * Rotações:
 * - LL (Left-Left): rotação direita simples
 * - RR (Right-Right): rotação esquerda simples
 * - LR (Left-Right): rotação esquerda-direita dupla
 * - RL (Right-Left): rotação direita-esquerda dupla
 *
 * Complexidade: O(log n) GARANTIDO
 */
DataStructureError avl_insert(AVLTree *tree, const void *data);

/**
 * @brief Busca elemento
 *
 * Complexidade: O(log n) GARANTIDO
 */
DataStructureError avl_search(const AVLTree *tree, const void *data, void *output);
bool avl_contains(const AVLTree *tree, const void *data);

/**
 * @brief Remove elemento mantendo balanceamento
 *
 * Complexidade: O(log n) GARANTIDO
 */
DataStructureError avl_remove(AVLTree *tree, const void *data);

/**
 * @brief Retorna min/max
 */
DataStructureError avl_min(const AVLTree *tree, void *output);
DataStructureError avl_max(const AVLTree *tree, void *output);

/**
 * @brief Range search
 */
DataStructureError avl_range_search(const AVLTree *tree, const void *min,
                                    const void *max, void **results, size_t *count);

// Travessias
typedef void (*AVLTraversalFn)(void *data, void *user_data);
void avl_inorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data);
void avl_preorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data);
void avl_postorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data);

// Propriedades
bool avl_is_empty(const AVLTree *tree);
size_t avl_size(const AVLTree *tree);
int avl_height(const AVLTree *tree);

/**
 * @brief Verifica se a árvore é AVL válida
 *
 * Verifica:
 * 1. Propriedade BST
 * 2. Fator de balanceamento de cada nó em [-1, 0, 1]
 * 3. Alturas corretas
 */
bool avl_is_valid(const AVLTree *tree);

void avl_clear(AVLTree *tree);
AVLTree* avl_clone(const AVLTree *tree, CopyFn copy_fn);
void avl_print(const AVLTree *tree, PrintFn print);

#endif // AVL_TREE_H
