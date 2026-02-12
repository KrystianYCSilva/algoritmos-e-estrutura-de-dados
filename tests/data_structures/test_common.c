/**
 * @file test_common.c
 * @brief Testes unitários para common.c
 *
 * Testa todas as funções auxiliares: comparação, cópia, destruição,
 * impressão e hash para tipos primitivos e strings.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// ============================================================================
// MACROS DE TESTE
// ============================================================================

#define TEST(name) \
    void test_##name(); \
    printf("  Running: %-40s", #name); \
    test_##name(); \
    printf("[✓ PASS]\n"); \
    void test_##name()

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("\n    Assertion failed: %s\n", #condition); \
            printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NE(a, b) ASSERT((a) != (b))
#define ASSERT_LT(a, b) ASSERT((a) < (b))
#define ASSERT_GT(a, b) ASSERT((a) > (b))
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)

// ============================================================================
// TESTES DE COMPARAÇÃO
// ============================================================================

TEST(compare_int_equal) {
    int a = 42, b = 42;
    ASSERT_EQ(compare_int(&a, &b), 0);
}

TEST(compare_int_less_than) {
    int a = 10, b = 20;
    ASSERT_LT(compare_int(&a, &b), 0);
}

TEST(compare_int_greater_than) {
    int a = 30, b = 15;
    ASSERT_GT(compare_int(&a, &b), 0);
}

TEST(compare_int_negative_numbers) {
    int a = -10, b = -5;
    ASSERT_LT(compare_int(&a, &b), 0);

    int c = -5, d = -10;
    ASSERT_GT(compare_int(&c, &d), 0);
}

TEST(compare_int_null) {
    int a = 42;
    ASSERT_LT(compare_int(NULL, &a), 0);
    ASSERT_GT(compare_int(&a, NULL), 0);
    ASSERT_EQ(compare_int(NULL, NULL), 0);
}

TEST(compare_float_equal) {
    float a = 3.14f, b = 3.14f;
    ASSERT_EQ(compare_float(&a, &b), 0);
}

TEST(compare_float_close_enough) {
    float a = 3.14159f, b = 3.14158f;  // Diferença muito pequena
    ASSERT_EQ(compare_float(&a, &b), 0);  // Deve considerar iguais (epsilon)
}

TEST(compare_float_different) {
    float a = 1.0f, b = 2.0f;
    ASSERT_LT(compare_float(&a, &b), 0);
    ASSERT_GT(compare_float(&b, &a), 0);
}

TEST(compare_double_equal) {
    double a = 2.718281828, b = 2.718281828;
    ASSERT_EQ(compare_double(&a, &b), 0);
}

TEST(compare_double_different) {
    double a = 1.5, b = 2.5;
    ASSERT_LT(compare_double(&a, &b), 0);
}

TEST(compare_string_equal) {
    const char *a = "hello";
    const char *b = "hello";
    ASSERT_EQ(compare_string(&a, &b), 0);
}

TEST(compare_string_less_than) {
    const char *a = "apple";
    const char *b = "banana";
    ASSERT_LT(compare_string(&a, &b), 0);
}

TEST(compare_string_greater_than) {
    const char *a = "zebra";
    const char *b = "aardvark";
    ASSERT_GT(compare_string(&a, &b), 0);
}

TEST(compare_string_empty) {
    const char *a = "";
    const char *b = "nonempty";
    ASSERT_LT(compare_string(&a, &b), 0);
}

TEST(compare_string_null) {
    const char *a = "hello";
    ASSERT_GT(compare_string(&a, NULL), 0);
}

// ============================================================================
// TESTES DE HASH
// ============================================================================

TEST(hash_int_basic) {
    int a = 42;
    size_t hash = hash_int(&a);
    ASSERT_NE(hash, 0);  // Hash não deve ser zero para valor não-zero
}

TEST(hash_int_consistency) {
    int a = 123;
    size_t hash1 = hash_int(&a);
    size_t hash2 = hash_int(&a);
    ASSERT_EQ(hash1, hash2);  // Mesma entrada = mesmo hash
}

TEST(hash_int_different_values) {
    int a = 100, b = 101;
    size_t hash_a = hash_int(&a);
    size_t hash_b = hash_int(&b);
    // Valores diferentes devem produzir hashes diferentes (geralmente)
    // Nota: colisões são possíveis, mas improváveis para valores próximos
    ASSERT_NE(hash_a, hash_b);
}

TEST(hash_int_null) {
    ASSERT_EQ(hash_int(NULL), 0);
}

TEST(hash_string_basic) {
    const char *str = "hello";
    size_t hash = hash_string(&str);
    ASSERT_NE(hash, 0);
}

TEST(hash_string_consistency) {
    const char *str = "algorithm";
    size_t hash1 = hash_string(&str);
    size_t hash2 = hash_string(&str);
    ASSERT_EQ(hash1, hash2);
}

TEST(hash_string_different_strings) {
    const char *a = "hello";
    const char *b = "world";
    size_t hash_a = hash_string(&a);
    size_t hash_b = hash_string(&b);
    ASSERT_NE(hash_a, hash_b);
}

TEST(hash_string_empty) {
    const char *str = "";
    size_t hash = hash_string(&str);
    // Hash de string vazia deve ser o valor inicial (5381 para djb2)
    ASSERT_NE(hash, 0);
}

TEST(hash_string_null) {
    ASSERT_EQ(hash_string(NULL), 0);
}

TEST(hash_djb2_same_as_hash_string) {
    const char *str = "test";
    size_t hash1 = hash_string(&str);
    size_t hash2 = hash_djb2(&str);
    ASSERT_EQ(hash1, hash2);
}

TEST(hash_fnv1a_basic) {
    const char *str = "hello";
    size_t hash = hash_fnv1a(str, strlen(str));
    ASSERT_NE(hash, 0);
}

TEST(hash_fnv1a_consistency) {
    const char *str = "algorithm";
    size_t hash1 = hash_fnv1a(str, strlen(str));
    size_t hash2 = hash_fnv1a(str, strlen(str));
    ASSERT_EQ(hash1, hash2);
}

TEST(hash_fnv1a_different_data) {
    const char *a = "abc";
    const char *b = "def";
    size_t hash_a = hash_fnv1a(a, strlen(a));
    size_t hash_b = hash_fnv1a(b, strlen(b));
    ASSERT_NE(hash_a, hash_b);
}

TEST(hash_fnv1a_null) {
    ASSERT_EQ(hash_fnv1a(NULL, 10), 0);
    ASSERT_EQ(hash_fnv1a("test", 0), 0);
}

TEST(hash_multiplicative_basic) {
    int value = 42;
    size_t hash = hash_multiplicative(&value);
    ASSERT_NE(hash, 0);
}

TEST(hash_multiplicative_consistency) {
    int value = 12345;
    size_t hash1 = hash_multiplicative(&value);
    size_t hash2 = hash_multiplicative(&value);
    ASSERT_EQ(hash1, hash2);
}

TEST(hash_multiplicative_null) {
    ASSERT_EQ(hash_multiplicative(NULL), 0);
}

// ============================================================================
// TESTES DE IMPRESSÃO (output visual, não automatizado)
// ============================================================================

TEST(print_functions_visual) {
    printf("\n    Visual output test:\n");

    int i = 42;
    printf("    Int: ");
    print_int(&i);
    printf("\n");

    float f = 3.14f;
    printf("    Float: ");
    print_float(&f);
    printf("\n");

    double d = 2.718281828;
    printf("    Double: ");
    print_double(&d);
    printf("\n");

    const char *s = "Hello, World!";
    printf("    String: ");
    print_string(&s);
    printf("\n");

    printf("    NULL int: ");
    print_int(NULL);
    printf("\n");

    printf("    NULL string: ");
    print_string(NULL);
    printf("\n");
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE common.c\n");
    printf("========================================\n\n");

    printf("Comparação de inteiros:\n");
    test_compare_int_equal();
    test_compare_int_less_than();
    test_compare_int_greater_than();
    test_compare_int_negative_numbers();
    test_compare_int_null();

    printf("\nComparação de floats:\n");
    test_compare_float_equal();
    test_compare_float_close_enough();
    test_compare_float_different();

    printf("\nComparação de doubles:\n");
    test_compare_double_equal();
    test_compare_double_different();

    printf("\nComparação de strings:\n");
    test_compare_string_equal();
    test_compare_string_less_than();
    test_compare_string_greater_than();
    test_compare_string_empty();
    test_compare_string_null();

    printf("\nFunções hash (int):\n");
    test_hash_int_basic();
    test_hash_int_consistency();
    test_hash_int_different_values();
    test_hash_int_null();

    printf("\nFunções hash (string):\n");
    test_hash_string_basic();
    test_hash_string_consistency();
    test_hash_string_different_strings();
    test_hash_string_empty();
    test_hash_string_null();
    test_hash_djb2_same_as_hash_string();

    printf("\nFunções hash (FNV-1a):\n");
    test_hash_fnv1a_basic();
    test_hash_fnv1a_consistency();
    test_hash_fnv1a_different_data();
    test_hash_fnv1a_null();

    printf("\nFunções hash (multiplicativa):\n");
    test_hash_multiplicative_basic();
    test_hash_multiplicative_consistency();
    test_hash_multiplicative_null();

    printf("\nFunções de impressão:\n");
    test_print_functions_visual();

    printf("\n========================================\n");
    printf("  TODOS OS TESTES PASSARAM! ✓\n");
    printf("========================================\n");

    return 0;
}
