/**
 * @file numerical.c
 * @brief Implementacao de 4 algoritmos numericos classicos
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 31
 * - Knuth (1997), TAOCP Vol 2, Chapter 4
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/numerical.h"

#include <stdlib.h>
#include <math.h>

// ============================================================================
// GCD - Cormen S31.2
// ============================================================================

long long gcd(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long lcm(long long a, long long b) {
    if (a == 0 || b == 0) return 0;
    long long abs_a = (a < 0) ? -a : a;
    long long abs_b = (b < 0) ? -b : b;
    return abs_a / gcd(abs_a, abs_b) * abs_b;
}

// ============================================================================
// EXTENDED GCD - Cormen S31.2
// ============================================================================

ExtendedGCDResult extended_gcd(long long a, long long b) {
    ExtendedGCDResult result;

    if (b == 0) {
        result.gcd = (a < 0) ? -a : a;
        result.x = (a < 0) ? -1 : 1;
        result.y = 0;
        return result;
    }

    long long old_r = a, r = b;
    long long old_s = 1, s = 0;
    long long old_t = 0, t = 1;

    while (r != 0) {
        long long q = old_r / r;
        long long temp;

        temp = r;
        r = old_r - q * r;
        old_r = temp;

        temp = s;
        s = old_s - q * s;
        old_s = temp;

        temp = t;
        t = old_t - q * t;
        old_t = temp;
    }

    result.gcd = old_r;
    result.x = old_s;
    result.y = old_t;

    if (result.gcd < 0) {
        result.gcd = -result.gcd;
        result.x = -result.x;
        result.y = -result.y;
    }

    return result;
}

// ============================================================================
// FAST EXPONENTIATION - Cormen S31.6
// ============================================================================

long long fast_pow_mod(long long base, long long exp, long long mod) {
    if (mod <= 0) return 0;
    if (mod == 1) return 0;
    if (exp < 0) return 0;

    long long result = 1;
    base = base % mod;
    if (base < 0) base += mod;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

long long fast_pow(long long base, long long exp) {
    if (exp < 0) return 0;
    if (exp == 0) return 1;

    long long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

// ============================================================================
// SIEVE OF ERATOSTHENES - Cormen S31.8
// ============================================================================

SieveResult sieve_of_eratosthenes(size_t limit) {
    SieveResult result = { NULL, NULL, 0, limit };

    if (limit < 2) return result;

    result.is_prime = calloc(limit + 1, sizeof(bool));
    if (result.is_prime == NULL) return result;

    for (size_t i = 2; i <= limit; i++) {
        result.is_prime[i] = true;
    }

    for (size_t i = 2; i * i <= limit; i++) {
        if (result.is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                result.is_prime[j] = false;
            }
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; i++) {
        if (result.is_prime[i]) count++;
    }

    result.primes = malloc(count * sizeof(int));
    if (result.primes == NULL) {
        free(result.is_prime);
        result.is_prime = NULL;
        return result;
    }

    result.count = count;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; i++) {
        if (result.is_prime[i]) {
            result.primes[idx++] = (int)i;
        }
    }

    return result;
}

bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void sieve_result_destroy(SieveResult *result) {
    if (result == NULL) return;
    free(result->is_prime);
    free(result->primes);
    result->is_prime = NULL;
    result->primes = NULL;
    result->count = 0;
}
