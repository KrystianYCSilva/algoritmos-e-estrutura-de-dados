/**
 * @file test_dynamic_programming.c
 * @brief Testes unitarios para algoritmos de programacao dinamica
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/dynamic_programming.h"
#include "../test_macros.h"

#include <string.h>

static bool is_subsequence(const char *sub, const char *str) {
    size_t si = 0;
    size_t sub_len = strlen(sub);
    for (size_t i = 0; str[i] != '\0' && si < sub_len; i++) {
        if (str[i] == sub[si]) si++;
    }
    return si == sub_len;
}

// ============================================================================
// FIBONACCI
// ============================================================================

TEST(fibonacci_base_cases) {
    ASSERT_EQ(dp_fibonacci_memo(0), 0);
    ASSERT_EQ(dp_fibonacci_memo(1), 1);
    ASSERT_EQ(dp_fibonacci_tab(0), 0);
    ASSERT_EQ(dp_fibonacci_tab(1), 1);
}

TEST(fibonacci_small) {
    ASSERT_EQ(dp_fibonacci_memo(10), 55);
    ASSERT_EQ(dp_fibonacci_tab(10), 55);
}

TEST(fibonacci_larger) {
    ASSERT_EQ(dp_fibonacci_memo(20), 6765);
    ASSERT_EQ(dp_fibonacci_tab(20), 6765);
}

TEST(fibonacci_consistency) {
    for (int i = 0; i <= 30; i++) {
        ASSERT_EQ(dp_fibonacci_memo(i), dp_fibonacci_tab(i));
    }
}

TEST(fibonacci_negative) {
    ASSERT_EQ(dp_fibonacci_memo(-1), -1);
    ASSERT_EQ(dp_fibonacci_tab(-1), -1);
}

// ============================================================================
// LCS
// ============================================================================

TEST(lcs_basic) {
    ASSERT_EQ(dp_lcs_length("ABCBDAB", "BDCAB"), 4);
}

TEST(lcs_identical) {
    ASSERT_EQ(dp_lcs_length("ABC", "ABC"), 3);
}

TEST(lcs_no_common) {
    ASSERT_EQ(dp_lcs_length("ABC", "XYZ"), 0);
}

TEST(lcs_empty) {
    ASSERT_EQ(dp_lcs_length("", "ABC"), 0);
    ASSERT_EQ(dp_lcs_length("ABC", ""), 0);
}

TEST(lcs_null) {
    ASSERT_EQ(dp_lcs_length(NULL, "ABC"), 0);
    ASSERT_EQ(dp_lcs_length("ABC", NULL), 0);
}

TEST(lcs_with_reconstruction) {
    LCSResult r = dp_lcs("ABCBDAB", "BDCAB");
    ASSERT_EQ(r.length, 4);
    ASSERT_NOT_NULL(r.sequence);
    ASSERT_EQ(strlen(r.sequence), 4);
    ASSERT_TRUE(is_subsequence(r.sequence, "ABCBDAB"));
    ASSERT_TRUE(is_subsequence(r.sequence, "BDCAB"));
    dp_lcs_result_destroy(&r);
}

TEST(lcs_cormen_example) {
    LCSResult r = dp_lcs("ABCBDAB", "BDCABA");
    ASSERT_EQ(r.length, 4);
    ASSERT_NOT_NULL(r.sequence);
    dp_lcs_result_destroy(&r);
}

// ============================================================================
// KNAPSACK 0/1
// ============================================================================

TEST(knapsack_basic) {
    int weights[] = {2, 3, 4, 5};
    int values[]  = {3, 4, 5, 6};
    ASSERT_EQ(dp_knapsack_value(weights, values, 4, 5), 7);
}

TEST(knapsack_exact_fit) {
    int weights[] = {1, 2, 3};
    int values[]  = {6, 10, 12};
    ASSERT_EQ(dp_knapsack_value(weights, values, 3, 5), 22);
}

TEST(knapsack_zero_capacity) {
    int weights[] = {1, 2};
    int values[]  = {10, 20};
    ASSERT_EQ(dp_knapsack_value(weights, values, 2, 0), 0);
}

TEST(knapsack_with_reconstruction) {
    int weights[] = {2, 3, 4, 5};
    int values[]  = {3, 4, 5, 6};
    KnapsackResult r = dp_knapsack(weights, values, 4, 5);
    ASSERT_EQ(r.max_value, 7);
    ASSERT_NOT_NULL(r.selected);

    int total_weight = 0;
    int total_value = 0;
    for (size_t i = 0; i < 4; i++) {
        if (r.selected[i]) {
            total_weight += weights[i];
            total_value += values[i];
        }
    }
    ASSERT_EQ(total_value, 7);
    ASSERT_TRUE(total_weight <= 5);
    dp_knapsack_result_destroy(&r);
}

// ============================================================================
// EDIT DISTANCE
// ============================================================================

TEST(edit_distance_basic) {
    ASSERT_EQ(dp_edit_distance("kitten", "sitting"), 3);
}

TEST(edit_distance_identical) {
    ASSERT_EQ(dp_edit_distance("abc", "abc"), 0);
}

TEST(edit_distance_empty) {
    ASSERT_EQ(dp_edit_distance("", "abc"), 3);
    ASSERT_EQ(dp_edit_distance("abc", ""), 3);
}

TEST(edit_distance_null) {
    ASSERT_EQ(dp_edit_distance(NULL, "abc"), 0);
}

TEST(edit_distance_single_op) {
    ASSERT_EQ(dp_edit_distance("abc", "ab"), 1);
    ASSERT_EQ(dp_edit_distance("abc", "abcd"), 1);
    ASSERT_EQ(dp_edit_distance("abc", "aXc"), 1);
}

// ============================================================================
// LIS
// ============================================================================

TEST(lis_basic) {
    int arr[] = {10, 9, 2, 5, 3, 7, 101, 18};
    ASSERT_EQ(dp_lis_length(arr, 8), 4);
}

TEST(lis_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(dp_lis_length(arr, 5), 5);
}

TEST(lis_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    ASSERT_EQ(dp_lis_length(arr, 5), 1);
}

TEST(lis_single) {
    int arr[] = {42};
    ASSERT_EQ(dp_lis_length(arr, 1), 1);
}

TEST(lis_with_reconstruction) {
    int arr[] = {3, 10, 2, 1, 20};
    LISResult r = dp_lis(arr, 5);
    ASSERT_EQ(r.length, 3);
    ASSERT_NOT_NULL(r.sequence);
    for (size_t i = 1; i < r.seq_len; i++) {
        ASSERT_TRUE(r.sequence[i] > r.sequence[i - 1]);
    }
    dp_lis_result_destroy(&r);
}

// ============================================================================
// ROD CUTTING
// ============================================================================

TEST(rod_cutting_cormen) {
    int prices[] = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    ASSERT_EQ(dp_rod_cutting_value(prices, 4), 10);
    ASSERT_EQ(dp_rod_cutting_value(prices, 8), 22);
    ASSERT_EQ(dp_rod_cutting_value(prices, 10), 30);
}

TEST(rod_cutting_single) {
    int prices[] = {5};
    ASSERT_EQ(dp_rod_cutting_value(prices, 1), 5);
}

TEST(rod_cutting_with_reconstruction) {
    int prices[] = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    RodCutResult r = dp_rod_cutting(prices, 4);
    ASSERT_EQ(r.max_revenue, 10);
    ASSERT_NOT_NULL(r.cuts);

    int total = 0;
    for (size_t i = 0; i < r.num_cuts; i++) {
        total += r.cuts[i];
    }
    ASSERT_EQ(total, 4);
    dp_rod_cut_result_destroy(&r);
}

// ============================================================================
// MATRIX CHAIN MULTIPLICATION
// ============================================================================

TEST(matrix_chain_cormen) {
    int dims[] = {30, 35, 15, 5, 10, 20, 25};
    ASSERT_EQ(dp_matrix_chain_value(dims, 6), 15125);
}

TEST(matrix_chain_two) {
    int dims[] = {10, 20, 30};
    ASSERT_EQ(dp_matrix_chain_value(dims, 2), 6000);
}

TEST(matrix_chain_single) {
    int dims[] = {10, 20};
    ASSERT_EQ(dp_matrix_chain_value(dims, 1), 0);
}

TEST(matrix_chain_with_parens) {
    int dims[] = {30, 35, 15, 5, 10, 20, 25};
    MatrixChainResult r = dp_matrix_chain(dims, 6);
    ASSERT_EQ(r.min_operations, 15125);
    ASSERT_NOT_NULL(r.parenthesization);
    dp_matrix_chain_result_destroy(&r);
}

// ============================================================================
// COIN CHANGE
// ============================================================================

TEST(coin_change_basic) {
    int coins[] = {1, 5, 10, 25};
    ASSERT_EQ(dp_coin_change_value(coins, 4, 30), 2);
}

TEST(coin_change_exact) {
    int coins[] = {1, 5, 10};
    ASSERT_EQ(dp_coin_change_value(coins, 3, 11), 2);
}

TEST(coin_change_impossible) {
    int coins[] = {2};
    ASSERT_EQ(dp_coin_change_value(coins, 1, 3), -1);
}

TEST(coin_change_zero) {
    int coins[] = {1, 2, 5};
    ASSERT_EQ(dp_coin_change_value(coins, 3, 0), 0);
}

TEST(coin_change_with_reconstruction) {
    int coins[] = {1, 5, 10, 25};
    CoinChangeResult r = dp_coin_change(coins, 4, 30);
    ASSERT_EQ(r.min_coins, 2);
    ASSERT_NOT_NULL(r.coins_used);

    int total = 0;
    for (size_t i = 0; i < r.num_coins_used; i++) {
        total += r.coins_used[i];
    }
    ASSERT_EQ(total, 30);
    dp_coin_change_result_destroy(&r);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Dynamic Programming Tests ===\n\n");

    printf("[Fibonacci]\n");
    RUN_TEST(fibonacci_base_cases);
    RUN_TEST(fibonacci_small);
    RUN_TEST(fibonacci_larger);
    RUN_TEST(fibonacci_consistency);
    RUN_TEST(fibonacci_negative);

    printf("\n[LCS]\n");
    RUN_TEST(lcs_basic);
    RUN_TEST(lcs_identical);
    RUN_TEST(lcs_no_common);
    RUN_TEST(lcs_empty);
    RUN_TEST(lcs_null);
    RUN_TEST(lcs_with_reconstruction);
    RUN_TEST(lcs_cormen_example);

    printf("\n[Knapsack 0/1]\n");
    RUN_TEST(knapsack_basic);
    RUN_TEST(knapsack_exact_fit);
    RUN_TEST(knapsack_zero_capacity);
    RUN_TEST(knapsack_with_reconstruction);

    printf("\n[Edit Distance]\n");
    RUN_TEST(edit_distance_basic);
    RUN_TEST(edit_distance_identical);
    RUN_TEST(edit_distance_empty);
    RUN_TEST(edit_distance_null);
    RUN_TEST(edit_distance_single_op);

    printf("\n[LIS]\n");
    RUN_TEST(lis_basic);
    RUN_TEST(lis_sorted);
    RUN_TEST(lis_reverse);
    RUN_TEST(lis_single);
    RUN_TEST(lis_with_reconstruction);

    printf("\n[Rod Cutting]\n");
    RUN_TEST(rod_cutting_cormen);
    RUN_TEST(rod_cutting_single);
    RUN_TEST(rod_cutting_with_reconstruction);

    printf("\n[Matrix Chain]\n");
    RUN_TEST(matrix_chain_cormen);
    RUN_TEST(matrix_chain_two);
    RUN_TEST(matrix_chain_single);
    RUN_TEST(matrix_chain_with_parens);

    printf("\n[Coin Change]\n");
    RUN_TEST(coin_change_basic);
    RUN_TEST(coin_change_exact);
    RUN_TEST(coin_change_impossible);
    RUN_TEST(coin_change_zero);
    RUN_TEST(coin_change_with_reconstruction);

    printf("\n=== All dynamic programming tests passed! ===\n");
    return 0;
}
