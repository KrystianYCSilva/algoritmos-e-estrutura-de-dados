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
# BIBLIOTECA DE ALGORITMOS
# ============================================================================

set(ALGORITHMS_SOURCES
    # Fase 2A: Ordenacao e Busca
    src/algorithms/sorting.c            # ✓ 10 algoritmos de ordenacao
    src/algorithms/searching.c          # ✓ 6 algoritmos de busca

    # Fase 2B: Algoritmos de Grafos
    src/algorithms/graph_algorithms.c   # ✓ Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim

    # Fase 2C: Strings, DP, Greedy, Numericos
    src/algorithms/string_matching.c    # ✓ Naive, KMP, Rabin-Karp, Boyer-Moore
    src/algorithms/dynamic_programming.c # ✓ Fibonacci, LCS, Knapsack, Edit Distance, LIS, Rod Cutting, Matrix Chain, Coin Change
    src/algorithms/greedy.c             # ✓ Activity Selection, Huffman, Fractional Knapsack
    src/algorithms/numerical.c          # ✓ GCD, Extended GCD, Fast Exp, Sieve

    # Fase 2D: Divisao e Conquista, Backtracking
    src/algorithms/divide_conquer.c     # ✓ Strassen, Closest Pair, Karatsuba, Max Subarray, Quick Select
    src/algorithms/backtracking.c       # ✓ N-Queens, Subset Sum, Permutations, Graph Coloring
)

add_library(algorithms STATIC ${ALGORITHMS_SOURCES})
target_link_libraries(algorithms data_structures)

# ============================================================================
# BIBLIOTECA DE OTIMIZAÇÃO (HEURÍSTICAS E META-HEURÍSTICAS)
# ============================================================================

set(OPTIMIZATION_SOURCES
    # Infraestrutura
    src/optimization/common.c               # ✓ OptSolution, OptResult, RNG

    # Benchmarks
    src/optimization/benchmarks/tsp.c       # ✓ TSP instances, tour_cost, neighbors
    src/optimization/benchmarks/continuous.c # ✓ Sphere, Rastrigin, Rosenbrock, Ackley, Schwefel

    # Phase 3A - Heurísticas Clássicas
    src/optimization/heuristics/hill_climbing.c          # ✓ Steepest, First Improvement, Random Restart, Stochastic
    src/optimization/metaheuristics/simulated_annealing.c # ✓ 4 cooling schedules, reheating, auto-calibrate
    src/optimization/metaheuristics/tabu_search.c         # ✓ Aspiration, diversification, intensification, reactive
    src/optimization/metaheuristics/genetic_algorithm.c   # ✓ Tournament/Roulette/Rank, OX/PMX/BLX, adaptive

    # Phase 3B - Heurísticas Avançadas
    src/optimization/metaheuristics/ils.c                 # ✓ 4 acceptance criteria (better/always/SA-like/restart)
    src/optimization/metaheuristics/grasp.c               # ✓ RCL construction, reactive GRASP, builtin constructors
    src/optimization/metaheuristics/pso.c                 # ✓ 3 inertia types (constant/linear/constriction)
    src/optimization/metaheuristics/aco.c                 # ✓ 3 variants (AS/Elitist/MMAS)
)

add_library(optimization STATIC ${OPTIMIZATION_SOURCES})
target_link_libraries(optimization m)

# ============================================================================
# EXECUTÁVEL PRINCIPAL
# ============================================================================

add_executable(algoritmos_heuristicos main.c)

# Linkar com bibliotecas
target_link_libraries(algoritmos_heuristicos optimization algorithms data_structures m)

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

    # ================================================================
    # TESTES DE ALGORITMOS
    # ================================================================

    # Teste de sorting
    add_executable(test_sorting tests/algorithms/test_sorting.c)
    target_link_libraries(test_sorting algorithms data_structures m)
    add_test(NAME SortingTests COMMAND test_sorting)

    # Teste de searching
    add_executable(test_searching tests/algorithms/test_searching.c)
    target_link_libraries(test_searching algorithms data_structures m)
    add_test(NAME SearchingTests COMMAND test_searching)

    # Teste de graph algorithms
    add_executable(test_graph_algorithms tests/algorithms/test_graph_algorithms.c)
    target_link_libraries(test_graph_algorithms algorithms data_structures m)
    add_test(NAME GraphAlgorithmsTests COMMAND test_graph_algorithms)

    # Teste de string matching
    add_executable(test_string_matching tests/algorithms/test_string_matching.c)
    target_link_libraries(test_string_matching algorithms data_structures m)
    add_test(NAME StringMatchingTests COMMAND test_string_matching)

    # Teste de dynamic programming
    add_executable(test_dynamic_programming tests/algorithms/test_dynamic_programming.c)
    target_link_libraries(test_dynamic_programming algorithms data_structures m)
    add_test(NAME DynamicProgrammingTests COMMAND test_dynamic_programming)

    # Teste de greedy algorithms
    add_executable(test_greedy tests/algorithms/test_greedy.c)
    target_link_libraries(test_greedy algorithms data_structures m)
    add_test(NAME GreedyTests COMMAND test_greedy)

    # Teste de numerical algorithms
    add_executable(test_numerical tests/algorithms/test_numerical.c)
    target_link_libraries(test_numerical algorithms data_structures m)
    add_test(NAME NumericalTests COMMAND test_numerical)

    # Teste de divide and conquer
    add_executable(test_divide_conquer tests/algorithms/test_divide_conquer.c)
    target_link_libraries(test_divide_conquer algorithms data_structures m)
    add_test(NAME DivideConquerTests COMMAND test_divide_conquer)

    # Teste de backtracking
    add_executable(test_backtracking tests/algorithms/test_backtracking.c)
    target_link_libraries(test_backtracking algorithms data_structures m)
    add_test(NAME BacktrackingTests COMMAND test_backtracking)

    # ================================================================
    # TESTES DE OTIMIZAÇÃO (PHASE 3)
    # ================================================================

    # Teste de benchmarks (TSP + Continuous)
    add_executable(test_benchmarks tests/optimization/test_benchmarks.c)
    target_link_libraries(test_benchmarks optimization m)
    add_test(NAME BenchmarkTests COMMAND test_benchmarks)

    # Teste de hill climbing
    add_executable(test_hill_climbing tests/optimization/test_hill_climbing.c)
    target_link_libraries(test_hill_climbing optimization m)
    add_test(NAME HillClimbingTests COMMAND test_hill_climbing)

    # Teste de simulated annealing
    add_executable(test_simulated_annealing tests/optimization/test_simulated_annealing.c)
    target_link_libraries(test_simulated_annealing optimization m)
    add_test(NAME SimulatedAnnealingTests COMMAND test_simulated_annealing)

    # Teste de tabu search
    add_executable(test_tabu_search tests/optimization/test_tabu_search.c)
    target_link_libraries(test_tabu_search optimization m)
    add_test(NAME TabuSearchTests COMMAND test_tabu_search)

    # Teste de genetic algorithm
    add_executable(test_genetic_algorithm tests/optimization/test_genetic_algorithm.c)
    target_link_libraries(test_genetic_algorithm optimization m)
    add_test(NAME GeneticAlgorithmTests COMMAND test_genetic_algorithm)

    # Phase 3B - Heurísticas Avançadas

    # Teste de ILS
    add_executable(test_ils tests/optimization/test_ils.c)
    target_link_libraries(test_ils optimization m)
    add_test(NAME ILSTests COMMAND test_ils)

    # Teste de GRASP
    add_executable(test_grasp tests/optimization/test_grasp.c)
    target_link_libraries(test_grasp optimization m)
    add_test(NAME GRASPTests COMMAND test_grasp)

    # Teste de PSO
    add_executable(test_pso tests/optimization/test_pso.c)
    target_link_libraries(test_pso optimization m)
    add_test(NAME PSOTests COMMAND test_pso)

    # Teste de ACO
    add_executable(test_aco tests/optimization/test_aco.c)
    target_link_libraries(test_aco optimization m)
    add_test(NAME ACOTests COMMAND test_aco)
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


