/**
 * @file binary_tree.c
 * @brief Implementação de Árvore Binária genérica
 *
 * Estrutura hierárquica onde cada nó tem no máximo dois filhos.
 * Implementa travessias (inorder, preorder, postorder, levelorder),
 * propriedades (altura, tamanho, completa, cheia, perfeita) e
 * operações avançadas (LCA, diâmetro, distância).
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 12 - Binary Search Trees
 * - Knuth (1997), TAOCP Vol 1, Section 2.3 - Trees
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/binary_tree.h"
#include "data_structures/queue.h"  // Para levelorder

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

/**
 * @brief Nó da árvore binária
 */
struct TreeNode {
    void *data;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
};

/**
 * @brief Estrutura da árvore binária
 */
struct BinaryTree {
    TreeNode *root;
    size_t size;
    size_t element_size;
    CompareFn compare;
    DestroyFn destroy;
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

/**
 * @brief Cria um nó alocando memória
 */
static TreeNode* create_tree_node(size_t element_size, const void *data) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        return NULL;
    }

    node->data = malloc(element_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, element_size);
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

/**
 * @brief Destrói um nó recursivamente (pós-ordem)
 */
static void destroy_node_recursive(TreeNode *node, DestroyFn destroy) {
    if (node == NULL) {
        return;
    }

    // Pós-ordem: esquerda → direita → raiz
    destroy_node_recursive(node->left, destroy);
    destroy_node_recursive(node->right, destroy);

    if (destroy != NULL) {
        destroy(node->data);
    }
    free(node->data);
    free(node);
}

/**
 * @brief Conta nós recursivamente
 */
static size_t count_nodes_recursive(const TreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes_recursive(node->left) + count_nodes_recursive(node->right);
}

/**
 * @brief Calcula altura recursivamente
 */
static int height_recursive(const TreeNode *node) {
    if (node == NULL) {
        return -1;  // Altura de árvore vazia é -1
    }

    int left_height = height_recursive(node->left);
    int right_height = height_recursive(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

/**
 * @brief Conta folhas recursivamente
 */
static size_t count_leaves_recursive(const TreeNode *node) {
    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return 1;  // É folha
    }

    return count_leaves_recursive(node->left) + count_leaves_recursive(node->right);
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

BinaryTree* btree_create(size_t element_size, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0) {
        return NULL;
    }

    BinaryTree *tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    tree->element_size = element_size;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;
}

void btree_destroy(BinaryTree *tree) {
    if (tree == NULL) {
        return;
    }

    destroy_node_recursive(tree->root, tree->destroy);
    free(tree);
}

// ============================================================================
// OPERAÇÕES DE NÓS
// ============================================================================

TreeNode* btree_create_node(BinaryTree *tree, const void *data) {
    if (tree == NULL || data == NULL) {
        return NULL;
    }

    TreeNode *node = create_tree_node(tree->element_size, data);
    if (node != NULL) {
        tree->size++;
    }

    return node;
}

void btree_set_root(BinaryTree *tree, TreeNode *root) {
    if (tree == NULL) {
        return;
    }

    tree->root = root;
    if (root != NULL) {
        root->parent = NULL;
    }
}

TreeNode* btree_root(const BinaryTree *tree) {
    return (tree == NULL) ? NULL : tree->root;
}

void btree_set_left(BinaryTree *tree, TreeNode *parent, TreeNode *left) {
    if (tree == NULL || parent == NULL) {
        return;
    }

    parent->left = left;
    if (left != NULL) {
        left->parent = parent;
    }
}

void btree_set_right(BinaryTree *tree, TreeNode *parent, TreeNode *right) {
    if (tree == NULL || parent == NULL) {
        return;
    }

    parent->right = right;
    if (right != NULL) {
        right->parent = parent;
    }
}

TreeNode* btree_left(const TreeNode *node) {
    return (node == NULL) ? NULL : node->left;
}

TreeNode* btree_right(const TreeNode *node) {
    return (node == NULL) ? NULL : node->right;
}

TreeNode* btree_parent(const TreeNode *node) {
    return (node == NULL) ? NULL : node->parent;
}

void* btree_node_data(const TreeNode *node) {
    return (node == NULL) ? NULL : node->data;
}

bool btree_is_leaf(const TreeNode *node) {
    if (node == NULL) {
        return false;
    }
    return (node->left == NULL && node->right == NULL);
}

// ============================================================================
// TRAVESSIAS
// ============================================================================

/**
 * @brief Travessia inorder recursiva
 *
 * Pseudocódigo (Cormen p. 288):
 * INORDER-TREE-WALK(x)
 *   if x ≠ NIL
 *       INORDER-TREE-WALK(x.left)
 *       print x.key
 *       INORDER-TREE-WALK(x.right)
 */
static void inorder_recursive(const TreeNode *node, TraversalFn callback, void *user_data) {
    if (node == NULL) {
        return;
    }

    inorder_recursive(node->left, callback, user_data);
    callback(node->data, user_data);
    inorder_recursive(node->right, callback, user_data);
}

void btree_inorder(const BinaryTree *tree, TraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) {
        return;
    }

    inorder_recursive(tree->root, callback, user_data);
}

/**
 * @brief Travessia preorder recursiva
 */
static void preorder_recursive(const TreeNode *node, TraversalFn callback, void *user_data) {
    if (node == NULL) {
        return;
    }

    callback(node->data, user_data);
    preorder_recursive(node->left, callback, user_data);
    preorder_recursive(node->right, callback, user_data);
}

void btree_preorder(const BinaryTree *tree, TraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) {
        return;
    }

    preorder_recursive(tree->root, callback, user_data);
}

/**
 * @brief Travessia postorder recursiva
 */
static void postorder_recursive(const TreeNode *node, TraversalFn callback, void *user_data) {
    if (node == NULL) {
        return;
    }

    postorder_recursive(node->left, callback, user_data);
    postorder_recursive(node->right, callback, user_data);
    callback(node->data, user_data);
}

void btree_postorder(const BinaryTree *tree, TraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) {
        return;
    }

    postorder_recursive(tree->root, callback, user_data);
}

/**
 * @brief Travessia level-order (BFS)
 *
 * Usa uma fila para processar nível por nível
 */
void btree_levelorder(const BinaryTree *tree, TraversalFn callback, void *user_data) {
    if (tree == NULL || tree->root == NULL || callback == NULL) {
        return;
    }

    Queue *queue = queue_create(sizeof(TreeNode*), QUEUE_LINKED, 0, NULL);
    if (queue == NULL) {
        return;
    }

    TreeNode *root = tree->root;
    queue_enqueue(queue, &root);

    while (!queue_is_empty(queue)) {
        TreeNode *node;
        queue_dequeue(queue, &node);

        callback(node->data, user_data);

        if (node->left != NULL) {
            queue_enqueue(queue, &node->left);
        }
        if (node->right != NULL) {
            queue_enqueue(queue, &node->right);
        }
    }

    queue_destroy(queue);
}

// ============================================================================
// CONSULTAS E PROPRIEDADES
// ============================================================================

bool btree_is_empty(const BinaryTree *tree) {
    return (tree == NULL || tree->root == NULL);
}

size_t btree_size(const BinaryTree *tree) {
    if (tree == NULL) {
        return 0;
    }
    // Alternativa: return count_nodes_recursive(tree->root);
    return tree->size;
}

int btree_height(const BinaryTree *tree) {
    if (tree == NULL) {
        return -1;
    }
    return height_recursive(tree->root);
}

size_t btree_leaf_count(const BinaryTree *tree) {
    if (tree == NULL) {
        return 0;
    }
    return count_leaves_recursive(tree->root);
}

/**
 * @brief Verifica se árvore é completa usando BFS
 *
 * Completa: todos os níveis cheios exceto último (preenchido esquerda→direita)
 */
bool btree_is_complete(const BinaryTree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return true;
    }

    Queue *queue = queue_create(sizeof(TreeNode*), QUEUE_LINKED, 0, NULL);
    if (queue == NULL) {
        return false;
    }

    TreeNode *root = tree->root;
    queue_enqueue(queue, &root);

    bool reached_incomplete = false;

    while (!queue_is_empty(queue)) {
        TreeNode *node;
        queue_dequeue(queue, &node);

        // Se já vimos nó incompleto, não deve haver mais filhos
        if (node->left != NULL) {
            if (reached_incomplete) {
                queue_destroy(queue);
                return false;
            }
            queue_enqueue(queue, &node->left);
        } else {
            reached_incomplete = true;
        }

        if (node->right != NULL) {
            if (reached_incomplete) {
                queue_destroy(queue);
                return false;
            }
            queue_enqueue(queue, &node->right);
        } else {
            reached_incomplete = true;
        }
    }

    queue_destroy(queue);
    return true;
}

/**
 * @brief Verifica se árvore é cheia (full)
 *
 * Cheia: cada nó tem 0 ou 2 filhos
 */
static bool is_full_recursive(const TreeNode *node) {
    if (node == NULL) {
        return true;
    }

    // Folha
    if (node->left == NULL && node->right == NULL) {
        return true;
    }

    // Ambos filhos presentes
    if (node->left != NULL && node->right != NULL) {
        return is_full_recursive(node->left) && is_full_recursive(node->right);
    }

    // Apenas um filho: não é full
    return false;
}

bool btree_is_full(const BinaryTree *tree) {
    if (tree == NULL) {
        return true;
    }
    return is_full_recursive(tree->root);
}

/**
 * @brief Verifica se árvore é perfeita
 *
 * Perfeita: todos os níveis completamente preenchidos
 * Propriedade: n = 2^(h+1) - 1
 */
bool btree_is_perfect(const BinaryTree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return true;
    }

    int h = btree_height(tree);
    size_t expected_size = (size_t)pow(2, h + 1) - 1;

    return (tree->size == expected_size) && btree_is_full(tree);
}

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Busca elemento recursivamente
 */
static TreeNode* find_recursive(const TreeNode *node, const void *data, CompareFn compare) {
    if (node == NULL) {
        return NULL;
    }

    if (compare(node->data, data) == 0) {
        return (TreeNode*)node;
    }

    TreeNode *left_result = find_recursive(node->left, data, compare);
    if (left_result != NULL) {
        return left_result;
    }

    return find_recursive(node->right, data, compare);
}

TreeNode* btree_find(const BinaryTree *tree, const void *data) {
    if (tree == NULL || data == NULL || tree->compare == NULL) {
        return NULL;
    }

    return find_recursive(tree->root, data, tree->compare);
}

/**
 * @brief Remove nó (estratégia simples: promove filho)
 */
DataStructureError btree_remove_node(BinaryTree *tree, TreeNode *node) {
    if (tree == NULL || node == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    // Caso 1: Nó é folha
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;  // Era raiz
        } else if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }

        if (tree->destroy != NULL) {
            tree->destroy(node->data);
        }
        free(node->data);
        free(node);
        tree->size--;
        return DS_SUCCESS;
    }

    // Caso 2: Nó tem um filho
    TreeNode *child = (node->left != NULL) ? node->left : node->right;

    if (node->parent == NULL) {
        tree->root = child;  // Nó era raiz
        child->parent = NULL;
    } else if (node->parent->left == node) {
        node->parent->left = child;
        child->parent = node->parent;
    } else {
        node->parent->right = child;
        child->parent = node->parent;
    }

    if (tree->destroy != NULL) {
        tree->destroy(node->data);
    }
    free(node->data);
    free(node);
    tree->size--;

    return DS_SUCCESS;
}

void btree_clear(BinaryTree *tree) {
    if (tree == NULL) {
        return;
    }

    destroy_node_recursive(tree->root, tree->destroy);
    tree->root = NULL;
    tree->size = 0;
}

/**
 * @brief Clona árvore recursivamente
 */
static TreeNode* clone_recursive(const TreeNode *node, size_t element_size, CopyFn copy_fn) {
    if (node == NULL) {
        return NULL;
    }

    TreeNode *new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = malloc(element_size);
    if (new_node->data == NULL) {
        free(new_node);
        return NULL;
    }

    if (copy_fn != NULL) {
        void *copied = copy_fn(node->data);
        memcpy(new_node->data, copied, element_size);
        free(copied);
    } else {
        memcpy(new_node->data, node->data, element_size);
    }

    new_node->left = clone_recursive(node->left, element_size, copy_fn);
    new_node->right = clone_recursive(node->right, element_size, copy_fn);
    new_node->parent = NULL;

    if (new_node->left != NULL) {
        new_node->left->parent = new_node;
    }
    if (new_node->right != NULL) {
        new_node->right->parent = new_node;
    }

    return new_node;
}

BinaryTree* btree_clone(const BinaryTree *tree, CopyFn copy_fn) {
    if (tree == NULL) {
        return NULL;
    }

    BinaryTree *new_tree = btree_create(tree->element_size, tree->compare, tree->destroy);
    if (new_tree == NULL) {
        return NULL;
    }

    new_tree->root = clone_recursive(tree->root, tree->element_size, copy_fn);
    new_tree->size = tree->size;

    return new_tree;
}

/**
 * @brief Imprime árvore recursivamente (formato hierárquico)
 */
static void print_recursive(const TreeNode *node, PrintFn print, int depth) {
    if (node == NULL) {
        return;
    }

    // Direita primeiro (para imprimir como árvore deitada)
    print_recursive(node->right, print, depth + 1);

    // Indentação
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    print(node->data);
    printf("\n");

    print_recursive(node->left, print, depth + 1);
}

void btree_print(const BinaryTree *tree, PrintFn print) {
    if (tree == NULL || print == NULL) {
        return;
    }

    printf("BinaryTree (size=%zu, height=%d):\n", tree->size, btree_height(tree));
    print_recursive(tree->root, print, 0);
}

/**
 * @brief Converte árvore para array
 */
DataStructureError btree_to_array(const BinaryTree *tree, void **array, size_t *size, bool inorder) {
    if (tree == NULL || array == NULL || size == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (tree->size == 0) {
        *array = NULL;
        *size = 0;
        return DS_SUCCESS;
    }

    void *result = malloc(tree->size * tree->element_size);
    if (result == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    // Usar callback para copiar elementos
    size_t index = 0;
    void copy_to_array(void *data, void *user_data) {
        void *dest = (char*)result + (index * tree->element_size);
        memcpy(dest, data, tree->element_size);
        index++;
    }

    if (inorder) {
        btree_inorder(tree, copy_to_array, NULL);
    } else {
        btree_levelorder(tree, copy_to_array, NULL);
    }

    *array = result;
    *size = tree->size;
    return DS_SUCCESS;
}

// ============================================================================
// OPERAÇÕES DE ANÁLISE
// ============================================================================

/**
 * @brief Calcula profundidade de um nó
 */
static int node_depth(const TreeNode *node) {
    int depth = 0;
    while (node->parent != NULL) {
        depth++;
        node = node->parent;
    }
    return depth;
}

/**
 * @brief Lowest Common Ancestor (LCA)
 */
TreeNode* btree_lca(const BinaryTree *tree, TreeNode *node1, TreeNode *node2) {
    if (tree == NULL || node1 == NULL || node2 == NULL) {
        return NULL;
    }

    // Igualar profundidades
    int depth1 = node_depth(node1);
    int depth2 = node_depth(node2);

    while (depth1 > depth2) {
        node1 = node1->parent;
        depth1--;
    }

    while (depth2 > depth1) {
        node2 = node2->parent;
        depth2--;
    }

    // Subir simultaneamente até encontrar ancestral comum
    while (node1 != node2) {
        node1 = node1->parent;
        node2 = node2->parent;
    }

    return node1;
}

/**
 * @brief Calcula distância entre dois nós
 */
int btree_distance(const BinaryTree *tree, TreeNode *node1, TreeNode *node2) {
    if (tree == NULL || node1 == NULL || node2 == NULL) {
        return -1;
    }

    TreeNode *lca = btree_lca(tree, node1, node2);
    if (lca == NULL) {
        return -1;  // Nós não conectados
    }

    int dist1 = node_depth(node1) - node_depth(lca);
    int dist2 = node_depth(node2) - node_depth(lca);

    return dist1 + dist2;
}

/**
 * @brief Calcula diâmetro recursivamente
 */
static int diameter_recursive(const TreeNode *node, int *diameter) {
    if (node == NULL) {
        return 0;
    }

    int left_height = diameter_recursive(node->left, diameter);
    int right_height = diameter_recursive(node->right, diameter);

    // Atualizar diâmetro se caminho por este nó é maior
    int path_through_node = left_height + right_height;
    if (path_through_node > *diameter) {
        *diameter = path_through_node;
    }

    return 1 + (left_height > right_height ? left_height : right_height);
}

int btree_diameter(const BinaryTree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }

    int diameter = 0;
    diameter_recursive(tree->root, &diameter);
    return diameter;
}
