/**
 * @file test_macros.h
 * @brief Macros compartilhadas para testes unitários
 *
 * Define macros padronizadas para declaração, execução e
 * asserção de testes. Todos os arquivos test_*.c devem
 * incluir este header em vez de redefinir suas próprias macros.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define TEST(name) static void test_##name(void)

#define RUN_TEST(name) do { \
    printf("  - " #name "... "); \
    fflush(stdout); \
    test_##name(); \
    printf("✓\n"); \
} while(0)

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("\n    Assertion failed: %s\n", #condition); \
            printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

#define ASSERT_EQ(a, b)       ASSERT((a) == (b))
#define ASSERT_NE(a, b)       ASSERT((a) != (b))
#define ASSERT_LT(a, b)       ASSERT((a) < (b))
#define ASSERT_GT(a, b)       ASSERT((a) > (b))
#define ASSERT_TRUE(cond)     ASSERT(cond)
#define ASSERT_FALSE(cond)    ASSERT(!(cond))
#define ASSERT_NULL(ptr)      ASSERT((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr)  ASSERT((ptr) != NULL)

#endif
