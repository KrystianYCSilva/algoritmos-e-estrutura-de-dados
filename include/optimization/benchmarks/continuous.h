/**
 * @file continuous.h
 * @brief Benchmark de funcoes continuas para algoritmos de otimizacao
 *
 * Fornece funcoes de teste classicas para otimizacao continua:
 * Sphere, Rastrigin, Rosenbrock, Ackley e Schwefel.
 * Cada funcao e compativel com ObjectiveFn e inclui geracao de vizinhos
 * gaussianos para uso com heuristicas e meta-heuristicas.
 *
 * Representacao: solucao como array de double (vetor em R^D)
 * Funcao objetivo: valor da funcao no ponto (minimizacao)
 *
 * Referencias:
 * - Jamil, M. & Yang, X.-S. (2013). "A Literature Survey of Benchmark
 *   Functions for Global Optimization Problems"
 * - Molga, M. & Smutnicki, C. (2005). "Test functions for optimization needs"
 * - Simon, D. (2013). Evolutionary Optimization Algorithms, Wiley
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_BENCHMARK_CONTINUOUS_H
#define OPT_BENCHMARK_CONTINUOUS_H

#include "optimization/common.h"
#include <stddef.h>

// ============================================================================
// INSTANCIA DE PROBLEMA CONTINUO
// ============================================================================

/**
 * @brief Tipo de funcao benchmark continua
 */
typedef enum {
    CONTINUOUS_SPHERE,       /**< f(x) = sum(xi^2), min=0 em (0,...,0) */
    CONTINUOUS_RASTRIGIN,    /**< f(x) = 10D + sum(xi^2 - 10*cos(2*pi*xi)), min=0 em (0,...,0) */
    CONTINUOUS_ROSENBROCK,   /**< f(x) = sum(100*(x_{i+1}-xi^2)^2 + (1-xi)^2), min=0 em (1,...,1) */
    CONTINUOUS_ACKLEY,       /**< f(x) = -20*exp(...) - exp(...) + 20 + e, min=0 em (0,...,0) */
    CONTINUOUS_SCHWEFEL       /**< f(x) = 418.9829D - sum(xi*sin(sqrt(|xi|))), min=0 em (420.9687,...) */
} ContinuousFnType;

/**
 * @brief Instancia de problema de otimizacao continua
 */
typedef struct {
    ContinuousFnType fn_type;   /**< Tipo da funcao benchmark */
    size_t dimensions;           /**< Numero de dimensoes D */
    double lower_bound;          /**< Limite inferior do dominio */
    double upper_bound;          /**< Limite superior do dominio */
    double known_optimum;        /**< Valor otimo conhecido */
    double neighbor_sigma;       /**< Desvio padrao para vizinhanca gaussiana */
} ContinuousInstance;

// ============================================================================
// CRIACAO DE INSTANCIAS
// ============================================================================

/**
 * @brief Cria instancia Sphere (D dimensoes)
 *
 * f(x) = sum(xi^2), dominio [-5.12, 5.12]^D, min=0 em (0,...,0)
 *
 * @param dimensions Numero de dimensoes
 * @return ContinuousInstance* Instancia alocada
 */
ContinuousInstance* continuous_create_sphere(size_t dimensions);

/**
 * @brief Cria instancia Rastrigin (D dimensoes)
 *
 * f(x) = 10D + sum(xi^2 - 10*cos(2*pi*xi)), dominio [-5.12, 5.12]^D, min=0 em (0,...,0)
 *
 * @param dimensions Numero de dimensoes
 * @return ContinuousInstance* Instancia alocada
 */
ContinuousInstance* continuous_create_rastrigin(size_t dimensions);

/**
 * @brief Cria instancia Rosenbrock (D dimensoes)
 *
 * f(x) = sum(100*(x_{i+1}-xi^2)^2 + (1-xi)^2), dominio [-5, 10]^D, min=0 em (1,...,1)
 *
 * @param dimensions Numero de dimensoes
 * @return ContinuousInstance* Instancia alocada
 */
ContinuousInstance* continuous_create_rosenbrock(size_t dimensions);

/**
 * @brief Cria instancia Ackley (D dimensoes)
 *
 * dominio [-32.768, 32.768]^D, min=0 em (0,...,0)
 *
 * @param dimensions Numero de dimensoes
 * @return ContinuousInstance* Instancia alocada
 */
ContinuousInstance* continuous_create_ackley(size_t dimensions);

/**
 * @brief Cria instancia Schwefel (D dimensoes)
 *
 * dominio [-500, 500]^D, min=0 em (420.9687,...,420.9687)
 *
 * @param dimensions Numero de dimensoes
 * @return ContinuousInstance* Instancia alocada
 */
ContinuousInstance* continuous_create_schwefel(size_t dimensions);

/**
 * @brief Libera memoria de uma instancia continua
 *
 * @param inst Instancia a destruir
 */
void continuous_instance_destroy(ContinuousInstance *inst);

// ============================================================================
// FUNCOES OBJETIVO (ObjectiveFn-compatible)
// ============================================================================

/**
 * @brief Avalia funcao benchmark no ponto dado
 *
 * Despacha para a funcao correta baseado em fn_type da instancia.
 *
 * @param solution_data Array double* com D componentes
 * @param size Numero de dimensoes
 * @param context ContinuousInstance*
 * @return double Valor da funcao
 *
 * Complexidade: O(D)
 */
double continuous_evaluate(const void *solution_data, size_t size, const void *context);

double continuous_sphere(const double *x, size_t d);
double continuous_rastrigin(const double *x, size_t d);
double continuous_rosenbrock(const double *x, size_t d);
double continuous_ackley(const double *x, size_t d);
double continuous_schwefel(const double *x, size_t d);

// ============================================================================
// VIZINHANCA (NeighborFn-compatible)
// ============================================================================

/**
 * @brief Gera vizinho por perturbacao gaussiana com clamping
 *
 * neighbor[i] = clamp(current[i] + N(0, sigma), lower, upper)
 *
 * @param current Solucao atual (double*)
 * @param neighbor Buffer para vizinho (double*, pre-alocado)
 * @param size Numero de dimensoes
 * @param context ContinuousInstance* (usa neighbor_sigma e bounds)
 *
 * Complexidade: O(D)
 */
void continuous_neighbor_gaussian(const void *current, void *neighbor,
                                  size_t size, const void *context);

// ============================================================================
// GERACAO (GenerateFn-compatible)
// ============================================================================

/**
 * @brief Gera solucao aleatoria uniforme no dominio
 *
 * Cada componente: uniform(lower_bound, upper_bound)
 *
 * @param solution_data Buffer double* pre-alocado para D doubles
 * @param size Numero de dimensoes
 * @param context ContinuousInstance* (usa bounds)
 *
 * Complexidade: O(D)
 */
void continuous_generate_random(void *solution_data, size_t size, const void *context);

// ============================================================================
// UTILIDADES
// ============================================================================

/**
 * @brief Retorna nome da funcao benchmark como string
 *
 * @param fn_type Tipo da funcao
 * @return const char* Nome (e.g., "Sphere", "Rastrigin")
 */
const char* continuous_fn_name(ContinuousFnType fn_type);

/**
 * @brief Retorna vetor com o otimo global conhecido
 *
 * @param fn_type Tipo da funcao
 * @param dimensions Numero de dimensoes
 * @return double* Array alocado (caller libera com free)
 */
double* continuous_known_optimum_point(ContinuousFnType fn_type, size_t dimensions);

#endif /* OPT_BENCHMARK_CONTINUOUS_H */
