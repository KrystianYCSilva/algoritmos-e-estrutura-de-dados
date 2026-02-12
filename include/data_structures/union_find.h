/**
 * @file union_find.h
 * @brief Union-Find (Disjoint Set) com Path Compression e Union by Rank
 *
 * Estrutura para manter coleção de conjuntos disjuntos e realizar operações:
 * - Make-Set: criar conjunto singleton
 * - Find: determinar a qual conjunto um elemento pertence
 * - Union: unir dois conjuntos
 *
 * Otimizações:
 * - Path Compression: durante Find, achatamos a árvore
 * - Union by Rank: unimos árvore menor sob árvore maior
 *
 * Com ambas otimizações:
 * - Find: O(α(n)) amortizado (α = inversa de Ackermann, praticamente constante)
 * - Union: O(α(n)) amortizado
 * - α(n) < 5 para todos os valores práticos de n
 *
 * Aplicações:
 * - Kruskal's algorithm (MST)
 * - Detectar ciclos em grafos não-direcionados
 * - Componentes conexos
 * - Percolation theory
 * - Image segmentation
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 21 - Data Structures for Disjoint Sets
 * - Tarjan, R. E. (1975). "Efficiency of a Good But Not Linear Set Union Algorithm"
 * - Sedgewick & Wayne (2011), Section 1.5
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct UnionFind UnionFind;

/**
 * @brief Cria uma estrutura Union-Find com n elementos
 *
 * @param n Número de elementos (0 a n-1)
 * @return UnionFind* Estrutura criada
 *
 * Inicialmente, cada elemento está em seu próprio conjunto.
 *
 * Pseudocódigo (Cormen et al., 2009, p. 562):
 * MAKE-SET(x)
 *   x.p = x
 *   x.rank = 0
 *
 * Complexidade: O(n)
 */
UnionFind* uf_create(size_t n);

void uf_destroy(UnionFind *uf);

/**
 * @brief Encontra o representante (raiz) do conjunto contendo x
 *
 * @param uf Ponteiro para Union-Find
 * @param x Elemento
 * @return size_t Representante do conjunto
 *
 * Pseudocódigo com Path Compression (Cormen et al., 2009, p. 569):
 * FIND-SET(x)
 *   if x ≠ x.p
 *       x.p = FIND-SET(x.p)
 *   return x.p
 *
 * Path Compression: durante busca, fazemos todos os nós no caminho
 * apontarem diretamente para a raiz, achatando a árvore.
 *
 * Complexidade: O(α(n)) amortizado
 */
size_t uf_find(UnionFind *uf, size_t x);

/**
 * @brief Une os conjuntos contendo x e y
 *
 * @param uf Ponteiro para Union-Find
 * @param x Primeiro elemento
 * @param y Segundo elemento
 * @return bool true se união foi realizada, false se já estavam no mesmo conjunto
 *
 * Pseudocódigo com Union by Rank (Cormen et al., 2009, p. 568):
 * UNION(x, y)
 *   LINK(FIND-SET(x), FIND-SET(y))
 *
 * LINK(x, y)
 *   if x.rank > y.rank
 *       y.p = x
 *   else
 *       x.p = y
 *       if x.rank == y.rank
 *           y.rank = y.rank + 1
 *
 * Union by Rank: sempre anexamos árvore com rank menor sob árvore com rank maior.
 * Rank é upper bound na altura da árvore.
 *
 * Complexidade: O(α(n)) amortizado
 */
bool uf_union(UnionFind *uf, size_t x, size_t y);

/**
 * @brief Verifica se x e y estão no mesmo conjunto
 *
 * @param uf Ponteiro para Union-Find
 * @param x Primeiro elemento
 * @param y Segundo elemento
 * @return bool true se estão no mesmo conjunto
 *
 * Implementação: return uf_find(uf, x) == uf_find(uf, y)
 *
 * Complexidade: O(α(n)) amortizado
 */
bool uf_connected(UnionFind *uf, size_t x, size_t y);

/**
 * @brief Retorna o número de conjuntos disjuntos
 *
 * @param uf Ponteiro para Union-Find
 * @return size_t Número de componentes
 *
 * Inicialmente n, decresce com cada união bem-sucedida.
 *
 * Complexidade: O(1)
 */
size_t uf_count(const UnionFind *uf);

/**
 * @brief Retorna o tamanho do conjunto contendo x
 *
 * @param uf Ponteiro para Union-Find
 * @param x Elemento
 * @return size_t Tamanho do conjunto
 *
 * Requer manter size de cada conjunto (além de rank).
 *
 * Complexidade: O(α(n)) amortizado
 */
size_t uf_size(UnionFind *uf, size_t x);

/**
 * @brief Retorna todos os elementos do conjunto contendo x
 *
 * @param uf Ponteiro para Union-Find
 * @param x Elemento
 * @param members Array de saída (será alocado)
 * @param size Tamanho do array
 * @return DataStructureError Código de erro
 *
 * Array deve ser liberado com free().
 *
 * Complexidade: O(n)
 */
DataStructureError uf_get_members(UnionFind *uf, size_t x,
                                  size_t **members, size_t *size);

/**
 * @brief Retorna array com ID do representante de cada elemento
 *
 * @param uf Ponteiro para Union-Find
 * @return size_t* Array onde components[i] = representante de i
 *
 * Array deve ser liberado com free().
 * Elementos com mesmo representante estão no mesmo conjunto.
 *
 * Complexidade: O(n α(n))
 */
size_t* uf_get_components(UnionFind *uf);

/**
 * @brief Reseta para estado inicial (todos em conjuntos separados)
 */
void uf_reset(UnionFind *uf);

/**
 * @brief Imprime estrutura (para debugging)
 *
 * Mostra parent e rank de cada elemento.
 */
void uf_print(const UnionFind *uf);

// ============================================================================
// ANÁLISE DE COMPLEXIDADE
// ============================================================================

/**
 * Teorema 21.14 (Cormen et al., 2009, p. 575):
 *
 * Uma sequência de m operações MAKE-SET, UNION e FIND-SET, onde n das
 * operações são MAKE-SET, pode ser realizada em tempo O(m α(n)) no pior caso,
 * onde α(n) é a inversa da função de Ackermann.
 *
 * Inversa de Ackermann α(n):
 * - α(n) ≤ 4 para n ≤ 2^(2^(2^2^16))  (número astronômico)
 * - Na prática, α(n) pode ser considerado constante (≤ 5)
 *
 * Crescimento:
 * α(1)     = 1
 * α(3)     = 2
 * α(7)     = 3
 * α(2047)  = 4
 * α(2^2047) = 5
 *
 * Portanto, para qualquer aplicação prática, Union-Find é efetivamente O(1).
 */

#endif // UNION_FIND_H
