/**
 * @file memetic.h
 * @brief Algoritmo Memetico (MA) - GA hibridizado com busca local
 *
 * Combina evolucao populacional (GA) com busca local intensiva.
 * Cada individuo eh refinado por busca local apos crossover/mutacao.
 *
 * Variantes de aprendizado:
 * - Lamarckiano: resultado da busca local substitui genotipo
 * - Baldwiniano: fitness melhorado pela busca local, genotipo inalterado
 *
 * Pseudocodigo (Moscato, 1989):
 *   P = [LOCAL-SEARCH(generate()) for _ in pop_size]
 *   for gen = 1 to max_gen:
 *     P_new = elites(P)
 *     while |P_new| < pop_size:
 *       p1, p2 = select(P)
 *       c1, c2 = crossover(p1, p2, p_c)
 *       mutate(c1, p_m); mutate(c2, p_m)
 *       c1 = LOCAL-SEARCH(c1); c2 = LOCAL-SEARCH(c2)
 *       P_new += [c1, c2]
 *     P = P_new
 *   return best(P)
 *
 * Referencias:
 * - Moscato, P. (1989). "On Evolution, Search, Optimization, Genetic
 *   Algorithms and Martial Arts: Towards Memetic Algorithms". Tech Report
 *   C3P 826, Caltech.
 * - Neri, F. & Cotta, C. (2012). "Memetic Algorithms and Memetic Computing
 *   Optimization: A Literature Review". Swarm and Evolutionary Computation.
 * - Krasnogor, N. & Smith, J. (2005). "A Tutorial for Competent Memetic
 *   Algorithms". IEEE Trans. Evolutionary Computation, 9(5).
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_MEMETIC_H
#define OPT_MEMETIC_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Tipo de aprendizado memetico
 */
typedef enum {
    MA_LAMARCKIAN,   /**< Genotipo = resultado da busca local */
    MA_BALDWINIAN    /**< Fitness = LS fitness, genotipo inalterado */
} MALearningType;

/**
 * @brief Metodo de selecao de pais
 */
typedef enum {
    MA_SELECT_TOURNAMENT,  /**< Tournament selection */
    MA_SELECT_ROULETTE,    /**< Roulette wheel */
    MA_SELECT_RANK         /**< Rank-based */
} MASelectionType;

/**
 * @brief Funcao de crossover para MA
 */
typedef void (*MACrossoverFn)(const void *parent1, const void *parent2,
                              void *child1, void *child2,
                              size_t size, const void *context);

/**
 * @brief Funcao de mutacao para MA
 */
typedef void (*MAMutationFn)(void *solution, size_t size,
                             double mutation_rate, const void *context);

/**
 * @brief Configuracao do Algoritmo Memetico
 */
typedef struct {
    size_t population_size;        /**< Tamanho da populacao */
    size_t max_generations;        /**< Geracoes maximas */
    double crossover_rate;         /**< Probabilidade de crossover */
    double mutation_rate;          /**< Probabilidade de mutacao */
    size_t elitism_count;          /**< Individuos preservados por elitismo */

    MASelectionType selection;     /**< Metodo de selecao */
    size_t tournament_size;        /**< k para tournament selection */

    MALearningType learning;       /**< Tipo de aprendizado (Lamarckian/Baldwinian) */

    size_t ls_iterations;          /**< Iteracoes da busca local */
    size_t ls_neighbors;           /**< Vizinhos por iteracao da busca local */
    double ls_probability;         /**< Probabilidade de aplicar LS a cada individuo (0.0-1.0) */

    bool ls_on_initial;            /**< Aplicar LS na populacao inicial */

    OptDirection direction;        /**< Minimizar ou maximizar */
    unsigned seed;                 /**< Semente RNG */
} MAConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para MA
 *
 * Defaults: pop=50, gen=200, pc=0.8, pm=0.05, elite=2,
 * tournament(k=3), Lamarckian, LS 50 iter / 10 neighbors,
 * ls_prob=1.0, ls_on_initial=true, minimize, seed=42
 *
 * @return MAConfig Configuracao padrao
 */
MAConfig ma_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa Algoritmo Memetico
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes de cada elemento
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param generate Funcao geradora
 * @param crossover Funcao de crossover
 * @param mutate Funcao de mutacao
 * @param neighbor Funcao de vizinhanca (para busca local)
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_gen * pop_size * (crossover + LS_iter * LS_neighbors))
 */
OptResult ma_run(const MAConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 GenerateFn generate,
                 MACrossoverFn crossover,
                 MAMutationFn mutate,
                 NeighborFn neighbor,
                 const void *context);

#endif /* OPT_MEMETIC_H */
