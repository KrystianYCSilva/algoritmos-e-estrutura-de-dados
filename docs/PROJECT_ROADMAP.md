---
description: "Plano geral do projeto: estruturas planejadas (13 totais), fases de implementação (1A/1B/1C), progresso atual e roadmap completo."
---

# Project Roadmap - Algoritmos e Heurísticas

## 📊 Visão Geral

**Objetivo**: Biblioteca acadêmica em C11 de estruturas de dados, algoritmos e heurísticas com rigor científico.

**Total de Estruturas**: 13 estruturas essenciais organizadas em 3 fases  
**Status Atual**: Fase 1A ✅ COMPLETA | Fase 1B ⏳ 40% | Fase 1C ⏳ PENDENTE

---

## 🎯 Estruturas Planejadas (13 Total)

### Fase 1A - Estruturas Lineares ✅ COMPLETA (5/5)

| # | Estrutura | Arquivo | Referência | Complexidade | Status |
|---|-----------|---------|------------|--------------|--------|
| 1 | **Queue** | queue.c/h | Cormen 10.1 | O(1) enqueue/dequeue | ✅ |
| 2 | **Stack** | stack.c/h | Cormen 10.1 | O(1) push/pop | ✅ |
| 3 | **LinkedList** | linked_list.c/h | Knuth Vol1 2.2 | O(1) insert c/ ptr | ✅ |
| 4 | **ArrayList** | array_list.c/h | Goodrich Ch7 | O(1) amort append | ✅ |
| 5 | **Common** | common.c/h | - | Funções auxiliares | ✅ |

**Implementado**: 2.660 linhas, 132 testes  
**Data Conclusão**: 2025-02-12

---

### Fase 1B - Associativas e Árvores ⏳ 40% (2/5)

| # | Estrutura | Arquivo | Referência | Complexidade | Status |
|---|-----------|---------|------------|--------------|--------|
| 6 | **HashTable** | hash_table.c/h | Cormen 11 | O(1) amort | ✅ |
| 7 | **BinaryTree** | binary_tree.c/h | Cormen 12 | Base p/ BST | ✅ |
| 8 | **BST** | bst.c/h | Cormen 12 | O(log n) médio | 🔄 PRÓXIMO |
| 9 | **Heap** | heap.c/h | Cormen 6 | O(log n) | ⏳ |
| 10 | **Graph** | graph.c/h | Cormen 22 | Varia | ⏳ |

**Implementado**: ~1.900 linhas, 56 testes  
**Estimativa Restante**: ~1.900 linhas, 60 testes

**HashTable Features**:
- Chaining + Open Addressing (Linear/Quadratic/Double Hashing)
- Rehashing automático (load factor 0.75 chaining, 0.5 open)
- Hash functions: djb2, FNV-1a, multiplicativa (Knuth)

**BinaryTree Features**:
- Travessias: inorder, preorder, postorder, level-order (BFS)
- Propriedades: altura, folhas, is_complete, is_full, is_perfect
- Algoritmos: LCA, distance, diameter

---

### Fase 1C - Balanceadas e Especializadas ⏳ PENDENTE (0/4)

| # | Estrutura | Arquivo | Referência | Complexidade | Status |
|---|-----------|---------|------------|--------------|--------|
| 11 | **AVL Tree** | avl_tree.c/h | Adelson-Velsky 1962 | O(log n) GARANTIDO | ⏳ |
| 12 | **PriorityQueue** | priority_queue.c/h | Cormen 6 | O(log n) | ⏳ |
| 13 | **Trie** | trie.c/h | Knuth Vol3 6.3 | O(m) | ⏳ |
| 14 | **UnionFind** | union_find.c/h | Cormen 21 | O(α(n)) ≈ O(1) | ⏳ |

**Estimativa**: ~1.500 linhas, 50 testes

---

## 📈 Progresso Atual

```
╔═══════════════════════════════════════════════════════════╗
║                  PROGRESSO FASE 1                         ║
╠═══════════════════════════════════════════════════════════╣
║ Fase 1A (Lineares):           ✅ 100% (5/5 estruturas)   ║
║   - Queue, Stack, LinkedList, ArrayList, Common          ║
║   - 2.660 linhas, 132 testes                             ║
║                                                           ║
║ Fase 1B (Associativas):       ⏳ 40% (2/5 estruturas)    ║
║   - HashTable ✅, BinaryTree ✅                           ║
║   - BST 🔄, Heap ⏳, Graph ⏳                             ║
║   - 1.900 linhas, 56 testes                              ║
║                                                           ║
║ Fase 1C (Especializadas):     ⏳ 0% (0/4 estruturas)     ║
║   - AVL, PriorityQueue, Trie, UnionFind                  ║
║                                                           ║
║ TOTAL GERAL:                  54% (7/13 estruturas)      ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🎓 Validação Acadêmica

### Referências Bibliográficas

**Principal**:
- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.)
  - Ch. 6: Heapsort
  - Ch. 10: Elementary Data Structures  
  - Ch. 11: Hash Tables
  - Ch. 12: Binary Search Trees
  - Ch. 21: Disjoint Sets
  - Ch. 22: Graph Algorithms

**Complementares**:
- Knuth, D. E. (1997-1998). *The Art of Computer Programming*
  - Vol 1: Fundamental Algorithms (Lists, Trees)
  - Vol 3: Sorting and Searching (Hash, BST, Trie)
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.)
- Goodrich, M. T., et al. (2011). *Data Structures and Algorithms in C++*

**Papers Clássicos**:
- Adelson-Velsky & Landis (1962) - AVL Tree
- Tarjan (1975) - Union-Find efficiency
- Fredkin (1960) - Trie memory

### Pseudocódigos Implementados

✅ **Fase 1A**:
- ENQUEUE/DEQUEUE (Cormen p. 235)
- PUSH/POP (Cormen p. 233)
- LIST-INSERT/DELETE (Cormen p. 238)
- BINARY-SEARCH (Cormen p. 799)

✅ **Fase 1B** (parcial):
- CHAINED-HASH-INSERT/SEARCH (Cormen p. 258)
- HASH-INSERT/SEARCH open addressing (Cormen p. 271)
- INORDER/PREORDER/POSTORDER-TREE-WALK (Cormen p. 288)

⏳ **Pendentes**:
- TREE-INSERT/DELETE/SEARCH (Cormen p. 290-298) - BST
- MAX-HEAPIFY, BUILD-MAX-HEAP (Cormen p. 154-157) - Heap
- BFS, DFS (Cormen p. 594, 604) - Graph

---

## 🔧 Design de Genericidade

### Tipos Genéricos em C

Todas as estruturas suportam tipos genéricos via `void*`:

```c
// Function pointers para operações customizadas
typedef int (*CompareFn)(const void *a, const void *b);
typedef void* (*CopyFn)(const void *src);
typedef void (*DestroyFn)(void *data);
typedef void (*PrintFn)(const void *data);
typedef size_t (*HashFn)(const void *data);

// Suporta:
✅ Primitivos: int, float, double, char
✅ Strings: char* com gerenciamento automático
✅ Ponteiros: void*, structs*
✅ Structs customizadas com copy/destroy
✅ Arrays e matrizes
```

---

## 📊 Estatísticas do Projeto

### Implementado (Fases 1A + parte 1B)

```
Estruturas Completas:   7/13 (54%)
Linhas de Código:       4.560
Testes Unitários:       188
Arquivos .c:            7
Arquivos .h:            14 (todos os headers criados)
Documentação:           100% dos arquivos
Referências:            15+ livros e papers
Pseudocódigos:          10+ algoritmos clássicos
```

### Estimativa Total (ao final)

```
Linhas de Código:       ~7.000
Testes Unitários:       ~290
Tempo Total:            8-10 semanas
```

---

## ⏭️ Próximos Passos

### Imediato (Fase 1B)

1. **BST** (Binary Search Tree) 🔄 PRÓXIMO
   - TREE-INSERT, TREE-SEARCH, TREE-DELETE
   - Min, Max, Successor, Predecessor
   - Validação BST
   - Estimativa: ~600 linhas, ~20 testes

2. **Heap** (Min/Max Binary Heap)
   - MAX-HEAPIFY, BUILD-MAX-HEAP
   - Insert, Extract-Min/Max
   - Heap Sort
   - Estimativa: ~500 linhas, ~18 testes

3. **Graph** (Adjacency List + Matrix)
   - BFS, DFS
   - Directed/Undirected, Weighted/Unweighted
   - Estimativa: ~800 linhas, ~22 testes

### Curto Prazo (Fase 1C)

4. **AVL Tree** - Auto-balanceamento garantido
5. **Priority Queue** - Implementada sobre Heap
6. **Trie** - Autocomplete e prefix matching
7. **Union-Find** - Disjoint sets com path compression

### Médio Prazo (Fase 2 - Futuro)

- Red-Black Tree
- B-Tree / B+ Tree
- Skip List
- Bloom Filter
- Segment Tree
- Fenwick Tree

### Longo Prazo (Fase 3+)

- **Algoritmos Clássicos**: Sorting, Searching, Graph algorithms
- **Heurísticas**: Greedy algorithms
- **Meta-heurísticas**: Genetic Algorithms, Simulated Annealing, Tabu Search

---

## 🔨 Build e Testes

### Compilar

```bash
# Debug build (padrão)
mkdir -p build && cd build
cmake ..
cmake --build .

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# Com sanitizers (recomendado desenvolvimento)
cmake -DENABLE_ASAN=ON -DENABLE_UBSAN=ON ..
cmake --build .
```

### Executar Testes

```bash
# Todos os testes
cd cmake-build-debug
ctest --output-on-failure

# Testes individuais
./test_common
./test_queue
./test_stack
./test_linked_list
./test_array_list
./test_hash_table
./test_binary_tree
```

### Verificar Memory Leaks

```bash
valgrind --leak-check=full --show-leak-kinds=all ./test_common
```

---

## ✅ Critérios de Qualidade

Para cada estrutura implementada:

### Correção
- [ ] Todos os testes unitários passam
- [ ] Valgrind: zero memory leaks
- [ ] UBSan: sem undefined behavior
- [ ] ASan: sem buffer overflows

### Performance
- [ ] Complexidade empírica = teórica
- [ ] Benchmarks dentro de ±10% esperado

### Qualidade de Código
- [ ] Compilação sem warnings (-Wall -Wextra -Wpedantic)
- [ ] Documentação Doxygen completa
- [ ] Pseudocódigo acadêmico seguido

### Validação Acadêmica
- [ ] Referências bibliográficas citadas
- [ ] Análise de complexidade documentada
- [ ] Algoritmos clássicos implementados corretamente

---

## 📚 Documentação Complementar

- **README.md** - Visão geral do projeto
- **AGENTS.md** - Regras para coding agents
- **docs/DATA_STRUCTURE_GUIDE.md** - Guia de escolha
- **docs/IMPLEMENTATION_PLAN.md** - Plano detalhado por fase
- **docs/references/ESSENTIAL_DATA_STRUCTURES.md** - Checklist acadêmica

---

## 🎯 Métricas de Sucesso (Fase 1 Completa)

| Métrica | Meta | Atual | Status |
|---------|------|-------|--------|
| Estruturas Fase 1 | 13 | 7 | ⏳ 54% |
| Linhas de Código | ~7.000 | 4.560 | ⏳ 65% |
| Testes | ~290 | 188 | ⏳ 65% |
| Cobertura | 100% | 100% | ✅ |
| Documentação | 100% | 100% | ✅ |
| Referências | Todas | Todas | ✅ |

---

**Última Atualização**: 2025-02-12  
**Status Geral**: ⏳ Fase 1B em andamento (40% completa)  
**Próximo Marco**: Completar BST, Heap, Graph (Fase 1B 100%)
