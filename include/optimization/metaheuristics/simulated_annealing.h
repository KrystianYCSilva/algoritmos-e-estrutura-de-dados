/**
 * @file simulated_annealing.h
 * @brief Simulated Annealing e variantes para otimizacao
 *
 * Implementa Simulated Annealing classico com quatro cooling schedules:
 * - Geometrico: T(k+1) = alpha * T(k)
 * - Linear: T(k+1) = T(k) - (T0 - Tmin) / max_iterations
 * - Logaritmico: T(k) = T0 / ln(1 + k)
 * - Adaptativo: ajusta T baseado na taxa de aceitacao
 *
 * Variantes adicionais:
 * - Reheating: reaquece quando taxa de aceitacao cai abaixo de limiar
 * - Auto-calibrate T0: determina T0 automaticamente para ~80% de aceitacao
 *
 * Pseudocodigo (Kirkpatrick et al., 1983):
 *   s = generate()
 *   T = T0
 *   while T > T_min:
 *     for i = 1 to L:
 *       s' = neighbor(s)
 *       delta = f(s') - f(s)
 *       if delta < 0: s = s'
 *       elif random() < exp(-delta/T): s = s'
 *       if f(s) < f(best): best = s
 *     T = cool(T)
 *
 * Referencias:
 * - Kirkpatrick, S., Gelatt, C. D. & Vecchi, M. P. (1983).
 *   "Optimization by Simulated Annealing". Science, 220(4598), 671-680.
 * - Cerny, V. (1985). "Thermodynamical Approach to the Traveling Salesman Problem"
 * - Hajek, B. (1988). "Cooling Schedules for Optimal Annealing"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_SIMULATED_ANNEALING_H
#define OPT_SIMULATED_ANNEALING_H

#include "optimization/common.h"
#include <stddef.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Cooling schedule para Simulated Annealing
 */
typedef enum {
    SA_COOLING_GEOMETRIC,   /**< T(k+1) = alpha * T(k) */
    SA_COOLING_LINEAR,      /**< T(k+1) = T(k) - delta */
    SA_COOLING_LOGARITHMIC, /**< T(k) = T0 / ln(1 + k) */
    SA_COOLING_ADAPTIVE     /**< Ajusta baseado na taxa de aceitacao */
} SACoolingSchedule;

/**
 * @brief Configuracao do Simulated Annealing
 */
typedef struct {
    double initial_temp;           /**< T0: temperatura inicial */
    double final_temp;             /**< T_min: temperatura minima */
    double alpha;                  /**< Fator de resfriamento geometrico (0.9-0.999) */
    SACoolingSchedule cooling;     /**< Tipo de cooling schedule */
    size_t max_iterations;         /**< Iteracoes totais maximas */
    size_t markov_chain_length;    /**< L: iteracoes por nivel de temperatura */

    bool enable_reheating;         /**< Ativa reaquecimento */
    double reheat_threshold;       /**< Taxa de aceitacao minima para reaquecimento */
    double reheat_factor;          /**< Fator de reaquecimento (ex: 2.0 = dobra T) */

    bool auto_calibrate_t0;        /**< Auto-determinar T0 para ~80% de aceitacao */
    size_t calibration_samples;    /**< Amostras para calibracao (default 100) */
    double target_acceptance;      /**< Taxa de aceitacao alvo para calibracao (0.8) */

    double adaptive_target_low;    /**< Limite inferior taxa de aceitacao (adaptive) */
    double adaptive_target_high;   /**< Limite superior taxa de aceitacao (adaptive) */
    double adaptive_factor;        /**< Fator de ajuste (adaptive, ex: 1.05) */

    OptDirection direction;        /**< Minimizar ou maximizar */
    unsigned seed;                 /**< Semente RNG */
} SAConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para Simulated Annealing
 *
 * Defaults: T0=100, Tmin=0.001, alpha=0.95, geometric, 10000 iter,
 * L=50, no reheating, no auto-calibrate, minimize, seed=42
 *
 * @return SAConfig Configuracao padrao
 */
SAConfig sa_default_config(void);

// ============================================================================
// ALGORITMOS
// ============================================================================

/**
 * @brief Executa Simulated Annealing classico
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica (n_cities, n_dimensions)
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora de solucao inicial
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * custo_objective)
 */
OptResult sa_run(const SAConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 NeighborFn neighbor,
                 GenerateFn generate,
                 const void *context);

/**
 * @brief Auto-calibra T0 para taxa de aceitacao alvo
 *
 * Gera calibration_samples vizinhos aleatorios, mede deltas medios,
 * calcula T0 tal que exp(-delta_medio / T0) = target_acceptance.
 *
 * @param config Configuracao (usa calibration_samples, target_acceptance)
 * @param element_size Tamanho em bytes
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param context Contexto
 * @return double T0 calibrado
 */
double sa_calibrate_t0(const SAConfig *config,
                       size_t element_size,
                       size_t solution_size,
                       ObjectiveFn objective,
                       NeighborFn neighbor,
                       GenerateFn generate,
                       const void *context);

#endif /* OPT_SIMULATED_ANNEALING_H */
