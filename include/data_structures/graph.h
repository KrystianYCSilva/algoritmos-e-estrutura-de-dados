/**
 * @file graph.h
 * @brief Implementação genérica de Grafo (Graph)
 *
 * Grafo G = (V, E) onde V é conjunto de vértices e E conjunto de arestas.
 * Suporta grafos direcionados e não-direcionados, ponderados e não-ponderados.
 *
 * Representações:
 * - Adjacency List: melhor para grafos esparsos (|E| << |V|²)
 * - Adjacency Matrix: melhor para grafos densos (|E| ≈ |V|²)
 *
 * Complexidade (Adjacency List):
 * - Add vertex: O(1)
 * - Add edge: O(1)
 * - Remove vertex: O(|V| + |E|)
 * - Remove edge: O(|V|)
 * - Check edge: O(|V|) pior caso
 * - Get neighbors: O(1) para acessar lista
 *
 * Complexidade (Adjacency Matrix):
 * - Add vertex: O(|V|²) [realloc]
 * - Add edge: O(1)
 * - Remove vertex: O(|V|²)
 * - Remove edge: O(1)
 * - Check edge: O(1)
 * - Get neighbors: O(|V|)
 *
 * Referências Acadêmicas:
 * - Cormen, T. H., et al. (2009). "Introduction to Algorithms" (3rd ed.),
 *   Chapter 22 - Elementary Graph Algorithms
 * - Sedgewick, R., & Wayne, K. (2011). "Algorithms" (4th ed.), Chapter 4
 * - Diestel, R. (2017). "Graph Theory" (5th ed.)
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// ESTRUTURAS OPACAS
// ============================================================================

typedef struct Graph Graph;
typedef size_t Vertex;  /**< Identificador de vértice (índice) */

/**
 * @brief Tipo de grafo
 */
typedef enum {
    GRAPH_DIRECTED,    /**< Grafo direcionado (digrafo) */
    GRAPH_UNDIRECTED   /**< Grafo não-direcionado */
} GraphType;

/**
 * @brief Representação interna
 */
typedef enum {
    GRAPH_ADJACENCY_LIST,    /**< Lista de adjacências (esparso) */
    GRAPH_ADJACENCY_MATRIX   /**< Matriz de adjacências (denso) */
} GraphRepresentation;

/**
 * @brief Aresta do grafo
 */
typedef struct {
    Vertex src;      /**< Vértice origem */
    Vertex dest;     /**< Vértice destino */
    double weight;   /**< Peso da aresta (1.0 se não-ponderado) */
} Edge;

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria um novo grafo
 *
 * @param num_vertices Número inicial de vértices
 * @param type Tipo (direcionado ou não)
 * @param representation Representação (lista ou matriz)
 * @param weighted Se true, arestas têm pesos
 * @return Graph* Grafo criado
 *
 * Complexidade: O(V) para lista, O(V²) para matriz
 */
Graph* graph_create(size_t num_vertices, GraphType type,
                    GraphRepresentation representation, bool weighted);

/**
 * @brief Destrói o grafo
 */
void graph_destroy(Graph *graph);

// ============================================================================
// OPERAÇÕES DE VÉRTICES
// ============================================================================

/**
 * @brief Adiciona um novo vértice
 *
 * @return Vertex Identificador do vértice criado
 */
Vertex graph_add_vertex(Graph *graph);

/**
 * @brief Remove um vértice e todas as arestas conectadas
 */
DataStructureError graph_remove_vertex(Graph *graph, Vertex v);

/**
 * @brief Retorna o número de vértices
 */
size_t graph_num_vertices(const Graph *graph);

// ============================================================================
// OPERAÇÕES DE ARESTAS
// ============================================================================

/**
 * @brief Adiciona uma aresta (ou atualiza peso se já existe)
 *
 * @param graph Ponteiro para o grafo
 * @param src Vértice origem
 * @param dest Vértice destino
 * @param weight Peso (1.0 se não-ponderado)
 *
 * Para grafo não-direcionado, adiciona aresta bidirecional.
 *
 * Complexidade: O(1) lista (amortizado), O(1) matriz
 */
DataStructureError graph_add_edge(Graph *graph, Vertex src, Vertex dest, double weight);

/**
 * @brief Remove uma aresta
 */
DataStructureError graph_remove_edge(Graph *graph, Vertex src, Vertex dest);

/**
 * @brief Verifica se existe aresta entre dois vértices
 */
bool graph_has_edge(const Graph *graph, Vertex src, Vertex dest);

/**
 * @brief Retorna o peso de uma aresta
 */
double graph_edge_weight(const Graph *graph, Vertex src, Vertex dest);

/**
 * @brief Retorna o número de arestas
 */
size_t graph_num_edges(const Graph *graph);

// ============================================================================
// GRAU DOS VÉRTICES
// ============================================================================

/**
 * @brief Retorna o grau de saída (out-degree) de um vértice
 */
size_t graph_out_degree(const Graph *graph, Vertex v);

/**
 * @brief Retorna o grau de entrada (in-degree) de um vértice
 */
size_t graph_in_degree(const Graph *graph, Vertex v);

/**
 * @brief Retorna o grau total (in + out) de um vértice
 */
size_t graph_degree(const Graph *graph, Vertex v);

// ============================================================================
// VIZINHOS E ADJACÊNCIA
// ============================================================================

/**
 * @brief Retorna array com vizinhos (sucessores) de um vértice
 *
 * @param graph Ponteiro para o grafo
 * @param v Vértice
 * @param neighbors Array de saída (será alocado)
 * @param count Número de vizinhos
 *
 * Array deve ser liberado com free().
 */
DataStructureError graph_neighbors(const Graph *graph, Vertex v,
                                   Vertex **neighbors, size_t *count);

/**
 * @brief Retorna todas as arestas do grafo
 *
 * @param graph Ponteiro para o grafo
 * @param edges Array de arestas (será alocado)
 * @param count Número de arestas
 */
DataStructureError graph_edges(const Graph *graph, Edge **edges, size_t *count);

// ============================================================================
// TRAVESSIAS
// ============================================================================

typedef void (*VertexVisitFn)(Vertex v, void *user_data);

/**
 * @brief Busca em largura (BFS - Breadth-First Search)
 *
 * @param graph Ponteiro para o grafo
 * @param start Vértice inicial
 * @param visit Função callback para cada vértice visitado
 * @param user_data Dados customizados
 *
 * Pseudocódigo (Cormen et al., 2009, p. 594):
 * BFS(G, s)
 *   for each vertex u ∈ G.V - {s}
 *       u.color = WHITE
 *       u.d = ∞
 *       u.π = NIL
 *   s.color = GRAY
 *   s.d = 0
 *   s.π = NIL
 *   Q = ∅
 *   ENQUEUE(Q, s)
 *   while Q ≠ ∅
 *       u = DEQUEUE(Q)
 *       for each v ∈ G.Adj[u]
 *           if v.color == WHITE
 *               v.color = GRAY
 *               v.d = u.d + 1
 *               v.π = u
 *               ENQUEUE(Q, v)
 *       u.color = BLACK
 *
 * Complexidade: O(V + E)
 */
void graph_bfs(const Graph *graph, Vertex start, VertexVisitFn visit, void *user_data);

/**
 * @brief Busca em profundidade (DFS - Depth-First Search)
 *
 * @param graph Ponteiro para o grafo
 * @param start Vértice inicial
 * @param visit Função callback
 * @param user_data Dados customizados
 *
 * Pseudocódigo (Cormen et al., 2009, p. 604):
 * DFS(G)
 *   for each vertex u ∈ G.V
 *       u.color = WHITE
 *       u.π = NIL
 *   time = 0
 *   for each vertex u ∈ G.V
 *       if u.color == WHITE
 *           DFS-VISIT(G, u)
 *
 * DFS-VISIT(G, u)
 *   time = time + 1
 *   u.d = time
 *   u.color = GRAY
 *   for each v ∈ G.Adj[u]
 *       if v.color == WHITE
 *           v.π = u
 *           DFS-VISIT(G, v)
 *   u.color = BLACK
 *   time = time + 1
 *   u.f = time
 *
 * Complexidade: O(V + E)
 */
void graph_dfs(const Graph *graph, Vertex start, VertexVisitFn visit, void *user_data);

// ============================================================================
// PROPRIEDADES DO GRAFO
// ============================================================================

/**
 * @brief Verifica se o grafo é conexo (não-direcionado)
 */
bool graph_is_connected(const Graph *graph);

/**
 * @brief Verifica se o grafo é fortemente conexo (direcionado)
 */
bool graph_is_strongly_connected(const Graph *graph);

/**
 * @brief Verifica se o grafo tem ciclos
 */
bool graph_has_cycle(const Graph *graph);

/**
 * @brief Verifica se o grafo é árvore
 * (conexo, sem ciclos, |E| = |V| - 1)
 */
bool graph_is_tree(const Graph *graph);

/**
 * @brief Verifica se o grafo é bipartido
 */
bool graph_is_bipartite(const Graph *graph);

// ============================================================================
// ORDENAÇÃO TOPOLÓGICA
// ============================================================================

/**
 * @brief Ordenação topológica (apenas grafos direcionados acíclicos - DAG)
 *
 * @param graph Grafo (deve ser DAG)
 * @param order Array de saída com ordem topológica (será alocado)
 * @param size Tamanho do array
 * @return DataStructureError DS_ERROR_INVALID_PARAM se grafo tem ciclo
 *
 * Pseudocódigo: DFS modificado (Cormen et al., 2009, p. 613)
 * TOPOLOGICAL-SORT(G)
 *   call DFS(G) to compute finishing times v.f for each vertex v
 *   as each vertex is finished, insert it onto the front of a linked list
 *   return the linked list of vertices
 *
 * Complexidade: O(V + E)
 */
DataStructureError graph_topological_sort(const Graph *graph, Vertex **order, size_t *size);

// ============================================================================
// COMPONENTES CONEXOS
// ============================================================================

/**
 * @brief Retorna número de componentes conexos
 */
size_t graph_num_connected_components(const Graph *graph);

/**
 * @brief Retorna componentes fortemente conexos (algoritmo de Kosaraju)
 *
 * Referência: Cormen et al., 2009, p. 615
 * Complexidade: O(V + E)
 */
DataStructureError graph_strongly_connected_components(const Graph *graph,
                                                       size_t **components,
                                                       size_t *num_components);

// ============================================================================
// CONVERSÕES E UTILIDADES
// ============================================================================

/**
 * @brief Converte para matriz de adjacências
 */
double** graph_to_adjacency_matrix(const Graph *graph);

/**
 * @brief Imprime o grafo
 */
void graph_print(const Graph *graph);

/**
 * @brief Cria cópia do grafo
 */
Graph* graph_clone(const Graph *graph);

/**
 * @brief Retorna o grafo transposto (inverte todas as arestas)
 *
 * Útil para algoritmos como Kosaraju (SCCs).
 * Apenas para grafos direcionados.
 *
 * Complexidade: O(V + E)
 */
Graph* graph_transpose(const Graph *graph);

#endif // GRAPH_H
