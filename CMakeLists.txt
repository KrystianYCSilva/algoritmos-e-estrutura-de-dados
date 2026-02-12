cmake_minimum_required(VERSION 3.10)
project(algoritmos_heuristicos C)

# ============================================================================
# CONFIGURAÇÕES DO COMPILADOR
# ============================================================================

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Flags de compilação rigorosas
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Opções para análise de memória e sanitizers
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if(ENABLE_UBSAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# ============================================================================
# DIRETÓRIOS DE INCLUDE
# ============================================================================

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/data_structures)

# ============================================================================
# BIBLIOTECA DE ESTRUTURAS DE DADOS
# ============================================================================

set(DATA_STRUCTURES_SOURCES
    # Common utilities ✓ IMPLEMENTADO
    src/data_structures/common.c

    # Fase 1A: Lineares ✅ COMPLETO
    src/data_structures/queue.c        # ✓ IMPLEMENTADO (array + linked)
    src/data_structures/stack.c        # ✓ IMPLEMENTADO (array + linked)
    src/data_structures/linked_list.c  # ✓ IMPLEMENTADO (singly/doubly/circular)
    src/data_structures/array_list.c   # ✓ IMPLEMENTADO (dynamic array)
    # src/data_structures/stack_array.c
    # src/data_structures/stack_linked.c
    # src/data_structures/linked_list.c
    # src/data_structures/array_list.c

    # Fase 1B: Hash e Árvores
    src/data_structures/hash_table.c    # ✓ IMPLEMENTADO (chaining + open addressing)
    src/data_structures/binary_tree.c   # ✓ IMPLEMENTADO (travessias + propriedades)
    src/data_structures/bst.c           # ✓ IMPLEMENTADO (BST completa)
    src/data_structures/heap.c          # ✓ IMPLEMENTADO (binary heap)
    src/data_structures/graph.c         # ✓ IMPLEMENTADO (adj list + matrix, BFS/DFS)

    # Fase 1C: Balanceadas e Especializadas
    src/data_structures/avl_tree.c      # ✓ IMPLEMENTADO (AVL auto-balanceada)
    src/data_structures/priority_queue.c # ✓ IMPLEMENTADO (sobre heap)
    src/data_structures/trie.c          # ✓ IMPLEMENTADO (prefix tree)
    src/data_structures/union_find.c    # ✓ IMPLEMENTADO (disjoint set)
)

# Criar biblioteca estática com estruturas de dados
add_library(data_structures STATIC ${DATA_STRUCTURES_SOURCES})

# ============================================================================
# EXECUTÁVEL PRINCIPAL
# ============================================================================

add_executable(algoritmos_heuristicos main.c)

# Linkar com a biblioteca de estruturas de dados
target_link_libraries(algoritmos_heuristicos data_structures)

# ============================================================================
# TESTES (OPCIONAL - UNITY FRAMEWORK)
# ============================================================================

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()

    # Teste do common.c
    add_executable(test_common tests/data_structures/test_common.c)
    target_link_libraries(test_common data_structures)
    add_test(NAME CommonTests COMMAND test_common)

    # Teste do queue.c
    add_executable(test_queue tests/data_structures/test_queue.c)
    target_link_libraries(test_queue data_structures)
    add_test(NAME QueueTests COMMAND test_queue)

    # Teste do stack.c
    add_executable(test_stack tests/data_structures/test_stack.c)
    target_link_libraries(test_stack data_structures)
    add_test(NAME StackTests COMMAND test_stack)

    # Teste do linked_list.c
    add_executable(test_linked_list tests/data_structures/test_linked_list.c)
    target_link_libraries(test_linked_list data_structures)
    add_test(NAME LinkedListTests COMMAND test_linked_list)

    # Teste do array_list.c
    add_executable(test_array_list tests/data_structures/test_array_list.c)
    target_link_libraries(test_array_list data_structures)
    add_test(NAME ArrayListTests COMMAND test_array_list)

    # Teste do hash_table.c
    add_executable(test_hash_table tests/data_structures/test_hash_table.c)
    target_link_libraries(test_hash_table data_structures)
    add_test(NAME HashTableTests COMMAND test_hash_table)

    # Teste do binary_tree.c
    add_executable(test_binary_tree tests/data_structures/test_binary_tree.c)
    target_link_libraries(test_binary_tree data_structures)
    add_test(NAME BinaryTreeTests COMMAND test_binary_tree)

    # Teste do bst.c
    add_executable(test_bst tests/data_structures/test_bst.c)
    target_link_libraries(test_bst data_structures)
    add_test(NAME BSTTests COMMAND test_bst)

    # Teste do heap.c
    add_executable(test_heap tests/data_structures/test_heap.c)
    target_link_libraries(test_heap data_structures)
    add_test(NAME HeapTests COMMAND test_heap)

    # Teste do avl_tree.c
    add_executable(test_avl_tree tests/data_structures/test_avl_tree.c)
    target_link_libraries(test_avl_tree data_structures m)
    add_test(NAME AVLTreeTests COMMAND test_avl_tree)

    # Teste do priority_queue.c
    add_executable(test_priority_queue tests/data_structures/test_priority_queue.c)
    target_link_libraries(test_priority_queue data_structures)
    add_test(NAME PriorityQueueTests COMMAND test_priority_queue)

    # Teste do trie.c
    add_executable(test_trie tests/data_structures/test_trie.c)
    target_link_libraries(test_trie data_structures)
    add_test(NAME TrieTests COMMAND test_trie)

    # Teste do union_find.c
    add_executable(test_union_find tests/data_structures/test_union_find.c)
    target_link_libraries(test_union_find data_structures)
    add_test(NAME UnionFindTests COMMAND test_union_find)

    # Teste do graph.c
    add_executable(test_graph tests/data_structures/test_graph.c)
    target_link_libraries(test_graph data_structures)
    add_test(NAME GraphTests COMMAND test_graph)
endif()

# ============================================================================
# EXEMPLOS
# ============================================================================

option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    # Adicionar exemplos quando forem implementados
    # add_subdirectory(examples)
endif()

# ============================================================================
# BENCHMARKS
# ============================================================================

option(BUILD_BENCHMARKS "Build benchmarks" OFF)

if(BUILD_BENCHMARKS)
    # Adicionar benchmarks quando forem implementados
    # add_subdirectory(benchmarks)
endif()

# ============================================================================
# INSTALAÇÃO (OPCIONAL)
# ============================================================================

# install(TARGETS data_structures
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib)
#
# install(DIRECTORY include/
#         DESTINATION include
#         FILES_MATCHING PATTERN "*.h")

# ============================================================================
# MENSAGENS DE CONFIGURAÇÃO
# ============================================================================

message(STATUS "")
message(STATUS "============================================")
message(STATUS "  Algoritmos e Heurísticas - Build Config")
message(STATUS "============================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Standard: C${CMAKE_C_STANDARD}")
message(STATUS "AddressSanitizer: ${ENABLE_ASAN}")
message(STATUS "UBSanitizer: ${ENABLE_UBSAN}")
message(STATUS "Code Coverage: ${ENABLE_COVERAGE}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "Build Benchmarks: ${BUILD_BENCHMARKS}")
message(STATUS "============================================")
message(STATUS "")
