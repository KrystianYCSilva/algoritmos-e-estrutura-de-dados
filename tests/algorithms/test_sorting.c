/**
 * @file test_sorting.c
 * @brief Testes unitarios para algoritmos de ordenacao
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/sorting.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>
#include <stdlib.h>

// ============================================================================
// HELPERS
// ============================================================================

static void fill_reverse(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = (int)(n - i);
}

static void fill_random(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = (int)(i * 97 + 31) % 1000;
}

static void assert_sorted_int(int *arr, size_t n) {
    ASSERT_TRUE(is_sorted(arr, n, sizeof(int), compare_int));
}

// ============================================================================
// BUBBLE SORT
// ============================================================================

TEST(bubble_sort_basic) {
    int arr[] = {5, 3, 8, 1, 2};
    bubble_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[4], 8);
}

TEST(bubble_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    bubble_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
}

TEST(bubble_sort_single) {
    int arr[] = {42};
    bubble_sort(arr, 1, sizeof(int), compare_int);
    ASSERT_EQ(arr[0], 42);
}

// ============================================================================
// SELECTION SORT
// ============================================================================

TEST(selection_sort_basic) {
    int arr[] = {64, 25, 12, 22, 11};
    selection_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
    ASSERT_EQ(arr[0], 11);
}

// ============================================================================
// INSERTION SORT
// ============================================================================

TEST(insertion_sort_basic) {
    int arr[] = {12, 11, 13, 5, 6};
    insertion_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
}

TEST(insertion_sort_nearly_sorted) {
    int arr[] = {1, 2, 4, 3, 5};
    insertion_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
}

// ============================================================================
// SHELL SORT
// ============================================================================

TEST(shell_sort_basic) {
    int arr[] = {35, 33, 42, 10, 14, 19, 27, 44};
    shell_sort(arr, 8, sizeof(int), compare_int);
    assert_sorted_int(arr, 8);
}

// ============================================================================
// MERGE SORT
// ============================================================================

TEST(merge_sort_basic) {
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    merge_sort(arr, 7, sizeof(int), compare_int);
    assert_sorted_int(arr, 7);
}

TEST(merge_sort_large) {
    int arr[200];
    fill_reverse(arr, 200);
    merge_sort(arr, 200, sizeof(int), compare_int);
    assert_sorted_int(arr, 200);
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[199], 200);
}

// ============================================================================
// QUICK SORT
// ============================================================================

TEST(quick_sort_basic) {
    int arr[] = {10, 7, 8, 9, 1, 5};
    quick_sort(arr, 6, sizeof(int), compare_int);
    assert_sorted_int(arr, 6);
}

TEST(quick_sort_large) {
    int arr[500];
    fill_random(arr, 500);
    quick_sort(arr, 500, sizeof(int), compare_int);
    assert_sorted_int(arr, 500);
}

TEST(quick_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    quick_sort(arr, 11, sizeof(int), compare_int);
    assert_sorted_int(arr, 11);
}

// ============================================================================
// HEAP SORT
// ============================================================================

TEST(heap_sort_basic) {
    int arr[] = {4, 10, 3, 5, 1};
    heap_sort(arr, 5, sizeof(int), compare_int);
    assert_sorted_int(arr, 5);
}

TEST(heap_sort_large) {
    int arr[300];
    fill_reverse(arr, 300);
    heap_sort(arr, 300, sizeof(int), compare_int);
    assert_sorted_int(arr, 300);
}

// ============================================================================
// COUNTING SORT
// ============================================================================

TEST(counting_sort_basic) {
    int arr[] = {4, 2, 2, 8, 3, 3, 1};
    counting_sort(arr, 7, 8);
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[6], 8);
    for (size_t i = 0; i < 6; i++) ASSERT_TRUE(arr[i] <= arr[i + 1]);
}

// ============================================================================
// RADIX SORT
// ============================================================================

TEST(radix_sort_basic) {
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    radix_sort(arr, 8);
    ASSERT_EQ(arr[0], 2);
    ASSERT_EQ(arr[7], 802);
    for (size_t i = 0; i < 7; i++) ASSERT_TRUE(arr[i] <= arr[i + 1]);
}

// ============================================================================
// BUCKET SORT
// ============================================================================

TEST(bucket_sort_basic) {
    double arr[] = {0.897, 0.565, 0.656, 0.123, 0.665, 0.343};
    bucket_sort(arr, 6);
    for (size_t i = 0; i < 5; i++) ASSERT_TRUE(arr[i] <= arr[i + 1]);
    ASSERT_TRUE(arr[0] > 0.12 && arr[0] < 0.13);
}

// ============================================================================
// IS_SORTED
// ============================================================================

TEST(is_sorted_check) {
    int sorted[] = {1, 2, 3, 4, 5};
    int unsorted[] = {5, 3, 1, 4, 2};
    ASSERT_TRUE(is_sorted(sorted, 5, sizeof(int), compare_int));
    ASSERT_FALSE(is_sorted(unsorted, 5, sizeof(int), compare_int));
    ASSERT_TRUE(is_sorted(sorted, 0, sizeof(int), compare_int));
    ASSERT_TRUE(is_sorted(sorted, 1, sizeof(int), compare_int));
}

// ============================================================================
// GENERICS - doubles
// ============================================================================

TEST(sort_doubles) {
    double arr[] = {3.14, 1.41, 2.72, 0.57, 1.73};
    merge_sort(arr, 5, sizeof(double), compare_double);
    ASSERT_TRUE(is_sorted(arr, 5, sizeof(double), compare_double));
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(null_and_empty) {
    bubble_sort(NULL, 5, sizeof(int), compare_int);
    int arr[] = {1};
    quick_sort(arr, 0, sizeof(int), compare_int);
    merge_sort(arr, 1, sizeof(int), compare_int);
    ASSERT_EQ(arr[0], 1);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Sorting Algorithm Tests ===\n");

    RUN_TEST(bubble_sort_basic);
    RUN_TEST(bubble_sort_already_sorted);
    RUN_TEST(bubble_sort_single);
    RUN_TEST(selection_sort_basic);
    RUN_TEST(insertion_sort_basic);
    RUN_TEST(insertion_sort_nearly_sorted);
    RUN_TEST(shell_sort_basic);
    RUN_TEST(merge_sort_basic);
    RUN_TEST(merge_sort_large);
    RUN_TEST(quick_sort_basic);
    RUN_TEST(quick_sort_large);
    RUN_TEST(quick_sort_duplicates);
    RUN_TEST(heap_sort_basic);
    RUN_TEST(heap_sort_large);
    RUN_TEST(counting_sort_basic);
    RUN_TEST(radix_sort_basic);
    RUN_TEST(bucket_sort_basic);
    RUN_TEST(is_sorted_check);
    RUN_TEST(sort_doubles);
    RUN_TEST(null_and_empty);

    printf("\nAll Sorting tests passed!\n");
    return 0;
}
