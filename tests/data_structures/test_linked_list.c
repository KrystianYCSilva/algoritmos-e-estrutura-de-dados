/**
 * @file test_linked_list.c
 * @brief Testes unitários para LinkedList
 *
 * Testa as três variantes: SINGLY, DOUBLY, CIRCULAR
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/linked_list.h"
#include "data_structures/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define ASSERT_TRUE(cond) ASSERT(cond)
#define ASSERT_FALSE(cond) ASSERT(!(cond))
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)

// ============================================================================
// TESTES BÁSICOS - DOUBLY LINKED LIST
// ============================================================================

TEST(doubly_create_destroy) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(list_is_empty(list));
    ASSERT_EQ(list_size(list), 0);
    list_destroy(list);
}

TEST(doubly_push_front_back) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int val1 = 10, val2 = 20, val3 = 30;

    list_push_back(list, &val1);   // [10]
    list_push_back(list, &val2);   // [10, 20]
    list_push_front(list, &val3);  // [30, 10, 20]

    ASSERT_EQ(list_size(list), 3);

    int output;
    list_get(list, 0, &output);
    ASSERT_EQ(output, 30);

    list_get(list, 1, &output);
    ASSERT_EQ(output, 10);

    list_get(list, 2, &output);
    ASSERT_EQ(output, 20);

    list_destroy(list);
}

TEST(doubly_pop_front_back) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);  // [1, 2, 3, 4, 5]
    }

    int output;

    list_pop_front(list, &output);  // Remove 1
    ASSERT_EQ(output, 1);
    ASSERT_EQ(list_size(list), 4);

    list_pop_back(list, &output);   // Remove 5
    ASSERT_EQ(output, 5);
    ASSERT_EQ(list_size(list), 3);

    // Deve ter [2, 3, 4]
    list_get(list, 0, &output);
    ASSERT_EQ(output, 2);

    list_get(list, 2, &output);
    ASSERT_EQ(output, 4);

    list_destroy(list);
}

TEST(doubly_insert_at) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int val1 = 10, val2 = 30, val3 = 20;

    list_push_back(list, &val1);    // [10]
    list_push_back(list, &val2);    // [10, 30]
    list_insert_at(list, 1, &val3); // [10, 20, 30]

    ASSERT_EQ(list_size(list), 3);

    int output;
    list_get(list, 1, &output);
    ASSERT_EQ(output, 20);

    list_destroy(list);
}

TEST(doubly_remove_at) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);  // [1, 2, 3, 4, 5]
    }

    int output;
    list_remove_at(list, 2, &output);  // Remove 3
    ASSERT_EQ(output, 3);
    ASSERT_EQ(list_size(list), 4);

    // Deve ter [1, 2, 4, 5]
    list_get(list, 2, &output);
    ASSERT_EQ(output, 4);

    list_destroy(list);
}

TEST(doubly_find) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int val1 = 10, val2 = 20, val3 = 30;
    list_push_back(list, &val1);
    list_push_back(list, &val2);
    list_push_back(list, &val3);

    int search = 20;
    ListNode *node = list_find(list, &search, compare_int);
    ASSERT_NOT_NULL(node);

    int *data = (int*)list_node_data(node);
    ASSERT_EQ(*data, 20);

    list_destroy(list);
}

TEST(doubly_index_of) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 10; i <= 50; i += 10) {
        list_push_back(list, &i);  // [10, 20, 30, 40, 50]
    }

    int search = 30;
    ssize_t index = list_index_of(list, &search, compare_int);
    ASSERT_EQ(index, 2);

    int not_found = 99;
    index = list_index_of(list, &not_found, compare_int);
    ASSERT_EQ(index, -1);

    list_destroy(list);
}

TEST(doubly_remove_value) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);
    }

    int to_remove = 3;
    DataStructureError err = list_remove(list, &to_remove, compare_int);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(list_size(list), 4);

    // Verificar que 3 não existe mais
    ssize_t index = list_index_of(list, &to_remove, compare_int);
    ASSERT_EQ(index, -1);

    list_destroy(list);
}

TEST(doubly_clear) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 10; i++) {
        list_push_back(list, &i);
    }

    ASSERT_EQ(list_size(list), 10);

    list_clear(list);

    ASSERT_TRUE(list_is_empty(list));
    ASSERT_EQ(list_size(list), 0);

    list_destroy(list);
}

TEST(doubly_reverse) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);  // [1, 2, 3, 4, 5]
    }

    list_reverse(list);

    // Deve ter [5, 4, 3, 2, 1]
    int output;
    for (int i = 0; i < 5; i++) {
        list_get(list, i, &output);
        ASSERT_EQ(output, 5 - i);
    }

    list_destroy(list);
}

TEST(doubly_iterators) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);
    }

    // Iterar para frente
    int expected = 1;
    ListNode *node = list_begin(list);
    while (node != NULL) {
        int *data = (int*)list_node_data(node);
        ASSERT_EQ(*data, expected);
        expected++;
        node = list_next(node);
    }

    // Iterar para trás
    expected = 5;
    node = list_end(list);
    while (node != NULL) {
        int *data = (int*)list_node_data(node);
        ASSERT_EQ(*data, expected);
        expected--;
        node = list_prev(node);
        if (expected == 0) break;  // Evitar loop infinito
    }

    list_destroy(list);
}

// ============================================================================
// TESTES - SINGLY LINKED LIST
// ============================================================================

TEST(singly_create_destroy) {
    LinkedList *list = list_create(sizeof(int), LIST_SINGLY, NULL);
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(list_is_empty(list));
    list_destroy(list);
}

TEST(singly_push_front_back) {
    LinkedList *list = list_create(sizeof(int), LIST_SINGLY, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);
    }

    ASSERT_EQ(list_size(list), 5);

    int output;
    list_get(list, 0, &output);
    ASSERT_EQ(output, 1);

    list_get(list, 4, &output);
    ASSERT_EQ(output, 5);

    list_destroy(list);
}

TEST(singly_pop_operations) {
    LinkedList *list = list_create(sizeof(int), LIST_SINGLY, NULL);

    for (int i = 1; i <= 3; i++) {
        list_push_back(list, &i);
    }

    int output;

    // Pop front é O(1) mesmo para singly
    list_pop_front(list, &output);
    ASSERT_EQ(output, 1);
    ASSERT_EQ(list_size(list), 2);

    // Pop back é O(n) para singly (precisa percorrer)
    list_pop_back(list, &output);
    ASSERT_EQ(output, 3);
    ASSERT_EQ(list_size(list), 1);

    list_destroy(list);
}

// ============================================================================
// TESTES - CIRCULAR LINKED LIST
// ============================================================================

TEST(circular_create_destroy) {
    LinkedList *list = list_create(sizeof(int), LIST_CIRCULAR, NULL);
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(list_is_empty(list));
    list_destroy(list);
}

TEST(circular_push_operations) {
    LinkedList *list = list_create(sizeof(int), LIST_CIRCULAR, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(list, &i);
    }

    ASSERT_EQ(list_size(list), 5);

    // Verificar que é circular: tail->next aponta para head
    ListNode *tail = list_end(list);
    ListNode *next_of_tail = list_next(tail);
    ListNode *head = list_begin(list);

    ASSERT_EQ(next_of_tail, head);  // Circular!

    list_destroy(list);
}

TEST(circular_iteration) {
    LinkedList *list = list_create(sizeof(int), LIST_CIRCULAR, NULL);

    for (int i = 1; i <= 3; i++) {
        list_push_back(list, &i);
    }

    // Iterar limitado (evitar loop infinito)
    int count = 0;
    ListNode *node = list_begin(list);
    while (count < 6) {  // Dar 2 voltas
        int *data = (int*)list_node_data(node);
        int expected = (count % 3) + 1;
        ASSERT_EQ(*data, expected);
        node = list_next(node);
        count++;
    }

    list_destroy(list);
}

// ============================================================================
// TESTES COM STRINGS
// ============================================================================

TEST(doubly_strings) {
    LinkedList *list = list_create(sizeof(char*), LIST_DOUBLY, NULL);

    const char *str1 = "First";
    const char *str2 = "Second";
    const char *str3 = "Third";

    list_push_back(list, &str1);
    list_push_back(list, &str2);
    list_push_back(list, &str3);

    ASSERT_EQ(list_size(list), 3);

    char *output;
    list_get(list, 1, &output);
    ASSERT_EQ(strcmp(output, "Second"), 0);

    list_destroy(list);
}

// ============================================================================
// TESTES COM STRUCTS
// ============================================================================

typedef struct {
    int id;
    double value;
} TestStruct;

TEST(doubly_structs) {
    LinkedList *list = list_create(sizeof(TestStruct), LIST_DOUBLY, NULL);

    TestStruct s1 = {1, 1.5};
    TestStruct s2 = {2, 2.5};
    TestStruct s3 = {3, 3.5};

    list_push_back(list, &s1);
    list_push_back(list, &s2);
    list_push_back(list, &s3);

    TestStruct output;
    list_get(list, 1, &output);
    ASSERT_EQ(output.id, 2);
    ASSERT_EQ(output.value, 2.5);

    list_destroy(list);
}

// ============================================================================
// TESTES DE ERRO
// ============================================================================

TEST(error_null_pointers) {
    // Create com element_size = 0 deve falhar
    LinkedList *list1 = list_create(0, LIST_DOUBLY, NULL);
    ASSERT_NULL(list1);

    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);
    int value = 42;

    // Operações com NULL
    ASSERT_EQ(list_push_back(NULL, &value), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(list_push_back(list, NULL), DS_ERROR_NULL_POINTER);

    int output;
    ASSERT_EQ(list_get(NULL, 0, &output), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(list_get(list, 0, NULL), DS_ERROR_NULL_POINTER);

    list_destroy(list);
}

TEST(error_invalid_index) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int value = 42;
    list_push_back(list, &value);

    int output;

    // Índice fora do range
    ASSERT_EQ(list_get(list, 10, &output), DS_ERROR_INVALID_INDEX);
    ASSERT_EQ(list_remove_at(list, 10, &output), DS_ERROR_INVALID_INDEX);

    list_destroy(list);
}

TEST(error_empty_operations) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int output;

    // Pop em lista vazia
    ASSERT_EQ(list_pop_front(list, &output), DS_ERROR_EMPTY);
    ASSERT_EQ(list_pop_back(list, &output), DS_ERROR_EMPTY);

    list_destroy(list);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(print_visual) {
    printf("\n    Visual print test:\n");

    LinkedList *doubly = list_create(sizeof(int), LIST_DOUBLY, NULL);
    LinkedList *singly = list_create(sizeof(int), LIST_SINGLY, NULL);
    LinkedList *circular = list_create(sizeof(int), LIST_CIRCULAR, NULL);

    for (int i = 1; i <= 5; i++) {
        list_push_back(doubly, &i);
        list_push_back(singly, &i);
        list_push_back(circular, &i);
    }

    printf("    ");
    list_print(doubly, print_int);

    printf("    ");
    list_print(singly, print_int);

    printf("    ");
    list_print(circular, print_int);

    list_destroy(doubly);
    list_destroy(singly);
    list_destroy(circular);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE LINKED LIST\n");
    printf("========================================\n\n");

    printf("DOUBLY LINKED LIST:\n");
    test_doubly_create_destroy();
    test_doubly_push_front_back();
    test_doubly_pop_front_back();
    test_doubly_insert_at();
    test_doubly_remove_at();
    test_doubly_find();
    test_doubly_index_of();
    test_doubly_remove_value();
    test_doubly_clear();
    test_doubly_reverse();
    test_doubly_iterators();

    printf("\nSINGLY LINKED LIST:\n");
    test_singly_create_destroy();
    test_singly_push_front_back();
    test_singly_pop_operations();

    printf("\nCIRCULAR LINKED LIST:\n");
    test_circular_create_destroy();
    test_circular_push_operations();
    test_circular_iteration();

    printf("\nTestes com Strings:\n");
    test_doubly_strings();

    printf("\nTestes com Structs:\n");
    test_doubly_structs();

    printf("\nTestes de Erro:\n");
    test_error_null_pointers();
    test_error_invalid_index();
    test_error_empty_operations();

    printf("\nPrint Visual:\n");
    test_print_visual();

    printf("\n========================================\n");
    printf("  TODOS OS TESTES PASSARAM! ✓\n");
    printf("  Total: 24 testes\n");
    printf("========================================\n");

    return 0;
}
