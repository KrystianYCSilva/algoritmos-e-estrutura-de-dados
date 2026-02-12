/**
 * @file union_find.c
 * @brief Implementação de Union-Find (Disjoint Set) com Path Compression e Union by Rank
 *
 * Implementa a estrutura de conjuntos disjuntos com duas otimizações clássicas:
 * - Path Compression: achata a árvore durante Find
 * - Union by Rank: anexa árvore menor sob árvore maior
 *
 * Com ambas otimizações, qualquer sequência de m operações em n elementos
 * executa em tempo O(m α(n)), onde α é a inversa da função de Ackermann.
 *
 * Referências:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 21 - Data Structures for Disjoint Sets
 * - Tarjan, R. E. (1975). "Efficiency of a Good But Not Linear Set Union Algorithm",
 *   Journal of the ACM 22(2): 215–225
 * - Sedgewick, R. & Wayne, K. (2011). "Algorithms" (4th ed.),
 *   Section 1.5 - Union-Find
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/union_find.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA
// ============================================================================

struct UnionFind {
    size_t *parent;
    size_t *rank;
    size_t *set_size;
    size_t num_elements;
    size_t num_sets;
};

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

UnionFind* uf_create(size_t n) {
    if (n == 0) {
        return NULL;
    }

    UnionFind *uf = (UnionFind *)malloc(sizeof(UnionFind));
    if (uf == NULL) {
        return NULL;
    }

    uf->parent = (size_t *)malloc(n * sizeof(size_t));
    uf->rank = (size_t *)malloc(n * sizeof(size_t));
    uf->set_size = (size_t *)malloc(n * sizeof(size_t));

    if (uf->parent == NULL || uf->rank == NULL || uf->set_size == NULL) {
        free(uf->parent);
        free(uf->rank);
        free(uf->set_size);
        free(uf);
        return NULL;
    }

    /* MAKE-SET(x): x.p = x, x.rank = 0 (Cormen et al., 2009, p. 562) */
    for (size_t i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->set_size[i] = 1;
    }

    uf->num_elements = n;
    uf->num_sets = n;
    return uf;
}

void uf_destroy(UnionFind *uf) {
    if (uf == NULL) {
        return;
    }

    free(uf->parent);
    free(uf->rank);
    free(uf->set_size);
    free(uf);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS
// ============================================================================

/**
 * FIND-SET com Path Compression por path splitting (iterativo).
 *
 * Variante iterativa (Tarjan, 1975):
 * while parent[x] != x:
 *     parent[x] = parent[parent[x]]  (path halving)
 *     x = parent[x]
 *
 * Complexidade: O(α(n)) amortizado
 */
size_t uf_find(UnionFind *uf, size_t x) {
    if (uf == NULL || x >= uf->num_elements) {
        return x;
    }

    while (uf->parent[x] != x) {
        uf->parent[x] = uf->parent[uf->parent[x]];
        x = uf->parent[x];
    }

    return x;
}

/**
 * UNION by Rank (Cormen et al., 2009, p. 568):
 * LINK(x, y)
 *   if x.rank > y.rank
 *       y.p = x
 *   else
 *       x.p = y
 *       if x.rank == y.rank
 *           y.rank = y.rank + 1
 *
 * Complexidade: O(α(n)) amortizado
 */
bool uf_union(UnionFind *uf, size_t x, size_t y) {
    if (uf == NULL || x >= uf->num_elements || y >= uf->num_elements) {
        return false;
    }

    size_t root_x = uf_find(uf, x);
    size_t root_y = uf_find(uf, y);

    if (root_x == root_y) {
        return false;
    }

    if (uf->rank[root_x] > uf->rank[root_y]) {
        uf->parent[root_y] = root_x;
        uf->set_size[root_x] += uf->set_size[root_y];
    } else if (uf->rank[root_x] < uf->rank[root_y]) {
        uf->parent[root_x] = root_y;
        uf->set_size[root_y] += uf->set_size[root_x];
    } else {
        uf->parent[root_y] = root_x;
        uf->set_size[root_x] += uf->set_size[root_y];
        uf->rank[root_x]++;
    }

    uf->num_sets--;
    return true;
}

bool uf_connected(UnionFind *uf, size_t x, size_t y) {
    if (uf == NULL || x >= uf->num_elements || y >= uf->num_elements) {
        return false;
    }

    return uf_find(uf, x) == uf_find(uf, y);
}

// ============================================================================
// CONSULTAS
// ============================================================================

size_t uf_count(const UnionFind *uf) {
    if (uf == NULL) {
        return 0;
    }
    return uf->num_sets;
}

size_t uf_size(UnionFind *uf, size_t x) {
    if (uf == NULL || x >= uf->num_elements) {
        return 0;
    }

    size_t root = uf_find(uf, x);
    return uf->set_size[root];
}

DataStructureError uf_get_members(UnionFind *uf, size_t x,
                                  size_t **members, size_t *size) {
    if (uf == NULL || members == NULL || size == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (x >= uf->num_elements) {
        return DS_ERROR_INVALID_INDEX;
    }

    size_t root = uf_find(uf, x);
    size_t set_sz = uf->set_size[root];

    *members = (size_t *)malloc(set_sz * sizeof(size_t));
    if (*members == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    *size = 0;
    for (size_t i = 0; i < uf->num_elements; i++) {
        if (uf_find(uf, i) == root) {
            (*members)[*size] = i;
            (*size)++;
        }
    }

    return DS_SUCCESS;
}

size_t* uf_get_components(UnionFind *uf) {
    if (uf == NULL) {
        return NULL;
    }

    size_t *components = (size_t *)malloc(uf->num_elements * sizeof(size_t));
    if (components == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < uf->num_elements; i++) {
        components[i] = uf_find(uf, i);
    }

    return components;
}

// ============================================================================
// UTILITÁRIOS
// ============================================================================

void uf_reset(UnionFind *uf) {
    if (uf == NULL) {
        return;
    }

    for (size_t i = 0; i < uf->num_elements; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->set_size[i] = 1;
    }

    uf->num_sets = uf->num_elements;
}

void uf_print(const UnionFind *uf) {
    if (uf == NULL) {
        return;
    }

    printf("UnionFind(n=%zu, sets=%zu)\n", uf->num_elements, uf->num_sets);
    printf("  Index:  ");
    for (size_t i = 0; i < uf->num_elements; i++) {
        printf("%3zu ", i);
    }
    printf("\n  Parent: ");
    for (size_t i = 0; i < uf->num_elements; i++) {
        printf("%3zu ", uf->parent[i]);
    }
    printf("\n  Rank:   ");
    for (size_t i = 0; i < uf->num_elements; i++) {
        printf("%3zu ", uf->rank[i]);
    }
    printf("\n  Size:   ");
    for (size_t i = 0; i < uf->num_elements; i++) {
        printf("%3zu ", uf->set_size[i]);
    }
    printf("\n");
}
