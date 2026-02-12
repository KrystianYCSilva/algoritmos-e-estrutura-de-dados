/**
 * @file hash_table.h
 * @brief Implementação genérica de Tabela Hash (Hash Table / Hash Map)
 *
 * Hash Table é uma estrutura de dados que mapeia chaves para valores,
 * oferecendo operações de inserção, busca e remoção em tempo O(1) amortizado.
 *
 * Complexidade das Operações (amortizada):
 * - Inserção (put): O(1)
 * - Busca (get): O(1)
 * - Remoção (remove): O(1)
 * - No pior caso (muitas colisões): O(n)
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 11 - Hash Tables
 * - Knuth, D. E. (1998). "The Art of Computer Programming, Vol 3", Section 6.4
 *   "Hashing" - Análise detalhada de funções hash
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Section 3.4
 *
 * Estratégias de Colisão:
 * 1. Chaining (Separate Chaining): Usa listas encadeadas
 * 2. Open Addressing: Linear Probing, Quadratic Probing, Double Hashing
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURA DA HASH TABLE (OPACA)
// ============================================================================

/**
 * @brief Estrutura opaca da tabela hash
 */
typedef struct HashTable HashTable;

/**
 * @brief Estratégias de resolução de colisões
 */
typedef enum {
    HASH_CHAINING,           /**< Separate chaining com listas encadeadas */
    HASH_LINEAR_PROBING,     /**< Open addressing com linear probing */
    HASH_QUADRATIC_PROBING,  /**< Open addressing com quadratic probing */
    HASH_DOUBLE_HASHING      /**< Open addressing com double hashing */
} CollisionStrategy;

// ============================================================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova tabela hash
 *
 * @param key_size Tamanho da chave em bytes
 * @param value_size Tamanho do valor em bytes
 * @param initial_capacity Capacidade inicial (será ajustada para próximo primo)
 * @param hash_fn Função hash para as chaves
 * @param compare_fn Função de comparação de chaves
 * @param strategy Estratégia de resolução de colisões
 * @param destroy_key Função para destruir chaves (NULL se não necessário)
 * @param destroy_value Função para destruir valores (NULL se não necessário)
 * @return HashTable* Ponteiro para a tabela criada, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * // Hash table string -> int
 * HashTable *ht = hashtable_create(sizeof(char*), sizeof(int), 16,
 *                                   hash_string, compare_string,
 *                                   HASH_CHAINING, destroy_string, NULL);
 * @endcode
 *
 * Load Factor: rehash automático quando load > 0.75 (chaining) ou > 0.5 (open addressing)
 *
 * Complexidade: O(capacity)
 */
HashTable* hashtable_create(size_t key_size, size_t value_size, size_t initial_capacity,
                            HashFn hash_fn, CompareFn compare_fn,
                            CollisionStrategy strategy,
                            DestroyFn destroy_key, DestroyFn destroy_value);

/**
 * @brief Destrói a tabela hash e libera memória
 *
 * @param table Ponteiro para a tabela
 *
 * Complexidade: O(n + capacity)
 */
void hashtable_destroy(HashTable *table);

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * @brief Insere ou atualiza um par chave-valor
 *
 * @param table Ponteiro para a tabela
 * @param key Ponteiro para a chave
 * @param value Ponteiro para o valor
 * @return DataStructureError Código de erro
 *
 * Se a chave já existe, o valor é atualizado.
 * Se load factor > threshold, rehashing automático é executado.
 *
 * Pseudocódigo (Cormen et al., 2009, p. 258 - Chaining):
 * CHAINED-HASH-INSERT(T, x)
 *   insert x at the head of list T[h(x.key)]
 *
 * Pseudocódigo (Cormen et al., 2009, p. 271 - Open Addressing):
 * HASH-INSERT(T, k)
 *   i = 0
 *   repeat
 *       j = h(k, i)
 *       if T[j] == NIL
 *           T[j] = k
 *           return j
 *       else i = i + 1
 *   until i == m
 *   error "hash table overflow"
 *
 * Complexidade: O(1) amortizado
 */
DataStructureError hashtable_put(HashTable *table, const void *key, const void *value);

/**
 * @brief Busca um valor pela chave
 *
 * @param table Ponteiro para a tabela
 * @param key Ponteiro para a chave
 * @param value Buffer para armazenar o valor encontrado
 * @return DataStructureError DS_SUCCESS ou DS_ERROR_NOT_FOUND
 *
 * Pseudocódigo (Cormen et al., 2009, p. 258 - Chaining):
 * CHAINED-HASH-SEARCH(T, k)
 *   search for an element with key k in list T[h(k)]
 *
 * Pseudocódigo (Cormen et al., 2009, p. 271 - Open Addressing):
 * HASH-SEARCH(T, k)
 *   i = 0
 *   repeat
 *       j = h(k, i)
 *       if T[j].key == k
 *           return j
 *       i = i + 1
 *   until T[j] == NIL or i == m
 *   return NIL
 *
 * Complexidade: O(1) esperado, O(n) pior caso
 */
DataStructureError hashtable_get(const HashTable *table, const void *key, void *value);

/**
 * @brief Retorna ponteiro direto para o valor (uso avançado)
 *
 * @param table Ponteiro para a tabela
 * @param key Ponteiro para a chave
 * @return void* Ponteiro para o valor, ou NULL se não encontrado
 *
 * AVISO: Ponteiro pode ser invalidado após rehashing
 *
 * Complexidade: O(1) esperado
 */
void* hashtable_get_ptr(const HashTable *table, const void *key);

/**
 * @brief Remove um par chave-valor
 *
 * @param table Ponteiro para a tabela
 * @param key Ponteiro para a chave
 * @param old_value Buffer para armazenar o valor removido (NULL para descartar)
 * @return DataStructureError DS_SUCCESS ou DS_ERROR_NOT_FOUND
 *
 * Pseudocódigo (Cormen et al., 2009, p. 258 - Chaining):
 * CHAINED-HASH-DELETE(T, x)
 *   delete x from the list T[h(x.key)]
 *
 * Complexidade: O(1) esperado
 */
DataStructureError hashtable_remove(HashTable *table, const void *key, void *old_value);

/**
 * @brief Verifica se uma chave existe
 *
 * @param table Ponteiro para a tabela
 * @param key Ponteiro para a chave
 * @return true se existe, false caso contrário
 *
 * Complexidade: O(1) esperado
 */
bool hashtable_contains(const HashTable *table, const void *key);

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

/**
 * @brief Verifica se a tabela está vazia
 *
 * @param table Ponteiro para a tabela
 * @return true se vazia, false caso contrário
 *
 * Complexidade: O(1)
 */
bool hashtable_is_empty(const HashTable *table);

/**
 * @brief Retorna o número de pares chave-valor
 *
 * @param table Ponteiro para a tabela
 * @return size_t Número de elementos
 *
 * Complexidade: O(1)
 */
size_t hashtable_size(const HashTable *table);

/**
 * @brief Retorna a capacidade atual da tabela
 *
 * @param table Ponteiro para a tabela
 * @return size_t Capacidade
 *
 * Complexidade: O(1)
 */
size_t hashtable_capacity(const HashTable *table);

/**
 * @brief Retorna o load factor (size / capacity)
 *
 * @param table Ponteiro para a tabela
 * @return double Load factor
 *
 * Load Factor:
 * - Chaining: pode ser > 1.0
 * - Open Addressing: sempre < 1.0
 * - Rehash: quando > 0.75 (chaining) ou > 0.5 (open addressing)
 *
 * Complexidade: O(1)
 */
double hashtable_load_factor(const HashTable *table);

/**
 * @brief Remove todos os pares chave-valor
 *
 * @param table Ponteiro para a tabela
 *
 * Complexidade: O(n + capacity)
 */
void hashtable_clear(HashTable *table);

// ============================================================================
// OPERAÇÕES DE ITERAÇÃO
// ============================================================================

/**
 * @brief Iterator opaco para percorrer a tabela
 */
typedef struct HashTableIterator HashTableIterator;

/**
 * @brief Par chave-valor retornado pelo iterator
 */
typedef struct {
    void *key;
    void *value;
} HashTableEntry;

/**
 * @brief Cria um iterator para percorrer a tabela
 *
 * @param table Ponteiro para a tabela
 * @return HashTableIterator* Iterator, ou NULL em caso de erro
 *
 * Exemplo:
 * @code
 * HashTableIterator *it = hashtable_iterator(table);
 * while (hashtable_iterator_has_next(it)) {
 *     HashTableEntry *entry = hashtable_iterator_next(it);
 *     printf("Key: %s, Value: %d\n", (char*)entry->key, *(int*)entry->value);
 * }
 * hashtable_iterator_destroy(it);
 * @endcode
 *
 * Complexidade: O(1)
 */
HashTableIterator* hashtable_iterator(const HashTable *table);

/**
 * @brief Verifica se há mais elementos
 *
 * @param iter Ponteiro para o iterator
 * @return true se há próximo elemento
 *
 * Complexidade: O(1) amortizado
 */
bool hashtable_iterator_has_next(HashTableIterator *iter);

/**
 * @brief Retorna o próximo par chave-valor
 *
 * @param iter Ponteiro para o iterator
 * @return HashTableEntry* Ponteiro para o entry, ou NULL se fim
 *
 * Complexidade: O(1) amortizado
 */
HashTableEntry* hashtable_iterator_next(HashTableIterator *iter);

/**
 * @brief Destrói o iterator
 *
 * @param iter Ponteiro para o iterator
 *
 * Complexidade: O(1)
 */
void hashtable_iterator_destroy(HashTableIterator *iter);

// ============================================================================
// OPERAÇÕES AVANÇADAS
// ============================================================================

/**
 * @brief Força um rehashing da tabela
 *
 * @param table Ponteiro para a tabela
 * @param new_capacity Nova capacidade (será ajustada para próximo primo)
 * @return DataStructureError Código de erro
 *
 * Útil para pré-alocar espaço quando tamanho final é conhecido.
 *
 * Complexidade: O(n + old_capacity + new_capacity)
 */
DataStructureError hashtable_rehash(HashTable *table, size_t new_capacity);

/**
 * @brief Retorna array com todas as chaves
 *
 * @param table Ponteiro para a tabela
 * @param keys Ponteiro para array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho
 * @return DataStructureError Código de erro
 *
 * Array deve ser liberado com free() pelo caller.
 *
 * Complexidade: O(n + capacity)
 */
DataStructureError hashtable_keys(const HashTable *table, void **keys, size_t *size);

/**
 * @brief Retorna array com todos os valores
 *
 * @param table Ponteiro para a tabela
 * @param values Ponteiro para array de saída (será alocado)
 * @param size Ponteiro para armazenar o tamanho
 * @return DataStructureError Código de erro
 *
 * Array deve ser liberado com free() pelo caller.
 *
 * Complexidade: O(n + capacity)
 */
DataStructureError hashtable_values(const HashTable *table, void **values, size_t *size);

/**
 * @brief Estatísticas de performance da tabela
 */
typedef struct {
    size_t size;               /**< Número de elementos */
    size_t capacity;           /**< Capacidade */
    double load_factor;        /**< Load factor */
    size_t collisions;         /**< Número total de colisões */
    size_t max_chain_length;   /**< Tamanho da maior cadeia (chaining) */
    size_t empty_buckets;      /**< Número de buckets vazios */
} HashTableStats;

/**
 * @brief Retorna estatísticas da tabela
 *
 * @param table Ponteiro para a tabela
 * @return HashTableStats Estrutura com estatísticas
 *
 * Útil para análise de performance e debugging.
 *
 * Complexidade: O(capacity)
 */
HashTableStats hashtable_stats(const HashTable *table);

// ============================================================================
// FUNÇÕES HASH AUXILIARES
// ============================================================================

/**
 * @brief Função hash djb2 para strings (Dan Bernstein)
 *
 * @param data Ponteiro para a string (char*)
 * @return size_t Hash value
 *
 * Algoritmo: hash = hash * 33 + c
 * Referência: http://www.cse.yorku.ca/~oz/hash.html
 */
size_t hash_djb2(const void *data);

/**
 * @brief Função hash FNV-1a para dados binários
 *
 * @param data Ponteiro para os dados
 * @param size Tamanho dos dados
 * @return size_t Hash value
 *
 * Referência: http://www.isthe.com/chongo/tech/comp/fnv/
 */
size_t hash_fnv1a(const void *data, size_t size);

/**
 * @brief Função hash multiplicativa (Knuth)
 *
 * @param data Ponteiro para inteiro
 * @return size_t Hash value
 *
 * Usa a constante de ouro: φ = (√5 - 1) / 2 ≈ 0.618...
 * Referência: Knuth TAOCP Vol 3, Section 6.4
 */
size_t hash_multiplicative(const void *data);

#endif // HASH_TABLE_H
