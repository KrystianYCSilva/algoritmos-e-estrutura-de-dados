/**
 * @file test_trie.c
 * @brief Testes unitários para Trie (Prefix Tree)
 *
 * Testa todas as operações do Trie: insert, search, remove,
 * starts_with, autocomplete, longest_common_prefix, to_array.
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/trie.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// FUNÇÕES AUXILIARES DE TESTE
// ============================================================================

static void free_string_array(char **array, size_t count) {
    if (array == NULL) return;
    for (size_t i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}

// ============================================================================
// TESTES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

TEST(create_destroy) {
    Trie *trie = trie_create(26);
    ASSERT_NOT_NULL(trie);
    ASSERT_TRUE(trie_is_empty(trie));
    ASSERT_EQ(trie_size(trie), 0);
    trie_destroy(trie);
}

// ============================================================================
// TESTES DE INSERT E SEARCH
// ============================================================================

TEST(insert_search_single) {
    Trie *trie = trie_create(26);

    ASSERT_EQ(trie_insert(trie, "hello"), DS_SUCCESS);
    ASSERT_TRUE(trie_search(trie, "hello"));
    ASSERT_EQ(trie_size(trie), 1);

    trie_destroy(trie);
}

TEST(insert_search_multiple) {
    Trie *trie = trie_create(26);

    ASSERT_EQ(trie_insert(trie, "apple"), DS_SUCCESS);
    ASSERT_EQ(trie_insert(trie, "app"), DS_SUCCESS);
    ASSERT_EQ(trie_insert(trie, "banana"), DS_SUCCESS);
    ASSERT_EQ(trie_insert(trie, "band"), DS_SUCCESS);

    ASSERT_TRUE(trie_search(trie, "apple"));
    ASSERT_TRUE(trie_search(trie, "app"));
    ASSERT_TRUE(trie_search(trie, "banana"));
    ASSERT_TRUE(trie_search(trie, "band"));
    ASSERT_EQ(trie_size(trie), 4);

    trie_destroy(trie);
}

TEST(search_not_found) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "hello");

    ASSERT_FALSE(trie_search(trie, "hell"));
    ASSERT_FALSE(trie_search(trie, "hellos"));
    ASSERT_FALSE(trie_search(trie, "world"));
    ASSERT_FALSE(trie_search(trie, ""));

    trie_destroy(trie);
}

TEST(search_prefix_only) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "apple");

    ASSERT_FALSE(trie_search(trie, "app"));
    ASSERT_FALSE(trie_search(trie, "a"));
    ASSERT_TRUE(trie_search(trie, "apple"));

    trie_destroy(trie);
}

TEST(insert_duplicate) {
    Trie *trie = trie_create(26);

    ASSERT_EQ(trie_insert(trie, "hello"), DS_SUCCESS);
    ASSERT_EQ(trie_size(trie), 1);

    ASSERT_EQ(trie_insert(trie, "hello"), DS_SUCCESS);
    ASSERT_EQ(trie_size(trie), 1);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE STARTS_WITH
// ============================================================================

TEST(starts_with) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "apple");
    trie_insert(trie, "application");
    trie_insert(trie, "banana");

    ASSERT_TRUE(trie_starts_with(trie, "app"));
    ASSERT_TRUE(trie_starts_with(trie, "apple"));
    ASSERT_TRUE(trie_starts_with(trie, "applic"));
    ASSERT_TRUE(trie_starts_with(trie, "ban"));
    ASSERT_TRUE(trie_starts_with(trie, ""));
    ASSERT_FALSE(trie_starts_with(trie, "cat"));
    ASSERT_FALSE(trie_starts_with(trie, "bans"));

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE REMOVE
// ============================================================================

TEST(remove_word) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "hello");
    trie_insert(trie, "world");
    ASSERT_EQ(trie_size(trie), 2);

    ASSERT_EQ(trie_remove(trie, "hello"), DS_SUCCESS);
    ASSERT_FALSE(trie_search(trie, "hello"));
    ASSERT_TRUE(trie_search(trie, "world"));
    ASSERT_EQ(trie_size(trie), 1);

    ASSERT_EQ(trie_remove(trie, "notfound"), DS_ERROR_NOT_FOUND);

    trie_destroy(trie);
}

TEST(remove_prefix_shared) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "apple");
    trie_insert(trie, "app");

    ASSERT_EQ(trie_remove(trie, "apple"), DS_SUCCESS);
    ASSERT_FALSE(trie_search(trie, "apple"));
    ASSERT_TRUE(trie_search(trie, "app"));
    ASSERT_EQ(trie_size(trie), 1);

    ASSERT_EQ(trie_remove(trie, "app"), DS_SUCCESS);
    ASSERT_FALSE(trie_search(trie, "app"));
    ASSERT_EQ(trie_size(trie), 0);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE AUTOCOMPLETE
// ============================================================================

TEST(autocomplete) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "car");
    trie_insert(trie, "card");
    trie_insert(trie, "care");
    trie_insert(trie, "careful");
    trie_insert(trie, "dog");

    char **results = NULL;
    size_t count = 0;

    ASSERT_EQ(trie_autocomplete(trie, "car", &results, &count), DS_SUCCESS);
    ASSERT_EQ(count, 4);

    ASSERT_EQ(strcmp(results[0], "car"), 0);
    ASSERT_EQ(strcmp(results[1], "card"), 0);
    ASSERT_EQ(strcmp(results[2], "care"), 0);
    ASSERT_EQ(strcmp(results[3], "careful"), 0);

    free_string_array(results, count);

    results = NULL;
    count = 0;
    ASSERT_EQ(trie_autocomplete(trie, "xyz", &results, &count), DS_SUCCESS);
    ASSERT_EQ(count, 0);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE LONGEST COMMON PREFIX
// ============================================================================

TEST(longest_common_prefix) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "flower");
    trie_insert(trie, "flow");
    trie_insert(trie, "flight");

    char *lcp = trie_longest_common_prefix(trie);
    ASSERT_NOT_NULL(lcp);
    ASSERT_EQ(strcmp(lcp, "fl"), 0);
    free(lcp);

    trie_destroy(trie);
}

TEST(longest_common_prefix_full) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "test");

    char *lcp = trie_longest_common_prefix(trie);
    ASSERT_NOT_NULL(lcp);
    ASSERT_EQ(strcmp(lcp, "test"), 0);
    free(lcp);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE TO_ARRAY (ORDEM LEXICOGRÁFICA)
// ============================================================================

TEST(to_array_sorted) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "dog");
    trie_insert(trie, "apple");
    trie_insert(trie, "cat");
    trie_insert(trie, "banana");

    char **strings = NULL;
    size_t count = 0;

    ASSERT_EQ(trie_to_array(trie, &strings, &count), DS_SUCCESS);
    ASSERT_EQ(count, 4);

    ASSERT_EQ(strcmp(strings[0], "apple"), 0);
    ASSERT_EQ(strcmp(strings[1], "banana"), 0);
    ASSERT_EQ(strcmp(strings[2], "cat"), 0);
    ASSERT_EQ(strcmp(strings[3], "dog"), 0);

    free_string_array(strings, count);
    trie_destroy(trie);
}

// ============================================================================
// TESTES DE SIZE TRACKING
// ============================================================================

TEST(size_tracking) {
    Trie *trie = trie_create(26);

    ASSERT_EQ(trie_size(trie), 0);

    trie_insert(trie, "a");
    ASSERT_EQ(trie_size(trie), 1);

    trie_insert(trie, "b");
    ASSERT_EQ(trie_size(trie), 2);

    trie_insert(trie, "a");
    ASSERT_EQ(trie_size(trie), 2);

    trie_remove(trie, "a");
    ASSERT_EQ(trie_size(trie), 1);

    trie_remove(trie, "b");
    ASSERT_EQ(trie_size(trie), 0);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE CLEAR
// ============================================================================

TEST(clear) {
    Trie *trie = trie_create(26);

    trie_insert(trie, "hello");
    trie_insert(trie, "world");
    trie_insert(trie, "test");

    ASSERT_EQ(trie_size(trie), 3);

    trie_clear(trie);

    ASSERT_TRUE(trie_is_empty(trie));
    ASSERT_EQ(trie_size(trie), 0);
    ASSERT_FALSE(trie_search(trie, "hello"));

    ASSERT_EQ(trie_insert(trie, "new"), DS_SUCCESS);
    ASSERT_TRUE(trie_search(trie, "new"));
    ASSERT_EQ(trie_size(trie), 1);

    trie_destroy(trie);
}

// ============================================================================
// TESTES DE NULL POINTER
// ============================================================================

TEST(null_pointer_checks) {
    ASSERT_EQ(trie_insert(NULL, "hello"), DS_ERROR_NULL_POINTER);
    ASSERT_FALSE(trie_search(NULL, "hello"));
    ASSERT_FALSE(trie_starts_with(NULL, "he"));
    ASSERT_EQ(trie_remove(NULL, "hello"), DS_ERROR_NULL_POINTER);
    ASSERT_TRUE(trie_is_empty(NULL));
    ASSERT_EQ(trie_size(NULL), 0);

    Trie *trie = trie_create(26);
    ASSERT_EQ(trie_insert(trie, NULL), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(trie_remove(trie, NULL), DS_ERROR_NULL_POINTER);
    trie_destroy(trie);
}

// ============================================================================
// MAIN - RUNNER DE TESTES
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  TESTES DE TRIE (PREFIX TREE)\n");
    printf("========================================\n\n");

    printf("Criacao e Destruicao:\n");
    RUN_TEST(create_destroy);

    printf("\nInsert e Search:\n");
    RUN_TEST(insert_search_single);
    RUN_TEST(insert_search_multiple);
    RUN_TEST(search_not_found);
    RUN_TEST(search_prefix_only);
    RUN_TEST(insert_duplicate);

    printf("\nStarts With:\n");
    RUN_TEST(starts_with);

    printf("\nRemove:\n");
    RUN_TEST(remove_word);
    RUN_TEST(remove_prefix_shared);

    printf("\nAutocomplete:\n");
    RUN_TEST(autocomplete);

    printf("\nLongest Common Prefix:\n");
    RUN_TEST(longest_common_prefix);
    RUN_TEST(longest_common_prefix_full);

    printf("\nTo Array (Sorted):\n");
    RUN_TEST(to_array_sorted);

    printf("\nSize Tracking:\n");
    RUN_TEST(size_tracking);

    printf("\nClear:\n");
    RUN_TEST(clear);

    printf("\nNull Pointer:\n");
    RUN_TEST(null_pointer_checks);

    printf("\n============================================\n");
    printf("  TODOS OS TESTES PASSARAM! (14 testes)\n");
    printf("============================================\n");

    return 0;
}
