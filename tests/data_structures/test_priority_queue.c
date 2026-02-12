/**
 * @file test_priority_queue.c
 * @brief Testes unitários para Priority Queue
 *
 * Testa operações de fila de prioridade (min e max) implementada sobre heap.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/priority_queue.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// TESTES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

TEST(create_destroy_min) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);
    ASSERT_NOT_NULL(pq);
    ASSERT_TRUE(pq_is_empty(pq));
    ASSERT_EQ(pq_size(pq), 0);
    pq_destroy(pq);
}

TEST(create_destroy_max) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MAX, compare_int, NULL);
    ASSERT_NOT_NULL(pq);
    ASSERT_TRUE(pq_is_empty(pq));
    ASSERT_EQ(pq_size(pq), 0);
    pq_destroy(pq);
}

// ============================================================================
// TESTES DE INSERT E EXTRACT
// ============================================================================

TEST(insert_extract_min_order) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    int values[] = {30, 10, 50, 20, 40};
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(pq_insert(pq, &values[i]), DS_SUCCESS);
    }

    ASSERT_EQ(pq_size(pq), 5);

    int output;
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 10);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 20);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 30);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 40);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 50);

    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

TEST(insert_extract_max_order) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MAX, compare_int, NULL);

    int values[] = {30, 10, 50, 20, 40};
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(pq_insert(pq, &values[i]), DS_SUCCESS);
    }

    ASSERT_EQ(pq_size(pq), 5);

    int output;
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 50);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 40);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 30);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 20);
    ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 10);

    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

// ============================================================================
// TESTES DE PEEK
// ============================================================================

TEST(peek_min) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    int val1 = 50, val2 = 10, val3 = 30;
    pq_insert(pq, &val1);
    pq_insert(pq, &val2);
    pq_insert(pq, &val3);

    int output;
    ASSERT_EQ(pq_peek(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 10);
    ASSERT_EQ(pq_size(pq), 3);

    pq_destroy(pq);
}

TEST(peek_max) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MAX, compare_int, NULL);

    int val1 = 10, val2 = 50, val3 = 30;
    pq_insert(pq, &val1);
    pq_insert(pq, &val2);
    pq_insert(pq, &val3);

    int output;
    ASSERT_EQ(pq_peek(pq, &output), DS_SUCCESS);
    ASSERT_EQ(output, 50);
    ASSERT_EQ(pq_size(pq), 3);

    pq_destroy(pq);
}

// ============================================================================
// TESTES DE OPERAÇÕES EM FILA VAZIA
// ============================================================================

TEST(empty_operations) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    int output;
    ASSERT_EQ(pq_extract(pq, &output), DS_ERROR_EMPTY);
    ASSERT_EQ(pq_peek(pq, &output), DS_ERROR_EMPTY);
    ASSERT_TRUE(pq_is_empty(pq));
    ASSERT_EQ(pq_size(pq), 0);

    pq_destroy(pq);
}

// ============================================================================
// TESTES DE CLEAR
// ============================================================================

TEST(clear) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    for (int i = 0; i < 10; i++) {
        pq_insert(pq, &i);
    }

    ASSERT_EQ(pq_size(pq), 10);

    pq_clear(pq);

    ASSERT_TRUE(pq_is_empty(pq));
    ASSERT_EQ(pq_size(pq), 0);

    int val = 42;
    ASSERT_EQ(pq_insert(pq, &val), DS_SUCCESS);
    ASSERT_EQ(pq_size(pq), 1);

    pq_destroy(pq);
}

// ============================================================================
// TESTE DE STRESS
// ============================================================================

TEST(stress_test) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    for (int i = 100; i > 0; i--) {
        ASSERT_EQ(pq_insert(pq, &i), DS_SUCCESS);
    }

    ASSERT_EQ(pq_size(pq), 100);

    int prev = -1;
    for (int i = 0; i < 100; i++) {
        int output;
        ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
        ASSERT_TRUE(output > prev);
        prev = output;
    }

    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

TEST(stress_test_max) {
    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MAX, compare_int, NULL);

    for (int i = 1; i <= 100; i++) {
        ASSERT_EQ(pq_insert(pq, &i), DS_SUCCESS);
    }

    ASSERT_EQ(pq_size(pq), 100);

    int prev = 101;
    for (int i = 0; i < 100; i++) {
        int output;
        ASSERT_EQ(pq_extract(pq, &output), DS_SUCCESS);
        ASSERT_TRUE(output < prev);
        prev = output;
    }

    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

// ============================================================================
// TESTES DE NULL POINTER
// ============================================================================

TEST(null_pointer_checks) {
    int val = 42;
    int output;

    ASSERT_NULL(pq_create(0, 16, PQ_MIN, compare_int, NULL));
    ASSERT_NULL(pq_create(sizeof(int), 16, PQ_MIN, NULL, NULL));

    ASSERT_EQ(pq_insert(NULL, &val), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(pq_extract(NULL, &output), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(pq_peek(NULL, &output), DS_ERROR_NULL_POINTER);

    ASSERT_TRUE(pq_is_empty(NULL));
    ASSERT_EQ(pq_size(NULL), 0);

    PriorityQueue *pq = pq_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);
    ASSERT_EQ(pq_insert(pq, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(pq_peek(pq, NULL), DS_ERROR_NULL_POINTER);
    pq_destroy(pq);
}

// ============================================================================
// TESTES COM DOUBLES
// ============================================================================

TEST(doubles_min) {
    PriorityQueue *pq = pq_create(sizeof(double), 16, PQ_MIN, compare_double, NULL);

    double values[] = {3.14, 1.41, 2.72, 0.57, 1.73};
    for (int i = 0; i < 5; i++) {
        pq_insert(pq, &values[i]);
    }

    double output;
    pq_extract(pq, &output);
    ASSERT_TRUE(output < 0.58);

    pq_extract(pq, &output);
    ASSERT_TRUE(output < 1.42);

    pq_destroy(pq);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE PRIORITY QUEUE\n");
    printf("========================================\n\n");

    printf("Criacao e Destruicao:\n");
    RUN_TEST(create_destroy_min);
    RUN_TEST(create_destroy_max);

    printf("\nInsert e Extract:\n");
    RUN_TEST(insert_extract_min_order);
    RUN_TEST(insert_extract_max_order);

    printf("\nPeek:\n");
    RUN_TEST(peek_min);
    RUN_TEST(peek_max);

    printf("\nOperacoes em Fila Vazia:\n");
    RUN_TEST(empty_operations);

    printf("\nClear:\n");
    RUN_TEST(clear);

    printf("\nStress Test:\n");
    RUN_TEST(stress_test);
    RUN_TEST(stress_test_max);

    printf("\nNull Pointer:\n");
    RUN_TEST(null_pointer_checks);

    printf("\nDoubles:\n");
    RUN_TEST(doubles_min);

    printf("\n============================================\n");
    printf("  TODOS OS TESTES PASSARAM! (12 testes)\n");
    printf("============================================\n");

    return 0;
}
