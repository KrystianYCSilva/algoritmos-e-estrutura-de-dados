---
description: "Protocolo de gerenciamento de memória do projeto: quando ler, quando atualizar, formato e regras."
---

# Memory Management Protocol

## 📋 Quando Ler `.itzamna/memory.md`

### OBRIGATÓRIO (K2+)
Leia **SEMPRE** antes de iniciar:
- Implementação de novas estruturas de dados
- Debugging de problemas complexos
- Refatoração de código existente
- Planejamento de próximas fases
- Revisão de código ou arquitetura

### OPCIONAL (K0/K1)
Pode pular para tarefas triviais:
- Correções de typos
- Ajustes de formatação
- Consultas rápidas de sintaxe

---

## ✍️ Quando Atualizar `.itzamna/memory.md`

### OBRIGATÓRIO
Atualize ao final de sessões que incluam:
- ✅ Completar uma estrutura de dados
- ✅ Completar uma fase (1A, 1B, 1C)
- ✅ Tomar decisões arquiteturais (ADRs)
- ✅ Mudanças significativas no roadmap
- ✅ Resolver bugs complexos que impactam design

### OPCIONAL (mas recomendado)
- Ao final de cada dia de trabalho intenso
- Após implementar features não-triviais
- Quando houver aprendizados importantes

### NÃO NECESSÁRIO
- Correções triviais
- Ajustes de documentação sem mudança de estado
- Testes individuais que passam

---

## 📝 Formato de Atualização

### 1. Estado Atual
Mantenha sempre atualizado o progresso das fases:
```markdown
## Estado atual

**Fase 1B em andamento (60% completa)**
- ✅ Fase 1A COMPLETA
- ⏳ Fase 1B (3/5): HashTable ✅, BinaryTree ✅, BST ✅, Heap 🔄 PRÓXIMO
- Total: 8/13 estruturas (62%)
```

### 2. Decisões
Adicione apenas decisões **significativas** que afetam arquitetura:
```markdown
| Data | Decisao | Justificativa |
|------|---------|---------------|
| 2025-02-13 | BST sem parent pointer | Simplifica implementação, suficiente para operações básicas |
```

❌ **NÃO adicione**:
- Decisões triviais de implementação
- Escolhas de variáveis ou nomes
- Ajustes de estilo

### 3. Próximos Passos
Use checklist clara:
```markdown
- [x] Implementar BST
- [ ] **PRÓXIMO**: Implementar Heap
- [ ] Implementar Graph
```

### 4. Sessões
Adicione resumo conciso (1-2 linhas):
```markdown
| # | Data | Nivel | Resumo |
|---|------|-------|--------|
| 2 | 2025-02-13 | K2 | Implementou BST completo (insert/delete/search), 20 testes, todos passando |
```

---

## 🔒 Regras Invioláveis

### T0 (NUNCA)
1. ❌ NUNCA modifique `.itzamna/memory.md` sem aprovação humana
2. ❌ NUNCA apague entradas existentes (append-only)
3. ❌ NUNCA modifique datas ou informações históricas

### T1 (SEMPRE)
1. ✅ SEMPRE leia memory antes de tarefas K2+
2. ✅ SEMPRE proponha atualização ao final de sessões significativas
3. ✅ SEMPRE mantenha formato consistente (markdown tables)
4. ✅ SEMPRE use formato de data YYYY-MM-DD

### T2 (RECOMENDADO)
1. 📋 Mantenha resumos concisos (máximo 2 linhas)
2. 📋 Use emojis padrão: ✅ completo, ⏳ pendente, 🔄 em andamento
3. 📋 Atualize "Estado atual" a cada marco significativo
4. 📋 Marque item **PRÓXIMO** claramente

---

## 🎯 Workflow de Atualização

### 1. Proposta
Ao final de sessão significativa:
```
📝 Proposta de atualização de memory:

Estado atual:
- Fase 1B agora 60% (3/5 estruturas)

Nova decisão:
- BST sem parent pointer (simplifica código)

Próximos passos:
- [x] BST ✅
- [ ] Heap 🔄 PRÓXIMO

Nova sessão:
| 2 | 2025-02-13 | K2 | Implementou BST: insert/delete/search, validação, 20 testes passando |

Confirma atualização? (sim/não)
```

### 2. Aprovação
Aguarde **SEMPRE** aprovação explícita do humano:
- "sim" / "confirma" / "ok" → Proceder
- "não" / "espera" → NÃO atualizar
- Sem resposta → Perguntar novamente

### 3. Execução
Após aprovação:
1. Leia `.itzamna/memory.md` atual
2. Faça append (não delete)
3. Mantenha formatação
4. Confirme escrita bem-sucedida

---

## 📊 Exemplos de Boas Atualizações

### ✅ BOM - Conciso e Informativo
```markdown
| 3 | 2025-02-14 | K2 | Completou Fase 1B (5/5): BST, Heap, Graph implementados e testados (60 testes) |
```

### ✅ BOM - Decisão Clara
```markdown
| 2025-02-14 | Graph: Adjacency List como default | Lista é O(V+E) vs Matrix O(V²), melhor para grafos esparsos |
```

### ❌ RUIM - Muito Verboso
```markdown
| 3 | 2025-02-14 | K2 | Hoje foi um dia produtivo onde implementamos o BST completo com todas as operações de insert, delete, search, min, max, successor e predecessor. Também criamos 20 testes que cobrem todos os casos possíveis e fizemos benchmarks... |
```

### ❌ RUIM - Sem Contexto
```markdown
| 3 | 2025-02-14 | K2 | Fez umas coisas no código |
```

---

## 🔄 Comandos Disponíveis

### Ler memória
```
/itzamna.memory
```
Mostra resumo: estado atual, últimas decisões, próximos passos

### Atualizar memória
```
/itzamna.memory update
```
Propõe atualização baseada na sessão, aguarda aprovação

### Verificar memória
```
/itzamna.memory check
```
Verifica se memória está desatualizada (> 3 sessões sem update)

---

## ✅ Checklist de Qualidade

Antes de propor atualização, verifique:
- [ ] Estado atual reflete progresso real?
- [ ] Decisões são arquiteturais (não triviais)?
- [ ] Próximos passos estão claros e priorizados?
- [ ] Resumo da sessão é conciso (≤ 2 linhas)?
- [ ] Data no formato YYYY-MM-DD?
- [ ] Nível correto (K0/K1/K2)?
- [ ] Não deletei informações históricas?

---

**Regra de Ouro**: Memory é **append-only**, **sempre com aprovação**, **conciso e informativo**.

*Protocolo criado: 2025-02-12*
