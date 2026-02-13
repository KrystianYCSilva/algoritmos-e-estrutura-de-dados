/**
 * @file grasp.h
 * @brief GRASP - Greedy Randomized Adaptive Search Procedure
 *
 * Implementa GRASP classico: construcao gulosa randomizada + busca local.
 * Cada iteracao constroi uma solucao com a RCL (Restricted Candidate List)
 * e depois aplica busca local para melhorar.
 *
 * O parametro alpha controla o trade-off entre guloso e aleatorio:
 * - alpha = 0.0: construcao puramente gulosa (deterministica)
 * - alpha = 1.0: construcao puramente aleatoria
 * - alpha intermediario: selecao da RCL com threshold adaptativo
 *
 * Pseudocodigo (Feo & Resende, 1995):
 *   s_best = null
 *   for iter = 1 to max_iter:
 *     s = GREEDY-RANDOMIZED-CONSTRUCTION(alpha)
 *     s = LOCAL-SEARCH(s)
 *     if f(s) < f(s_best): s_best = s
 *   return s_best
 *
 * Construcao com RCL:
 *   s = empty; CL = candidates
 *   while s incomplete:
 *     g_min = min(cost(e) for e in CL)
 *     g_max = max(cost(e) for e in CL)
 *     threshold = g_min + alpha * (g_max - g_min)
 *     RCL = {e in CL : cost(e) <= threshold}
 *     e = random(RCL)
 *     s = s + e; update(CL)
 *
 * Variantes:
 * - Reactive GRASP: ajusta alpha dinamicamente baseado em performance historica
 *
 * Referencias:
 * - Feo, T. A. & Resende, M. G. C. (1995). "Greedy Randomized Adaptive
 *   Search Procedures". J. Global Optimization, 6(2), 109-133.
 * - Resende, M. G. C. & Ribeiro, C. C. (2003). "Greedy Randomized Adaptive
 *   Search Procedures". In Handbook of Metaheuristics, Ch. 8.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_GRASP_H
#define OPT_GRASP_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Funcao de construcao gulosa randomizada
 *
 * Constroi uma solucao usando a abordagem GRASP com RCL.
 * O parametro alpha controla a randomizacao.
 *
 * @param solution Buffer pre-alocado para a solucao construida
 * @param size Dimensao logica
 * @param alpha Grau de randomizacao [0.0=greedy, 1.0=random]
 * @param context Contexto do problema
 */
typedef void (*GRASPConstructFn)(void *solution, size_t size,
                                 double alpha, const void *context);

/**
 * @brief Configuracao do GRASP
 */
typedef struct {
    size_t max_iterations;           /**< Iteracoes GRASP (multi-start) */
    double alpha;                    /**< Parametro RCL [0.0, 1.0] */

    size_t local_search_iterations;  /**< Iteracoes da busca local interna */
    size_t local_search_neighbors;   /**< Vizinhos por iteracao da busca local */

    bool enable_reactive;            /**< Reactive GRASP (ajusta alpha) */
    size_t reactive_num_alphas;      /**< Numero de alphas candidatos */
    size_t reactive_block_size;      /**< Iteracoes por bloco de atualizacao */

    OptDirection direction;          /**< Minimizar ou maximizar */
    unsigned seed;                   /**< Semente RNG */
} GRASPConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para GRASP
 *
 * Defaults: 500 iter, alpha=0.3, LS 100 iter/20 neighbors,
 * no reactive, minimize, seed=42
 *
 * @return GRASPConfig Configuracao padrao
 */
GRASPConfig grasp_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa GRASP
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param construct Funcao de construcao gulosa randomizada
 * @param neighbor Funcao de vizinhanca (para busca local interna)
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * (construcao + LS_iterations * neighbors))
 */
OptResult grasp_run(const GRASPConfig *config,
                    size_t element_size,
                    size_t solution_size,
                    ObjectiveFn objective,
                    GRASPConstructFn construct,
                    NeighborFn neighbor,
                    const void *context);

// ============================================================================
// CONSTRUTORES BUILTIN
// ============================================================================

/**
 * @brief Construcao gulosa randomizada para TSP (nearest neighbor + RCL)
 *
 * Em cada passo, calcula distancia a todas cidades nao visitadas,
 * monta RCL com threshold = d_min + alpha * (d_max - d_min),
 * e seleciona aleatoriamente da RCL.
 *
 * @param solution Buffer int* para o tour
 * @param size Numero de cidades
 * @param alpha Parametro RCL [0.0=nearest neighbor, 1.0=random]
 * @param context TSPInstance*
 */
void grasp_construct_tsp_nn(void *solution, size_t size,
                            double alpha, const void *context);

/**
 * @brief Construcao gulosa randomizada para problemas continuos
 *
 * Para problemas continuos, gera solucao como mistura entre
 * centro do dominio (greedy) e posicao aleatoria (random),
 * controlada por alpha.
 *
 * @param solution Buffer double* para a solucao
 * @param size Numero de dimensoes
 * @param alpha Parametro [0.0=centro do dominio, 1.0=uniforme aleatorio]
 * @param context ContinuousInstance*
 */
void grasp_construct_continuous(void *solution, size_t size,
                                double alpha, const void *context);

#endif /* OPT_GRASP_H */
