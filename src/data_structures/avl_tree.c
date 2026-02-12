/**
 * @file avl_tree.c
 * @brief Implementação de Árvore AVL (BST auto-balanceada)
 *
 * Implementa árvore AVL com balanceamento automático através de rotações.
 * Garante altura O(log n) mantendo |balance_factor| ≤ 1 para todos os nós.
 *
 * Rotações:
 * - LL (Left-Left): rotação direita simples
 * - RR (Right-Right): rotação esquerda simples
 * - LR (Left-Right): rotação esquerda no filho esquerdo, depois direita
 * - RL (Right-Left): rotação direita no filho direito, depois esquerda
 *
 * Referências:
 * - Adelson-Velsky, G., & Landis, E. M. (1962). "An algorithm for the
 *   organization of information". Doklady Akademii Nauk SSSR, 146, 263-266.
 * - Cormen et al. (2009), "Introduction to Algorithms" 3rd ed., Ch. 13
 * - Knuth TAOCP Vol 3, Section 6.2.3
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

struct AVLNode {
    void *data;
    AVLNode *left;
    AVLNode *right;
    int height;
};

struct AVLTree {
    AVLNode *root;
    size_t element_size;
    size_t size;
    CompareFn compare;
    DestroyFn destroy;
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

static int node_height(const AVLNode *node) {
    return node == NULL ? -1 : node->height;
}

static int balance_factor(const AVLNode *node) {
    return node == NULL ? 0 : node_height(node->left) - node_height(node->right);
}

static void update_height(AVLNode *node) {
    if (node == NULL) return;
    int lh = node_height(node->left);
    int rh = node_height(node->right);
    node->height = 1 + (lh > rh ? lh : rh);
}

/**
 * Rotação direita (caso LL):
 *
 *       y            x
 *      / \          / \
 *     x   C  =>   A   y
 *    / \              / \
 *   A   B            B   C
 */
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *B = x->right;

    x->right = y;
    y->left = B;

    update_height(y);
    update_height(x);

    return x;
}

/**
 * Rotação esquerda (caso RR):
 *
 *     x              y
 *    / \            / \
 *   A   y    =>   x   C
 *      / \       / \
 *     B   C     A   B
 */
static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *B = y->left;

    y->left = x;
    x->right = B;

    update_height(x);
    update_height(y);

    return y;
}

static AVLNode* balance(AVLNode *node) {
    if (node == NULL) return NULL;

    update_height(node);
    int bf = balance_factor(node);

    /* LL: left-heavy, left child is left-heavy or balanced */
    if (bf > 1 && balance_factor(node->left) >= 0) {
        return rotate_right(node);
    }

    /* LR: left-heavy, left child is right-heavy */
    if (bf > 1 && balance_factor(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    /* RR: right-heavy, right child is right-heavy or balanced */
    if (bf < -1 && balance_factor(node->right) <= 0) {
        return rotate_left(node);
    }

    /* RL: right-heavy, right child is left-heavy */
    if (bf < -1 && balance_factor(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

static AVLNode* create_node(const void *data, size_t element_size) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (node == NULL) return NULL;

    node->data = malloc(element_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, element_size);
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

static void destroy_node(AVLNode *node, DestroyFn destroy) {
    if (node == NULL) return;

    if (destroy != NULL && node->data != NULL) {
        destroy(node->data);
    }

    free(node->data);
    free(node);
}

static void destroy_recursive(AVLNode *node, DestroyFn destroy) {
    if (node == NULL) return;

    destroy_recursive(node->left, destroy);
    destroy_recursive(node->right, destroy);
    destroy_node(node, destroy);
}

static AVLNode* find_min_node(AVLNode *node) {
    if (node == NULL) return NULL;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static AVLNode* find_max_node(AVLNode *node) {
    if (node == NULL) return NULL;
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

static AVLNode* insert_recursive(AVLNode *node, const void *data,
                                 size_t element_size, CompareFn compare,
                                 bool *success) {
    if (node == NULL) {
        AVLNode *new_node = create_node(data, element_size);
        if (new_node == NULL) {
            *success = false;
        }
        return new_node;
    }

    int cmp = compare(data, node->data);
    if (cmp < 0) {
        node->left = insert_recursive(node->left, data, element_size, compare, success);
    } else if (cmp > 0) {
        node->right = insert_recursive(node->right, data, element_size, compare, success);
    } else {
        node->right = insert_recursive(node->right, data, element_size, compare, success);
    }

    return balance(node);
}

static AVLNode* remove_recursive(AVLNode *node, const void *key,
                                 CompareFn compare, DestroyFn destroy,
                                 size_t element_size, bool *found) {
    if (node == NULL) {
        *found = false;
        return NULL;
    }

    int cmp = compare(key, node->data);

    if (cmp < 0) {
        node->left = remove_recursive(node->left, key, compare, destroy, element_size, found);
    } else if (cmp > 0) {
        node->right = remove_recursive(node->right, key, compare, destroy, element_size, found);
    } else {
        *found = true;

        if (node->left == NULL || node->right == NULL) {
            AVLNode *child = node->left ? node->left : node->right;

            if (child == NULL) {
                destroy_node(node, destroy);
                return NULL;
            } else {
                destroy_node(node, destroy);
                return child;
            }
        } else {
            /* In-order successor: smallest node in right subtree */
            AVLNode *successor = find_min_node(node->right);
            void *temp_data = malloc(element_size);
            if (temp_data != NULL) {
                memcpy(temp_data, successor->data, element_size);

                if (destroy != NULL && node->data != NULL) {
                    destroy(node->data);
                }
                memcpy(node->data, temp_data, element_size);
                free(temp_data);

                bool successor_found = false;
                node->right = remove_recursive(node->right, successor->data,
                                               compare, destroy, element_size,
                                               &successor_found);
            }
        }
    }

    return balance(node);
}

static void inorder_recursive(const AVLNode *node, AVLTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    inorder_recursive(node->left, callback, user_data);
    callback(node->data, user_data);
    inorder_recursive(node->right, callback, user_data);
}

static void preorder_recursive(const AVLNode *node, AVLTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    callback(node->data, user_data);
    preorder_recursive(node->left, callback, user_data);
    preorder_recursive(node->right, callback, user_data);
}

static void postorder_recursive(const AVLNode *node, AVLTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    postorder_recursive(node->left, callback, user_data);
    postorder_recursive(node->right, callback, user_data);
    callback(node->data, user_data);
}

static bool is_valid_recursive(const AVLNode *node, const void *min, const void *max,
                               CompareFn compare, int *computed_height) {
    if (node == NULL) {
        *computed_height = -1;
        return true;
    }

    if (min != NULL && compare(node->data, min) < 0) return false;
    if (max != NULL && compare(node->data, max) > 0) return false;

    int left_height, right_height;

    if (!is_valid_recursive(node->left, min, node->data, compare, &left_height)) return false;
    if (!is_valid_recursive(node->right, node->data, max, compare, &right_height)) return false;

    int expected_height = 1 + (left_height > right_height ? left_height : right_height);
    if (node->height != expected_height) return false;

    int bf = left_height - right_height;
    if (bf < -1 || bf > 1) return false;

    *computed_height = expected_height;
    return true;
}

static AVLNode* clone_recursive(const AVLNode *node, size_t element_size, CopyFn copy_fn) {
    if (node == NULL) return NULL;

    AVLNode *new_node = (AVLNode*)malloc(sizeof(AVLNode));
    if (new_node == NULL) return NULL;

    new_node->data = malloc(element_size);
    if (new_node->data == NULL) {
        free(new_node);
        return NULL;
    }

    if (copy_fn != NULL) {
        void *copied = copy_fn(node->data);
        if (copied == NULL) {
            free(new_node->data);
            free(new_node);
            return NULL;
        }
        memcpy(new_node->data, copied, element_size);
        free(copied);
    } else {
        memcpy(new_node->data, node->data, element_size);
    }

    new_node->height = node->height;
    new_node->left = clone_recursive(node->left, element_size, copy_fn);
    new_node->right = clone_recursive(node->right, element_size, copy_fn);

    return new_node;
}

static size_t count_nodes(const AVLNode *node) {
    if (node == NULL) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

static void range_collect(const AVLNode *node, const void *min, const void *max,
                          CompareFn compare, size_t element_size,
                          void *array, size_t *count, size_t *capacity) {
    if (node == NULL) return;

    int cmp_min = compare(node->data, min);
    int cmp_max = compare(node->data, max);

    /* Prune: only search left if node->data > min (there might be values >= min in left subtree) */
    if (cmp_min > 0) {
        range_collect(node->left, min, max, compare, element_size, array, count, capacity);
    }

    /* Collect node if in range [min, max] */
    if (cmp_min >= 0 && cmp_max <= 0 && *count < *capacity) {
        memcpy((char*)array + (*count * element_size), node->data, element_size);
        (*count)++;
    }

    /* Prune: only search right if node->data < max (there might be values <= max in right subtree) */
    if (cmp_max < 0) {
        range_collect(node->right, min, max, compare, element_size, array, count, capacity);
    }
}

static void range_count_nodes(const AVLNode *node, const void *min, const void *max,
                              CompareFn compare, size_t *count) {
    if (node == NULL) return;

    int cmp_min = compare(node->data, min);
    int cmp_max = compare(node->data, max);

    if (cmp_min > 0) {
        range_count_nodes(node->left, min, max, compare, count);
    }

    if (cmp_min >= 0 && cmp_max <= 0) {
        (*count)++;
    }

    if (cmp_max < 0) {
        range_count_nodes(node->right, min, max, compare, count);
    }
}

static void print_recursive(const AVLNode *node, PrintFn print, int depth) {
    if (node == NULL) return;

    print_recursive(node->right, print, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    print(node->data);
    printf(" (h=%d, bf=%d)\n", node->height, balance_factor(node));

    print_recursive(node->left, print, depth + 1);
}

// ============================================================================
// FUNÇÕES PÚBLICAS
// ============================================================================

AVLTree* avl_create(size_t element_size, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0 || compare == NULL) return NULL;

    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (tree == NULL) return NULL;

    tree->root = NULL;
    tree->element_size = element_size;
    tree->size = 0;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;
}

void avl_destroy(AVLTree *tree) {
    if (tree == NULL) return;

    destroy_recursive(tree->root, tree->destroy);
    free(tree);
}

DataStructureError avl_insert(AVLTree *tree, const void *data) {
    if (tree == NULL || data == NULL) return DS_ERROR_NULL_POINTER;

    bool success = true;
    tree->root = insert_recursive(tree->root, data, tree->element_size,
                                  tree->compare, &success);

    if (!success) return DS_ERROR_OUT_OF_MEMORY;

    tree->size++;
    return DS_SUCCESS;
}

DataStructureError avl_search(const AVLTree *tree, const void *data, void *output) {
    if (tree == NULL || data == NULL || output == NULL) return DS_ERROR_NULL_POINTER;

    AVLNode *current = tree->root;
    while (current != NULL) {
        int cmp = tree->compare(data, current->data);
        if (cmp == 0) {
            memcpy(output, current->data, tree->element_size);
            return DS_SUCCESS;
        }
        current = cmp < 0 ? current->left : current->right;
    }

    return DS_ERROR_NOT_FOUND;
}

bool avl_contains(const AVLTree *tree, const void *data) {
    if (tree == NULL || data == NULL) return false;

    AVLNode *current = tree->root;
    while (current != NULL) {
        int cmp = tree->compare(data, current->data);
        if (cmp == 0) return true;
        current = cmp < 0 ? current->left : current->right;
    }

    return false;
}

DataStructureError avl_remove(AVLTree *tree, const void *data) {
    if (tree == NULL || data == NULL) return DS_ERROR_NULL_POINTER;
    if (tree->root == NULL) return DS_ERROR_EMPTY;

    bool found = true;
    tree->root = remove_recursive(tree->root, data, tree->compare,
                                  tree->destroy, tree->element_size, &found);

    if (!found) return DS_ERROR_NOT_FOUND;

    tree->size--;
    return DS_SUCCESS;
}

DataStructureError avl_min(const AVLTree *tree, void *output) {
    if (tree == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    if (tree->root == NULL) return DS_ERROR_EMPTY;

    AVLNode *min_node = find_min_node(tree->root);
    memcpy(output, min_node->data, tree->element_size);
    return DS_SUCCESS;
}

DataStructureError avl_max(const AVLTree *tree, void *output) {
    if (tree == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    if (tree->root == NULL) return DS_ERROR_EMPTY;

    AVLNode *max_node = find_max_node(tree->root);
    memcpy(output, max_node->data, tree->element_size);
    return DS_SUCCESS;
}

DataStructureError avl_range_search(const AVLTree *tree, const void *min,
                                    const void *max, void **results, size_t *count) {
    if (tree == NULL || min == NULL || max == NULL || results == NULL || count == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (tree->root == NULL) {
        *results = NULL;
        *count = 0;
        return DS_SUCCESS;
    }

    size_t total = 0;
    range_count_nodes(tree->root, min, max, tree->compare, &total);

    if (total == 0) {
        *results = NULL;
        *count = 0;
        return DS_SUCCESS;
    }

    void *array = malloc(total * tree->element_size);
    if (array == NULL) return DS_ERROR_OUT_OF_MEMORY;

    size_t collected = 0;
    range_collect(tree->root, min, max, tree->compare, tree->element_size,
                  array, &collected, &total);

    *results = array;
    *count = collected;
    return DS_SUCCESS;
}

void avl_inorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) return;
    inorder_recursive(tree->root, callback, user_data);
}

void avl_preorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) return;
    preorder_recursive(tree->root, callback, user_data);
}

void avl_postorder(const AVLTree *tree, AVLTraversalFn callback, void *user_data) {
    if (tree == NULL || callback == NULL) return;
    postorder_recursive(tree->root, callback, user_data);
}

bool avl_is_empty(const AVLTree *tree) {
    return tree == NULL || tree->root == NULL;
}

size_t avl_size(const AVLTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

int avl_height(const AVLTree *tree) {
    if (tree == NULL) return -1;
    return node_height(tree->root);
}

bool avl_is_valid(const AVLTree *tree) {
    if (tree == NULL) return false;

    int computed_height;
    if (!is_valid_recursive(tree->root, NULL, NULL, tree->compare, &computed_height)) {
        return false;
    }

    return count_nodes(tree->root) == tree->size;
}

void avl_clear(AVLTree *tree) {
    if (tree == NULL) return;

    destroy_recursive(tree->root, tree->destroy);
    tree->root = NULL;
    tree->size = 0;
}

AVLTree* avl_clone(const AVLTree *tree, CopyFn copy_fn) {
    if (tree == NULL) return NULL;

    AVLTree *new_tree = avl_create(tree->element_size, tree->compare, tree->destroy);
    if (new_tree == NULL) return NULL;

    new_tree->root = clone_recursive(tree->root, tree->element_size, copy_fn);
    new_tree->size = tree->size;

    return new_tree;
}

void avl_print(const AVLTree *tree, PrintFn print) {
    if (tree == NULL || print == NULL) return;

    printf("AVL Tree (size=%zu, height=%d):\n", tree->size, avl_height(tree));
    print_recursive(tree->root, print, 0);
}
