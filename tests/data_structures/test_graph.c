/**
 * @file test_graph.c
 * @brief Testes unitarios para Graph (Grafo)
 *
 * Testa ambas representacoes (adj list + matrix), grafos direcionados
 * e nao-direcionados, travessias BFS/DFS, propriedades, topological sort,
 * componentes conexos (Kosaraju), clone, transpose.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "data_structures/graph.h"
#include "data_structures/common.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// HELPERS
// ============================================================================

typedef struct {
    Vertex *visited;
    size_t count;
} VisitData;

static void collect_vertex(Vertex v, void *user_data) {
    VisitData *data = (VisitData*)user_data;
    data->visited[data->count++] = v;
}

static Graph* create_sample_undirected_list(void) {
    Graph *g = graph_create(5, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(g, 0, 1, 1.0);
    graph_add_edge(g, 0, 4, 1.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 1, 3, 1.0);
    graph_add_edge(g, 1, 4, 1.0);
    graph_add_edge(g, 2, 3, 1.0);
    graph_add_edge(g, 3, 4, 1.0);
    return g;
}

static Graph* create_sample_directed_list(void) {
    Graph *g = graph_create(6, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(g, 0, 1, 1.0);
    graph_add_edge(g, 0, 3, 1.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 2, 3, 1.0);
    graph_add_edge(g, 4, 5, 1.0);
    return g;
}

// ============================================================================
// TESTES - CRIACAO E DESTRUICAO
// ============================================================================

TEST(create_destroy_list) {
    Graph *g = graph_create(5, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    ASSERT_NOT_NULL(g);
    ASSERT_EQ(graph_num_vertices(g), 5);
    ASSERT_EQ(graph_num_edges(g), 0);
    graph_destroy(g);
}

TEST(create_destroy_matrix) {
    Graph *g = graph_create(5, GRAPH_DIRECTED, GRAPH_ADJACENCY_MATRIX, true);
    ASSERT_NOT_NULL(g);
    ASSERT_EQ(graph_num_vertices(g), 5);
    ASSERT_EQ(graph_num_edges(g), 0);
    graph_destroy(g);
}

// ============================================================================
// TESTES - VERTICES
// ============================================================================

TEST(add_vertex) {
    Graph *g = graph_create(2, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    ASSERT_EQ(graph_num_vertices(g), 2);

    Vertex v = graph_add_vertex(g);
    ASSERT_EQ(v, 2);
    ASSERT_EQ(graph_num_vertices(g), 3);

    v = graph_add_vertex(g);
    ASSERT_EQ(v, 3);
    ASSERT_EQ(graph_num_vertices(g), 4);

    graph_destroy(g);
}

TEST(add_vertex_matrix) {
    Graph *g = graph_create(2, GRAPH_DIRECTED, GRAPH_ADJACENCY_MATRIX, false);
    graph_add_edge(g, 0, 1, 1.0);
    ASSERT_TRUE(graph_has_edge(g, 0, 1));

    Vertex v = graph_add_vertex(g);
    ASSERT_EQ(v, 2);
    ASSERT_TRUE(graph_has_edge(g, 0, 1));

    graph_add_edge(g, 2, 0, 1.0);
    ASSERT_TRUE(graph_has_edge(g, 2, 0));

    graph_destroy(g);
}

TEST(remove_vertex) {
    Graph *g = create_sample_undirected_list();
    size_t edges_before = graph_num_edges(g);

    DataStructureError err = graph_remove_vertex(g, 1);
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_TRUE(edges_before > graph_num_edges(g));
    ASSERT_FALSE(graph_has_edge(g, 0, 1));
    ASSERT_FALSE(graph_has_edge(g, 1, 2));

    graph_destroy(g);
}

// ============================================================================
// TESTES - ARESTAS
// ============================================================================

TEST(add_remove_edges_list) {
    Graph *g = graph_create(4, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);

    ASSERT_EQ(graph_add_edge(g, 0, 1, 1.0), DS_SUCCESS);
    ASSERT_EQ(graph_add_edge(g, 1, 2, 1.0), DS_SUCCESS);
    ASSERT_EQ(graph_add_edge(g, 2, 3, 1.0), DS_SUCCESS);
    ASSERT_EQ(graph_num_edges(g), 3);

    ASSERT_TRUE(graph_has_edge(g, 0, 1));
    ASSERT_FALSE(graph_has_edge(g, 1, 0));

    ASSERT_EQ(graph_remove_edge(g, 1, 2), DS_SUCCESS);
    ASSERT_EQ(graph_num_edges(g), 2);
    ASSERT_FALSE(graph_has_edge(g, 1, 2));

    graph_destroy(g);
}

TEST(add_remove_edges_matrix) {
    Graph *g = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_MATRIX, false);

    ASSERT_EQ(graph_add_edge(g, 0, 1, 1.0), DS_SUCCESS);
    ASSERT_EQ(graph_add_edge(g, 1, 2, 1.0), DS_SUCCESS);
    ASSERT_EQ(graph_num_edges(g), 2);

    ASSERT_TRUE(graph_has_edge(g, 0, 1));
    ASSERT_TRUE(graph_has_edge(g, 1, 0));

    ASSERT_EQ(graph_remove_edge(g, 0, 1), DS_SUCCESS);
    ASSERT_EQ(graph_num_edges(g), 1);
    ASSERT_FALSE(graph_has_edge(g, 0, 1));
    ASSERT_FALSE(graph_has_edge(g, 1, 0));

    graph_destroy(g);
}

TEST(weighted_edges) {
    Graph *g = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);

    graph_add_edge(g, 0, 1, 2.5);
    graph_add_edge(g, 1, 2, 3.7);

    ASSERT_TRUE(graph_edge_weight(g, 0, 1) > 2.4 && graph_edge_weight(g, 0, 1) < 2.6);
    ASSERT_TRUE(graph_edge_weight(g, 1, 2) > 3.6 && graph_edge_weight(g, 1, 2) < 3.8);

    graph_add_edge(g, 0, 1, 9.0);
    ASSERT_TRUE(graph_edge_weight(g, 0, 1) > 8.9 && graph_edge_weight(g, 0, 1) < 9.1);

    graph_destroy(g);
}

TEST(invalid_edge_operations) {
    Graph *g = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);

    ASSERT_EQ(graph_add_edge(g, 0, 5, 1.0), DS_ERROR_INVALID_INDEX);
    ASSERT_EQ(graph_add_edge(NULL, 0, 1, 1.0), DS_ERROR_NULL_POINTER);
    ASSERT_EQ(graph_remove_edge(g, 0, 1), DS_ERROR_NOT_FOUND);

    graph_destroy(g);
}

// ============================================================================
// TESTES - GRAU
// ============================================================================

TEST(degree) {
    Graph *g = create_sample_undirected_list();

    ASSERT_EQ(graph_degree(g, 1), 4);
    ASSERT_EQ(graph_degree(g, 0), 2);

    graph_destroy(g);

    Graph *dg = create_sample_directed_list();
    ASSERT_EQ(graph_out_degree(dg, 0), 2);
    ASSERT_EQ(graph_in_degree(dg, 3), 2);
    ASSERT_EQ(graph_degree(dg, 0), 2);
    ASSERT_EQ(graph_degree(dg, 3), 2);

    graph_destroy(dg);
}

// ============================================================================
// TESTES - VIZINHOS
// ============================================================================

TEST(neighbors) {
    Graph *g = create_sample_directed_list();

    Vertex *nbrs = NULL;
    size_t count = 0;
    ASSERT_EQ(graph_neighbors(g, 0, &nbrs, &count), DS_SUCCESS);
    ASSERT_EQ(count, 2);
    free(nbrs);

    ASSERT_EQ(graph_neighbors(g, 5, &nbrs, &count), DS_SUCCESS);
    ASSERT_EQ(count, 0);
    ASSERT_NULL(nbrs);

    graph_destroy(g);
}

TEST(get_all_edges) {
    Graph *g = create_sample_directed_list();

    Edge *edges = NULL;
    size_t count = 0;
    ASSERT_EQ(graph_edges(g, &edges, &count), DS_SUCCESS);
    ASSERT_EQ(count, 5);

    free(edges);
    graph_destroy(g);
}

// ============================================================================
// TESTES - BFS E DFS
// ============================================================================

TEST(bfs_traversal) {
    Graph *g = create_sample_undirected_list();
    Vertex buf[5];
    VisitData data = { buf, 0 };

    graph_bfs(g, 0, collect_vertex, &data);
    ASSERT_EQ(data.count, 5);
    ASSERT_EQ(data.visited[0], 0);

    graph_destroy(g);
}

TEST(dfs_traversal) {
    Graph *g = create_sample_undirected_list();
    Vertex buf[5];
    VisitData data = { buf, 0 };

    graph_dfs(g, 0, collect_vertex, &data);
    ASSERT_EQ(data.count, 5);
    ASSERT_EQ(data.visited[0], 0);

    graph_destroy(g);
}

// ============================================================================
// TESTES - PROPRIEDADES
// ============================================================================

TEST(is_connected) {
    Graph *g = create_sample_undirected_list();
    ASSERT_TRUE(graph_is_connected(g));
    graph_destroy(g);

    Graph *disc = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(disc, 0, 1, 1.0);
    graph_add_edge(disc, 2, 3, 1.0);
    ASSERT_FALSE(graph_is_connected(disc));
    graph_destroy(disc);
}

TEST(has_cycle) {
    Graph *dag = graph_create(4, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(dag, 0, 1, 1.0);
    graph_add_edge(dag, 1, 2, 1.0);
    graph_add_edge(dag, 2, 3, 1.0);
    ASSERT_FALSE(graph_has_cycle(dag));
    graph_destroy(dag);

    Graph *cyc = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(cyc, 0, 1, 1.0);
    graph_add_edge(cyc, 1, 2, 1.0);
    graph_add_edge(cyc, 2, 0, 1.0);
    ASSERT_TRUE(graph_has_cycle(cyc));
    graph_destroy(cyc);
}

TEST(is_tree) {
    Graph *tree = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(tree, 0, 1, 1.0);
    graph_add_edge(tree, 0, 2, 1.0);
    graph_add_edge(tree, 0, 3, 1.0);
    ASSERT_TRUE(graph_is_tree(tree));
    graph_destroy(tree);

    Graph *not_tree = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(not_tree, 0, 1, 1.0);
    graph_add_edge(not_tree, 1, 2, 1.0);
    graph_add_edge(not_tree, 2, 0, 1.0);
    graph_add_edge(not_tree, 0, 3, 1.0);
    ASSERT_FALSE(graph_is_tree(not_tree));
    graph_destroy(not_tree);
}

TEST(is_bipartite) {
    Graph *bp = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(bp, 0, 1, 1.0);
    graph_add_edge(bp, 1, 2, 1.0);
    graph_add_edge(bp, 2, 3, 1.0);
    graph_add_edge(bp, 3, 0, 1.0);
    ASSERT_TRUE(graph_is_bipartite(bp));
    graph_destroy(bp);

    Graph *nbp = graph_create(3, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(nbp, 0, 1, 1.0);
    graph_add_edge(nbp, 1, 2, 1.0);
    graph_add_edge(nbp, 2, 0, 1.0);
    ASSERT_FALSE(graph_is_bipartite(nbp));
    graph_destroy(nbp);
}

// ============================================================================
// TESTES - TOPOLOGICAL SORT
// ============================================================================

TEST(topological_sort) {
    Graph *dag = graph_create(6, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(dag, 5, 2, 1.0);
    graph_add_edge(dag, 5, 0, 1.0);
    graph_add_edge(dag, 4, 0, 1.0);
    graph_add_edge(dag, 4, 1, 1.0);
    graph_add_edge(dag, 2, 3, 1.0);
    graph_add_edge(dag, 3, 1, 1.0);

    Vertex *order = NULL;
    size_t size = 0;
    ASSERT_EQ(graph_topological_sort(dag, &order, &size), DS_SUCCESS);
    ASSERT_EQ(size, 6);

    size_t pos[6];
    for (size_t i = 0; i < 6; i++) pos[order[i]] = i;
    ASSERT_TRUE(pos[5] < pos[2]);
    ASSERT_TRUE(pos[5] < pos[0]);
    ASSERT_TRUE(pos[2] < pos[3]);
    ASSERT_TRUE(pos[3] < pos[1]);
    ASSERT_TRUE(pos[4] < pos[0]);
    ASSERT_TRUE(pos[4] < pos[1]);

    free(order);
    graph_destroy(dag);

    Graph *cyc = graph_create(3, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(cyc, 0, 1, 1.0);
    graph_add_edge(cyc, 1, 2, 1.0);
    graph_add_edge(cyc, 2, 0, 1.0);
    ASSERT_EQ(graph_topological_sort(cyc, &order, &size), DS_ERROR_INVALID_PARAM);
    graph_destroy(cyc);
}

// ============================================================================
// TESTES - COMPONENTES CONEXOS
// ============================================================================

TEST(connected_components) {
    Graph *g = graph_create(6, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(g, 0, 1, 1.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 3, 4, 1.0);

    ASSERT_EQ(graph_num_connected_components(g), 3);

    graph_destroy(g);
}

TEST(strongly_connected_components) {
    Graph *g = graph_create(5, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, false);
    graph_add_edge(g, 0, 1, 1.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 2, 0, 1.0);
    graph_add_edge(g, 1, 3, 1.0);
    graph_add_edge(g, 3, 4, 1.0);

    size_t *components = NULL;
    size_t num_scc = 0;
    ASSERT_EQ(graph_strongly_connected_components(g, &components, &num_scc), DS_SUCCESS);
    ASSERT_TRUE(num_scc >= 3);
    ASSERT_EQ(components[0], components[1]);
    ASSERT_EQ(components[1], components[2]);
    ASSERT_NE(components[0], components[3]);
    ASSERT_NE(components[3], components[4]);

    free(components);
    graph_destroy(g);
}

// ============================================================================
// TESTES - CLONE E TRANSPOSE
// ============================================================================

TEST(clone) {
    Graph *g = create_sample_directed_list();
    Graph *c = graph_clone(g);
    ASSERT_NOT_NULL(c);
    ASSERT_EQ(graph_num_vertices(c), graph_num_vertices(g));
    ASSERT_EQ(graph_num_edges(c), graph_num_edges(g));
    ASSERT_TRUE(graph_has_edge(c, 0, 1));
    ASSERT_TRUE(graph_has_edge(c, 4, 5));
    ASSERT_FALSE(graph_has_edge(c, 1, 0));

    graph_destroy(c);
    graph_destroy(g);
}

TEST(transpose) {
    Graph *g = create_sample_directed_list();
    Graph *t = graph_transpose(g);
    ASSERT_NOT_NULL(t);
    ASSERT_EQ(graph_num_edges(t), graph_num_edges(g));

    ASSERT_TRUE(graph_has_edge(t, 1, 0));
    ASSERT_TRUE(graph_has_edge(t, 3, 0));
    ASSERT_FALSE(graph_has_edge(t, 0, 1));

    graph_destroy(t);
    graph_destroy(g);
}

// ============================================================================
// TESTES - MATRIX REPRESENTATION
// ============================================================================

TEST(matrix_full) {
    Graph *g = graph_create(4, GRAPH_DIRECTED, GRAPH_ADJACENCY_MATRIX, true);
    graph_add_edge(g, 0, 1, 2.0);
    graph_add_edge(g, 1, 2, 3.0);
    graph_add_edge(g, 2, 3, 4.0);
    graph_add_edge(g, 3, 0, 5.0);

    ASSERT_EQ(graph_num_edges(g), 4);
    ASSERT_TRUE(graph_has_edge(g, 0, 1));
    ASSERT_FALSE(graph_has_edge(g, 1, 0));
    ASSERT_TRUE(graph_edge_weight(g, 2, 3) > 3.9 && graph_edge_weight(g, 2, 3) < 4.1);

    ASSERT_TRUE(graph_has_cycle(g));

    Vertex buf[4];
    VisitData data = { buf, 0 };
    graph_bfs(g, 0, collect_vertex, &data);
    ASSERT_EQ(data.count, 4);

    double **mat = graph_to_adjacency_matrix(g);
    ASSERT_NOT_NULL(mat);
    ASSERT_TRUE(mat[0][1] > 1.9 && mat[0][1] < 2.1);
    ASSERT_TRUE(mat[1][0] < 0.01 && mat[1][0] > -0.01);
    for (size_t i = 0; i < 4; i++) free(mat[i]);
    free(mat);

    graph_destroy(g);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Graph Tests ===\n");

    RUN_TEST(create_destroy_list);
    RUN_TEST(create_destroy_matrix);
    RUN_TEST(add_vertex);
    RUN_TEST(add_vertex_matrix);
    RUN_TEST(remove_vertex);
    RUN_TEST(add_remove_edges_list);
    RUN_TEST(add_remove_edges_matrix);
    RUN_TEST(weighted_edges);
    RUN_TEST(invalid_edge_operations);
    RUN_TEST(degree);
    RUN_TEST(neighbors);
    RUN_TEST(get_all_edges);
    RUN_TEST(bfs_traversal);
    RUN_TEST(dfs_traversal);
    RUN_TEST(is_connected);
    RUN_TEST(has_cycle);
    RUN_TEST(is_tree);
    RUN_TEST(is_bipartite);
    RUN_TEST(topological_sort);
    RUN_TEST(connected_components);
    RUN_TEST(strongly_connected_components);
    RUN_TEST(clone);
    RUN_TEST(transpose);
    RUN_TEST(matrix_full);

    printf("\nAll Graph tests passed!\n");
    return 0;
}
