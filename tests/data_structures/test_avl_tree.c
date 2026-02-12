/**
 * @file test_avl_tree.c
 * @brief Testes unitários para Árvore AVL
 *
 * Testa todas as operações da AVL Tree: insert, search, remove,
 * min/max, travessias, validação, rotações, range search, clone.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/avl_tree.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>
#include <math.h>

// ============================================================================
// TESTES
// ============================================================================

TEST(create_destroy) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    ASSERT_NOT_NULL(tree);
    ASSERT_TRUE(avl_is_empty(tree));
    ASSERT_EQ(avl_size(tree), 0);
    ASSERT_EQ(avl_height(tree), -1);
    avl_destroy(tree);
}

TEST(insert_single) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int val = 42;

    ASSERT_EQ(avl_insert(tree, &val), DS_SUCCESS);
    ASSERT_FALSE(avl_is_empty(tree));
    ASSERT_EQ(avl_size(tree), 1);
    ASSERT_EQ(avl_height(tree), 0);
    ASSERT_TRUE(avl_is_valid(tree));

    avl_destroy(tree);
}

TEST(insert_multiple) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        ASSERT_EQ(avl_insert(tree, &values[i]), DS_SUCCESS);
    }

    ASSERT_EQ(avl_size(tree), 7);
    ASSERT_TRUE(avl_is_valid(tree));

    for (size_t i = 0; i < 7; i++) {
        ASSERT_TRUE(avl_contains(tree, &values[i]));
    }

    avl_destroy(tree);
}

TEST(insert_triggers_rotations) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);

    /* Inserir 1, 2, 3 em sequência deve triggerar rotação esquerda (RR) */
    int v1 = 1, v2 = 2, v3 = 3;
    avl_insert(tree, &v1);
    avl_insert(tree, &v2);
    avl_insert(tree, &v3);

    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(avl_height(tree), 1);

    avl_destroy(tree);

    /* LL rotation: inserir 3, 2, 1 */
    tree = avl_create(sizeof(int), compare_int, NULL);
    v1 = 3; v2 = 2; v3 = 1;
    avl_insert(tree, &v1);
    avl_insert(tree, &v2);
    avl_insert(tree, &v3);

    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(avl_height(tree), 1);

    avl_destroy(tree);

    /* LR rotation: inserir 3, 1, 2 */
    tree = avl_create(sizeof(int), compare_int, NULL);
    v1 = 3; v2 = 1; v3 = 2;
    avl_insert(tree, &v1);
    avl_insert(tree, &v2);
    avl_insert(tree, &v3);

    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(avl_height(tree), 1);

    avl_destroy(tree);

    /* RL rotation: inserir 1, 3, 2 */
    tree = avl_create(sizeof(int), compare_int, NULL);
    v1 = 1; v2 = 3; v3 = 2;
    avl_insert(tree, &v1);
    avl_insert(tree, &v2);
    avl_insert(tree, &v3);

    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(avl_height(tree), 1);

    avl_destroy(tree);
}

TEST(search_existing) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};

    for (size_t i = 0; i < 5; i++) {
        avl_insert(tree, &values[i]);
    }

    int output;
    ASSERT_EQ(avl_search(tree, &values[2], &output), DS_SUCCESS);
    ASSERT_EQ(output, 70);

    ASSERT_EQ(avl_search(tree, &values[0], &output), DS_SUCCESS);
    ASSERT_EQ(output, 50);

    avl_destroy(tree);
}

TEST(search_not_found) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70};

    for (size_t i = 0; i < 3; i++) {
        avl_insert(tree, &values[i]);
    }

    int search_val = 100;
    int output;
    ASSERT_EQ(avl_search(tree, &search_val, &output), DS_ERROR_NOT_FOUND);

    avl_destroy(tree);
}

TEST(remove_leaf) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    int leaf = 20;
    ASSERT_EQ(avl_remove(tree, &leaf), DS_SUCCESS);
    ASSERT_EQ(avl_size(tree), 6);
    ASSERT_FALSE(avl_contains(tree, &leaf));
    ASSERT_TRUE(avl_is_valid(tree));

    avl_destroy(tree);
}

TEST(remove_internal) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    int internal = 30;
    ASSERT_EQ(avl_remove(tree, &internal), DS_SUCCESS);
    ASSERT_EQ(avl_size(tree), 6);
    ASSERT_FALSE(avl_contains(tree, &internal));
    ASSERT_TRUE(avl_is_valid(tree));

    avl_destroy(tree);
}

TEST(remove_root) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    int root = 50;
    ASSERT_EQ(avl_remove(tree, &root), DS_SUCCESS);
    ASSERT_EQ(avl_size(tree), 6);
    ASSERT_FALSE(avl_contains(tree, &root));
    ASSERT_TRUE(avl_is_valid(tree));

    for (size_t i = 1; i < 7; i++) {
        ASSERT_TRUE(avl_contains(tree, &values[i]));
    }

    avl_destroy(tree);
}

TEST(min_max) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);

    int output;
    ASSERT_EQ(avl_min(tree, &output), DS_ERROR_EMPTY);
    ASSERT_EQ(avl_max(tree, &output), DS_ERROR_EMPTY);

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    int min_val, max_val;
    ASSERT_EQ(avl_min(tree, &min_val), DS_SUCCESS);
    ASSERT_EQ(min_val, 20);

    ASSERT_EQ(avl_max(tree, &max_val), DS_SUCCESS);
    ASSERT_EQ(max_val, 80);

    avl_destroy(tree);
}

typedef struct {
    int *array;
    int count;
} TraversalCtx;

static void collect_inorder(void *data, void *user_data) {
    TraversalCtx *ctx = (TraversalCtx*)user_data;
    ctx->array[ctx->count++] = *(int*)data;
}

TEST(inorder_traversal) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    int result[7];
    TraversalCtx ctx = { result, 0 };
    avl_inorder(tree, collect_inorder, &ctx);

    ASSERT_EQ(ctx.count, 7);

    for (int i = 0; i < ctx.count - 1; i++) {
        ASSERT_TRUE(result[i] < result[i + 1]);
    }

    int expected[] = {20, 30, 40, 50, 60, 70, 80};
    for (int i = 0; i < 7; i++) {
        ASSERT_EQ(result[i], expected[i]);
    }

    avl_destroy(tree);
}

TEST(height_balanced) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);

    for (int i = 1; i <= 31; i++) {
        avl_insert(tree, &i);
    }

    ASSERT_TRUE(avl_is_valid(tree));

    int h = avl_height(tree);
    double max_avl_height = 1.44 * log2(31 + 2) - 0.328;
    ASSERT_TRUE(h <= (int)max_avl_height + 1);

    avl_destroy(tree);
}

TEST(is_valid_after_operations) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);

    for (int i = 0; i < 20; i++) {
        avl_insert(tree, &i);
        ASSERT_TRUE(avl_is_valid(tree));
    }

    for (int i = 0; i < 20; i += 2) {
        avl_remove(tree, &i);
        ASSERT_TRUE(avl_is_valid(tree));
    }

    ASSERT_EQ(avl_size(tree), 10);

    avl_destroy(tree);
}

TEST(range_search) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

    for (size_t i = 0; i < 9; i++) {
        avl_insert(tree, &values[i]);
    }

    int min = 30, max = 70;
    void *results = NULL;
    size_t count = 0;
    ASSERT_EQ(avl_range_search(tree, &min, &max, &results, &count), DS_SUCCESS);
    ASSERT_EQ(count, 5);

    int *int_results = (int*)results;
    for (size_t i = 0; i < count; i++) {
        ASSERT_TRUE(int_results[i] >= 30 && int_results[i] <= 70);
    }

    free(results);
    avl_destroy(tree);
}

TEST(clear) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40};

    for (size_t i = 0; i < 5; i++) {
        avl_insert(tree, &values[i]);
    }

    avl_clear(tree);
    ASSERT_TRUE(avl_is_empty(tree));
    ASSERT_EQ(avl_size(tree), 0);
    ASSERT_EQ(avl_height(tree), -1);

    int val = 10;
    ASSERT_EQ(avl_insert(tree, &val), DS_SUCCESS);
    ASSERT_EQ(avl_size(tree), 1);

    avl_destroy(tree);
}

TEST(clone) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < 7; i++) {
        avl_insert(tree, &values[i]);
    }

    AVLTree *cloned = avl_clone(tree, NULL);
    ASSERT_NOT_NULL(cloned);
    ASSERT_EQ(avl_size(cloned), avl_size(tree));
    ASSERT_EQ(avl_height(cloned), avl_height(tree));
    ASSERT_TRUE(avl_is_valid(cloned));

    for (size_t i = 0; i < 7; i++) {
        ASSERT_TRUE(avl_contains(cloned, &values[i]));
    }

    int remove_val = 50;
    avl_remove(tree, &remove_val);
    ASSERT_TRUE(avl_contains(cloned, &remove_val));

    avl_destroy(tree);
    avl_destroy(cloned);
}

TEST(stress_test) {
    AVLTree *tree = avl_create(sizeof(int), compare_int, NULL);

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(avl_insert(tree, &i), DS_SUCCESS);
    }

    ASSERT_EQ(avl_size(tree), 100);
    ASSERT_TRUE(avl_is_valid(tree));

    int h = avl_height(tree);
    double max_avl_height = 1.44 * log2(100 + 2);
    ASSERT_TRUE(h <= (int)max_avl_height + 1);

    for (int i = 0; i < 100; i++) {
        ASSERT_TRUE(avl_contains(tree, &i));
    }

    for (int i = 0; i < 50; i++) {
        ASSERT_EQ(avl_remove(tree, &i), DS_SUCCESS);
        ASSERT_TRUE(avl_is_valid(tree));
    }

    ASSERT_EQ(avl_size(tree), 50);

    for (int i = 50; i < 100; i++) {
        ASSERT_TRUE(avl_contains(tree, &i));
    }

    avl_destroy(tree);
}

TEST(null_pointer_checks) {
    ASSERT_NULL(avl_create(0, compare_int, NULL));
    ASSERT_NULL(avl_create(sizeof(int), NULL, NULL));

    ASSERT_EQ(avl_insert(NULL, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(avl_search(NULL, NULL, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(avl_remove(NULL, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(avl_min(NULL, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(avl_max(NULL, NULL), DS_ERROR_NULL_POINTER);

    ASSERT_FALSE(avl_contains(NULL, NULL));
    ASSERT_TRUE(avl_is_empty(NULL));
    ASSERT_EQ(avl_size(NULL), 0);
    ASSERT_EQ(avl_height(NULL), -1);
    ASSERT_FALSE(avl_is_valid(NULL));
    ASSERT_NULL(avl_clone(NULL, NULL));

    avl_destroy(NULL);
    avl_clear(NULL);
    avl_inorder(NULL, NULL, NULL);
    avl_preorder(NULL, NULL, NULL);
    avl_postorder(NULL, NULL, NULL);
    avl_print(NULL, NULL);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("==============================================================\n");
    printf("            AVL Tree (Adelson-Velsky & Landis) - Tests         \n");
    printf("==============================================================\n");
    printf("\n");

    RUN_TEST(create_destroy);
    RUN_TEST(insert_single);
    RUN_TEST(insert_multiple);
    RUN_TEST(insert_triggers_rotations);
    RUN_TEST(search_existing);
    RUN_TEST(search_not_found);
    RUN_TEST(remove_leaf);
    RUN_TEST(remove_internal);
    RUN_TEST(remove_root);
    RUN_TEST(min_max);
    RUN_TEST(inorder_traversal);
    RUN_TEST(height_balanced);
    RUN_TEST(is_valid_after_operations);
    RUN_TEST(range_search);
    RUN_TEST(clear);
    RUN_TEST(clone);
    RUN_TEST(stress_test);
    RUN_TEST(null_pointer_checks);

    printf("\n============================================\n");
    printf("  TODOS OS TESTES PASSARAM! (18 testes)\n");
    printf("============================================\n\n");

    return 0;
}
