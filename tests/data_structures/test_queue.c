/**
 * @file test_queue.c
 * @brief Testes unitários para Queue (FIFO)
 *
 * Testa ambas implementações: QUEUE_ARRAY (circular buffer) e QUEUE_LINKED
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/queue.h"
#include "data_structures/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// MACROS DE TESTE
// ============================================================================

#define TEST(name) \
    void test_##name(); \
    printf("  %-50s", #name); \
    test_##name(); \
    printf("[✓ PASS]\n"); \
    void test_##name()

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("\n    ✗ Assertion failed: %s\n", #condition); \
            printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NE(a, b) ASSERT((a) != (b))
#define ASSERT_TRUE(cond) ASSERT(cond)
#define ASSERT_FALSE(cond) ASSERT(!(cond))
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)

// ============================================================================
// TESTES PARA QUEUE_ARRAY
// ============================================================================

TEST(queue_array_create_destroy) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);
    ASSERT_NOT_NULL(q);
    ASSERT_TRUE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 0);
    ASSERT_EQ(queue_capacity(q), 10);
    queue_destroy(q);
}

TEST(queue_array_enqueue_dequeue_single) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);

    int value = 42;
    ASSERT_EQ(queue_enqueue(q, &value), DS_SUCCESS);
    ASSERT_FALSE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 1);

    int output;
    ASSERT_EQ(queue_dequeue(q, &output), DS_SUCCESS);
    ASSERT_EQ(output, 42);
    ASSERT_TRUE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 0);

    queue_destroy(q);
}

TEST(queue_array_enqueue_multiple) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);

    // Enfileirar 5 elementos
    for (int i = 1; i <= 5; i++) {
        ASSERT_EQ(queue_enqueue(q, &i), DS_SUCCESS);
    }

    ASSERT_EQ(queue_size(q), 5);

    // Desenfileirar e verificar ordem FIFO
    for (int i = 1; i <= 5; i++) {
        int output;
        ASSERT_EQ(queue_dequeue(q, &output), DS_SUCCESS);
        ASSERT_EQ(output, i);
    }

    ASSERT_TRUE(queue_is_empty(q));
    queue_destroy(q);
}

TEST(queue_array_circular_buffer) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 4, NULL);

    // Enfileirar 4 elementos (enche a fila)
    for (int i = 1; i <= 4; i++) {
        queue_enqueue(q, &i);
    }

    // Desenfileirar 2 (head avança)
    int dummy;
    queue_dequeue(q, &dummy);
    queue_dequeue(q, &dummy);

    // Enfileirar 2 novos (tail vai dar wrap-around)
    int val5 = 5, val6 = 6;
    queue_enqueue(q, &val5);
    queue_enqueue(q, &val6);

    // Verificar ordem: deve ter 3, 4, 5, 6
    int output;
    queue_dequeue(q, &output);
    ASSERT_EQ(output, 3);
    queue_dequeue(q, &output);
    ASSERT_EQ(output, 4);
    queue_dequeue(q, &output);
    ASSERT_EQ(output, 5);
    queue_dequeue(q, &output);
    ASSERT_EQ(output, 6);

    ASSERT_TRUE(queue_is_empty(q));
    queue_destroy(q);
}

TEST(queue_array_resize) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 2, NULL);

    // Enfileirar mais que a capacidade inicial
    for (int i = 1; i <= 10; i++) {
        ASSERT_EQ(queue_enqueue(q, &i), DS_SUCCESS);
    }

    ASSERT_EQ(queue_size(q), 10);
    ASSERT_TRUE(queue_capacity(q) >= 10);  // Deve ter crescido

    // Verificar ordem
    for (int i = 1; i <= 10; i++) {
        int output;
        queue_dequeue(q, &output);
        ASSERT_EQ(output, i);
    }

    queue_destroy(q);
}

TEST(queue_array_front) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);

    int val1 = 10, val2 = 20, val3 = 30;
    queue_enqueue(q, &val1);
    queue_enqueue(q, &val2);
    queue_enqueue(q, &val3);

    int output;
    ASSERT_EQ(queue_front(q, &output), DS_SUCCESS);
    ASSERT_EQ(output, 10);  // Primeiro inserido

    // Front não remove
    ASSERT_EQ(queue_size(q), 3);

    queue_destroy(q);
}

TEST(queue_array_clear) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);

    for (int i = 1; i <= 5; i++) {
        queue_enqueue(q, &i);
    }

    ASSERT_EQ(queue_size(q), 5);

    queue_clear(q);

    ASSERT_TRUE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 0);

    // Deve poder usar novamente
    int val = 99;
    queue_enqueue(q, &val);
    int output;
    queue_dequeue(q, &output);
    ASSERT_EQ(output, 99);

    queue_destroy(q);
}

TEST(queue_array_empty_operations) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);

    int output;

    // Dequeue em fila vazia deve falhar
    ASSERT_EQ(queue_dequeue(q, &output), DS_ERROR_EMPTY);

    // Front em fila vazia deve falhar
    ASSERT_EQ(queue_front(q, &output), DS_ERROR_EMPTY);

    queue_destroy(q);
}

TEST(queue_array_stress_test) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 4, NULL);

    // Teste de stress: muitas operações
    const int N = 1000;

    for (int i = 0; i < N; i++) {
        queue_enqueue(q, &i);
    }

    ASSERT_EQ(queue_size(q), N);

    for (int i = 0; i < N; i++) {
        int output;
        queue_dequeue(q, &output);
        ASSERT_EQ(output, i);
    }

    ASSERT_TRUE(queue_is_empty(q));
    queue_destroy(q);
}

// ============================================================================
// TESTES PARA QUEUE_LINKED
// ============================================================================

TEST(queue_linked_create_destroy) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);
    ASSERT_NOT_NULL(q);
    ASSERT_TRUE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 0);
    ASSERT_EQ(queue_capacity(q), 0);  // Linked não tem capacidade fixa
    queue_destroy(q);
}

TEST(queue_linked_enqueue_dequeue_single) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    int value = 42;
    ASSERT_EQ(queue_enqueue(q, &value), DS_SUCCESS);
    ASSERT_FALSE(queue_is_empty(q));
    ASSERT_EQ(queue_size(q), 1);

    int output;
    ASSERT_EQ(queue_dequeue(q, &output), DS_SUCCESS);
    ASSERT_EQ(output, 42);
    ASSERT_TRUE(queue_is_empty(q));

    queue_destroy(q);
}

TEST(queue_linked_enqueue_multiple) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        ASSERT_EQ(queue_enqueue(q, &i), DS_SUCCESS);
    }

    ASSERT_EQ(queue_size(q), 5);

    for (int i = 1; i <= 5; i++) {
        int output;
        queue_dequeue(q, &output);
        ASSERT_EQ(output, i);
    }

    ASSERT_TRUE(queue_is_empty(q));
    queue_destroy(q);
}

TEST(queue_linked_front) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    int val1 = 10, val2 = 20;
    queue_enqueue(q, &val1);
    queue_enqueue(q, &val2);

    int output;
    queue_front(q, &output);
    ASSERT_EQ(output, 10);
    ASSERT_EQ(queue_size(q), 2);  // Front não remove

    queue_destroy(q);
}

TEST(queue_linked_clear) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        queue_enqueue(q, &i);
    }

    queue_clear(q);
    ASSERT_TRUE(queue_is_empty(q));

    queue_destroy(q);
}

TEST(queue_linked_unlimited_size) {
    Queue *q = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    // Linked não tem limite de capacidade
    const int LARGE = 10000;

    for (int i = 0; i < LARGE; i++) {
        ASSERT_EQ(queue_enqueue(q, &i), DS_SUCCESS);
    }

    ASSERT_EQ(queue_size(q), LARGE);

    for (int i = 0; i < LARGE; i++) {
        int output;
        queue_dequeue(q, &output);
        ASSERT_EQ(output, i);
    }

    queue_destroy(q);
}

// ============================================================================
// TESTES COM STRINGS
// ============================================================================

TEST(queue_array_strings) {
    Queue *q = queue_create(sizeof(char*), QUEUE_ARRAY, 5, NULL);

    const char *str1 = "Hello";
    const char *str2 = "World";
    const char *str3 = "Queue";

    queue_enqueue(q, &str1);
    queue_enqueue(q, &str2);
    queue_enqueue(q, &str3);

    char *output;
    queue_dequeue(q, &output);
    ASSERT_EQ(strcmp(output, "Hello"), 0);

    queue_dequeue(q, &output);
    ASSERT_EQ(strcmp(output, "World"), 0);

    queue_dequeue(q, &output);
    ASSERT_EQ(strcmp(output, "Queue"), 0);

    queue_destroy(q);
}

TEST(queue_linked_strings) {
    Queue *q = queue_create(sizeof(char*), QUEUE_LINKED, 0, NULL);

    const char *str1 = "Linked";
    const char *str2 = "Queue";

    queue_enqueue(q, &str1);
    queue_enqueue(q, &str2);

    char *output;
    queue_front(q, &output);
    ASSERT_EQ(strcmp(output, "Linked"), 0);

    queue_dequeue(q, &output);
    queue_dequeue(q, &output);
    ASSERT_EQ(strcmp(output, "Queue"), 0);

    queue_destroy(q);
}

// ============================================================================
// TESTES COM STRUCTS
// ============================================================================

typedef struct {
    int id;
    double value;
} TestStruct;

TEST(queue_array_structs) {
    Queue *q = queue_create(sizeof(TestStruct), QUEUE_ARRAY, 3, NULL);

    TestStruct s1 = {1, 1.5};
    TestStruct s2 = {2, 2.5};
    TestStruct s3 = {3, 3.5};

    queue_enqueue(q, &s1);
    queue_enqueue(q, &s2);
    queue_enqueue(q, &s3);

    TestStruct output;
    queue_dequeue(q, &output);
    ASSERT_EQ(output.id, 1);
    ASSERT_EQ(output.value, 1.5);

    queue_dequeue(q, &output);
    ASSERT_EQ(output.id, 2);

    queue_destroy(q);
}

// ============================================================================
// TESTES DE ERRO
// ============================================================================

TEST(queue_null_pointer_checks) {
    int value = 42;

    // Criar com element_size = 0 deve falhar
    Queue *q1 = queue_create(0, QUEUE_ARRAY, 10, NULL);
    ASSERT_NULL(q1);

    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);

    // Enqueue com NULL deve falhar
    ASSERT_EQ(queue_enqueue(NULL, &value), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(queue_enqueue(q, NULL), DS_ERROR_NULL_POINTER);

    // Front com NULL deve falhar
    int output;
    ASSERT_EQ(queue_front(NULL, &output), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(queue_front(q, NULL), DS_ERROR_NULL_POINTER);

    // Dequeue com fila NULL deve falhar
    ASSERT_EQ(queue_dequeue(NULL, &output), DS_ERROR_NULL_POINTER);

    queue_destroy(q);
}

// ============================================================================
// TESTE VISUAL (PRINT)
// ============================================================================

TEST(queue_print_visual) {
    printf("\n    Visual print test:\n");

    Queue *q_array = queue_create(sizeof(int), QUEUE_ARRAY, 5, NULL);
    Queue *q_linked = queue_create(sizeof(int), QUEUE_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        queue_enqueue(q_array, &i);
        queue_enqueue(q_linked, &i);
    }

    printf("    ARRAY:  ");
    queue_print(q_array, print_int);

    printf("    LINKED: ");
    queue_print(q_linked, print_int);

    queue_destroy(q_array);
    queue_destroy(q_linked);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE QUEUE (FIFO)\n");
    printf("========================================\n\n");

    printf("QUEUE_ARRAY (Circular Buffer):\n");
    test_queue_array_create_destroy();
    test_queue_array_enqueue_dequeue_single();
    test_queue_array_enqueue_multiple();
    test_queue_array_circular_buffer();
    test_queue_array_resize();
    test_queue_array_front();
    test_queue_array_clear();
    test_queue_array_empty_operations();
    test_queue_array_stress_test();

    printf("\nQUEUE_LINKED:\n");
    test_queue_linked_create_destroy();
    test_queue_linked_enqueue_dequeue_single();
    test_queue_linked_enqueue_multiple();
    test_queue_linked_front();
    test_queue_linked_clear();
    test_queue_linked_unlimited_size();

    printf("\nTestes com Strings:\n");
    test_queue_array_strings();
    test_queue_linked_strings();

    printf("\nTestes com Structs:\n");
    test_queue_array_structs();

    printf("\nTestes de Erro:\n");
    test_queue_null_pointer_checks();

    printf("\nPrint Visual:\n");
    test_queue_print_visual();

    printf("\n========================================\n");
    printf("  TODOS OS TESTES PASSARAM! ✓\n");
    printf("  Total: 24 testes\n");
    printf("========================================\n");

    return 0;
}
