/**
 * @file hill_climbing.h
 * @brief Hill Climbing e variantes para otimizacao combinatoria e continua
 *
 * Implementa quatro variantes de Hill Climbing:
 * - Steepest Ascent/Descent: avalia k vizinhos por iteracao, escolhe o melhor
 * - First Improvement: aceita o primeiro vizinho melhor encontrado
 * - Random Restart: multiplas execucoes com pontos iniciais aleatorios
 * - Stochastic: aceita vizinhos melhores com probabilidade proporcional
 *
 * Todas as variantes operam sobre os tipos genericos de optimization/common.h,
 * podendo resolver TSP (int*), funcoes continuas (double*), ou qualquer outro
 * problema que forneca ObjectiveFn + NeighborFn + GenerateFn.
 *
 * Pseudocodigo base (Steepest):
 *   current = generate()
 *   for iter = 1 to max_iter:
 *     best_neighbor = NULL
 *     for k = 1 to neighbors_per_iter:
 *       candidate = neighbor(current)
 *       if is_better(candidate, best_neighbor):
 *         best_neighbor = candidate
 *     if is_better(best_neighbor, current):
 *       current = best_neighbor
 *     else: break  // otimo local
 *
 * Referencias:
 * - Russell, S. & Norvig, P. (2010). Artificial Intelligence, Ch. 4.1
 * - Talbi, E.-G. (2009). Metaheuristics: From Design to Implementation, Ch. 3
 * - Luke, S. (2013). Essentials of Metaheuristics, Ch. 2
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_HILL_CLIMBING_H
#define OPT_HILL_CLIMBING_H

#include "optimization/common.h"
#include <stddef.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Variante do Hill Climbing
 */
typedef enum {
    HC_STEEPEST,          /**< Avalia k vizinhos, escolhe o melhor */
    HC_FIRST_IMPROVEMENT, /**< Aceita primeiro vizinho melhor */
    HC_RANDOM_RESTART,    /**< Multiplas execucoes com restart aleatorio */
    HC_STOCHASTIC         /**< Aceita vizinho melhor com prob. proporcional a delta */
} HCVariant;

/**
 * @brief Configuracao do Hill Climbing
 */
typedef struct {
    HCVariant variant;             /**< Variante a utilizar */
    size_t max_iterations;         /**< Iteracoes maximas por execucao */
    size_t neighbors_per_iter;     /**< Vizinhos avaliados por iteracao (steepest) */
    size_t num_restarts;           /**< Numero de restarts (random restart) */
    double stochastic_temperature; /**< Temperatura para variante estocastica */
    OptDirection direction;        /**< Minimizar ou maximizar */
    unsigned seed;                 /**< Semente RNG */
} HCConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para Hill Climbing
 *
 * Defaults: steepest, 1000 iter, 20 vizinhos/iter, 10 restarts,
 * temp=1.0, minimize, seed=42
 *
 * @return HCConfig Configuracao padrao
 */
HCConfig hc_default_config(void);

// ============================================================================
// ALGORITMOS
// ============================================================================

/**
 * @brief Executa Hill Climbing generico
 *
 * Despacha para a variante configurada em config.variant.
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica (n_cities, n_dimensions)
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora de solucao inicial
 * @param context Contexto do problema (TSPInstance*, ContinuousInstance*, etc.)
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * neighbors_per_iter * custo_objective)
 */
OptResult hc_run(const HCConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 NeighborFn neighbor,
                 GenerateFn generate,
                 const void *context);

/**
 * @brief Hill Climbing Steepest Descent/Ascent
 *
 * Avalia neighbors_per_iter vizinhos por iteracao, aceita o melhor
 * se for estritamente melhor que o atual. Para ao atingir otimo local.
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param context Contexto do problema
 * @return OptResult Resultado
 *
 * Complexidade: O(max_iterations * neighbors_per_iter)
 */
OptResult hc_steepest(const HCConfig *config,
                      size_t element_size,
                      size_t solution_size,
                      ObjectiveFn objective,
                      NeighborFn neighbor,
                      GenerateFn generate,
                      const void *context);

/**
 * @brief Hill Climbing First Improvement
 *
 * Avalia vizinhos sequencialmente, aceita o primeiro melhor.
 * Mais rapido por iteracao que steepest, pode convergir diferente.
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param context Contexto do problema
 * @return OptResult Resultado
 *
 * Complexidade: O(max_iterations * neighbors_per_iter) worst case
 */
OptResult hc_first_improvement(const HCConfig *config,
                               size_t element_size,
                               size_t solution_size,
                               ObjectiveFn objective,
                               NeighborFn neighbor,
                               GenerateFn generate,
                               const void *context);

/**
 * @brief Hill Climbing com Random Restart
 *
 * Executa num_restarts vezes o steepest HC com solucoes iniciais
 * diferentes, retorna o melhor resultado global.
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param context Contexto do problema
 * @return OptResult Resultado global
 *
 * Complexidade: O(num_restarts * max_iterations * neighbors_per_iter)
 */
OptResult hc_random_restart(const HCConfig *config,
                            size_t element_size,
                            size_t solution_size,
                            ObjectiveFn objective,
                            NeighborFn neighbor,
                            GenerateFn generate,
                            const void *context);

/**
 * @brief Hill Climbing Estocastico
 *
 * Aceita vizinho melhor sempre; vizinho pior com probabilidade
 * exp(-delta / temperature) onde delta = |f(neighbor) - f(current)|.
 * Diferente de SA por ter temperatura fixa.
 *
 * @param config Configuracao (usa stochastic_temperature)
 * @param element_size Tamanho em bytes
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param context Contexto do problema
 * @return OptResult Resultado
 *
 * Complexidade: O(max_iterations)
 */
OptResult hc_stochastic(const HCConfig *config,
                        size_t element_size,
                        size_t solution_size,
                        ObjectiveFn objective,
                        NeighborFn neighbor,
                        GenerateFn generate,
                        const void *context);

#endif /* OPT_HILL_CLIMBING_H */
