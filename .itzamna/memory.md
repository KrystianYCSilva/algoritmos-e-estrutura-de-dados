---
description: |
  Project episodic memory: current state, decisions, next steps, session history.
  Use when: starting any K2+ task (MANDATORY read), ending significant sessions (MANDATORY update).
---

# Memory - Estado do Projeto

> Lido pelo agente no inicio de sessoes K2+.
> Atualizado ao final de sessoes significativas.
> Append-only: nunca apague entradas.

---

## Projeto

- **Nome:** Algoritmos e Heurísticas - Biblioteca Acadêmica em C
- **Stack:** C11, CMake 3.10+, Custom Test Framework
- **Repositório:** Local (C:\Users\kryst\CLionProjects\algoritmos-heuriticos)

---

## Estado atual

**Fase 1B em andamento (40% completa)** - Implementando estruturas associativas e árvores.

- ✅ **Fase 1A COMPLETA**: Queue, Stack, LinkedList, ArrayList, Common (2.660 linhas, 132 testes)
- ⏳ **Fase 1B (2/5)**: HashTable ✅, BinaryTree ✅, BST 🔄 PRÓXIMO, Heap ⏳, Graph ⏳
- ⏳ **Fase 1C (0/4)**: AVL Tree, PriorityQueue, Trie, UnionFind (planejadas)

**Total**: 7/13 estruturas completas (54%), 4.560 linhas, 188 testes

---

## Decisoes

| Data | Decisao | Justificativa |
|------|---------|---------------|
| 2025-02 | Opaque pointers para encapsulamento | Information hiding, permite mudar implementação sem quebrar API |
| 2025-02 | Dual implementations (array/linked) para Queue/Stack | Permite escolher trade-off: performance (array) vs flexibilidade (linked) |
| 2025-02 | Custom test framework (não Unity) | Evitar dependências externas, mantém projeto auto-contido |
| 2025-02 | Doxygen + análise de complexidade obrigatória | Rigor acadêmico, documentação de qualidade para ensino |
| 2025-02-12 | Consolidar documentação em docs/PROJECT_ROADMAP.md | Reduzir redundância, único ponto de verdade para progresso |
| 2025-02-12 | Remover arquivos de progresso redundantes do root | Manter root limpo (apenas README + AGENTS.md) |

---

## Proximos passos

- [x] Completar Fase 1A (Queue, Stack, LinkedList, ArrayList)
- [x] Implementar HashTable (chaining + open addressing)
- [x] Implementar BinaryTree (travessias + propriedades)
- [ ] **PRÓXIMO**: Implementar BST (Binary Search Tree)
- [ ] Implementar Heap (Min/Max Binary Heap)
- [ ] Implementar Graph (Adjacency List + Matrix)
- [ ] Completar Fase 1B (5/5 estruturas)
- [ ] Iniciar Fase 1C (AVL, PriorityQueue, Trie, UnionFind)

---

## Sessoes

| # | Data | Nivel | Resumo |
|---|------|-------|--------|
| 1 | 2025-02-12 | K2 | Setup Itzamna: consolidou documentação (PROJECT_ROADMAP.md), preencheu .context/, criou AGENTS.md com regras C11, adicionou frontmatter em todos .md |

---

*Ultima atualizacao: 2025-02-12.*
