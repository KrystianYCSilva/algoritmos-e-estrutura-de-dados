/**
 * @file string_matching.c
 * @brief Implementacao de 4 algoritmos classicos de busca em strings
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 32.1-32.4
 * - Knuth, Morris, Pratt (1977), "Fast pattern matching in strings"
 * - Boyer, Moore (1977), "A fast string searching algorithm"
 * - Karp, Rabin (1987), "Efficient randomized pattern-matching algorithms"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/string_matching.h"

#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256
#define RK_BASE 256
#define RK_PRIME 101

// ============================================================================
// MATCH RESULT
// ============================================================================

static bool match_result_append(MatchResult *result, size_t pos) {
    if (result->count == result->capacity) {
        size_t new_cap = (result->capacity == 0) ? 8 : result->capacity * 2;
        size_t *new_arr = realloc(result->positions, new_cap * sizeof(size_t));
        if (new_arr == NULL) return false;
        result->positions = new_arr;
        result->capacity = new_cap;
    }
    result->positions[result->count++] = pos;
    return true;
}

MatchResult match_result_create(void) {
    MatchResult r = { NULL, 0, 0 };
    return r;
}

void match_result_destroy(MatchResult *result) {
    if (result == NULL) return;
    free(result->positions);
    result->positions = NULL;
    result->count = 0;
    result->capacity = 0;
}

// ============================================================================
// NAIVE - Cormen S32.1
// ============================================================================

size_t naive_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return SM_NOT_FOUND;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return SM_NOT_FOUND;

    for (size_t s = 0; s <= n - m; s++) {
        size_t j = 0;
        while (j < m && text[s + j] == pattern[j]) {
            j++;
        }
        if (j == m) return s;
    }

    return SM_NOT_FOUND;
}

MatchResult naive_search_all(const char *text, const char *pattern) {
    MatchResult result = match_result_create();
    if (text == NULL || pattern == NULL) return result;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0 || m > n) return result;

    for (size_t s = 0; s <= n - m; s++) {
        size_t j = 0;
        while (j < m && text[s + j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            match_result_append(&result, s);
        }
    }

    return result;
}

// ============================================================================
// KMP - Cormen S32.4
// ============================================================================

void kmp_compute_failure(const char *pattern, size_t m, size_t *failure) {
    if (pattern == NULL || failure == NULL || m == 0) return;

    failure[0] = 0;
    size_t k = 0;

    for (size_t q = 1; q < m; q++) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = failure[k - 1];
        }
        if (pattern[k] == pattern[q]) {
            k++;
        }
        failure[q] = k;
    }
}

size_t kmp_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return SM_NOT_FOUND;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return SM_NOT_FOUND;

    size_t *failure = malloc(m * sizeof(size_t));
    if (failure == NULL) return SM_NOT_FOUND;

    kmp_compute_failure(pattern, m, failure);

    size_t q = 0;
    for (size_t i = 0; i < n; i++) {
        while (q > 0 && pattern[q] != text[i]) {
            q = failure[q - 1];
        }
        if (pattern[q] == text[i]) {
            q++;
        }
        if (q == m) {
            free(failure);
            return i - m + 1;
        }
    }

    free(failure);
    return SM_NOT_FOUND;
}

MatchResult kmp_search_all(const char *text, const char *pattern) {
    MatchResult result = match_result_create();
    if (text == NULL || pattern == NULL) return result;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0 || m > n) return result;

    size_t *failure = malloc(m * sizeof(size_t));
    if (failure == NULL) return result;

    kmp_compute_failure(pattern, m, failure);

    size_t q = 0;
    for (size_t i = 0; i < n; i++) {
        while (q > 0 && pattern[q] != text[i]) {
            q = failure[q - 1];
        }
        if (pattern[q] == text[i]) {
            q++;
        }
        if (q == m) {
            match_result_append(&result, i - m + 1);
            q = failure[q - 1];
        }
    }

    free(failure);
    return result;
}

// ============================================================================
// RABIN-KARP - Cormen S32.2
// ============================================================================

size_t rabin_karp_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return SM_NOT_FOUND;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return SM_NOT_FOUND;

    long long h = 1;
    for (size_t i = 0; i < m - 1; i++) {
        h = (h * RK_BASE) % RK_PRIME;
    }

    long long p_hash = 0;
    long long t_hash = 0;

    for (size_t i = 0; i < m; i++) {
        p_hash = (RK_BASE * p_hash + (unsigned char)pattern[i]) % RK_PRIME;
        t_hash = (RK_BASE * t_hash + (unsigned char)text[i]) % RK_PRIME;
    }

    for (size_t s = 0; s <= n - m; s++) {
        if (p_hash == t_hash) {
            if (memcmp(text + s, pattern, m) == 0) {
                return s;
            }
        }

        if (s < n - m) {
            t_hash = (RK_BASE * (t_hash - (unsigned char)text[s] * h) +
                       (unsigned char)text[s + m]) % RK_PRIME;
            if (t_hash < 0) t_hash += RK_PRIME;
        }
    }

    return SM_NOT_FOUND;
}

MatchResult rabin_karp_search_all(const char *text, const char *pattern) {
    MatchResult result = match_result_create();
    if (text == NULL || pattern == NULL) return result;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0 || m > n) return result;

    long long h = 1;
    for (size_t i = 0; i < m - 1; i++) {
        h = (h * RK_BASE) % RK_PRIME;
    }

    long long p_hash = 0;
    long long t_hash = 0;

    for (size_t i = 0; i < m; i++) {
        p_hash = (RK_BASE * p_hash + (unsigned char)pattern[i]) % RK_PRIME;
        t_hash = (RK_BASE * t_hash + (unsigned char)text[i]) % RK_PRIME;
    }

    for (size_t s = 0; s <= n - m; s++) {
        if (p_hash == t_hash) {
            if (memcmp(text + s, pattern, m) == 0) {
                match_result_append(&result, s);
            }
        }

        if (s < n - m) {
            t_hash = (RK_BASE * (t_hash - (unsigned char)text[s] * h) +
                       (unsigned char)text[s + m]) % RK_PRIME;
            if (t_hash < 0) t_hash += RK_PRIME;
        }
    }

    return result;
}

// ============================================================================
// BOYER-MOORE - Cormen S32.3
// ============================================================================

static void compute_bad_char(const char *pattern, size_t m, int bad_char[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = -1;
    }
    for (size_t i = 0; i < m; i++) {
        bad_char[(unsigned char)pattern[i]] = (int)i;
    }
}

static void compute_good_suffix(const char *pattern, size_t m, size_t *good_suffix) {
    size_t *suffix = malloc(m * sizeof(size_t));
    if (suffix == NULL) {
        for (size_t i = 0; i < m; i++) good_suffix[i] = m;
        return;
    }

    suffix[m - 1] = m;
    size_t g = m - 1;
    size_t f = 0;

    for (size_t i = m - 1; i > 0; i--) {
        size_t idx = i - 1;
        if (idx > g && suffix[idx + m - 1 - f] < idx - g) {
            suffix[idx] = suffix[idx + m - 1 - f];
        } else {
            if (idx < g) g = idx;
            f = idx;
            while (g < m && pattern[g] == pattern[g + m - 1 - f]) {
                if (g == 0) { g--; break; }
                g--;
            }
            suffix[idx] = f - g;
        }
    }

    for (size_t i = 0; i < m; i++) {
        good_suffix[i] = m;
    }

    size_t j = 0;
    for (size_t i = m; i > 0; i--) {
        size_t idx = i - 1;
        if (suffix[idx] == idx + 1) {
            while (j < m - 1 - idx) {
                if (good_suffix[j] == m) {
                    good_suffix[j] = m - 1 - idx;
                }
                j++;
            }
        }
    }

    for (size_t i = 0; i < m - 1; i++) {
        good_suffix[m - 1 - suffix[i]] = m - 1 - i;
    }

    free(suffix);
}

size_t boyer_moore_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return SM_NOT_FOUND;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return SM_NOT_FOUND;

    int bad_char[ALPHABET_SIZE];
    compute_bad_char(pattern, m, bad_char);

    size_t *good_suffix = malloc(m * sizeof(size_t));
    if (good_suffix == NULL) return SM_NOT_FOUND;
    compute_good_suffix(pattern, m, good_suffix);

    size_t s = 0;
    while (s <= n - m) {
        size_t j = m;
        while (j > 0 && pattern[j - 1] == text[s + j - 1]) {
            j--;
        }

        if (j == 0) {
            free(good_suffix);
            return s;
        }

        size_t bc_shift = 0;
        int bc_val = bad_char[(unsigned char)text[s + j - 1]];
        if ((int)(j - 1) > bc_val) {
            bc_shift = (size_t)((int)(j - 1) - bc_val);
        } else {
            bc_shift = 1;
        }

        size_t gs_shift = good_suffix[j - 1];
        s += (bc_shift > gs_shift) ? bc_shift : gs_shift;
    }

    free(good_suffix);
    return SM_NOT_FOUND;
}

MatchResult boyer_moore_search_all(const char *text, const char *pattern) {
    MatchResult result = match_result_create();
    if (text == NULL || pattern == NULL) return result;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0 || m > n) return result;

    int bad_char[ALPHABET_SIZE];
    compute_bad_char(pattern, m, bad_char);

    size_t *good_suffix = malloc(m * sizeof(size_t));
    if (good_suffix == NULL) return result;
    compute_good_suffix(pattern, m, good_suffix);

    size_t s = 0;
    while (s <= n - m) {
        size_t j = m;
        while (j > 0 && pattern[j - 1] == text[s + j - 1]) {
            j--;
        }

        if (j == 0) {
            match_result_append(&result, s);
            s += good_suffix[0];
        } else {
            size_t bc_shift = 0;
            int bc_val = bad_char[(unsigned char)text[s + j - 1]];
            if ((int)(j - 1) > bc_val) {
                bc_shift = (size_t)((int)(j - 1) - bc_val);
            } else {
                bc_shift = 1;
            }

            size_t gs_shift = good_suffix[j - 1];
            s += (bc_shift > gs_shift) ? bc_shift : gs_shift;
        }
    }

    free(good_suffix);
    return result;
}
