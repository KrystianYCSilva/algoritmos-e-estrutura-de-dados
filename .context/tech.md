---
description: |
  Tech stack and architecture decisions.
  Use when: you need to understand the technologies, patterns, and architectural choices.
---

# Tech Stack & Architecture

## Stack

| Camada | Tecnologia | Versão |
|--------|------------|--------|
| Linguagem | C | C11 (ISO/IEC 9899:2011) |
| Build System | CMake | 3.10+ |
| Compilador | GCC/Clang | Compatível com C11 |
| Testes | Custom Framework | Macros TEST/ASSERT (tests/data_structures/) |
| Sanitizers | ASan/UBSan | Optional (ENABLE_ASAN, ENABLE_UBSAN) |
| Coverage | gcov/lcov | Optional (ENABLE_COVERAGE) |

## Arquitetura

**Biblioteca estática modular**
- Headers públicos em `include/data_structures/`
- Implementações em `src/data_structures/`
- Estruturas opacas (opaque pointers) para encapsulamento
- Tipos genéricos via `void*` + function pointers (`CompareFn`, `CopyFn`, `DestroyFn`)

## Estrutura de diretorios

```
algoritmos-heuristicos/
├── include/data_structures/  # Headers públicos
├── src/data_structures/       # Implementações
├── tests/data_structures/     # Testes unitários
├── examples/                  # Exemplos de uso
├── benchmarks/                # Benchmarks de performance
├── docs/                      # Documentação e referências
└── CMakeLists.txt
```

## Dependencias criticas

**Nenhuma dependência externa** - apenas stdlib C11:
- `<stdlib.h>` - malloc, free
- `<string.h>` - memcpy, strcmp
- `<stdbool.h>` - bool type
- `<stddef.h>` - size_t
- `<math.h>` - fabs (para comparação de floats)
