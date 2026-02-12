/**
 * @file trie.c
 * @brief Implementação de Trie (Prefix Tree) para strings
 *
 * Trie é uma árvore de recuperação eficiente onde cada nó representa um
 * caractere. Caminhos da raiz às folhas formam strings completas.
 *
 * Referências:
 * - Knuth, D. E. (1998). "The Art of Computer Programming" Vol 3,
 *   Section 6.3 - Digital Searching
 * - Fredkin, E. (1960). "Trie memory", Communications of the ACM 3(9): 490–499
 * - Sedgewick, R. & Wayne, K. (2011). "Algorithms" (4th ed.),
 *   Section 5.2 - Tries
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

typedef struct TrieNode {
    struct TrieNode **children;
    bool is_end_of_word;
    size_t alphabet_size;
} TrieNode;

struct Trie {
    TrieNode *root;
    size_t size;
    size_t alphabet_size;
};

#define DEFAULT_ALPHABET_SIZE 26

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

static TrieNode* trie_node_create(size_t alphabet_size) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL) {
        return NULL;
    }

    node->children = (TrieNode **)calloc(alphabet_size, sizeof(TrieNode *));
    if (node->children == NULL) {
        free(node);
        return NULL;
    }

    node->is_end_of_word = false;
    node->alphabet_size = alphabet_size;
    return node;
}

static void trie_node_destroy(TrieNode *node) {
    if (node == NULL) {
        return;
    }

    for (size_t i = 0; i < node->alphabet_size; i++) {
        trie_node_destroy(node->children[i]);
    }

    free(node->children);
    free(node);
}

static bool trie_node_has_children(const TrieNode *node) {
    for (size_t i = 0; i < node->alphabet_size; i++) {
        if (node->children[i] != NULL) {
            return true;
        }
    }
    return false;
}

static size_t trie_char_index(char c) {
    return (size_t)(c - 'a');
}

/**
 * Recursive removal returning true if caller should delete this node.
 *
 * Algorithm (Sedgewick & Wayne, 2011):
 * 1. Traverse to end of word
 * 2. Unmark end_of_word
 * 3. On backtrack, delete nodes that have no children and are not end_of_word
 */
static bool trie_remove_recursive(TrieNode *node, const char *str, size_t depth,
                                  bool *found) {
    if (node == NULL) {
        return false;
    }

    if (str[depth] == '\0') {
        if (node->is_end_of_word) {
            node->is_end_of_word = false;
            *found = true;
        }
        return !trie_node_has_children(node);
    }

    size_t index = trie_char_index(str[depth]);
    if (index >= node->alphabet_size || node->children[index] == NULL) {
        return false;
    }

    bool should_delete = trie_remove_recursive(node->children[index], str,
                                               depth + 1, found);

    if (should_delete) {
        trie_node_destroy(node->children[index]);
        node->children[index] = NULL;
        return !node->is_end_of_word && !trie_node_has_children(node);
    }

    return false;
}

static void trie_collect_words(const TrieNode *node, char *buffer, size_t depth,
                               char ***results, size_t *count, size_t *capacity) {
    if (node == NULL) {
        return;
    }

    if (node->is_end_of_word) {
        buffer[depth] = '\0';

        if (*count >= *capacity) {
            size_t new_capacity = (*capacity == 0) ? 16 : *capacity * 2;
            char **new_results = (char **)realloc(*results, new_capacity * sizeof(char *));
            if (new_results == NULL) {
                return;
            }
            *results = new_results;
            *capacity = new_capacity;
        }

        (*results)[*count] = (char *)malloc(depth + 1);
        if ((*results)[*count] == NULL) {
            return;
        }
        memcpy((*results)[*count], buffer, depth + 1);
        (*count)++;
    }

    for (size_t i = 0; i < node->alphabet_size; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = (char)('a' + i);
            trie_collect_words(node->children[i], buffer, depth + 1,
                               results, count, capacity);
        }
    }
}

static void trie_print_words(const TrieNode *node, char *buffer, size_t depth) {
    if (node == NULL) {
        return;
    }

    if (node->is_end_of_word) {
        buffer[depth] = '\0';
        printf("  %s\n", buffer);
    }

    for (size_t i = 0; i < node->alphabet_size; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = (char)('a' + i);
            trie_print_words(node->children[i], buffer, depth + 1);
        }
    }
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

Trie* trie_create(size_t alphabet_size) {
    if (alphabet_size == 0) {
        alphabet_size = DEFAULT_ALPHABET_SIZE;
    }

    Trie *trie = (Trie *)malloc(sizeof(Trie));
    if (trie == NULL) {
        return NULL;
    }

    trie->root = trie_node_create(alphabet_size);
    if (trie->root == NULL) {
        free(trie);
        return NULL;
    }

    trie->size = 0;
    trie->alphabet_size = alphabet_size;
    return trie;
}

void trie_destroy(Trie *trie) {
    if (trie == NULL) {
        return;
    }

    trie_node_destroy(trie->root);
    free(trie);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

DataStructureError trie_insert(Trie *trie, const char *str) {
    if (trie == NULL || str == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    TrieNode *current = trie->root;

    for (size_t i = 0; str[i] != '\0'; i++) {
        size_t index = trie_char_index(str[i]);
        if (index >= trie->alphabet_size) {
            return DS_ERROR_INVALID_PARAM;
        }

        if (current->children[index] == NULL) {
            current->children[index] = trie_node_create(trie->alphabet_size);
            if (current->children[index] == NULL) {
                return DS_ERROR_OUT_OF_MEMORY;
            }
        }

        current = current->children[index];
    }

    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        trie->size++;
    }

    return DS_SUCCESS;
}

bool trie_search(const Trie *trie, const char *str) {
    if (trie == NULL || str == NULL) {
        return false;
    }

    const TrieNode *current = trie->root;

    for (size_t i = 0; str[i] != '\0'; i++) {
        size_t index = trie_char_index(str[i]);
        if (index >= trie->alphabet_size || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }

    return current->is_end_of_word;
}

bool trie_starts_with(const Trie *trie, const char *prefix) {
    if (trie == NULL || prefix == NULL) {
        return false;
    }

    const TrieNode *current = trie->root;

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        size_t index = trie_char_index(prefix[i]);
        if (index >= trie->alphabet_size || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }

    return true;
}

DataStructureError trie_remove(Trie *trie, const char *str) {
    if (trie == NULL || str == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    bool found = false;
    trie_remove_recursive(trie->root, str, 0, &found);

    if (!found) {
        return DS_ERROR_NOT_FOUND;
    }

    trie->size--;
    return DS_SUCCESS;
}

// ============================================================================
// AUTOCOMPLETE E PREFIXOS
// ============================================================================

DataStructureError trie_autocomplete(const Trie *trie, const char *prefix,
                                     char ***results, size_t *count) {
    if (trie == NULL || prefix == NULL || results == NULL || count == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    *results = NULL;
    *count = 0;

    const TrieNode *current = trie->root;
    for (size_t i = 0; prefix[i] != '\0'; i++) {
        size_t index = trie_char_index(prefix[i]);
        if (index >= trie->alphabet_size || current->children[index] == NULL) {
            return DS_SUCCESS;
        }
        current = current->children[index];
    }

    size_t prefix_len = strlen(prefix);
    size_t max_word_len = 256;
    char *buffer = (char *)malloc(max_word_len);
    if (buffer == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    memcpy(buffer, prefix, prefix_len);

    size_t capacity = 0;
    trie_collect_words(current, buffer, prefix_len, results, count, &capacity);

    free(buffer);
    return DS_SUCCESS;
}

char* trie_longest_common_prefix(const Trie *trie) {
    if (trie == NULL) {
        return NULL;
    }

    char *result = (char *)malloc(256);
    if (result == NULL) {
        return NULL;
    }

    size_t depth = 0;
    const TrieNode *current = trie->root;

    /* Traverse while exactly one child exists and current is not end_of_word
     * (Knuth TAOCP Vol 3, Section 6.3) */
    while (current != NULL && !current->is_end_of_word) {
        int child_count = 0;
        size_t child_index = 0;

        for (size_t i = 0; i < current->alphabet_size; i++) {
            if (current->children[i] != NULL) {
                child_count++;
                child_index = i;
            }
        }

        if (child_count != 1) {
            break;
        }

        result[depth] = (char)('a' + child_index);
        depth++;
        current = current->children[child_index];
    }

    result[depth] = '\0';
    return result;
}

// ============================================================================
// CONSULTAS
// ============================================================================

size_t trie_size(const Trie *trie) {
    if (trie == NULL) {
        return 0;
    }
    return trie->size;
}

bool trie_is_empty(const Trie *trie) {
    return (trie == NULL || trie->size == 0);
}

void trie_clear(Trie *trie) {
    if (trie == NULL) {
        return;
    }

    trie_node_destroy(trie->root);
    trie->root = trie_node_create(trie->alphabet_size);
    trie->size = 0;
}

// ============================================================================
// CONVERSÕES
// ============================================================================

DataStructureError trie_to_array(const Trie *trie, char ***strings, size_t *count) {
    if (trie == NULL || strings == NULL || count == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    *strings = NULL;
    *count = 0;

    if (trie->size == 0) {
        return DS_SUCCESS;
    }

    char *buffer = (char *)malloc(256);
    if (buffer == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t capacity = 0;
    trie_collect_words(trie->root, buffer, 0, strings, count, &capacity);

    free(buffer);
    return DS_SUCCESS;
}

// ============================================================================
// IMPRESSÃO
// ============================================================================

void trie_print(const Trie *trie) {
    if (trie == NULL) {
        return;
    }

    printf("Trie(size=%zu, alphabet=%zu):\n", trie->size, trie->alphabet_size);

    char buffer[256];
    trie_print_words(trie->root, buffer, 0);
}
