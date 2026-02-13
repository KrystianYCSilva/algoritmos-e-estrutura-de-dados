/**
 * @file vns.h
 * @brief Variable Neighborhood Search (VNS) para otimizacao combinatoria e continua
 *
 * Implementa VNS com variantes:
 * - Basic VNS (BVNS): shaking + local search + move-or-not
 * - Reduced VNS (RVNS): shaking only, sem local search (rapido)
 * - General VNS (GVNS): VNS com VND (multiple neighborhoods) na busca local
 *
 * O VNS opera sobre k vizinhancas de tamanho crescente. Quando a busca
 * local encontra melhoria, retorna para N_1. Sem melhoria, avanca para N_{k+1}.
 *
 * A vizinhanca k eh implementada via NeighborFn com perturbacao de forca k:
 * o parametro `strength` do shake controla quantos passos de vizinhanca aplicar.
 *
 * Pseudocodigo (Mladenovic & Hansen, 1997):
 *   s = generate()
 *   for iter = 1 to max_iter:
 *     k = 1
 *     while k <= k_max:
 *       s' = SHAKE(s, k)
 *       s'' = LOCAL-SEARCH(s')
 *       if f(s'') < f(s): s = s''; k = 1
 *       else: k = k + 1
 *   return s
 *
 * Referencias:
 * - Mladenovic, N. & Hansen, P. (1997). "Variable Neighborhood Search".
 *   Computers & Operations Research, 24(11), 1097-1100.
 * - Hansen, P. & Mladenovic, N. (2001). "Variable Neighborhood Search:
 *   Principles and Applications". European J. Operational Research, 130(3).
 * - Hansen, P., Mladenovic, N. & Moreno Perez, J. A. (2010).
 *   "Variable Neighbourhood Search: Methods and Applications". Annals of OR.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_VNS_H
#define OPT_VNS_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Variante do VNS
 */
typedef enum {
    VNS_BASIC,      /**< Basic VNS: shake + local search */
    VNS_REDUCED,    /**< Reduced VNS: shake only (sem local search) */
    VNS_GENERAL     /**< General VNS: shake + VND (multi-neighborhood LS) */
} VNSVariant;

/**
 * @brief Funcao de shaking: gera solucao aleatoria na vizinhanca k
 *
 * @param current Solucao atual (read-only)
 * @param shaken Buffer pre-alocado para solucao perturbada
 * @param size Dimensao logica
 * @param k Indice da vizinhanca (1..k_max), controla intensidade
 * @param context Contexto do problema
 */
typedef void (*ShakeFn)(const void *current, void *shaken, size_t size,
                        int k, const void *context);

/**
 * @brief Configuracao do VNS
 */
typedef struct {
    size_t max_iterations;          /**< Iteracoes maximas do loop externo */
    int k_max;                      /**< Numero de vizinhancas (1..k_max) */

    size_t local_search_iterations; /**< Iteracoes da busca local interna */
    size_t local_search_neighbors;  /**< Vizinhos por iteracao da busca local */

    VNSVariant variant;             /**< Variante do VNS */

    int vnd_num_neighborhoods;      /**< Numero de vizinhancas no VND (GVNS) */

    OptDirection direction;         /**< Minimizar ou maximizar */
    unsigned seed;                  /**< Semente RNG */
} VNSConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para VNS
 *
 * Defaults: 1000 iter, k_max=5, LS 200 iter / 20 neighbors,
 * basic variant, minimize, seed=42
 *
 * @return VNSConfig Configuracao padrao
 */
VNSConfig vns_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa Variable Neighborhood Search
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada elemento da solucao
 * @param solution_size Dimensao logica (n_cities, n_dimensions)
 * @param objective Funcao objetivo
 * @param shake Funcao de shaking (vizinhanca k)
 * @param neighbor Funcao de vizinhanca (para busca local)
 * @param generate Funcao geradora de solucao inicial
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iter * k_max * LS_iter * LS_neighbors)
 */
OptResult vns_run(const VNSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  ShakeFn shake,
                  NeighborFn neighbor,
                  GenerateFn generate,
                  const void *context);

// ============================================================================
// SHAKE FUNCTIONS BUILTIN
// ============================================================================

/**
 * @brief Shake para TSP: aplica k swaps aleatorios
 *
 * Vizinhanca k = k swaps independentes sobre a permutacao.
 * k=1 eh swap simples, k grande eh perturbacao forte.
 */
void vns_shake_tsp(const void *current, void *shaken, size_t size,
                   int k, const void *context);

/**
 * @brief Shake para problemas continuos: perturbacao gaussiana com sigma=k*0.5
 *
 * Vizinhanca k = gaussiana com desvio crescente.
 */
void vns_shake_continuous(const void *current, void *shaken, size_t size,
                          int k, const void *context);

#endif /* OPT_VNS_H */
