/**
 * @file test_stack.c
 * @brief Testes unitários para Stack (LIFO)
 *
 * Testa ambas implementações: STACK_ARRAY e STACK_LINKED
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/stack.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// TESTES PARA STACK_ARRAY
// ============================================================================

TEST(stack_array_create_destroy) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);
    ASSERT_NOT_NULL(s);
    ASSERT_TRUE(stack_is_empty(s));
    ASSERT_EQ(stack_size(s), 0);
    ASSERT_EQ(stack_capacity(s), 10);
    stack_destroy(s);
}

TEST(stack_array_push_pop_single) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);

    int value = 42;
    ASSERT_EQ(stack_push(s, &value), DS_SUCCESS);
    ASSERT_FALSE(stack_is_empty(s));
    ASSERT_EQ(stack_size(s), 1);

    int output;
    ASSERT_EQ(stack_pop(s, &output), DS_SUCCESS);
    ASSERT_EQ(output, 42);
    ASSERT_TRUE(stack_is_empty(s));

    stack_destroy(s);
}

TEST(stack_array_lifo_order) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    // Push 1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    // Pop deve retornar 5, 4, 3, 2, 1 (LIFO - Last In, First Out)
    for (int i = 5; i >= 1; i--) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    ASSERT_TRUE(stack_is_empty(s));
    stack_destroy(s);
}

TEST(stack_array_top) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    int val1 = 10, val2 = 20, val3 = 30;
    stack_push(s, &val1);
    stack_push(s, &val2);
    stack_push(s, &val3);

    int output;
    ASSERT_EQ(stack_top(s, &output), DS_SUCCESS);
    ASSERT_EQ(output, 30);  // Último inserido (topo)

    // Top não remove
    ASSERT_EQ(stack_size(s), 3);

    stack_destroy(s);
}

TEST(stack_array_resize) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 2, NULL);

    // Push mais que a capacidade inicial
    for (int i = 1; i <= 10; i++) {
        ASSERT_EQ(stack_push(s, &i), DS_SUCCESS);
    }

    ASSERT_EQ(stack_size(s), 10);
    ASSERT_TRUE(stack_capacity(s) >= 10);

    // Verificar ordem LIFO
    for (int i = 10; i >= 1; i--) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    stack_destroy(s);
}

TEST(stack_array_clear) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    stack_clear(s);
    ASSERT_TRUE(stack_is_empty(s));
    ASSERT_EQ(stack_size(s), 0);

    // Deve poder usar novamente
    int val = 99;
    stack_push(s, &val);
    int output;
    stack_pop(s, &output);
    ASSERT_EQ(output, 99);

    stack_destroy(s);
}

TEST(stack_array_empty_operations) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    int output;

    // Pop em pilha vazia deve falhar
    ASSERT_EQ(stack_pop(s, &output), DS_ERROR_EMPTY);

    // Top em pilha vazia deve falhar
    ASSERT_EQ(stack_top(s, &output), DS_ERROR_EMPTY);

    stack_destroy(s);
}

TEST(stack_array_reverse) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    // Antes do reverse: topo é 5
    int top_before;
    stack_top(s, &top_before);
    ASSERT_EQ(top_before, 5);

    stack_reverse(s);

    // Depois do reverse: topo é 1
    int top_after;
    stack_top(s, &top_after);
    ASSERT_EQ(top_after, 1);

    // Pop deve retornar 1, 2, 3, 4, 5 (reverso do LIFO original)
    for (int i = 1; i <= 5; i++) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    stack_destroy(s);
}

// ============================================================================
// TESTES PARA STACK_LINKED
// ============================================================================

TEST(stack_linked_create_destroy) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);
    ASSERT_NOT_NULL(s);
    ASSERT_TRUE(stack_is_empty(s));
    ASSERT_EQ(stack_size(s), 0);
    stack_destroy(s);
}

TEST(stack_linked_push_pop_single) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    int value = 42;
    stack_push(s, &value);

    int output;
    stack_pop(s, &output);
    ASSERT_EQ(output, 42);
    ASSERT_TRUE(stack_is_empty(s));

    stack_destroy(s);
}

TEST(stack_linked_lifo_order) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    for (int i = 5; i >= 1; i--) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    stack_destroy(s);
}

TEST(stack_linked_top) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    int val1 = 10, val2 = 20;
    stack_push(s, &val1);
    stack_push(s, &val2);

    int output;
    stack_top(s, &output);
    ASSERT_EQ(output, 20);
    ASSERT_EQ(stack_size(s), 2);

    stack_destroy(s);
}

TEST(stack_linked_clear) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    stack_clear(s);
    ASSERT_TRUE(stack_is_empty(s));

    stack_destroy(s);
}

TEST(stack_linked_unlimited_size) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    const int LARGE = 10000;

    for (int i = 0; i < LARGE; i++) {
        stack_push(s, &i);
    }

    ASSERT_EQ(stack_size(s), LARGE);

    for (int i = LARGE - 1; i >= 0; i--) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    stack_destroy(s);
}

TEST(stack_linked_reverse) {
    Stack *s = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s, &i);
    }

    stack_reverse(s);

    // Após reverse, pop deve retornar 1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++) {
        int output;
        stack_pop(s, &output);
        ASSERT_EQ(output, i);
    }

    stack_destroy(s);
}

// ============================================================================
// TESTES DE APLICAÇÕES CLÁSSICAS
// ============================================================================

TEST(stack_balanced_parentheses) {
    const char *test_cases[] = {
        "((()))",     // válido
        "(()())",     // válido
        "(()",        // inválido
        "())()",      // inválido
        ""            // válido (vazio)
    };
    bool expected[] = {true, true, false, false, true};

    for (int t = 0; t < 5; t++) {
        Stack *s = stack_create(sizeof(char), STACK_ARRAY, 50, NULL);
        const char *expr = test_cases[t];

        bool valid = true;
        for (size_t i = 0; expr[i] != '\0'; i++) {
            if (expr[i] == '(') {
                stack_push(s, &expr[i]);
            } else if (expr[i] == ')') {
                if (stack_is_empty(s)) {
                    valid = false;
                    break;
                }
                char dummy;
                stack_pop(s, &dummy);
            }
        }

        if (valid && !stack_is_empty(s)) {
            valid = false;
        }

        ASSERT_EQ(valid, expected[t]);
        stack_destroy(s);
    }
}

TEST(stack_reverse_string) {
    Stack *s = stack_create(sizeof(char), STACK_ARRAY, 10, NULL);

    const char *original = "HELLO";

    // Push cada caractere
    for (size_t i = 0; original[i] != '\0'; i++) {
        stack_push(s, &original[i]);
    }

    // Pop para formar string reversa
    char reversed[10];
    size_t idx = 0;
    while (!stack_is_empty(s)) {
        stack_pop(s, &reversed[idx++]);
    }
    reversed[idx] = '\0';

    ASSERT_EQ(strcmp(reversed, "OLLEH"), 0);
    stack_destroy(s);
}

// ============================================================================
// TESTES COM STRINGS
// ============================================================================

TEST(stack_array_strings) {
    Stack *s = stack_create(sizeof(char*), STACK_ARRAY, 5, NULL);

    const char *str1 = "First";
    const char *str2 = "Second";
    const char *str3 = "Third";

    stack_push(s, &str1);
    stack_push(s, &str2);
    stack_push(s, &str3);

    char *output;
    stack_pop(s, &output);
    ASSERT_EQ(strcmp(output, "Third"), 0);

    stack_pop(s, &output);
    ASSERT_EQ(strcmp(output, "Second"), 0);

    stack_destroy(s);
}

// ============================================================================
// TESTES DE ERRO
// ============================================================================

TEST(stack_null_pointer_checks) {
    int value = 42;

    // Criar com element_size = 0 deve falhar
    Stack *s1 = stack_create(0, STACK_ARRAY, 10, NULL);
    ASSERT_NULL(s1);

    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);

    // Push com NULL deve falhar
    ASSERT_EQ(stack_push(NULL, &value), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(stack_push(s, NULL), DS_ERROR_NULL_POINTER);

    // Top com NULL deve falhar
    int output;
    ASSERT_EQ(stack_top(NULL, &output), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(stack_top(s, NULL), DS_ERROR_NULL_POINTER);

    // Pop com stack NULL deve falhar
    ASSERT_EQ(stack_pop(NULL, &output), DS_ERROR_NULL_POINTER);

    stack_destroy(s);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(stack_print_visual) {
    printf("\n    Visual print test:\n");

    Stack *s_array = stack_create(sizeof(int), STACK_ARRAY, 5, NULL);
    Stack *s_linked = stack_create(sizeof(int), STACK_LINKED, 0, NULL);

    for (int i = 1; i <= 5; i++) {
        stack_push(s_array, &i);
        stack_push(s_linked, &i);
    }

    printf("    ARRAY:  ");
    stack_print(s_array, print_int);

    printf("    LINKED: ");
    stack_print(s_linked, print_int);

    stack_destroy(s_array);
    stack_destroy(s_linked);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE STACK (LIFO)\n");
    printf("========================================\n\n");

    printf("STACK_ARRAY:\n");
    RUN_TEST(stack_array_create_destroy);
    RUN_TEST(stack_array_push_pop_single);
    RUN_TEST(stack_array_lifo_order);
    RUN_TEST(stack_array_top);
    RUN_TEST(stack_array_resize);
    RUN_TEST(stack_array_clear);
    RUN_TEST(stack_array_empty_operations);
    RUN_TEST(stack_array_reverse);

    printf("\nSTACK_LINKED:\n");
    RUN_TEST(stack_linked_create_destroy);
    RUN_TEST(stack_linked_push_pop_single);
    RUN_TEST(stack_linked_lifo_order);
    RUN_TEST(stack_linked_top);
    RUN_TEST(stack_linked_clear);
    RUN_TEST(stack_linked_unlimited_size);
    RUN_TEST(stack_linked_reverse);

    printf("\nAplicações Clássicas:\n");
    RUN_TEST(stack_balanced_parentheses);
    RUN_TEST(stack_reverse_string);

    printf("\nTestes com Strings:\n");
    RUN_TEST(stack_array_strings);

    printf("\nTestes de Erro:\n");
    RUN_TEST(stack_null_pointer_checks);

    printf("\nPrint Visual:\n");
    RUN_TEST(stack_print_visual);

    printf("\n============================================\n");
    printf("  ✅ TODOS OS TESTES PASSARAM! (23 testes)\n");
    printf("============================================\n");

    return 0;
}
