/**
 * @file trie.h
 * @brief Trie (Prefix Tree) para strings
 *
 * Trie é uma árvore de recuperação eficiente para strings, onde cada nó
 * representa um caractere. Caminhos da raiz às folhas formam strings.
 *
 * Complexidade (m = tamanho da string):
 * - Insert: O(m)
 * - Search: O(m)
 * - Delete: O(m)
 * - Prefix search: O(m + k) onde k é número de resultados
 * - Espaço: O(ALPHABET_SIZE * m * n) onde n é número de strings
 *
 * vs Hash Table: Trie é melhor para prefix matching, autocomplete
 * vs BST: Trie é melhor quando muitas strings compartilham prefixos
 *
 * Aplicações:
 * - Autocomplete
 * - Spell checker
 * - IP routing (longest prefix match)
 * - T9 predictive text
 *
 * Referências:
 * - Knuth TAOCP Vol 3, Section 6.3 "Digital Searching"
 * - Fredkin, E. (1960). "Trie memory"
 * - Sedgewick & Wayne (2011), Section 5.2
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef TRIE_H
#define TRIE_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct Trie Trie;

/**
 * @brief Cria um novo trie
 *
 * @param alphabet_size Tamanho do alfabeto (256 para ASCII, 26 para [a-z])
 */
Trie* trie_create(size_t alphabet_size);
void trie_destroy(Trie *trie);

/**
 * @brief Insere uma string no trie
 *
 * @param trie Ponteiro para o trie
 * @param str String a inserir (null-terminated)
 * @return DataStructureError Código de erro
 *
 * Algoritmo:
 * 1. Começar na raiz
 * 2. Para cada caractere c em str:
 *    - Se filho[c] não existe, criar novo nó
 *    - Mover para filho[c]
 * 3. Marcar último nó como fim de palavra
 *
 * Complexidade: O(m) onde m = |str|
 */
DataStructureError trie_insert(Trie *trie, const char *str);

/**
 * @brief Busca uma string no trie
 *
 * @param trie Ponteiro para o trie
 * @param str String a buscar
 * @return true se encontrada, false caso contrário
 *
 * Complexidade: O(m)
 */
bool trie_search(const Trie *trie, const char *str);

/**
 * @brief Verifica se existe alguma string com o prefixo dado
 *
 * @param trie Ponteiro para o trie
 * @param prefix Prefixo a verificar
 * @return true se existe, false caso contrário
 *
 * Diferença de search: retorna true mesmo se prefix não é palavra completa,
 * mas é prefixo de alguma palavra.
 *
 * Complexidade: O(m) onde m = |prefix|
 */
bool trie_starts_with(const Trie *trie, const char *prefix);

/**
 * @brief Remove uma string do trie
 *
 * @param trie Ponteiro para o trie
 * @param str String a remover
 * @return DataStructureError Código de erro
 *
 * Remove nós que não são mais necessários (não têm outros filhos).
 *
 * Complexidade: O(m)
 */
DataStructureError trie_remove(Trie *trie, const char *str);

/**
 * @brief Retorna todas as strings com um prefixo dado (autocomplete)
 *
 * @param trie Ponteiro para o trie
 * @param prefix Prefixo
 * @param results Array de strings (será alocado)
 * @param count Número de resultados
 * @return DataStructureError Código de erro
 *
 * Útil para autocomplete: digitar "pre" retorna ["prefix", "prepare", "present"]
 *
 * Complexidade: O(p + k) onde p = |prefix|, k = número de resultados
 */
DataStructureError trie_autocomplete(const Trie *trie, const char *prefix,
                                     char ***results, size_t *count);

/**
 * @brief Retorna a string com maior prefixo comum
 *
 * @param trie Ponteiro para o trie
 * @return char* Longest common prefix (deve ser liberado com free)
 *
 * Exemplo: se trie tem ["flower", "flow", "flight"], retorna "fl"
 */
char* trie_longest_common_prefix(const Trie *trie);

/**
 * @brief Conta número de strings no trie
 */
size_t trie_size(const Trie *trie);

/**
 * @brief Verifica se o trie está vazio
 */
bool trie_is_empty(const Trie *trie);

/**
 * @brief Remove todas as strings
 */
void trie_clear(Trie *trie);

/**
 * @brief Converte trie para array de strings
 *
 * @param trie Ponteiro para o trie
 * @param strings Array de strings (será alocado)
 * @param count Número de strings
 *
 * Strings em ordem lexicográfica.
 */
DataStructureError trie_to_array(const Trie *trie, char ***strings, size_t *count);

/**
 * @brief Imprime todas as strings do trie
 */
void trie_print(const Trie *trie);

#endif // TRIE_H
