/**
 * @file bst.c
 * @brief Implementação de Binary Search Tree (BST) genérica
 *
 * Implementa BST com a propriedade: esquerda < nó < direita
 * Todos os pseudocódigos seguem Cormen et al. (2009), Chapter 12.
 *
 * Referências:
 * - Cormen et al. (2009), "Introduction to Algorithms" 3rd ed., Ch. 12
 * - Knuth TAOCP Vol 3, Section 6.2.2
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

/**
 * @brief Nó da BST
 */
struct BSTNode {
    void *data;
    struct BSTNode *left;
    struct BSTNode *right;
    struct BSTNode *parent;
};

/**
 * @brief Estrutura da BST
 */
struct BST {
    BSTNode *root;
    size_t element_size;
    size_t size;
    CompareFn compare;
    DestroyFn destroy;
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

static BSTNode* create_node(const void *data, size_t element_size) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node == NULL) return NULL;
    
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

static void destroy_node(BSTNode *node, DestroyFn destroy) {
    if (node == NULL) return;
    
    if (destroy != NULL && node->data != NULL) {
        destroy(node->data);
    }
    
    free(node->data);
    free(node);
}

static void destroy_tree_recursive(BSTNode *node, DestroyFn destroy) {
    if (node == NULL) return;
    
    destroy_tree_recursive(node->left, destroy);
    destroy_tree_recursive(node->right, destroy);
    destroy_node(node, destroy);
}

static BSTNode* tree_minimum(BSTNode *node) {
    if (node == NULL) return NULL;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static BSTNode* tree_maximum(BSTNode *node) {
    if (node == NULL) return NULL;
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

static BSTNode* tree_successor(BSTNode *node) {
    if (node == NULL) return NULL;
    
    if (node->right != NULL) {
        return tree_minimum(node->right);
    }
    
    BSTNode *parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

static BSTNode* tree_predecessor(BSTNode *node) {
    if (node == NULL) return NULL;
    
    if (node->left != NULL) {
        return tree_maximum(node->left);
    }
    
    BSTNode *parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

static void transplant(BST *bst, BSTNode *u, BSTNode *v) {
    if (u->parent == NULL) {
        bst->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != NULL) {
        v->parent = u->parent;
    }
}

static BSTNode* search_node(BSTNode *node, const void *data, CompareFn compare) {
    while (node != NULL) {
        int cmp = compare(data, node->data);
        if (cmp == 0) return node;
        if (cmp < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}

static int height_recursive(BSTNode *node) {
    if (node == NULL) return -1;
    
    int left_height = height_recursive(node->left);
    int right_height = height_recursive(node->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

static bool is_valid_bst_recursive(BSTNode *node, const void *min, const void *max, CompareFn compare) {
    if (node == NULL) return true;
    
    if (min != NULL && compare(node->data, min) < 0) return false;
    if (max != NULL && compare(node->data, max) > 0) return false;
    
    return is_valid_bst_recursive(node->left, min, node->data, compare) &&
           is_valid_bst_recursive(node->right, node->data, max, compare);
}

static bool is_balanced_recursive(BSTNode *node, int *height) {
    if (node == NULL) {
        *height = -1;
        return true;
    }
    
    int left_height, right_height;
    
    if (!is_balanced_recursive(node->left, &left_height)) return false;
    if (!is_balanced_recursive(node->right, &right_height)) return false;
    
    *height = 1 + (left_height > right_height ? left_height : right_height);
    
    int diff = left_height - right_height;
    if (diff < 0) diff = -diff;
    
    return diff <= 1;
}

static void inorder_recursive(BSTNode *node, BSTTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    inorder_recursive(node->left, callback, user_data);
    callback(node->data, user_data);
    inorder_recursive(node->right, callback, user_data);
}

static void preorder_recursive(BSTNode *node, BSTTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    callback(node->data, user_data);
    preorder_recursive(node->left, callback, user_data);
    preorder_recursive(node->right, callback, user_data);
}

static void postorder_recursive(BSTNode *node, BSTTraversalFn callback, void *user_data) {
    if (node == NULL) return;
    postorder_recursive(node->left, callback, user_data);
    postorder_recursive(node->right, callback, user_data);
    callback(node->data, user_data);
}

static BSTNode* clone_recursive(BSTNode *node, size_t element_size, CopyFn copy_fn) {
    if (node == NULL) return NULL;
    
    BSTNode *new_node = (BSTNode*)malloc(sizeof(BSTNode));
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
    
    new_node->left = clone_recursive(node->left, element_size, copy_fn);
    new_node->right = clone_recursive(node->right, element_size, copy_fn);
    new_node->parent = NULL;
    
    if (new_node->left != NULL) new_node->left->parent = new_node;
    if (new_node->right != NULL) new_node->right->parent = new_node;
    
    return new_node;
}

// ============================================================================
// FUNÇÕES PÚBLICAS
// ============================================================================

BST* bst_create(size_t element_size, CompareFn compare, DestroyFn destroy) {
    if (element_size == 0 || compare == NULL) return NULL;
    
    BST *bst = (BST*)malloc(sizeof(BST));
    if (bst == NULL) return NULL;
    
    bst->root = NULL;
    bst->element_size = element_size;
    bst->size = 0;
    bst->compare = compare;
    bst->destroy = destroy;
    
    return bst;
}

void bst_destroy(BST *bst) {
    if (bst == NULL) return;
    
    destroy_tree_recursive(bst->root, bst->destroy);
    free(bst);
}

DataStructureError bst_insert(BST *bst, const void *data) {
    if (bst == NULL || data == NULL) return DS_ERROR_NULL_POINTER;
    
    BSTNode *new_node = create_node(data, bst->element_size);
    if (new_node == NULL) return DS_ERROR_OUT_OF_MEMORY;
    
    BSTNode *parent = NULL;
    BSTNode *current = bst->root;
    
    while (current != NULL) {
        parent = current;
        int cmp = bst->compare(data, current->data);
        if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    new_node->parent = parent;
    
    if (parent == NULL) {
        bst->root = new_node;
    } else {
        int cmp = bst->compare(data, parent->data);
        if (cmp < 0) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }
    
    bst->size++;
    return DS_SUCCESS;
}

DataStructureError bst_search(const BST *bst, const void *data, void *output) {
    if (bst == NULL || data == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    
    BSTNode *node = search_node(bst->root, data, bst->compare);
    if (node == NULL) return DS_ERROR_NOT_FOUND;
    
    memcpy(output, node->data, bst->element_size);
    return DS_SUCCESS;
}

BSTNode* bst_find_node(const BST *bst, const void *data) {
    if (bst == NULL || data == NULL) return NULL;
    return search_node(bst->root, data, bst->compare);
}

bool bst_contains(const BST *bst, const void *data) {
    if (bst == NULL || data == NULL) return false;
    return search_node(bst->root, data, bst->compare) != NULL;
}

DataStructureError bst_remove(BST *bst, const void *data) {
    if (bst == NULL || data == NULL) return DS_ERROR_NULL_POINTER;
    
    BSTNode *node = search_node(bst->root, data, bst->compare);
    if (node == NULL) return DS_ERROR_NOT_FOUND;
    
    if (node->left == NULL) {
        transplant(bst, node, node->right);
    } else if (node->right == NULL) {
        transplant(bst, node, node->left);
    } else {
        BSTNode *successor = tree_minimum(node->right);
        if (successor->parent != node) {
            transplant(bst, successor, successor->right);
            successor->right = node->right;
            successor->right->parent = successor;
        }
        transplant(bst, node, successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }
    
    destroy_node(node, bst->destroy);
    bst->size--;
    return DS_SUCCESS;
}

DataStructureError bst_min(const BST *bst, void *output) {
    if (bst == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    if (bst->root == NULL) return DS_ERROR_EMPTY;
    
    BSTNode *min_node = tree_minimum(bst->root);
    memcpy(output, min_node->data, bst->element_size);
    return DS_SUCCESS;
}

DataStructureError bst_max(const BST *bst, void *output) {
    if (bst == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    if (bst->root == NULL) return DS_ERROR_EMPTY;
    
    BSTNode *max_node = tree_maximum(bst->root);
    memcpy(output, max_node->data, bst->element_size);
    return DS_SUCCESS;
}

DataStructureError bst_successor(const BST *bst, const void *data, void *output) {
    if (bst == NULL || data == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    
    BSTNode *node = search_node(bst->root, data, bst->compare);
    if (node == NULL) return DS_ERROR_NOT_FOUND;
    
    BSTNode *succ = tree_successor(node);
    if (succ == NULL) return DS_ERROR_NOT_FOUND;
    
    memcpy(output, succ->data, bst->element_size);
    return DS_SUCCESS;
}

DataStructureError bst_predecessor(const BST *bst, const void *data, void *output) {
    if (bst == NULL || data == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    
    BSTNode *node = search_node(bst->root, data, bst->compare);
    if (node == NULL) return DS_ERROR_NOT_FOUND;
    
    BSTNode *pred = tree_predecessor(node);
    if (pred == NULL) return DS_ERROR_NOT_FOUND;
    
    memcpy(output, pred->data, bst->element_size);
    return DS_SUCCESS;
}

bool bst_is_empty(const BST *bst) {
    return bst == NULL || bst->root == NULL;
}

size_t bst_size(const BST *bst) {
    return bst == NULL ? 0 : bst->size;
}

int bst_height(const BST *bst) {
    if (bst == NULL) return -1;
    return height_recursive(bst->root);
}

bool bst_is_valid(const BST *bst) {
    if (bst == NULL) return false;
    return is_valid_bst_recursive(bst->root, NULL, NULL, bst->compare);
}

bool bst_is_balanced(const BST *bst) {
    if (bst == NULL) return false;
    int height;
    return is_balanced_recursive(bst->root, &height);
}

void bst_clear(BST *bst) {
    if (bst == NULL) return;
    
    destroy_tree_recursive(bst->root, bst->destroy);
    bst->root = NULL;
    bst->size = 0;
}

void bst_inorder(const BST *bst, BSTTraversalFn callback, void *user_data) {
    if (bst == NULL || callback == NULL) return;
    inorder_recursive(bst->root, callback, user_data);
}

void bst_preorder(const BST *bst, BSTTraversalFn callback, void *user_data) {
    if (bst == NULL || callback == NULL) return;
    preorder_recursive(bst->root, callback, user_data);
}

void bst_postorder(const BST *bst, BSTTraversalFn callback, void *user_data) {
    if (bst == NULL || callback == NULL) return;
    postorder_recursive(bst->root, callback, user_data);
}

void bst_levelorder(const BST *bst, BSTTraversalFn callback, void *user_data) {
    if (bst == NULL || callback == NULL || bst->root == NULL) return;
    
    BSTNode **queue = (BSTNode**)malloc(bst->size * sizeof(BSTNode*));
    if (queue == NULL) return;
    
    size_t front = 0, rear = 0;
    queue[rear++] = bst->root;
    
    while (front < rear) {
        BSTNode *node = queue[front++];
        callback(node->data, user_data);
        
        if (node->left != NULL) queue[rear++] = node->left;
        if (node->right != NULL) queue[rear++] = node->right;
    }
    
    free(queue);
}

BST* bst_clone(const BST *bst, CopyFn copy_fn) {
    if (bst == NULL) return NULL;
    
    BST *new_bst = bst_create(bst->element_size, bst->compare, bst->destroy);
    if (new_bst == NULL) return NULL;
    
    new_bst->root = clone_recursive(bst->root, bst->element_size, copy_fn);
    new_bst->size = bst->size;
    
    return new_bst;
}

void bst_print(const BST *bst, PrintFn print) {
    if (bst == NULL || print == NULL) return;
    
    printf("BST (size=%zu, height=%d):\n", bst->size, bst_height(bst));
    printf("Inorder: ");
    bst_inorder(bst, (BSTTraversalFn)print, NULL);
    printf("\n");
}

DataStructureError bst_to_array(const BST *bst, void **array, size_t *size) {
    if (bst == NULL || array == NULL || size == NULL) return DS_ERROR_NULL_POINTER;
    if (bst->size == 0) {
        *array = NULL;
        *size = 0;
        return DS_SUCCESS;
    }
    
    void *result = malloc(bst->size * bst->element_size);
    if (result == NULL) return DS_ERROR_OUT_OF_MEMORY;
    
    size_t index = 0;
    
    void collect_inorder(void *data, void *user_data) {
        size_t *idx = (size_t*)user_data;
        memcpy((char*)result + (*idx * bst->element_size), data, bst->element_size);
        (*idx)++;
    }
    
    bst_inorder(bst, collect_inorder, &index);
    
    *array = result;
    *size = bst->size;
    return DS_SUCCESS;
}

DataStructureError bst_range_search(const BST *bst, const void *min, const void *max,
                                   void **results, size_t *count) {
    if (bst == NULL || min == NULL || max == NULL || results == NULL || count == NULL) {
        return DS_ERROR_NULL_POINTER;
    }
    
    size_t capacity = 16;
    void *array = malloc(capacity * bst->element_size);
    if (array == NULL) return DS_ERROR_OUT_OF_MEMORY;
    
    size_t result_count = 0;
    
    void collect_range(void *data, void *user_data) {
        (void)user_data;
        if (bst->compare(data, min) >= 0 && bst->compare(data, max) <= 0) {
            if (result_count >= capacity) {
                capacity *= 2;
                void *new_array = realloc(array, capacity * bst->element_size);
                if (new_array == NULL) return;
                array = new_array;
            }
            memcpy((char*)array + (result_count * bst->element_size), data, bst->element_size);
            result_count++;
        }
    }
    
    bst_inorder(bst, collect_range, NULL);
    
    *results = array;
    *count = result_count;
    return DS_SUCCESS;
}

size_t bst_range_count(const BST *bst, const void *min, const void *max) {
    if (bst == NULL || min == NULL || max == NULL) return 0;
    
    size_t count = 0;
    
    void count_range(void *data, void *user_data) {
        if (bst->compare(data, min) >= 0 && bst->compare(data, max) <= 0) {
            (*(size_t*)user_data)++;
        }
    }
    
    bst_inorder(bst, count_range, &count);
    return count;
}

DataStructureError bst_select(const BST *bst, size_t k, void *output) {
    if (bst == NULL || output == NULL) return DS_ERROR_NULL_POINTER;
    if (k == 0 || k > bst->size) return DS_ERROR_INVALID_INDEX;
    
    size_t index = 0;
    DataStructureError result = DS_ERROR_NOT_FOUND;
    
    void select_kth(void *data, void *user_data) {
        size_t *idx = (size_t*)user_data;
        (*idx)++;
        if (*idx == k) {
            memcpy(output, data, bst->element_size);
            result = DS_SUCCESS;
        }
    }
    
    bst_inorder(bst, select_kth, &index);
    return result;
}

size_t bst_rank(const BST *bst, const void *data) {
    if (bst == NULL || data == NULL) return 0;
    
    size_t rank = 0;
    bool found = false;
    
    void count_smaller(void *node_data, void *user_data) {
        if (found) return;
        
        if (bst->compare(node_data, data) < 0) {
            (*(size_t*)user_data)++;
        } else if (bst->compare(node_data, data) == 0) {
            found = true;
        }
    }
    
    bst_inorder(bst, count_smaller, &rank);
    return rank;
}

BST* bst_from_sorted_array(size_t element_size, const void *array, size_t size,
                           CompareFn compare, DestroyFn destroy) {
    if (array == NULL || size == 0 || compare == NULL) return NULL;
    
    BST *bst = bst_create(element_size, compare, destroy);
    if (bst == NULL) return NULL;
    
    BSTNode* build_balanced(size_t left, size_t right, BSTNode *parent) {
        if (left > right) return NULL;
        
        size_t mid = left + (right - left) / 2;
        const void *mid_data = (const char*)array + (mid * element_size);
        
        BSTNode *node = create_node(mid_data, element_size);
        if (node == NULL) return NULL;
        
        node->parent = parent;
        node->left = build_balanced(left, mid - 1, node);
        node->right = build_balanced(mid + 1, right, node);
        
        return node;
    }
    
    bst->root = build_balanced(0, size - 1, NULL);
    bst->size = size;
    
    return bst;
}

DataStructureError bst_balance(BST *bst) {
    if (bst == NULL) return DS_ERROR_NULL_POINTER;
    if (bst->size == 0) return DS_SUCCESS;
    
    void *array;
    size_t size;
    DataStructureError err = bst_to_array(bst, &array, &size);
    if (err != DS_SUCCESS) return err;
    
    bst_clear(bst);
    
    BST *balanced = bst_from_sorted_array(bst->element_size, array, size,
                                          bst->compare, bst->destroy);
    free(array);
    
    if (balanced == NULL) return DS_ERROR_OUT_OF_MEMORY;
    
    bst->root = balanced->root;
    bst->size = balanced->size;
    free(balanced);
    
    return DS_SUCCESS;
}
