/**
 * @file test_bst.c
 * @brief Testes unitários para Binary Search Tree (BST)
 *
 * Testa todas as operações da BST: insert, search, remove,
 * min/max, successor/predecessor, travessias, validação.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/bst.h"
#include "data_structures/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// MACROS DE TESTE
// ============================================================================

#define TEST(name) \
    void test_##name(); \
    printf("  Running: %-50s", #name); \
    test_##name(); \
    printf("[✓ PASS]\n"); \
    void test_##name()

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("\n    Assertion failed: %s\n", #condition); \
            printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NE(a, b) ASSERT((a) != (b))
#define ASSERT_TRUE(x) ASSERT(x)
#define ASSERT_FALSE(x) ASSERT(!(x))
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)

// ============================================================================
// TESTES
// ============================================================================

TEST(create_destroy) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    ASSERT_NOT_NULL(bst);
    ASSERT_TRUE(bst_is_empty(bst));
    ASSERT_EQ(bst_size(bst), 0);
    bst_destroy(bst);
}

TEST(insert_single) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int val = 50;
    
    ASSERT_EQ(bst_insert(bst, &val), DS_SUCCESS);
    ASSERT_FALSE(bst_is_empty(bst));
    ASSERT_EQ(bst_size(bst), 1);
    
    bst_destroy(bst);
}

TEST(insert_multiple) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        ASSERT_EQ(bst_insert(bst, &values[i]), DS_SUCCESS);
    }
    
    ASSERT_EQ(bst_size(bst), 7);
    
    bst_destroy(bst);
}

TEST(search_existing) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};
    
    for (size_t i = 0; i < 5; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int output;
    ASSERT_EQ(bst_search(bst, &values[2], &output), DS_SUCCESS);
    ASSERT_EQ(output, 70);
    
    bst_destroy(bst);
}

TEST(search_not_found) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70};
    
    for (size_t i = 0; i < 3; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int search_val = 100;
    int output;
    ASSERT_EQ(bst_search(bst, &search_val, &output), DS_ERROR_NOT_FOUND);
    
    bst_destroy(bst);
}

TEST(contains) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70};
    
    for (size_t i = 0; i < 3; i++) {
        bst_insert(bst, &values[i]);
    }
    
    ASSERT_TRUE(bst_contains(bst, &values[0]));
    ASSERT_TRUE(bst_contains(bst, &values[1]));
    
    int not_present = 100;
    ASSERT_FALSE(bst_contains(bst, &not_present));
    
    bst_destroy(bst);
}

TEST(remove_leaf) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20};
    
    for (size_t i = 0; i < 4; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int leaf = 20;
    ASSERT_EQ(bst_remove(bst, &leaf), DS_SUCCESS);
    ASSERT_EQ(bst_size(bst), 3);
    ASSERT_FALSE(bst_contains(bst, &leaf));
    
    bst_destroy(bst);
}

TEST(remove_one_child) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20};
    
    for (size_t i = 0; i < 4; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int node = 30;
    ASSERT_EQ(bst_remove(bst, &node), DS_SUCCESS);
    ASSERT_EQ(bst_size(bst), 3);
    ASSERT_FALSE(bst_contains(bst, &node));
    
    bst_destroy(bst);
}

TEST(remove_two_children) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int root = 50;
    ASSERT_EQ(bst_remove(bst, &root), DS_SUCCESS);
    ASSERT_EQ(bst_size(bst), 6);
    ASSERT_FALSE(bst_contains(bst, &root));
    
    bst_destroy(bst);
}

TEST(min_max) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int min, max;
    ASSERT_EQ(bst_min(bst, &min), DS_SUCCESS);
    ASSERT_EQ(min, 20);
    
    ASSERT_EQ(bst_max(bst, &max), DS_SUCCESS);
    ASSERT_EQ(max, 80);
    
    bst_destroy(bst);
}

TEST(successor) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};
    
    for (size_t i = 0; i < 5; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int val = 30, succ;
    ASSERT_EQ(bst_successor(bst, &val, &succ), DS_SUCCESS);
    ASSERT_EQ(succ, 40);
    
    bst_destroy(bst);
}

TEST(predecessor) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};
    
    for (size_t i = 0; i < 5; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int val = 50, pred;
    ASSERT_EQ(bst_predecessor(bst, &val, &pred), DS_SUCCESS);
    ASSERT_EQ(pred, 40);
    
    bst_destroy(bst);
}

TEST(is_valid_bst) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        bst_insert(bst, &values[i]);
    }
    
    ASSERT_TRUE(bst_is_valid(bst));
    
    bst_destroy(bst);
}

TEST(height) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    
    ASSERT_EQ(bst_height(bst), -1);
    
    int val = 50;
    bst_insert(bst, &val);
    ASSERT_EQ(bst_height(bst), 0);
    
    int val2 = 30;
    bst_insert(bst, &val2);
    ASSERT_EQ(bst_height(bst), 1);
    
    bst_destroy(bst);
}

TEST(inorder_traversal) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int expected[] = {20, 30, 40, 50, 60, 70, 80};
    int index = 0;
    
    void check_order(void *data, void *user_data) {
        int *idx = (int*)user_data;
        int val = *(int*)data;
        ASSERT_EQ(val, expected[*idx]);
        (*idx)++;
    }
    
    bst_inorder(bst, check_order, &index);
    ASSERT_EQ(index, 7);
    
    bst_destroy(bst);
}

TEST(clear) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70};
    
    for (size_t i = 0; i < 3; i++) {
        bst_insert(bst, &values[i]);
    }
    
    bst_clear(bst);
    ASSERT_TRUE(bst_is_empty(bst));
    ASSERT_EQ(bst_size(bst), 0);
    
    bst_destroy(bst);
}

TEST(to_array) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};
    
    for (size_t i = 0; i < 5; i++) {
        bst_insert(bst, &values[i]);
    }
    
    void *array;
    size_t size;
    ASSERT_EQ(bst_to_array(bst, &array, &size), DS_SUCCESS);
    ASSERT_EQ(size, 5);
    
    int *int_array = (int*)array;
    int expected[] = {20, 30, 40, 50, 70};
    for (size_t i = 0; i < size; i++) {
        ASSERT_EQ(int_array[i], expected[i]);
    }
    
    free(array);
    bst_destroy(bst);
}

TEST(range_count) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (size_t i = 0; i < 7; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int min = 30, max = 70;
    size_t count = bst_range_count(bst, &min, &max);
    ASSERT_EQ(count, 5);
    
    bst_destroy(bst);
}

TEST(select_kth) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};
    
    for (size_t i = 0; i < 5; i++) {
        bst_insert(bst, &values[i]);
    }
    
    int output;
    ASSERT_EQ(bst_select(bst, 3, &output), DS_SUCCESS);
    ASSERT_EQ(output, 40);
    
    bst_destroy(bst);
}

TEST(clone) {
    BST *bst = bst_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70};
    
    for (size_t i = 0; i < 3; i++) {
        bst_insert(bst, &values[i]);
    }
    
    BST *clone = bst_clone(bst, NULL);
    ASSERT_NOT_NULL(clone);
    ASSERT_EQ(bst_size(clone), bst_size(bst));
    
    for (size_t i = 0; i < 3; i++) {
        ASSERT_TRUE(bst_contains(clone, &values[i]));
    }
    
    bst_destroy(bst);
    bst_destroy(clone);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║            BST (Binary Search Tree) - Tests                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    TEST(create_destroy);
    TEST(insert_single);
    TEST(insert_multiple);
    TEST(search_existing);
    TEST(search_not_found);
    TEST(contains);
    TEST(remove_leaf);
    TEST(remove_one_child);
    TEST(remove_two_children);
    TEST(min_max);
    TEST(successor);
    TEST(predecessor);
    TEST(is_valid_bst);
    TEST(height);
    TEST(inorder_traversal);
    TEST(clear);
    TEST(to_array);
    TEST(range_count);
    TEST(select_kth);
    TEST(clone);

    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ✓ ALL TESTS PASSED                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    return 0;
}
