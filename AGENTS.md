---
description: "Project-specific rules, standards, and workflows for implementing data structures and algorithms in C11. Read .opencode/AGENTS.md for agent protocols."
---

# AGENTS.md - Agent Instructions for Algoritmos e Heurísticas

This file provides guidelines for AI coding agents working on this C-based academic data structures and algorithms library.

**IMPORTANT:** Before starting, read `.opencode/AGENTS.md` for Itzamna workflows and protocols.

## T0 Rules (Inviolable - MUST Follow)

### Memory Management (CRITICAL)

**ALWAYS update `MEMORY.md` when:**
1. ✅ **Feature complete**: New data structure fully implemented and tested
2. ✅ **Release**: Phase complete (1A/1B/1C) or version published
3. ✅ **Hotfix**: Critical bug fix applied
4. ✅ **Breaking change**: API-breaking modification introduced

**Protocol:**
1. At end of implementation, **propose update** to `MEMORY.md`
2. Show **diff** of what will be added (never delete history)
3. **Wait for explicit approval** before writing
4. Update includes: features, metrics (LOC, tests), references
5. **NEVER modify MEMORY.md without approval** (T0 violation)

**Hierarchy:**
- `MEMORY.md` (root) = Long-term memory (features implemented, releases, hotfixes)
- `.itzamna/memory.md` = Short-term memory (current state, next steps, recent sessions)

Both are **append-only**, but `MEMORY.md` updates are **mandatory** for releases/features.

---

## Project Overview

Academic library implementing efficient data structures, classical algorithms, heuristics, and meta-heuristics in C11. All implementations follow rigorous academic principles with references to papers and recognized pseudocode.

**Language:** C11 (ISO/IEC 9899:2011)  
**Build System:** CMake 3.10+  
**Test Framework:** Custom lightweight framework (see tests/data_structures/test_*.c)

## Build Commands

### Standard Build
```bash
# From project root
mkdir -p build && cd build
cmake ..
cmake --build .
```

### Build from CLion build directory
```bash
cd cmake-build-debug
cmake --build .
```

### Running Tests

**All tests:**
```bash
# From project root after building
cd cmake-build-debug
ctest --output-on-failure

# Or manually run each test executable
./test_common
./test_queue
./test_stack
./test_linked_list
./test_array_list
./test_hash_table
./test_binary_tree
```

**Single test:**
```bash
# From cmake-build-debug directory
./test_<structure_name>

# Example:
./test_queue
./test_stack
./test_hash_table
```

### Build Options

**With Address Sanitizer:**
```bash
cmake -DENABLE_ASAN=ON ..
cmake --build .
```

**With Undefined Behavior Sanitizer:**
```bash
cmake -DENABLE_UBSAN=ON ..
cmake --build .
```

**With Code Coverage:**
```bash
cmake -DENABLE_COVERAGE=ON ..
cmake --build .
```

**Release Build:**
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Code Style Guidelines

### File Structure

**Headers (include/data_structures/):**
- Start with Doxygen file header documenting purpose, complexity, and references
- Use include guards: `#ifndef STRUCTURE_NAME_H` / `#define STRUCTURE_NAME_H` / `#endif`
- Include only necessary dependencies: `common.h`, `<stdbool.h>`, `<stddef.h>`
- Opaque pointers for all data structures (hide implementation details)
- Document all public functions with Doxygen comments including complexity analysis

**Implementation files (src/data_structures/):**
- Start with Doxygen file header with academic references
- Define internal structures after includes
- Group functions logically with section comments (80 = signs)
- Private/static helper functions before public functions

**Test files (tests/data_structures/):**
- Use custom test macros: `TEST(name)`, `ASSERT(condition)`, `ASSERT_EQ`, etc.
- Group tests by functionality
- Include edge cases: NULL pointers, empty structures, boundary conditions
- Test both array and linked variants where applicable

### Naming Conventions

**Functions:**
- `structure_operation` format (e.g., `queue_enqueue`, `stack_push`, `list_insert`)
- Private/static functions: `structure_operation_detail` (e.g., `queue_resize_array`)

**Types:**
- PascalCase for opaque structs: `Queue`, `Stack`, `LinkedList`, `ArrayList`
- Snake_case for internal structs: `QueueNode`, `StackNode`
- PascalCase for enums and their values: `QueueType`, `QUEUE_ARRAY`, `QUEUE_LINKED`

**Variables:**
- snake_case for all variables
- Descriptive names: `element_size`, `capacity`, `head`, `tail`, `front`, `rear`

**Constants:**
- UPPER_SNAKE_CASE for macros: `INITIAL_CAPACITY`, `LOAD_FACTOR_THRESHOLD`

### Comments and Documentation

**Required Doxygen tags:**
```c
/**
 * @file filename.c/h
 * @brief One-line description
 *
 * Detailed description with references.
 *
 * Referências:
 * - Cormen et al. (2009), Chapter X.Y
 * - Paper citations
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */
```

**Function documentation:**
```c
/**
 * @brief Short description
 *
 * Detailed description if needed.
 *
 * @param param1 Description
 * @param param2 Description
 * @return Return value description
 *
 * Complexidade: O(n) or O(1) amortizado
 */
```

**IMPORTANT:** Do NOT add inline comments unless they explain complex logic or academic algorithms. Code should be self-documenting through clear naming.

### Type Safety and Generics

**Always use:**
- `void*` for generic data storage
- `size_t` for sizes, capacities, and array indices
- `bool` from `<stdbool.h>` for boolean values
- Function pointers for custom operations: `CompareFn`, `CopyFn`, `DestroyFn`, `PrintFn`, `HashFn`

**Generic data pattern:**
```c
typedef struct Structure Structure;

Structure* structure_create(size_t element_size, /* params */, DestroyFn destroy);
DataStructureError structure_operation(Structure *s, const void *data);
```

### Error Handling

**Return values:**
- `DataStructureError` enum for operations that can fail
- `NULL` for creation functions that fail
- `bool` for simple success/failure checks

**Always check:**
- NULL pointer arguments at function entry
- Memory allocation failures (`malloc`, `realloc`, `calloc`)
- Boundary conditions (empty, full, invalid indices)

**Pattern:**
```c
DataStructureError function(Structure *s, const void *data) {
    if (s == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }
    
    void *new_mem = malloc(size);
    if (new_mem == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }
    
    return DS_SUCCESS;
}
```

### Memory Management

**Rules:**
1. Every `malloc`/`calloc` must have a corresponding `free`
2. Use `destroy` function pointer for custom cleanup
3. In error paths, free all allocated memory before returning
4. NULL-check before calling `destroy` function
5. Set pointers to NULL after freeing

**Cleanup pattern:**
```c
void structure_destroy(Structure *s) {
    if (s == NULL) return;
    
    if (s->destroy != NULL) {
        // Call destroy on each element
    }
    
    free(s->internal_data);
    free(s);
}
```

## Implementation Patterns

### Structure Types

**Prefer dual implementations where beneficial:**
- Queue: `QUEUE_ARRAY` (circular buffer) and `QUEUE_LINKED`
- Stack: `STACK_ARRAY` and `STACK_LINKED`
- Hash Table: Chaining and Open Addressing

### Academic References

**Always include in file headers:**
- Cormen et al. "Introduction to Algorithms" (specific chapter)
- Knuth "The Art of Computer Programming" (specific volume/section)
- Original papers for specialized algorithms
- Complexity analysis: time and space

### Testing Standards

**Every implementation must have:**
1. Basic operations test (create, insert, remove, search)
2. Edge cases test (NULL, empty, single element)
3. Stress test (large datasets, many operations)
4. Memory test (no leaks when run with sanitizers)
5. Type-specific tests (int, float, double, string)

**Test macro usage:**
```c
TEST(descriptive_test_name) {
    // Setup
    Structure *s = structure_create(...);
    
    // Execute
    DataStructureError err = structure_operation(s, data);
    
    // Verify
    ASSERT_EQ(err, DS_SUCCESS);
    ASSERT_EQ(structure_size(s), expected_size);
    
    // Cleanup
    structure_destroy(s);
}
```

## Git Workflow

**Never commit:**
- Build artifacts (cmake-build-*/, *.exe, *.o, *.a)
- IDE files (.idea/, .vscode/)
- Test outputs, logs, coverage files
- Secrets or credentials (N/A for this project)

**Always commit:**
- Source files (*.c, *.h)
- CMakeLists.txt changes
- Documentation updates
- Test files

## Current Project Status

**Phase 1A - COMPLETE:** Queue, Stack, LinkedList, ArrayList  
**Phase 1B - IN PROGRESS:** HashTable ✓, BinaryTree ✓, BST, Heap, Graph  
**Phase 1C - PLANNED:** AVL Tree, Priority Queue, Trie, Union-Find

**When implementing new structures:**
1. Read docs/DATA_STRUCTURE_GUIDE.md and docs/references/
2. Create header in include/data_structures/
3. Implement in src/data_structures/
4. Write comprehensive tests in tests/data_structures/
5. Update CMakeLists.txt to include new files
6. Run all tests to ensure no regressions
7. Update progress in README.md roadmap section

## Additional Notes

- This is an **academic project** - prioritize correctness and clarity over premature optimization
- Include complexity analysis (Big-O) in all function documentation
- Follow existing code patterns - consistency is critical
- When in doubt, check existing implementations (queue.c, stack.c, etc.)
- Memory safety is paramount - always run with sanitizers during development
