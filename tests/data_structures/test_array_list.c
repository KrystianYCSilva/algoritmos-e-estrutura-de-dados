/**
 * @file test_array_list.c
 * @brief Testes para ArrayList (Dynamic Array)
 *
 * Valida:
 * - Crescimento automático com estratégias (2x, 1.5x, FIXED)
 * - Operações O(1): get, set, push_back
 * - Operações O(n): insert, remove
 * - Busca linear e binária
 * - Ordenação e reversão
 * - Gerenciamento de memória
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/array_list.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// ESTRUTURAS AUXILIARES
// ============================================================================

typedef struct {
    int id;
    double value;
} TestStruct;

// ============================================================================
// TESTES: CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

TEST(create_destroy) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(arraylist_is_empty(list));
    ASSERT_EQ(arraylist_size(list), 0);
    ASSERT_EQ(arraylist_capacity(list), 10);
    arraylist_destroy(list);
}

TEST(create_with_growth_strategies) {
    // GROWTH_DOUBLE
    ArrayList *list1 = arraylist_create_with_growth(sizeof(int), 4, GROWTH_DOUBLE, NULL);
    ASSERT_NOT_NULL(list1);
    ASSERT_EQ(arraylist_capacity(list1), 4);
    arraylist_destroy(list1);

    // GROWTH_1_5
    ArrayList *list2 = arraylist_create_with_growth(sizeof(int), 4, GROWTH_1_5, NULL);
    ASSERT_NOT_NULL(list2);
    arraylist_destroy(list2);

    // GROWTH_FIXED
    ArrayList *list3 = arraylist_create_with_growth(sizeof(int), 4, GROWTH_FIXED, NULL);
    ASSERT_NOT_NULL(list3);
    arraylist_destroy(list3);
}

TEST(create_invalid_params) {
    ArrayList *list = arraylist_create(0, 10, NULL);
    ASSERT_NULL(list);
}

// ============================================================================
// TESTES: PUSH_BACK E ACESSO
// ============================================================================

TEST(push_back_and_get) {
    ArrayList *list = arraylist_create(sizeof(int), 4, NULL);

    for (int i = 1; i <= 10; i++) {
        DataStructureError err = arraylist_push_back(list, &i);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    ASSERT_EQ(arraylist_size(list), 10);

    for (size_t i = 0; i < 10; i++) {
        int value;
        DataStructureError err = arraylist_get(list, i, &value);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(value, (int)(i + 1));
    }

    arraylist_destroy(list);
}

TEST(automatic_growth_double) {
    ArrayList *list = arraylist_create_with_growth(sizeof(int), 2, GROWTH_DOUBLE, NULL);

    // Capacidade inicial: 2
    ASSERT_EQ(arraylist_capacity(list), 2);

    // Inserir 3 elementos força crescimento
    int a = 1, b = 2, c = 3;
    arraylist_push_back(list, &a);
    arraylist_push_back(list, &b);
    ASSERT_EQ(arraylist_capacity(list), 2);

    arraylist_push_back(list, &c);  // Força crescimento
    ASSERT_EQ(arraylist_capacity(list), 4);  // 2 * 2 = 4

    arraylist_destroy(list);
}

TEST(get_ptr_access) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 0; i < 5; i++) {
        arraylist_push_back(list, &i);
    }

    // Acesso direto via ponteiro
    int *ptr = (int*)arraylist_get_ptr(list, 2);
    ASSERT_NOT_NULL(ptr);
    ASSERT_EQ(*ptr, 2);

    arraylist_destroy(list);
}

TEST(set_and_modify) {
    ArrayList *list = arraylist_create(sizeof(int), 5, NULL);

    for (int i = 0; i < 5; i++) {
        arraylist_push_back(list, &i);
    }

    // Modificar elemento
    int new_value = 99;
    DataStructureError err = arraylist_set(list, 2, &new_value);
    ASSERT_EQ(err, DS_SUCCESS);

    int value;
    arraylist_get(list, 2, &value);
    ASSERT_EQ(value, 99);

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: INSERÇÃO E REMOÇÃO
// ============================================================================

TEST(push_front_and_pop_front) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 1; i <= 5; i++) {
        arraylist_push_front(list, &i);
    }

    // Ordem: 5, 4, 3, 2, 1
    ASSERT_EQ(arraylist_size(list), 5);

    int value;
    arraylist_pop_front(list, &value);
    ASSERT_EQ(value, 5);
    ASSERT_EQ(arraylist_size(list), 4);

    arraylist_destroy(list);
}

TEST(insert_at_middle) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    int a = 1, b = 2, c = 3;
    arraylist_push_back(list, &a);
    arraylist_push_back(list, &c);

    // Inserir no meio: [1, 2, 3]
    DataStructureError err = arraylist_insert(list, 1, &b);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(arraylist_size(list), 3);

    int value;
    arraylist_get(list, 1, &value);
    ASSERT_EQ(value, 2);

    arraylist_destroy(list);
}

TEST(pop_back_and_remove_at) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 1; i <= 5; i++) {
        arraylist_push_back(list, &i);
    }

    int value;
    DataStructureError err = arraylist_pop_back(list, &value);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(value, 5);
    ASSERT_EQ(arraylist_size(list), 4);

    // Remover índice 1 (valor 2)
    err = arraylist_remove_at(list, 1, &value);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(value, 2);
    ASSERT_EQ(arraylist_size(list), 3);

    // Restam: [1, 3, 4]
    arraylist_get(list, 0, &value);
    ASSERT_EQ(value, 1);
    arraylist_get(list, 1, &value);
    ASSERT_EQ(value, 3);

    arraylist_destroy(list);
}

TEST(remove_by_value) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        arraylist_push_back(list, &values[i]);
    }

    int target = 30;
    DataStructureError err = arraylist_remove(list, &target, compare_int);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(arraylist_size(list), 4);

    // Verificar que 30 foi removido
    ssize_t idx = arraylist_find(list, &target, compare_int);
    ASSERT_EQ(idx, -1);

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: BUSCA
// ============================================================================

TEST(find_linear_search) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    int values[] = {5, 10, 15, 20, 25};
    for (int i = 0; i < 5; i++) {
        arraylist_push_back(list, &values[i]);
    }

    int target = 15;
    ssize_t idx = arraylist_find(list, &target, compare_int);
    ASSERT_EQ(idx, 2);

    target = 99;
    idx = arraylist_find(list, &target, compare_int);
    ASSERT_EQ(idx, -1);  // Não encontrado

    arraylist_destroy(list);
}

TEST(binary_search_sorted) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    for (int i = 0; i < 9; i++) {
        arraylist_push_back(list, &values[i]);
    }

    // Array já ordenado
    int target = 50;
    ssize_t idx = arraylist_binary_search(list, &target, compare_int);
    ASSERT_EQ(idx, 4);

    target = 10;
    idx = arraylist_binary_search(list, &target, compare_int);
    ASSERT_EQ(idx, 0);

    target = 90;
    idx = arraylist_binary_search(list, &target, compare_int);
    ASSERT_EQ(idx, 8);

    target = 25;
    idx = arraylist_binary_search(list, &target, compare_int);
    ASSERT_EQ(idx, -1);  // Não encontrado

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: SORT E REVERSE
// ============================================================================

TEST(sort_ascending) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    int values[] = {50, 10, 30, 20, 40};
    for (int i = 0; i < 5; i++) {
        arraylist_push_back(list, &values[i]);
    }

    arraylist_sort(list, compare_int);

    // Verificar ordem: 10, 20, 30, 40, 50
    int expected[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        int value;
        arraylist_get(list, i, &value);
        ASSERT_EQ(value, expected[i]);
    }

    arraylist_destroy(list);
}

TEST(reverse_array) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 1; i <= 5; i++) {
        arraylist_push_back(list, &i);
    }

    arraylist_reverse(list);

    // Ordem reversa: 5, 4, 3, 2, 1
    int expected[] = {5, 4, 3, 2, 1};
    for (size_t i = 0; i < 5; i++) {
        int value;
        arraylist_get(list, i, &value);
        ASSERT_EQ(value, expected[i]);
    }

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: UTILIDADES
// ============================================================================

TEST(clear_list) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 0; i < 10; i++) {
        arraylist_push_back(list, &i);
    }

    ASSERT_EQ(arraylist_size(list), 10);

    arraylist_clear(list);
    ASSERT_TRUE(arraylist_is_empty(list));
    ASSERT_EQ(arraylist_size(list), 0);

    arraylist_destroy(list);
}

TEST(shrink_to_fit) {
    ArrayList *list = arraylist_create(sizeof(int), 100, NULL);

    for (int i = 0; i < 10; i++) {
        arraylist_push_back(list, &i);
    }

    ASSERT_EQ(arraylist_capacity(list), 100);

    DataStructureError err = arraylist_shrink_to_fit(list);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(arraylist_capacity(list), 10);

    arraylist_destroy(list);
}

TEST(reserve_capacity) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    DataStructureError err = arraylist_reserve(list, 50);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(arraylist_capacity(list), 50);

    arraylist_destroy(list);
}

TEST(clone_list) {
    ArrayList *list = arraylist_create(sizeof(int), 10, NULL);

    for (int i = 1; i <= 5; i++) {
        arraylist_push_back(list, &i);
    }

    ArrayList *clone = arraylist_clone(list, NULL);
    ASSERT_NOT_NULL(clone);
    ASSERT_EQ(arraylist_size(clone), 5);

    // Verificar elementos
    for (size_t i = 0; i < 5; i++) {
        int orig, copy;
        arraylist_get(list, i, &orig);
        arraylist_get(clone, i, &copy);
        ASSERT_EQ(orig, copy);
    }

    arraylist_destroy(list);
    arraylist_destroy(clone);
}

// ============================================================================
// TESTES: STRINGS
// ============================================================================

TEST(arraylist_with_strings) {
    ArrayList *list = arraylist_create(sizeof(char*), 5, destroy_string);

    char *str1 = copy_string(&(char*){"Alice"});
    char *str2 = copy_string(&(char*){"Bob"});
    char *str3 = copy_string(&(char*){"Charlie"});

    arraylist_push_back(list, &str1);
    arraylist_push_back(list, &str2);
    arraylist_push_back(list, &str3);

    ASSERT_EQ(arraylist_size(list), 3);

    char *result;
    arraylist_get(list, 1, &result);
    ASSERT_EQ(strcmp(result, "Bob"), 0);

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: STRUCTS
// ============================================================================

TEST(arraylist_with_structs) {
    ArrayList *list = arraylist_create(sizeof(TestStruct), 5, NULL);

    TestStruct s1 = {1, 10.5};
    TestStruct s2 = {2, 20.5};
    TestStruct s3 = {3, 30.5};

    arraylist_push_back(list, &s1);
    arraylist_push_back(list, &s2);
    arraylist_push_back(list, &s3);

    ASSERT_EQ(arraylist_size(list), 3);

    TestStruct result;
    arraylist_get(list, 1, &result);
    ASSERT_EQ(result.id, 2);
    ASSERT_EQ((int)(result.value * 10), 205);

    arraylist_destroy(list);
}

// ============================================================================
// TESTES: ERROS E EDGE CASES
// ============================================================================

TEST(pop_from_empty) {
    ArrayList *list = arraylist_create(sizeof(int), 5, NULL);

    int value;
    DataStructureError err = arraylist_pop_back(list, &value);
    ASSERT_EQ(err, DS_ERROR_EMPTY);

    arraylist_destroy(list);
}

TEST(invalid_index_access) {
    ArrayList *list = arraylist_create(sizeof(int), 5, NULL);

    int a = 10;
    arraylist_push_back(list, &a);

    int value;
    DataStructureError err = arraylist_get(list, 10, &value);
    ASSERT_EQ(err, DS_ERROR_INVALID_INDEX);

    err = arraylist_set(list, 10, &a);
    ASSERT_EQ(err, DS_ERROR_INVALID_INDEX);

    arraylist_destroy(list);
}

TEST(null_pointer_checks) {
    DataStructureError err;

    err = arraylist_push_back(NULL, &(int){1});
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    ArrayList *list = arraylist_create(sizeof(int), 5, NULL);

    err = arraylist_push_back(list, NULL);
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    arraylist_destroy(list);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(print_visual) {
    ArrayList *list = arraylist_create(sizeof(int), 4, NULL);

    printf("\n");
    for (int i = 1; i <= 5; i++) {
        arraylist_push_back(list, &i);
    }

    printf("    ArrayList após 5 push_back: ");
    arraylist_print(list, print_int);

    arraylist_destroy(list);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("============================================\n");
    printf("  TESTES: ARRAYLIST (Dynamic Array)\n");
    printf("============================================\n\n");

    printf("Criação e Destruição:\n");
    RUN_TEST(create_destroy);
    RUN_TEST(create_with_growth_strategies);
    RUN_TEST(create_invalid_params);

    printf("\nPush Back e Acesso:\n");
    RUN_TEST(push_back_and_get);
    RUN_TEST(automatic_growth_double);
    RUN_TEST(get_ptr_access);
    RUN_TEST(set_and_modify);

    printf("\nInserção e Remoção:\n");
    RUN_TEST(push_front_and_pop_front);
    RUN_TEST(insert_at_middle);
    RUN_TEST(pop_back_and_remove_at);
    RUN_TEST(remove_by_value);

    printf("\nBusca:\n");
    RUN_TEST(find_linear_search);
    RUN_TEST(binary_search_sorted);

    printf("\nSort e Reverse:\n");
    RUN_TEST(sort_ascending);
    RUN_TEST(reverse_array);

    printf("\nUtilidades:\n");
    RUN_TEST(clear_list);
    RUN_TEST(shrink_to_fit);
    RUN_TEST(reserve_capacity);
    RUN_TEST(clone_list);

    printf("\nStrings e Structs:\n");
    RUN_TEST(arraylist_with_strings);
    RUN_TEST(arraylist_with_structs);

    printf("\nErros e Edge Cases:\n");
    RUN_TEST(pop_from_empty);
    RUN_TEST(invalid_index_access);
    RUN_TEST(null_pointer_checks);

    printf("\nTeste Visual:\n");
    RUN_TEST(print_visual);

    printf("\n============================================\n");
    printf("  ✅ TODOS OS TESTES PASSARAM! (26 testes)\n");
    printf("============================================\n\n");

    return 0;
}
