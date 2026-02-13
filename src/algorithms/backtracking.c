/**
 * @file backtracking.c
 * @brief Implementacao de 4 problemas classicos de backtracking
 *
 * Referencias:
 * - Knuth (2011), TAOCP Vol 4A, Chapter 7
 * - Wirth (1976), "Algorithms + Data Structures = Programs"
 * - Cormen et al. (2009), Chapter 34
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/backtracking.h"

#include <stdlib.h>
#include <string.h>

// ============================================================================
// N-QUEENS - Knuth TAOCP 4A S7.2.2
// ============================================================================

static bool nqueens_add_solution(NQueensResult *result, const int *board, size_t n) {
    if (result->count == result->capacity) {
        size_t new_cap = (result->capacity == 0) ? 8 : result->capacity * 2;
        int **new_arr = realloc(result->solutions, new_cap * sizeof(int *));
        if (new_arr == NULL) return false;
        result->solutions = new_arr;
        result->capacity = new_cap;
    }
    result->solutions[result->count] = malloc(n * sizeof(int));
    if (result->solutions[result->count] == NULL) return false;
    memcpy(result->solutions[result->count], board, n * sizeof(int));
    result->count++;
    return true;
}

static bool nqueens_is_safe(const int *board, size_t row, int col) {
    for (size_t i = 0; i < row; i++) {
        if (board[i] == col) return false;
        int diff = (int)row - (int)i;
        if (board[i] - col == diff || board[i] - col == -diff) return false;
    }
    return true;
}

static void nqueens_solve_rec(int *board, size_t row, size_t n, NQueensResult *result) {
    if (row == n) {
        nqueens_add_solution(result, board, n);
        return;
    }
    for (int col = 0; col < (int)n; col++) {
        if (nqueens_is_safe(board, row, col)) {
            board[row] = col;
            nqueens_solve_rec(board, row + 1, n, result);
        }
    }
}

NQueensResult nqueens_solve(size_t n) {
    NQueensResult result = { NULL, 0, 0, n };
    if (n == 0) return result;

    int *board = malloc(n * sizeof(int));
    if (board == NULL) return result;

    nqueens_solve_rec(board, 0, n, &result);
    free(board);
    return result;
}

static void nqueens_count_rec(int *board, size_t row, size_t n, size_t *count) {
    if (row == n) {
        (*count)++;
        return;
    }
    for (int col = 0; col < (int)n; col++) {
        if (nqueens_is_safe(board, row, col)) {
            board[row] = col;
            nqueens_count_rec(board, row + 1, n, count);
        }
    }
}

size_t nqueens_count(size_t n) {
    if (n == 0) return 0;
    int *board = malloc(n * sizeof(int));
    if (board == NULL) return 0;
    size_t count = 0;
    nqueens_count_rec(board, 0, n, &count);
    free(board);
    return count;
}

void nqueens_result_destroy(NQueensResult *result) {
    if (result == NULL) return;
    for (size_t i = 0; i < result->count; i++) {
        free(result->solutions[i]);
    }
    free(result->solutions);
    result->solutions = NULL;
    result->count = 0;
    result->capacity = 0;
}

// ============================================================================
// SUBSET SUM - Cormen S34.5.5
// ============================================================================

static bool subset_sum_add(SubsetSumResult *result, const bool *included, size_t n) {
    if (result->count == result->capacity) {
        size_t new_cap = (result->capacity == 0) ? 8 : result->capacity * 2;
        bool **new_arr = realloc(result->subsets, new_cap * sizeof(bool *));
        if (new_arr == NULL) return false;
        result->subsets = new_arr;
        result->capacity = new_cap;
    }
    result->subsets[result->count] = malloc(n * sizeof(bool));
    if (result->subsets[result->count] == NULL) return false;
    memcpy(result->subsets[result->count], included, n * sizeof(bool));
    result->count++;
    return true;
}

static void subset_sum_rec(const int *set, size_t n, int target, size_t idx,
                           int current_sum, bool *included, SubsetSumResult *result) {
    if (current_sum == target) {
        subset_sum_add(result, included, n);
        return;
    }
    if (idx >= n || current_sum > target) return;

    included[idx] = true;
    subset_sum_rec(set, n, target, idx + 1, current_sum + set[idx], included, result);

    included[idx] = false;
    subset_sum_rec(set, n, target, idx + 1, current_sum, included, result);
}

SubsetSumResult subset_sum_all(const int *set, size_t n, int target) {
    SubsetSumResult result = { NULL, 0, 0, n };
    if (set == NULL || n == 0) return result;

    bool *included = calloc(n, sizeof(bool));
    if (included == NULL) return result;

    subset_sum_rec(set, n, target, 0, 0, included, &result);
    free(included);
    return result;
}

static bool subset_sum_exists_rec(const int *set, size_t n, int target, size_t idx, int current) {
    if (current == target) return true;
    if (idx >= n || current > target) return false;
    if (subset_sum_exists_rec(set, n, target, idx + 1, current + set[idx])) return true;
    return subset_sum_exists_rec(set, n, target, idx + 1, current);
}

bool subset_sum_exists(const int *set, size_t n, int target) {
    if (set == NULL || n == 0) return (target == 0);
    return subset_sum_exists_rec(set, n, target, 0, 0);
}

void subset_sum_result_destroy(SubsetSumResult *result) {
    if (result == NULL) return;
    for (size_t i = 0; i < result->count; i++) {
        free(result->subsets[i]);
    }
    free(result->subsets);
    result->subsets = NULL;
    result->count = 0;
    result->capacity = 0;
}

// ============================================================================
// PERMUTATIONS - Knuth TAOCP 4A S7.2.1.2
// ============================================================================

static bool perm_add(PermutationResult *result, const int *arr, size_t n) {
    if (result->count == result->capacity) {
        size_t new_cap = (result->capacity == 0) ? 8 : result->capacity * 2;
        int **new_arr = realloc(result->perms, new_cap * sizeof(int *));
        if (new_arr == NULL) return false;
        result->perms = new_arr;
        result->capacity = new_cap;
    }
    result->perms[result->count] = malloc(n * sizeof(int));
    if (result->perms[result->count] == NULL) return false;
    memcpy(result->perms[result->count], arr, n * sizeof(int));
    result->count++;
    return true;
}

static void perm_swap(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

static void perm_rec(int *arr, size_t n, size_t idx, PermutationResult *result) {
    if (idx == n) {
        perm_add(result, arr, n);
        return;
    }
    for (size_t i = idx; i < n; i++) {
        perm_swap(&arr[idx], &arr[i]);
        perm_rec(arr, n, idx + 1, result);
        perm_swap(&arr[idx], &arr[i]);
    }
}

PermutationResult permutations_generate(const int *arr, size_t n) {
    PermutationResult result = { NULL, 0, 0, n };
    if (arr == NULL || n == 0) return result;

    int *work = malloc(n * sizeof(int));
    if (work == NULL) return result;
    memcpy(work, arr, n * sizeof(int));

    perm_rec(work, n, 0, &result);
    free(work);
    return result;
}

size_t permutations_count(size_t n) {
    size_t result = 1;
    for (size_t i = 2; i <= n; i++) result *= i;
    return result;
}

void permutation_result_destroy(PermutationResult *result) {
    if (result == NULL) return;
    for (size_t i = 0; i < result->count; i++) {
        free(result->perms[i]);
    }
    free(result->perms);
    result->perms = NULL;
    result->count = 0;
    result->capacity = 0;
}

// ============================================================================
// GRAPH COLORING
// ============================================================================

static bool coloring_is_safe(const int *adj, size_t n, const int *colors, size_t v, int c) {
    for (size_t i = 0; i < n; i++) {
        if (adj[v * n + i] && colors[i] == c) return false;
    }
    return true;
}

static bool coloring_rec(const int *adj, size_t n, int m, int *colors, size_t v) {
    if (v == n) return true;

    for (int c = 1; c <= m; c++) {
        if (coloring_is_safe(adj, n, colors, v, c)) {
            colors[v] = c;
            if (coloring_rec(adj, n, m, colors, v + 1)) return true;
            colors[v] = 0;
        }
    }
    return false;
}

GraphColoringResult graph_coloring(const int *adj, size_t n, int m) {
    GraphColoringResult result = { NULL, n, false };
    if (adj == NULL || n == 0 || m <= 0) return result;

    result.colors = calloc(n, sizeof(int));
    if (result.colors == NULL) return result;

    result.solvable = coloring_rec(adj, n, m, result.colors, 0);
    return result;
}

void graph_coloring_result_destroy(GraphColoringResult *result) {
    if (result == NULL) return;
    free(result->colors);
    result->colors = NULL;
    result->solvable = false;
}
