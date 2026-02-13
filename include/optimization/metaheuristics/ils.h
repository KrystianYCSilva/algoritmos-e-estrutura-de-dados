/**
 * @file ils.h
 * @brief Iterated Local Search (ILS) para otimizacao combinatoria e continua
 *
 * Implementa ILS classico: perturba solucao localmente otima e re-otimiza.
 * Combina busca local com perturbacao para escapar de otimos locais.
 *
 * Criterios de aceitacao disponiveis:
 * - Better: aceita apenas se melhor que o incumbent
 * - Always: aceita sempre (random walk)
 * - SA-like: aceita piores com probabilidade decrescente
 * - Restart: reinicia apos k iteracoes sem melhoria
 *
 * Pseudocodigo (Lourenco et al., 2003):
 *   s = LOCAL-SEARCH(generate())
 *   s_best = s
 *   for iter = 1 to max_iter:
 *     s' = PERTURBATION(s, strength)
 *     s'' = LOCAL-SEARCH(s')
 *     s = ACCEPTANCE(s, s'', history)
 *     if f(s) < f(s_best): s_best = s
 *   return s_best
 *
 * Referencias:
 * - Lourenco, H. R., Martin, O. C. & Stutzle, T. (2003).
 *   "Iterated Local Search". In Handbook of Metaheuristics, Ch. 11.
 * - Talbi, E.-G. (2009). Metaheuristics: From Design to Implementation, Ch. 3
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_ILS_H
#define OPT_ILS_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Criterio de aceitacao do ILS
 */
typedef enum {
    ILS_ACCEPT_BETTER,     /**< Aceita apenas se estritamente melhor */
    ILS_ACCEPT_ALWAYS,     /**< Aceita sempre (random walk) */
    ILS_ACCEPT_SA_LIKE,    /**< Aceita piores com prob exp(-delta/T), T decresce */
    ILS_ACCEPT_RESTART     /**< Reinicia do melhor apos k iter sem melhoria */
} ILSAcceptance;

/**
 * @brief Configuracao do ILS
 */
typedef struct {
    size_t max_iterations;           /**< Iteracoes maximas do loop principal */
    size_t local_search_iterations;  /**< Iteracoes da busca local interna */
    size_t local_search_neighbors;   /**< Vizinhos por iteracao da busca local */
    int perturbation_strength;       /**< Forca da perturbacao (passada ao PerturbFn) */

    ILSAcceptance acceptance;        /**< Criterio de aceitacao */
    double sa_initial_temp;          /**< Temp inicial para SA-like acceptance */
    double sa_alpha;                 /**< Fator de resfriamento para SA-like */
    size_t restart_threshold;        /**< Iter sem melhoria para restart */

    OptDirection direction;          /**< Minimizar ou maximizar */
    unsigned seed;                   /**< Semente RNG */
} ILSConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para ILS
 *
 * Defaults: 1000 iter, LS 200 iter / 20 neighbors, strength=1,
 * accept better, minimize, seed=42
 *
 * @return ILSConfig Configuracao padrao
 */
ILSConfig ils_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa Iterated Local Search
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica (n_cities, n_dimensions)
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca (para busca local interna)
 * @param perturb Funcao de perturbacao (mais forte que neighbor)
 * @param generate Funcao geradora de solucao inicial
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * local_search_iterations * neighbors)
 */
OptResult ils_run(const ILSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  NeighborFn neighbor,
                  PerturbFn perturb,
                  GenerateFn generate,
                  const void *context);

#endif /* OPT_ILS_H */
