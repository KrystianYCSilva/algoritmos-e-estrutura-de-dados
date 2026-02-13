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

**TODAS AS FASES COMPLETAS** - Projeto v0.6.0-alpha.

- ✅ **Fase 1A COMPLETA**: Queue, Stack, LinkedList, ArrayList, Common (2.660 linhas, 132 testes)
- ✅ **Fase 1B COMPLETA**: HashTable, BinaryTree, BST, Heap, Graph (4.410 linhas, 117 testes)
- ✅ **Fase 1C COMPLETA**: AVL Tree, PriorityQueue, Trie, UnionFind (1.507 linhas, 59 testes)
- ✅ **Fase 2 COMPLETA**: ~45 algoritmos em 9 categorias (6.000+ linhas, 199 testes)
- ✅ **Fase 3A COMPLETA**: Hill Climbing, SA, Tabu Search, GA + benchmarks (4.500 linhas, 91 testes)
- ✅ **Fase 3B COMPLETA**: ILS, GRASP, PSO, ACO (2.500 linhas, 41 testes)
- ✅ **Fase 3C COMPLETA**: DE, VNS, Memetic, LNS/ALNS (1.515 linhas, 40 testes)

**Total**: 14 estruturas + ~45 algoritmos + 12 heurísticas + 2 benchmarks, ~23.000+ linhas, ~679 testes

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
- [x] Completar Fase 1B (HashTable, BinaryTree, BST, Heap, Graph)
- [x] Completar Fase 1C (AVL, PriorityQueue, Trie, UnionFind)
- [x] Completar Fase 2 (~45 algoritmos fundamentais)
- [x] Completar Fase 3A (Hill Climbing, SA, Tabu Search, GA + benchmarks)
- [x] Completar Fase 3B (ILS, GRASP, PSO, ACO)
- [x] Completar Fase 3C (DE, VNS, Memetic, LNS/ALNS)
- [ ] **Potenciais extensões**: Nice-to-Have algorithms (38 listados no ALGORITHM_CATALOG.md)
- [ ] **Potenciais benchmarks**: VRP, Knapsack variants, Scheduling

---

## Sessoes

| # | Data | Nivel | Resumo |
|---|------|-------|--------|
| 1 | 2025-02-12 | K2 | Setup Itzamna: consolidou documentação (PROJECT_ROADMAP.md), preencheu .context/, criou AGENTS.md com regras C11, adicionou frontmatter em todos .md |
| 2 | 2025-02-12 | K3 | Fase 1A completa: Queue, Stack, LinkedList, ArrayList, Common (132 testes) |
| 3 | 2025-02-12 | K3 | Fases 1B+1C completas: HashTable, BinaryTree, BST, Heap, Graph, AVL, PQ, Trie, UnionFind (176 testes) |
| 4 | 2026-02-12 | K3 | Fase 2 completa: ~45 algoritmos em 9 categorias (199 testes) |
| 5 | 2026-02-12 | K3 | Fase 3A completa: HC, SA, TS, GA + TSP/Continuous benchmarks (91 testes) |
| 6 | 2026-02-13 | K3 | Fase 3B completa: ILS, GRASP, PSO, ACO (41 testes) |
| 7 | 2026-02-13 | K2 | Fase 3C documentação: código já implementado (DE, VNS, Memetic, LNS/ALNS), 40 testes verificados 100%, atualização de MEMORY.md, IMPLEMENTATION_PLAN, PROJECT_ROADMAP, ALGORITHM_CATALOG, .itzamna/memory, USAGE_EXAMPLES |

---

*Ultima atualizacao: 2026-02-13.*
