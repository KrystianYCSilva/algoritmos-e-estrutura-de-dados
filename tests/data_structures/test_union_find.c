/**
 * @file test_union_find.c
 * @brief Testes unitarios para Union-Find (Disjoint Set)
 *
 * Testa todas as operacoes: create, find, union, connected,
 * count, size, get_members, get_components, reset.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "data_structures/union_find.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// TESTES DE CRIACAO E DESTRUICAO
// ============================================================================

TEST(create_destroy) {
    UnionFind *uf = uf_create(10);
    ASSERT_NOT_NULL(uf);
    ASSERT_EQ(uf_count(uf), 10);
    uf_destroy(uf);
}

TEST(create_zero) {
    UnionFind *uf = uf_create(0);
    ASSERT_NULL(uf);
}

// ============================================================================
// TESTES DE ESTADO INICIAL
// ============================================================================

TEST(initially_disconnected) {
    UnionFind *uf = uf_create(5);

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = i + 1; j < 5; j++) {
            ASSERT_FALSE(uf_connected(uf, i, j));
        }
    }

    ASSERT_EQ(uf_count(uf), 5);

    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(uf_find(uf, i), i);
        ASSERT_EQ(uf_size(uf, i), 1);
    }

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE UNION
// ============================================================================

TEST(union_two) {
    UnionFind *uf = uf_create(5);

    ASSERT_TRUE(uf_union(uf, 0, 1));
    ASSERT_TRUE(uf_connected(uf, 0, 1));
    ASSERT_FALSE(uf_connected(uf, 0, 2));
    ASSERT_EQ(uf_count(uf), 4);
    ASSERT_EQ(uf_size(uf, 0), 2);
    ASSERT_EQ(uf_size(uf, 1), 2);

    ASSERT_FALSE(uf_union(uf, 0, 1));
    ASSERT_EQ(uf_count(uf), 4);

    uf_destroy(uf);
}

TEST(union_chain) {
    UnionFind *uf = uf_create(5);

    uf_union(uf, 0, 1);
    uf_union(uf, 1, 2);
    uf_union(uf, 2, 3);
    uf_union(uf, 3, 4);

    ASSERT_EQ(uf_count(uf), 1);
    ASSERT_EQ(uf_size(uf, 0), 5);

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = i + 1; j < 5; j++) {
            ASSERT_TRUE(uf_connected(uf, i, j));
        }
    }

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE FIND COM PATH COMPRESSION
// ============================================================================

TEST(find_with_path_compression) {
    UnionFind *uf = uf_create(10);

    uf_union(uf, 0, 1);
    uf_union(uf, 1, 2);
    uf_union(uf, 2, 3);
    uf_union(uf, 3, 4);

    size_t root = uf_find(uf, 4);
    ASSERT_EQ(uf_find(uf, 0), root);
    ASSERT_EQ(uf_find(uf, 1), root);
    ASSERT_EQ(uf_find(uf, 2), root);
    ASSERT_EQ(uf_find(uf, 3), root);

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE CONNECTED
// ============================================================================

TEST(connected_check) {
    UnionFind *uf = uf_create(6);

    uf_union(uf, 0, 1);
    uf_union(uf, 2, 3);

    ASSERT_TRUE(uf_connected(uf, 0, 1));
    ASSERT_TRUE(uf_connected(uf, 2, 3));
    ASSERT_FALSE(uf_connected(uf, 0, 2));
    ASSERT_FALSE(uf_connected(uf, 1, 3));

    uf_union(uf, 1, 3);

    ASSERT_TRUE(uf_connected(uf, 0, 2));
    ASSERT_TRUE(uf_connected(uf, 0, 3));
    ASSERT_TRUE(uf_connected(uf, 1, 2));

    ASSERT_FALSE(uf_connected(uf, 0, 4));
    ASSERT_FALSE(uf_connected(uf, 0, 5));

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE COUNT
// ============================================================================

TEST(count_sets) {
    UnionFind *uf = uf_create(5);

    ASSERT_EQ(uf_count(uf), 5);

    uf_union(uf, 0, 1);
    ASSERT_EQ(uf_count(uf), 4);

    uf_union(uf, 2, 3);
    ASSERT_EQ(uf_count(uf), 3);

    uf_union(uf, 0, 2);
    ASSERT_EQ(uf_count(uf), 2);

    uf_union(uf, 0, 4);
    ASSERT_EQ(uf_count(uf), 1);

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE SET SIZE
// ============================================================================

TEST(set_size) {
    UnionFind *uf = uf_create(6);

    ASSERT_EQ(uf_size(uf, 0), 1);

    uf_union(uf, 0, 1);
    ASSERT_EQ(uf_size(uf, 0), 2);
    ASSERT_EQ(uf_size(uf, 1), 2);

    uf_union(uf, 2, 3);
    uf_union(uf, 0, 2);
    ASSERT_EQ(uf_size(uf, 0), 4);
    ASSERT_EQ(uf_size(uf, 3), 4);

    ASSERT_EQ(uf_size(uf, 4), 1);
    ASSERT_EQ(uf_size(uf, 5), 1);

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE GET_MEMBERS
// ============================================================================

TEST(get_members) {
    UnionFind *uf = uf_create(6);

    uf_union(uf, 0, 1);
    uf_union(uf, 0, 2);

    size_t *members = NULL;
    size_t size = 0;

    ASSERT_EQ(uf_get_members(uf, 0, &members, &size), DS_SUCCESS);
    ASSERT_EQ(size, 3);

    int found[3] = {0, 0, 0};
    for (size_t i = 0; i < size; i++) {
        if (members[i] == 0) found[0] = 1;
        if (members[i] == 1) found[1] = 1;
        if (members[i] == 2) found[2] = 1;
    }
    ASSERT_TRUE(found[0] && found[1] && found[2]);

    free(members);

    ASSERT_EQ(uf_get_members(uf, 3, &members, &size), DS_SUCCESS);
    ASSERT_EQ(size, 1);
    ASSERT_EQ(members[0], 3);
    free(members);

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE GET_COMPONENTS
// ============================================================================

TEST(get_components) {
    UnionFind *uf = uf_create(6);

    uf_union(uf, 0, 1);
    uf_union(uf, 0, 2);
    uf_union(uf, 3, 4);

    size_t *components = uf_get_components(uf);
    ASSERT_NOT_NULL(components);

    ASSERT_EQ(components[0], components[1]);
    ASSERT_EQ(components[0], components[2]);
    ASSERT_EQ(components[3], components[4]);
    ASSERT_NE(components[0], components[3]);
    ASSERT_NE(components[0], components[5]);
    ASSERT_NE(components[3], components[5]);

    free(components);
    uf_destroy(uf);
}

// ============================================================================
// TESTES DE RESET
// ============================================================================

TEST(reset) {
    UnionFind *uf = uf_create(5);

    uf_union(uf, 0, 1);
    uf_union(uf, 2, 3);
    uf_union(uf, 0, 4);
    ASSERT_EQ(uf_count(uf), 2);

    uf_reset(uf);

    ASSERT_EQ(uf_count(uf), 5);
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(uf_find(uf, i), i);
        ASSERT_EQ(uf_size(uf, i), 1);
    }
    for (size_t i = 0; i < 4; i++) {
        ASSERT_FALSE(uf_connected(uf, i, i + 1));
    }

    uf_destroy(uf);
}

// ============================================================================
// TESTE DE STRESS
// ============================================================================

TEST(stress_test) {
    const size_t N = 1000;
    UnionFind *uf = uf_create(N);

    for (size_t i = 0; i < N - 1; i += 2) {
        uf_union(uf, i, i + 1);
    }
    ASSERT_EQ(uf_count(uf), N / 2);

    for (size_t i = 0; i < N - 1; i += 2) {
        ASSERT_TRUE(uf_connected(uf, i, i + 1));
    }

    for (size_t i = 0; i < N - 2; i += 2) {
        uf_union(uf, i, i + 2);
    }
    ASSERT_EQ(uf_count(uf), 1);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = i + 1; j < N && j < i + 5; j++) {
            ASSERT_TRUE(uf_connected(uf, i, j));
        }
    }

    ASSERT_EQ(uf_size(uf, 0), N);

    uf_destroy(uf);
}

// ============================================================================
// TESTES DE NULL POINTER E INDICE INVALIDO
// ============================================================================

TEST(null_pointer_checks) {
    ASSERT_EQ(uf_count(NULL), 0);
    ASSERT_EQ(uf_size(NULL, 0), 0);
    ASSERT_FALSE(uf_union(NULL, 0, 1));
    ASSERT_FALSE(uf_connected(NULL, 0, 1));
    ASSERT_NULL(uf_get_components(NULL));

    size_t *members = NULL;
    size_t size = 0;
    ASSERT_EQ(uf_get_members(NULL, 0, &members, &size), DS_ERROR_NULL_POINTER);
}

TEST(invalid_index) {
    UnionFind *uf = uf_create(5);

    ASSERT_FALSE(uf_union(uf, 0, 10));
    ASSERT_FALSE(uf_union(uf, 10, 0));
    ASSERT_FALSE(uf_connected(uf, 0, 10));
    ASSERT_EQ(uf_size(uf, 10), 0);

    size_t *members = NULL;
    size_t size = 0;
    ASSERT_EQ(uf_get_members(uf, 10, &members, &size), DS_ERROR_INVALID_INDEX);

    uf_destroy(uf);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE UNION-FIND (DISJOINT SET)\n");
    printf("========================================\n\n");

    printf("Criacao e Destruicao:\n");
    RUN_TEST(create_destroy);
    RUN_TEST(create_zero);

    printf("\nEstado Inicial:\n");
    RUN_TEST(initially_disconnected);

    printf("\nUnion:\n");
    RUN_TEST(union_two);
    RUN_TEST(union_chain);

    printf("\nFind com Path Compression:\n");
    RUN_TEST(find_with_path_compression);

    printf("\nConnected:\n");
    RUN_TEST(connected_check);

    printf("\nCount Sets:\n");
    RUN_TEST(count_sets);

    printf("\nSet Size:\n");
    RUN_TEST(set_size);

    printf("\nGet Members:\n");
    RUN_TEST(get_members);

    printf("\nGet Components:\n");
    RUN_TEST(get_components);

    printf("\nReset:\n");
    RUN_TEST(reset);

    printf("\nStress Test:\n");
    RUN_TEST(stress_test);

    printf("\nNull Pointer e Indice Invalido:\n");
    RUN_TEST(null_pointer_checks);
    RUN_TEST(invalid_index);

    printf("\n============================================\n");
    printf("  TODOS OS TESTES PASSARAM! (14 testes)\n");
    printf("============================================\n");

    return 0;
}
