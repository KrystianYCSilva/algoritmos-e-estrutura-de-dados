---
description: |
  Project-specific coding rules and standards beyond CONSTITUTION.md.
  Use when: writing or reviewing code for this specific project.
---

# Project Rules & Standards

> Regras especificas DESTE projeto. Regras gerais estao em CONSTITUTION.md.

## Regras do projeto

1. **Rigor Acadêmico**: Toda implementação DEVE ter referências a literatura (Cormen, Knuth, papers)
2. **Complexidade Documentada**: Toda função pública DEVE documentar complexidade Big-O
3. **Memória Segura**: Zero memory leaks - validar com ASAN/UBSAN antes de commit
4. **Testes Obrigatórios**: Nova estrutura = novo test_*.c completo
5. **Código Auto-Documentado**: Não adicionar comentários inline desnecessários

## Padroes de codigo

**Nomenclatura:**
- Funções: `structure_operation` (snake_case)
- Tipos opacos: PascalCase (`Queue`, `Stack`)
- Structs internas: snake_case (`QueueNode`)
- Macros/constantes: UPPER_SNAKE_CASE

**Doxygen obrigatório:**
- File header com @file, @brief, referências, @author, @date
- Função pública com @param, @return, complexidade

**Padrão genérico:**
- `void*` para dados + `size_t element_size`
- Function pointers: `CompareFn`, `CopyFn`, `DestroyFn`, `PrintFn`, `HashFn`
- Error handling: `DataStructureError` enum

## Decisoes arquiteturais (ADRs)

| # | Data | Decisao | Status |
|---|------|---------|--------|
| 1 | 2025-02 | Opaque pointers para encapsulamento de structs | Aceita |
| 2 | 2025-02 | Dual implementations (array/linked) para Queue e Stack | Aceita |
| 3 | 2025-02 | Custom test framework (não Unity) para evitar deps externas | Aceita |
| 4 | 2025-02 | Doxygen para documentação + análise de complexidade | Aceita |
