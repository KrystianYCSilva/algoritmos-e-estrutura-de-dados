/**
 * @file numerical.h
 * @brief Algoritmos Numericos (Numerical Algorithms)
 *
 * Implementacao de 4 algoritmos numericos classicos:
 * 1. Euclidean GCD - maximo divisor comum
 * 2. Extended GCD - GCD estendido com coeficientes de Bezout
 * 3. Fast Exponentiation - exponenciacao rapida (modular)
 * 4. Sieve of Eratosthenes - crivo de Eratostenes
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 31
 * - Knuth (1997), TAOCP Vol 2, Chapter 4
 * - Euclid, "Elements", Book VII, Propositions 1-2 (~300 BC)
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// GCD - Euclidean Algorithm
// ============================================================================

/**
 * @brief GCD iterativo (algoritmo de Euclides)
 *
 * @param a Primeiro inteiro (nao-negativo)
 * @param b Segundo inteiro (nao-negativo)
 * @return long long GCD(a, b)
 *
 * Complexidade: O(log(min(a,b)))
 * Referencia: Cormen S31.2 (pseudocodigo EUCLID p. 935); Knuth TAOCP 2 S4.5.2
 */
long long gcd(long long a, long long b);

/**
 * @brief LCM - minimo multiplo comum
 *
 * @return long long LCM(a, b) = |a*b| / GCD(a,b)
 */
long long lcm(long long a, long long b);

// ============================================================================
// EXTENDED GCD
// ============================================================================

/**
 * @brief Resultado do GCD estendido
 *
 * Encontra x, y tal que a*x + b*y = gcd(a, b)
 */
typedef struct {
    long long gcd;
    long long x;
    long long y;
} ExtendedGCDResult;

/**
 * @brief GCD estendido (coeficientes de Bezout)
 *
 * Encontra gcd(a,b) e coeficientes x,y tais que a*x + b*y = gcd(a,b).
 *
 * @param a Primeiro inteiro
 * @param b Segundo inteiro
 * @return ExtendedGCDResult com gcd, x, y
 *
 * Complexidade: O(log(min(a,b)))
 * Referencia: Cormen S31.2 (pseudocodigo EXTENDED-EUCLID p. 937)
 */
ExtendedGCDResult extended_gcd(long long a, long long b);

// ============================================================================
// FAST EXPONENTIATION
// ============================================================================

/**
 * @brief Exponenciacao rapida modular: (base^exp) mod mod
 *
 * Usa repeated squaring para calcular em O(log exp).
 *
 * @param base Base
 * @param exp Expoente (nao-negativo)
 * @param mod Modulo (deve ser > 0)
 * @return long long (base^exp) mod mod
 *
 * Complexidade: O(log exp)
 * Referencia: Cormen S31.6 (pseudocodigo MODULAR-EXPONENTIATION p. 957)
 */
long long fast_pow_mod(long long base, long long exp, long long mod);

/**
 * @brief Exponenciacao rapida sem modulo: base^exp
 *
 * @param base Base
 * @param exp Expoente (nao-negativo)
 * @return long long base^exp (cuidado com overflow)
 *
 * Complexidade: O(log exp)
 * Referencia: Knuth TAOCP 2 S4.6.3
 */
long long fast_pow(long long base, long long exp);

// ============================================================================
// SIEVE OF ERATOSTHENES
// ============================================================================

/**
 * @brief Resultado do Crivo de Eratostenes
 */
typedef struct {
    bool *is_prime;
    int *primes;
    size_t count;
    size_t limit;
} SieveResult;

/**
 * @brief Crivo de Eratostenes - encontra todos os primos ate limit
 *
 * @param limit Limite superior (inclusive)
 * @return SieveResult com array booleano e lista de primos
 *
 * Complexidade: O(n log log n) tempo, O(n) espaco
 * Referencia: Cormen S31.8; Eratostenes (~240 BC)
 */
SieveResult sieve_of_eratosthenes(size_t limit);

/**
 * @brief Verifica se um numero e primo (usando trial division)
 *
 * @param n Numero a verificar
 * @return bool true se primo
 *
 * Complexidade: O(sqrt(n))
 */
bool is_prime(long long n);

/**
 * @brief Libera memoria de SieveResult
 */
void sieve_result_destroy(SieveResult *result);

#endif // NUMERICAL_H
