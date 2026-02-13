/**
 * @file graph_algorithms.h
 * @brief Algoritmos de Grafos (Graph Algorithms)
 *
 * Implementacao dos algoritmos classicos de caminhos minimos e
 * arvores geradoras minimas.
 *
 * Nota: BFS, DFS, Topological Sort e Kosaraju SCCs ja estao
 * implementados em data_structures/graph.h.
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 23-25
 * - Sedgewick & Wayne (2011), Chapter 4
 * - Dijkstra, E. W. (1959). "A note on two problems in connexion with graphs"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data_structures/graph.h"
#include <stddef.h>
#include <float.h>

#define GRAPH_INFINITY DBL_MAX
#define GRAPH_NO_PARENT ((size_t)-1)

/**
 * @brief Resultado de shortest path (single-source)
 */
typedef struct {
    double *dist;
    size_t *parent;
    size_t num_vertices;
    bool has_negative_cycle;
} ShortestPathResult;

/**
 * @brief Resultado de all-pairs shortest paths
 */
typedef struct {
    double **dist;
    size_t **next;
    size_t num_vertices;
} AllPairsResult;

/**
 * @brief Aresta para MST
 */
typedef struct {
    Vertex u;
    Vertex v;
    double weight;
} MSTEdge;

/**
 * @brief Resultado de MST
 */
typedef struct {
    MSTEdge *edges;
    size_t num_edges;
    double total_weight;
} MSTResult;

void shortest_path_free(ShortestPathResult *result);
void all_pairs_free(AllPairsResult *result);
void mst_free(MSTResult *result);

// ============================================================================
// CAMINHOS MINIMOS - SINGLE SOURCE
// ============================================================================

/**
 * @brief Dijkstra - Caminho minimo de fonte unica (pesos nao-negativos)
 *
 * Pseudocodigo (Cormen et al., 2009, p. 658):
 * DIJKSTRA(G, w, s)
 *   INITIALIZE-SINGLE-SOURCE(G, s)
 *   S = empty
 *   Q = G.V
 *   while Q != empty
 *       u = EXTRACT-MIN(Q)
 *       S = S U {u}
 *       for each vertex v in G.Adj[u]
 *           RELAX(u, v, w)
 *
 * @param graph Grafo (pesos >= 0)
 * @param source Vertice origem
 * @return ShortestPathResult* Distancias e predecessores (caller libera)
 *
 * Complexidade: O((V+E) log V) com min-heap
 */
ShortestPathResult* dijkstra(const Graph *graph, Vertex source);

/**
 * @brief Bellman-Ford - Caminho minimo com pesos negativos
 *
 * Pseudocodigo (Cormen et al., 2009, p. 651):
 * BELLMAN-FORD(G, w, s)
 *   INITIALIZE-SINGLE-SOURCE(G, s)
 *   for i = 1 to |G.V| - 1
 *       for each edge (u, v) in G.E
 *           RELAX(u, v, w)
 *   for each edge (u, v) in G.E
 *       if v.d > u.d + w(u, v)
 *           return FALSE
 *   return TRUE
 *
 * @param graph Grafo (pode ter pesos negativos)
 * @param source Vertice origem
 * @return ShortestPathResult* (has_negative_cycle = true se ciclo negativo)
 *
 * Complexidade: O(V * E)
 */
ShortestPathResult* bellman_ford(const Graph *graph, Vertex source);

// ============================================================================
// CAMINHOS MINIMOS - ALL PAIRS
// ============================================================================

/**
 * @brief Floyd-Warshall - Caminhos minimos entre todos os pares
 *
 * Pseudocodigo (Cormen et al., 2009, p. 695):
 * FLOYD-WARSHALL(W)
 *   n = W.rows
 *   D(0) = W
 *   for k = 1 to n
 *       for i = 1 to n
 *           for j = 1 to n
 *               d(k)[i][j] = min(d(k-1)[i][j], d(k-1)[i][k] + d(k-1)[k][j])
 *   return D(n)
 *
 * @param graph Grafo
 * @return AllPairsResult* Matriz de distancias e next-hop (caller libera)
 *
 * Complexidade: O(V^3)
 * Espaco: O(V^2)
 */
AllPairsResult* floyd_warshall(const Graph *graph);

// ============================================================================
// ARVORE GERADORA MINIMA (MST)
// ============================================================================

/**
 * @brief Kruskal - MST via ordenacao de arestas + Union-Find
 *
 * Pseudocodigo (Cormen et al., 2009, p. 631):
 * MST-KRUSKAL(G, w)
 *   A = empty
 *   for each vertex v in G.V
 *       MAKE-SET(v)
 *   sort edges of G.E by weight w nondecreasing
 *   for each edge (u, v) in G.E (sorted)
 *       if FIND-SET(u) != FIND-SET(v)
 *           A = A U {(u, v)}
 *           UNION(u, v)
 *   return A
 *
 * @param graph Grafo nao-direcionado ponderado
 * @return MSTResult* Arestas da MST (caller libera)
 *
 * Complexidade: O(E log E)
 */
MSTResult* kruskal(const Graph *graph);

/**
 * @brief Prim - MST via vertice mais proximo (greedy)
 *
 * Pseudocodigo (Cormen et al., 2009, p. 634):
 * MST-PRIM(G, w, r)
 *   for each u in G.V
 *       u.key = infinity
 *       u.pi = NIL
 *   r.key = 0
 *   Q = G.V
 *   while Q != empty
 *       u = EXTRACT-MIN(Q)
 *       for each v in G.Adj[u]
 *           if v in Q and w(u,v) < v.key
 *               v.pi = u
 *               v.key = w(u,v)
 *
 * @param graph Grafo nao-direcionado ponderado
 * @return MSTResult* Arestas da MST (caller libera)
 *
 * Complexidade: O((V+E) log V) com min-heap
 */
MSTResult* prim(const Graph *graph);

#endif // GRAPH_ALGORITHMS_H
