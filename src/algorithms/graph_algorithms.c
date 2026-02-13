/**
 * @file graph_algorithms.c
 * @brief Implementacao de algoritmos de grafos: Dijkstra, Bellman-Ford,
 *        Floyd-Warshall, Kruskal, Prim
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 23-25
 * - Sedgewick & Wayne (2011), Chapter 4
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/graph_algorithms.h"
#include "data_structures/union_find.h"

#include <stdlib.h>
#include <string.h>

// ============================================================================
// HELPERS
// ============================================================================

void shortest_path_free(ShortestPathResult *result) {
    if (result == NULL) return;
    free(result->dist);
    free(result->parent);
    free(result);
}

void all_pairs_free(AllPairsResult *result) {
    if (result == NULL) return;
    for (size_t i = 0; i < result->num_vertices; i++) {
        free(result->dist[i]);
        free(result->next[i]);
    }
    free(result->dist);
    free(result->next);
    free(result);
}

void mst_free(MSTResult *result) {
    if (result == NULL) return;
    free(result->edges);
    free(result);
}

static ShortestPathResult* create_sp_result(size_t n) {
    ShortestPathResult *r = (ShortestPathResult *)malloc(sizeof(ShortestPathResult));
    if (r == NULL) return NULL;
    r->dist = (double *)malloc(n * sizeof(double));
    r->parent = (size_t *)malloc(n * sizeof(size_t));
    r->num_vertices = n;
    r->has_negative_cycle = false;
    if (r->dist == NULL || r->parent == NULL) {
        shortest_path_free(r);
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        r->dist[i] = GRAPH_INFINITY;
        r->parent[i] = GRAPH_NO_PARENT;
    }
    return r;
}

// ============================================================================
// DIJKSTRA - Cormen S24.3 (min-heap via array simples)
// ============================================================================

ShortestPathResult* dijkstra(const Graph *graph, Vertex source) {
    if (graph == NULL) return NULL;
    size_t n = graph_num_vertices(graph);
    if (source >= n) return NULL;

    ShortestPathResult *r = create_sp_result(n);
    if (r == NULL) return NULL;

    r->dist[source] = 0.0;

    bool *visited = (bool *)calloc(n, sizeof(bool));
    if (visited == NULL) { shortest_path_free(r); return NULL; }

    for (size_t iter = 0; iter < n; iter++) {
        size_t u = (size_t)-1;
        double min_d = GRAPH_INFINITY;
        for (size_t i = 0; i < n; i++) {
            if (!visited[i] && r->dist[i] < min_d) {
                min_d = r->dist[i];
                u = i;
            }
        }
        if (u == (size_t)-1) break;
        visited[u] = true;

        Vertex *neighbors = NULL;
        size_t count = 0;
        graph_neighbors(graph, u, &neighbors, &count);

        for (size_t i = 0; i < count; i++) {
            Vertex v = neighbors[i];
            double w = graph_edge_weight(graph, u, v);
            if (r->dist[u] + w < r->dist[v]) {
                r->dist[v] = r->dist[u] + w;
                r->parent[v] = u;
            }
        }
        free(neighbors);
    }

    free(visited);
    return r;
}

// ============================================================================
// BELLMAN-FORD - Cormen S24.1
// ============================================================================

ShortestPathResult* bellman_ford(const Graph *graph, Vertex source) {
    if (graph == NULL) return NULL;
    size_t n = graph_num_vertices(graph);
    if (source >= n) return NULL;

    ShortestPathResult *r = create_sp_result(n);
    if (r == NULL) return NULL;
    r->dist[source] = 0.0;

    Edge *edges = NULL;
    size_t num_edges = 0;
    graph_edges(graph, &edges, &num_edges);

    for (size_t i = 0; i < n - 1; i++) {
        bool changed = false;
        for (size_t j = 0; j < num_edges; j++) {
            Vertex u = edges[j].src;
            Vertex v = edges[j].dest;
            double w = edges[j].weight;
            if (r->dist[u] < GRAPH_INFINITY && r->dist[u] + w < r->dist[v]) {
                r->dist[v] = r->dist[u] + w;
                r->parent[v] = u;
                changed = true;
            }
        }
        if (!changed) break;
    }

    for (size_t j = 0; j < num_edges; j++) {
        Vertex u = edges[j].src;
        Vertex v = edges[j].dest;
        double w = edges[j].weight;
        if (r->dist[u] < GRAPH_INFINITY && r->dist[u] + w < r->dist[v]) {
            r->has_negative_cycle = true;
            break;
        }
    }

    free(edges);
    return r;
}

// ============================================================================
// FLOYD-WARSHALL - Cormen S25.2
// ============================================================================

AllPairsResult* floyd_warshall(const Graph *graph) {
    if (graph == NULL) return NULL;
    size_t n = graph_num_vertices(graph);
    if (n == 0) return NULL;

    AllPairsResult *r = (AllPairsResult *)malloc(sizeof(AllPairsResult));
    if (r == NULL) return NULL;
    r->num_vertices = n;

    r->dist = (double **)malloc(n * sizeof(double *));
    r->next = (size_t **)malloc(n * sizeof(size_t *));
    if (r->dist == NULL || r->next == NULL) {
        free(r->dist); free(r->next); free(r);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        r->dist[i] = (double *)malloc(n * sizeof(double));
        r->next[i] = (size_t *)malloc(n * sizeof(size_t));
        if (r->dist[i] == NULL || r->next[i] == NULL) {
            for (size_t j = 0; j <= i; j++) {
                free(r->dist[j]); free(r->next[j]);
            }
            free(r->dist); free(r->next); free(r);
            return NULL;
        }
        for (size_t j = 0; j < n; j++) {
            r->dist[i][j] = (i == j) ? 0.0 : GRAPH_INFINITY;
            r->next[i][j] = GRAPH_NO_PARENT;
        }
    }

    for (size_t u = 0; u < n; u++) {
        Vertex *neighbors = NULL;
        size_t count = 0;
        graph_neighbors(graph, u, &neighbors, &count);
        for (size_t i = 0; i < count; i++) {
            Vertex v = neighbors[i];
            r->dist[u][v] = graph_edge_weight(graph, u, v);
            r->next[u][v] = v;
        }
        free(neighbors);
    }

    for (size_t k = 0; k < n; k++) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (r->dist[i][k] < GRAPH_INFINITY && r->dist[k][j] < GRAPH_INFINITY) {
                    double through_k = r->dist[i][k] + r->dist[k][j];
                    if (through_k < r->dist[i][j]) {
                        r->dist[i][j] = through_k;
                        r->next[i][j] = r->next[i][k];
                    }
                }
            }
        }
    }

    return r;
}

// ============================================================================
// KRUSKAL - Cormen S23.2 (Union-Find)
// ============================================================================

static int compare_mst_edge(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    if (ea->weight < eb->weight) return -1;
    if (ea->weight > eb->weight) return 1;
    return 0;
}

MSTResult* kruskal(const Graph *graph) {
    if (graph == NULL) return NULL;
    size_t n = graph_num_vertices(graph);
    if (n == 0) return NULL;

    Edge *edges = NULL;
    size_t num_edges = 0;
    graph_edges(graph, &edges, &num_edges);
    if (num_edges == 0) {
        free(edges);
        MSTResult *r = (MSTResult *)calloc(1, sizeof(MSTResult));
        return r;
    }

    qsort(edges, num_edges, sizeof(Edge), compare_mst_edge);

    UnionFind *uf = uf_create(n);
    if (uf == NULL) { free(edges); return NULL; }

    MSTResult *r = (MSTResult *)malloc(sizeof(MSTResult));
    if (r == NULL) { free(edges); uf_destroy(uf); return NULL; }
    r->edges = (MSTEdge *)malloc((n - 1) * sizeof(MSTEdge));
    r->num_edges = 0;
    r->total_weight = 0.0;
    if (r->edges == NULL) { free(edges); uf_destroy(uf); free(r); return NULL; }

    for (size_t i = 0; i < num_edges && r->num_edges < n - 1; i++) {
        if (!uf_connected(uf, edges[i].src, edges[i].dest)) {
            uf_union(uf, edges[i].src, edges[i].dest);
            r->edges[r->num_edges].u = edges[i].src;
            r->edges[r->num_edges].v = edges[i].dest;
            r->edges[r->num_edges].weight = edges[i].weight;
            r->total_weight += edges[i].weight;
            r->num_edges++;
        }
    }

    free(edges);
    uf_destroy(uf);
    return r;
}

// ============================================================================
// PRIM - Cormen S23.2 (min-key via array simples)
// ============================================================================

MSTResult* prim(const Graph *graph) {
    if (graph == NULL) return NULL;
    size_t n = graph_num_vertices(graph);
    if (n == 0) return NULL;

    double *key = (double *)malloc(n * sizeof(double));
    size_t *parent = (size_t *)malloc(n * sizeof(size_t));
    bool *in_mst = (bool *)calloc(n, sizeof(bool));
    if (key == NULL || parent == NULL || in_mst == NULL) {
        free(key); free(parent); free(in_mst);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        key[i] = GRAPH_INFINITY;
        parent[i] = GRAPH_NO_PARENT;
    }
    key[0] = 0.0;

    for (size_t iter = 0; iter < n; iter++) {
        size_t u = (size_t)-1;
        double min_key = GRAPH_INFINITY;
        for (size_t i = 0; i < n; i++) {
            if (!in_mst[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }
        if (u == (size_t)-1) break;
        in_mst[u] = true;

        Vertex *neighbors = NULL;
        size_t count = 0;
        graph_neighbors(graph, u, &neighbors, &count);
        for (size_t i = 0; i < count; i++) {
            Vertex v = neighbors[i];
            double w = graph_edge_weight(graph, u, v);
            if (!in_mst[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
        free(neighbors);
    }

    MSTResult *r = (MSTResult *)malloc(sizeof(MSTResult));
    if (r == NULL) { free(key); free(parent); free(in_mst); return NULL; }
    r->edges = (MSTEdge *)malloc((n - 1) * sizeof(MSTEdge));
    r->num_edges = 0;
    r->total_weight = 0.0;

    if (r->edges == NULL) {
        free(key); free(parent); free(in_mst); free(r);
        return NULL;
    }

    for (size_t i = 1; i < n; i++) {
        if (parent[i] != GRAPH_NO_PARENT) {
            r->edges[r->num_edges].u = parent[i];
            r->edges[r->num_edges].v = i;
            r->edges[r->num_edges].weight = key[i];
            r->total_weight += key[i];
            r->num_edges++;
        }
    }

    free(key);
    free(parent);
    free(in_mst);
    return r;
}
