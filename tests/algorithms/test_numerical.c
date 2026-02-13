/**
 * @file test_numerical.c
 * @brief Testes unitarios para algoritmos numericos
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/numerical.h"
#include "../test_macros.h"

// ============================================================================
// GCD
// ============================================================================

TEST(gcd_basic) {
    ASSERT_EQ(gcd(12, 8), 4);
    ASSERT_EQ(gcd(48, 18), 6);
    ASSERT_EQ(gcd(100, 75), 25);
}

TEST(gcd_coprime) {
    ASSERT_EQ(gcd(7, 13), 1);
    ASSERT_EQ(gcd(17, 31), 1);
}

TEST(gcd_one_zero) {
    ASSERT_EQ(gcd(0, 5), 5);
    ASSERT_EQ(gcd(5, 0), 5);
    ASSERT_EQ(gcd(0, 0), 0);
}

TEST(gcd_negative) {
    ASSERT_EQ(gcd(-12, 8), 4);
    ASSERT_EQ(gcd(12, -8), 4);
    ASSERT_EQ(gcd(-12, -8), 4);
}

TEST(gcd_same) {
    ASSERT_EQ(gcd(7, 7), 7);
}

TEST(lcm_basic) {
    ASSERT_EQ(lcm(4, 6), 12);
    ASSERT_EQ(lcm(12, 18), 36);
    ASSERT_EQ(lcm(7, 13), 91);
}

TEST(lcm_zero) {
    ASSERT_EQ(lcm(0, 5), 0);
    ASSERT_EQ(lcm(5, 0), 0);
}

// ============================================================================
// EXTENDED GCD
// ============================================================================

TEST(extended_gcd_basic) {
    ExtendedGCDResult r = extended_gcd(35, 15);
    ASSERT_EQ(r.gcd, 5);
    ASSERT_EQ(35 * r.x + 15 * r.y, 5);
}

TEST(extended_gcd_coprime) {
    ExtendedGCDResult r = extended_gcd(7, 13);
    ASSERT_EQ(r.gcd, 1);
    ASSERT_EQ(7 * r.x + 13 * r.y, 1);
}

TEST(extended_gcd_large) {
    ExtendedGCDResult r = extended_gcd(240, 46);
    ASSERT_EQ(r.gcd, 2);
    ASSERT_EQ(240 * r.x + 46 * r.y, 2);
}

TEST(extended_gcd_zero) {
    ExtendedGCDResult r = extended_gcd(0, 5);
    ASSERT_EQ(r.gcd, 5);
}

// ============================================================================
// FAST EXPONENTIATION
// ============================================================================

TEST(fast_pow_basic) {
    ASSERT_EQ(fast_pow(2, 10), 1024);
    ASSERT_EQ(fast_pow(3, 5), 243);
    ASSERT_EQ(fast_pow(5, 3), 125);
}

TEST(fast_pow_edge) {
    ASSERT_EQ(fast_pow(2, 0), 1);
    ASSERT_EQ(fast_pow(0, 5), 0);
    ASSERT_EQ(fast_pow(1, 100), 1);
}

TEST(fast_pow_mod_basic) {
    ASSERT_EQ(fast_pow_mod(2, 10, 1000), 24);
    ASSERT_EQ(fast_pow_mod(3, 5, 13), 9);
}

TEST(fast_pow_mod_large) {
    ASSERT_EQ(fast_pow_mod(2, 20, 1000000007), 1048576);
}

TEST(fast_pow_mod_edge) {
    ASSERT_EQ(fast_pow_mod(5, 0, 7), 1);
    ASSERT_EQ(fast_pow_mod(5, 1, 7), 5);
    ASSERT_EQ(fast_pow_mod(100, 5, 1), 0);
}

TEST(fast_pow_mod_fermat) {
    ASSERT_EQ(fast_pow_mod(2, 6, 7), 1);
    ASSERT_EQ(fast_pow_mod(3, 10, 11), 1);
}

// ============================================================================
// SIEVE OF ERATOSTHENES
// ============================================================================

TEST(sieve_small) {
    SieveResult r = sieve_of_eratosthenes(10);
    ASSERT_EQ(r.count, 4);
    ASSERT_EQ(r.primes[0], 2);
    ASSERT_EQ(r.primes[1], 3);
    ASSERT_EQ(r.primes[2], 5);
    ASSERT_EQ(r.primes[3], 7);
    sieve_result_destroy(&r);
}

TEST(sieve_30) {
    SieveResult r = sieve_of_eratosthenes(30);
    ASSERT_EQ(r.count, 10);
    sieve_result_destroy(&r);
}

TEST(sieve_100) {
    SieveResult r = sieve_of_eratosthenes(100);
    ASSERT_EQ(r.count, 25);
    sieve_result_destroy(&r);
}

TEST(sieve_boolean) {
    SieveResult r = sieve_of_eratosthenes(20);
    ASSERT_TRUE(r.is_prime[2]);
    ASSERT_TRUE(r.is_prime[3]);
    ASSERT_FALSE(r.is_prime[4]);
    ASSERT_TRUE(r.is_prime[5]);
    ASSERT_FALSE(r.is_prime[6]);
    ASSERT_TRUE(r.is_prime[7]);
    ASSERT_FALSE(r.is_prime[9]);
    ASSERT_TRUE(r.is_prime[11]);
    ASSERT_TRUE(r.is_prime[19]);
    sieve_result_destroy(&r);
}

TEST(sieve_edge) {
    SieveResult r = sieve_of_eratosthenes(1);
    ASSERT_EQ(r.count, 0);
    sieve_result_destroy(&r);
}

TEST(is_prime_basic) {
    ASSERT_FALSE(is_prime(0));
    ASSERT_FALSE(is_prime(1));
    ASSERT_TRUE(is_prime(2));
    ASSERT_TRUE(is_prime(3));
    ASSERT_FALSE(is_prime(4));
    ASSERT_TRUE(is_prime(5));
    ASSERT_TRUE(is_prime(97));
    ASSERT_FALSE(is_prime(100));
    ASSERT_TRUE(is_prime(997));
}

TEST(is_prime_negative) {
    ASSERT_FALSE(is_prime(-5));
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== Numerical Algorithm Tests ===\n\n");

    printf("[GCD]\n");
    RUN_TEST(gcd_basic);
    RUN_TEST(gcd_coprime);
    RUN_TEST(gcd_one_zero);
    RUN_TEST(gcd_negative);
    RUN_TEST(gcd_same);
    RUN_TEST(lcm_basic);
    RUN_TEST(lcm_zero);

    printf("\n[Extended GCD]\n");
    RUN_TEST(extended_gcd_basic);
    RUN_TEST(extended_gcd_coprime);
    RUN_TEST(extended_gcd_large);
    RUN_TEST(extended_gcd_zero);

    printf("\n[Fast Exponentiation]\n");
    RUN_TEST(fast_pow_basic);
    RUN_TEST(fast_pow_edge);
    RUN_TEST(fast_pow_mod_basic);
    RUN_TEST(fast_pow_mod_large);
    RUN_TEST(fast_pow_mod_edge);
    RUN_TEST(fast_pow_mod_fermat);

    printf("\n[Sieve of Eratosthenes]\n");
    RUN_TEST(sieve_small);
    RUN_TEST(sieve_30);
    RUN_TEST(sieve_100);
    RUN_TEST(sieve_boolean);
    RUN_TEST(sieve_edge);
    RUN_TEST(is_prime_basic);
    RUN_TEST(is_prime_negative);

    printf("\n=== All numerical algorithm tests passed! ===\n");
    return 0;
}
