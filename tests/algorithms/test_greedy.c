/**
 * @file test_greedy.c
 * @brief Testes unitarios para algoritmos gulosos
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/greedy.h"
#include "../test_macros.h"

#include <string.h>
#include <math.h>

#define APPROX_EQ(a, b) ASSERT(fabs((a) - (b)) < 1e-6)

// ============================================================================
// ACTIVITY SELECTION
// ============================================================================

TEST(activity_basic) {
    int start[]  = {1, 3, 0, 5, 8, 5};
    int finish[] = {2, 4, 6, 7, 9, 9};
    ActivityResult r = greedy_activity_selection(start, finish, 6);
    ASSERT_EQ(r.count, 4);
    activity_result_destroy(&r);
}

TEST(activity_all_compatible) {
    int start[]  = {1, 3, 5, 7};
    int finish[] = {2, 4, 6, 8};
    ActivityResult r = greedy_activity_selection(start, finish, 4);
    ASSERT_EQ(r.count, 4);
    activity_result_destroy(&r);
}

TEST(activity_all_overlapping) {
    int start[]  = {0, 0, 0};
    int finish[] = {10, 10, 10};
    ActivityResult r = greedy_activity_selection(start, finish, 3);
    ASSERT_EQ(r.count, 1);
    activity_result_destroy(&r);
}

TEST(activity_single) {
    int start[]  = {1};
    int finish[] = {5};
    ActivityResult r = greedy_activity_selection(start, finish, 1);
    ASSERT_EQ(r.count, 1);
    activity_result_destroy(&r);
}

TEST(activity_null) {
    ActivityResult r = greedy_activity_selection(NULL, NULL, 0);
    ASSERT_EQ(r.count, 0);
    activity_result_destroy(&r);
}

TEST(activity_cormen) {
    int start[]  = {1, 3, 0, 5, 3, 5, 6, 8, 8, 2, 12};
    int finish[] = {4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16};
    ActivityResult r = greedy_activity_selection(start, finish, 11);
    ASSERT_TRUE(r.count >= 4);
    activity_result_destroy(&r);
}

// ============================================================================
// HUFFMAN CODING
// ============================================================================

TEST(huffman_basic) {
    char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int freqs[]  = {5, 9, 12, 13, 16, 45};
    HuffmanResult r = greedy_huffman(chars, freqs, 6);
    ASSERT_NOT_NULL(r.root);

    ASSERT_NOT_NULL(r.codes[(unsigned char)'f']);
    ASSERT_EQ(strlen(r.codes[(unsigned char)'f']), 1);

    for (int i = 0; i < 6; i++) {
        ASSERT_NOT_NULL(r.codes[(unsigned char)chars[i]]);
        ASSERT_TRUE(strlen(r.codes[(unsigned char)chars[i]]) > 0);
    }

    huffman_result_destroy(&r);
}

TEST(huffman_two_chars) {
    char chars[] = {'a', 'b'};
    int freqs[]  = {10, 20};
    HuffmanResult r = greedy_huffman(chars, freqs, 2);
    ASSERT_NOT_NULL(r.root);
    ASSERT_NOT_NULL(r.codes[(unsigned char)'a']);
    ASSERT_NOT_NULL(r.codes[(unsigned char)'b']);
    ASSERT_EQ(strlen(r.codes[(unsigned char)'a']), 1);
    ASSERT_EQ(strlen(r.codes[(unsigned char)'b']), 1);
    huffman_result_destroy(&r);
}

TEST(huffman_single) {
    char chars[] = {'x'};
    int freqs[]  = {100};
    HuffmanResult r = greedy_huffman(chars, freqs, 1);
    ASSERT_NOT_NULL(r.root);
    ASSERT_NOT_NULL(r.codes[(unsigned char)'x']);
    huffman_result_destroy(&r);
}

TEST(huffman_prefix_free) {
    char chars[] = {'a', 'b', 'c', 'd'};
    int freqs[]  = {1, 2, 3, 4};
    HuffmanResult r = greedy_huffman(chars, freqs, 4);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) continue;
            char *ci = r.codes[(unsigned char)chars[i]];
            char *cj = r.codes[(unsigned char)chars[j]];
            ASSERT_NOT_NULL(ci);
            ASSERT_NOT_NULL(cj);
            size_t li = strlen(ci);
            size_t lj = strlen(cj);
            size_t min_len = (li < lj) ? li : lj;
            ASSERT_FALSE(strncmp(ci, cj, min_len) == 0 && (li == min_len || lj == min_len));
        }
    }

    huffman_result_destroy(&r);
}

TEST(huffman_null) {
    HuffmanResult r = greedy_huffman(NULL, NULL, 0);
    ASSERT_NULL(r.root);
    huffman_result_destroy(&r);
}

// ============================================================================
// FRACTIONAL KNAPSACK
// ============================================================================

TEST(fractional_knapsack_basic) {
    double weights[] = {10.0, 20.0, 30.0};
    double values[]  = {60.0, 100.0, 120.0};
    double result = greedy_fractional_knapsack_value(weights, values, 3, 50.0);
    APPROX_EQ(result, 240.0);
}

TEST(fractional_knapsack_all_fit) {
    double weights[] = {10.0, 20.0, 30.0};
    double values[]  = {60.0, 100.0, 120.0};
    double result = greedy_fractional_knapsack_value(weights, values, 3, 100.0);
    APPROX_EQ(result, 280.0);
}

TEST(fractional_knapsack_zero_capacity) {
    double weights[] = {10.0};
    double values[]  = {60.0};
    double result = greedy_fractional_knapsack_value(weights, values, 1, 0.0);
    APPROX_EQ(result, 0.0);
}

TEST(fractional_knapsack_with_fractions) {
    double weights[] = {10.0, 20.0, 30.0};
    double values[]  = {60.0, 100.0, 120.0};
    FractionalKnapsackResult r = greedy_fractional_knapsack(weights, values, 3, 50.0);
    APPROX_EQ(r.max_value, 240.0);
    ASSERT_NOT_NULL(r.fractions);

    double total_weight = 0.0;
    for (size_t i = 0; i < 3; i++) {
        ASSERT_TRUE(r.fractions[i] >= 0.0 && r.fractions[i] <= 1.0);
        total_weight += r.fractions[i] * weights[i];
    }
    ASSERT_TRUE(total_weight <= 50.0 + 1e-6);

    fractional_knapsack_result_destroy(&r);
}

TEST(fractional_knapsack_null) {
    double result = greedy_fractional_knapsack_value(NULL, NULL, 0, 10.0);
    APPROX_EQ(result, 0.0);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Greedy Algorithm Tests ===\n\n");

    printf("[Activity Selection]\n");
    RUN_TEST(activity_basic);
    RUN_TEST(activity_all_compatible);
    RUN_TEST(activity_all_overlapping);
    RUN_TEST(activity_single);
    RUN_TEST(activity_null);
    RUN_TEST(activity_cormen);

    printf("\n[Huffman Coding]\n");
    RUN_TEST(huffman_basic);
    RUN_TEST(huffman_two_chars);
    RUN_TEST(huffman_single);
    RUN_TEST(huffman_prefix_free);
    RUN_TEST(huffman_null);

    printf("\n[Fractional Knapsack]\n");
    RUN_TEST(fractional_knapsack_basic);
    RUN_TEST(fractional_knapsack_all_fit);
    RUN_TEST(fractional_knapsack_zero_capacity);
    RUN_TEST(fractional_knapsack_with_fractions);
    RUN_TEST(fractional_knapsack_null);

    printf("\n=== All greedy algorithm tests passed! ===\n");
    return 0;
}
