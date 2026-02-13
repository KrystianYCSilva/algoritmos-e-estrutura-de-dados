/**
 * @file test_backtracking.c
 * @brief Testes unitarios para algoritmos de backtracking
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/backtracking.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// N-QUEENS
// ============================================================================

TEST(nqueens_1) {
    ASSERT_EQ(nqueens_count(1), 1);
}

TEST(nqueens_4) {
    ASSERT_EQ(nqueens_count(4), 2);
}

TEST(nqueens_8) {
    ASSERT_EQ(nqueens_count(8), 92);
}

TEST(nqueens_solve_4) {
    NQueensResult r = nqueens_solve(4);
    ASSERT_EQ(r.count, 2);
    ASSERT_EQ(r.n, 4);

    for (size_t s = 0; s < r.count; s++) {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = i + 1; j < 4; j++) {
                ASSERT_NE(r.solutions[s][i], r.solutions[s][j]);
                int diff = (int)j - (int)i;
                ASSERT_NE(r.solutions[s][i] - r.solutions[s][j], diff);
                ASSERT_NE(r.solutions[s][i] - r.solutions[s][j], -diff);
            }
        }
    }
    nqueens_result_destroy(&r);
}

TEST(nqueens_2_3_impossible) {
    ASSERT_EQ(nqueens_count(2), 0);
    ASSERT_EQ(nqueens_count(3), 0);
}

// ============================================================================
// SUBSET SUM
// ============================================================================

TEST(subset_sum_basic) {
    int set[] = {3, 34, 4, 12, 5, 2};
    ASSERT_TRUE(subset_sum_exists(set, 6, 9));
}

TEST(subset_sum_not_found) {
    int set[] = {3, 34, 4, 12, 5, 2};
    ASSERT_FALSE(subset_sum_exists(set, 6, 30));
}

TEST(subset_sum_zero) {
    int set[] = {1, 2, 3};
    ASSERT_TRUE(subset_sum_exists(set, 3, 0));
}

TEST(subset_sum_all_results) {
    int set[] = {1, 2, 3};
    SubsetSumResult r = subset_sum_all(set, 3, 3);
    ASSERT_EQ(r.count, 2);

    for (size_t i = 0; i < r.count; i++) {
        int sum = 0;
        for (size_t j = 0; j < 3; j++) {
            if (r.subsets[i][j]) sum += set[j];
        }
        ASSERT_EQ(sum, 3);
    }
    subset_sum_result_destroy(&r);
}

TEST(subset_sum_single) {
    int set[] = {5};
    ASSERT_TRUE(subset_sum_exists(set, 1, 5));
    ASSERT_FALSE(subset_sum_exists(set, 1, 3));
}

// ============================================================================
// PERMUTATIONS
// ============================================================================

TEST(permutations_3) {
    int arr[] = {1, 2, 3};
    PermutationResult r = permutations_generate(arr, 3);
    ASSERT_EQ(r.count, 6);
    ASSERT_EQ(permutations_count(3), 6);

    for (size_t i = 0; i < r.count; i++) {
        int sum = 0;
        for (size_t j = 0; j < 3; j++) sum += r.perms[i][j];
        ASSERT_EQ(sum, 6);
    }
    permutation_result_destroy(&r);
}

TEST(permutations_1) {
    int arr[] = {42};
    PermutationResult r = permutations_generate(arr, 1);
    ASSERT_EQ(r.count, 1);
    ASSERT_EQ(r.perms[0][0], 42);
    permutation_result_destroy(&r);
}

TEST(permutations_count_check) {
    ASSERT_EQ(permutations_count(0), 1);
    ASSERT_EQ(permutations_count(1), 1);
    ASSERT_EQ(permutations_count(4), 24);
    ASSERT_EQ(permutations_count(5), 120);
}

TEST(permutations_unique) {
    int arr[] = {1, 2, 3};
    PermutationResult r = permutations_generate(arr, 3);
    for (size_t i = 0; i < r.count; i++) {
        for (size_t j = i + 1; j < r.count; j++) {
            ASSERT_FALSE(memcmp(r.perms[i], r.perms[j], 3 * sizeof(int)) == 0);
        }
    }
    permutation_result_destroy(&r);
}

// ============================================================================
// GRAPH COLORING
// ============================================================================

TEST(graph_coloring_triangle) {
    int adj[] = {0,1,1, 1,0,1, 1,1,0};
    GraphColoringResult r = graph_coloring(adj, 3, 3);
    ASSERT_TRUE(r.solvable);
    ASSERT_NOT_NULL(r.colors);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = i + 1; j < 3; j++) {
            if (adj[i * 3 + j]) {
                ASSERT_NE(r.colors[i], r.colors[j]);
            }
        }
    }
    graph_coloring_result_destroy(&r);
}

TEST(graph_coloring_triangle_2colors) {
    int adj[] = {0,1,1, 1,0,1, 1,1,0};
    GraphColoringResult r = graph_coloring(adj, 3, 2);
    ASSERT_FALSE(r.solvable);
    graph_coloring_result_destroy(&r);
}

TEST(graph_coloring_bipartite) {
    int adj[] = {0,1,0,1, 1,0,1,0, 0,1,0,1, 1,0,1,0};
    GraphColoringResult r = graph_coloring(adj, 4, 2);
    ASSERT_TRUE(r.solvable);

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = i + 1; j < 4; j++) {
            if (adj[i * 4 + j]) {
                ASSERT_NE(r.colors[i], r.colors[j]);
            }
        }
    }
    graph_coloring_result_destroy(&r);
}

TEST(graph_coloring_no_edges) {
    int adj[] = {0,0,0, 0,0,0, 0,0,0};
    GraphColoringResult r = graph_coloring(adj, 3, 1);
    ASSERT_TRUE(r.solvable);
    graph_coloring_result_destroy(&r);
}

TEST(graph_coloring_petersen) {
    int adj[100];
    memset(adj, 0, sizeof(adj));
    int edges[][2] = {{0,1},{1,2},{2,3},{3,4},{4,0},{0,5},{1,6},{2,7},{3,8},{4,9},{5,7},{7,9},{9,6},{6,8},{8,5}};
    for (int i = 0; i < 15; i++) {
        adj[edges[i][0]*10 + edges[i][1]] = 1;
        adj[edges[i][1]*10 + edges[i][0]] = 1;
    }
    GraphColoringResult r = graph_coloring(adj, 10, 3);
    ASSERT_TRUE(r.solvable);

    for (int i = 0; i < 15; i++) {
        ASSERT_NE(r.colors[edges[i][0]], r.colors[edges[i][1]]);
    }
    graph_coloring_result_destroy(&r);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Backtracking Tests ===\n\n");

    printf("[N-Queens]\n");
    RUN_TEST(nqueens_1);
    RUN_TEST(nqueens_4);
    RUN_TEST(nqueens_8);
    RUN_TEST(nqueens_solve_4);
    RUN_TEST(nqueens_2_3_impossible);

    printf("\n[Subset Sum]\n");
    RUN_TEST(subset_sum_basic);
    RUN_TEST(subset_sum_not_found);
    RUN_TEST(subset_sum_zero);
    RUN_TEST(subset_sum_all_results);
    RUN_TEST(subset_sum_single);

    printf("\n[Permutations]\n");
    RUN_TEST(permutations_3);
    RUN_TEST(permutations_1);
    RUN_TEST(permutations_count_check);
    RUN_TEST(permutations_unique);

    printf("\n[Graph Coloring]\n");
    RUN_TEST(graph_coloring_triangle);
    RUN_TEST(graph_coloring_triangle_2colors);
    RUN_TEST(graph_coloring_bipartite);
    RUN_TEST(graph_coloring_no_edges);
    RUN_TEST(graph_coloring_petersen);

    printf("\n=== All backtracking tests passed! ===\n");
    return 0;
}
