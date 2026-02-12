/**
 * @file test_binary_tree.c
 * @brief Testes para Binary Tree
 *
 * Valida:
 * - Criação e manipulação de nós
 * - Travessias: inorder, preorder, postorder, levelorder
 * - Propriedades: altura, tamanho, folhas, completa, cheia, perfeita
 * - Operações avançadas: busca, remoção, clone, LCA, diâmetro
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/binary_tree.h"
#include "data_structures/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// MACROS DE TESTE
// ============================================================================

#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
    printf("  - " #name "... "); \
    fflush(stdout); \
    test_##name(); \
    printf("✓\n"); \
} while(0)

#define ASSERT_TRUE(expr) assert(expr)
#define ASSERT_FALSE(expr) assert(!(expr))
#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_NE(a, b) assert((a) != (b))
#define ASSERT_NULL(ptr) assert((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) assert((ptr) != NULL)

// ============================================================================
// HELPERS
// ============================================================================

// Contador para travessias
static int traversal_count;
static int traversal_values[100];

void count_callback(void *data, void *user_data) {
    (void)user_data;
    traversal_values[traversal_count++] = *(int*)data;
}

// ============================================================================
// TESTES: CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

TEST(create_destroy) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);
    ASSERT_NOT_NULL(tree);
    ASSERT_TRUE(btree_is_empty(tree));
    ASSERT_EQ(btree_size(tree), 0);
    ASSERT_EQ(btree_height(tree), -1);
    btree_destroy(tree);
}

TEST(create_node) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int value = 42;
    TreeNode *node = btree_create_node(tree, &value);
    ASSERT_NOT_NULL(node);

    int *data = (int*)btree_node_data(node);
    ASSERT_EQ(*data, 42);

    btree_destroy(tree);
}

// ============================================================================
// TESTES: CONSTRUÇÃO MANUAL DE ÁRVORE
// ============================================================================

TEST(build_simple_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     */

    int v10 = 10, v5 = 5, v15 = 15;

    TreeNode *root = btree_create_node(tree, &v10);
    TreeNode *left = btree_create_node(tree, &v5);
    TreeNode *right = btree_create_node(tree, &v15);

    btree_set_root(tree, root);
    btree_set_left(tree, root, left);
    btree_set_right(tree, root, right);

    ASSERT_EQ(btree_size(tree), 3);
    ASSERT_EQ(btree_height(tree), 1);

    ASSERT_EQ(btree_left(root), left);
    ASSERT_EQ(btree_right(root), right);
    ASSERT_EQ(btree_parent(left), root);
    ASSERT_EQ(btree_parent(right), root);

    btree_destroy(tree);
}

// ============================================================================
// TESTES: TRAVESSIAS
// ============================================================================

TEST(inorder_traversal) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // Inorder: 3, 5, 7, 10, 15
    traversal_count = 0;
    btree_inorder(tree, count_callback, NULL);

    ASSERT_EQ(traversal_count, 5);
    ASSERT_EQ(traversal_values[0], 3);
    ASSERT_EQ(traversal_values[1], 5);
    ASSERT_EQ(traversal_values[2], 7);
    ASSERT_EQ(traversal_values[3], 10);
    ASSERT_EQ(traversal_values[4], 15);

    btree_destroy(tree);
}

TEST(preorder_traversal) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     */

    int v10 = 10, v5 = 5, v15 = 15;
    TreeNode *root = btree_create_node(tree, &v10);
    TreeNode *left = btree_create_node(tree, &v5);
    TreeNode *right = btree_create_node(tree, &v15);

    btree_set_root(tree, root);
    btree_set_left(tree, root, left);
    btree_set_right(tree, root, right);

    // Preorder: 10, 5, 15
    traversal_count = 0;
    btree_preorder(tree, count_callback, NULL);

    ASSERT_EQ(traversal_count, 3);
    ASSERT_EQ(traversal_values[0], 10);
    ASSERT_EQ(traversal_values[1], 5);
    ASSERT_EQ(traversal_values[2], 15);

    btree_destroy(tree);
}

TEST(postorder_traversal) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     */

    int v10 = 10, v5 = 5, v15 = 15;
    TreeNode *root = btree_create_node(tree, &v10);
    TreeNode *left = btree_create_node(tree, &v5);
    TreeNode *right = btree_create_node(tree, &v15);

    btree_set_root(tree, root);
    btree_set_left(tree, root, left);
    btree_set_right(tree, root, right);

    // Postorder: 5, 15, 10
    traversal_count = 0;
    btree_postorder(tree, count_callback, NULL);

    ASSERT_EQ(traversal_count, 3);
    ASSERT_EQ(traversal_values[0], 5);
    ASSERT_EQ(traversal_values[1], 15);
    ASSERT_EQ(traversal_values[2], 10);

    btree_destroy(tree);
}

TEST(levelorder_traversal) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // Levelorder: 10, 5, 15, 3, 7
    traversal_count = 0;
    btree_levelorder(tree, count_callback, NULL);

    ASSERT_EQ(traversal_count, 5);
    ASSERT_EQ(traversal_values[0], 10);
    ASSERT_EQ(traversal_values[1], 5);
    ASSERT_EQ(traversal_values[2], 15);
    ASSERT_EQ(traversal_values[3], 3);
    ASSERT_EQ(traversal_values[4], 7);

    btree_destroy(tree);
}

// ============================================================================
// TESTES: PROPRIEDADES
// ============================================================================

TEST(height_calculation) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    // Árvore vazia: altura -1
    ASSERT_EQ(btree_height(tree), -1);

    // Apenas raiz: altura 0
    int v10 = 10;
    TreeNode *root = btree_create_node(tree, &v10);
    btree_set_root(tree, root);
    ASSERT_EQ(btree_height(tree), 0);

    // Com filhos: altura 1
    int v5 = 5, v15 = 15;
    TreeNode *left = btree_create_node(tree, &v5);
    TreeNode *right = btree_create_node(tree, &v15);
    btree_set_left(tree, root, left);
    btree_set_right(tree, root, right);
    ASSERT_EQ(btree_height(tree), 1);

    // Com neto: altura 2
    int v3 = 3;
    TreeNode *leftleft = btree_create_node(tree, &v3);
    btree_set_left(tree, left, leftleft);
    ASSERT_EQ(btree_height(tree), 2);

    btree_destroy(tree);
}

TEST(leaf_count) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // Folhas: 3, 7, 15
    ASSERT_EQ(btree_leaf_count(tree), 3);

    btree_destroy(tree);
}

TEST(is_leaf_check) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int v10 = 10, v5 = 5;
    TreeNode *root = btree_create_node(tree, &v10);
    TreeNode *left = btree_create_node(tree, &v5);

    btree_set_root(tree, root);
    btree_set_left(tree, root, left);

    ASSERT_FALSE(btree_is_leaf(root));   // Tem filho
    ASSERT_TRUE(btree_is_leaf(left));     // É folha

    btree_destroy(tree);
}

TEST(is_complete_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     * Árvore completa:
     *      10
     *     /  \
     *    5    15
     *   /
     *  3
     */

    int values[] = {10, 5, 15, 3};
    TreeNode *nodes[4];

    for (int i = 0; i < 4; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);

    ASSERT_TRUE(btree_is_complete(tree));

    btree_destroy(tree);
}

TEST(is_full_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     * Árvore cheia:
     *      10
     *     /  \
     *    5    15
     */

    int values[] = {10, 5, 15};
    TreeNode *nodes[3];

    for (int i = 0; i < 3; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);

    ASSERT_TRUE(btree_is_full(tree));

    btree_destroy(tree);
}

TEST(is_perfect_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     * Árvore perfeita:
     *      10
     *     /  \
     *    5    15
     *   / \   / \
     *  3  7  12 20
     */

    int values[] = {10, 5, 15, 3, 7, 12, 20};
    TreeNode *nodes[7];

    for (int i = 0; i < 7; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);
    btree_set_left(tree, nodes[2], nodes[5]);
    btree_set_right(tree, nodes[2], nodes[6]);

    ASSERT_TRUE(btree_is_perfect(tree));

    btree_destroy(tree);
}

// ============================================================================
// TESTES: BUSCA
// ============================================================================

TEST(find_element) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    int target = 7;
    TreeNode *found = btree_find(tree, &target);
    ASSERT_NOT_NULL(found);
    ASSERT_EQ(*(int*)btree_node_data(found), 7);

    int not_found = 99;
    TreeNode *result = btree_find(tree, &not_found);
    ASSERT_NULL(result);

    btree_destroy(tree);
}

// ============================================================================
// TESTES: CLEAR E REMOVE
// ============================================================================

TEST(clear_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int values[] = {10, 5, 15};
    TreeNode *nodes[3];

    for (int i = 0; i < 3; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);

    ASSERT_EQ(btree_size(tree), 3);

    btree_clear(tree);

    ASSERT_TRUE(btree_is_empty(tree));
    ASSERT_EQ(btree_size(tree), 0);

    btree_destroy(tree);
}

TEST(remove_leaf_node) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int values[] = {10, 5, 15};
    TreeNode *nodes[3];

    for (int i = 0; i < 3; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);

    // Remover folha
    DataStructureError err = btree_remove_node(tree, nodes[1]);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(btree_size(tree), 2);

    btree_destroy(tree);
}

// ============================================================================
// TESTES: CLONE
// ============================================================================

TEST(clone_tree) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int values[] = {10, 5, 15};
    TreeNode *nodes[3];

    for (int i = 0; i < 3; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);

    BinaryTree *clone = btree_clone(tree, NULL);
    ASSERT_NOT_NULL(clone);
    ASSERT_EQ(btree_size(clone), btree_size(tree));
    ASSERT_EQ(btree_height(clone), btree_height(tree));

    // Verificar que dados foram copiados
    TreeNode *clone_root = btree_root(clone);
    ASSERT_EQ(*(int*)btree_node_data(clone_root), 10);

    btree_destroy(tree);
    btree_destroy(clone);
}

// ============================================================================
// TESTES: TO_ARRAY
// ============================================================================

TEST(to_array_inorder) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    int values[] = {10, 5, 15};
    TreeNode *nodes[3];

    for (int i = 0; i < 3; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);

    void *array;
    size_t size;
    DataStructureError err = btree_to_array(tree, &array, &size, true);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(size, 3);

    // Inorder: 5, 10, 15
    int *arr = (int*)array;
    ASSERT_EQ(arr[0], 5);
    ASSERT_EQ(arr[1], 10);
    ASSERT_EQ(arr[2], 15);

    free(array);
    btree_destroy(tree);
}

// ============================================================================
// TESTES: LCA, DISTANCE, DIAMETER
// ============================================================================

TEST(lowest_common_ancestor) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // LCA de 3 e 7 = 5
    TreeNode *lca = btree_lca(tree, nodes[3], nodes[4]);
    ASSERT_EQ(lca, nodes[1]);

    // LCA de 3 e 15 = 10
    lca = btree_lca(tree, nodes[3], nodes[2]);
    ASSERT_EQ(lca, nodes[0]);

    btree_destroy(tree);
}

TEST(node_distance) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // Distância entre 3 e 7 = 2
    int dist = btree_distance(tree, nodes[3], nodes[4]);
    ASSERT_EQ(dist, 2);

    // Distância entre 3 e 15 = 3
    dist = btree_distance(tree, nodes[3], nodes[2]);
    ASSERT_EQ(dist, 3);

    btree_destroy(tree);
}

TEST(tree_diameter) {
    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    // Diâmetro = 3 (caminho: 3 -> 5 -> 7 ou similar)
    int diam = btree_diameter(tree);
    ASSERT_EQ(diam, 3);

    btree_destroy(tree);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(print_visual) {
    printf("\n");

    BinaryTree *tree = btree_create(sizeof(int), compare_int, NULL);

    /**
     *      10
     *     /  \
     *    5    15
     *   / \
     *  3   7
     */

    int values[] = {10, 5, 15, 3, 7};
    TreeNode *nodes[5];

    for (int i = 0; i < 5; i++) {
        nodes[i] = btree_create_node(tree, &values[i]);
    }

    btree_set_root(tree, nodes[0]);
    btree_set_left(tree, nodes[0], nodes[1]);
    btree_set_right(tree, nodes[0], nodes[2]);
    btree_set_left(tree, nodes[1], nodes[3]);
    btree_set_right(tree, nodes[1], nodes[4]);

    printf("    ");
    btree_print(tree, print_int);

    btree_destroy(tree);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("============================================\n");
    printf("  TESTES: BINARY TREE\n");
    printf("============================================\n\n");

    printf("Criação e Destruição:\n");
    RUN_TEST(create_destroy);
    RUN_TEST(create_node);

    printf("\nConstrução Manual:\n");
    RUN_TEST(build_simple_tree);

    printf("\nTravessias:\n");
    RUN_TEST(inorder_traversal);
    RUN_TEST(preorder_traversal);
    RUN_TEST(postorder_traversal);
    RUN_TEST(levelorder_traversal);

    printf("\nPropriedades:\n");
    RUN_TEST(height_calculation);
    RUN_TEST(leaf_count);
    RUN_TEST(is_leaf_check);
    RUN_TEST(is_complete_tree);
    RUN_TEST(is_full_tree);
    RUN_TEST(is_perfect_tree);

    printf("\nBusca:\n");
    RUN_TEST(find_element);

    printf("\nClear e Remove:\n");
    RUN_TEST(clear_tree);
    RUN_TEST(remove_leaf_node);

    printf("\nClone:\n");
    RUN_TEST(clone_tree);

    printf("\nTo Array:\n");
    RUN_TEST(to_array_inorder);

    printf("\nLCA, Distance, Diameter:\n");
    RUN_TEST(lowest_common_ancestor);
    RUN_TEST(node_distance);
    RUN_TEST(tree_diameter);

    printf("\nTeste Visual:\n");
    RUN_TEST(print_visual);

    printf("\n============================================\n");
    printf("  ✅ TODOS OS TESTES PASSARAM! (24 testes)\n");
    printf("============================================\n\n");

    return 0;
}
