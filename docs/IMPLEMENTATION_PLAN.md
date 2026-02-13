---
description: "Plano detalhado de implementação: estruturas de dados (3 fases), algoritmos fundamentais (3 waves), heurísticas (3 phases). Status completo."
---

# Plano de Implementação

## Overview

Plano detalhado para implementação da biblioteca acadêmica em C11. Organizado em 3 macro-fases:
1. **Fase 1**: Estruturas de Dados (14 estruturas) - ✅ COMPLETA
2. **Fase 2**: Algoritmos Fundamentais (~45 algoritmos) - ✅ COMPLETA
3. **Fase 3**: Heurísticas e Meta-Heurísticas (12+ algoritmos) - 🔄 EM PROGRESSO (3A ✅, 3B ✅)

---

## Fase 1: Estruturas de Dados ✅ COMPLETA

### 1A - Estruturas Lineares ✅

- [x] Implementar `common.c` (comparação, cópia, hash, impressão)
- [x] Configurar framework de testes (`test_macros.h`)
- [x] Implementar Queue (array circular + linked)
- [x] Implementar Stack (array dinâmico + linked)
- [x] Implementar LinkedList (singly, doubly, circular)
- [x] Implementar ArrayList (growth strategies: double, 1.5x, fixed)
- [x] Testes unitários completos (132 testes)

### 1B - Associativas e Árvores ✅

- [x] Implementar HashTable (chaining, linear/quadratic probing, double hashing)
- [x] Implementar BinaryTree (travessias, propriedades, LCA, diameter)
- [x] Implementar BST (insert, search, remove, range operations)
- [x] Implementar Heap (min/max, Floyd's build-heap, heapsort)
- [x] Implementar Graph (adjacency list + matrix, BFS, DFS, topological sort, SCC)
- [x] Testes unitários completos (117 testes)

### 1C - Balanceadas e Especializadas ✅

- [x] Implementar AVL Tree (rotações LL/RR/LR/RL, auto-balanceamento)
- [x] Implementar Priority Queue (wrapper sobre Heap)
- [x] Implementar Trie (insert, search, autocomplete, longest prefix)
- [x] Implementar Union-Find (path compression + union by rank)
- [x] Testes unitários completos (59 testes)

---

## Fase 2: Algoritmos Fundamentais ✅ COMPLETA

### Wave 1 - Core ✅

- [x] Sorting: Bubble, Selection, Insertion, Shell, Merge, Quick, Heap, Counting, Radix, Bucket
- [x] Searching: Linear, Binary, Interpolation, Ternary, Jump, Exponential
- [x] Graph Algorithms: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim
- [x] Testes (47 testes)

### Wave 2 - Classical ✅

- [x] String Matching: Naive, KMP, Rabin-Karp, Boyer-Moore
- [x] Dynamic Programming: Fibonacci, LCS, Knapsack 0/1, Edit Distance, LIS, Rod Cutting, Matrix Chain, Coin Change
- [x] Greedy: Activity Selection, Huffman Coding, Fractional Knapsack
- [x] Numerical: GCD, Extended GCD, Fast Exponentiation, Sieve, is_prime
- [x] Testes (110 testes)

### Wave 3 - Advanced ✅

- [x] Divide & Conquer: Strassen, Closest Pair, Karatsuba, Max Subarray (Kadane), Quick Select
- [x] Backtracking: N-Queens, Subset Sum, Permutations, Graph Coloring
- [x] Testes (42 testes)

---

## Fase 3: Heurísticas e Meta-Heurísticas 🔄 EM PROGRESSO

### Phase 3A - Classical ✅ COMPLETA (91 testes)

- [x] Hill Climbing (steepest ascent, first improvement, random restart, stochastic) — 16 testes
- [x] Simulated Annealing (geometric/linear/log/adaptive cooling, reheating, auto-calibrate T0) — 15 testes
- [x] Tabu Search (tabu list, aspiration, diversification, intensification, reactive tenure) — 15 testes
- [x] Genetic Algorithm (tournament/roulette/rank, OX/PMX/BLX, swap/inversion/gaussian, adaptive) — 13 testes
- [x] Benchmarks: TSP (5/10/20 cities), Continuous (Sphere, Rastrigin, Rosenbrock, Ackley, Schwefel) — 32 testes

### Phase 3B - Advanced ✅ COMPLETA (41 testes)

- [x] ILS (better/always/SA-like/restart acceptance, optional perturbation) — 11 testes
- [x] GRASP (RCL construction, reactive GRASP, builtin TSP/continuous constructors) — 10 testes
- [x] PSO (constant/linear decreasing/constriction inertia, velocity clamping) — 10 testes
- [x] ACO (Ant System/Elitist/MAX-MIN, pheromone evaporation, builtin TSP heuristic) — 10 testes

### Phase 3C - Specialized ⏳

- [ ] Differential Evolution
- [ ] VNS (Variable Neighborhood Search)
- [ ] Memetic Algorithm (GA + Local Search)
- [ ] LNS (Large Neighborhood Search)
- [ ] Continuous benchmarks: Rastrigin, Rosenbrock, Ackley

---

## Critérios de Validação

Para cada componente implementado:

### Correção
- [x] Todos os testes unitários passam
- [x] Sem memory leaks (ASan)
- [x] Sem undefined behavior (UBSan)

### Qualidade de Código
- [x] Compilação sem warnings (-Wall -Wextra)
- [x] Documentação Doxygen completa
- [x] Pseudocódigo acadêmico seguido

### Validação Acadêmica
- [x] Referências bibliográficas citadas
- [x] Análise de complexidade documentada
- [x] Algoritmos clássicos implementados corretamente

---

**Última atualização**: 2026-02-13
