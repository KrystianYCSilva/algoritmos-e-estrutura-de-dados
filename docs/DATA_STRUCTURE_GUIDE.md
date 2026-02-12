---
description: "Guia de decisão para escolher a estrutura de dados correta baseado em padrões de acesso, complexidade e requisitos de performance."
---

# Guia de Escolha de Estruturas de Dados

## 🎯 Como Escolher a Estrutura de Dados Correta

Este guia ajuda a escolher a estrutura de dados mais apropriada para cada situação, baseado em padrões de acesso e requisitos de performance.

## 📊 Tabela Comparativa Rápida

| Estrutura | Acesso | Inserção (fim) | Inserção (início) | Inserção (meio) | Busca | Uso de Memória |
|-----------|--------|----------------|-------------------|-----------------|-------|----------------|
| **ArrayList** | O(1) | O(1)* | O(n) | O(n) | O(n) | Baixo (contíguo) |
| **LinkedList** | O(n) | O(1) | O(1) | O(1)† | O(n) | Alto (ponteiros) |
| **Stack** | O(1)‡ | O(1)* | N/A | N/A | N/A | Baixo |
| **Queue** | O(1)‡ | O(1)* | N/A | N/A | N/A | Baixo |

*: Amortizado
†: Com ponteiro para a posição
‡: Apenas topo/front

## 🔍 Casos de Uso Detalhados

### ArrayList - Use Quando:

✅ **INDICADO**:
- Acesso frequente por índice (ex: `array[i]`)
- Inserções principalmente no final
- Tamanho aproximado conhecido antecipadamente
- Memória contígua é importante (melhor cache locality)
- Iteração sequencial frequente
- Busca binária necessária (após ordenação)

❌ **NÃO INDICADO**:
- Inserções/remoções frequentes no início ou meio
- Tamanho varia muito e imprevisível
- Realocações frequentes são problemáticas

**Exemplos Práticos**:
```c
// ✅ BOM USO: Buffer de dados, histórico
ArrayList *history = arraylist_create(sizeof(Action), 100, NULL);

// ✅ BOM USO: Coordenadas em um jogo
typedef struct { float x, y, z; } Point3D;
ArrayList *vertices = arraylist_create(sizeof(Point3D), 1000, NULL);

// ❌ MAU USO: Fila de prioridade com muitas inserções no meio
// Use heap ou lista encadeada ordenada
```

**Complexidade Detalhada**:
- `arraylist_get(i)`: O(1) - acesso direto via ponteiro
- `arraylist_push_back()`: O(1) amortizado - análise: 1 + 2 + 4 + ... + n < 2n
- `arraylist_insert(i)`: O(n - i) - precisa deslocar elementos
- `arraylist_binary_search()`: O(log n) - requer array ordenado

---

### LinkedList - Use Quando:

✅ **INDICADO**:
- Inserções/remoções frequentes em posições arbitrárias
- Tamanho muito variável e imprevisível
- Não precisa de acesso por índice
- Iteração sempre sequencial
- Implementar outras estruturas (Queue, Stack, Graph adjacencies)
- Algoritmos que requerem splice/merge constantes

❌ **NÃO INDICADO**:
- Acesso frequente por índice
- Cache locality é crítica
- Memória limitada (overhead de ponteiros)
- Precisa de busca binária

**Exemplos Práticos**:
```c
// ✅ BOM USO: Lista de tarefas com prioridades dinâmicas
LinkedList *tasks = list_create(sizeof(Task), LIST_DOUBLY, destroy_task);

// ✅ BOM USO: Histórico de navegação (fácil inserir/remover)
LinkedList *browser_history = list_create(sizeof(URL), LIST_DOUBLY, free_url);

// ✅ BOM USO: Implementar LRU Cache
LinkedList *lru_list = list_create(sizeof(CacheEntry), LIST_DOUBLY, free_entry);

// ❌ MAU USO: Array de pixels de uma imagem (acesso aleatório frequente)
// Use ArrayList
```

**Complexidade Detalhada**:
- `list_get(i)`: O(i) - precisa percorrer até a posição
- `list_push_front/back()`: O(1) - apenas ajusta ponteiros
- `list_insert_after(node)`: O(1) - COM ponteiro para o nó
- `list_find()`: O(n) - sempre linear
- Overhead de memória: +16 bytes por elemento (2 ponteiros em 64-bit)

---

### Stack - Use Quando:

✅ **INDICADO**:
- Precisar de LIFO (Last In, First Out)
- Backtracking / recursão iterativa
- Desfazer/refazer operações (undo/redo)
- Validação de parênteses, tags, etc.
- DFS (Depth-First Search)
- Parsing de expressões

❌ **NÃO INDICADO**:
- Precisa acessar elementos além do topo
- Precisa de FIFO (use Queue)

**Exemplos Práticos**:
```c
// ✅ CLÁSSICO: Validação de parênteses balanceados
Stack *paren_stack = stack_create(sizeof(char), STACK_ARRAY, 50, NULL);

// ✅ CLÁSSICO: Undo/Redo
Stack *undo_stack = stack_create(sizeof(Command), STACK_ARRAY, 100, destroy_cmd);
Stack *redo_stack = stack_create(sizeof(Command), STACK_ARRAY, 100, destroy_cmd);

// ✅ CLÁSSICO: Avaliação de expressões RPN (Reverse Polish Notation)
Stack *rpn_stack = stack_create(sizeof(double), STACK_ARRAY, 30, NULL);

// ✅ ALGORITMO: DFS iterativo
Stack *dfs_stack = stack_create(sizeof(Node*), STACK_ARRAY, 1000, NULL);
```

**Quando usar STACK_ARRAY vs STACK_LINKED**:
- **STACK_ARRAY**: Melhor performance, tamanho máximo conhecido
- **STACK_LINKED**: Tamanho ilimitado, mas mais lento (cache misses)

---

### Queue - Use Quando:

✅ **INDICADO**:
- Precisar de FIFO (First In, First Out)
- BFS (Breadth-First Search)
- Buffer de eventos/mensagens
- Scheduling de tarefas
- Producer-Consumer pattern
- Simulações (filas de atendimento)

❌ **NÃO INDICADO**:
- Precisa acessar elementos no meio
- Precisa de LIFO (use Stack)

**Exemplos Práticos**:
```c
// ✅ CLÁSSICO: BFS em grafos
Queue *bfs_queue = queue_create(sizeof(Node*), QUEUE_ARRAY, 1000, NULL);

// ✅ SISTEMA: Fila de tarefas
Queue *task_queue = queue_create(sizeof(Task), QUEUE_LINKED, 0, destroy_task);

// ✅ SIMULAÇÃO: Fila de atendimento bancário
Queue *bank_queue = queue_create(sizeof(Customer), QUEUE_ARRAY, 50, NULL);

// ✅ EVENTOS: Buffer circular para logs
Queue *log_buffer = queue_create(sizeof(LogEntry), QUEUE_ARRAY, 1024, free_log);
```

**Quando usar QUEUE_ARRAY vs QUEUE_LINKED**:
- **QUEUE_ARRAY**: Melhor performance (circular buffer), tamanho previsível
- **QUEUE_LINKED**: Tamanho ilimitado, útil para picos imprevisíveis

---

## 🧪 Benchmarks Esperados

### Inserção de 1 milhão de elementos:

| Estrutura | Tempo (ms) | Memória (MB) | Cache Misses |
|-----------|------------|--------------|--------------|
| ArrayList (push_back) | ~50 | ~8 | Baixo |
| LinkedList (push_back) | ~150 | ~24 | Alto |
| Stack Array (push) | ~50 | ~8 | Baixo |
| Queue Array (enqueue) | ~60 | ~8 | Baixo |

### Acesso aleatório (1M operações):

| Estrutura | Tempo (ms) | Razão |
|-----------|------------|-------|
| ArrayList[random] | ~10 | - |
| LinkedList[random] | ~500000 | 50000x mais lento! |

### Busca linear (1M elementos):

| Estrutura | Tempo (ms) | Observação |
|-----------|------------|------------|
| ArrayList | ~15 | Cache-friendly |
| LinkedList | ~50 | Cache misses |

## 📖 Referências para Escolha

**Literatura**:
1. Cormen et al. (2009), Chapter 10 - "Elementary Data Structures"
   - Discussão sobre trade-offs entre arrays e listas encadeadas

2. Sedgewick & Wayne (2011), Section 1.3 - "Bags, Queues, and Stacks"
   - Análise empírica de performance

3. Skiena (2020), "The Algorithm Design Manual" (3rd ed.), Chapter 3
   - Guia prático de escolha de estruturas

**Regras de Ouro**:

1. **"Acesso aleatório frequente → ArrayList"**
2. **"Inserções/remoções frequentes → LinkedList"**
3. **"LIFO → Stack"**
4. **"FIFO → Queue"**
5. **"Quando em dúvida e performance importa → ArrayList"** (melhor cache locality)

## 🔬 Experimentos Recomendados

Para seu caso de uso específico, recomendamos:

1. **Implementar protótipo** com a estrutura "óbvia"
2. **Medir com profiler** (gprof, perf, Valgrind)
3. **Identificar gargalos** (CPU-bound vs memory-bound)
4. **Testar alternativa** se necessário
5. **Comparar empiricamente**

**Exemplo de análise**:
```bash
# Compilar com profiling
gcc -pg -O2 programa.c -o programa

# Executar
./programa

# Analisar
gprof programa gmon.out > analysis.txt
```

---

**Nota**: Estas recomendações assumem hardware moderno (cache L1/L2/L3, RAM abundante).
Para sistemas embarcados ou restrições específicas, análise adicional pode ser necessária.
