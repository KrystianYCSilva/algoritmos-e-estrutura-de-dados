/**
 * @file genetic_algorithm.h
 * @brief Algoritmo Genetico generico com operadores configuraveis
 *
 * Implementa GA classico com:
 * - Selecao: Tournament, Roulette Wheel, Rank-Based
 * - Crossover generico via function pointer (caller fornece)
 * - Mutacao generica via function pointer (caller fornece)
 * - Elitismo configuravel
 * - Busca local opcional (memetic-like)
 * - Taxas adaptativas opcionais
 *
 * O GA opera sobre representacao generica (void*), delegando crossover
 * e mutacao ao caller para suportar tanto permutacoes (TSP) quanto
 * vetores continuos. Operadores builtin sao fornecidos para ambos.
 *
 * Pseudocodigo (Holland, 1975; Goldberg, 1989):
 *   P = [generate() for _ in pop_size]
 *   evaluate(P)
 *   for gen = 1 to max_gen:
 *     P_new = elites(P)
 *     while |P_new| < pop_size:
 *       p1, p2 = select(P), select(P)
 *       c1, c2 = crossover(p1, p2) if rand() < p_c else copy(p1, p2)
 *       mutate(c1, p_m); mutate(c2, p_m)
 *       if local_search: improve(c1); improve(c2)
 *       P_new += [c1, c2]
 *     P = P_new
 *   return best(P)
 *
 * Referencias:
 * - Holland, J. H. (1975). Adaptation in Natural and Artificial Systems
 * - Goldberg, D. E. (1989). Genetic Algorithms in Search, Optimization, and ML
 * - Eiben, A. E. & Smith, J. E. (2015). Introduction to Evolutionary Computing
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_GENETIC_ALGORITHM_H
#define OPT_GENETIC_ALGORITHM_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Metodo de selecao de pais
 */
typedef enum {
    GA_SELECT_TOURNAMENT,   /**< Tournament selection (k configuravel) */
    GA_SELECT_ROULETTE,     /**< Roulette wheel (fitness-proportional) */
    GA_SELECT_RANK          /**< Rank-based selection */
} GASelectionType;

/**
 * @brief Funcao de crossover: combina dois pais em dois filhos
 *
 * @param parent1 Pai 1 (read-only)
 * @param parent2 Pai 2 (read-only)
 * @param child1 Buffer pre-alocado para filho 1
 * @param child2 Buffer pre-alocado para filho 2
 * @param size Dimensao logica
 * @param context Contexto do problema
 */
typedef void (*CrossoverFn)(const void *parent1, const void *parent2,
                            void *child1, void *child2,
                            size_t size, const void *context);

/**
 * @brief Funcao de mutacao: modifica solucao in-place
 *
 * @param solution Solucao a mutar (in-place)
 * @param size Dimensao logica
 * @param mutation_rate Taxa de mutacao
 * @param context Contexto do problema
 */
typedef void (*MutationFn)(void *solution, size_t size,
                           double mutation_rate, const void *context);

/**
 * @brief Funcao de busca local (para GA memetico)
 *
 * @param solution Solucao a melhorar (in-place, dados + custo retornado)
 * @param size Dimensao logica
 * @param objective Funcao objetivo
 * @param context Contexto
 * @return double Novo custo apos busca local
 */
typedef double (*LocalSearchFn)(void *solution, size_t size,
                                ObjectiveFn objective, const void *context);

/**
 * @brief Configuracao do Algoritmo Genetico
 */
typedef struct {
    size_t population_size;       /**< Tamanho da populacao */
    size_t max_generations;       /**< Geracoes maximas */
    double crossover_rate;        /**< Probabilidade de crossover (0.6-0.9) */
    double mutation_rate;         /**< Probabilidade de mutacao (0.01-0.1) */
    size_t elitism_count;         /**< Individuos preservados por elitismo */

    GASelectionType selection;    /**< Metodo de selecao */
    size_t tournament_size;       /**< k para tournament selection */

    bool enable_local_search;     /**< GA memetico */

    bool enable_adaptive_rates;   /**< Adaptar crossover/mutation rates */
    double adaptive_min_mutation; /**< Taxa minima de mutacao adaptativa */
    double adaptive_max_mutation; /**< Taxa maxima de mutacao adaptativa */

    OptDirection direction;       /**< Minimizar ou maximizar */
    unsigned seed;                /**< Semente RNG */
} GAConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para GA
 *
 * Defaults: pop=50, gen=500, pc=0.8, pm=0.05, elite=2,
 * tournament(k=3), no local search, no adaptive, minimize, seed=42
 *
 * @return GAConfig Configuracao padrao
 */
GAConfig ga_default_config(void);

// ============================================================================
// ALGORITMO PRINCIPAL
// ============================================================================

/**
 * @brief Executa Algoritmo Genetico
 *
 * @param config Configuracao
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param generate Funcao geradora de solucao
 * @param crossover Funcao de crossover
 * @param mutate Funcao de mutacao
 * @param local_search Funcao de busca local (NULL se desativada)
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_gen * pop_size * custo_objective)
 */
OptResult ga_run(const GAConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 GenerateFn generate,
                 CrossoverFn crossover,
                 MutationFn mutate,
                 LocalSearchFn local_search,
                 const void *context);

// ============================================================================
// CROSSOVER OPERATORS BUILTIN
// ============================================================================

/**
 * @brief Crossover OX (Order Crossover) para permutacoes
 *
 * Preserva ordem relativa. Adequado para TSP.
 * Ref: Davis (1985)
 */
void ga_crossover_ox(const void *p1, const void *p2,
                     void *c1, void *c2,
                     size_t size, const void *context);

/**
 * @brief Crossover PMX (Partially Mapped) para permutacoes
 *
 * Preserva posicoes absolutas. Adequado para TSP.
 * Ref: Goldberg & Lingle (1985)
 */
void ga_crossover_pmx(const void *p1, const void *p2,
                      void *c1, void *c2,
                      size_t size, const void *context);

/**
 * @brief Crossover BLX-alpha para vetores continuos
 *
 * child[i] = uniform(min - alpha*range, max + alpha*range)
 * com alpha=0.5 (default). Bom para exploração.
 * Ref: Eshelman & Schaffer (1993)
 */
void ga_crossover_blx(const void *p1, const void *p2,
                      void *c1, void *c2,
                      size_t size, const void *context);

// ============================================================================
// MUTATION OPERATORS BUILTIN
// ============================================================================

/**
 * @brief Mutacao swap para permutacoes
 *
 * Com prob mutation_rate, troca duas posicoes aleatorias.
 */
void ga_mutation_swap(void *solution, size_t size,
                      double mutation_rate, const void *context);

/**
 * @brief Mutacao inversion para permutacoes
 *
 * Com prob mutation_rate, inverte um segmento aleatorio.
 */
void ga_mutation_inversion(void *solution, size_t size,
                           double mutation_rate, const void *context);

/**
 * @brief Mutacao gaussiana para vetores continuos
 *
 * Para cada componente, com prob mutation_rate,
 * adiciona N(0, sigma) onde sigma e derivado do contexto.
 */
void ga_mutation_gaussian(void *solution, size_t size,
                          double mutation_rate, const void *context);

#endif /* OPT_GENETIC_ALGORITHM_H */
