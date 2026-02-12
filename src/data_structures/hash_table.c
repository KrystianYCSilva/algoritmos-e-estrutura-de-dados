/**
 * @file hash_table.c
 * @brief Implementação de Hash Table genérica com múltiplas estratégias
 *
 * Implementa 4 estratégias de resolução de colisões:
 * 1. Chaining (Separate Chaining)
 * 2. Linear Probing
 * 3. Quadratic Probing
 * 4. Double Hashing
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 11 - Hash Tables
 * - Knuth (1998), TAOCP Vol 3, Section 6.4
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

/**
 * @brief Nó para chaining
 */
typedef struct ChainNode {
    void *key;
    void *value;
    struct ChainNode *next;
} ChainNode;

/**
 * @brief Entry para open addressing
 */
typedef struct {
    void *key;
    void *value;
    bool occupied;      // Slot está ocupado?
    bool deleted;       // Slot foi deletado? (para probing)
} OpenAddressEntry;

/**
 * @brief Estrutura da Hash Table
 */
struct HashTable {
    size_t key_size;
    size_t value_size;
    size_t size;           // Número de elementos
    size_t capacity;       // Capacidade da tabela

    CollisionStrategy strategy;
    HashFn hash_fn;
    CompareFn compare_fn;
    DestroyFn destroy_key;
    DestroyFn destroy_value;

    // Para CHAINING
    ChainNode **buckets;

    // Para OPEN ADDRESSING
    OpenAddressEntry *entries;
};

/**
 * @brief Estrutura do Iterator
 */
struct HashTableIterator {
    const HashTable *table;
    size_t current_bucket;
    ChainNode *current_node;  // Para chaining
    HashTableEntry entry;
};

// ============================================================================
// FUNÇÕES AUXILIARES - NÚMEROS PRIMOS
// ============================================================================

/**
 * @brief Verifica se um número é primo
 */
static bool is_prime(size_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    size_t limit = (size_t)sqrt((double)n);
    for (size_t i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Retorna o próximo primo >= n
 *
 * Usar capacidade prima reduz colisões.
 * Referência: Knuth TAOCP Vol 3, Section 6.4
 */
static size_t next_prime(size_t n) {
    if (n < 2) return 2;
    if (n == 2) return 2;

    // Se par, torna ímpar
    if (n % 2 == 0) n++;

    while (!is_prime(n)) {
        n += 2;
    }

    return n;
}

// ============================================================================
// FUNÇÕES HASH AUXILIARES
// ============================================================================

/**
 * @brief Hash primário: hash_fn(key) % capacity
 */
static size_t hash_primary(const HashTable *table, const void *key) {
    size_t h = table->hash_fn(key);
    return h % table->capacity;
}

/**
 * @brief Hash secundário para double hashing
 *
 * h2(k) = 1 + (k mod (m-1))
 * Garante que h2(k) seja relativamente primo com m
 */
static size_t hash_secondary(const HashTable *table, const void *key) {
    size_t h = table->hash_fn(key);
    return 1 + (h % (table->capacity - 1));
}

/**
 * @brief Calcula índice para probing
 *
 * Linear Probing: h(k, i) = (h1(k) + i) mod m
 * Quadratic Probing: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
 * Double Hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
 */
static size_t probe_index(const HashTable *table, const void *key, size_t i) {
    size_t h1 = hash_primary(table, key);

    switch (table->strategy) {
        case HASH_LINEAR_PROBING:
            return (h1 + i) % table->capacity;

        case HASH_QUADRATIC_PROBING:
            // c1 = c2 = 1/2 (Cormen p. 273)
            return (h1 + i + i*i) % table->capacity;

        case HASH_DOUBLE_HASHING: {
            size_t h2 = hash_secondary(table, key);
            return (h1 + i * h2) % table->capacity;
        }

        default:
            return h1;
    }
}

// ============================================================================
// FUNÇÕES AUXILIARES - CHAINING
// ============================================================================

/**
 * @brief Cria um nó de chain
 */
static ChainNode* chainnode_create(const HashTable *table, const void *key, const void *value) {
    ChainNode *node = (ChainNode*)malloc(sizeof(ChainNode));
    if (node == NULL) {
        return NULL;
    }

    node->key = malloc(table->key_size);
    node->value = malloc(table->value_size);

    if (node->key == NULL || node->value == NULL) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }

    memcpy(node->key, key, table->key_size);
    memcpy(node->value, value, table->value_size);
    node->next = NULL;

    return node;
}

/**
 * @brief Destrói um nó de chain
 */
static void chainnode_destroy(ChainNode *node, DestroyFn destroy_key, DestroyFn destroy_value) {
    if (node == NULL) {
        return;
    }

    if (destroy_key != NULL) {
        destroy_key(node->key);
    }
    if (destroy_value != NULL) {
        destroy_value(node->value);
    }

    free(node->key);
    free(node->value);
    free(node);
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

HashTable* hashtable_create(size_t key_size, size_t value_size, size_t initial_capacity,
                            HashFn hash_fn, CompareFn compare_fn,
                            CollisionStrategy strategy,
                            DestroyFn destroy_key, DestroyFn destroy_value) {
    if (key_size == 0 || value_size == 0 || hash_fn == NULL || compare_fn == NULL) {
        return NULL;
    }

    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;
    }

    // Capacidade deve ser prima
    size_t capacity = next_prime(initial_capacity > 0 ? initial_capacity : 17);

    table->key_size = key_size;
    table->value_size = value_size;
    table->size = 0;
    table->capacity = capacity;
    table->strategy = strategy;
    table->hash_fn = hash_fn;
    table->compare_fn = compare_fn;
    table->destroy_key = destroy_key;
    table->destroy_value = destroy_value;

    if (strategy == HASH_CHAINING) {
        table->buckets = (ChainNode**)calloc(capacity, sizeof(ChainNode*));
        table->entries = NULL;

        if (table->buckets == NULL) {
            free(table);
            return NULL;
        }
    } else {
        table->buckets = NULL;
        table->entries = (OpenAddressEntry*)calloc(capacity, sizeof(OpenAddressEntry));

        if (table->entries == NULL) {
            free(table);
            return NULL;
        }
    }

    return table;
}

void hashtable_destroy(HashTable *table) {
    if (table == NULL) {
        return;
    }

    hashtable_clear(table);

    if (table->strategy == HASH_CHAINING) {
        free(table->buckets);
    } else {
        free(table->entries);
    }

    free(table);
}

// ============================================================================
// OPERAÇÕES - CHAINING
// ============================================================================

/**
 * @brief Insere em chaining
 *
 * Pseudocódigo (Cormen p. 258):
 * CHAINED-HASH-INSERT(T, x)
 *   insert x at the head of list T[h(x.key)]
 */
static DataStructureError hashtable_put_chaining(HashTable *table, const void *key, const void *value) {
    size_t index = hash_primary(table, key);

    // Verificar se chave já existe
    ChainNode *current = table->buckets[index];
    while (current != NULL) {
        if (table->compare_fn(current->key, key) == 0) {
            // Atualizar valor existente
            if (table->destroy_value != NULL) {
                table->destroy_value(current->value);
            }
            memcpy(current->value, value, table->value_size);
            return DS_SUCCESS;
        }
        current = current->next;
    }

    // Inserir novo nó no início da lista
    ChainNode *new_node = chainnode_create(table, key, value);
    if (new_node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;

    return DS_SUCCESS;
}

/**
 * @brief Busca em chaining
 */
static DataStructureError hashtable_get_chaining(const HashTable *table, const void *key, void *value) {
    size_t index = hash_primary(table, key);

    ChainNode *current = table->buckets[index];
    while (current != NULL) {
        if (table->compare_fn(current->key, key) == 0) {
            if (value != NULL) {
                memcpy(value, current->value, table->value_size);
            }
            return DS_SUCCESS;
        }
        current = current->next;
    }

    return DS_ERROR_NOT_FOUND;
}

/**
 * @brief Remove em chaining
 */
static DataStructureError hashtable_remove_chaining(HashTable *table, const void *key, void *old_value) {
    size_t index = hash_primary(table, key);

    ChainNode *current = table->buckets[index];
    ChainNode *prev = NULL;

    while (current != NULL) {
        if (table->compare_fn(current->key, key) == 0) {
            // Encontrado
            if (old_value != NULL) {
                memcpy(old_value, current->value, table->value_size);
            }

            // Remover da lista
            if (prev == NULL) {
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            chainnode_destroy(current, table->destroy_key, table->destroy_value);
            table->size--;
            return DS_SUCCESS;
        }

        prev = current;
        current = current->next;
    }

    return DS_ERROR_NOT_FOUND;
}

// ============================================================================
// OPERAÇÕES - OPEN ADDRESSING
// ============================================================================

/**
 * @brief Insere em open addressing
 *
 * Pseudocódigo (Cormen p. 271):
 * HASH-INSERT(T, k)
 *   i = 0
 *   repeat
 *       j = h(k, i)
 *       if T[j] == NIL
 *           T[j] = k
 *           return j
 *       else i = i + 1
 *   until i == m
 */
static DataStructureError hashtable_put_open(HashTable *table, const void *key, const void *value) {
    // Verificar se chave já existe ou encontrar slot vazio
    for (size_t i = 0; i < table->capacity; i++) {
        size_t index = probe_index(table, key, i);
        OpenAddressEntry *entry = &table->entries[index];

        if (!entry->occupied || entry->deleted) {
            // Slot disponível: vazio ou deletado
            if (!entry->occupied) {
                entry->key = malloc(table->key_size);
                entry->value = malloc(table->value_size);

                if (entry->key == NULL || entry->value == NULL) {
                    free(entry->key);
                    free(entry->value);
                    return DS_ERROR_OUT_OF_MEMORY;
                }
            }

            memcpy(entry->key, key, table->key_size);
            memcpy(entry->value, value, table->value_size);
            entry->occupied = true;
            entry->deleted = false;
            table->size++;
            return DS_SUCCESS;
        }

        if (table->compare_fn(entry->key, key) == 0) {
            // Chave já existe: atualizar
            if (table->destroy_value != NULL) {
                table->destroy_value(entry->value);
            }
            memcpy(entry->value, value, table->value_size);
            return DS_SUCCESS;
        }
    }

    // Tabela cheia
    return DS_ERROR_FULL;
}

/**
 * @brief Busca em open addressing
 */
static DataStructureError hashtable_get_open(const HashTable *table, const void *key, void *value) {
    for (size_t i = 0; i < table->capacity; i++) {
        size_t index = probe_index(table, key, i);
        const OpenAddressEntry *entry = &table->entries[index];

        if (!entry->occupied && !entry->deleted) {
            // Slot vazio: chave não existe
            return DS_ERROR_NOT_FOUND;
        }

        if (entry->occupied && !entry->deleted &&
            table->compare_fn(entry->key, key) == 0) {
            if (value != NULL) {
                memcpy(value, entry->value, table->value_size);
            }
            return DS_SUCCESS;
        }
    }

    return DS_ERROR_NOT_FOUND;
}

/**
 * @brief Remove em open addressing
 *
 * Marca como deleted (lazy deletion) para não quebrar probing chains
 */
static DataStructureError hashtable_remove_open(HashTable *table, const void *key, void *old_value) {
    for (size_t i = 0; i < table->capacity; i++) {
        size_t index = probe_index(table, key, i);
        OpenAddressEntry *entry = &table->entries[index];

        if (!entry->occupied && !entry->deleted) {
            return DS_ERROR_NOT_FOUND;
        }

        if (entry->occupied && !entry->deleted &&
            table->compare_fn(entry->key, key) == 0) {
            if (old_value != NULL) {
                memcpy(old_value, entry->value, table->value_size);
            }

            // Lazy deletion
            entry->deleted = true;
            table->size--;
            return DS_SUCCESS;
        }
    }

    return DS_ERROR_NOT_FOUND;
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS (DISPATCHER)
// ============================================================================

DataStructureError hashtable_put(HashTable *table, const void *key, const void *value) {
    if (table == NULL || key == NULL || value == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    // Verificar se precisa rehash
    double load = hashtable_load_factor(table);
    double threshold = (table->strategy == HASH_CHAINING) ? 0.75 : 0.5;

    if (load > threshold) {
        size_t new_capacity = next_prime(table->capacity * 2);
        DataStructureError err = hashtable_rehash(table, new_capacity);
        if (err != DS_SUCCESS) {
            return err;
        }
    }

    if (table->strategy == HASH_CHAINING) {
        return hashtable_put_chaining(table, key, value);
    } else {
        return hashtable_put_open(table, key, value);
    }
}

DataStructureError hashtable_get(const HashTable *table, const void *key, void *value) {
    if (table == NULL || key == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (table->strategy == HASH_CHAINING) {
        return hashtable_get_chaining(table, key, value);
    } else {
        return hashtable_get_open(table, key, value);
    }
}

void* hashtable_get_ptr(const HashTable *table, const void *key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }

    if (table->strategy == HASH_CHAINING) {
        size_t index = hash_primary(table, key);
        ChainNode *current = table->buckets[index];

        while (current != NULL) {
            if (table->compare_fn(current->key, key) == 0) {
                return current->value;
            }
            current = current->next;
        }
    } else {
        for (size_t i = 0; i < table->capacity; i++) {
            size_t index = probe_index(table, key, i);
            OpenAddressEntry *entry = &table->entries[index];

            if (!entry->occupied && !entry->deleted) {
                break;
            }

            if (entry->occupied && !entry->deleted &&
                table->compare_fn(entry->key, key) == 0) {
                return entry->value;
            }
        }
    }

    return NULL;
}

DataStructureError hashtable_remove(HashTable *table, const void *key, void *old_value) {
    if (table == NULL || key == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (table->strategy == HASH_CHAINING) {
        return hashtable_remove_chaining(table, key, old_value);
    } else {
        return hashtable_remove_open(table, key, old_value);
    }
}

bool hashtable_contains(const HashTable *table, const void *key) {
    return (hashtable_get(table, key, NULL) == DS_SUCCESS);
}

// ============================================================================
// CONSULTAS
// ============================================================================

bool hashtable_is_empty(const HashTable *table) {
    return (table == NULL || table->size == 0);
}

size_t hashtable_size(const HashTable *table) {
    return (table == NULL) ? 0 : table->size;
}

size_t hashtable_capacity(const HashTable *table) {
    return (table == NULL) ? 0 : table->capacity;
}

double hashtable_load_factor(const HashTable *table) {
    if (table == NULL || table->capacity == 0) {
        return 0.0;
    }
    return (double)table->size / (double)table->capacity;
}

void hashtable_clear(HashTable *table) {
    if (table == NULL) {
        return;
    }

    if (table->strategy == HASH_CHAINING) {
        for (size_t i = 0; i < table->capacity; i++) {
            ChainNode *current = table->buckets[i];
            while (current != NULL) {
                ChainNode *next = current->next;
                chainnode_destroy(current, table->destroy_key, table->destroy_value);
                current = next;
            }
            table->buckets[i] = NULL;
        }
    } else {
        for (size_t i = 0; i < table->capacity; i++) {
            OpenAddressEntry *entry = &table->entries[i];
            if (entry->occupied) {
                if (table->destroy_key != NULL) {
                    table->destroy_key(entry->key);
                }
                if (table->destroy_value != NULL) {
                    table->destroy_value(entry->value);
                }
                free(entry->key);
                free(entry->value);
                entry->occupied = false;
                entry->deleted = false;
            }
        }
    }

    table->size = 0;
}

// ============================================================================
// REHASHING
// ============================================================================

DataStructureError hashtable_rehash(HashTable *table, size_t new_capacity) {
    if (table == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    new_capacity = next_prime(new_capacity);

    // Salvar estado antigo
    ChainNode **old_buckets = table->buckets;
    OpenAddressEntry *old_entries = table->entries;
    size_t old_capacity = table->capacity;

    // Criar novo estado
    table->capacity = new_capacity;
    table->size = 0;

    if (table->strategy == HASH_CHAINING) {
        table->buckets = (ChainNode**)calloc(new_capacity, sizeof(ChainNode*));
        if (table->buckets == NULL) {
            table->buckets = old_buckets;
            table->capacity = old_capacity;
            return DS_ERROR_OUT_OF_MEMORY;
        }

        // Reinserir elementos
        for (size_t i = 0; i < old_capacity; i++) {
            ChainNode *current = old_buckets[i];
            while (current != NULL) {
                hashtable_put_chaining(table, current->key, current->value);
                ChainNode *next = current->next;
                chainnode_destroy(current, table->destroy_key, table->destroy_value);
                current = next;
            }
        }

        free(old_buckets);

    } else {
        table->entries = (OpenAddressEntry*)calloc(new_capacity, sizeof(OpenAddressEntry));
        if (table->entries == NULL) {
            table->entries = old_entries;
            table->capacity = old_capacity;
            return DS_ERROR_OUT_OF_MEMORY;
        }

        // Reinserir elementos
        for (size_t i = 0; i < old_capacity; i++) {
            OpenAddressEntry *entry = &old_entries[i];
            if (entry->occupied && !entry->deleted) {
                hashtable_put_open(table, entry->key, entry->value);

                if (table->destroy_key != NULL) {
                    table->destroy_key(entry->key);
                }
                if (table->destroy_value != NULL) {
                    table->destroy_value(entry->value);
                }
                free(entry->key);
                free(entry->value);
            }
        }

        free(old_entries);
    }

    return DS_SUCCESS;
}

// ============================================================================
// ITERADOR
// ============================================================================

HashTableIterator* hashtable_iterator(const HashTable *table) {
    if (table == NULL) {
        return NULL;
    }

    HashTableIterator *iter = (HashTableIterator*)malloc(sizeof(HashTableIterator));
    if (iter == NULL) {
        return NULL;
    }

    iter->table = table;
    iter->current_bucket = 0;
    iter->current_node = NULL;

    // Para chaining, posicionar no primeiro nó não-vazio
    if (table->strategy == HASH_CHAINING) {
        while (iter->current_bucket < table->capacity) {
            if (table->buckets[iter->current_bucket] != NULL) {
                iter->current_node = table->buckets[iter->current_bucket];
                break;
            }
            iter->current_bucket++;
        }
    }

    return iter;
}

bool hashtable_iterator_has_next(HashTableIterator *iter) {
    if (iter == NULL || iter->table == NULL) {
        return false;
    }

    if (iter->table->strategy == HASH_CHAINING) {
        return (iter->current_bucket < iter->table->capacity);
    } else {
        // Open addressing: encontrar próximo slot ocupado
        for (size_t i = iter->current_bucket; i < iter->table->capacity; i++) {
            const OpenAddressEntry *entry = &iter->table->entries[i];
            if (entry->occupied && !entry->deleted) {
                return true;
            }
        }
        return false;
    }
}

HashTableEntry* hashtable_iterator_next(HashTableIterator *iter) {
    if (iter == NULL || iter->table == NULL) {
        return NULL;
    }

    if (iter->table->strategy == HASH_CHAINING) {
        if (iter->current_node != NULL) {
            iter->entry.key = iter->current_node->key;
            iter->entry.value = iter->current_node->value;

            // Avançar
            iter->current_node = iter->current_node->next;

            // Se fim da chain, próximo bucket
            if (iter->current_node == NULL) {
                iter->current_bucket++;
                while (iter->current_bucket < iter->table->capacity) {
                    if (iter->table->buckets[iter->current_bucket] != NULL) {
                        iter->current_node = iter->table->buckets[iter->current_bucket];
                        break;
                    }
                    iter->current_bucket++;
                }
            }

            return &iter->entry;
        }

    } else {
        // Open addressing
        for (size_t i = iter->current_bucket; i < iter->table->capacity; i++) {
            const OpenAddressEntry *entry = &iter->table->entries[i];
            if (entry->occupied && !entry->deleted) {
                iter->entry.key = entry->key;
                iter->entry.value = entry->value;
                iter->current_bucket = i + 1;
                return &iter->entry;
            }
        }
    }

    return NULL;
}

void hashtable_iterator_destroy(HashTableIterator *iter) {
    free(iter);
}

// ============================================================================
// ESTATÍSTICAS
// ============================================================================

HashTableStats hashtable_stats(const HashTable *table) {
    HashTableStats stats = {0};

    if (table == NULL) {
        return stats;
    }

    stats.size = table->size;
    stats.capacity = table->capacity;
    stats.load_factor = hashtable_load_factor(table);

    if (table->strategy == HASH_CHAINING) {
        for (size_t i = 0; i < table->capacity; i++) {
            ChainNode *current = table->buckets[i];
            size_t chain_length = 0;

            while (current != NULL) {
                chain_length++;
                current = current->next;
            }

            if (chain_length == 0) {
                stats.empty_buckets++;
            } else if (chain_length > 1) {
                stats.collisions += (chain_length - 1);
            }

            if (chain_length > stats.max_chain_length) {
                stats.max_chain_length = chain_length;
            }
        }
    } else {
        for (size_t i = 0; i < table->capacity; i++) {
            if (!table->entries[i].occupied) {
                stats.empty_buckets++;
            }
        }
    }

    return stats;
}

// ============================================================================
// KEYS E VALUES
// ============================================================================

DataStructureError hashtable_keys(const HashTable *table, void **keys, size_t *size) {
    if (table == NULL || keys == NULL || size == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (table->size == 0) {
        *keys = NULL;
        *size = 0;
        return DS_SUCCESS;
    }

    void *key_array = malloc(table->size * table->key_size);
    if (key_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t count = 0;

    if (table->strategy == HASH_CHAINING) {
        for (size_t i = 0; i < table->capacity; i++) {
            ChainNode *current = table->buckets[i];
            while (current != NULL) {
                void *dest = (char*)key_array + (count * table->key_size);
                memcpy(dest, current->key, table->key_size);
                count++;
                current = current->next;
            }
        }
    } else {
        for (size_t i = 0; i < table->capacity; i++) {
            const OpenAddressEntry *entry = &table->entries[i];
            if (entry->occupied && !entry->deleted) {
                void *dest = (char*)key_array + (count * table->key_size);
                memcpy(dest, entry->key, table->key_size);
                count++;
            }
        }
    }

    *keys = key_array;
    *size = count;
    return DS_SUCCESS;
}

DataStructureError hashtable_values(const HashTable *table, void **values, size_t *size) {
    if (table == NULL || values == NULL || size == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (table->size == 0) {
        *values = NULL;
        *size = 0;
        return DS_SUCCESS;
    }

    void *value_array = malloc(table->size * table->value_size);
    if (value_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t count = 0;

    if (table->strategy == HASH_CHAINING) {
        for (size_t i = 0; i < table->capacity; i++) {
            ChainNode *current = table->buckets[i];
            while (current != NULL) {
                void *dest = (char*)value_array + (count * table->value_size);
                memcpy(dest, current->value, table->value_size);
                count++;
                current = current->next;
            }
        }
    } else {
        for (size_t i = 0; i < table->capacity; i++) {
            const OpenAddressEntry *entry = &table->entries[i];
            if (entry->occupied && !entry->deleted) {
                void *dest = (char*)value_array + (count * table->value_size);
                memcpy(dest, entry->value, table->value_size);
                count++;
            }
        }
    }

    *values = value_array;
    *size = count;
    return DS_SUCCESS;
}
