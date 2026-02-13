/**
 * @file test_searching.c
 * @brief Testes unitarios para algoritmos de busca
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/searching.h"
#include "data_structures/common.h"
#include "../test_macros.h"

// ============================================================================
// LINEAR SEARCH
// ============================================================================

TEST(linear_search_found) {
    int arr[] = {5, 3, 8, 1, 2, 7};
    int target = 8;
    ASSERT_EQ(linear_search(arr, 6, sizeof(int), &target, compare_int), 2);
}

TEST(linear_search_not_found) {
    int arr[] = {5, 3, 8, 1, 2};
    int target = 99;
    ASSERT_EQ(linear_search(arr, 5, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

TEST(linear_search_first) {
    int arr[] = {42, 1, 2, 3};
    int target = 42;
    ASSERT_EQ(linear_search(arr, 4, sizeof(int), &target, compare_int), 0);
}

TEST(linear_search_empty) {
    int arr[] = {1};
    int target = 1;
    ASSERT_EQ(linear_search(arr, 0, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// BINARY SEARCH
// ============================================================================

TEST(binary_search_found) {
    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int target = 7;
    ASSERT_EQ(binary_search(arr, 7, sizeof(int), &target, compare_int), 3);
}

TEST(binary_search_first) {
    int arr[] = {1, 3, 5, 7, 9};
    int target = 1;
    ASSERT_EQ(binary_search(arr, 5, sizeof(int), &target, compare_int), 0);
}

TEST(binary_search_last) {
    int arr[] = {1, 3, 5, 7, 9};
    int target = 9;
    ASSERT_EQ(binary_search(arr, 5, sizeof(int), &target, compare_int), 4);
}

TEST(binary_search_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    int target = 6;
    ASSERT_EQ(binary_search(arr, 5, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

TEST(binary_search_single) {
    int arr[] = {42};
    int target = 42;
    ASSERT_EQ(binary_search(arr, 1, sizeof(int), &target, compare_int), 0);
    int miss = 99;
    ASSERT_EQ(binary_search(arr, 1, sizeof(int), &miss, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// INTERPOLATION SEARCH
// ============================================================================

TEST(interpolation_search_found) {
    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    ASSERT_EQ(interpolation_search(arr, 10, 50), 4);
    ASSERT_EQ(interpolation_search(arr, 10, 10), 0);
    ASSERT_EQ(interpolation_search(arr, 10, 100), 9);
}

TEST(interpolation_search_not_found) {
    int arr[] = {10, 20, 30, 40, 50};
    ASSERT_EQ(interpolation_search(arr, 5, 35), SEARCH_NOT_FOUND);
    ASSERT_EQ(interpolation_search(arr, 5, 5), SEARCH_NOT_FOUND);
}

// ============================================================================
// TERNARY SEARCH
// ============================================================================

TEST(ternary_search_found) {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int target = 11;
    ASSERT_EQ(ternary_search(arr, 8, sizeof(int), &target, compare_int), 5);
}

TEST(ternary_search_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    int target = 4;
    ASSERT_EQ(ternary_search(arr, 5, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// JUMP SEARCH
// ============================================================================

TEST(jump_search_found) {
    int arr[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    int target = 55;
    ASSERT_EQ(jump_search(arr, 12, sizeof(int), &target, compare_int), 10);
}

TEST(jump_search_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    int target = 6;
    ASSERT_EQ(jump_search(arr, 5, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// EXPONENTIAL SEARCH
// ============================================================================

TEST(exponential_search_found) {
    int arr[] = {2, 3, 4, 10, 40};
    int target = 10;
    ASSERT_EQ(exponential_search(arr, 5, sizeof(int), &target, compare_int), 3);
}

TEST(exponential_search_first) {
    int arr[] = {2, 3, 4, 10, 40};
    int target = 2;
    ASSERT_EQ(exponential_search(arr, 5, sizeof(int), &target, compare_int), 0);
}

TEST(exponential_search_not_found) {
    int arr[] = {2, 3, 4, 10, 40};
    int target = 99;
    ASSERT_EQ(exponential_search(arr, 5, sizeof(int), &target, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(null_params) {
    ASSERT_EQ(linear_search(NULL, 5, sizeof(int), NULL, compare_int), SEARCH_NOT_FOUND);
    ASSERT_EQ(binary_search(NULL, 5, sizeof(int), NULL, compare_int), SEARCH_NOT_FOUND);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Searching Algorithm Tests ===\n");

    RUN_TEST(linear_search_found);
    RUN_TEST(linear_search_not_found);
    RUN_TEST(linear_search_first);
    RUN_TEST(linear_search_empty);
    RUN_TEST(binary_search_found);
    RUN_TEST(binary_search_first);
    RUN_TEST(binary_search_last);
    RUN_TEST(binary_search_not_found);
    RUN_TEST(binary_search_single);
    RUN_TEST(interpolation_search_found);
    RUN_TEST(interpolation_search_not_found);
    RUN_TEST(ternary_search_found);
    RUN_TEST(ternary_search_not_found);
    RUN_TEST(jump_search_found);
    RUN_TEST(jump_search_not_found);
    RUN_TEST(exponential_search_found);
    RUN_TEST(exponential_search_first);
    RUN_TEST(exponential_search_not_found);
    RUN_TEST(null_params);

    printf("\nAll Searching tests passed!\n");
    return 0;
}
