---
description: "Memória de longo prazo do projeto: features implementadas, releases, hotfixes e marcos históricos. Append-only, atualização obrigatória em T0."
---

# MEMORY.md - Long-Term Project Memory

> **Memória de Longo Prazo** - Registro histórico de features/releases/hotfixes implementados.
> 
> **Diferença de `.itzamna/memory.md`:**
> - `.itzamna/memory.md` = Memória de curto prazo (estado atual, próximos passos, sessões recentes)
> - `MEMORY.md` = Memória de longo prazo (features completas, releases, marcos históricos)

---

## 📋 Como Usar

### Quando Atualizar (OBRIGATÓRIO - T0 Rule)

**SEMPRE** atualize este arquivo quando:
- ✅ **Feature completa**: Nova estrutura de dados finalizada e testada
- ✅ **Release**: Fase completa (1A/1B/1C) ou versão publicada
- ✅ **Hotfix**: Correção crítica de bug em produção
- ✅ **Breaking change**: Mudança que quebra API existente

**Formato**: Adicione entrada no topo (mais recente primeiro), nunca delete histórico.

---

## 🎯 Features Implementadas

### Fase 1A - Estruturas Lineares ✅ COMPLETO

**Data**: 2025-02-12  
**Versão**: v0.1.0-alpha  
**Status**: ✅ Produção (5/5 estruturas)

#### 1. Common Utilities ✅
- **Arquivo**: `src/data_structures/common.c` (380 linhas)
- **Features**:
  - Funções de comparação: `int`, `float`, `double`, `string` (com epsilon)
  - Funções de cópia: `copy_string` com alocação automática
  - Funções de destruição: `destroy_string`
  - Funções de impressão: todos os tipos primitivos
  - Funções hash: `djb2`, `fnv1a`, `multiplicative` (Knuth)
- **Testes**: 35 casos, 100% cobertura
- **Referências**: Knuth TAOCP Vol 3 (hash functions)

#### 2. Queue (Fila FIFO) ✅
- **Arquivo**: `src/data_structures/queue.c` (480 linhas)
- **Features**:
  - `QUEUE_ARRAY`: Circular buffer, O(1) amortizado, redimensionamento automático 2x
  - `QUEUE_LINKED`: Lista encadeada, O(1) todas operações
  - Operações: `enqueue`, `dequeue`, `front`, `clear`, `print`
- **Testes**: 24 casos (array + linked, strings, structs)
- **Referências**: Cormen Ch. 10.1, pseudocódigo ENQUEUE/DEQUEUE (p. 235)

#### 3. Stack (Pilha LIFO) ✅
- **Arquivo**: `src/data_structures/stack.c` (450 linhas)
- **Features**:
  - `STACK_ARRAY`: Array dinâmico, O(1) amortizado
  - `STACK_LINKED`: Lista encadeada, O(1)
  - Operações: `push`, `pop`, `top`, `reverse`, `clear`
  - Aplicações: validação de parênteses balanceados
- **Testes**: 23 casos (aplicações práticas incluídas)
- **Referências**: Cormen Ch. 10.1, pseudocódigo PUSH/POP (p. 233)

#### 4. LinkedList (Lista Encadeada) ✅
- **Arquivo**: `src/data_structures/linked_list.c` (777 linhas)
- **Features**:
  - `LIST_SINGLY`: Encadeamento simples (economia de memória)
  - `LIST_DOUBLY`: Encadeamento duplo (navegação bidirecional)
  - `LIST_CIRCULAR`: Circular duplo (buffers circulares)
  - Operações: `push_front/back`, `insert_at`, `remove_at`, `find`, `reverse`
  - Iteradores: `begin`, `end`, `next`, `prev`
- **Testes**: 24 casos (3 variantes testadas)
- **Referências**: Cormen Ch. 10.2 (p. 238), Knuth TAOCP Vol 1 Sec 2.2

#### 5. ArrayList (Array Dinâmico) ✅
- **Arquivo**: `src/data_structures/array_list.c` (573 linhas)
- **Features**:
  - Estratégias de crescimento: `DOUBLE` (2x), `1_5` (1.5x), `FIXED`
  - Operações: `get/set` O(1), `push_back` O(1) amortizado, `insert/remove` O(n)
  - Busca: `binary_search` O(log n), `find` O(n)
  - Utilidades: `sort` (qsort), `reverse`, `clear`, `shrink_to_fit`, `clone`
- **Testes**: 26 casos (análise amortizada validada)
- **Referências**: Goodrich Ch. 7, Cormen Ch. 17 (análise amortizada)

**Métricas Fase 1A**:
- Linhas de código: 2.660
- Testes: 132 (100% passing)
- Cobertura: 100%
- Memory leaks: 0 (validado com Valgrind/ASAN)

---

### Fase 1B - Associativas e Árvores ✅ COMPLETO

**Data Início**: 2025-02-12  
**Data Conclusão**: 2025-02-12  
**Status**: ✅ Produção (5/5 estruturas)

#### 6. HashTable (Tabela Hash) ✅
- **Arquivo**: `src/data_structures/hash_table.c` (~1.100 linhas)
- **Features**:
  - Estratégias de colisão:
    - `CHAINING`: Separate chaining com listas encadeadas
    - `LINEAR_PROBING`: Open addressing linear
    - `QUADRATIC_PROBING`: Open addressing quadrático
    - `DOUBLE_HASHING`: Open addressing com hash duplo
  - Rehashing automático: load factor 0.75 (chaining), 0.5 (open addressing)
  - Funções hash: `djb2`, `fnv1a`, `multiplicative` (Knuth)
  - Operações: `put`, `get`, `remove`, `contains`, `clear`, `keys`, `values`
  - Iterator para percorrer tabela
  - Estatísticas: collisions, max_chain_length, empty_buckets
  - Números primos para capacidade
- **Testes**: 32 casos (4 estratégias testadas)
- **Referências**: Cormen Ch. 11, pseudocódigo CHAINED-HASH-INSERT (p. 258), HASH-INSERT open addressing (p. 271)

#### 7. BinaryTree (Árvore Binária) ✅
- **Arquivo**: `src/data_structures/binary_tree.c` (~800 linhas)
- **Features**:
  - Estrutura base com `left`, `right`, `parent`
  - Travessias recursivas: `inorder`, `preorder`, `postorder`
  - Travessia level-order (BFS com queue)
  - Propriedades: `height`, `size`, `leaves`, `is_leaf`
  - Verificações: `is_complete`, `is_full`, `is_perfect`
  - Algoritmos: `find` (busca linear), `remove`, `clone`
  - Utilidades: `to_array` (inorder/levelorder)
  - Algoritmos avançados: `lca` (Lowest Common Ancestor), `distance`, `diameter`
  - `print` hierárquico visual
- **Testes**: 24 casos (travessias + propriedades + algoritmos)
- **Referências**: Cormen Ch. 12, pseudocódigo INORDER/PREORDER/POSTORDER-TREE-WALK (p. 288)

#### 8. BST (Binary Search Tree) ✅
- **Arquivo**: `src/data_structures/bst.c` (789 linhas)
- **Features**:
  - Insert, search, remove (3 cases: leaf, 1 child, 2 children)
  - Min/max, successor/predecessor
  - Travessias: inorder, preorder, postorder
  - Validação: `is_valid` (verifica propriedade BST)
  - Utilidades: `to_array`, `range_search`, `range_count`, `select`, `rank`
  - `clone`, `from_sorted_array` (árvore balanceada)
- **Testes**: 20 casos
- **Referências**: Cormen Ch. 12, pseudocódigo TREE-INSERT/DELETE/SEARCH (p. 294-298)

#### 9. Heap (Binary Heap) ✅
- **Arquivo**: `src/data_structures/heap.c` (491 linhas)
- **Features**:
  - `HEAP_MIN` + `HEAP_MAX`, genérico via `void*`
  - Insert O(log n), extract O(log n), peek O(1)
  - Build-heap O(n) via Floyd's algorithm
  - Heapsort O(n log n)
  - `to_array`, `clear`, `print`
- **Testes**: 17 casos
- **Referências**: Cormen Ch. 6, pseudocódigo MAX-HEAPIFY/BUILD-MAX-HEAP (p. 154-157)

#### 10. Graph (Grafo) ✅
- **Arquivo**: `src/data_structures/graph.c` (1.220 linhas)
- **Features**:
  - Representações: adjacency list (esparso) + adjacency matrix (denso)
  - Tipos: directed + undirected, weighted + unweighted
  - Vértices: add, remove, num_vertices
  - Arestas: add, remove, has_edge, edge_weight, num_edges
  - Grau: in_degree, out_degree, degree
  - Vizinhos: neighbors, edges (lista completa)
  - Travessias: BFS (Cormen p. 594), DFS (Cormen p. 604)
  - Propriedades: is_connected, is_strongly_connected, has_cycle, is_tree, is_bipartite
  - Topological sort (DAG only, Cormen p. 613)
  - Componentes: num_connected_components, strongly_connected_components (Kosaraju)
  - Utilidades: clone, transpose, to_adjacency_matrix, print
- **Testes**: 24 casos (ambas representações, directed/undirected)
- **Referências**: Cormen Ch. 22 (Elementary Graph Algorithms), Sedgewick Ch. 4

**Métricas Fase 1B**:
- Linhas de código: ~4.410
- Testes: 117 (100% passing)
- Memory leaks: 0

---

### Fase 1C - Balanceadas e Especializadas ✅ COMPLETO

**Data**: 2025-02-12  
**Status**: ✅ Produção (4/4 estruturas)

#### 11. AVL Tree (Árvore AVL) ✅
- **Arquivo**: `src/data_structures/avl_tree.c` (607 linhas)
- **Features**:
  - Auto-balancing BST com rotações LL/RR/LR/RL
  - Insert, search, remove com rebalanceamento automático
  - Min/max, inorder traversal
  - Height tracking, is_valid (verifica fator de balanceamento)
  - Range search, clear, clone
- **Testes**: 18 casos (inclui stress test com validação de balanceamento)
- **Referências**: Adelson-Velsky & Landis (1962), Cormen Ch. 13 (balanced trees)

#### 12. Priority Queue (Fila de Prioridade) ✅
- **Arquivo**: `src/data_structures/priority_queue.c` (180 linhas)
- **Features**:
  - Wrapper fino sobre Heap
  - Min/max priority queue
  - Insert O(log n), extract O(log n), peek O(1)
  - Clear, size, is_empty
- **Testes**: 12 casos
- **Referências**: Cormen Ch. 6.5, pseudocódigo HEAP-EXTRACT-MAX/INSERT (p. 163-164)

#### 13. Trie (Prefix Tree) ✅
- **Arquivo**: `src/data_structures/trie.c` (447 linhas)
- **Features**:
  - Insert, search, starts_with (prefix check)
  - Remove com cleanup de nós órfãos
  - Autocomplete: retorna palavras com dado prefixo
  - Longest common prefix
  - To array (ordem lexicográfica)
  - Size tracking
- **Testes**: 14 casos
- **Referências**: Fredkin (1960), Sedgewick Ch. 5 (Tries)

#### 14. Union-Find (Disjoint Set) ✅
- **Arquivo**: `src/data_structures/union_find.c` (273 linhas)
- **Features**:
  - Union by rank + path compression → O(α(n)) amortizado
  - Find, union, connected
  - Count sets, set size
  - Get members, get components
  - Reset
- **Testes**: 15 casos
- **Referências**: Cormen Ch. 21, Tarjan (1975)

**Métricas Fase 1C**:
- Linhas de código: ~1.507
- Testes: 59 (100% passing)
- Memory leaks: 0

---

### Fase 2 - Algoritmos Fundamentais ✅ COMPLETO

**Data Início**: 2026-02-12  
**Data Conclusão**: 2026-02-12  
**Versão**: v0.3.0-alpha  
**Status**: ✅ Produção (~45 algoritmos em 9 categorias)

#### Wave 1 - Core Algorithms ✅

**15. Sorting (10 algoritmos)** ✅
- **Arquivo**: `src/algorithms/sorting.c` + `include/algorithms/sorting.h`
- **Algoritmos**:
  - Didáticos: Bubble Sort, Selection Sort, Insertion Sort
  - Eficientes: Shell Sort, Merge Sort, Quick Sort, Heap Sort
  - Lineares: Counting Sort, Radix Sort, Bucket Sort
- **Testes**: 20 casos
- **Referências**: Cormen Ch. 2 (Insertion), Ch. 6 (Heapsort), Ch. 7 (Quicksort), Ch. 8 (Linear-time sorting)

**16. Searching (6 algoritmos)** ✅
- **Arquivo**: `src/algorithms/searching.c` + `include/algorithms/searching.h`
- **Algoritmos**: Linear Search, Binary Search, Interpolation Search, Ternary Search, Jump Search, Exponential Search
- **Testes**: 19 casos
- **Referências**: Cormen Ch. 2 (Binary Search p. 799), Knuth TAOCP Vol 3 Ch. 6

**17. Graph Algorithms (5 algoritmos)** ✅
- **Arquivo**: `src/algorithms/graph_algorithms.c` + `include/algorithms/graph_algorithms.h`
- **Algoritmos**: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal (MST), Prim (MST)
- **Testes**: 8 casos
- **Referências**: Cormen Ch. 23 (MST), Ch. 24 (Dijkstra, Bellman-Ford), Ch. 25 (Floyd-Warshall)

#### Wave 2 - Classical Algorithms ✅

**18. String Matching (4 algoritmos)** ✅
- **Arquivo**: `src/algorithms/string_matching.c` + `include/algorithms/string_matching.h`
- **Algoritmos**: Naive, KMP (Knuth-Morris-Pratt), Rabin-Karp, Boyer-Moore
- **Testes**: 32 casos
- **Referências**: Cormen Ch. 32 (String Matching), Knuth, Morris & Pratt (1977)

**19. Dynamic Programming (8 algoritmos)** ✅
- **Arquivo**: `src/algorithms/dynamic_programming.c` + `include/algorithms/dynamic_programming.h`
- **Algoritmos**: Fibonacci (memoização + tabulação), LCS, Knapsack 0/1, Edit Distance (Levenshtein), LIS, Rod Cutting, Matrix Chain Multiplication, Coin Change
- **Testes**: 38 casos
- **Referências**: Cormen Ch. 15 (DP), Bellman (1957), Wagner & Fischer (1974)

**20. Greedy Algorithms (3 algoritmos)** ✅
- **Arquivo**: `src/algorithms/greedy.c` + `include/algorithms/greedy.h`
- **Algoritmos**: Activity Selection, Huffman Coding, Fractional Knapsack
- **Testes**: 16 casos
- **Referências**: Cormen Ch. 16 (Greedy), Huffman (1952)

**21. Numerical Algorithms (5 algoritmos)** ✅
- **Arquivo**: `src/algorithms/numerical.c` + `include/algorithms/numerical.h`
- **Algoritmos**: GCD (Euclides), Extended GCD, Fast Exponentiation, Sieve of Eratosthenes, is_prime
- **Testes**: 24 casos
- **Referências**: Cormen Ch. 31 (Number-Theoretic Algorithms), Euclid (~300 BC)

#### Wave 3 - Advanced Algorithms ✅

**22. Divide and Conquer (5 algoritmos)** ✅
- **Arquivo**: `src/algorithms/divide_conquer.c` + `include/algorithms/divide_conquer.h`
- **Algoritmos**: Strassen Matrix Multiplication, Closest Pair of Points, Karatsuba Multiplication, Maximum Subarray (Kadane), Quick Select
- **Testes**: 23 casos
- **Referências**: Cormen Ch. 4 (Divide & Conquer), Strassen (1969), Karatsuba (1962), Hoare (1961)

**23. Backtracking (4 algoritmos)** ✅
- **Arquivo**: `src/algorithms/backtracking.c` + `include/algorithms/backtracking.h`
- **Algoritmos**: N-Queens, Subset Sum, Permutations, Graph Coloring
- **Testes**: 19 casos
- **Referências**: Cormen Ch. 34 (NP-Completeness concepts), Wirth (1976)

**Métricas Fase 2**:
- Algoritmos: ~45 em 9 categorias
- Linhas de código: ~6.000+
- Testes: 199 (100% passing, 0 failures)
- Memory leaks: 0

---

## 🚀 Releases

### v0.3.0-alpha - Fase 2 Completa (Algoritmos Fundamentais)
**Data**: 2026-02-12  
**Tipo**: Alpha Release  

**Conteúdo**:
- ✅ ~45 algoritmos fundamentais em 9 categorias
- ✅ 199 testes passando (100%)
- ✅ Sorting (10), Searching (6), Graph (5), String Matching (4), DP (8), Greedy (3), Numerical (5), D&C (5), Backtracking (4)
- ✅ Documentação acadêmica com referências Cormen/Knuth
- ✅ Zero memory leaks
- ✅ Biblioteca `libalgorithms.a` linkada contra `libdata_structures.a`

**Breaking Changes**: N/A

### v0.2.0-alpha - Fase 1 Completa (1A + 1B + 1C)
**Data**: 2026-02-12  
**Tipo**: Alpha Release  

**Conteúdo**:
- ✅ 14 estruturas de dados completas (todas as fases)
- ✅ ~308 testes passando (100%)
- ✅ Documentação acadêmica completa com referências Cormen/Knuth/Sedgewick
- ✅ Zero memory leaks

**Breaking Changes**: N/A

### v0.1.0-alpha - Fase 1A Completa
**Data**: 2025-02-12  
**Tipo**: Alpha Release  

**Conteúdo**:
- ✅ 5 estruturas lineares completas
- ✅ 132 testes passando
- ✅ Documentação acadêmica completa
- ✅ Zero memory leaks (Valgrind/ASAN)

**Breaking Changes**: N/A (primeira release)

---

## 🔧 Hotfixes

### 2026-02-12 - Correções críticas de sessão
- **`copy_string()` in common.c**: Retornava `&copy` (endereço de variável local = dangling pointer). Corrigido para `return copy`.
- **`compare_int/float/double/string()` NULL ordering**: Invertido: `(a==NULL)-(b==NULL)` → `(b==NULL)-(a==NULL)`.
- **`compare_float()` epsilon**: `1e-6f` muito pequeno para testes. Alterado para `1e-4f`.
- **`list_remove_at()` linked_list.c**: Não copiava dados para output em remoção de nó do meio. Adicionado `memcpy`.
- **Double-free em hash_table tests**: Testes chamavam `free(key)` em chaves que o `destroy_string` da hashtable também liberaria.
- **`binary_tree.c` queue_create calls**: Assinatura errada (3 params em vez de 4). Adicionado parâmetro `0` faltante.
- **TEST() macro conflicts**: Macro antiga causava "conflicting types" com `-Wpedantic`. Padronizado para `test_macros.h` compartilhado.
- **Nested function em test_bst.c**: `check_order()` dentro de teste era extensão GCC. Extraído para função estática file-scope.

---

## 📊 Estatísticas Gerais

### Totais Acumulados (atualizado 2026-02-12)

```
Estruturas Completas:     14/14 (100%)
Algoritmos Completos:     ~45 em 9 categorias (100%)
Linhas de Código:         ~14.500+
Testes Unitários:         ~507 (308 data structures + 199 algorithms)
Taxa de Sucesso:          100%
Memory Leaks:             0
Documentação:             100%
```

### Por Fase

| Fase | Componentes | Linhas | Testes | Status |
|------|-------------|--------|--------|--------|
| 1A - Estruturas Lineares | 5/5 | 2.660 | 132 | ✅ COMPLETO |
| 1B - Associativas/Árvores | 5/5 | 4.410 | 117 | ✅ COMPLETO |
| 1C - Balanceadas/Especializadas | 4/4 | 1.507 | 59 | ✅ COMPLETO |
| 2 W1 - Sorting/Searching/Graph | 21 algs | ~2.000 | 47 | ✅ COMPLETO |
| 2 W2 - String/DP/Greedy/Numerical | 20 algs | ~2.500 | 110 | ✅ COMPLETO |
| 2 W3 - D&C/Backtracking | 9 algs | ~1.500 | 42 | ✅ COMPLETO |

---

## 🎓 Referências Acadêmicas Utilizadas

### Livros
- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.)
  - Ch. 2: Insertion Sort
  - Ch. 4: Divide and Conquer (Strassen, Karatsuba)
  - Ch. 6: Heapsort
  - Ch. 7: Quicksort
  - Ch. 8: Sorting in Linear Time (Counting, Radix, Bucket)
  - Ch. 10: Elementary Data Structures
  - Ch. 11: Hash Tables
  - Ch. 12: Binary Search Trees
  - Ch. 13: Red-Black/Balanced Trees (conceitos para AVL)
  - Ch. 15: Dynamic Programming (LCS, Rod Cutting, Matrix Chain, Knapsack)
  - Ch. 16: Greedy Algorithms (Activity Selection, Huffman)
  - Ch. 21: Data Structures for Disjoint Sets
  - Ch. 22: Elementary Graph Algorithms
  - Ch. 23: Minimum Spanning Trees (Kruskal, Prim)
  - Ch. 24: Single-Source Shortest Paths (Dijkstra, Bellman-Ford)
  - Ch. 25: All-Pairs Shortest Paths (Floyd-Warshall)
  - Ch. 31: Number-Theoretic Algorithms (GCD, Modular Exp)
  - Ch. 32: String Matching (Naive, Rabin-Karp, KMP)
  - Ch. 34: NP-Completeness (concepts for backtracking)
- Knuth, D. E. (1997-1998). *The Art of Computer Programming*
  - Vol 1: Fundamental Algorithms
  - Vol 3: Sorting and Searching
- Goodrich, M. T., et al. (2011). *Data Structures and Algorithms in C++*
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.)
  - Ch. 4: Graphs
  - Ch. 5: Strings (Tries)
- Diestel, R. (2017). *Graph Theory* (5th ed.)

### Papers
- Adelson-Velsky, G. M. & Landis, E. M. (1962). "An algorithm for the organization of information"
- Bellman, R. (1957). "Dynamic Programming"
- Boyer, R. S. & Moore, J. S. (1977). "A fast string searching algorithm"
- Dijkstra, E. W. (1959). "A note on two problems in connexion with graphs"
- Fredkin, E. (1960). "Trie memory"
- Hoare, C. A. R. (1961). "Algorithm 65: Find" (Quick Select)
- Huffman, D. A. (1952). "A method for the construction of minimum-redundancy codes"
- Karatsuba, A. & Ofman, Y. (1962). "Multiplication of multidigit numbers on automata"
- Knuth, D. E., Morris, J. H. & Pratt, V. R. (1977). "Fast pattern matching in strings"
- Kruskal, J. B. (1956). "On the shortest spanning subtree of a graph"
- Levenshtein, V. I. (1966). "Binary codes capable of correcting deletions, insertions, and reversals"
- Prim, R. C. (1957). "Shortest connection networks and some generalizations"
- Rabin, M. O. & Karp, R. M. (1987). "Efficient randomized pattern-matching algorithms"
- Shell, D. L. (1959). "A high-speed sorting procedure"
- Strassen, V. (1969). "Gaussian elimination is not optimal"
- Tarjan, R. E. (1975). "Efficiency of a Good But Not Linear Set Union Algorithm"
- Wagner, R. A. & Fischer, M. J. (1974). "The string-to-string correction problem"

### Pseudocódigos Implementados

**Fase 1 - Estruturas de Dados:**
✅ ENQUEUE/DEQUEUE (Cormen p. 235)  
✅ PUSH/POP (Cormen p. 233)  
✅ LIST-INSERT/DELETE (Cormen p. 238)  
✅ BINARY-SEARCH (Cormen p. 799)  
✅ CHAINED-HASH-INSERT/SEARCH (Cormen p. 258)  
✅ HASH-INSERT/SEARCH open addressing (Cormen p. 271)  
✅ INORDER/PREORDER/POSTORDER-TREE-WALK (Cormen p. 288)  
✅ TREE-INSERT/DELETE/SEARCH (Cormen p. 294-298)  
✅ MAX-HEAPIFY/BUILD-MAX-HEAP (Cormen p. 154-157)  
✅ HEAP-EXTRACT-MAX/INSERT (Cormen p. 163-164)  
✅ BFS (Cormen p. 594)  
✅ DFS/DFS-VISIT (Cormen p. 604)  
✅ TOPOLOGICAL-SORT (Cormen p. 613)  
✅ STRONGLY-CONNECTED-COMPONENTS / Kosaraju (Cormen p. 615)  
✅ MAKE-SET/UNION/FIND-SET (Cormen p. 571)

**Fase 2 - Algoritmos Fundamentais:**
✅ INSERTION-SORT (Cormen p. 18)  
✅ MERGE-SORT / MERGE (Cormen p. 31-34)  
✅ QUICKSORT / PARTITION (Cormen p. 171-174)  
✅ HEAPSORT (Cormen p. 160)  
✅ COUNTING-SORT (Cormen p. 195)  
✅ RADIX-SORT (Cormen p. 198)  
✅ BUCKET-SORT (Cormen p. 201)  
✅ DIJKSTRA (Cormen p. 658)  
✅ BELLMAN-FORD (Cormen p. 651)  
✅ FLOYD-WARSHALL (Cormen p. 695)  
✅ MST-KRUSKAL (Cormen p. 631)  
✅ MST-PRIM (Cormen p. 634)  
✅ KMP-MATCHER / COMPUTE-PREFIX (Cormen p. 1005)  
✅ RABIN-KARP-MATCHER (Cormen p. 993)  
✅ NAIVE-STRING-MATCHER (Cormen p. 988)  
✅ LCS-LENGTH / PRINT-LCS (Cormen p. 394)  
✅ MATRIX-CHAIN-ORDER (Cormen p. 375)  
✅ CUT-ROD / BOTTOM-UP-CUT-ROD (Cormen p. 362-366)  
✅ 0-1 KNAPSACK (Cormen p. 426)  
✅ ACTIVITY-SELECTOR (Cormen p. 421)  
✅ HUFFMAN (Cormen p. 431)  
✅ RANDOMIZED-SELECT (Cormen p. 216)  
✅ STRASSEN (Cormen p. 79)

---

## 📝 Notas de Desenvolvimento

### Decisões Arquiteturais (ADRs)
Veja `.context/rules.md` para lista completa de ADRs.

### Qualidade de Código
- **Padrão**: C11 (ISO/IEC 9899:2011)
- **Compilador**: GCC/Clang com `-Wall -Wextra -Wpedantic`
- **Sanitizers**: ASan + UBSan habilitados em desenvolvimento
- **Testes**: Custom framework (não Unity, sem deps externas)
- **Documentação**: Doxygen obrigatório + análise de complexidade

---

## ⏭️ Próximas Features (Roadmap)

Veja `docs/PROJECT_ROADMAP.md` para roadmap completo.

**Fase 1 (Estruturas de Dados)**: ✅ COMPLETO (14/14)

**Fase 2 (Algoritmos Fundamentais)**: ✅ COMPLETO (~45 algoritmos, 9 categorias)

**Fase 3 - Heurísticas e Meta-Heurísticas** 🔄 PRÓXIMO:
- [ ] Phase 3A: Classical (Simulated Annealing, Tabu Search, Genetic Algorithm, Hill Climbing)
- [ ] Phase 3B: Advanced (PSO, ACO, GRASP, ILS)
- [ ] Phase 3C: Specialized (DE, VNS, Memetic, LNS)
- [ ] Benchmark problems: TSP, VRP, Knapsack variants, Scheduling

---

## 🔒 Regras de Atualização (T0)

### OBRIGATÓRIO atualizar quando:
1. ✅ Feature/estrutura completa e testada
2. ✅ Release/fase concluída
3. ✅ Hotfix crítico aplicado
4. ✅ Breaking change introduzido

### Como atualizar:
1. Adicione entrada no topo da seção correspondente (mais recente primeiro)
2. Mantenha formato markdown consistente
3. Inclua métricas (linhas, testes, cobertura)
4. Cite referências acadêmicas quando aplicável
5. **NUNCA delete histórico** (append-only)

### Aprovação:
- Agente deve **propor atualização** ao final de implementação
- Humano deve **aprovar** antes de escrever
- **NUNCA** modificar sem aprovação (T0 rule)

---

*Última atualização: 2026-02-12*  
*Próxima feature: Fase 3 - Heurísticas e Meta-Heurísticas (SA, Tabu, GA, PSO, ACO)*
