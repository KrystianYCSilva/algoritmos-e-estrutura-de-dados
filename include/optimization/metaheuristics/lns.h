/**
 * @file lns.h
 * @brief Large Neighborhood Search (LNS) e Adaptive LNS (ALNS)
 *
 * Implementa LNS classico e variante adaptativa:
 * - LNS: destroy + repair com operadores fixos
 * - ALNS: multiplos operadores destroy/repair com selecao adaptativa
 *
 * O paradigma destroy-and-repair remove parte da solucao (destroy) e
 * reconstroi (repair), explorando vizinhancas muito grandes.
 *
 * Aceitacao pode ser:
 * - Better only: aceita apenas se melhor
 * - SA-like: aceita piores com probabilidade decrescente
 *
 * Pseudocodigo LNS (Shaw, 1998):
 *   s = generate()
 *   s_best = s
 *   for iter = 1 to max_iter:
 *     s' = REPAIR(DESTROY(s, d))
 *     if ACCEPT(s, s'): s = s'
 *     if f(s) < f(s_best): s_best = s
 *   return s_best
 *
 * Pseudocodigo ALNS (Ropke & Pisinger, 2006):
 *   s = generate()
 *   weights_d = [1,...,1]; weights_r = [1,...,1]
 *   for iter = 1 to max_iter:
 *     d = SELECT(weights_d); r = SELECT(weights_r)
 *     s' = REPAIR_r(DESTROY_d(s))
 *     if ACCEPT(s, s'): s = s'; UPDATE-WEIGHTS(d, r)
 *     if f(s) < f(s_best): s_best = s
 *   return s_best
 *
 * Referencias:
 * - Shaw, P. (1998). "Using Constraint Programming and Local Search Methods
 *   to Solve Vehicle Routing Problems". CP-98, LNCS 1520, pp. 417-431.
 * - Ropke, S. & Pisinger, D. (2006). "An Adaptive Large Neighborhood Search
 *   Heuristic for the Pickup and Delivery Problem with Time Windows".
 *   Transportation Science, 40(4), 455-472.
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_LNS_H
#define OPT_LNS_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

#define LNS_MAX_OPERATORS 10

/**
 * @brief Variante do LNS
 */
typedef enum {
    LNS_BASIC,     /**< LNS basico com operadores fixos */
    LNS_ADAPTIVE   /**< ALNS com selecao adaptativa de operadores */
} LNSVariant;

/**
 * @brief Criterio de aceitacao do LNS
 */
typedef enum {
    LNS_ACCEPT_BETTER,   /**< Aceita apenas se estritamente melhor */
    LNS_ACCEPT_SA_LIKE   /**< Aceita piores com probabilidade exp(-delta/T) */
} LNSAcceptance;

/**
 * @brief Funcao de destruicao: remove elementos da solucao
 *
 * @param solution Solucao a destruir (in-place ou copiada para destroyed)
 * @param destroyed Buffer para solucao destruida (parcial)
 * @param size Dimensao logica
 * @param degree Grau de destruicao (0.0-1.0, fracao de elementos removidos)
 * @param context Contexto do problema
 */
typedef void (*DestroyFn)(const void *solution, void *destroyed, size_t size,
                          double degree, const void *context);

/**
 * @brief Funcao de reparo: reconstroi solucao a partir de solucao parcial
 *
 * @param destroyed Solucao parcial (resultado do destroy)
 * @param repaired Buffer para solucao completa reconstruida
 * @param size Dimensao logica
 * @param context Contexto do problema
 */
typedef void (*RepairFn)(const void *destroyed, void *repaired, size_t size,
                         const void *context);

/**
 * @brief Configuracao do LNS/ALNS
 */
typedef struct {
    size_t max_iterations;           /**< Iteracoes maximas */
    double destroy_degree;           /**< Fracao de elementos destruidos (0.1-0.5) */

    LNSVariant variant;              /**< LNS basico ou ALNS */
    LNSAcceptance acceptance;        /**< Criterio de aceitacao */

    double sa_initial_temp;          /**< Temp inicial para SA-like */
    double sa_alpha;                 /**< Fator de resfriamento */

    size_t num_destroy_ops;          /**< Numero de operadores destroy (ALNS) */
    size_t num_repair_ops;           /**< Numero de operadores repair (ALNS) */

    double reward_best;              /**< Peso para nova melhor global (ALNS) */
    double reward_better;            /**< Peso para melhoria do incumbente (ALNS) */
    double reward_accepted;          /**< Peso para solucao aceita (ALNS) */
    size_t weight_update_interval;   /**< Intervalo de atualizacao de pesos (ALNS) */
    double weight_decay;             /**< Fator de decaimento de pesos (ALNS, 0.0-1.0) */

    OptDirection direction;          /**< Minimizar ou maximizar */
    unsigned seed;                   /**< Semente RNG */
} LNSConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para LNS
 *
 * Defaults: 1000 iter, degree=0.3, basic, accept better,
 * SA T0=100 alpha=0.99, minimize, seed=42
 *
 * @return LNSConfig Configuracao padrao
 */
LNSConfig lns_default_config(void);

// ============================================================================
// ALGORITMO - LNS BASICO
// ============================================================================

/**
 * @brief Executa LNS basico com um par destroy/repair
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes de cada elemento
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param generate Funcao geradora de solucao inicial
 * @param destroy Funcao de destruicao
 * @param repair Funcao de reparo
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iter * (destroy + repair + objective))
 */
OptResult lns_run(const LNSConfig *config,
                  size_t element_size,
                  size_t solution_size,
                  ObjectiveFn objective,
                  GenerateFn generate,
                  DestroyFn destroy,
                  RepairFn repair,
                  const void *context);

/**
 * @brief Executa ALNS com multiplos operadores destroy/repair
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes de cada elemento
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param generate Funcao geradora de solucao inicial
 * @param destroy_ops Array de funcoes destroy
 * @param repair_ops Array de funcoes repair
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 */
OptResult alns_run(const LNSConfig *config,
                   size_t element_size,
                   size_t solution_size,
                   ObjectiveFn objective,
                   GenerateFn generate,
                   const DestroyFn *destroy_ops,
                   const RepairFn *repair_ops,
                   const void *context);

// ============================================================================
// DESTROY/REPAIR BUILTIN - TSP
// ============================================================================

/**
 * @brief Destroy aleatorio para TSP: marca posicoes como -1
 */
void lns_destroy_tsp_random(const void *solution, void *destroyed, size_t size,
                            double degree, const void *context);

/**
 * @brief Repair greedy para TSP: insere cidades removidas na melhor posicao
 */
void lns_repair_tsp_greedy(const void *destroyed, void *repaired, size_t size,
                           const void *context);

/**
 * @brief Destroy worst para TSP: remove cidades com maiores contribuicoes de custo
 */
void lns_destroy_tsp_worst(const void *solution, void *destroyed, size_t size,
                           double degree, const void *context);

/**
 * @brief Repair random para TSP: insere cidades removidas em posicoes aleatorias
 */
void lns_repair_tsp_random(const void *destroyed, void *repaired, size_t size,
                           const void *context);

#endif /* OPT_LNS_H */
