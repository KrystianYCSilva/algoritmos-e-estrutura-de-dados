---
description: "Guia de decisão para escolher a estrutura de dados correta baseado em padrões de acesso, complexidade e requisitos de performance."
---

# Guia de Escolha de Estruturas de Dados

## Como Escolher a Estrutura de Dados Correta

Este guia ajuda a escolher a estrutura mais apropriada para cada situação, baseado em padrões de acesso e requisitos de performance.

## Tabela Comparativa Rápida

### Estruturas Lineares

| Estrutura | Acesso | Inserção (fim) | Inserção (início) | Busca | Memória |
|-----------|--------|----------------|-------------------|-------|---------|
| **ArrayList** | O(1) | O(1)* | O(n) | O(n) / O(log n)† | Baixo |
| **LinkedList** | O(n) | O(1) | O(1) | O(n) | Alto |
| **Stack** | O(1)‡ | O(1)* | N/A | N/A | Baixo |
| **Queue** | O(1)‡ | O(1)* | N/A | N/A | Baixo |

### Estruturas Associativas e Árvores

| Estrutura | Insert | Search | Delete | Min/Max | Ordenado |
|-----------|--------|--------|--------|---------|----------|
| **HashTable** | O(1)* | O(1)* | O(1)* | O(n) | Não |
| **BST** | O(log n)§ | O(log n)§ | O(log n)§ | O(log n)§ | Sim |
| **AVL Tree** | O(log n) | O(log n) | O(log n) | O(log n) | Sim |
| **Heap** | O(log n) | O(n) | O(log n)‖ | O(1) | Parcial |
| **Trie** | O(m) | O(m) | O(m) | N/A | Sim (lex) |

*: Amortizado | †: Com array ordenado | ‡: Apenas topo/front | §: Caso médio, O(n) pior caso | ‖: Extract-min/max | m: comprimento da string

### Estruturas Especializadas

| Estrutura | Operação Principal | Complexidade | Uso Típico |
|-----------|-------------------|-------------|------------|
| **Priority Queue** | Extract-min/max | O(log n) | Scheduling, Dijkstra |
| **Union-Find** | Find / Union | O(α(n)) ≈ O(1) | Componentes conexos, Kruskal |
| **Graph** | BFS/DFS | O(V+E) | Redes, caminhos |

---

## Casos de Uso Detalhados

### ArrayList - Use Quando:

✅ **INDICADO**:
- Acesso frequente por índice
- Inserções principalmente no final
- Memória contígua (cache locality)
- Busca binária após ordenação

❌ **NÃO INDICADO**:
- Inserções/remoções frequentes no início/meio
- Tamanho varia muito e imprevisível

```c
ArrayList *arr = arraylist_create(sizeof(int), 100, NULL);
arraylist_push_back(arr, &val);
arraylist_get(arr, i, &out);
arraylist_sort(arr, compare_int);
```

---

### LinkedList - Use Quando:

✅ **INDICADO**:
- Inserções/remoções em posições arbitrárias
- Tamanho muito variável
- Implementar outras estruturas (Graph adjacencies)

❌ **NÃO INDICADO**:
- Acesso por índice frequente
- Cache locality crítica

```c
LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);
list_push_front(list, &val);
list_push_back(list, &val);
```

---

### Stack - Use Quando:

✅ **INDICADO**: LIFO, backtracking, undo/redo, DFS, parsing de expressões, parênteses balanceados

```c
Stack *s = stack_create(sizeof(int), STACK_ARRAY, 50, NULL);
stack_push(s, &val);
stack_pop(s, &out);
```

---

### Queue - Use Quando:

✅ **INDICADO**: FIFO, BFS, buffer de eventos, scheduling, producer-consumer

```c
Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 100, NULL);
queue_enqueue(q, &val);
queue_dequeue(q, &out);
```

---

### HashTable - Use Quando:

✅ **INDICADO**:
- Lookup O(1) por chave
- Dicionários, caches, sets, contagem de frequência
- Deduplicação

❌ **NÃO INDICADO**:
- Dados precisam estar ordenados
- Range queries
- Memória limitada (overhead de buckets)

```c
HashTable *ht = hashtable_create(sizeof(char*), sizeof(int), 16,
    hash_string, compare_string, HASH_CHAINING,
    destroy_string, NULL);
hashtable_put(ht, &key, &val);
hashtable_get(ht, &key, &out);
```

**Estratégias de colisão**:
- `HASH_CHAINING`: Melhor para load factors altos, simples
- `HASH_LINEAR_PROBING`: Melhor cache locality, clustering
- `HASH_QUADRATIC_PROBING`: Menos clustering que linear
- `HASH_DOUBLE_HASHING`: Melhor distribuição, mais lento

---

### BST / AVL Tree - Use Quando:

✅ **INDICADO**:
- Dados ordenados com insert/search/delete O(log n)
- Range queries eficientes
- Successor/predecessor queries
- Min/max em O(log n)

❌ **NÃO INDICADO**:
- Apenas lookup por chave (use HashTable)
- Dados não têm ordem natural

```c
// BST - O(log n) médio, O(n) pior caso
BST *tree = bst_create(sizeof(int), compare_int, NULL);
bst_insert(tree, &val);

// AVL - O(log n) GARANTIDO
AVLTree *avl = avl_create(sizeof(int), compare_int, NULL);
avl_insert(avl, &val);
```

**Quando BST vs AVL?**
- **BST**: Dados inseridos aleatoriamente, implementação simples
- **AVL**: Dados podem vir ordenados (BST degenera para lista), precisa de garantia O(log n)

---

### Heap / Priority Queue - Use Quando:

✅ **INDICADO**:
- Extrair mínimo/máximo repetidamente
- Dijkstra, A*, scheduling
- Top-K elements
- Mediana em streaming

❌ **NÃO INDICADO**:
- Busca por chave arbitrária (O(n))
- Dados precisam estar totalmente ordenados

```c
Heap *h = heap_create(sizeof(int), 16, HEAP_MIN, compare_int, NULL);
PriorityQueue *pq = priority_queue_create(sizeof(int), 16, PQ_MIN, compare_int, NULL);
```

---

### Graph - Use Quando:

✅ **INDICADO**: Modelagem de redes, caminhos mínimos, árvore geradora, ciclos, componentes

```c
// Adjacency List (esparso) - recomendado para maioria dos casos
Graph *g = graph_create(100, GRAPH_UNDIRECTED, GRAPH_ADJACENCY_LIST, true);

// Adjacency Matrix (denso) - quando E ≈ V²
Graph *g = graph_create(100, GRAPH_DIRECTED, GRAPH_ADJACENCY_MATRIX, true);
```

**Quando List vs Matrix?**
- **List**: E << V² (esparso), maioria dos grafos reais
- **Matrix**: E ≈ V² (denso), Floyd-Warshall, verificar aresta O(1)

---

### Trie - Use Quando:

✅ **INDICADO**: Autocomplete, prefix matching, dicionários de strings, longest common prefix

```c
Trie *t = trie_create(26);
trie_insert(t, "algorithm");
trie_starts_with(t, "algo");  // true
trie_autocomplete(t, "al", &results, &count);
```

---

### Union-Find - Use Quando:

✅ **INDICADO**: Componentes conexos dinâmicos, Kruskal MST, equivalência de classes

```c
UnionFind *uf = union_find_create(n);
union_find_union(uf, a, b);
union_find_connected(uf, a, b);
```

---

## Regras de Ouro

1. **Acesso aleatório frequente** → ArrayList
2. **Inserções/remoções frequentes** → LinkedList
3. **LIFO** → Stack
4. **FIFO** → Queue
5. **Lookup por chave O(1)** → HashTable
6. **Dados ordenados + range queries** → BST / AVL
7. **Extrair min/max repetidamente** → Heap / Priority Queue
8. **Prefix matching em strings** → Trie
9. **"Quem pertence ao mesmo grupo?"** → Union-Find
10. **Modelar relações entre entidades** → Graph
11. **Quando em dúvida e performance importa** → ArrayList (cache locality)

---

## Referências

1. Cormen et al. (2009), Chapters 6, 10-13, 21-22
2. Knuth (1997-1998), TAOCP Vol 1-3
3. Sedgewick & Wayne (2011), Algorithms (4th ed.)
4. Skiena (2020), The Algorithm Design Manual (3rd ed.), Chapter 3

---

**Última atualização**: 2026-02-12
