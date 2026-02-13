/**
 * @file common.h
 * @brief Tipos genericos e infraestrutura para algoritmos de otimizacao
 *
 * Define os tipos fundamentais compartilhados por todas as heuristicas
 * e meta-heuristicas: representacao de solucao, funcao objetivo,
 * vizinhanca, perturbacao e resultado de otimizacao.
 *
 * Todos os algoritmos de otimizacao operam sobre estes tipos genericos,
 * permitindo resolver problemas discretos (TSP, Knapsack) e continuos
 * (Rastrigin, Rosenbrock) com a mesma interface.
 *
 * Referencias:
 * - Talbi, E.-G. (2009). Metaheuristics: From Design to Implementation
 * - Luke, S. (2013). Essentials of Metaheuristics
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_COMMON_H
#define OPT_COMMON_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS DE OTIMIZACAO
// ============================================================================

/**
 * @brief Direcao da otimizacao
 */
typedef enum {
    OPT_MINIMIZE,   /**< Minimizar funcao objetivo */
    OPT_MAXIMIZE    /**< Maximizar funcao objetivo */
} OptDirection;

// ============================================================================
// FUNCTION POINTER TYPES
// ============================================================================

/**
 * @brief Funcao objetivo: avalia qualidade de uma solucao
 *
 * @param solution_data Ponteiro para dados da solucao (int* para permutacao, double* para continuo)
 * @param size Tamanho da solucao (n_cities para TSP, n_dimensions para continuo)
 * @param context Contexto do problema (TSPInstance*, ContinuousInstance*, etc.)
 * @return double Custo/fitness da solucao (menor = melhor para minimizacao)
 */
typedef double (*ObjectiveFn)(const void *solution_data, size_t size, const void *context);

/**
 * @brief Funcao de vizinhanca: gera um vizinho a partir da solucao atual
 *
 * @param current Solucao atual (read-only)
 * @param neighbor Buffer pre-alocado para vizinho (mesmo tamanho que current)
 * @param size Tamanho da solucao
 * @param context Contexto do problema
 */
typedef void (*NeighborFn)(const void *current, void *neighbor, size_t size, const void *context);

/**
 * @brief Funcao de perturbacao: gera perturbacao mais forte que vizinhanca
 *
 * @param current Solucao atual (read-only)
 * @param perturbed Buffer pre-alocado para solucao perturbada
 * @param size Tamanho da solucao
 * @param strength Forca da perturbacao (1=fraca, valores maiores=mais forte)
 * @param context Contexto do problema
 */
typedef void (*PerturbFn)(const void *current, void *perturbed, size_t size,
                          int strength, const void *context);

/**
 * @brief Funcao geradora: cria solucao aleatoria valida
 *
 * @param solution Buffer pre-alocado para a solucao gerada
 * @param size Tamanho da solucao
 * @param context Contexto do problema
 */
typedef void (*GenerateFn)(void *solution, size_t size, const void *context);

// ============================================================================
// SOLUCAO DE OTIMIZACAO
// ============================================================================

/**
 * @brief Representacao generica de uma solucao de otimizacao
 */
typedef struct {
    void *data;          /**< Dados da solucao (heap-allocated) */
    size_t data_size;    /**< Tamanho em bytes dos dados */
    double cost;         /**< Valor da funcao objetivo */
} OptSolution;

// ============================================================================
// RESULTADO DE OTIMIZACAO
// ============================================================================

/**
 * @brief Resultado completo de uma execucao de algoritmo de otimizacao
 */
typedef struct {
    OptSolution best;            /**< Melhor solucao encontrada */
    double *convergence;         /**< Historico de convergencia (best cost por iteracao) */
    size_t convergence_size;     /**< Tamanho alocado do array convergence */
    size_t num_iterations;       /**< Iteracoes executadas */
    size_t num_evaluations;      /**< Avaliacoes da funcao objetivo */
    double elapsed_time_ms;      /**< Tempo de execucao em milliseconds */
} OptResult;

// ============================================================================
// CRIACAO E DESTRUICAO
// ============================================================================

/**
 * @brief Cria uma solucao com data_size bytes alocados, cost = DBL_MAX
 *
 * @param data_size Tamanho em bytes para alocar
 * @return OptSolution Solucao inicializada (data pode ser NULL se falha)
 */
OptSolution opt_solution_create(size_t data_size);

/**
 * @brief Clona uma solucao (deep copy dos dados)
 *
 * @param src Solucao a clonar
 * @return OptSolution Copia independente
 */
OptSolution opt_solution_clone(const OptSolution *src);

/**
 * @brief Libera memoria de uma solucao
 *
 * @param sol Solucao a destruir (data eh liberado, ponteiro nao)
 */
void opt_solution_destroy(OptSolution *sol);

/**
 * @brief Cria resultado com array de convergencia pre-alocado
 *
 * @param max_iterations Tamanho do array de convergencia
 * @return OptResult Resultado inicializado
 */
OptResult opt_result_create(size_t max_iterations);

/**
 * @brief Libera memoria de um resultado de otimizacao
 *
 * @param result Resultado a destruir
 */
void opt_result_destroy(OptResult *result);

// ============================================================================
// UTILIDADES
// ============================================================================

/**
 * @brief Seta seed global para funcoes de otimizacao
 *
 * @param seed Semente para gerador de numeros aleatorios
 */
void opt_set_seed(unsigned seed);

/**
 * @brief Retorna proximo numero aleatorio uniforme em [0, 1)
 *
 * @return double Valor uniforme
 */
double opt_random_uniform(void);

/**
 * @brief Retorna inteiro aleatorio em [min, max] (inclusive)
 *
 * @param min Limite inferior
 * @param max Limite superior
 * @return int Valor aleatorio
 */
int opt_random_int(int min, int max);

/**
 * @brief Retorna valor aleatorio gaussiano N(0,1) via Box-Muller
 *
 * @return double Valor gaussiano
 */
double opt_random_gaussian(void);

#endif /* OPT_COMMON_H */
