---
description: "Plano detalhado de implementação das estruturas de dados em 3 fases (lineares, associativas/árvores, balanceadas/especializadas)."
---

# Plano de Implementação - Estruturas de Dados

## 📋 Overview

Este documento descreve o plano detalhado para implementação das estruturas de dados fundamentais do projeto, seguindo princípios acadêmicos rigorosos e melhores práticas de engenharia de software.

## 🎯 Fase 1: Fundação (Semanas 1-2)

### 1.1 Implementar common.c
**Prioridade**: Alta
**Dependências**: Nenhuma
**Arquivos**: `src/data_structures/common.c`

**Tarefas**:
- [ ] Implementar funções de comparação para tipos primitivos
  - `compare_int`, `compare_float`, `compare_double`, `compare_string`
- [ ] Implementar funções de cópia e destruição para strings
  - `copy_string`, `destroy_string`
- [ ] Implementar funções de impressão
  - `print_int`, `print_float`, `print_double`, `print_string`
- [ ] Implementar funções hash
  - `hash_int`, `hash_string` (usar algoritmo djb2 ou FNV-1a)
- [ ] Escrever testes unitários completos
- [ ] Documentar com exemplos

**Referências**:
- FNV Hash: http://www.isthe.com/chongo/tech/comp/fnv/
- djb2 Hash: http://www.cse.yorku.ca/~oz/hash.html

### 1.2 Setup de Testes
**Prioridade**: Alta
**Dependências**: Nenhuma
**Arquivos**: `tests/test_framework.h`, `tests/test_framework.c`

**Tarefas**:
- [ ] Integrar framework de testes (Unity ou similar)
- [ ] Criar macros de assertion customizadas
- [ ] Setup de cobertura de código (gcov/lcov)
- [ ] Configurar CI/CD (opcional)

## 🎯 Fase 2: Queue (Semanas 2-3)

### 2.1 Queue Array-Based
**Prioridade**: Alta
**Dependências**: common.c
**Arquivos**: `src/data_structures/queue_array.c`

**Estrutura Interna**:
```c
struct Queue {
    void *data;              // Array de elementos
    size_t element_size;     // Tamanho de cada elemento
    size_t capacity;         // Capacidade do array
    size_t size;             // Número de elementos
    size_t head;             // Índice do primeiro elemento
    size_t tail;             // Índice da próxima posição livre
    DestroyFn destroy;       // Função de destruição
    QueueType type;          // Tipo da fila
};
```

**Implementação Circular Buffer**:
- Usar módulo para wrap-around: `(index + 1) % capacity`
- Head e tail se movem circularmente
- Fila cheia quando: `(tail + 1) % capacity == head`
- Redimensionar por fator de 2 quando cheio

**Pseudocódigo** (Cormen et al., 2009, p. 235):
```
ENQUEUE(Q, x):
    Q[Q.tail] = x
    if Q.tail == Q.length - 1
        Q.tail = 0
    else Q.tail = Q.tail + 1

DEQUEUE(Q):
    x = Q[Q.head]
    if Q.head == Q.length - 1
        Q.head = 0
    else Q.head = Q.head + 1
    return x
```

**Tarefas**:
- [ ] Implementar `queue_create` (QUEUE_ARRAY)
- [ ] Implementar `queue_destroy`
- [ ] Implementar `queue_enqueue` com redimensionamento automático
- [ ] Implementar `queue_dequeue`
- [ ] Implementar `queue_front`
- [ ] Implementar funções auxiliares (size, is_empty, etc.)
- [ ] Implementar `queue_clone`
- [ ] Testes unitários (casos normais + edge cases)
- [ ] Testes de stress (1M+ elementos)
- [ ] Benchmarks de performance

### 2.2 Queue Linked-Based
**Prioridade**: Média
**Dependências**: common.c
**Arquivos**: `src/data_structures/queue_linked.c`

**Estrutura Interna**:
```c
typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
} QueueNode;

struct Queue {
    QueueNode *head;         // Primeiro nó
    QueueNode *tail;         // Último nó
    size_t element_size;
    size_t size;
    DestroyFn destroy;
    QueueType type;
};
```

**Tarefas**:
- [ ] Implementar versão linked
- [ ] Testes unitários
- [ ] Comparação de performance vs array-based

## 🎯 Fase 3: Stack (Semanas 3-4)

### 3.1 Stack Array-Based
**Prioridade**: Alta
**Dependências**: common.c
**Arquivos**: `src/data_structures/stack_array.c`

**Estrutura Interna**:
```c
struct Stack {
    void *data;              // Array de elementos
    size_t element_size;
    size_t capacity;
    size_t top;              // Índice do topo (size - 1)
    DestroyFn destroy;
    StackType type;
};
```

**Pseudocódigo** (Cormen et al., 2009, p. 233):
```
PUSH(S, x):
    S.top = S.top + 1
    S[S.top] = x

POP(S):
    if STACK-EMPTY(S)
        error "underflow"
    else S.top = S.top - 1
        return S[S.top + 1]
```

**Tarefas**:
- [ ] Implementar todas as operações de stack.h
- [ ] Implementar `stack_reverse`
- [ ] Testes unitários
- [ ] Testes de aplicações clássicas (parênteses balanceados, RPN, etc.)

### 3.2 Stack Linked-Based
**Prioridade**: Média
**Arquivos**: `src/data_structures/stack_linked.c`

**Tarefas**:
- [ ] Implementar versão linked (push/pop sempre no head)
- [ ] Testes e benchmarks

## 🎯 Fase 4: LinkedList (Semanas 4-6)

### 4.1 Doubly Linked List
**Prioridade**: Alta
**Dependências**: common.c
**Arquivos**: `src/data_structures/linked_list.c`

**Estrutura Interna**:
```c
typedef struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

struct LinkedList {
    ListNode *head;
    ListNode *tail;
    size_t element_size;
    size_t size;
    ListType type;
    DestroyFn destroy;
};
```

**Pseudocódigo** (Cormen et al., 2009, p. 238):
```
LIST-INSERT(L, x):
    x.next = L.head
    if L.head ≠ NIL
        L.head.prev = x
    L.head = x
    x.prev = NIL

LIST-DELETE(L, x):
    if x.prev ≠ NIL
        x.prev.next = x.next
    else L.head = x.next
    if x.next ≠ NIL
        x.next.prev = x.prev
```

**Tarefas**:
- [ ] Implementar operações de inserção (front, back, at, after, before)
- [ ] Implementar operações de remoção
- [ ] Implementar operações de acesso e busca
- [ ] Implementar iteradores
- [ ] Implementar `list_reverse` (trocar prev/next de todos os nós)
- [ ] Implementar `list_sort` (merge sort)
  - Referência: Knuth Vol 3, Section 5.2.4
- [ ] Testes unitários extensivos
- [ ] Testes de iteração

### 4.2 Singly Linked List
**Prioridade**: Média

**Tarefas**:
- [ ] Suportar LIST_SINGLY no mesmo arquivo
- [ ] Adaptar operações para single link

### 4.3 Circular Linked List
**Prioridade**: Baixa

**Tarefas**:
- [ ] Suportar LIST_CIRCULAR
- [ ] Tail->next aponta para head

## 🎯 Fase 5: ArrayList (Semanas 6-7)

### 5.1 ArrayList com Crescimento Dinâmico
**Prioridade**: Alta
**Dependências**: common.c
**Arquivos**: `src/data_structures/array_list.c`

**Estrutura Interna**:
```c
struct ArrayList {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
    GrowthStrategy growth;
    DestroyFn destroy;
};
```

**Estratégias de Crescimento**:
1. **GROWTH_DOUBLE**: `new_capacity = capacity * 2`
   - Análise amortizada: O(1) por inserção
   - Série geométrica: 1 + 2 + 4 + ... + n = 2n - 1
2. **GROWTH_1_5**: `new_capacity = capacity * 1.5`
   - Menos desperdício de memória
   - Ainda O(1) amortizado
3. **GROWTH_FIXED**: `new_capacity = capacity + FIXED_INCREMENT`

**Tarefas**:
- [ ] Implementar crescimento automático
- [ ] Implementar todas as operações básicas
- [ ] Implementar `arraylist_binary_search`
  - Pseudocódigo: Cormen p. 799
- [ ] Implementar `arraylist_sort` (wrapper para qsort)
- [ ] Implementar `arraylist_shrink_to_fit`
- [ ] Implementar `arraylist_reserve`
- [ ] Testes unitários
- [ ] Testes de análise amortizada (medir realocações)
- [ ] Benchmarks vs LinkedList

## 🎯 Fase 6: Validação e Documentação (Semana 8)

### 6.1 Testes de Integração
- [ ] Testar interoperabilidade entre estruturas
- [ ] Exemplo: Queue de Stacks, Stack de ArrayLists, etc.

### 6.2 Benchmarks Comparativos
**Arquivo**: `benchmarks/compare_structures.c`

**Métricas**:
- Tempo de inserção (10k, 100k, 1M elementos)
- Tempo de remoção
- Tempo de acesso
- Uso de memória
- Análise de cache (cache misses)

**Comparações**:
- Queue Array vs Queue Linked
- Stack Array vs Stack Linked
- LinkedList vs ArrayList para diferentes workloads

### 6.3 Documentação
- [ ] README com exemplos de uso
- [ ] Documentação de API completa (Doxygen)
- [ ] Guia de escolha de estrutura de dados
- [ ] Análise de complexidade empírica vs teórica

## 📊 Critérios de Validação

Cada estrutura de dados deve passar por:

1. **Correção**:
   - ✅ Todos os testes unitários passam
   - ✅ Sem memory leaks (Valgrind)
   - ✅ Sem undefined behavior (UBSan)

2. **Performance**:
   - ✅ Complexidade empírica corresponde à teórica
   - ✅ Benchmarks dentro de ±10% da implementação de referência

3. **Qualidade de Código**:
   - ✅ Sem warnings em -Wall -Wextra -Wpedantic
   - ✅ Formatação consistente
   - ✅ Documentação completa

4. **Validação Acadêmica**:
   - ✅ Implementação segue pseudocódigo de referência
   - ✅ Análise de complexidade documentada
   - ✅ Referências bibliográficas citadas

## 🔧 Ferramentas e Ambiente

- **Compilador**: GCC/Clang com C11
- **Build System**: CMake 3.10+
- **Testes**: Unity ou similar
- **Análise**: Valgrind, AddressSanitizer, UBSan
- **Profiling**: gprof, perf
- **Cobertura**: gcov/lcov
- **Documentação**: Doxygen

## 📚 Referências Principais

1. **Cormen et al.** - Introduction to Algorithms (3rd ed.)
   - Capítulos 10 (Estruturas Básicas), 17 (Análise Amortizada)

2. **Knuth** - The Art of Computer Programming, Vol 1
   - Seção 2.2 (Listas Lineares)

3. **Sedgewick & Wayne** - Algorithms (4th ed.)
   - Seção 1.3 (Bags, Queues, Stacks)

4. **Goodrich et al.** - Data Structures and Algorithms in C++
   - Capítulo 7 (Arrays e Listas)

## ✅ Checklist de Próximos Passos

- [ ] Implementar `common.c` e testar
- [ ] Configurar framework de testes
- [ ] Implementar Queue Array-based
- [ ] Implementar Stack Array-based
- [ ] Implementar LinkedList Doubly
- [ ] Implementar ArrayList
- [ ] Executar bateria completa de testes
- [ ] Gerar relatórios de benchmark
- [ ] Documentar resultados

---

**Última atualização**: 2025-02-12
