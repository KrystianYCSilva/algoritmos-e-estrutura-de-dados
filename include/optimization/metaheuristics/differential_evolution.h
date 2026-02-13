/**
 * @file differential_evolution.h
 * @brief Differential Evolution (DE) para otimizacao continua
 *
 * Implementa DE classico com multiplas estrategias de mutacao:
 * - DE/rand/1/bin: mutacao com 3 vetores aleatorios
 * - DE/best/1/bin: mutacao dirigida pelo melhor individuo
 * - DE/current-to-best/1/bin: combinacao do atual com melhor
 * - DE/rand/2/bin: mutacao com 5 vetores aleatorios
 * - DE/best/2/bin: mutacao dirigida com 4 vetores de diferenca
 *
 * Pseudocodigo (Storn & Price, 1997):
 *   P = INITIALIZE-POPULATION(NP, D)
 *   EVALUATE(P)
 *   for gen = 1 to max_gen:
 *     for i = 1 to NP:
 *       v = MUTATION(P, i, strategy)
 *       u = CROSSOVER(P[i], v, CR)
 *       if f(u) <= f(P[i]): P[i] = u
 *   return best(P)
 *
 * Referencias:
 * - Storn, R. & Price, K. (1997). "Differential Evolution - A Simple and
 *   Efficient Heuristic for Global Optimization over Continuous Spaces".
 *   J. Global Optimization, 11(4), 341-359.
 * - Price, K., Storn, R. & Lampinen, J. (2005). Differential Evolution:
 *   A Practical Approach to Global Optimization. Springer.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_DIFFERENTIAL_EVOLUTION_H
#define OPT_DIFFERENTIAL_EVOLUTION_H

#include "optimization/common.h"
#include <stddef.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Estrategia de mutacao do DE
 *
 * Convencao de nomes: DE/base/num_diffs/crossover
 */
typedef enum {
    DE_RAND_1,            /**< v = x_r1 + F*(x_r2 - x_r3) */
    DE_BEST_1,            /**< v = x_best + F*(x_r1 - x_r2) */
    DE_CURRENT_TO_BEST_1, /**< v = x_i + F*(x_best - x_i) + F*(x_r1 - x_r2) */
    DE_RAND_2,            /**< v = x_r1 + F*(x_r2 - x_r3) + F*(x_r4 - x_r5) */
    DE_BEST_2             /**< v = x_best + F*(x_r1 - x_r2) + F*(x_r3 - x_r4) */
} DEStrategy;

/**
 * @brief Configuracao do Differential Evolution
 */
typedef struct {
    size_t population_size;   /**< Tamanho da populacao (NP), tipico: 5*D a 10*D */
    size_t max_generations;   /**< Geracoes maximas */
    double F;                 /**< Fator de escala (scaling factor), tipico: 0.5-0.9 */
    double CR;                /**< Taxa de crossover (crossover rate), tipico: 0.1-1.0 */

    DEStrategy strategy;      /**< Estrategia de mutacao */

    double lower_bound;       /**< Limite inferior do dominio */
    double upper_bound;       /**< Limite superior do dominio */

    OptDirection direction;   /**< Minimizar ou maximizar */
    unsigned seed;            /**< Semente RNG */
} DEConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para DE
 *
 * Defaults: pop=50, gen=1000, F=0.8, CR=0.9, DE/rand/1,
 * bounds=[-5.12, 5.12], minimize, seed=42
 *
 * @return DEConfig Configuracao padrao
 */
DEConfig de_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa Differential Evolution
 *
 * @param config Configuracao do algoritmo
 * @param solution_size Numero de dimensoes (D)
 * @param objective Funcao objetivo
 * @param context Contexto do problema (ContinuousInstance*, etc.)
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_gen * pop_size * D)
 */
OptResult de_run(const DEConfig *config,
                 size_t solution_size,
                 ObjectiveFn objective,
                 const void *context);

#endif /* OPT_DIFFERENTIAL_EVOLUTION_H */
