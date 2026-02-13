/**
 * @file aco.h
 * @brief Ant Colony Optimization (ACO) para problemas combinatorios
 *
 * Implementa ACO para TSP e problemas de roteamento, com variantes:
 * - Ant System (AS): todas as formigas depositam feromonio
 * - Elitist AS: melhor global tambem deposita (com peso extra)
 * - MAX-MIN AS (MMAS): apenas melhor formiga deposita, com limites tau_min/tau_max
 *
 * Regra de transicao probabilistica:
 *   p(j) = [tau(i,j)^alpha * eta(i,j)^beta] / sum_l [tau(i,l)^alpha * eta(i,l)^beta]
 *
 * Atualizacao de feromonio:
 *   tau(i,j) = (1 - rho) * tau(i,j) + sum_k delta_tau_k(i,j)
 *   delta_tau_k(i,j) = Q / f(s_k) se (i,j) em s_k, senao 0
 *
 * Pseudocodigo (Dorigo & Stutzle, 2004):
 *   tau = tau_0
 *   for iter = 1 to max_iter:
 *     for k = 1 to n_ants:
 *       s_k = CONSTRUCT-SOLUTION(tau, alpha, beta, eta)
 *     EVAPORATE(tau, rho)
 *     DEPOSIT(tau, solutions)
 *   return best_overall
 *
 * Referencias:
 * - Dorigo, M. & Stutzle, T. (2004). Ant Colony Optimization. MIT Press.
 * - Dorigo, M. (1992). PhD Thesis, Politecnico di Milano.
 * - Stutzle, T. & Hoos, H. H. (2000). "MAX-MIN Ant System".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_ACO_H
#define OPT_ACO_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Variante do ACO
 */
typedef enum {
    ACO_ANT_SYSTEM,     /**< Todas as formigas depositam */
    ACO_ELITIST,        /**< AS + melhor global com peso extra */
    ACO_MAX_MIN         /**< Apenas melhor deposita, limites tau_min/tau_max */
} ACOVariant;

/**
 * @brief Funcao de informacao heuristica
 *
 * Retorna a desejabilidade de ir do no i para o no j.
 * Para TSP: eta(i,j) = 1/dist(i,j)
 *
 * @param i No de origem
 * @param j No de destino
 * @param context Contexto do problema (TSPInstance*, etc.)
 * @return double Valor heuristico (maior = mais desejavel)
 */
typedef double (*ACOHeuristicFn)(size_t i, size_t j, const void *context);

/**
 * @brief Configuracao do ACO
 */
typedef struct {
    size_t n_ants;            /**< Numero de formigas por iteracao */
    size_t max_iterations;    /**< Iteracoes maximas */

    double alpha;             /**< Peso do feromonio (expoente) */
    double beta;              /**< Peso da heuristica (expoente) */
    double rho;               /**< Taxa de evaporacao (0.0-1.0) */
    double q;                 /**< Constante de deposito */
    double tau_0;             /**< Feromonio inicial */

    ACOVariant variant;       /**< Variante do ACO */
    double elitist_weight;    /**< Peso extra para melhor global (elitist) */
    double tau_min;           /**< Feromonio minimo (MMAS) */
    double tau_max;           /**< Feromonio maximo (MMAS) */

    OptDirection direction;   /**< Minimizar ou maximizar */
    unsigned seed;            /**< Semente RNG */
} ACOConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para ACO
 *
 * Defaults: n_ants=20, 500 iter, alpha=1.0, beta=3.0, rho=0.1,
 * Q=1.0, tau_0=0.1, AS variant, minimize, seed=42
 *
 * @return ACOConfig Configuracao padrao
 */
ACOConfig aco_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa ACO para problema combinatorio com n nos
 *
 * @param config Configuracao
 * @param n_nodes Numero de nos do problema (cidades para TSP)
 * @param objective Funcao objetivo (recebe int* tour, size_t n, context)
 * @param heuristic Funcao de informacao heuristica eta(i,j)
 * @param context Contexto do problema (TSPInstance*, etc.)
 * @return OptResult Resultado (best.data = int* tour)
 *
 * Complexidade: O(max_iterations * n_ants * n_nodes^2)
 */
OptResult aco_run(const ACOConfig *config,
                  size_t n_nodes,
                  ObjectiveFn objective,
                  ACOHeuristicFn heuristic,
                  const void *context);

// ============================================================================
// HEURISTICAS BUILTIN
// ============================================================================

/**
 * @brief Heuristica TSP: eta(i,j) = 1 / dist(i,j)
 *
 * @param i Cidade origem
 * @param j Cidade destino
 * @param context TSPInstance*
 * @return double 1/dist(i,j)
 */
double aco_heuristic_tsp(size_t i, size_t j, const void *context);

#endif /* OPT_ACO_H */
