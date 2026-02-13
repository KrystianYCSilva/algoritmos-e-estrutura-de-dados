/**
 * @file pso.h
 * @brief Particle Swarm Optimization (PSO) para otimizacao continua
 *
 * Implementa PSO classico com enxame de particulas que exploram
 * o espaco de busca guiadas por sua melhor posicao pessoal (pbest)
 * e a melhor posicao global (gbest).
 *
 * Equacao de velocidade:
 *   v[i][d] = w * v[i][d]
 *           + c1 * r1 * (pbest[i][d] - x[i][d])
 *           + c2 * r2 * (gbest[d] - x[i][d])
 *
 * Variantes:
 * - Inertia weight (w): constante ou linear decreasing
 * - Constriction factor (chi): Clerc & Kennedy (2002)
 * - Velocity clamping: limita v_max
 *
 * Pseudocodigo (Kennedy & Eberhart, 1995):
 *   for i = 1 to n_particles:
 *     x[i] = random_position(); v[i] = random_velocity()
 *     pbest[i] = x[i]
 *   gbest = argmin(f(pbest[i]))
 *   for iter = 1 to max_iter:
 *     for i = 1 to n_particles:
 *       update velocity, position
 *       clamp position to bounds
 *       if f(x[i]) < f(pbest[i]): pbest[i] = x[i]
 *       if f(x[i]) < f(gbest): gbest = x[i]
 *
 * Referencias:
 * - Kennedy, J. & Eberhart, R. (1995). "Particle Swarm Optimization".
 *   Proc. IEEE Int. Conf. Neural Networks, pp. 1942-1948.
 * - Clerc, M. & Kennedy, J. (2002). "The Particle Swarm: Explosion,
 *   Stability, and Convergence". IEEE Trans. Evolutionary Computation.
 * - Shi, Y. & Eberhart, R. (1998). "A Modified Particle Swarm Optimizer".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_PSO_H
#define OPT_PSO_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Estrategia do peso de inercia
 */
typedef enum {
    PSO_INERTIA_CONSTANT,          /**< w fixo */
    PSO_INERTIA_LINEAR_DECREASING, /**< w decresce linearmente de w_max a w_min */
    PSO_INERTIA_CONSTRICTION       /**< Fator de constricao chi (Clerc & Kennedy) */
} PSOInertiaType;

/**
 * @brief Configuracao do PSO
 */
typedef struct {
    size_t num_particles;       /**< Tamanho do enxame */
    size_t max_iterations;      /**< Iteracoes maximas */

    double w;                   /**< Peso de inercia (constante) ou w_max */
    double w_min;               /**< Peso de inercia minimo (linear decreasing) */
    double c1;                  /**< Coeficiente cognitivo (atracao ao pbest) */
    double c2;                  /**< Coeficiente social (atracao ao gbest) */
    double v_max_ratio;         /**< v_max como fracao do range (0.1 = 10% do range) */

    PSOInertiaType inertia_type; /**< Tipo de inercia */

    double lower_bound;         /**< Limite inferior do dominio */
    double upper_bound;         /**< Limite superior do dominio */

    OptDirection direction;     /**< Minimizar ou maximizar */
    unsigned seed;              /**< Semente RNG */
} PSOConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para PSO
 *
 * Defaults: 30 particles, 500 iter, w=0.729, c1=1.49445, c2=1.49445
 * (constriction factor defaults), v_max=10% range, linear decreasing,
 * bounds [-5.12, 5.12], minimize, seed=42
 *
 * @return PSOConfig Configuracao padrao
 */
PSOConfig pso_default_config(void);

// ============================================================================
// ALGORITMO
// ============================================================================

/**
 * @brief Executa PSO
 *
 * @param config Configuracao do algoritmo
 * @param solution_size Dimensao do problema (D)
 * @param objective Funcao objetivo (recebe double*, size, context)
 * @param context Contexto do problema (ContinuousInstance*, etc.)
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * num_particles * D)
 */
OptResult pso_run(const PSOConfig *config,
                  size_t solution_size,
                  ObjectiveFn objective,
                  const void *context);

#endif /* OPT_PSO_H */
