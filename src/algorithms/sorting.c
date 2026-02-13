/**
 * @file sorting.c
 * @brief Implementacao de 10 algoritmos classicos de ordenacao
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 2, 6, 7, 8
 * - Knuth (1998), TAOCP Vol 3, Chapter 5
 * - Sedgewick & Wayne (2011), Chapter 2
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/sorting.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// HELPERS INTERNOS
// ============================================================================

static inline void swap_elements(void *a, void *b, size_t size) {
    unsigned char tmp[size];
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

static inline void *elem_at(void *arr, size_t i, size_t size) {
    return (unsigned char *)arr + i * size;
}

static inline const void *celem_at(const void *arr, size_t i, size_t size) {
    return (const unsigned char *)arr + i * size;
}

// ============================================================================
// BUBBLE SORT - Cormen Problem 2-2
// ============================================================================

void bubble_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < n - 1 - i; j++) {
            void *a = elem_at(arr, j, elem_size);
            void *b = elem_at(arr, j + 1, elem_size);
            if (cmp(a, b) > 0) {
                swap_elements(a, b, elem_size);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ============================================================================
// SELECTION SORT - Cormen Exercise 2.2-2
// ============================================================================

void selection_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; j++) {
            if (cmp(elem_at(arr, j, elem_size), elem_at(arr, min_idx, elem_size)) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap_elements(elem_at(arr, i, elem_size), elem_at(arr, min_idx, elem_size), elem_size);
        }
    }
}

// ============================================================================
// INSERTION SORT - Cormen S2.1 (pseudocodigo p. 18)
// ============================================================================

void insertion_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    unsigned char key[elem_size];

    for (size_t i = 1; i < n; i++) {
        memcpy(key, elem_at(arr, i, elem_size), elem_size);
        size_t j = i;
        while (j > 0 && cmp(elem_at(arr, j - 1, elem_size), key) > 0) {
            memcpy(elem_at(arr, j, elem_size), elem_at(arr, j - 1, elem_size), elem_size);
            j--;
        }
        memcpy(elem_at(arr, j, elem_size), key, elem_size);
    }
}

// ============================================================================
// SHELL SORT - Knuth TAOCP 3 S5.2.1 (gap sequence: h = 3h + 1)
// ============================================================================

void shell_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    size_t gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;

    unsigned char key[elem_size];

    while (gap >= 1) {
        for (size_t i = gap; i < n; i++) {
            memcpy(key, elem_at(arr, i, elem_size), elem_size);
            size_t j = i;
            while (j >= gap && cmp(elem_at(arr, j - gap, elem_size), key) > 0) {
                memcpy(elem_at(arr, j, elem_size), elem_at(arr, j - gap, elem_size), elem_size);
                j -= gap;
            }
            memcpy(elem_at(arr, j, elem_size), key, elem_size);
        }
        gap /= 3;
    }
}

// ============================================================================
// MERGE SORT - Cormen S2.3 (pseudocodigo MERGE p. 31)
// ============================================================================

static void merge(void *arr, size_t left, size_t mid, size_t right,
                  size_t elem_size, CompareFn cmp, void *temp) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    unsigned char *L = (unsigned char *)temp;
    unsigned char *R = L + n1 * elem_size;

    memcpy(L, elem_at(arr, left, elem_size), n1 * elem_size);
    memcpy(R, elem_at(arr, mid + 1, elem_size), n2 * elem_size);

    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp(L + i * elem_size, R + j * elem_size) <= 0) {
            memcpy(elem_at(arr, k, elem_size), L + i * elem_size, elem_size);
            i++;
        } else {
            memcpy(elem_at(arr, k, elem_size), R + j * elem_size, elem_size);
            j++;
        }
        k++;
    }
    while (i < n1) {
        memcpy(elem_at(arr, k, elem_size), L + i * elem_size, elem_size);
        i++; k++;
    }
    while (j < n2) {
        memcpy(elem_at(arr, k, elem_size), R + j * elem_size, elem_size);
        j++; k++;
    }
}

static void merge_sort_recursive(void *arr, size_t left, size_t right,
                                  size_t elem_size, CompareFn cmp, void *temp) {
    if (left >= right) return;

    size_t mid = left + (right - left) / 2;
    merge_sort_recursive(arr, left, mid, elem_size, cmp, temp);
    merge_sort_recursive(arr, mid + 1, right, elem_size, cmp, temp);
    merge(arr, left, mid, right, elem_size, cmp, temp);
}

void merge_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    void *temp = malloc(n * elem_size);
    if (temp == NULL) return;

    merge_sort_recursive(arr, 0, n - 1, elem_size, cmp, temp);
    free(temp);
}

// ============================================================================
// QUICK SORT - Cormen S7 (PARTITION p. 171, median-of-three)
// ============================================================================

static size_t median_of_three(void *arr, size_t lo, size_t hi,
                               size_t elem_size, CompareFn cmp) {
    size_t mid = lo + (hi - lo) / 2;
    if (cmp(elem_at(arr, lo, elem_size), elem_at(arr, mid, elem_size)) > 0)
        swap_elements(elem_at(arr, lo, elem_size), elem_at(arr, mid, elem_size), elem_size);
    if (cmp(elem_at(arr, lo, elem_size), elem_at(arr, hi, elem_size)) > 0)
        swap_elements(elem_at(arr, lo, elem_size), elem_at(arr, hi, elem_size), elem_size);
    if (cmp(elem_at(arr, mid, elem_size), elem_at(arr, hi, elem_size)) > 0)
        swap_elements(elem_at(arr, mid, elem_size), elem_at(arr, hi, elem_size), elem_size);
    return mid;
}

static size_t partition(void *arr, size_t lo, size_t hi,
                        size_t elem_size, CompareFn cmp) {
    size_t pivot_idx = median_of_three(arr, lo, hi, elem_size, cmp);
    swap_elements(elem_at(arr, pivot_idx, elem_size), elem_at(arr, hi, elem_size), elem_size);

    void *pivot = elem_at(arr, hi, elem_size);
    size_t i = lo;

    for (size_t j = lo; j < hi; j++) {
        if (cmp(elem_at(arr, j, elem_size), pivot) <= 0) {
            swap_elements(elem_at(arr, i, elem_size), elem_at(arr, j, elem_size), elem_size);
            i++;
        }
    }
    swap_elements(elem_at(arr, i, elem_size), elem_at(arr, hi, elem_size), elem_size);
    return i;
}

static void quick_sort_recursive(void *arr, size_t lo, size_t hi,
                                  size_t elem_size, CompareFn cmp) {
    if (lo >= hi) return;

    if (hi - lo < 16) {
        unsigned char *sub = (unsigned char *)arr + lo * elem_size;
        insertion_sort(sub, hi - lo + 1, elem_size, cmp);
        return;
    }

    size_t p = partition(arr, lo, hi, elem_size, cmp);
    if (p > 0) quick_sort_recursive(arr, lo, p - 1, elem_size, cmp);
    quick_sort_recursive(arr, p + 1, hi, elem_size, cmp);
}

void quick_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;
    quick_sort_recursive(arr, 0, n - 1, elem_size, cmp);
}

// ============================================================================
// HEAP SORT - Cormen S6.4 (HEAPSORT p. 160)
// ============================================================================

static void sift_down(void *arr, size_t n, size_t i, size_t elem_size, CompareFn cmp) {
    size_t largest = i;

    for (;;) {
        size_t left = 2 * largest + 1;
        size_t right = 2 * largest + 2;
        size_t next = largest;

        if (left < n && cmp(elem_at(arr, left, elem_size), elem_at(arr, next, elem_size)) > 0)
            next = left;
        if (right < n && cmp(elem_at(arr, right, elem_size), elem_at(arr, next, elem_size)) > 0)
            next = right;

        if (next == largest) break;
        swap_elements(elem_at(arr, largest, elem_size), elem_at(arr, next, elem_size), elem_size);
        largest = next;
    }
}

void heap_sort(void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return;

    for (size_t i = n / 2; i > 0; i--) {
        sift_down(arr, n, i - 1, elem_size, cmp);
    }

    for (size_t i = n - 1; i > 0; i--) {
        swap_elements(elem_at(arr, 0, elem_size), elem_at(arr, i, elem_size), elem_size);
        sift_down(arr, i, 0, elem_size, cmp);
    }
}

// ============================================================================
// COUNTING SORT - Cormen S8.2 (pseudocodigo p. 195)
// ============================================================================

void counting_sort(int *arr, size_t n, int max_val) {
    if (arr == NULL || n <= 1 || max_val < 0) return;

    size_t k = (size_t)max_val + 1;
    int *count = (int *)calloc(k, sizeof(int));
    int *output = (int *)malloc(n * sizeof(int));
    if (count == NULL || output == NULL) {
        free(count);
        free(output);
        return;
    }

    for (size_t i = 0; i < n; i++)
        count[arr[i]]++;

    for (size_t i = 1; i < k; i++)
        count[i] += count[i - 1];

    for (size_t i = n; i > 0; i--) {
        output[count[arr[i - 1]] - 1] = arr[i - 1];
        count[arr[i - 1]]--;
    }

    memcpy(arr, output, n * sizeof(int));
    free(count);
    free(output);
}

// ============================================================================
// RADIX SORT - Cormen S8.3 (LSD, base 10)
// ============================================================================

static void counting_sort_by_digit(int *arr, size_t n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};
    if (output == NULL) return;

    for (size_t i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (size_t i = n; i > 0; i--) {
        int digit = (arr[i - 1] / exp) % 10;
        output[count[digit] - 1] = arr[i - 1];
        count[digit]--;
    }

    memcpy(arr, output, n * sizeof(int));
    free(output);
}

void radix_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) return;

    int max_val = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }

    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        counting_sort_by_digit(arr, n, exp);
    }
}

// ============================================================================
// BUCKET SORT - Cormen S8.4 (para doubles em [0.0, 1.0))
// ============================================================================

typedef struct BucketNode {
    double value;
    struct BucketNode *next;
} BucketNode;

static void bucket_insert_sorted(BucketNode **head, double value) {
    BucketNode *node = (BucketNode *)malloc(sizeof(BucketNode));
    if (node == NULL) return;
    node->value = value;
    node->next = NULL;

    if (*head == NULL || value < (*head)->value) {
        node->next = *head;
        *head = node;
        return;
    }

    BucketNode *curr = *head;
    while (curr->next != NULL && curr->next->value <= value) {
        curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
}

void bucket_sort(double *arr, size_t n) {
    if (arr == NULL || n <= 1) return;

    BucketNode **buckets = (BucketNode **)calloc(n, sizeof(BucketNode *));
    if (buckets == NULL) return;

    for (size_t i = 0; i < n; i++) {
        size_t idx = (size_t)(arr[i] * (double)n);
        if (idx >= n) idx = n - 1;
        bucket_insert_sorted(&buckets[idx], arr[i]);
    }

    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        BucketNode *curr = buckets[i];
        while (curr != NULL) {
            arr[k++] = curr->value;
            BucketNode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }

    free(buckets);
}

// ============================================================================
// IS_SORTED - Utilidade
// ============================================================================

bool is_sorted(const void *arr, size_t n, size_t elem_size, CompareFn cmp) {
    if (arr == NULL || cmp == NULL || n <= 1) return true;

    for (size_t i = 0; i < n - 1; i++) {
        if (cmp(celem_at(arr, i, elem_size), celem_at(arr, i + 1, elem_size)) > 0)
            return false;
    }
    return true;
}
