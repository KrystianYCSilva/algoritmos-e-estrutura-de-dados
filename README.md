---
description: "Biblioteca acadêmica em C11 de estruturas de dados, algoritmos clássicos, heurísticas e meta-heurísticas com rigor científico e análise de complexidade."
---

# Algoritmos e Heurísticas - Biblioteca Acadêmica em C

[![C Standard](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

## 📚 Sobre o Projeto

Biblioteca acadêmica focada em implementações eficientes e validadas de estruturas de dados, algoritmos clássicos, heurísticas e meta-heurísticas. Todas as implementações seguem princípios acadêmicos rigorosos com referências a papers e pseudocódigos reconhecidos.

### Objetivos

- ✅ **Eficiência**: Implementações otimizadas com análise de complexidade
- ✅ **Genericidade**: Suporte a tipos primitivos, strings, ponteiros, vetores e matrizes
- ✅ **Validação Acadêmica**: Cada algoritmo referencia papers e literatura científica
- ✅ **Documentação**: Código bem documentado com exemplos práticos
- ✅ **Testes**: Cobertura completa com testes unitários e benchmarks

## 🗂️ Estrutura do Projeto

```
algoritmos-heuriticos/
├── include/                    # Headers públicos
│   ├── data_structures/       # Estruturas de dados
│   │   ├── common.h           # Tipos genéricos
│   │   ├── queue.h            # Fila FIFO
│   │   ├── stack.h            # Pilha LIFO
│   │   ├── linked_list.h      # Lista Encadeada
│   │   ├── array_list.h       # Array Dinâmico
│   │   ├── hash_table.h       # Tabela Hash
│   │   ├── binary_tree.h      # Árvore Binária
│   │   ├── bst.h              # Binary Search Tree
│   │   ├── avl_tree.h         # AVL Tree (auto-balanceada)
│   │   ├── heap.h             # Binary Heap
│   │   ├── priority_queue.h   # Fila de Prioridade
│   │   ├── trie.h             # Trie (Prefix Tree)
│   │   ├── graph.h            # Grafo
│   │   └── union_find.h       # Disjoint Set
│   ├── algorithms/            # Algoritmos clássicos
│   │   ├── sorting/
│   │   ├── searching/
│   │   └── graph_algorithms/
│   ├── heuristics/            # Heurísticas construtivas
│   │   └── greedy/
│   └── metaheuristics/        # Meta-heurísticas
│       ├── genetic_algorithm.h
│       ├── simulated_annealing.h
│       ├── tabu_search.h
│       └── ant_colony.h
├── src/                       # Implementações
│   ├── data_structures/
│   ├── algorithms/
│   ├── heuristics/
│   └── metaheuristics/
├── tests/                     # Testes unitários
├── examples/                  # Exemplos de uso
├── benchmarks/                # Benchmarks de performance
├── docs/                      # Documentação adicional
│   ├── references/            # Papers e referências
│   └── complexity_analysis/   # Análise de complexidade
└── CMakeLists.txt
```

## 🔧 Estruturas de Dados

### Fase 1A - Lineares (Básicas)
1. **Queue** (Fila FIFO) - Cormen Ch. 10.1 - O(1) enqueue/dequeue
2. **Stack** (Pilha LIFO) - Cormen Ch. 10.1 - O(1) push/pop
3. **LinkedList** (Lista Encadeada) - Knuth Vol 1, Sec 2.2 - O(1) insert/remove com ponteiro
4. **ArrayList** (Array Dinâmico) - Goodrich Ch. 7 - O(1) acesso, O(1) amortizado append

### Fase 1B - Associativas e Árvores (Críticas)
5. **Hash Table** - Cormen Ch. 11 - O(1) amortizado insert/search/delete
   - Chaining e Open Addressing (Linear/Quadratic/Double Hashing)
6. **Binary Tree** - Base para BST, AVL, etc.
7. **BST** (Binary Search Tree) - Cormen Ch. 12 - O(h) operações
8. **Heap** (Min/Max Binary Heap) - Cormen Ch. 6 - O(log n) insert/extract
9. **Graph** (Grafo) - Cormen Ch. 22 - Adjacency List e Matrix

### Fase 1C - Balanceadas e Especializadas (Importantes)
10. **AVL Tree** - Adelson-Velsky & Landis (1962) - O(log n) GARANTIDO
11. **Priority Queue** - Implementada sobre Heap - O(log n) insert/extract
12. **Trie** (Prefix Tree) - Knuth Vol 3, Sec 6.3 - O(m) onde m = tamanho string
13. **Union-Find** (Disjoint Set) - Cormen Ch. 21 - O(α(n)) ≈ O(1)

## 🧬 Design de Tipos Genéricos

Para suportar genericidade em C, utilizamos a seguinte estratégia:

```c
// Estrutura base genérica
typedef struct {
    void *data;              // Ponteiro para dados
    size_t size;             // Tamanho do tipo
    void (*destructor)(void*); // Função de destruição customizada
    void* (*copy)(const void*); // Função de cópia customizada
} GenericData;

// Funções auxiliares para comparação, hash, etc.
typedef int (*CompareFn)(const void*, const void*);
typedef size_t (*HashFn)(const void*);
typedef void (*PrintFn)(const void*);
```

### Exemplos de Uso

```c
// Criando uma fila de inteiros
Queue *int_queue = queue_create(sizeof(int), NULL, NULL);

// Criando uma pilha de strings
Stack *str_stack = stack_create(sizeof(char*), free_string, copy_string);

// Criando uma lista de structs customizadas
LinkedList *list = list_create(sizeof(Person), free_person, copy_person);
```

## 📖 Referências Bibliográficas

### Estruturas de Dados
- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Knuth, D. E. (1997). *The Art of Computer Programming, Volume 1: Fundamental Algorithms* (3rd ed.). Addison-Wesley.
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley.

### Algoritmos e Heurísticas
- Talbi, E.-G. (2009). *Metaheuristics: From Design to Implementation*. Wiley.
- Gendreau, M., & Potvin, J.-Y. (2010). *Handbook of Metaheuristics* (2nd ed.). Springer.
- Papadimitriou, C. H., & Steiglitz, K. (1998). *Combinatorial Optimization: Algorithms and Complexity*. Dover.

## 🚀 Como Compilar

```bash
# Criar diretório de build
mkdir build && cd build

# Configurar com CMake
cmake ..

# Compilar
cmake --build .

# Executar testes
ctest --output-on-failure
```

## 🧪 Testes e Validação

Cada estrutura de dados e algoritmo inclui:
- ✅ Testes unitários (correção)
- ✅ Testes de stress (robustez)
- ✅ Benchmarks (performance)
- ✅ Análise de complexidade empírica

## 📝 Roadmap

### Fase 1A: Estruturas Lineares ⏳ (Estrutura Criada)
- [ ] Queue (Array circular buffer e Linked)
- [ ] Stack (Array e Linked)
- [ ] Linked List (Single, Double, Circular)
- [ ] ArrayList (Dynamic Array)

### Fase 1B: Hash e Árvores ⏳ (Estrutura Criada)
- [ ] Hash Table (Chaining, Linear/Quadratic/Double Hashing)
- [ ] Binary Tree (base)
- [ ] Binary Search Tree (BST)
- [ ] Heap (Min/Max Binary Heap)
- [ ] Graph (Adjacency List e Matrix)

### Fase 1C: Balanceadas e Especializadas ⏳ (Estrutura Criada)
- [ ] AVL Tree (auto-balanceada)
- [ ] Priority Queue (sobre Heap)
- [ ] Trie (Prefix Tree para strings)
- [ ] Union-Find (Disjoint Set)

### Fase 2: Estruturas Avançadas (Futuro)
- [ ] Red-Black Tree
- [ ] B-Tree / B+ Tree
- [ ] Skip List
- [ ] Bloom Filter
- [ ] Segment Tree
- [ ] Fenwick Tree

### Fase 3: Algoritmos Clássicos
- [ ] Sorting (QuickSort, MergeSort, HeapSort)
- [ ] Searching (Binary Search, Interpolation Search)
- [ ] Graph Algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)

### Fase 4: Heurísticas e Meta-heurísticas
- [ ] Greedy Algorithms
- [ ] Genetic Algorithms
- [ ] Simulated Annealing
- [ ] Tabu Search
- [ ] Ant Colony Optimization
- [ ] Particle Swarm Optimization

## 📄 Licença

Este projeto é licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 👥 Contribuindo

Contribuições são bem-vindas! Por favor:
1. Inclua referências acadêmicas para novos algoritmos
2. Adicione testes unitários completos
3. Documente a análise de complexidade
4. Siga o estilo de código do projeto

## 📧 Contato

Para dúvidas ou sugestões sobre implementações acadêmicas, abra uma issue no repositório.

---

**Nota**: Este projeto tem fins educacionais e de pesquisa. Todas as implementações seguem princípios acadêmicos rigorosos e incluem análise de complexidade teórica e empírica.
