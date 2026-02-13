/**
 * @file string_matching.h
 * @brief Algoritmos de Busca em Strings (String Matching / Pattern Matching)
 *
 * Implementacao de 4 algoritmos classicos de busca de padroes em texto:
 * - Naive (forca bruta)
 * - Knuth-Morris-Pratt (KMP)
 * - Rabin-Karp (hashing)
 * - Boyer-Moore (bad character + good suffix)
 *
 * Complexidades:
 * - Naive: O(n*m) pior caso
 * - KMP: O(n+m) garantido
 * - Rabin-Karp: O(n+m) medio, O(n*m) pior caso
 * - Boyer-Moore: O(n/m) melhor caso, O(n*m) pior caso
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 32.1-32.4
 * - Knuth, Morris, Pratt (1977), "Fast pattern matching in strings"
 * - Boyer, Moore (1977), "A fast string searching algorithm"
 * - Karp, Rabin (1987), "Efficient randomized pattern-matching algorithms"
 * - Sedgewick & Wayne (2011), Chapter 5.3
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef STRING_MATCHING_H
#define STRING_MATCHING_H

#include <stddef.h>
#include <stdbool.h>

#define SM_NOT_FOUND ((size_t)-1)

/**
 * @brief Resultado de busca com multiplas ocorrencias
 *
 * Armazena todas as posicoes onde o padrao foi encontrado no texto.
 */
typedef struct {
    size_t *positions;
    size_t count;
    size_t capacity;
} MatchResult;

/**
 * @brief Cria um MatchResult vazio
 * @return MatchResult inicializado ou {NULL, 0, 0} em caso de falha
 */
MatchResult match_result_create(void);

/**
 * @brief Libera memoria de um MatchResult
 * @param result Ponteiro para o resultado
 */
void match_result_destroy(MatchResult *result);

// ============================================================================
// NAIVE - Forca Bruta
// ============================================================================

/**
 * @brief Busca Naive (Forca Bruta) - primeira ocorrencia
 *
 * Desliza o padrao sobre o texto, comparando caractere a caractere.
 *
 * @param text Texto onde buscar
 * @param pattern Padrao a encontrar
 * @return size_t Posicao da primeira ocorrencia ou SM_NOT_FOUND
 *
 * Complexidade: O(n*m) pior caso, O(n) caso medio
 * Espaco: O(1)
 *
 * Referencia: Cormen S32.1 (pseudocodigo NAIVE-STRING-MATCHER p. 988)
 */
size_t naive_search(const char *text, const char *pattern);

/**
 * @brief Busca Naive - todas as ocorrencias
 *
 * @param text Texto onde buscar
 * @param pattern Padrao a encontrar
 * @return MatchResult com todas as posicoes encontradas
 */
MatchResult naive_search_all(const char *text, const char *pattern);

// ============================================================================
// KMP - Knuth-Morris-Pratt
// ============================================================================

/**
 * @brief Computa a funcao de falha (prefix function) para KMP
 *
 * Para cada posicao i no padrao, pi[i] = comprimento do maior
 * prefixo proprio que tambem e sufixo de pattern[0..i].
 *
 * @param pattern Padrao
 * @param m Comprimento do padrao
 * @param failure Array de saida (deve ter tamanho m)
 *
 * Complexidade: O(m)
 * Referencia: Cormen S32.4 (pseudocodigo COMPUTE-PREFIX-FUNCTION p. 1006)
 */
void kmp_compute_failure(const char *pattern, size_t m, size_t *failure);

/**
 * @brief Busca KMP - primeira ocorrencia
 *
 * Usa a funcao de falha para evitar re-comparacoes de caracteres
 * ja verificados, garantindo O(n+m).
 *
 * @param text Texto onde buscar
 * @param pattern Padrao a encontrar
 * @return size_t Posicao da primeira ocorrencia ou SM_NOT_FOUND
 *
 * Complexidade: O(n+m)
 * Espaco: O(m) para tabela de falha
 *
 * Referencia: Cormen S32.4 (pseudocodigo KMP-MATCHER p. 1005)
 */
size_t kmp_search(const char *text, const char *pattern);

/**
 * @brief Busca KMP - todas as ocorrencias
 */
MatchResult kmp_search_all(const char *text, const char *pattern);

// ============================================================================
// RABIN-KARP
// ============================================================================

/**
 * @brief Busca Rabin-Karp - primeira ocorrencia
 *
 * Usa rolling hash para comparar janelas de texto com o padrao.
 * Usa base 256 e primo grande para minimizar colisoes.
 *
 * @param text Texto onde buscar
 * @param pattern Padrao a encontrar
 * @return size_t Posicao da primeira ocorrencia ou SM_NOT_FOUND
 *
 * Complexidade: O(n+m) medio, O(n*m) pior caso (muitas colisoes)
 * Espaco: O(1)
 *
 * Referencia: Cormen S32.2 (pseudocodigo RABIN-KARP-MATCHER p. 993)
 */
size_t rabin_karp_search(const char *text, const char *pattern);

/**
 * @brief Busca Rabin-Karp - todas as ocorrencias
 */
MatchResult rabin_karp_search_all(const char *text, const char *pattern);

// ============================================================================
// BOYER-MOORE
// ============================================================================

/**
 * @brief Busca Boyer-Moore - primeira ocorrencia
 *
 * Combina bad character rule e good suffix rule para pular
 * o maximo de posicoes possivel. Compara da direita para a esquerda.
 *
 * Implementacao simplificada com bad character rule completa
 * e good suffix heuristic.
 *
 * @param text Texto onde buscar
 * @param pattern Padrao a encontrar
 * @return size_t Posicao da primeira ocorrencia ou SM_NOT_FOUND
 *
 * Complexidade: O(n/m) melhor caso (sublinear!), O(n*m) pior caso
 * Espaco: O(k + m) onde k = tamanho do alfabeto (256 para ASCII)
 *
 * Referencia: Cormen S32.3; Boyer & Moore (1977)
 */
size_t boyer_moore_search(const char *text, const char *pattern);

/**
 * @brief Busca Boyer-Moore - todas as ocorrencias
 */
MatchResult boyer_moore_search_all(const char *text, const char *pattern);

#endif // STRING_MATCHING_H
