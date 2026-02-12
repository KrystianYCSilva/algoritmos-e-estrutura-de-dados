/**
 * @file test_heap.c
 * @brief Testes para Binary Heap (Min-Heap e Max-Heap)
 *
 * Valida:
 * - Criacao e destruicao (min e max)
 * - Insercao e extracao com ordenacao correta
 * - Peek sem remocao
 * - Build-Heap a partir de array
 * - Heapsort in-place
 * - Clear, to_array, operacoes em heap vazio
 * - Robustez com ponteiros nulos
 * - Stress test com 1000 elementos
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "data_structures/heap.h"
#include "data_structures/common.h"
#include "../test_macros.h"
#include <string.h>

// ============================================================================
// TESTES: CRIACAO E DESTRUICAO
// ============================================================================

TEST(create_destroy_min) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);
    ASSERT_TRUE(heap_is_empty(heap));
    ASSERT_EQ(heap_size(heap), 0);
    heap_destroy(heap);
}

TEST(create_destroy_max) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MAX, compare_int, NULL);
    ASSERT_NOT_NULL(heap);
    ASSERT_TRUE(heap_is_empty(heap));
    ASSERT_EQ(heap_size(heap), 0);
    heap_destroy(heap);
}

// ============================================================================
// TESTES: INSERCAO
// ============================================================================

TEST(insert_single) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int val = 42;
    DataStructureError err = heap_insert(heap, &val);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(heap_size(heap), 1);
    ASSERT_FALSE(heap_is_empty(heap));

    int out;
    err = heap_peek(heap, &out);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(out, 42);

    heap_destroy(heap);
}

TEST(insert_multiple) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    for (size_t i = 0; i < 8; i++) {
        DataStructureError err = heap_insert(heap, &values[i]);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    ASSERT_EQ(heap_size(heap), 8);
    ASSERT_FALSE(heap_is_empty(heap));

    heap_destroy(heap);
}

// ============================================================================
// TESTES: EXTRACAO EM ORDEM
// ============================================================================

TEST(extract_min_order) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    for (size_t i = 0; i < 8; i++) {
        heap_insert(heap, &values[i]);
    }

    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (size_t i = 0; i < 8; i++) {
        int out;
        DataStructureError err = heap_extract(heap, &out);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(out, expected[i]);
    }

    ASSERT_TRUE(heap_is_empty(heap));
    ASSERT_EQ(heap_size(heap), 0);

    heap_destroy(heap);
}

TEST(extract_max_order) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MAX, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    for (size_t i = 0; i < 8; i++) {
        heap_insert(heap, &values[i]);
    }

    int expected[] = {8, 7, 6, 5, 4, 3, 2, 1};
    for (size_t i = 0; i < 8; i++) {
        int out;
        DataStructureError err = heap_extract(heap, &out);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(out, expected[i]);
    }

    ASSERT_TRUE(heap_is_empty(heap));

    heap_destroy(heap);
}

// ============================================================================
// TESTES: PEEK
// ============================================================================

TEST(peek_min) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4};
    for (size_t i = 0; i < 5; i++) {
        heap_insert(heap, &values[i]);
    }

    int out;
    DataStructureError err = heap_peek(heap, &out);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(out, 1);
    ASSERT_EQ(heap_size(heap), 5);

    heap_destroy(heap);
}

TEST(peek_max) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MAX, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4};
    for (size_t i = 0; i < 5; i++) {
        heap_insert(heap, &values[i]);
    }

    int out;
    DataStructureError err = heap_peek(heap, &out);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(out, 8);
    ASSERT_EQ(heap_size(heap), 5);

    heap_destroy(heap);
}

// ============================================================================
// TESTES: BUILD-HEAP
// ============================================================================

TEST(build_from_array_min) {
    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    Heap *heap = heap_build(sizeof(int), values, 8, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(heap_size(heap), 8);

    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (size_t i = 0; i < 8; i++) {
        int out;
        DataStructureError err = heap_extract(heap, &out);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(out, expected[i]);
    }

    heap_destroy(heap);
}

TEST(build_from_array_max) {
    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    Heap *heap = heap_build(sizeof(int), values, 8, HEAP_MAX, compare_int, NULL);
    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(heap_size(heap), 8);

    int expected[] = {8, 7, 6, 5, 4, 3, 2, 1};
    for (size_t i = 0; i < 8; i++) {
        int out;
        DataStructureError err = heap_extract(heap, &out);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(out, expected[i]);
    }

    heap_destroy(heap);
}

// ============================================================================
// TESTES: HEAPSORT
// ============================================================================

TEST(heap_sort_test) {
    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};

    heap_sort(values, sizeof(int), 8, compare_int);

    for (size_t i = 0; i < 8; i++) {
        ASSERT_EQ(values[i], expected[i]);
    }
}

// ============================================================================
// TESTES: CLEAR
// ============================================================================

TEST(clear_heap) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4};
    for (size_t i = 0; i < 5; i++) {
        heap_insert(heap, &values[i]);
    }

    ASSERT_EQ(heap_size(heap), 5);
    ASSERT_FALSE(heap_is_empty(heap));

    heap_clear(heap);

    ASSERT_EQ(heap_size(heap), 0);
    ASSERT_TRUE(heap_is_empty(heap));

    int val = 99;
    DataStructureError err = heap_insert(heap, &val);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(heap_size(heap), 1);

    int out;
    err = heap_peek(heap, &out);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(out, 99);

    heap_destroy(heap);
}

// ============================================================================
// TESTES: TO_ARRAY
// ============================================================================

TEST(to_array_sorted) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    for (size_t i = 0; i < 8; i++) {
        heap_insert(heap, &values[i]);
    }

    void *array = NULL;
    size_t size = 0;
    DataStructureError err = heap_to_array(heap, &array, &size);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(size, 8);
    ASSERT_NOT_NULL(array);

    int *arr = (int *)array;
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (size_t i = 0; i < 8; i++) {
        ASSERT_EQ(arr[i], expected[i]);
    }

    ASSERT_EQ(heap_size(heap), 8);

    free(array);
    heap_destroy(heap);
}

// ============================================================================
// TESTES: STRESS TEST
// ============================================================================

TEST(stress_test) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    for (int i = 0; i < 1000; i++) {
        int val = (i * 7 + 13) % 1000;
        DataStructureError err = heap_insert(heap, &val);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    ASSERT_EQ(heap_size(heap), 1000);

    int prev;
    DataStructureError err = heap_extract(heap, &prev);
    ASSERT_EQ(err, DS_SUCCESS);

    for (int i = 1; i < 1000; i++) {
        int curr;
        err = heap_extract(heap, &curr);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_TRUE(curr >= prev);
        prev = curr;
    }

    ASSERT_TRUE(heap_is_empty(heap));

    heap_destroy(heap);
}

// ============================================================================
// TESTES: OPERACOES EM HEAP VAZIO
// ============================================================================

TEST(empty_operations) {
    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    int out;
    DataStructureError err = heap_extract(heap, &out);
    ASSERT_EQ(err, DS_ERROR_EMPTY);

    err = heap_peek(heap, &out);
    ASSERT_EQ(err, DS_ERROR_EMPTY);

    ASSERT_TRUE(heap_is_empty(heap));
    ASSERT_EQ(heap_size(heap), 0);

    heap_destroy(heap);
}

// ============================================================================
// TESTES: NULL POINTER CHECKS
// ============================================================================

TEST(null_pointer_checks) {
    int val = 42;
    int out;

    DataStructureError err = heap_insert(NULL, &val);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    err = heap_extract(NULL, &out);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    err = heap_peek(NULL, &out);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    ASSERT_TRUE(heap_is_empty(NULL));
    ASSERT_EQ(heap_size(NULL), 0);
    ASSERT_EQ(heap_capacity(NULL), 0);

    Heap *heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(heap);

    err = heap_insert(heap, NULL);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    err = heap_peek(heap, NULL);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    Heap *bad = heap_create(0, 16, HEAP_MIN, compare_int, NULL);
    ASSERT_NULL(bad);

    bad = heap_create(sizeof(int), 16, HEAP_MIN, NULL, NULL);
    ASSERT_NULL(bad);

    bad = heap_build(sizeof(int), NULL, 8, HEAP_MIN, compare_int, NULL);
    ASSERT_NULL(bad);

    bad = heap_build(0, &val, 1, HEAP_MIN, compare_int, NULL);
    ASSERT_NULL(bad);

    bad = heap_build(sizeof(int), &val, 1, HEAP_MIN, NULL, NULL);
    ASSERT_NULL(bad);

    heap_heapify(NULL, 0);

    heap_clear(NULL);

    heap_destroy(NULL);

    void *arr = NULL;
    size_t sz = 0;
    err = heap_to_array(NULL, &arr, &sz);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    err = heap_to_array(heap, NULL, &sz);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    err = heap_to_array(heap, &arr, NULL);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    heap_sort(NULL, sizeof(int), 8, compare_int);
    heap_sort(&val, sizeof(int), 8, NULL);

    heap_print(NULL, print_int);
    heap_print(heap, NULL);

    heap_destroy(heap);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(print_visual) {
    printf("\n");

    Heap *min_heap = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
    int values[] = {5, 3, 8, 1, 4, 2, 7, 6};
    for (size_t i = 0; i < 8; i++) {
        heap_insert(min_heap, &values[i]);
    }

    printf("    Min-Heap: ");
    heap_print(min_heap, print_int);

    Heap *max_heap = heap_create(sizeof(int), 16, HEAP_MAX, compare_int, NULL);
    for (size_t i = 0; i < 8; i++) {
        heap_insert(max_heap, &values[i]);
    }

    printf("    Max-Heap: ");
    heap_print(max_heap, print_int);

    heap_destroy(min_heap);
    heap_destroy(max_heap);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("============================================\n");
    printf("  TESTES: HEAP (Binary Heap)\n");
    printf("============================================\n\n");

    printf("Criacao e Destruicao:\n");
    RUN_TEST(create_destroy_min);
    RUN_TEST(create_destroy_max);

    printf("\nInsercao:\n");
    RUN_TEST(insert_single);
    RUN_TEST(insert_multiple);

    printf("\nExtracao em Ordem:\n");
    RUN_TEST(extract_min_order);
    RUN_TEST(extract_max_order);

    printf("\nPeek:\n");
    RUN_TEST(peek_min);
    RUN_TEST(peek_max);

    printf("\nBuild-Heap:\n");
    RUN_TEST(build_from_array_min);
    RUN_TEST(build_from_array_max);

    printf("\nHeapsort:\n");
    RUN_TEST(heap_sort_test);

    printf("\nClear:\n");
    RUN_TEST(clear_heap);

    printf("\nTo Array:\n");
    RUN_TEST(to_array_sorted);

    printf("\nStress Test:\n");
    RUN_TEST(stress_test);

    printf("\nOperacoes em Heap Vazio:\n");
    RUN_TEST(empty_operations);

    printf("\nNull Pointer Checks:\n");
    RUN_TEST(null_pointer_checks);

    printf("\nTeste Visual:\n");
    RUN_TEST(print_visual);

    printf("\n============================================\n");
    printf("  ✅ TODOS OS TESTES PASSARAM! (17 testes)\n");
    printf("============================================\n\n");

    return 0;
}
