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

### Fase 1B - Associativas e Árvores ⏳ 40% (2/5)

**Data Início**: 2025-02-12  
**Status**: ⏳ Em andamento

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

**Métricas Fase 1B (parcial)**:
- Linhas de código: ~1.900
- Testes: 56 (100% passing)
- Pendente: BST, Heap, Graph

---

## 🚀 Releases

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

_Nenhum hotfix registrado ainda._

---

## 📊 Estatísticas Gerais

### Totais Acumulados (atualizado 2025-02-12)

```
Estruturas Completas:     7/13 (54%)
Linhas de Código:         4.560
Testes Unitários:         188
Taxa de Sucesso:          100%
Cobertura de Testes:      100%
Memory Leaks:             0
Documentação:             100%
```

### Por Fase

| Fase | Estruturas | Linhas | Testes | Status |
|------|------------|--------|--------|--------|
| 1A   | 5/5        | 2.660  | 132    | ✅ COMPLETO |
| 1B   | 2/5        | 1.900  | 56     | ⏳ 40% |
| 1C   | 0/4        | 0      | 0      | ⏳ PENDENTE |

---

## 🎓 Referências Acadêmicas Utilizadas

### Livros
- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.)
  - Ch. 6: Heapsort
  - Ch. 10: Elementary Data Structures
  - Ch. 11: Hash Tables
  - Ch. 12: Binary Search Trees
- Knuth, D. E. (1997-1998). *The Art of Computer Programming*
  - Vol 1: Fundamental Algorithms
  - Vol 3: Sorting and Searching
- Goodrich, M. T., et al. (2011). *Data Structures and Algorithms in C++*
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.)

### Pseudocódigos Implementados
✅ ENQUEUE/DEQUEUE (Cormen p. 235)  
✅ PUSH/POP (Cormen p. 233)  
✅ LIST-INSERT/DELETE (Cormen p. 238)  
✅ BINARY-SEARCH (Cormen p. 799)  
✅ CHAINED-HASH-INSERT/SEARCH (Cormen p. 258)  
✅ HASH-INSERT/SEARCH open addressing (Cormen p. 271)  
✅ INORDER/PREORDER/POSTORDER-TREE-WALK (Cormen p. 288)

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

**Imediato (Fase 1B)**:
- [ ] BST (Binary Search Tree) 🔄 PRÓXIMO
- [ ] Heap (Min/Max Binary Heap)
- [ ] Graph (Adjacency List + Matrix)

**Curto Prazo (Fase 1C)**:
- [ ] AVL Tree (auto-balanceamento)
- [ ] Priority Queue (sobre Heap)
- [ ] Trie (autocomplete)
- [ ] Union-Find (disjoint sets)

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

*Última atualização: 2025-02-12*  
*Próxima feature: BST (Binary Search Tree)*
