---
description: "Roadmap completo do projeto: estruturas de dados (14), algoritmos fundamentais (~45), heurísticas (planejado). Progresso e métricas."
---

# Project Roadmap - Algoritmos e Heurísticas

## Visão Geral

**Objetivo**: Biblioteca acadêmica em C11 de estruturas de dados, algoritmos clássicos, heurísticas e meta-heurísticas com rigor científico.

**Status Atual**:
- Fase 1 (Estruturas de Dados): ✅ COMPLETA (14/14)
- Fase 2 (Algoritmos Fundamentais): ✅ COMPLETA (~45 algoritmos, 9 categorias)
- Fase 3 (Heurísticas e Meta-Heurísticas): 🔄 EM PROGRESSO (3A ✅, 3B ✅, 3C ⏳)

---

## Fase 1 - Estruturas de Dados ✅ COMPLETA

### Fase 1A - Estruturas Lineares ✅ (5/5)

| # | Estrutura | Arquivo | Referência | Status |
|---|-----------|---------|------------|--------|
| 1 | Common Utilities | common.c/h | Knuth TAOCP Vol 3 | ✅ |
| 2 | Queue | queue.c/h | Cormen Ch. 10.1 | ✅ |
| 3 | Stack | stack.c/h | Cormen Ch. 10.1 | ✅ |
| 4 | LinkedList | linked_list.c/h | Knuth Vol 1 Sec 2.2 | ✅ |
| 5 | ArrayList | array_list.c/h | Goodrich Ch. 7 | ✅ |

**Métricas**: 2.660 linhas, 132 testes | **Conclusão**: 2025-02-12

### Fase 1B - Associativas e Árvores ✅ (5/5)

| # | Estrutura | Arquivo | Referência | Status |
|---|-----------|---------|------------|--------|
| 6 | HashTable | hash_table.c/h | Cormen Ch. 11 | ✅ |
| 7 | BinaryTree | binary_tree.c/h | Cormen Ch. 12 | ✅ |
| 8 | BST | bst.c/h | Cormen Ch. 12 | ✅ |
| 9 | Heap | heap.c/h | Cormen Ch. 6 | ✅ |
| 10 | Graph | graph.c/h | Cormen Ch. 22 | ✅ |

**Métricas**: ~4.410 linhas, 117 testes | **Conclusão**: 2025-02-12

### Fase 1C - Balanceadas e Especializadas ✅ (4/4)

| # | Estrutura | Arquivo | Referência | Status |
|---|-----------|---------|------------|--------|
| 11 | AVL Tree | avl_tree.c/h | Adelson-Velsky & Landis (1962) | ✅ |
| 12 | Priority Queue | priority_queue.c/h | Cormen Ch. 6.5 | ✅ |
| 13 | Trie | trie.c/h | Fredkin (1960) | ✅ |
| 14 | Union-Find | union_find.c/h | Cormen Ch. 21 | ✅ |

**Métricas**: ~1.507 linhas, 59 testes | **Conclusão**: 2025-02-12

**Total Fase 1**: 14 estruturas, ~8.577 linhas, ~308 testes, 0 memory leaks

---

## Fase 2 - Algoritmos Fundamentais ✅ COMPLETA

### Wave 1 - Core ✅

| Categoria | Algoritmos | Testes | Referência |
|-----------|-----------|--------|------------|
| Sorting (10) | Bubble, Selection, Insertion, Shell, Merge, Quick, Heap, Counting, Radix, Bucket | 20 | Cormen Ch. 2, 6, 7, 8 |
| Searching (6) | Linear, Binary, Interpolation, Ternary, Jump, Exponential | 19 | Cormen Ch. 2; Knuth Vol 3 |
| Graph Algorithms (5) | Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim | 8 | Cormen Ch. 23-25 |

### Wave 2 - Classical ✅

| Categoria | Algoritmos | Testes | Referência |
|-----------|-----------|--------|------------|
| String Matching (4) | Naive, KMP, Rabin-Karp, Boyer-Moore | 32 | Cormen Ch. 32 |
| Dynamic Programming (8) | Fibonacci, LCS, Knapsack 0/1, Edit Distance, LIS, Rod Cutting, Matrix Chain, Coin Change | 38 | Cormen Ch. 15 |
| Greedy (3) | Activity Selection, Huffman Coding, Fractional Knapsack | 16 | Cormen Ch. 16 |
| Numerical (5) | GCD, Extended GCD, Fast Exp, Sieve, is_prime | 24 | Cormen Ch. 31 |

### Wave 3 - Advanced ✅

| Categoria | Algoritmos | Testes | Referência |
|-----------|-----------|--------|------------|
| Divide & Conquer (5) | Strassen, Closest Pair, Karatsuba, Max Subarray, Quick Select | 23 | Cormen Ch. 4 |
| Backtracking (4) | N-Queens, Subset Sum, Permutations, Graph Coloring | 19 | Cormen Ch. 34 |

**Total Fase 2**: ~45 algoritmos, ~6.000 linhas, 199 testes, 0 failures

---

## Fase 3 - Heurísticas e Meta-Heurísticas 🔄 EM PROGRESSO

### Phase 3A - Classical ✅ COMPLETA
| Algoritmo | Tipo | Referência | Testes | Status |
|-----------|------|------------|--------|--------|
| Hill Climbing | Local Search | Russell & Norvig (2010) | 16 | ✅ |
| Simulated Annealing | Single-Solution | Kirkpatrick et al. (1983) | 15 | ✅ |
| Tabu Search | Single-Solution | Glover (1986) | 15 | ✅ |
| Genetic Algorithm | Population-Based | Holland (1975) | 13 | ✅ |

**Infraestrutura**: OptSolution/OptResult types, RNG utilities, TSP benchmarks (5/10/20 cities), Continuous benchmarks (Sphere, Rastrigin, Rosenbrock, Ackley, Schwefel) — 32 testes

**Métricas**: ~4.500 linhas, 91 testes | **Conclusão**: 2026-02-13

### Phase 3B - Advanced ✅ COMPLETA
| Algoritmo | Tipo | Referência | Testes | Status |
|-----------|------|------------|--------|--------|
| ILS | Single-Solution | Lourenço et al. (2003) | 11 | ✅ |
| GRASP | Hybrid | Feo & Resende (1995) | 10 | ✅ |
| PSO | Population-Based | Kennedy & Eberhart (1995) | 10 | ✅ |
| ACO | Population-Based | Dorigo (1992) | 10 | ✅ |

**Métricas**: ~2.500 linhas, 41 testes | **Conclusão**: 2026-02-13

### Phase 3C - Specialized ⏳
| Algoritmo | Tipo | Referência | Status |
|-----------|------|------------|--------|
| Differential Evolution | Population-Based | Storn & Price (1997) | ⏳ |
| VNS | Single-Solution | Mladenović & Hansen (1997) | ⏳ |
| Memetic Algorithm | Hybrid | Moscato (1989) | ⏳ |
| LNS | Single-Solution | Shaw (1998) | ⏳ |

### Benchmark Problems
- TSP (Traveling Salesman Problem)
- VRP (Vehicle Routing Problem)
- Knapsack variants
- Job Shop / Flow Shop Scheduling
- Graph Coloring
- Continuous: Rastrigin, Rosenbrock, Ackley

---

## Progresso Geral

```
╔═══════════════════════════════════════════════════════════════╗
║                    PROGRESSO DO PROJETO                       ║
╠═══════════════════════════════════════════════════════════════╣
║ Fase 1 (Estruturas de Dados):    ✅ 100% (14/14)             ║
║   - 1A Lineares: 5/5 ✅                                      ║
║   - 1B Associativas/Árvores: 5/5 ✅                          ║
║   - 1C Balanceadas/Especializadas: 4/4 ✅                    ║
║                                                               ║
║ Fase 2 (Algoritmos Fundamentais): ✅ 100% (~45 algoritmos)   ║
║   - W1 Sorting/Searching/Graph: 21 algs ✅                   ║
║   - W2 String/DP/Greedy/Numerical: 20 algs ✅                ║
║   - W3 D&C/Backtracking: 9 algs ✅                           ║
║                                                               ║
║ Fase 3 (Heurísticas):             🔄 67% (8/12 + 2 bench)   ║
║   - 3A Classical: HC, SA, TS, GA ✅ (91 testes)             ║
║   - 3B Advanced: ILS, GRASP, PSO, ACO ✅ (41 testes)        ║
║   - 3C Specialized: DE, VNS, Memetic, LNS ⏳                ║
║                                                               ║
║ TOTAL: ~89% (Fases 1+2 completas, Fase 3A+3B completas)     ║
╚═══════════════════════════════════════════════════════════════╝
```

---

## Estatísticas do Projeto

```
Estruturas de Dados:      14 completas
Algoritmos:               ~45 completos
Heurísticas:              8 completas + 2 benchmarks (Phase 3A+3B)
Linhas de Código:         ~21.500+
Testes Unitários:         ~639 (100% passing)
Memory Leaks:             0
Categorias:               10 (sorting, searching, graph, string, DP, greedy, numerical, D&C, backtracking, optimization)
Referências Acadêmicas:   40+ livros e papers
Pseudocódigos:            35+ implementados do CLRS
```

---

## Documentação

| Documento | Descrição |
|-----------|-----------|
| `ALGORITHM_CATALOG.md` | Catálogo completo com status de implementação |
| `USAGE_EXAMPLES.md` | Exemplos de uso da API |
| `DATA_STRUCTURE_GUIDE.md` | Guia de escolha de estruturas |
| `IMPLEMENTATION_PLAN.md` | Plano detalhado por fase |
| `references/ESSENTIAL_DATA_STRUCTURES.md` | Checklist acadêmica |
| `references/HEURISTICS_AND_METAHEURISTICS.md` | Pesquisa para Fase 3 |

---

**Última Atualização**: 2026-02-13  
**Próximo Marco**: Fase 3C - Specialized (DE, VNS, Memetic, LNS)
