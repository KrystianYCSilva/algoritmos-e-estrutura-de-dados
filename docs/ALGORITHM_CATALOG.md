---
description: "Catálogo completo de algoritmos do projeto com status de implementação, organizado por categoria com classificação Must-Have e Nice-to-Have."
---

# Algorithm Catalog - Algoritmos e Heurísticas

> Catálogo completo de algoritmos organizados por categoria, com status de implementação.
> ✅ = Implementado e testado | ⏳ = Planejado | 🔸 = Nice-to-Have

---

## 1. Sorting Algorithms

### Must-Have
| # | Algoritmo | Complexidade (Avg) | Complexidade (Worst) | Estável | In-Place | Status |
|---|-----------|-------------------|---------------------|---------|----------|--------|
| 1 | Bubble Sort | O(n²) | O(n²) | Sim | Sim | ✅ |
| 2 | Selection Sort | O(n²) | O(n²) | Não | Sim | ✅ |
| 3 | Insertion Sort | O(n²) | O(n²) | Sim | Sim | ✅ |
| 4 | Shell Sort | O(n log² n) | O(n²) | Não | Sim | ✅ |
| 5 | Merge Sort | O(n log n) | O(n log n) | Sim | Não | ✅ |
| 6 | Quick Sort | O(n log n) | O(n²) | Não | Sim | ✅ |
| 7 | Heap Sort | O(n log n) | O(n log n) | Não | Sim | ✅ |
| 8 | Counting Sort | O(n + k) | O(n + k) | Sim | Não | ✅ |
| 9 | Radix Sort | O(d(n + k)) | O(d(n + k)) | Sim | Não | ✅ |
| 10 | Bucket Sort | O(n + k) | O(n²) | Sim | Não | ✅ |

### Nice-to-Have
| # | Algoritmo | Complexidade | Status |
|---|-----------|-------------|--------|
| 11 | Tim Sort | O(n log n) | 🔸 |
| 12 | Introsort | O(n log n) | 🔸 |
| 13 | Comb Sort | O(n²/2^p) | 🔸 |
| 14 | Gnome Sort | O(n²) | 🔸 |
| 15 | Bitonic Sort | O(n log² n) | 🔸 |

**Referências**: Cormen Ch. 2, 6, 7, 8; Knuth TAOCP Vol 3

---

## 2. Searching Algorithms

### Must-Have
| # | Algoritmo | Complexidade | Requisitos | Status |
|---|-----------|-------------|------------|--------|
| 1 | Linear Search | O(n) | Nenhum | ✅ |
| 2 | Binary Search | O(log n) | Array ordenado | ✅ |
| 3 | Interpolation Search | O(log log n) avg | Distribuição uniforme | ✅ |
| 4 | Ternary Search | O(log₃ n) | Array ordenado | ✅ |
| 5 | Jump Search | O(√n) | Array ordenado | ✅ |
| 6 | Exponential Search | O(log n) | Array ordenado | ✅ |

### Nice-to-Have
| # | Algoritmo | Complexidade | Status |
|---|-----------|-------------|--------|
| 7 | Fibonacci Search | O(log n) | 🔸 |
| 8 | Sentinel Linear Search | O(n) | 🔸 |

**Referências**: Cormen Ch. 2 (p. 799); Knuth TAOCP Vol 3 Ch. 6

---

## 3. Graph Algorithms

### Must-Have
| # | Algoritmo | Categoria | Complexidade | Status |
|---|-----------|-----------|-------------|--------|
| 1 | Dijkstra | Shortest Path | O((V+E) log V) | ✅ |
| 2 | Bellman-Ford | Shortest Path | O(VE) | ✅ |
| 3 | Floyd-Warshall | All-Pairs SP | O(V³) | ✅ |
| 4 | Kruskal (MST) | Minimum Spanning Tree | O(E log E) | ✅ |
| 5 | Prim (MST) | Minimum Spanning Tree | O((V+E) log V) | ✅ |

### Nice-to-Have
| # | Algoritmo | Categoria | Complexidade | Status |
|---|-----------|-----------|-------------|--------|
| 6 | A* Search | Shortest Path | O(E) heuristic | 🔸 |
| 7 | Johnson's Algorithm | All-Pairs SP | O(V² log V + VE) | 🔸 |
| 8 | Edmonds-Karp | Max Flow | O(VE²) | 🔸 |
| 9 | Ford-Fulkerson | Max Flow | O(Ef) | 🔸 |
| 10 | Tarjan (SCC) | Connectivity | O(V+E) | 🔸 |
| 11 | Articulation Points | Connectivity | O(V+E) | 🔸 |
| 12 | Bridges | Connectivity | O(V+E) | 🔸 |
| 13 | Euler Path/Circuit | Traversal | O(V+E) | 🔸 |
| 14 | Hamiltonian Path | Traversal | O(n!) | 🔸 |

**Nota**: BFS, DFS, Topological Sort e Kosaraju SCC estão na estrutura Graph (Phase 1B).

**Referências**: Cormen Ch. 22-26; Sedgewick Ch. 4

---

## 4. String Matching Algorithms

### Must-Have
| # | Algoritmo | Complexidade (Preprocessing) | Complexidade (Matching) | Status |
|---|-----------|------------------------------|------------------------|--------|
| 1 | Naive | - | O(nm) | ✅ |
| 2 | KMP (Knuth-Morris-Pratt) | O(m) | O(n) | ✅ |
| 3 | Rabin-Karp | O(m) | O(n) avg, O(nm) worst | ✅ |
| 4 | Boyer-Moore | O(m + σ) | O(n/m) best, O(nm) worst | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 5 | Aho-Corasick (multi-pattern) | 🔸 |
| 6 | Suffix Array + LCP | 🔸 |
| 7 | Z-Algorithm | 🔸 |

**Referências**: Cormen Ch. 32; Knuth, Morris & Pratt (1977); Boyer & Moore (1977)

---

## 5. Dynamic Programming

### Must-Have
| # | Algoritmo | Complexidade | Espaço | Status |
|---|-----------|-------------|--------|--------|
| 1 | Fibonacci (memo + tab) | O(n) | O(n) / O(1) | ✅ |
| 2 | LCS (Longest Common Subsequence) | O(mn) | O(mn) | ✅ |
| 3 | Knapsack 0/1 | O(nW) | O(nW) | ✅ |
| 4 | Edit Distance (Levenshtein) | O(mn) | O(mn) | ✅ |
| 5 | LIS (Longest Increasing Subsequence) | O(n log n) | O(n) | ✅ |
| 6 | Rod Cutting | O(n²) | O(n) | ✅ |
| 7 | Matrix Chain Multiplication | O(n³) | O(n²) | ✅ |
| 8 | Coin Change | O(nS) | O(S) | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 9 | Optimal BST | 🔸 |
| 10 | Longest Palindromic Subsequence | 🔸 |
| 11 | Subset Sum (DP) | 🔸 |
| 12 | Traveling Salesman (DP bitmask) | 🔸 |
| 13 | Floyd-Warshall (already in Graph) | ✅ |

**Referências**: Cormen Ch. 15; Bellman (1957)

---

## 6. Greedy Algorithms

### Must-Have
| # | Algoritmo | Complexidade | Status |
|---|-----------|-------------|--------|
| 1 | Activity Selection | O(n log n) | ✅ |
| 2 | Huffman Coding | O(n log n) | ✅ |
| 3 | Fractional Knapsack | O(n log n) | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 4 | Job Sequencing with Deadlines | 🔸 |
| 5 | Minimum Platforms | 🔸 |
| 6 | Egyptian Fractions | 🔸 |

**Referências**: Cormen Ch. 16; Huffman (1952)

---

## 7. Numerical Algorithms

### Must-Have
| # | Algoritmo | Complexidade | Status |
|---|-----------|-------------|--------|
| 1 | GCD (Euclides) | O(log min(a,b)) | ✅ |
| 2 | Extended GCD | O(log min(a,b)) | ✅ |
| 3 | Fast Exponentiation | O(log n) | ✅ |
| 4 | Sieve of Eratosthenes | O(n log log n) | ✅ |
| 5 | Primality Test | O(√n) | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 6 | Miller-Rabin Primality | 🔸 |
| 7 | Chinese Remainder Theorem | 🔸 |
| 8 | Euler's Totient | 🔸 |
| 9 | Newton's Method | 🔸 |
| 10 | Gaussian Elimination | 🔸 |

**Referências**: Cormen Ch. 31; Euclid (~300 BC)

---

## 8. Divide and Conquer

### Must-Have
| # | Algoritmo | Complexidade | Status |
|---|-----------|-------------|--------|
| 1 | Strassen Matrix Multiplication | O(n^2.807) | ✅ |
| 2 | Closest Pair of Points | O(n log n) | ✅ |
| 3 | Karatsuba Multiplication | O(n^1.585) | ✅ |
| 4 | Maximum Subarray (Kadane) | O(n) | ✅ |
| 5 | Quick Select | O(n) avg | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 6 | Median of Medians | 🔸 |
| 7 | Convex Hull (D&C) | 🔸 |
| 8 | FFT (Fast Fourier Transform) | 🔸 |

**Referências**: Cormen Ch. 4; Strassen (1969); Karatsuba (1962)

---

## 9. Backtracking

### Must-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 1 | N-Queens | ✅ |
| 2 | Subset Sum | ✅ |
| 3 | Permutations | ✅ |
| 4 | Graph Coloring | ✅ |

### Nice-to-Have
| # | Algoritmo | Status |
|---|-----------|--------|
| 5 | Sudoku Solver | 🔸 |
| 6 | Hamiltonian Path | 🔸 |
| 7 | Knight's Tour | 🔸 |
| 8 | Rat in a Maze | 🔸 |

**Referências**: Cormen Ch. 34 (NP-Completeness concepts); Wirth (1976)

---

## 10. Heurísticas e Meta-Heurísticas (Fase 3)

### Phase 3A - Classical ✅ COMPLETO
| # | Algoritmo | Tipo | Variantes | Status |
|---|-----------|------|-----------|--------|
| 1 | Hill Climbing | Local Search | Steepest, First Improvement, Random Restart, Stochastic | ✅ |
| 2 | Simulated Annealing | Single-Solution | 4 cooling schedules (geometric/linear/log/adaptive), reheating, auto-calibrate T0 | ✅ |
| 3 | Tabu Search | Single-Solution | Aspiration, diversification, intensification, reactive tenure | ✅ |
| 4 | Genetic Algorithm | Population-Based | Tournament/Roulette/Rank selection, OX/PMX/BLX crossover, Swap/Inversion/Gaussian mutation, adaptive rates | ✅ |

### Phase 3A - Benchmarks ✅ COMPLETO
| # | Benchmark | Tipo | Detalhes | Status |
|---|-----------|------|----------|--------|
| 5 | TSP | Discreto | 3 instâncias hardcoded (5/10/20), aleatórias, swap/2-opt neighbors, double-bridge perturbation | ✅ |
| 6 | Continuous Functions | Contínuo | Sphere, Rastrigin, Rosenbrock, Ackley, Schwefel, vizinhança gaussiana | ✅ |

### Phase 3B - Advanced ✅ COMPLETO
| # | Algoritmo | Tipo | Variantes | Status |
|---|-----------|------|-----------|--------|
| 7 | ILS (Iterated Local Search) | Single-Solution | 4 acceptance criteria (better/always/SA-like/restart) | ✅ |
| 8 | GRASP | Hybrid | RCL construction, reactive GRASP, builtin TSP/continuous constructors | ✅ |
| 9 | PSO (Particle Swarm) | Population-Based | 3 inertia types (constant/linear decreasing/constriction factor) | ✅ |
| 10 | ACO (Ant Colony) | Population-Based | 3 variants (Ant System/Elitist/MAX-MIN), builtin TSP heuristic | ✅ |

### Phase 3C - Specialized ⏳
| # | Algoritmo | Tipo | Status |
|---|-----------|------|--------|
| 11 | DE (Differential Evolution) | Population-Based | ⏳ |
| 12 | VNS (Variable Neighborhood) | Single-Solution | ⏳ |
| 13 | Memetic Algorithm | Hybrid | ⏳ |
| 14 | LNS (Large Neighborhood Search) | Single-Solution | ⏳ |

**Referências**: Talbi (2009); Gendreau & Potvin (2019); Luke (2013); Kirkpatrick et al. (1983); Glover (1986); Holland (1975)

---

## Resumo de Implementação

### Totais por Categoria

| Categoria | Must-Have | Implementados | Nice-to-Have | Total |
|-----------|-----------|---------------|-------------|-------|
| Sorting | 10 | 10 ✅ | 5 | 15 |
| Searching | 6 | 6 ✅ | 2 | 8 |
| Graph Algorithms | 5 | 5 ✅ | 9 | 14 |
| String Matching | 4 | 4 ✅ | 3 | 7 |
| Dynamic Programming | 8 | 8 ✅ | 4 | 12 |
| Greedy | 3 | 3 ✅ | 3 | 6 |
| Numerical | 5 | 5 ✅ | 5 | 10 |
| Divide & Conquer | 5 | 5 ✅ | 3 | 8 |
| Backtracking | 4 | 4 ✅ | 4 | 8 |
| Heurísticas (Phase 3) | 12 | 8 ✅ (3A+3B) | - | 12 |
| Benchmarks (Phase 3) | 2 | 2 ✅ (3A) | - | 2 |
| **TOTAL** | **64** | **60/64 ✅** | **38** | **102** |

### Status Geral
```
Must-Have Algorithms:     50/50 (100%) ✅
Phase 3A (Classical):     4/4 (100%) ✅ + 2 benchmarks
Phase 3B (Advanced):      4/4 (100%) ✅ (ILS, GRASP, PSO, ACO)
Phase 3C (Specialized):   0/4 (0%) ⏳
Nice-to-Have:             0/38 (0%)
Total Testes:             ~639 (308 DS + 199 Alg + 91 Opt 3A + 41 Opt 3B)
```

---

*Última atualização: 2026-02-13*
