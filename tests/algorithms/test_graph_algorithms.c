/**
 * @file test_graph_algorithms.c
 * @brief Testes unitarios para algoritmos de grafos
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/graph_algorithms.h"
#include "data_structures/graph.h"
#include "../test_macros.h"

#include <math.h>

#define APPROX_EQ(a, b) ASSERT_TRUE(fabs((a) - (b)) < 0.01)

// ============================================================================
// DIJKSTRA
// ============================================================================

TEST(dijkstra_basic) {
    Graph *g = graph_create(5, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 10.0);
    graph_add_edge(g, 0, 3, 5.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 1, 3, 2.0);
    graph_add_edge(g, 2, 4, 4.0);
    graph_add_edge(g, 3, 1, 3.0);
    graph_add_edge(g, 3, 2, 9.0);
    graph_add_edge(g, 3, 4, 2.0);
    graph_add_edge(g, 4, 2, 6.0);

    ShortestPathResult *r = dijkstra(g, 0);
    ASSERT_NOT_NULL(r);
    APPROX_EQ(r->dist[0], 0.0);
    APPROX_EQ(r->dist[1], 8.0);
    APPROX_EQ(r->dist[2], 9.0);
    APPROX_EQ(r->dist[3], 5.0);
    APPROX_EQ(r->dist[4], 7.0);

    shortest_path_free(r);
    graph_destroy(g);
}

TEST(dijkstra_unreachable) {
    Graph *g = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 1.0);

    ShortestPathResult *r = dijkstra(g, 0);
    ASSERT_NOT_NULL(r);
    APPROX_EQ(r->dist[0], 0.0);
    APPROX_EQ(r->dist[1], 1.0);
    ASSERT_TRUE(r->dist[2] > 1e300);

    shortest_path_free(r);
    graph_destroy(g);
}

// ============================================================================
// BELLMAN-FORD
// ============================================================================

TEST(bellman_ford_basic) {
    Graph *g = graph_create(5, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 6.0);
    graph_add_edge(g, 0, 3, 7.0);
    graph_add_edge(g, 1, 2, 5.0);
    graph_add_edge(g, 1, 3, 8.0);
    graph_add_edge(g, 1, 4, -4.0);
    graph_add_edge(g, 2, 1, -2.0);
    graph_add_edge(g, 3, 2, -3.0);
    graph_add_edge(g, 3, 4, 9.0);
    graph_add_edge(g, 4, 2, 7.0);

    ShortestPathResult *r = bellman_ford(g, 0);
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_negative_cycle);
    APPROX_EQ(r->dist[0], 0.0);
    APPROX_EQ(r->dist[1], 2.0);
    APPROX_EQ(r->dist[2], 4.0);
    APPROX_EQ(r->dist[3], 7.0);
    APPROX_EQ(r->dist[4], -2.0);

    shortest_path_free(r);
    graph_destroy(g);
}

TEST(bellman_ford_negative_cycle) {
    Graph *g = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 1.0);
    graph_add_edge(g, 1, 2, -3.0);
    graph_add_edge(g, 2, 0, 1.0);

    ShortestPathResult *r = bellman_ford(g, 0);
    ASSERT_NOT_NULL(r);
    ASSERT_TRUE(r->has_negative_cycle);

    shortest_path_free(r);
    graph_destroy(g);
}

// ============================================================================
// FLOYD-WARSHALL
// ============================================================================

TEST(floyd_warshall_basic) {
    Graph *g = graph_create(4, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 3.0);
    graph_add_edge(g, 0, 2, 8.0);
    graph_add_edge(g, 1, 2, 2.0);
    graph_add_edge(g, 1, 3, 5.0);
    graph_add_edge(g, 2, 3, 1.0);

    AllPairsResult *r = floyd_warshall(g);
    ASSERT_NOT_NULL(r);
    APPROX_EQ(r->dist[0][0], 0.0);
    APPROX_EQ(r->dist[0][1], 3.0);
    APPROX_EQ(r->dist[0][2], 5.0);
    APPROX_EQ(r->dist[0][3], 6.0);

    all_pairs_free(r);
    graph_destroy(g);
}

// ============================================================================
// KRUSKAL
// ============================================================================

TEST(kruskal_basic) {
    Graph *g = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 10.0);
    graph_add_edge(g, 0, 2, 6.0);
    graph_add_edge(g, 0, 3, 5.0);
    graph_add_edge(g, 1, 3, 15.0);
    graph_add_edge(g, 2, 3, 4.0);

    MSTResult *r = kruskal(g);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(r->num_edges, 3);
    APPROX_EQ(r->total_weight, 19.0);

    mst_free(r);
    graph_destroy(g);
}

// ============================================================================
// PRIM
// ============================================================================

TEST(prim_basic) {
    Graph *g = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 10.0);
    graph_add_edge(g, 0, 2, 6.0);
    graph_add_edge(g, 0, 3, 5.0);
    graph_add_edge(g, 1, 3, 15.0);
    graph_add_edge(g, 2, 3, 4.0);

    MSTResult *r = prim(g);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(r->num_edges, 3);
    APPROX_EQ(r->total_weight, 19.0);

    mst_free(r);
    graph_destroy(g);
}

TEST(kruskal_prim_agree) {
    Graph *g = graph_create(6, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(g, 0, 1, 4.0);
    graph_add_edge(g, 0, 2, 4.0);
    graph_add_edge(g, 1, 2, 2.0);
    graph_add_edge(g, 1, 3, 6.0);
    graph_add_edge(g, 2, 3, 8.0);
    graph_add_edge(g, 2, 4, 9.0);
    graph_add_edge(g, 3, 4, 5.0);
    graph_add_edge(g, 3, 5, 1.0);
    graph_add_edge(g, 4, 5, 7.0);

    MSTResult *rk = kruskal(g);
    MSTResult *rp = prim(g);
    ASSERT_NOT_NULL(rk);
    ASSERT_NOT_NULL(rp);
    APPROX_EQ(rk->total_weight, rp->total_weight);
    ASSERT_EQ(rk->num_edges, 5);
    ASSERT_EQ(rp->num_edges, 5);

    mst_free(rk);
    mst_free(rp);
    graph_destroy(g);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Graph Algorithm Tests ===\n");

    RUN_TEST(dijkstra_basic);
    RUN_TEST(dijkstra_unreachable);
    RUN_TEST(bellman_ford_basic);
    RUN_TEST(bellman_ford_negative_cycle);
    RUN_TEST(floyd_warshall_basic);
    RUN_TEST(kruskal_basic);
    RUN_TEST(prim_basic);
    RUN_TEST(kruskal_prim_agree);

    printf("\nAll Graph Algorithm tests passed!\n");
    return 0;
}
