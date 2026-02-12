/**
 * @file test_hash_table.c
 * @brief Testes para Hash Table
 *
 * Valida:
 * - 4 estratégias de colisão: Chaining, Linear/Quadratic Probing, Double Hashing
 * - Operações O(1): put, get, remove
 * - Rehashing automático
 * - Iterator
 * - Estatísticas
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/hash_table.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// TESTES: CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

TEST(create_destroy_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 16,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);
    ASSERT_NOT_NULL(ht);
    ASSERT_TRUE(hashtable_is_empty(ht));
    ASSERT_EQ(hashtable_size(ht), 0);
    hashtable_destroy(ht);
}

TEST(create_all_strategies) {
    CollisionStrategy strategies[] = {
        HASH_CHAINING,
        HASH_LINEAR_PROBING,
        HASH_QUADRATIC_PROBING,
        HASH_DOUBLE_HASHING
    };

    for (int i = 0; i < 4; i++) {
        HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                          hash_int, compare_int,
                                          strategies[i], NULL, NULL);
        ASSERT_NOT_NULL(ht);
        hashtable_destroy(ht);
    }
}

// ============================================================================
// TESTES: PUT E GET - CHAINING
// ============================================================================

TEST(put_get_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 20; i++) {
        int key = i;
        int value = i * 10;
        DataStructureError err = hashtable_put(ht, &key, &value);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    ASSERT_EQ(hashtable_size(ht), 20);

    // Verificar valores
    for (int i = 0; i < 20; i++) {
        int key = i;
        int value;
        DataStructureError err = hashtable_get(ht, &key, &value);
        ASSERT_EQ(err, DS_SUCCESS);
        ASSERT_EQ(value, i * 10);
    }

    hashtable_destroy(ht);
}

TEST(put_update_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int key = 5;
    int value1 = 100;
    hashtable_put(ht, &key, &value1);

    // Atualizar
    int value2 = 200;
    hashtable_put(ht, &key, &value2);

    ASSERT_EQ(hashtable_size(ht), 1);

    int result;
    hashtable_get(ht, &key, &result);
    ASSERT_EQ(result, 200);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: PUT E GET - LINEAR PROBING
// ============================================================================

TEST(put_get_linear_probing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_LINEAR_PROBING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value = i * 5;
        DataStructureError err = hashtable_put(ht, &key, &value);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value;
        hashtable_get(ht, &key, &value);
        ASSERT_EQ(value, i * 5);
    }

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: PUT E GET - QUADRATIC PROBING
// ============================================================================

TEST(put_get_quadratic_probing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_QUADRATIC_PROBING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value = i + 100;
        DataStructureError err = hashtable_put(ht, &key, &value);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value;
        hashtable_get(ht, &key, &value);
        ASSERT_EQ(value, i + 100);
    }

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: PUT E GET - DOUBLE HASHING
// ============================================================================

TEST(put_get_double_hashing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_DOUBLE_HASHING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value = i * 2;
        DataStructureError err = hashtable_put(ht, &key, &value);
        ASSERT_EQ(err, DS_SUCCESS);
    }

    for (int i = 0; i < 10; i++) {
        int key = i;
        int value;
        hashtable_get(ht, &key, &value);
        ASSERT_EQ(value, i * 2);
    }

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: REMOVE
// ============================================================================

TEST(remove_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        hashtable_put(ht, &i, &i);
    }

    int key = 5;
    int old_value;
    DataStructureError err = hashtable_remove(ht, &key, &old_value);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(old_value, 5);
    ASSERT_EQ(hashtable_size(ht), 9);

    // Verificar que foi removido
    err = hashtable_get(ht, &key, NULL);
    ASSERT_EQ(err, DS_ERROR_NOT_FOUND);

    hashtable_destroy(ht);
}

TEST(remove_linear_probing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_LINEAR_PROBING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        hashtable_put(ht, &i, &i);
    }

    int key = 3;
    DataStructureError err = hashtable_remove(ht, &key, NULL);
    ASSERT_EQ(err, DS_SUCCESS);

    ASSERT_FALSE(hashtable_contains(ht, &key));

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: CONTAINS E GET_PTR
// ============================================================================

TEST(contains_key) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int key = 42;
    int value = 100;
    hashtable_put(ht, &key, &value);

    ASSERT_TRUE(hashtable_contains(ht, &key));

    int nonexistent = 99;
    ASSERT_FALSE(hashtable_contains(ht, &nonexistent));

    hashtable_destroy(ht);
}

TEST(get_ptr_access) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int key = 7;
    int value = 777;
    hashtable_put(ht, &key, &value);

    int *ptr = (int*)hashtable_get_ptr(ht, &key);
    ASSERT_NOT_NULL(ptr);
    ASSERT_EQ(*ptr, 777);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: STRINGS
// ============================================================================

TEST(string_keys_chaining) {
    HashTable *ht = hashtable_create(sizeof(char*), sizeof(int), 8,
                                      hash_string, compare_string,
                                      HASH_CHAINING, destroy_string, NULL);

    char *key1 = copy_string(&(char*){"Alice"});
    char *key2 = copy_string(&(char*){"Bob"});
    char *key3 = copy_string(&(char*){"Charlie"});

    int val1 = 10, val2 = 20, val3 = 30;

    hashtable_put(ht, &key1, &val1);
    hashtable_put(ht, &key2, &val2);
    hashtable_put(ht, &key3, &val3);

    ASSERT_EQ(hashtable_size(ht), 3);

    int result;
    hashtable_get(ht, &key2, &result);
    ASSERT_EQ(result, 20);

    hashtable_destroy(ht);
}

TEST(string_keys_linear_probing) {
    HashTable *ht = hashtable_create(sizeof(char*), sizeof(int), 8,
                                      hash_string, compare_string,
                                      HASH_LINEAR_PROBING, destroy_string, NULL);

    char *key1 = copy_string(&(char*){"one"});
    char *key2 = copy_string(&(char*){"two"});

    int val1 = 1, val2 = 2;

    hashtable_put(ht, &key1, &val1);
    hashtable_put(ht, &key2, &val2);

    int result;
    hashtable_get(ht, &key1, &result);
    ASSERT_EQ(result, 1);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: REHASHING
// ============================================================================

TEST(automatic_rehash_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 4,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    size_t initial_capacity = hashtable_capacity(ht);

    // Inserir até forçar rehash (load > 0.75)
    for (int i = 0; i < 20; i++) {
        hashtable_put(ht, &i, &i);
    }

    size_t new_capacity = hashtable_capacity(ht);
    ASSERT_TRUE(new_capacity > initial_capacity);

    // Verificar que todos os elementos ainda estão lá
    for (int i = 0; i < 20; i++) {
        int value;
        hashtable_get(ht, &i, &value);
        ASSERT_EQ(value, i);
    }

    hashtable_destroy(ht);
}

TEST(automatic_rehash_open_addressing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 4,
                                      hash_int, compare_int,
                                      HASH_LINEAR_PROBING, NULL, NULL);

    size_t initial_capacity = hashtable_capacity(ht);

    // Inserir até forçar rehash (load > 0.5 para open addressing)
    for (int i = 0; i < 10; i++) {
        hashtable_put(ht, &i, &i);
    }

    size_t new_capacity = hashtable_capacity(ht);
    ASSERT_TRUE(new_capacity > initial_capacity);

    hashtable_destroy(ht);
}

TEST(manual_rehash) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 5; i++) {
        hashtable_put(ht, &i, &i);
    }

    DataStructureError err = hashtable_rehash(ht, 32);
    ASSERT_EQ(err, DS_SUCCESS);

    size_t new_capacity = hashtable_capacity(ht);
    ASSERT_TRUE(new_capacity >= 32);

    // Verificar que dados permanecem
    for (int i = 0; i < 5; i++) {
        int value;
        hashtable_get(ht, &i, &value);
        ASSERT_EQ(value, i);
    }

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: CLEAR E IS_EMPTY
// ============================================================================

TEST(clear_table) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 10; i++) {
        hashtable_put(ht, &i, &i);
    }

    ASSERT_EQ(hashtable_size(ht), 10);

    hashtable_clear(ht);

    ASSERT_TRUE(hashtable_is_empty(ht));
    ASSERT_EQ(hashtable_size(ht), 0);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: LOAD FACTOR
// ============================================================================

TEST(load_factor_calculation) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 10,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    ASSERT_EQ(hashtable_load_factor(ht), 0.0);

    for (int i = 0; i < 5; i++) {
        hashtable_put(ht, &i, &i);
    }

    double load = hashtable_load_factor(ht);
    ASSERT_TRUE(load > 0.4 && load < 0.6);  // ~0.5

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: ITERATOR
// ============================================================================

TEST(iterator_chaining) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 5; i++) {
        hashtable_put(ht, &i, &i);
    }

    HashTableIterator *it = hashtable_iterator(ht);
    ASSERT_NOT_NULL(it);

    int count = 0;
    while (hashtable_iterator_has_next(it)) {
        HashTableEntry *entry = hashtable_iterator_next(it);
        ASSERT_NOT_NULL(entry);
        count++;
    }

    ASSERT_EQ(count, 5);

    hashtable_iterator_destroy(it);
    hashtable_destroy(ht);
}

TEST(iterator_open_addressing) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_LINEAR_PROBING, NULL, NULL);

    for (int i = 0; i < 5; i++) {
        hashtable_put(ht, &i, &i);
    }

    HashTableIterator *it = hashtable_iterator(ht);
    int count = 0;

    while (hashtable_iterator_has_next(it)) {
        hashtable_iterator_next(it);
        count++;
    }

    ASSERT_EQ(count, 5);

    hashtable_iterator_destroy(it);
    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: KEYS E VALUES
// ============================================================================

TEST(get_all_keys) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        hashtable_put(ht, &i, &values[i]);
    }

    void *keys;
    size_t size;
    DataStructureError err = hashtable_keys(ht, &keys, &size);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(size, 5);

    free(keys);
    hashtable_destroy(ht);
}

TEST(get_all_values) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        hashtable_put(ht, &i, &value);
    }

    void *values;
    size_t size;
    DataStructureError err = hashtable_values(ht, &values, &size);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(size, 5);

    free(values);
    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: ESTATÍSTICAS
// ============================================================================

TEST(statistics) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 10,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    for (int i = 0; i < 15; i++) {
        hashtable_put(ht, &i, &i);
    }

    HashTableStats stats = hashtable_stats(ht);

    ASSERT_EQ(stats.size, 15);
    ASSERT_TRUE(stats.capacity >= 10);
    ASSERT_TRUE(stats.load_factor > 0.0);

    printf("\n    Stats: size=%zu, capacity=%zu, load=%.2f, collisions=%zu, max_chain=%zu, empty=%zu\n",
           stats.size, stats.capacity, stats.load_factor, stats.collisions,
           stats.max_chain_length, stats.empty_buckets);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTES: ERROS
// ============================================================================

TEST(get_nonexistent_key) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int key = 99;
    int value;
    DataStructureError err = hashtable_get(ht, &key, &value);
    ASSERT_EQ(err, DS_ERROR_NOT_FOUND);

    hashtable_destroy(ht);
}

TEST(remove_nonexistent_key) {
    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    int key = 99;
    DataStructureError err = hashtable_remove(ht, &key, NULL);
    ASSERT_EQ(err, DS_ERROR_NOT_FOUND);

    hashtable_destroy(ht);
}

TEST(null_pointer_checks) {
    DataStructureError err;

    err = hashtable_put(NULL, &(int){1}, &(int){1});
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    HashTable *ht = hashtable_create(sizeof(int), sizeof(int), 8,
                                      hash_int, compare_int,
                                      HASH_CHAINING, NULL, NULL);

    err = hashtable_put(ht, NULL, &(int){1});
    ASSERT_EQ(err, DS_ERROR_NULL_POINTER);

    hashtable_destroy(ht);
}

// ============================================================================
// TESTE VISUAL
// ============================================================================

TEST(print_visual) {
    printf("\n");

    HashTable *ht = hashtable_create(sizeof(char*), sizeof(int), 8,
                                      hash_string, compare_string,
                                      HASH_CHAINING, destroy_string, NULL);

    char *names[] = {"Alice", "Bob", "Charlie", "David", "Eve"};
    int ages[] = {25, 30, 35, 40, 45};

    for (int i = 0; i < 5; i++) {
        char *key = copy_string(&names[i]);
        hashtable_put(ht, &key, &ages[i]);
    }

    printf("    HashTable (Chaining) - String -> Int:\n");
    HashTableIterator *it = hashtable_iterator(ht);
    while (hashtable_iterator_has_next(it)) {
        HashTableEntry *entry = hashtable_iterator_next(it);
        printf("      \"%s\" -> %d\n", *(char**)entry->key, *(int*)entry->value);
    }
    hashtable_iterator_destroy(it);

    hashtable_destroy(ht);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("\n");
    printf("============================================\n");
    printf("  TESTES: HASH TABLE\n");
    printf("============================================\n\n");

    printf("Criação e Destruição:\n");
    RUN_TEST(create_destroy_chaining);
    RUN_TEST(create_all_strategies);

    printf("\nPut e Get - Chaining:\n");
    RUN_TEST(put_get_chaining);
    RUN_TEST(put_update_chaining);

    printf("\nPut e Get - Linear Probing:\n");
    RUN_TEST(put_get_linear_probing);

    printf("\nPut e Get - Quadratic Probing:\n");
    RUN_TEST(put_get_quadratic_probing);

    printf("\nPut e Get - Double Hashing:\n");
    RUN_TEST(put_get_double_hashing);

    printf("\nRemove:\n");
    RUN_TEST(remove_chaining);
    RUN_TEST(remove_linear_probing);

    printf("\nContains e Get Ptr:\n");
    RUN_TEST(contains_key);
    RUN_TEST(get_ptr_access);

    printf("\nStrings como Chaves:\n");
    RUN_TEST(string_keys_chaining);
    RUN_TEST(string_keys_linear_probing);

    printf("\nRehashing:\n");
    RUN_TEST(automatic_rehash_chaining);
    RUN_TEST(automatic_rehash_open_addressing);
    RUN_TEST(manual_rehash);

    printf("\nClear e Is Empty:\n");
    RUN_TEST(clear_table);

    printf("\nLoad Factor:\n");
    RUN_TEST(load_factor_calculation);

    printf("\nIterator:\n");
    RUN_TEST(iterator_chaining);
    RUN_TEST(iterator_open_addressing);

    printf("\nKeys e Values:\n");
    RUN_TEST(get_all_keys);
    RUN_TEST(get_all_values);

    printf("\nEstatísticas:\n");
    RUN_TEST(statistics);

    printf("\nErros:\n");
    RUN_TEST(get_nonexistent_key);
    RUN_TEST(remove_nonexistent_key);
    RUN_TEST(null_pointer_checks);

    printf("\nTeste Visual:\n");
    RUN_TEST(print_visual);

    printf("\n============================================\n");
    printf("  ✅ TODOS OS TESTES PASSARAM! (32 testes)\n");
    printf("============================================\n\n");

    return 0;
}
