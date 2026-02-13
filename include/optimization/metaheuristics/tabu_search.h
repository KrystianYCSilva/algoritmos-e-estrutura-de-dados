/**
 * @file tabu_search.h
 * @brief Tabu Search classico e variantes avancadas
 *
 * Implementa Tabu Search com:
 * - Lista tabu classica (recency-based, FIFO circular)
 * - Criterio de aspiracao (aceita tabu se melhora best)
 * - Memoria de longo prazo (frequencia) para diversificacao
 * - Memoria de medio prazo (frequencia) para intensificacao
 * - Tabu tenure reativo (ajusta dinamicamente baseado em ciclagem)
 *
 * A lista tabu armazena hashes de solucoes (solution-based tabu) em vez
 * de move-based tabu, tornando o algoritmo generico para qualquer problema.
 *
 * Pseudocodigo (Glover, 1986):
 *   s = generate()
 *   best = s
 *   tabu_list = {}
 *   for iter = 1 to max_iter:
 *     candidates = {neighbor(s) for k in 1..K}
 *     s' = argmin_{c in candidates : c not tabu OR f(c) < f(best)} f(c)
 *     tabu_list.add(hash(s'))
 *     if |tabu_list| > tenure: tabu_list.remove_oldest()
 *     s = s'
 *     if f(s) < f(best): best = s
 *
 * Referencias:
 * - Glover, F. (1986). "Future Paths for Integer Programming and Links
 *   to Artificial Intelligence". Computers & OR, 13(5), 533-549.
 * - Glover, F. & Laguna, M. (1997). Tabu Search. Kluwer Academic.
 * - Battiti, R. & Tecchiolli, G. (1994). "The Reactive Tabu Search"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef OPT_TABU_SEARCH_H
#define OPT_TABU_SEARCH_H

#include "optimization/common.h"
#include <stddef.h>
#include <stdint.h>

// ============================================================================
// TIPOS
// ============================================================================

/**
 * @brief Funcao hash para solucoes (usada na lista tabu)
 *
 * @param solution_data Dados da solucao
 * @param size Dimensao logica
 * @return uint64_t Hash da solucao
 */
typedef uint64_t (*TabuHashFn)(const void *solution_data, size_t size);

/**
 * @brief Configuracao do Tabu Search
 */
typedef struct {
    size_t max_iterations;          /**< Iteracoes maximas */
    size_t neighbors_per_iter;      /**< Candidatos avaliados por iteracao */
    size_t tabu_tenure;             /**< Duracao do tabu (tamanho da lista) */

    bool enable_aspiration;         /**< Criterio de aspiracao (default: true) */

    bool enable_diversification;    /**< Memoria de longo prazo */
    double diversification_weight;  /**< Peso da penalizacao por frequencia */
    size_t diversification_trigger; /**< Iteracoes sem melhoria para diversificar */

    bool enable_intensification;    /**< Memoria de medio prazo */
    size_t intensification_trigger; /**< Iteracoes sem melhoria para intensificar */

    bool enable_reactive_tenure;    /**< Tenure reativo */
    size_t reactive_increase;       /**< Incremento do tenure ao detectar ciclagem */
    size_t reactive_decrease;       /**< Decremento do tenure quando nao cicla */
    size_t min_tenure;              /**< Tenure minimo (reativo) */
    size_t max_tenure;              /**< Tenure maximo (reativo) */

    OptDirection direction;         /**< Minimizar ou maximizar */
    unsigned seed;                  /**< Semente RNG */
} TSConfig;

// ============================================================================
// CONFIGURACAO
// ============================================================================

/**
 * @brief Retorna configuracao padrao para Tabu Search
 *
 * Defaults: 5000 iter, 20 candidates, tenure=15, aspiration=true,
 * no diversification, no intensification, no reactive, minimize, seed=42
 *
 * @return TSConfig Configuracao padrao
 */
TSConfig ts_default_config(void);

// ============================================================================
// ALGORITMOS
// ============================================================================

/**
 * @brief Executa Tabu Search
 *
 * @param config Configuracao do algoritmo
 * @param element_size Tamanho em bytes de cada solucao
 * @param solution_size Dimensao logica
 * @param objective Funcao objetivo
 * @param neighbor Funcao de vizinhanca
 * @param generate Funcao geradora
 * @param hash_fn Funcao hash para solucoes (NULL = usar hash generico)
 * @param context Contexto do problema
 * @return OptResult Resultado com melhor solucao e historico
 *
 * Complexidade: O(max_iterations * neighbors_per_iter * custo_objective)
 */
OptResult ts_run(const TSConfig *config,
                 size_t element_size,
                 size_t solution_size,
                 ObjectiveFn objective,
                 NeighborFn neighbor,
                 GenerateFn generate,
                 TabuHashFn hash_fn,
                 const void *context);

// ============================================================================
// HASH FUNCTIONS BUILTIN
// ============================================================================

/**
 * @brief Hash generico baseado em FNV-1a sobre os bytes da solucao
 *
 * @param solution_data Ponteiro para dados
 * @param size Dimensao logica (nao usada, usa element_size global)
 * @return uint64_t Hash FNV-1a
 */
uint64_t ts_hash_bytes(const void *solution_data, size_t size);

/**
 * @brief Hash para tours TSP (int array)
 *
 * Hash baseado na sequencia de cidades (normaliza para comecar em 0).
 *
 * @param solution_data Array de int (tour)
 * @param size Numero de cidades
 * @return uint64_t Hash
 */
uint64_t ts_hash_int_array(const void *solution_data, size_t size);

/**
 * @brief Hash para vetores double (discretiza com precisao fixa)
 *
 * @param solution_data Array de double
 * @param size Numero de dimensoes
 * @return uint64_t Hash
 */
uint64_t ts_hash_double_array(const void *solution_data, size_t size);

#endif /* OPT_TABU_SEARCH_H */
