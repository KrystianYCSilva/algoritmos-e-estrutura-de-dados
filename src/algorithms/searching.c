/**
 * @file searching.c
 * @brief Implementacao de algoritmos de busca
 *
 * Referencias:
 * - Cormen et al. (2009), Chapter 2
 * - Knuth (1998), TAOCP Vol 3, Chapter 6
 * - Sedgewick & Wayne (2011), Chapter 3
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/searching.h"

#include <math.h>

static inline const void *celem_at(const void *arr, size_t i, size_t size) {
    return (const unsigned char *)arr + i * size;
}

// ============================================================================
// LINEAR SEARCH - Cormen S2.1
// ============================================================================

size_t linear_search(const void *arr, size_t n, size_t elem_size,
                     const void *target, CompareFn cmp) {
    if (arr == NULL || target == NULL || cmp == NULL) return SEARCH_NOT_FOUND;

    for (size_t i = 0; i < n; i++) {
        if (cmp(celem_at(arr, i, elem_size), target) == 0)
            return i;
    }
    return SEARCH_NOT_FOUND;
}

// ============================================================================
// BINARY SEARCH - Cormen S2.3
// ============================================================================

size_t binary_search(const void *arr, size_t n, size_t elem_size,
                     const void *target, CompareFn cmp) {
    if (arr == NULL || target == NULL || cmp == NULL || n == 0)
        return SEARCH_NOT_FOUND;

    size_t lo = 0, hi = n - 1;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        int result = cmp(celem_at(arr, mid, elem_size), target);
        if (result == 0) return mid;
        if (result < 0) lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return SEARCH_NOT_FOUND;
}

// ============================================================================
// INTERPOLATION SEARCH - Knuth TAOCP 3 S6.2.1
// ============================================================================

size_t interpolation_search(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return SEARCH_NOT_FOUND;

    size_t lo = 0, hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        if (lo == hi) {
            if (arr[lo] == target) return lo;
            return SEARCH_NOT_FOUND;
        }

        size_t pos = lo + (size_t)(((double)(target - arr[lo]) /
                     (double)(arr[hi] - arr[lo])) * (double)(hi - lo));

        if (pos > hi) pos = hi;

        if (arr[pos] == target) return pos;
        if (arr[pos] < target) lo = pos + 1;
        else {
            if (pos == 0) break;
            hi = pos - 1;
        }
    }
    return SEARCH_NOT_FOUND;
}

// ============================================================================
// TERNARY SEARCH - Sedgewick S3.1
// ============================================================================

size_t ternary_search(const void *arr, size_t n, size_t elem_size,
                      const void *target, CompareFn cmp) {
    if (arr == NULL || target == NULL || cmp == NULL || n == 0)
        return SEARCH_NOT_FOUND;

    size_t lo = 0, hi = n - 1;
    while (lo <= hi) {
        size_t third = (hi - lo) / 3;
        size_t m1 = lo + third;
        size_t m2 = hi - third;

        int r1 = cmp(celem_at(arr, m1, elem_size), target);
        if (r1 == 0) return m1;

        int r2 = cmp(celem_at(arr, m2, elem_size), target);
        if (r2 == 0) return m2;

        if (r1 > 0) {
            if (m1 == 0) break;
            hi = m1 - 1;
        } else if (r2 < 0) {
            lo = m2 + 1;
        } else {
            lo = m1 + 1;
            hi = m2 - 1;
        }
    }
    return SEARCH_NOT_FOUND;
}

// ============================================================================
// JUMP SEARCH - Knuth TAOCP 3 S6.2.1
// ============================================================================

size_t jump_search(const void *arr, size_t n, size_t elem_size,
                   const void *target, CompareFn cmp) {
    if (arr == NULL || target == NULL || cmp == NULL || n == 0)
        return SEARCH_NOT_FOUND;

    size_t step = (size_t)sqrt((double)n);
    size_t prev = 0;
    size_t curr = step;

    while (curr < n && cmp(celem_at(arr, curr, elem_size), target) < 0) {
        prev = curr + 1;
        curr += step;
    }

    for (size_t i = prev; i < n && i <= curr; i++) {
        if (cmp(celem_at(arr, i, elem_size), target) == 0)
            return i;
    }
    return SEARCH_NOT_FOUND;
}

// ============================================================================
// EXPONENTIAL SEARCH - combina busca exponencial + binaria
// ============================================================================

size_t exponential_search(const void *arr, size_t n, size_t elem_size,
                          const void *target, CompareFn cmp) {
    if (arr == NULL || target == NULL || cmp == NULL || n == 0)
        return SEARCH_NOT_FOUND;

    if (cmp(celem_at(arr, 0, elem_size), target) == 0) return 0;

    size_t bound = 1;
    while (bound < n && cmp(celem_at(arr, bound, elem_size), target) < 0) {
        bound *= 2;
    }

    size_t lo = bound / 2;
    size_t hi = (bound < n) ? bound : n - 1;

    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        int result = cmp(celem_at(arr, mid, elem_size), target);
        if (result == 0) return mid;
        if (result < 0) lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return SEARCH_NOT_FOUND;
}
