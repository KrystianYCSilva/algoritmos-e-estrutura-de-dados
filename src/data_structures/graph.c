/**
 * @file graph.c
 * @brief Implementacao de Grafo (Graph) - Lista de Adjacencias e Matriz
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 22 - Elementary Graph Algorithms
 * - Sedgewick & Wayne (2011), Chapter 4 - Graphs
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "data_structures/graph.h"
#include "data_structures/queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

// ============================================================================
// ESTRUTURAS INTERNAS
// ============================================================================

#define NO_EDGE 0.0
#define DEFAULT_WEIGHT 1.0

typedef struct AdjNode {
    Vertex dest;
    double weight;
    struct AdjNode *next;
} AdjNode;

struct Graph {
    size_t num_vertices;
    size_t num_edges;
    size_t capacity;
    GraphType type;
    GraphRepresentation representation;
    bool weighted;
    AdjNode **adj_list;
    double **adj_matrix;
};

// ============================================================================
// HELPERS INTERNOS
// ============================================================================

static AdjNode* create_adj_node(Vertex dest, double weight) {
    AdjNode *node = (AdjNode*)malloc(sizeof(AdjNode));
    if (node == NULL) return NULL;
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

static double** alloc_matrix(size_t n) {
    double **matrix = (double**)calloc(n, sizeof(double*));
    if (matrix == NULL) return NULL;
    for (size_t i = 0; i < n; i++) {
        matrix[i] = (double*)calloc(n, sizeof(double));
        if (matrix[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

static void free_matrix(double **matrix, size_t n) {
    if (matrix == NULL) return;
    for (size_t i = 0; i < n; i++) free(matrix[i]);
    free(matrix);
}

static void free_adj_list(AdjNode **list, size_t n) {
    if (list == NULL) return;
    for (size_t i = 0; i < n; i++) {
        AdjNode *curr = list[i];
        while (curr != NULL) {
            AdjNode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(list);
}

// ============================================================================
// CRIACAO E DESTRUICAO
// ============================================================================

Graph* graph_create(size_t num_vertices, GraphType type,
                    GraphRepresentation representation, bool weighted) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->num_vertices = num_vertices;
    g->num_edges = 0;
    g->capacity = num_vertices;
    g->type = type;
    g->representation = representation;
    g->weighted = weighted;
    g->adj_list = NULL;
    g->adj_matrix = NULL;

    if (representation == GRAPH_ADJACENCY_LIST) {
        g->adj_list = (AdjNode**)calloc(num_vertices, sizeof(AdjNode*));
        if (g->adj_list == NULL) {
            free(g);
            return NULL;
        }
    } else {
        g->adj_matrix = alloc_matrix(num_vertices);
        if (g->adj_matrix == NULL) {
            free(g);
            return NULL;
        }
    }

    return g;
}

void graph_destroy(Graph *graph) {
    if (graph == NULL) return;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        free_adj_list(graph->adj_list, graph->capacity);
    } else {
        free_matrix(graph->adj_matrix, graph->capacity);
    }

    free(graph);
}

// ============================================================================
// OPERACOES DE VERTICES
// ============================================================================

Vertex graph_add_vertex(Graph *graph) {
    if (graph == NULL) return (Vertex)-1;

    size_t new_v = graph->num_vertices;

    if (new_v >= graph->capacity) {
        size_t new_cap = graph->capacity == 0 ? 4 : graph->capacity * 2;

        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode **new_list = (AdjNode**)realloc(graph->adj_list, new_cap * sizeof(AdjNode*));
            if (new_list == NULL) return (Vertex)-1;
            memset(new_list + graph->capacity, 0, (new_cap - graph->capacity) * sizeof(AdjNode*));
            graph->adj_list = new_list;
        } else {
            double **new_matrix = alloc_matrix(new_cap);
            if (new_matrix == NULL) return (Vertex)-1;
            for (size_t i = 0; i < graph->num_vertices; i++) {
                memcpy(new_matrix[i], graph->adj_matrix[i], graph->num_vertices * sizeof(double));
            }
            free_matrix(graph->adj_matrix, graph->capacity);
            graph->adj_matrix = new_matrix;
        }

        graph->capacity = new_cap;
    }

    graph->num_vertices++;
    return new_v;
}

DataStructureError graph_remove_vertex(Graph *graph, Vertex v) {
    if (graph == NULL) return DS_ERROR_NULL_POINTER;
    if (v >= graph->num_vertices) return DS_ERROR_INVALID_INDEX;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[v];
        while (curr != NULL) {
            AdjNode *tmp = curr;
            curr = curr->next;
            if (graph->type == GRAPH_UNDIRECTED) {
                AdjNode **head = &graph->adj_list[tmp->dest];
                AdjNode *prev = NULL;
                AdjNode *node = *head;
                while (node != NULL) {
                    if (node->dest == v) {
                        if (prev == NULL) *head = node->next;
                        else prev->next = node->next;
                        free(node);
                        break;
                    }
                    prev = node;
                    node = node->next;
                }
            }
            graph->num_edges--;
            free(tmp);
        }
        graph->adj_list[v] = NULL;

        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (i == v) continue;
            AdjNode **head = &graph->adj_list[i];
            AdjNode *prev = NULL;
            AdjNode *node = *head;
            while (node != NULL) {
                if (node->dest == v) {
                    if (prev == NULL) *head = node->next;
                    else prev->next = node->next;
                    AdjNode *tmp = node;
                    node = node->next;
                    free(tmp);
                    if (graph->type == GRAPH_DIRECTED) graph->num_edges--;
                    continue;
                }
                prev = node;
                node = node->next;
            }
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[v][i] != NO_EDGE) {
                graph->adj_matrix[v][i] = NO_EDGE;
                graph->num_edges--;
            }
            if (graph->adj_matrix[i][v] != NO_EDGE && i != v) {
                graph->adj_matrix[i][v] = NO_EDGE;
                if (graph->type == GRAPH_DIRECTED) graph->num_edges--;
            }
        }
    }

    return DS_SUCCESS;
}

size_t graph_num_vertices(const Graph *graph) {
    if (graph == NULL) return 0;
    return graph->num_vertices;
}

// ============================================================================
// OPERACOES DE ARESTAS
// ============================================================================

DataStructureError graph_add_edge(Graph *graph, Vertex src, Vertex dest, double weight) {
    if (graph == NULL) return DS_ERROR_NULL_POINTER;
    if (src >= graph->num_vertices || dest >= graph->num_vertices)
        return DS_ERROR_INVALID_INDEX;

    if (!graph->weighted) weight = DEFAULT_WEIGHT;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[src];
        while (curr != NULL) {
            if (curr->dest == dest) {
                curr->weight = weight;
                if (graph->type == GRAPH_UNDIRECTED) {
                    AdjNode *rev = graph->adj_list[dest];
                    while (rev != NULL) {
                        if (rev->dest == src) { rev->weight = weight; break; }
                        rev = rev->next;
                    }
                }
                return DS_SUCCESS;
            }
            curr = curr->next;
        }

        AdjNode *node = create_adj_node(dest, weight);
        if (node == NULL) return DS_ERROR_OUT_OF_MEMORY;
        node->next = graph->adj_list[src];
        graph->adj_list[src] = node;

        if (graph->type == GRAPH_UNDIRECTED) {
            AdjNode *rev = create_adj_node(src, weight);
            if (rev == NULL) return DS_ERROR_OUT_OF_MEMORY;
            rev->next = graph->adj_list[dest];
            graph->adj_list[dest] = rev;
        }
    } else {
        graph->adj_matrix[src][dest] = weight;
        if (graph->type == GRAPH_UNDIRECTED)
            graph->adj_matrix[dest][src] = weight;
    }

    graph->num_edges++;
    return DS_SUCCESS;
}

DataStructureError graph_remove_edge(Graph *graph, Vertex src, Vertex dest) {
    if (graph == NULL) return DS_ERROR_NULL_POINTER;
    if (src >= graph->num_vertices || dest >= graph->num_vertices)
        return DS_ERROR_INVALID_INDEX;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode **head = &graph->adj_list[src];
        AdjNode *prev = NULL;
        AdjNode *curr = *head;
        bool found = false;

        while (curr != NULL) {
            if (curr->dest == dest) {
                if (prev == NULL) *head = curr->next;
                else prev->next = curr->next;
                free(curr);
                found = true;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if (!found) return DS_ERROR_NOT_FOUND;

        if (graph->type == GRAPH_UNDIRECTED) {
            head = &graph->adj_list[dest];
            prev = NULL;
            curr = *head;
            while (curr != NULL) {
                if (curr->dest == src) {
                    if (prev == NULL) *head = curr->next;
                    else prev->next = curr->next;
                    free(curr);
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
        }
    } else {
        if (graph->adj_matrix[src][dest] == NO_EDGE) return DS_ERROR_NOT_FOUND;
        graph->adj_matrix[src][dest] = NO_EDGE;
        if (graph->type == GRAPH_UNDIRECTED)
            graph->adj_matrix[dest][src] = NO_EDGE;
    }

    graph->num_edges--;
    return DS_SUCCESS;
}

bool graph_has_edge(const Graph *graph, Vertex src, Vertex dest) {
    if (graph == NULL) return false;
    if (src >= graph->num_vertices || dest >= graph->num_vertices) return false;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[src];
        while (curr != NULL) {
            if (curr->dest == dest) return true;
            curr = curr->next;
        }
        return false;
    } else {
        return graph->adj_matrix[src][dest] != NO_EDGE;
    }
}

double graph_edge_weight(const Graph *graph, Vertex src, Vertex dest) {
    if (graph == NULL) return 0.0;
    if (src >= graph->num_vertices || dest >= graph->num_vertices) return 0.0;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[src];
        while (curr != NULL) {
            if (curr->dest == dest) return curr->weight;
            curr = curr->next;
        }
        return 0.0;
    } else {
        return graph->adj_matrix[src][dest];
    }
}

size_t graph_num_edges(const Graph *graph) {
    if (graph == NULL) return 0;
    return graph->num_edges;
}
// ============================================================================
// GRAU DOS VERTICES
// ============================================================================

size_t graph_out_degree(const Graph *graph, Vertex v) {
    if (graph == NULL || v >= graph->num_vertices) return 0;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        size_t count = 0;
        AdjNode *curr = graph->adj_list[v];
        while (curr != NULL) {
            count++;
            curr = curr->next;
        }
        return count;
    } else {
        size_t count = 0;
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[v][i] != NO_EDGE) count++;
        }
        return count;
    }
}

size_t graph_in_degree(const Graph *graph, Vertex v) {
    if (graph == NULL || v >= graph->num_vertices) return 0;

    if (graph->type == GRAPH_UNDIRECTED) return graph_out_degree(graph, v);

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        size_t count = 0;
        for (size_t i = 0; i < graph->num_vertices; i++) {
            AdjNode *curr = graph->adj_list[i];
            while (curr != NULL) {
                if (curr->dest == v) count++;
                curr = curr->next;
            }
        }
        return count;
    } else {
        size_t count = 0;
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[i][v] != NO_EDGE) count++;
        }
        return count;
    }
}

size_t graph_degree(const Graph *graph, Vertex v) {
    if (graph == NULL || v >= graph->num_vertices) return 0;
    if (graph->type == GRAPH_UNDIRECTED) return graph_out_degree(graph, v);
    return graph_in_degree(graph, v) + graph_out_degree(graph, v);
}

// ============================================================================
// VIZINHOS E ADJACENCIA
// ============================================================================

DataStructureError graph_neighbors(const Graph *graph, Vertex v,
                                   Vertex **neighbors, size_t *count) {
    if (graph == NULL || neighbors == NULL || count == NULL)
        return DS_ERROR_NULL_POINTER;
    if (v >= graph->num_vertices) return DS_ERROR_INVALID_INDEX;

    size_t deg = graph_out_degree(graph, v);
    *count = deg;

    if (deg == 0) {
        *neighbors = NULL;
        return DS_SUCCESS;
    }

    *neighbors = (Vertex*)malloc(deg * sizeof(Vertex));
    if (*neighbors == NULL) return DS_ERROR_OUT_OF_MEMORY;

    size_t idx = 0;
    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[v];
        while (curr != NULL) {
            (*neighbors)[idx++] = curr->dest;
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[v][i] != NO_EDGE)
                (*neighbors)[idx++] = i;
        }
    }

    return DS_SUCCESS;
}

DataStructureError graph_edges(const Graph *graph, Edge **edges, size_t *count) {
    if (graph == NULL || edges == NULL || count == NULL)
        return DS_ERROR_NULL_POINTER;

    *count = graph->num_edges;
    if (graph->num_edges == 0) {
        *edges = NULL;
        return DS_SUCCESS;
    }

    *edges = (Edge*)malloc(graph->num_edges * sizeof(Edge));
    if (*edges == NULL) return DS_ERROR_OUT_OF_MEMORY;

    size_t idx = 0;
    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            AdjNode *curr = graph->adj_list[i];
            while (curr != NULL) {
                if (graph->type == GRAPH_DIRECTED || i <= curr->dest) {
                    (*edges)[idx].src = i;
                    (*edges)[idx].dest = curr->dest;
                    (*edges)[idx].weight = curr->weight;
                    idx++;
                }
                curr = curr->next;
            }
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            size_t start = (graph->type == GRAPH_UNDIRECTED) ? i : 0;
            for (size_t j = start; j < graph->num_vertices; j++) {
                if (graph->adj_matrix[i][j] != NO_EDGE) {
                    (*edges)[idx].src = i;
                    (*edges)[idx].dest = j;
                    (*edges)[idx].weight = graph->adj_matrix[i][j];
                    idx++;
                }
            }
        }
    }

    *count = idx;
    return DS_SUCCESS;
}

// ============================================================================
// TRAVESSIAS - BFS
// ============================================================================

void graph_bfs(const Graph *graph, Vertex start, VertexVisitFn visit, void *user_data) {
    if (graph == NULL || start >= graph->num_vertices || visit == NULL) return;

    bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) return;

    Queue *queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);
    if (queue == NULL) {
        free(visited);
        return;
    }

    visited[start] = true;
    queue_enqueue(queue, &start);

    while (!queue_is_empty(queue)) {
        Vertex u;
        queue_dequeue(queue, &u);
        visit(u, user_data);

        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode *curr = graph->adj_list[u];
            while (curr != NULL) {
                if (!visited[curr->dest]) {
                    visited[curr->dest] = true;
                    queue_enqueue(queue, &curr->dest);
                }
                curr = curr->next;
            }
        } else {
            for (size_t i = 0; i < graph->num_vertices; i++) {
                if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i]) {
                    visited[i] = true;
                    Vertex vi = i;
                    queue_enqueue(queue, &vi);
                }
            }
        }
    }

    queue_destroy(queue);
    free(visited);
}

// ============================================================================
// TRAVESSIAS - DFS
// ============================================================================

static void dfs_visit(const Graph *graph, Vertex u, bool *visited,
                      VertexVisitFn visit, void *user_data) {
    visited[u] = true;
    if (visit != NULL) visit(u, user_data);

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (!visited[curr->dest])
                dfs_visit(graph, curr->dest, visited, visit, user_data);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i])
                dfs_visit(graph, i, visited, visit, user_data);
        }
    }
}

void graph_dfs(const Graph *graph, Vertex start, VertexVisitFn visit, void *user_data) {
    if (graph == NULL || start >= graph->num_vertices || visit == NULL) return;

    bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) return;

    dfs_visit(graph, start, visited, visit, user_data);

    free(visited);
}
// ============================================================================
// PROPRIEDADES DO GRAFO
// ============================================================================

bool graph_is_connected(const Graph *graph) {
    if (graph == NULL || graph->num_vertices == 0) return false;
    if (graph->num_vertices == 1) return true;

    bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) return false;

    Queue *queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);
    if (queue == NULL) { free(visited); return false; }

    Vertex start = 0;
    visited[0] = true;
    queue_enqueue(queue, &start);

    size_t count = 0;
    while (!queue_is_empty(queue)) {
        Vertex u;
        queue_dequeue(queue, &u);
        count++;

        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode *curr = graph->adj_list[u];
            while (curr != NULL) {
                if (!visited[curr->dest]) {
                    visited[curr->dest] = true;
                    queue_enqueue(queue, &curr->dest);
                }
                curr = curr->next;
            }
        } else {
            for (size_t i = 0; i < graph->num_vertices; i++) {
                if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i]) {
                    visited[i] = true;
                    Vertex vi = i;
                    queue_enqueue(queue, &vi);
                }
            }
        }
    }

    queue_destroy(queue);
    free(visited);
    return count == graph->num_vertices;
}

bool graph_is_strongly_connected(const Graph *graph) {
    if (graph == NULL || graph->num_vertices == 0) return false;
    if (graph->type == GRAPH_UNDIRECTED) return graph_is_connected(graph);

    bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) return false;

    Queue *queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);
    Vertex start = 0;
    visited[0] = true;
    queue_enqueue(queue, &start);
    size_t count = 0;

    while (!queue_is_empty(queue)) {
        Vertex u;
        queue_dequeue(queue, &u);
        count++;

        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode *curr = graph->adj_list[u];
            while (curr != NULL) {
                if (!visited[curr->dest]) {
                    visited[curr->dest] = true;
                    queue_enqueue(queue, &curr->dest);
                }
                curr = curr->next;
            }
        } else {
            for (size_t i = 0; i < graph->num_vertices; i++) {
                if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i]) {
                    visited[i] = true;
                    Vertex vi = i;
                    queue_enqueue(queue, &vi);
                }
            }
        }
    }

    queue_destroy(queue);
    free(visited);

    if (count != graph->num_vertices) return false;

    Graph *t = graph_transpose(graph);
    if (t == NULL) return false;

    visited = (bool*)calloc(t->num_vertices, sizeof(bool));
    queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);
    visited[0] = true;
    queue_enqueue(queue, &start);
    count = 0;

    while (!queue_is_empty(queue)) {
        Vertex u;
        queue_dequeue(queue, &u);
        count++;

        if (t->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode *curr = t->adj_list[u];
            while (curr != NULL) {
                if (!visited[curr->dest]) {
                    visited[curr->dest] = true;
                    queue_enqueue(queue, &curr->dest);
                }
                curr = curr->next;
            }
        } else {
            for (size_t i = 0; i < t->num_vertices; i++) {
                if (t->adj_matrix[u][i] != NO_EDGE && !visited[i]) {
                    visited[i] = true;
                    Vertex vi = i;
                    queue_enqueue(queue, &vi);
                }
            }
        }
    }

    queue_destroy(queue);
    free(visited);
    graph_destroy(t);
    return count == graph->num_vertices;
}

static bool has_cycle_directed_dfs(const Graph *graph, Vertex u, int *color) {
    color[u] = 1; // GRAY

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (color[curr->dest] == 1) return true;
            if (color[curr->dest] == 0 && has_cycle_directed_dfs(graph, curr->dest, color))
                return true;
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE) {
                if (color[i] == 1) return true;
                if (color[i] == 0 && has_cycle_directed_dfs(graph, i, color))
                    return true;
            }
        }
    }

    color[u] = 2; // BLACK
    return false;
}

static bool has_cycle_undirected_dfs(const Graph *graph, Vertex u,
                                     bool *visited, Vertex parent) {
    visited[u] = true;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (!visited[curr->dest]) {
                if (has_cycle_undirected_dfs(graph, curr->dest, visited, u))
                    return true;
            } else if (curr->dest != parent) {
                return true;
            }
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE) {
                if (!visited[i]) {
                    if (has_cycle_undirected_dfs(graph, i, visited, u))
                        return true;
                } else if (i != parent) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool graph_has_cycle(const Graph *graph) {
    if (graph == NULL || graph->num_vertices == 0) return false;

    if (graph->type == GRAPH_DIRECTED) {
        int *color = (int*)calloc(graph->num_vertices, sizeof(int));
        if (color == NULL) return false;

        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (color[i] == 0) {
                if (has_cycle_directed_dfs(graph, i, color)) {
                    free(color);
                    return true;
                }
            }
        }
        free(color);
        return false;
    } else {
        bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
        if (visited == NULL) return false;

        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (!visited[i]) {
                if (has_cycle_undirected_dfs(graph, i, visited, (Vertex)-1)) {
                    free(visited);
                    return true;
                }
            }
        }
        free(visited);
        return false;
    }
}

bool graph_is_tree(const Graph *graph) {
    if (graph == NULL) return false;
    if (graph->type == GRAPH_DIRECTED) return false;
    return graph_is_connected(graph) && graph->num_edges == graph->num_vertices - 1;
}

bool graph_is_bipartite(const Graph *graph) {
    if (graph == NULL || graph->num_vertices == 0) return true;

    int *color = (int*)malloc(graph->num_vertices * sizeof(int));
    if (color == NULL) return false;
    memset(color, -1, graph->num_vertices * sizeof(int));

    Queue *queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);

    for (size_t s = 0; s < graph->num_vertices; s++) {
        if (color[s] != -1) continue;
        color[s] = 0;
        queue_enqueue(queue, &s);

        while (!queue_is_empty(queue)) {
            Vertex u;
            queue_dequeue(queue, &u);

            if (graph->representation == GRAPH_ADJACENCY_LIST) {
                AdjNode *curr = graph->adj_list[u];
                while (curr != NULL) {
                    if (color[curr->dest] == -1) {
                        color[curr->dest] = 1 - color[u];
                        queue_enqueue(queue, &curr->dest);
                    } else if (color[curr->dest] == color[u]) {
                        queue_destroy(queue);
                        free(color);
                        return false;
                    }
                    curr = curr->next;
                }
            } else {
                for (size_t i = 0; i < graph->num_vertices; i++) {
                    if (graph->adj_matrix[u][i] != NO_EDGE) {
                        if (color[i] == -1) {
                            color[i] = 1 - color[u];
                            Vertex vi = i;
                            queue_enqueue(queue, &vi);
                        } else if (color[i] == color[u]) {
                            queue_destroy(queue);
                            free(color);
                            return false;
                        }
                    }
                }
            }
        }
    }

    queue_destroy(queue);
    free(color);
    return true;
}

// ============================================================================
// ORDENACAO TOPOLOGICA
// ============================================================================

static void topo_dfs(const Graph *graph, Vertex u, bool *visited,
                     Vertex *stack, size_t *stack_idx) {
    visited[u] = true;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (!visited[curr->dest])
                topo_dfs(graph, curr->dest, visited, stack, stack_idx);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i])
                topo_dfs(graph, i, visited, stack, stack_idx);
        }
    }

    stack[(*stack_idx)++] = u;
}

DataStructureError graph_topological_sort(const Graph *graph, Vertex **order, size_t *size) {
    if (graph == NULL || order == NULL || size == NULL)
        return DS_ERROR_NULL_POINTER;
    if (graph->type != GRAPH_DIRECTED) return DS_ERROR_INVALID_PARAM;
    if (graph_has_cycle(graph)) return DS_ERROR_INVALID_PARAM;

    size_t n = graph->num_vertices;
    *size = n;

    bool *visited = (bool*)calloc(n, sizeof(bool));
    Vertex *stack = (Vertex*)malloc(n * sizeof(Vertex));
    if (visited == NULL || stack == NULL) {
        free(visited);
        free(stack);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t stack_idx = 0;
    for (size_t i = 0; i < n; i++) {
        if (!visited[i])
            topo_dfs(graph, i, visited, stack, &stack_idx);
    }

    *order = (Vertex*)malloc(n * sizeof(Vertex));
    if (*order == NULL) {
        free(visited);
        free(stack);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < n; i++) {
        (*order)[i] = stack[n - 1 - i];
    }

    free(visited);
    free(stack);
    return DS_SUCCESS;
}

// ============================================================================
// COMPONENTES CONEXOS
// ============================================================================

size_t graph_num_connected_components(const Graph *graph) {
    if (graph == NULL || graph->num_vertices == 0) return 0;

    bool *visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) return 0;

    size_t components = 0;
    for (size_t i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            components++;
            Queue *queue = queue_create(sizeof(Vertex), QUEUE_LINKED, 0, NULL);
            visited[i] = true;
            Vertex vi = i;
            queue_enqueue(queue, &vi);

            while (!queue_is_empty(queue)) {
                Vertex u;
                queue_dequeue(queue, &u);

                if (graph->representation == GRAPH_ADJACENCY_LIST) {
                    AdjNode *curr = graph->adj_list[u];
                    while (curr != NULL) {
                        if (!visited[curr->dest]) {
                            visited[curr->dest] = true;
                            queue_enqueue(queue, &curr->dest);
                        }
                        curr = curr->next;
                    }
                } else {
                    for (size_t j = 0; j < graph->num_vertices; j++) {
                        if (graph->adj_matrix[u][j] != NO_EDGE && !visited[j]) {
                            visited[j] = true;
                            Vertex vj = j;
                            queue_enqueue(queue, &vj);
                        }
                    }
                }
            }
            queue_destroy(queue);
        }
    }

    free(visited);
    return components;
}

static void kosaraju_dfs1(const Graph *graph, Vertex u, bool *visited,
                          Vertex *finish_order, size_t *idx) {
    visited[u] = true;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (!visited[curr->dest])
                kosaraju_dfs1(graph, curr->dest, visited, finish_order, idx);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i])
                kosaraju_dfs1(graph, i, visited, finish_order, idx);
        }
    }

    finish_order[(*idx)++] = u;
}

static void kosaraju_dfs2(const Graph *graph, Vertex u, bool *visited, size_t comp_id,
                          size_t *comp_map) {
    visited[u] = true;
    comp_map[u] = comp_id;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        AdjNode *curr = graph->adj_list[u];
        while (curr != NULL) {
            if (!visited[curr->dest])
                kosaraju_dfs2(graph, curr->dest, visited, comp_id, comp_map);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[u][i] != NO_EDGE && !visited[i])
                kosaraju_dfs2(graph, i, visited, comp_id, comp_map);
        }
    }
}

DataStructureError graph_strongly_connected_components(const Graph *graph,
                                                       size_t **components,
                                                       size_t *num_components) {
    if (graph == NULL || components == NULL || num_components == NULL)
        return DS_ERROR_NULL_POINTER;

    size_t n = graph->num_vertices;
    bool *visited = (bool*)calloc(n, sizeof(bool));
    Vertex *finish_order = (Vertex*)malloc(n * sizeof(Vertex));
    if (visited == NULL || finish_order == NULL) {
        free(visited); free(finish_order);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    size_t idx = 0;
    for (size_t i = 0; i < n; i++) {
        if (!visited[i])
            kosaraju_dfs1(graph, i, visited, finish_order, &idx);
    }

    Graph *t = graph_transpose(graph);
    if (t == NULL) {
        free(visited); free(finish_order);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    *components = (size_t*)malloc(n * sizeof(size_t));
    if (*components == NULL) {
        free(visited); free(finish_order); graph_destroy(t);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    memset(visited, 0, n * sizeof(bool));
    *num_components = 0;

    for (size_t i = n; i > 0; i--) {
        Vertex u = finish_order[i - 1];
        if (!visited[u]) {
            kosaraju_dfs2(t, u, visited, *num_components, *components);
            (*num_components)++;
        }
    }

    free(visited);
    free(finish_order);
    graph_destroy(t);
    return DS_SUCCESS;
}

// ============================================================================
// CONVERSOES E UTILIDADES
// ============================================================================

double** graph_to_adjacency_matrix(const Graph *graph) {
    if (graph == NULL) return NULL;

    double **matrix = alloc_matrix(graph->num_vertices);
    if (matrix == NULL) return NULL;

    if (graph->representation == GRAPH_ADJACENCY_MATRIX) {
        for (size_t i = 0; i < graph->num_vertices; i++)
            memcpy(matrix[i], graph->adj_matrix[i], graph->num_vertices * sizeof(double));
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            AdjNode *curr = graph->adj_list[i];
            while (curr != NULL) {
                matrix[i][curr->dest] = curr->weight;
                curr = curr->next;
            }
        }
    }

    return matrix;
}

void graph_print(const Graph *graph) {
    if (graph == NULL) {
        printf("Graph: NULL\n");
        return;
    }

    printf("Graph(%s, %s, V=%zu, E=%zu):\n",
           graph->type == GRAPH_DIRECTED ? "directed" : "undirected",
           graph->representation == GRAPH_ADJACENCY_LIST ? "adj_list" : "adj_matrix",
           graph->num_vertices, graph->num_edges);

    for (size_t i = 0; i < graph->num_vertices; i++) {
        printf("  %zu -> ", i);
        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            AdjNode *curr = graph->adj_list[i];
            while (curr != NULL) {
                if (graph->weighted)
                    printf("%zu(%.1f) ", curr->dest, curr->weight);
                else
                    printf("%zu ", curr->dest);
                curr = curr->next;
            }
        } else {
            for (size_t j = 0; j < graph->num_vertices; j++) {
                if (graph->adj_matrix[i][j] != NO_EDGE) {
                    if (graph->weighted)
                        printf("%zu(%.1f) ", j, graph->adj_matrix[i][j]);
                    else
                        printf("%zu ", j);
                }
            }
        }
        printf("\n");
    }
}

Graph* graph_clone(const Graph *graph) {
    if (graph == NULL) return NULL;

    Graph *clone = graph_create(graph->num_vertices, graph->type,
                                graph->representation, graph->weighted);
    if (clone == NULL) return NULL;

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            AdjNode *curr = graph->adj_list[i];
            AdjNode **tail = &clone->adj_list[i];
            while (curr != NULL) {
                AdjNode *node = create_adj_node(curr->dest, curr->weight);
                if (node == NULL) { graph_destroy(clone); return NULL; }
                *tail = node;
                tail = &node->next;
                curr = curr->next;
            }
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++)
            memcpy(clone->adj_matrix[i], graph->adj_matrix[i],
                   graph->num_vertices * sizeof(double));
    }

    clone->num_edges = graph->num_edges;
    return clone;
}

Graph* graph_transpose(const Graph *graph) {
    if (graph == NULL) return NULL;

    Graph *t = graph_create(graph->num_vertices, graph->type,
                            graph->representation, graph->weighted);
    if (t == NULL) return NULL;

    if (graph->type == GRAPH_UNDIRECTED) {
        if (graph->representation == GRAPH_ADJACENCY_LIST) {
            for (size_t i = 0; i < graph->num_vertices; i++) {
                AdjNode *curr = graph->adj_list[i];
                AdjNode **tail = &t->adj_list[i];
                while (curr != NULL) {
                    AdjNode *node = create_adj_node(curr->dest, curr->weight);
                    *tail = node;
                    tail = &node->next;
                    curr = curr->next;
                }
            }
        } else {
            for (size_t i = 0; i < graph->num_vertices; i++)
                memcpy(t->adj_matrix[i], graph->adj_matrix[i],
                       graph->num_vertices * sizeof(double));
        }
        t->num_edges = graph->num_edges;
        return t;
    }

    if (graph->representation == GRAPH_ADJACENCY_LIST) {
        for (size_t i = 0; i < graph->num_vertices; i++) {
            AdjNode *curr = graph->adj_list[i];
            while (curr != NULL) {
                AdjNode *node = create_adj_node(i, curr->weight);
                node->next = t->adj_list[curr->dest];
                t->adj_list[curr->dest] = node;
                curr = curr->next;
            }
        }
    } else {
        for (size_t i = 0; i < graph->num_vertices; i++)
            for (size_t j = 0; j < graph->num_vertices; j++)
                t->adj_matrix[j][i] = graph->adj_matrix[i][j];
    }

    t->num_edges = graph->num_edges;
    return t;
}
