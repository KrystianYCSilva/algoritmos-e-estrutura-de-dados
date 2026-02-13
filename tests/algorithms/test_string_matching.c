/**
 * @file test_string_matching.c
 * @brief Testes unitarios para algoritmos de busca em strings
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/string_matching.h"
#include "../test_macros.h"

#include <string.h>

// ============================================================================
// NAIVE SEARCH
// ============================================================================

TEST(naive_basic) {
    ASSERT_EQ(naive_search("hello world", "world"), 6);
}

TEST(naive_beginning) {
    ASSERT_EQ(naive_search("abcdef", "abc"), 0);
}

TEST(naive_end) {
    ASSERT_EQ(naive_search("abcdef", "def"), 3);
}

TEST(naive_not_found) {
    ASSERT_EQ(naive_search("abcdef", "xyz"), SM_NOT_FOUND);
}

TEST(naive_empty_pattern) {
    ASSERT_EQ(naive_search("abc", ""), 0);
}

TEST(naive_pattern_longer) {
    ASSERT_EQ(naive_search("abc", "abcdef"), SM_NOT_FOUND);
}

TEST(naive_null_args) {
    ASSERT_EQ(naive_search(NULL, "abc"), SM_NOT_FOUND);
    ASSERT_EQ(naive_search("abc", NULL), SM_NOT_FOUND);
}

TEST(naive_single_char) {
    ASSERT_EQ(naive_search("abcabc", "c"), 2);
}

TEST(naive_all) {
    MatchResult r = naive_search_all("ababababab", "aba");
    ASSERT_EQ(r.count, 4);
    ASSERT_EQ(r.positions[0], 0);
    ASSERT_EQ(r.positions[1], 2);
    ASSERT_EQ(r.positions[2], 4);
    ASSERT_EQ(r.positions[3], 6);
    match_result_destroy(&r);
}

TEST(naive_all_no_match) {
    MatchResult r = naive_search_all("abcdef", "xyz");
    ASSERT_EQ(r.count, 0);
    match_result_destroy(&r);
}

// ============================================================================
// KMP SEARCH
// ============================================================================

TEST(kmp_basic) {
    ASSERT_EQ(kmp_search("hello world", "world"), 6);
}

TEST(kmp_beginning) {
    ASSERT_EQ(kmp_search("abcdef", "abc"), 0);
}

TEST(kmp_not_found) {
    ASSERT_EQ(kmp_search("abcdef", "xyz"), SM_NOT_FOUND);
}

TEST(kmp_null_args) {
    ASSERT_EQ(kmp_search(NULL, "abc"), SM_NOT_FOUND);
    ASSERT_EQ(kmp_search("abc", NULL), SM_NOT_FOUND);
}

TEST(kmp_empty_pattern) {
    ASSERT_EQ(kmp_search("abc", ""), 0);
}

TEST(kmp_repeated_pattern) {
    ASSERT_EQ(kmp_search("aaaaab", "aaab"), 2);
}

TEST(kmp_failure_function) {
    size_t failure[7];
    kmp_compute_failure("ababaca", 7, failure);
    ASSERT_EQ(failure[0], 0);
    ASSERT_EQ(failure[1], 0);
    ASSERT_EQ(failure[2], 1);
    ASSERT_EQ(failure[3], 2);
    ASSERT_EQ(failure[4], 3);
    ASSERT_EQ(failure[5], 0);
    ASSERT_EQ(failure[6], 1);
}

TEST(kmp_all) {
    MatchResult r = kmp_search_all("ababababab", "aba");
    ASSERT_EQ(r.count, 4);
    ASSERT_EQ(r.positions[0], 0);
    ASSERT_EQ(r.positions[1], 2);
    ASSERT_EQ(r.positions[2], 4);
    ASSERT_EQ(r.positions[3], 6);
    match_result_destroy(&r);
}

TEST(kmp_all_single) {
    MatchResult r = kmp_search_all("hello world", "world");
    ASSERT_EQ(r.count, 1);
    ASSERT_EQ(r.positions[0], 6);
    match_result_destroy(&r);
}

// ============================================================================
// RABIN-KARP SEARCH
// ============================================================================

TEST(rabin_karp_basic) {
    ASSERT_EQ(rabin_karp_search("hello world", "world"), 6);
}

TEST(rabin_karp_beginning) {
    ASSERT_EQ(rabin_karp_search("abcdef", "abc"), 0);
}

TEST(rabin_karp_not_found) {
    ASSERT_EQ(rabin_karp_search("abcdef", "xyz"), SM_NOT_FOUND);
}

TEST(rabin_karp_null_args) {
    ASSERT_EQ(rabin_karp_search(NULL, "abc"), SM_NOT_FOUND);
    ASSERT_EQ(rabin_karp_search("abc", NULL), SM_NOT_FOUND);
}

TEST(rabin_karp_repeated) {
    ASSERT_EQ(rabin_karp_search("aaaaab", "aaab"), 2);
}

TEST(rabin_karp_all) {
    MatchResult r = rabin_karp_search_all("ababababab", "aba");
    ASSERT_EQ(r.count, 4);
    ASSERT_EQ(r.positions[0], 0);
    ASSERT_EQ(r.positions[1], 2);
    ASSERT_EQ(r.positions[2], 4);
    ASSERT_EQ(r.positions[3], 6);
    match_result_destroy(&r);
}

// ============================================================================
// BOYER-MOORE SEARCH
// ============================================================================

TEST(boyer_moore_basic) {
    ASSERT_EQ(boyer_moore_search("hello world", "world"), 6);
}

TEST(boyer_moore_beginning) {
    ASSERT_EQ(boyer_moore_search("abcdef", "abc"), 0);
}

TEST(boyer_moore_not_found) {
    ASSERT_EQ(boyer_moore_search("abcdef", "xyz"), SM_NOT_FOUND);
}

TEST(boyer_moore_null_args) {
    ASSERT_EQ(boyer_moore_search(NULL, "abc"), SM_NOT_FOUND);
    ASSERT_EQ(boyer_moore_search("abc", NULL), SM_NOT_FOUND);
}

TEST(boyer_moore_single_char) {
    ASSERT_EQ(boyer_moore_search("abcdef", "d"), 3);
}

TEST(boyer_moore_all) {
    MatchResult r = boyer_moore_search_all("ababababab", "aba");
    ASSERT_EQ(r.count, 4);
    ASSERT_EQ(r.positions[0], 0);
    ASSERT_EQ(r.positions[1], 2);
    ASSERT_EQ(r.positions[2], 4);
    ASSERT_EQ(r.positions[3], 6);
    match_result_destroy(&r);
}

// ============================================================================
// CROSS-ALGORITHM CONSISTENCY
// ============================================================================

TEST(all_algorithms_agree) {
    const char *text = "the quick brown fox jumps over the lazy dog";
    const char *pattern = "the";

    size_t n = naive_search(text, pattern);
    size_t k = kmp_search(text, pattern);
    size_t r = rabin_karp_search(text, pattern);
    size_t b = boyer_moore_search(text, pattern);

    ASSERT_EQ(n, 0);
    ASSERT_EQ(k, 0);
    ASSERT_EQ(r, 0);
    ASSERT_EQ(b, 0);

    MatchResult rn = naive_search_all(text, pattern);
    MatchResult rk = kmp_search_all(text, pattern);
    MatchResult rr = rabin_karp_search_all(text, pattern);
    MatchResult rb = boyer_moore_search_all(text, pattern);

    ASSERT_EQ(rn.count, 2);
    ASSERT_EQ(rk.count, 2);
    ASSERT_EQ(rr.count, 2);
    ASSERT_EQ(rb.count, 2);

    for (size_t i = 0; i < rn.count; i++) {
        ASSERT_EQ(rn.positions[i], rk.positions[i]);
        ASSERT_EQ(rn.positions[i], rr.positions[i]);
        ASSERT_EQ(rn.positions[i], rb.positions[i]);
    }

    match_result_destroy(&rn);
    match_result_destroy(&rk);
    match_result_destroy(&rr);
    match_result_destroy(&rb);
}

TEST(stress_long_text) {
    char text[1001];
    memset(text, 'a', 1000);
    text[1000] = '\0';
    text[997] = 'b';

    size_t n = naive_search(text, "aab");
    size_t k = kmp_search(text, "aab");
    size_t r = rabin_karp_search(text, "aab");
    size_t b = boyer_moore_search(text, "aab");

    ASSERT_EQ(n, 995);
    ASSERT_EQ(k, 995);
    ASSERT_EQ(r, 995);
    ASSERT_EQ(b, 995);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("=== String Matching Tests ===\n\n");

    printf("[Naive Search]\n");
    RUN_TEST(naive_basic);
    RUN_TEST(naive_beginning);
    RUN_TEST(naive_end);
    RUN_TEST(naive_not_found);
    RUN_TEST(naive_empty_pattern);
    RUN_TEST(naive_pattern_longer);
    RUN_TEST(naive_null_args);
    RUN_TEST(naive_single_char);
    RUN_TEST(naive_all);
    RUN_TEST(naive_all_no_match);

    printf("\n[KMP Search]\n");
    RUN_TEST(kmp_basic);
    RUN_TEST(kmp_beginning);
    RUN_TEST(kmp_not_found);
    RUN_TEST(kmp_null_args);
    RUN_TEST(kmp_empty_pattern);
    RUN_TEST(kmp_repeated_pattern);
    RUN_TEST(kmp_failure_function);
    RUN_TEST(kmp_all);
    RUN_TEST(kmp_all_single);

    printf("\n[Rabin-Karp Search]\n");
    RUN_TEST(rabin_karp_basic);
    RUN_TEST(rabin_karp_beginning);
    RUN_TEST(rabin_karp_not_found);
    RUN_TEST(rabin_karp_null_args);
    RUN_TEST(rabin_karp_repeated);
    RUN_TEST(rabin_karp_all);

    printf("\n[Boyer-Moore Search]\n");
    RUN_TEST(boyer_moore_basic);
    RUN_TEST(boyer_moore_beginning);
    RUN_TEST(boyer_moore_not_found);
    RUN_TEST(boyer_moore_null_args);
    RUN_TEST(boyer_moore_single_char);
    RUN_TEST(boyer_moore_all);

    printf("\n[Cross-Algorithm Consistency]\n");
    RUN_TEST(all_algorithms_agree);
    RUN_TEST(stress_long_text);

    printf("\n=== All string matching tests passed! ===\n");
    return 0;
}
