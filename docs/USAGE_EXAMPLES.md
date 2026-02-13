---
description: "Exemplos de uso das estruturas de dados e algoritmos da biblioteca, com código C11 funcional."
---

# Exemplos de Uso - Algoritmos e Heurísticas

> Exemplos práticos demonstrando a API da biblioteca. Todos compilam com:
> ```bash
> gcc -std=c11 -Wall -Wextra -I include -o example example.c src/data_structures/*.c src/algorithms/*.c -lm
> ```

---

## 1. Estruturas de Dados

### 1.1 Queue (Fila)

```c
#include "data_structures/queue.h"
#include "data_structures/common.h"

int main(void) {
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);

    int values[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        queue_enqueue(q, &values[i]);
    }

    int front;
    queue_front(q, &front);       // front = 10

    queue_dequeue(q, &front);     // front = 10, removido
    queue_dequeue(q, &front);     // front = 20, removido

    // Tamanho atual: 3
    size_t size = queue_size(q);  // size = 3

    queue_destroy(q);
    return 0;
}
```

### 1.2 Stack (Pilha)

```c
#include "data_structures/stack.h"
#include "data_structures/common.h"

int main(void) {
    Stack *s = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);

    int val = 42;
    stack_push(s, &val);
    val = 99;
    stack_push(s, &val);

    int top;
    stack_top(s, &top);    // top = 99 (peek sem remover)
    stack_pop(s, &top);    // top = 99 (removido)
    stack_pop(s, &top);    // top = 42

    stack_destroy(s);
    return 0;
}
```

### 1.3 LinkedList

```c
#include "data_structures/linked_list.h"
#include "data_structures/common.h"

int main(void) {
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);

    int v1 = 10, v2 = 20, v3 = 30;
    list_push_back(list, &v1);
    list_push_back(list, &v2);
    list_push_front(list, &v3);   // [30, 10, 20]

    int out;
    list_get(list, 0, &out);      // out = 30
    list_get(list, 1, &out);      // out = 10

    list_remove_at(list, 0, &out); // remove 30

    list_destroy(list);
    return 0;
}
```

### 1.4 ArrayList

```c
#include "data_structures/array_list.h"
#include "data_structures/common.h"

int main(void) {
    ArrayList *arr = arraylist_create(sizeof(double), 16, NULL);

    double vals[] = {3.14, 2.71, 1.41, 1.73};
    for (size_t i = 0; i < 4; i++) {
        arraylist_push_back(arr, &vals[i]);
    }

    double out;
    arraylist_get(arr, 2, &out);   // out = 1.41

    arraylist_sort(arr, compare_double);

    // Após sort: [1.41, 1.73, 2.71, 3.14]
    double target = 2.71;
    size_t idx;
    arraylist_binary_search(arr, &target, compare_double, &idx);
    // idx = 2

    arraylist_destroy(arr);
    return 0;
}
```

### 1.5 HashTable

```c
#include "data_structures/hash_table.h"
#include "data_structures/common.h"
#include <string.h>

int main(void) {
    HashTable *ht = hashtable_create(
        sizeof(char*), sizeof(int), 16,
        hash_string, compare_string,
        HASH_CHAINING,
        destroy_string, NULL
    );

    char *key1 = strdup("idade");
    int val1 = 25;
    hashtable_put(ht, &key1, &val1);

    char *key2 = strdup("altura");
    int val2 = 180;
    hashtable_put(ht, &key2, &val2);

    int result;
    char *search = "idade";
    hashtable_get(ht, &search, &result);  // result = 25

    bool found = hashtable_contains(ht, &search);  // true

    hashtable_destroy(ht);
    return 0;
}
```

### 1.6 BST (Binary Search Tree)

```c
#include "data_structures/bst.h"
#include "data_structures/common.h"

int main(void) {
    BST *tree = bst_create(sizeof(int), compare_int, NULL);

    int vals[] = {50, 30, 70, 20, 40, 60, 80};
    for (size_t i = 0; i < 7; i++) {
        bst_insert(tree, &vals[i]);
    }

    int key = 40;
    bool found = bst_search(tree, &key);  // true

    int min, max;
    bst_min(tree, &min);   // min = 20
    bst_max(tree, &max);   // max = 80

    bst_remove(tree, &key);  // remove 40
    bst_destroy(tree);
    return 0;
}
```

### 1.7 AVL Tree

```c
#include "data_structures/avl_tree.h"
#include "data_structures/common.h"

int main(void) {
    AVLTree *avl = avl_create(sizeof(int), compare_int, NULL);

    for (int i = 1; i <= 100; i++) {
        avl_insert(avl, &i);
    }
    // Árvore auto-balanceada: altura ~7 (log2(100))

    int key = 42;
    bool found = avl_search(avl, &key);  // true

    avl_remove(avl, &key);
    // Rebalanceamento automático

    avl_destroy(avl);
    return 0;
}
```

### 1.8 Heap e Priority Queue

```c
#include "data_structures/heap.h"
#include "data_structures/priority_queue.h"
#include "data_structures/common.h"

int main(void) {
    // Min-Heap
    Heap *h = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);

    int vals[] = {42, 15, 88, 3, 57};
    for (size_t i = 0; i < 5; i++) {
        heap_insert(h, &vals[i]);
    }

    int min;
    heap_peek(h, &min);       // min = 3
    heap_extract(h, &min);    // min = 3 (removido)
    heap_peek(h, &min);       // min = 15

    heap_destroy(h);

    // Priority Queue (wrapper sobre Heap)
    PriorityQueue *pq = priority_queue_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);

    int p1 = 100, p2 = 5, p3 = 50;
    priority_queue_insert(pq, &p1);
    priority_queue_insert(pq, &p2);
    priority_queue_insert(pq, &p3);

    int highest;
    priority_queue_extract(pq, &highest);  // highest = 5 (menor prioridade)

    priority_queue_destroy(pq);
    return 0;
}
```

### 1.9 Graph

```c
#include "data_structures/graph.h"
#include "data_structures/common.h"

int main(void) {
    Graph *g = graph_create(5, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);

    graph_add_edge(g, 0, 1, 4.0);
    graph_add_edge(g, 0, 2, 1.0);
    graph_add_edge(g, 1, 3, 2.0);
    graph_add_edge(g, 2, 3, 5.0);
    graph_add_edge(g, 3, 4, 3.0);

    bool connected = graph_is_connected(g);  // true

    // BFS a partir do vértice 0
    size_t *bfs_order = NULL;
    size_t bfs_count = 0;
    graph_bfs(g, 0, &bfs_order, &bfs_count);
    // bfs_order = [0, 1, 2, 3, 4]
    free(bfs_order);

    graph_destroy(g);
    return 0;
}
```

### 1.10 Trie

```c
#include "data_structures/trie.h"

int main(void) {
    Trie *t = trie_create(26);  // alfabeto a-z

    trie_insert(t, "algorithm");
    trie_insert(t, "algo");
    trie_insert(t, "alpha");
    trie_insert(t, "beta");

    bool found = trie_search(t, "algo");          // true
    bool prefix = trie_starts_with(t, "alg");     // true

    // Autocomplete
    char **results = NULL;
    size_t count = 0;
    trie_autocomplete(t, "al", &results, &count);
    // results = ["algo", "algorithm", "alpha"], count = 3

    for (size_t i = 0; i < count; i++) free(results[i]);
    free(results);

    trie_destroy(t);
    return 0;
}
```

### 1.11 Union-Find

```c
#include "data_structures/union_find.h"

int main(void) {
    UnionFind *uf = union_find_create(10);

    union_find_union(uf, 0, 1);
    union_find_union(uf, 2, 3);
    union_find_union(uf, 1, 3);  // agora {0,1,2,3} estão conectados

    bool conn = union_find_connected(uf, 0, 3);  // true
    bool sep  = union_find_connected(uf, 0, 5);  // false

    size_t sets = union_find_count(uf);  // 7 conjuntos restantes

    union_find_destroy(uf);
    return 0;
}
```

---

## 2. Algoritmos

### 2.1 Sorting

```c
#include "algorithms/sorting.h"
#include "data_structures/common.h"
#include <stdio.h>

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    // Quick Sort (genérico, qualquer tipo)
    quick_sort(arr, n, sizeof(int), compare_int);
    // arr = [11, 12, 22, 25, 34, 64, 90]

    // Verificar se está ordenado
    bool sorted = is_sorted(arr, n, sizeof(int), compare_int);  // true

    // Counting Sort (inteiros, O(n+k))
    int arr2[] = {4, 2, 2, 8, 3, 3, 1};
    counting_sort(arr2, 7, 8);
    // arr2 = [1, 2, 2, 3, 3, 4, 8]

    // Bucket Sort (doubles em [0, 1))
    double arr3[] = {0.42, 0.32, 0.23, 0.52, 0.25, 0.47, 0.51};
    bucket_sort(arr3, 7);

    return 0;
}
```

### 2.2 Searching

```c
#include "algorithms/searching.h"
#include "data_structures/common.h"

int main(void) {
    int sorted_arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    size_t n = 10;

    int target = 23;
    size_t idx;

    // Binary Search - O(log n)
    idx = binary_search(sorted_arr, n, sizeof(int), &target, compare_int);
    // idx = 5

    // Jump Search - O(√n)
    idx = jump_search(sorted_arr, n, sizeof(int), &target, compare_int);
    // idx = 5

    // Linear Search (não requer array ordenado)
    int unsorted[] = {42, 7, 15, 99, 23};
    idx = linear_search(unsorted, 5, sizeof(int), &target, compare_int);
    // idx = 4

    // Não encontrado
    int missing = 100;
    idx = binary_search(sorted_arr, n, sizeof(int), &missing, compare_int);
    // idx == SEARCH_NOT_FOUND

    return 0;
}
```

### 2.3 Graph Algorithms

```c
#include "data_structures/graph.h"
#include "algorithms/graph_algorithms.h"
#include <stdio.h>

int main(void) {
    Graph *g = graph_create(5, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);

    graph_add_edge(g, 0, 1, 10.0);
    graph_add_edge(g, 0, 3, 5.0);
    graph_add_edge(g, 1, 2, 1.0);
    graph_add_edge(g, 1, 3, 2.0);
    graph_add_edge(g, 2, 4, 4.0);
    graph_add_edge(g, 3, 1, 3.0);
    graph_add_edge(g, 3, 2, 9.0);
    graph_add_edge(g, 3, 4, 2.0);

    // Dijkstra - Caminho mais curto a partir do vértice 0
    ShortestPathResult *sp = dijkstra(g, 0);
    // sp->dist[4] = 7.0 (caminho: 0→3→4, custo 5+2)
    printf("Dist 0->4: %.1f\n", sp->dist[4]);
    shortest_path_free(sp);

    // Floyd-Warshall - Todos os pares
    AllPairsResult *ap = floyd_warshall(g);
    printf("Dist 1->4: %.1f\n", ap->dist[1][4]);
    all_pairs_free(ap);

    // MST (para grafos não-direcionados)
    Graph *ug = graph_create(4, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);
    graph_add_edge(ug, 0, 1, 1.0);
    graph_add_edge(ug, 0, 2, 4.0);
    graph_add_edge(ug, 1, 2, 2.0);
    graph_add_edge(ug, 2, 3, 3.0);

    MSTResult *mst = kruskal(ug);
    printf("MST weight: %.1f\n", mst->total_weight);  // 6.0
    mst_free(mst);

    graph_destroy(g);
    graph_destroy(ug);
    return 0;
}
```

### 2.4 String Matching

```c
#include "algorithms/string_matching.h"
#include <stdio.h>

int main(void) {
    const char *text = "AABAACAADAABAABA";
    const char *pattern = "AABA";

    // KMP - O(n + m)
    size_t first = kmp_search(text, pattern);
    printf("Primeira ocorrência: %zu\n", first);  // 0

    // Todas as ocorrências com KMP
    MatchResult all = kmp_search_all(text, pattern);
    printf("Total: %zu ocorrências\n", all.count);  // 3
    for (size_t i = 0; i < all.count; i++) {
        printf("  posição %zu\n", all.positions[i]);
    }
    match_result_destroy(&all);

    // Boyer-Moore - O(n/m) best case
    first = boyer_moore_search(text, pattern);

    // Rabin-Karp - bom para múltiplos padrões
    first = rabin_karp_search(text, pattern);

    return 0;
}
```

### 2.5 Dynamic Programming

```c
#include "algorithms/dynamic_programming.h"
#include <stdio.h>

int main(void) {
    // Fibonacci
    long long fib = dp_fibonacci_tab(50);  // 12586269025

    // LCS
    LCSResult lcs = dp_lcs("ABCBDAB", "BDCAB");
    printf("LCS length: %zu, sequence: %s\n", lcs.length, lcs.sequence);
    dp_lcs_result_destroy(&lcs);

    // Knapsack 0/1
    int weights[] = {2, 3, 4, 5};
    int values[]  = {3, 4, 5, 6};
    KnapsackResult ks = dp_knapsack(weights, values, 4, 8);
    printf("Max value: %d\n", ks.max_value);
    dp_knapsack_result_destroy(&ks);

    // Edit Distance
    size_t dist = dp_edit_distance("kitten", "sitting");
    printf("Edit distance: %zu\n", dist);  // 3

    // Coin Change
    int coins[] = {1, 5, 10, 25};
    CoinChangeResult cc = dp_coin_change(coins, 4, 36);
    printf("Min coins for 36: %d\n", cc.min_coins);
    dp_coin_change_result_destroy(&cc);

    return 0;
}
```

### 2.6 Greedy Algorithms

```c
#include "algorithms/greedy.h"
#include <stdio.h>

int main(void) {
    // Activity Selection
    int start[]  = {1, 3, 0, 5, 8, 5};
    int finish[] = {2, 4, 6, 7, 9, 9};
    ActivityResult act = greedy_activity_selection(start, finish, 6);
    printf("Selected %zu activities\n", act.count);
    activity_result_destroy(&act);

    // Huffman Coding
    char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int freqs[]  = {5, 9, 12, 13, 16, 45};
    HuffmanResult huff = greedy_huffman(chars, freqs, 6);
    for (int i = 0; i < 256; i++) {
        if (huff.codes[i] != NULL) {
            printf("'%c': %s\n", (char)i, huff.codes[i]);
        }
    }
    huffman_result_destroy(&huff);

    // Fractional Knapsack
    double w[] = {10, 20, 30};
    double v[] = {60, 100, 120};
    double max_val = greedy_fractional_knapsack_value(w, v, 3, 50.0);
    printf("Max value: %.1f\n", max_val);  // 240.0

    return 0;
}
```

### 2.7 Numerical Algorithms

```c
#include "algorithms/numerical.h"
#include <stdio.h>

int main(void) {
    // GCD e LCM
    long long g = gcd(48, 18);   // 6
    long long l = lcm(48, 18);   // 144

    // Extended GCD: ax + by = gcd(a,b)
    ExtendedGCDResult ext = extended_gcd(35, 15);
    // ext.gcd = 5, ext.x = 1, ext.y = -2
    // 35*1 + 15*(-2) = 5

    // Fast Exponentiation
    long long result = fast_pow_mod(2, 30, 1000000007);  // 1073741824
    long long result2 = fast_pow(3, 20);  // 3486784401

    // Sieve of Eratosthenes
    SieveResult sieve = sieve_of_eratosthenes(100);
    printf("Primes up to 100: %zu\n", sieve.count);  // 25
    sieve_result_destroy(&sieve);

    // Primality test
    bool prime = is_prime(997);  // true

    return 0;
}
```

### 2.8 Divide and Conquer

```c
#include "algorithms/divide_conquer.h"
#include <stdio.h>

int main(void) {
    // Maximum Subarray (Kadane) - O(n)
    int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    MaxSubarrayResult msa = max_subarray_kadane(arr, 9);
    printf("Max sum: %lld [%zu..%zu]\n", msa.sum, msa.left, msa.right);
    // sum = 6, subarray = [4, -1, 2, 1]

    // Quick Select - k-ésimo menor elemento
    int arr2[] = {3, 2, 1, 5, 6, 4};
    bool found;
    int kth = quick_select(arr2, 6, 2, &found);  // 2nd smallest = 2

    // Karatsuba Multiplication
    long long product = karatsuba_multiply(12345, 67890);
    // product = 838102050

    // Closest Pair of Points
    Point2D points[] = {{0, 0}, {1, 1}, {3, 4}, {5, 1}, {1.1, 0.9}};
    ClosestPairResult cp = closest_pair(points, 5);
    printf("Closest distance: %.4f\n", cp.distance);

    return 0;
}
```

### 2.9 Backtracking

```c
#include "algorithms/backtracking.h"
#include <stdio.h>

int main(void) {
    // N-Queens
    size_t count = nqueens_count(8);
    printf("8-Queens solutions: %zu\n", count);  // 92

    NQueensResult nq = nqueens_solve(4);
    printf("4-Queens has %zu solutions\n", nq.count);  // 2
    nqueens_result_destroy(&nq);

    // Subset Sum
    int set[] = {3, 34, 4, 12, 5, 2};
    bool exists = subset_sum_exists(set, 6, 9);  // true (4 + 5)

    SubsetSumResult ss = subset_sum_all(set, 6, 9);
    printf("Found %zu subsets summing to 9\n", ss.count);
    subset_sum_result_destroy(&ss);

    // Permutations
    int perm_arr[] = {1, 2, 3};
    PermutationResult pr = permutations_generate(perm_arr, 3);
    printf("Permutations of [1,2,3]: %zu\n", pr.count);  // 6
    permutation_result_destroy(&pr);

    // Graph Coloring
    // Adjacency matrix (3 vertices, triangle)
    int adj[] = {0, 1, 1,
                 1, 0, 1,
                 1, 1, 0};
    GraphColoringResult gc = graph_coloring(adj, 3, 3);
    if (gc.solvable) {
        printf("Colors: %d %d %d\n", gc.colors[0], gc.colors[1], gc.colors[2]);
    }
    graph_coloring_result_destroy(&gc);

    return 0;
}
```

---

## 3. Heurísticas e Meta-Heurísticas (Otimização)

> Compilar exemplos de otimização:
> ```bash
> gcc -std=c11 -Wall -Wextra -I include -o example example.c \
>     src/optimization/common.c src/optimization/benchmarks/tsp.c \
>     src/optimization/benchmarks/continuous.c \
>     src/optimization/heuristics/hill_climbing.c \
>     src/optimization/metaheuristics/simulated_annealing.c \
>     src/optimization/metaheuristics/tabu_search.c \
>     src/optimization/metaheuristics/genetic_algorithm.c \
>     src/optimization/metaheuristics/ils.c \
>     src/optimization/metaheuristics/grasp.c \
>     src/optimization/metaheuristics/pso.c \
>     src/optimization/metaheuristics/aco.c \
>     src/optimization/metaheuristics/differential_evolution.c \
>     src/optimization/metaheuristics/vns.c \
>     src/optimization/metaheuristics/memetic.c \
>     src/optimization/metaheuristics/lns.c -lm
> ```

### 3.1 Hill Climbing - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/heuristics/hill_climbing.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    HCConfig config = hc_default_config();
    config.variant = HC_RANDOM_RESTART;
    config.max_iterations = 500;
    config.neighbors_per_iter = 20;
    config.num_restarts = 10;
    config.seed = 42;

    OptResult result = hc_run(&config,
                              sizeof(int),
                              tsp->n_cities,
                              tsp_tour_cost,
                              tsp_neighbor_2opt,
                              tsp_generate_random,
                              tsp);

    printf("Best tour cost: %.2f\n", result.best.cost);
    printf("Iterations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    tsp_print_tour((int*)result.best.data, tsp->n_cities, result.best.cost);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.2 Simulated Annealing - Continuous Function

```c
#include "optimization/common.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/simulated_annealing.h"
#include <stdio.h>

int main(void) {
    ContinuousInstance *rastrigin = continuous_create_rastrigin(10);

    SAConfig config = sa_default_config();
    config.initial_temp = 100.0;
    config.final_temp = 0.001;
    config.alpha = 0.95;
    config.cooling = SA_COOLING_GEOMETRIC;
    config.max_iterations = 10000;
    config.markov_chain_length = 50;
    config.seed = 42;

    OptResult result = sa_run(&config,
                              sizeof(double),
                              rastrigin->dimensions,
                              continuous_evaluate,
                              continuous_neighbor_gaussian,
                              continuous_generate_random,
                              rastrigin);

    printf("Rastrigin (D=10) best: %.6f (optimal=0.0)\n", result.best.cost);
    printf("Time: %.1f ms\n", result.elapsed_time_ms);

    opt_result_destroy(&result);
    continuous_instance_destroy(rastrigin);
    return 0;
}
```

### 3.3 Tabu Search - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/tabu_search.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_20();

    TSConfig config = ts_default_config();
    config.max_iterations = 2000;
    config.neighbors_per_iter = 30;
    config.tabu_tenure = 15;
    config.enable_aspiration = true;
    config.enable_diversification = true;
    config.diversification_weight = 0.5;
    config.diversification_trigger = 100;
    config.seed = 42;

    OptResult result = ts_run(&config,
                              sizeof(int),
                              tsp->n_cities,
                              tsp_tour_cost,
                              tsp_neighbor_swap,
                              tsp_generate_random,
                              ts_hash_int_array,
                              tsp);

    printf("Best tour cost: %.2f\n", result.best.cost);
    printf("Iterations: %zu\n", result.num_iterations);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.4 Genetic Algorithm - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/genetic_algorithm.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    GAConfig config = ga_default_config();
    config.population_size = 50;
    config.max_generations = 200;
    config.crossover_rate = 0.8;
    config.mutation_rate = 0.05;
    config.elitism_count = 2;
    config.selection = GA_SELECT_TOURNAMENT;
    config.tournament_size = 3;
    config.seed = 42;

    OptResult result = ga_run(&config,
                              sizeof(int),
                              tsp->n_cities,
                              tsp_tour_cost,
                              tsp_generate_random,
                              ga_crossover_ox,
                              ga_mutation_swap,
                              NULL,
                              tsp);

    printf("GA best tour cost: %.2f\n", result.best.cost);
    printf("Generations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.5 Genetic Algorithm - Continuous (BLX Crossover)

```c
#include "optimization/common.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/genetic_algorithm.h"
#include <stdio.h>

int main(void) {
    ContinuousInstance *sphere = continuous_create_sphere(5);

    GAConfig config = ga_default_config();
    config.population_size = 100;
    config.max_generations = 500;
    config.crossover_rate = 0.9;
    config.mutation_rate = 0.1;
    config.elitism_count = 2;
    config.selection = GA_SELECT_RANK;
    config.enable_adaptive_rates = true;
    config.seed = 42;

    OptResult result = ga_run(&config,
                              sizeof(double),
                              sphere->dimensions,
                              continuous_evaluate,
                              continuous_generate_random,
                              ga_crossover_blx,
                              ga_mutation_gaussian,
                              NULL,
                              sphere);

    printf("Sphere (D=5) best: %.6f (optimal=0.0)\n", result.best.cost);

    double *x = (double*)result.best.data;
    printf("Solution: [");
    for (size_t i = 0; i < sphere->dimensions; i++) {
        printf("%.4f%s", x[i], i < sphere->dimensions - 1 ? ", " : "");
    }
    printf("]\n");

    opt_result_destroy(&result);
    continuous_instance_destroy(sphere);
    return 0;
}
```

### 3.6 ILS - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/ils.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    ILSConfig config = ils_default_config();
    config.max_iterations = 100;
    config.local_search_iterations = 200;
    config.local_search_neighbors = 20;
    config.perturbation_strength = 3;
    config.acceptance = ILS_ACCEPT_SA_LIKE;
    config.sa_initial_temp = 50.0;
    config.sa_alpha = 0.95;
    config.seed = 42;

    OptResult result = ils_run(&config,
                               sizeof(int),
                               tsp->n_cities,
                               tsp_tour_cost,
                               tsp_neighbor_2opt,
                               tsp_perturb_double_bridge,
                               tsp_generate_random,
                               tsp);

    printf("ILS best tour cost: %.2f\n", result.best.cost);
    printf("Iterations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.7 GRASP - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/grasp.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    GRASPConfig config = grasp_default_config();
    config.max_iterations = 50;
    config.alpha = 0.3;
    config.local_search_iterations = 200;
    config.local_search_neighbors = 20;
    config.enable_reactive = true;
    config.reactive_num_alphas = 5;
    config.reactive_block_size = 10;
    config.seed = 42;

    OptResult result = grasp_run(&config,
                                 sizeof(int),
                                 tsp->n_cities,
                                 tsp_tour_cost,
                                 grasp_construct_tsp_nn,
                                 tsp_neighbor_2opt,
                                 tsp);

    printf("GRASP best tour cost: %.2f\n", result.best.cost);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.8 PSO - Continuous Function

```c
#include "optimization/common.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/pso.h"
#include <stdio.h>

int main(void) {
    ContinuousInstance *sphere = continuous_create_sphere(10);

    PSOConfig config = pso_default_config();
    config.num_particles = 30;
    config.max_iterations = 500;
    config.w = 0.9;
    config.w_min = 0.4;
    config.c1 = 2.0;
    config.c2 = 2.0;
    config.v_max_ratio = 0.1;
    config.inertia_type = PSO_INERTIA_LINEAR_DECREASING;
    config.lower_bound = -5.12;
    config.upper_bound = 5.12;
    config.seed = 42;

    OptResult result = pso_run(&config,
                               sphere->dimensions,
                               continuous_evaluate,
                               sphere);

    printf("PSO Sphere (D=10) best: %.6f (optimal=0.0)\n", result.best.cost);

    double *x = (double*)result.best.data;
    printf("Solution: [");
    for (size_t i = 0; i < sphere->dimensions; i++) {
        printf("%.4f%s", x[i], i < sphere->dimensions - 1 ? ", " : "");
    }
    printf("]\n");

    opt_result_destroy(&result);
    continuous_instance_destroy(sphere);
    return 0;
}
```

### 3.9 ACO - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/aco.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    ACOConfig config = aco_default_config();
    config.n_ants = 20;
    config.max_iterations = 200;
    config.alpha = 1.0;
    config.beta = 3.0;
    config.rho = 0.1;
    config.variant = ACO_ELITIST;
    config.elitist_weight = 5.0;
    config.seed = 42;

    OptResult result = aco_run(&config,
                               tsp->n_cities,
                               tsp_tour_cost,
                               aco_heuristic_tsp,
                               tsp);

    printf("ACO best tour cost: %.2f\n", result.best.cost);
    printf("Iterations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    tsp_print_tour((int*)result.best.data, tsp->n_cities, result.best.cost);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.10 Differential Evolution - Continuous Function

```c
#include "optimization/common.h"
#include "optimization/benchmarks/continuous.h"
#include "optimization/metaheuristics/differential_evolution.h"
#include <stdio.h>

int main(void) {
    ContinuousInstance *sphere = continuous_create_sphere(10);

    DEConfig config = de_default_config();
    config.population_size = 50;
    config.max_generations = 1000;
    config.F = 0.8;
    config.CR = 0.9;
    config.strategy = DE_RAND_1;
    config.lower_bound = -5.12;
    config.upper_bound = 5.12;
    config.seed = 42;

    OptResult result = de_run(&config, sphere->dimensions,
                              continuous_evaluate, sphere);

    printf("DE Sphere (D=10) best: %.6f (optimal=0.0)\n", result.best.cost);

    double *x = (double*)result.best.data;
    printf("Solution: [");
    for (size_t i = 0; i < sphere->dimensions; i++) {
        printf("%.4f%s", x[i], i < sphere->dimensions - 1 ? ", " : "");
    }
    printf("]\n");

    opt_result_destroy(&result);
    continuous_instance_destroy(sphere);
    return 0;
}
```

### 3.11 VNS - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/vns.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    VNSConfig config = vns_default_config();
    config.max_iterations = 50;
    config.k_max = 5;
    config.local_search_iterations = 100;
    config.local_search_neighbors = 15;
    config.variant = VNS_BASIC;
    config.seed = 42;

    OptResult result = vns_run(&config,
                               sizeof(int),
                               tsp->n_cities,
                               tsp_tour_cost,
                               vns_shake_tsp,
                               tsp_neighbor_2opt,
                               tsp_generate_random,
                               tsp);

    printf("VNS best tour cost: %.2f\n", result.best.cost);
    printf("Iterations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    tsp_print_tour((int*)result.best.data, tsp->n_cities, result.best.cost);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.12 Memetic Algorithm - TSP (Lamarckian)

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/memetic.h"
#include "optimization/metaheuristics/genetic_algorithm.h"
#include <stdio.h>

static void ma_crossover_ox(const void *p1, const void *p2,
                            void *c1, void *c2,
                            size_t size, const void *context) {
    ga_crossover_ox(p1, p2, c1, c2, size, context);
}

static void ma_mutation_swap(void *solution, size_t size,
                             double rate, const void *context) {
    ga_mutation_swap(solution, size, rate, context);
}

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    MAConfig config = ma_default_config();
    config.population_size = 30;
    config.max_generations = 50;
    config.crossover_rate = 0.8;
    config.mutation_rate = 0.05;
    config.elitism_count = 2;
    config.selection = MA_SELECT_TOURNAMENT;
    config.tournament_size = 3;
    config.learning = MA_LAMARCKIAN;
    config.ls_iterations = 50;
    config.ls_neighbors = 10;
    config.ls_probability = 1.0;
    config.seed = 42;

    OptResult result = ma_run(&config,
                              sizeof(int),
                              tsp->n_cities,
                              tsp_tour_cost,
                              tsp_generate_random,
                              ma_crossover_ox,
                              ma_mutation_swap,
                              tsp_neighbor_swap,
                              tsp);

    printf("MA best tour cost: %.2f\n", result.best.cost);
    printf("Generations: %zu, Evaluations: %zu\n",
           result.num_iterations, result.num_evaluations);

    opt_result_destroy(&result);
    tsp_instance_destroy(tsp);
    return 0;
}
```

### 3.13 LNS / ALNS - TSP

```c
#include "optimization/common.h"
#include "optimization/benchmarks/tsp.h"
#include "optimization/metaheuristics/lns.h"
#include <stdio.h>

int main(void) {
    TSPInstance *tsp = tsp_create_example_10();

    // --- LNS Basico ---
    LNSConfig config = lns_default_config();
    config.max_iterations = 500;
    config.destroy_degree = 0.3;
    config.acceptance = LNS_ACCEPT_BETTER;
    config.seed = 42;

    OptResult result = lns_run(&config,
                               sizeof(int),
                               tsp->n_cities,
                               tsp_tour_cost,
                               tsp_generate_random,
                               lns_destroy_tsp_random,
                               lns_repair_tsp_greedy,
                               tsp);

    printf("LNS best tour cost: %.2f\n", result.best.cost);
    opt_result_destroy(&result);

    // --- ALNS (Adaptive) ---
    config.variant = LNS_ADAPTIVE;
    config.acceptance = LNS_ACCEPT_SA_LIKE;
    config.sa_initial_temp = 50.0;
    config.sa_alpha = 0.995;
    config.num_destroy_ops = 2;
    config.num_repair_ops = 2;
    config.weight_update_interval = 50;

    DestroyFn destroys[] = { lns_destroy_tsp_random, lns_destroy_tsp_worst };
    RepairFn repairs[] = { lns_repair_tsp_greedy, lns_repair_tsp_random };

    OptResult result2 = alns_run(&config,
                                 sizeof(int),
                                 tsp->n_cities,
                                 tsp_tour_cost,
                                 tsp_generate_random,
                                 destroys, repairs,
                                 tsp);

    printf("ALNS best tour cost: %.2f\n", result2.best.cost);
    printf("Iterations: %zu, Evaluations: %zu\n",
           result2.num_iterations, result2.num_evaluations);

    opt_result_destroy(&result2);
    tsp_instance_destroy(tsp);
    return 0;
}
```

---

## 4. Padrões de Uso Comuns

### 4.1 Trabalhando com Strings

```c
// Strings usam char* → precisa copy_string/destroy_string
Queue *q = queue_create(sizeof(char*), QUEUE_LINKED, 0, destroy_string);

char *s = strdup("hello");
queue_enqueue(q, &s);   // queue faz cópia interna via memcpy do ponteiro

char *out;
queue_dequeue(q, &out);  // out contém o ponteiro
// destroy_string é chamado automaticamente no destroy da queue
```

### 4.2 Trabalhando com Structs Customizadas

```c
typedef struct {
    int id;
    char name[50];
    double score;
} Student;

int compare_student(const void *a, const void *b) {
    const Student *sa = (const Student *)a;
    const Student *sb = (const Student *)b;
    return compare_double(&sa->score, &sb->score);
}

// BST de estudantes ordenados por score
BST *tree = bst_create(sizeof(Student), compare_student, NULL);

Student s1 = {1, "Alice", 9.5};
Student s2 = {2, "Bob", 8.3};
bst_insert(tree, &s1);
bst_insert(tree, &s2);
```

### 4.3 Combinando Estruturas e Algoritmos

```c
// Ordenar ArrayList com merge_sort
ArrayList *arr = arraylist_create(sizeof(int), 100, NULL);
// ... popular com dados ...

// Obter ponteiro para dados internos e ordenar
void *data = arraylist_data(arr);
size_t n = arraylist_size(arr);
merge_sort(data, n, sizeof(int), compare_int);

// Usar graph + Dijkstra
Graph *g = graph_create(100, GRAPH_DIRECTED, GRAPH_ADJACENCY_LIST, true);
// ... adicionar arestas ...
ShortestPathResult *sp = dijkstra(g, 0);
printf("Shortest path to vertex 50: %.2f\n", sp->dist[50]);
shortest_path_free(sp);
```

---

*Última atualização: 2026-02-13 (Phase 3C adicionada)*
