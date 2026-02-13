/**
 * @file test_divide_conquer.c
 * @brief Testes unitarios para algoritmos de divisao e conquista
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/divide_conquer.h"
#include "../test_macros.h"

#include <math.h>
#include <string.h>

#define APPROX_EQ(a, b) ASSERT(fabs((a) - (b)) < 1e-4)

// ============================================================================
// STRASSEN MATRIX MULTIPLICATION
// ============================================================================

TEST(strassen_2x2) {
    double A[] = {1, 2, 3, 4};
    double B[] = {5, 6, 7, 8};
    double C[4];
    strassen_multiply(A, B, C, 2);
    APPROX_EQ(C[0], 19.0);
    APPROX_EQ(C[1], 22.0);
    APPROX_EQ(C[2], 43.0);
    APPROX_EQ(C[3], 50.0);
}

TEST(strassen_identity) {
    double A[] = {1, 0, 0, 1};
    double B[] = {5, 6, 7, 8};
    double C[4];
    strassen_multiply(A, B, C, 2);
    APPROX_EQ(C[0], 5.0);
    APPROX_EQ(C[1], 6.0);
    APPROX_EQ(C[2], 7.0);
    APPROX_EQ(C[3], 8.0);
}

TEST(strassen_vs_classic_4x4) {
    double A[16], B[16], C1[16], C2[16];
    for (int i = 0; i < 16; i++) {
        A[i] = (double)(i + 1);
        B[i] = (double)(16 - i);
    }
    matrix_multiply_classic(A, B, C1, 4);
    strassen_multiply(A, B, C2, 4);
    for (int i = 0; i < 16; i++) {
        APPROX_EQ(C1[i], C2[i]);
    }
}

TEST(strassen_3x3) {
    double A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double B[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    double C1[9], C2[9];
    matrix_multiply_classic(A, B, C1, 3);
    strassen_multiply(A, B, C2, 3);
    for (int i = 0; i < 9; i++) {
        APPROX_EQ(C1[i], C2[i]);
    }
}

// ============================================================================
// CLOSEST PAIR
// ============================================================================

TEST(closest_pair_basic) {
    Point2D pts[] = {{0,0}, {1,1}, {3,3}, {5,5}};
    ClosestPairResult r = closest_pair(pts, 4);
    APPROX_EQ(r.distance, sqrt(2.0));
}

TEST(closest_pair_collinear) {
    Point2D pts[] = {{0,0}, {2,0}, {5,0}, {9,0}};
    ClosestPairResult r = closest_pair(pts, 4);
    APPROX_EQ(r.distance, 2.0);
}

TEST(closest_pair_two) {
    Point2D pts[] = {{0,0}, {3,4}};
    ClosestPairResult r = closest_pair(pts, 2);
    APPROX_EQ(r.distance, 5.0);
}

TEST(closest_pair_many) {
    Point2D pts[] = {{0,0},{10,10},{20,20},{30,30},{5,0},{5,0.5}};
    ClosestPairResult r = closest_pair(pts, 6);
    APPROX_EQ(r.distance, 0.5);
}

// ============================================================================
// KARATSUBA
// ============================================================================

TEST(karatsuba_small) {
    ASSERT_EQ(karatsuba_multiply(12, 34), 408);
    ASSERT_EQ(karatsuba_multiply(5, 6), 30);
}

TEST(karatsuba_medium) {
    ASSERT_EQ(karatsuba_multiply(1234, 5678), 7006652);
    ASSERT_EQ(karatsuba_multiply(12345, 6789), 83810205);
}

TEST(karatsuba_with_zero) {
    ASSERT_EQ(karatsuba_multiply(0, 12345), 0);
    ASSERT_EQ(karatsuba_multiply(12345, 0), 0);
}

TEST(karatsuba_negative) {
    ASSERT_EQ(karatsuba_multiply(-12, 34), -408);
    ASSERT_EQ(karatsuba_multiply(12, -34), -408);
    ASSERT_EQ(karatsuba_multiply(-12, -34), 408);
}

TEST(karatsuba_one) {
    ASSERT_EQ(karatsuba_multiply(1, 99999), 99999);
}

// ============================================================================
// MAXIMUM SUBARRAY
// ============================================================================

TEST(max_subarray_basic) {
    int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    MaxSubarrayResult dc = max_subarray_dc(arr, 9);
    MaxSubarrayResult k = max_subarray_kadane(arr, 9);
    ASSERT_EQ(dc.sum, 6);
    ASSERT_EQ(k.sum, 6);
}

TEST(max_subarray_all_negative) {
    int arr[] = {-5, -3, -8, -1, -4};
    MaxSubarrayResult dc = max_subarray_dc(arr, 5);
    MaxSubarrayResult k = max_subarray_kadane(arr, 5);
    ASSERT_EQ(dc.sum, -1);
    ASSERT_EQ(k.sum, -1);
}

TEST(max_subarray_all_positive) {
    int arr[] = {1, 2, 3, 4, 5};
    MaxSubarrayResult dc = max_subarray_dc(arr, 5);
    MaxSubarrayResult k = max_subarray_kadane(arr, 5);
    ASSERT_EQ(dc.sum, 15);
    ASSERT_EQ(k.sum, 15);
}

TEST(max_subarray_single) {
    int arr[] = {42};
    MaxSubarrayResult dc = max_subarray_dc(arr, 1);
    MaxSubarrayResult k = max_subarray_kadane(arr, 1);
    ASSERT_EQ(dc.sum, 42);
    ASSERT_EQ(k.sum, 42);
}

TEST(max_subarray_cormen) {
    int arr[] = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    MaxSubarrayResult dc = max_subarray_dc(arr, 16);
    MaxSubarrayResult k = max_subarray_kadane(arr, 16);
    ASSERT_EQ(dc.sum, 43);
    ASSERT_EQ(k.sum, 43);
}

// ============================================================================
// QUICK SELECT
// ============================================================================

TEST(quick_select_basic) {
    int arr[] = {7, 10, 4, 3, 20, 15};
    bool found;
    int r = quick_select(arr, 6, 0, &found);
    ASSERT_TRUE(found);
    ASSERT_EQ(r, 3);
}

TEST(quick_select_largest) {
    int arr[] = {7, 10, 4, 3, 20, 15};
    bool found;
    int r = quick_select(arr, 6, 5, &found);
    ASSERT_TRUE(found);
    ASSERT_EQ(r, 20);
}

TEST(quick_select_median) {
    int arr[] = {7, 10, 4, 3, 20};
    int m = median(arr, 5);
    ASSERT_EQ(m, 7);
}

TEST(quick_select_invalid) {
    int arr[] = {1, 2, 3};
    bool found;
    quick_select(arr, 3, 5, &found);
    ASSERT_FALSE(found);
}

TEST(quick_select_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    bool found;
    ASSERT_EQ(quick_select(arr, 5, 2, &found), 3);
    ASSERT_TRUE(found);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Divide & Conquer Tests ===\n\n");

    printf("[Strassen]\n");
    RUN_TEST(strassen_2x2);
    RUN_TEST(strassen_identity);
    RUN_TEST(strassen_vs_classic_4x4);
    RUN_TEST(strassen_3x3);

    printf("\n[Closest Pair]\n");
    RUN_TEST(closest_pair_basic);
    RUN_TEST(closest_pair_collinear);
    RUN_TEST(closest_pair_two);
    RUN_TEST(closest_pair_many);

    printf("\n[Karatsuba]\n");
    RUN_TEST(karatsuba_small);
    RUN_TEST(karatsuba_medium);
    RUN_TEST(karatsuba_with_zero);
    RUN_TEST(karatsuba_negative);
    RUN_TEST(karatsuba_one);

    printf("\n[Maximum Subarray]\n");
    RUN_TEST(max_subarray_basic);
    RUN_TEST(max_subarray_all_negative);
    RUN_TEST(max_subarray_all_positive);
    RUN_TEST(max_subarray_single);
    RUN_TEST(max_subarray_cormen);

    printf("\n[Quick Select]\n");
    RUN_TEST(quick_select_basic);
    RUN_TEST(quick_select_largest);
    RUN_TEST(quick_select_median);
    RUN_TEST(quick_select_invalid);
    RUN_TEST(quick_select_sorted);

    printf("\n=== All divide & conquer tests passed! ===\n");
    return 0;
}
