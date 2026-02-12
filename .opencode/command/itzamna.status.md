---
description: "Mostra estado do sistema Itzamna: kernel, memoria, integracoes."
---

# /itzamna.status

Mostre o estado atual do sistema Itzamna para o usuario.

## Passos

1. Verifique se `.itzamna/` existe com kernel.md, constitution.md, memory.md
2. Verifique se `.itzamna/templates/` existe com os 4 templates operacionais
3. Verifique se `skills/` existe neste diretorio CLI e liste quantas skills ha
4. Detecte integracoes:
   - Hefesto: absorvido (comandos `/itzamna.skill.*`)
   - spec-kit: procure `.specify/` na raiz do projeto
5. Mostre o resultado em formato tabular

## Formato de saida

```
Itzamna Status
==============
.itzamna/:
  kernel.md:       OK | MISSING
  constitution.md: OK | MISSING
  memory.md:       OK (X bytes) | MISSING
  templates/:      4/4 OK | X/4
Skills:            X skills instaladas
Hefesto:           absorbed (built-in)
spec-kit:          linked | not found
```

## Regras

- Este comando e somente leitura (nao modifica nada)
- Se algo estiver faltando, sugira `itzamna init` para corrigir
