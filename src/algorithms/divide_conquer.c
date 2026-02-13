/**
 * @file divide_conquer.c
 * @brief Implementacao de 5 algoritmos de divisao e conquista
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 4, 9, 33
 * - Strassen (1969), Karatsuba & Ofman (1962)
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/divide_conquer.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

// ============================================================================
// STRASSEN MATRIX MULTIPLICATION - Cormen S4.2
// ============================================================================

#define STRASSEN_THRESHOLD 64

void matrix_multiply_classic(const double *A, const double *B, double *C, size_t n) {
    if (A == NULL || B == NULL || C == NULL || n == 0) return;

    memset(C, 0, n * n * sizeof(double));
    for (size_t i = 0; i < n; i++) {
        for (size_t k = 0; k < n; k++) {
            double a_ik = A[i * n + k];
            for (size_t j = 0; j < n; j++) {
                C[i * n + j] += a_ik * B[k * n + j];
            }
        }
    }
}

static void matrix_add(const double *A, const double *B, double *C, size_t n) {
    for (size_t i = 0; i < n * n; i++) C[i] = A[i] + B[i];
}

static void matrix_sub(const double *A, const double *B, double *C, size_t n) {
    for (size_t i = 0; i < n * n; i++) C[i] = A[i] - B[i];
}

static void extract_submatrix(const double *M, double *sub, size_t n, size_t row, size_t col) {
    size_t half = n / 2;
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < half; j++) {
            sub[i * half + j] = M[(row + i) * n + (col + j)];
        }
    }
}

static void place_submatrix(double *M, const double *sub, size_t n, size_t row, size_t col) {
    size_t half = n / 2;
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < half; j++) {
            M[(row + i) * n + (col + j)] = sub[i * half + j];
        }
    }
}

static void strassen_recursive(const double *A, const double *B, double *C, size_t n) {
    if (n <= STRASSEN_THRESHOLD) {
        matrix_multiply_classic(A, B, C, n);
        return;
    }

    size_t half = n / 2;
    size_t sub_size = half * half;

    double *A11 = malloc(sub_size * sizeof(double));
    double *A12 = malloc(sub_size * sizeof(double));
    double *A21 = malloc(sub_size * sizeof(double));
    double *A22 = malloc(sub_size * sizeof(double));
    double *B11 = malloc(sub_size * sizeof(double));
    double *B12 = malloc(sub_size * sizeof(double));
    double *B21 = malloc(sub_size * sizeof(double));
    double *B22 = malloc(sub_size * sizeof(double));

    double *M1 = malloc(sub_size * sizeof(double));
    double *M2 = malloc(sub_size * sizeof(double));
    double *M3 = malloc(sub_size * sizeof(double));
    double *M4 = malloc(sub_size * sizeof(double));
    double *M5 = malloc(sub_size * sizeof(double));
    double *M6 = malloc(sub_size * sizeof(double));
    double *M7 = malloc(sub_size * sizeof(double));

    double *T1 = malloc(sub_size * sizeof(double));
    double *T2 = malloc(sub_size * sizeof(double));

    if (!A11 || !A12 || !A21 || !A22 || !B11 || !B12 || !B21 || !B22 ||
        !M1 || !M2 || !M3 || !M4 || !M5 || !M6 || !M7 || !T1 || !T2) {
        free(A11); free(A12); free(A21); free(A22);
        free(B11); free(B12); free(B21); free(B22);
        free(M1); free(M2); free(M3); free(M4); free(M5); free(M6); free(M7);
        free(T1); free(T2);
        matrix_multiply_classic(A, B, C, n);
        return;
    }

    extract_submatrix(A, A11, n, 0, 0);
    extract_submatrix(A, A12, n, 0, half);
    extract_submatrix(A, A21, n, half, 0);
    extract_submatrix(A, A22, n, half, half);
    extract_submatrix(B, B11, n, 0, 0);
    extract_submatrix(B, B12, n, 0, half);
    extract_submatrix(B, B21, n, half, 0);
    extract_submatrix(B, B22, n, half, half);

    matrix_add(A11, A22, T1, half);
    matrix_add(B11, B22, T2, half);
    strassen_recursive(T1, T2, M1, half);

    matrix_add(A21, A22, T1, half);
    strassen_recursive(T1, B11, M2, half);

    matrix_sub(B12, B22, T1, half);
    strassen_recursive(A11, T1, M3, half);

    matrix_sub(B21, B11, T1, half);
    strassen_recursive(A22, T1, M4, half);

    matrix_add(A11, A12, T1, half);
    strassen_recursive(T1, B22, M5, half);

    matrix_sub(A21, A11, T1, half);
    matrix_add(B11, B12, T2, half);
    strassen_recursive(T1, T2, M6, half);

    matrix_sub(A12, A22, T1, half);
    matrix_add(B21, B22, T2, half);
    strassen_recursive(T1, T2, M7, half);

    matrix_add(M1, M4, T1, half);
    matrix_sub(T1, M5, T2, half);
    matrix_add(T2, M7, T1, half);
    place_submatrix(C, T1, n, 0, 0);

    matrix_add(M3, M5, T1, half);
    place_submatrix(C, T1, n, 0, half);

    matrix_add(M2, M4, T1, half);
    place_submatrix(C, T1, n, half, 0);

    matrix_sub(M1, M2, T1, half);
    matrix_add(T1, M3, T2, half);
    matrix_add(T2, M6, T1, half);
    place_submatrix(C, T1, n, half, half);

    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(M1); free(M2); free(M3); free(M4); free(M5); free(M6); free(M7);
    free(T1); free(T2);
}

void strassen_multiply(const double *A, const double *B, double *C, size_t n) {
    if (A == NULL || B == NULL || C == NULL || n == 0) return;

    size_t padded = 1;
    while (padded < n) padded <<= 1;

    if (padded == n) {
        strassen_recursive(A, B, C, n);
        return;
    }

    double *Ap = calloc(padded * padded, sizeof(double));
    double *Bp = calloc(padded * padded, sizeof(double));
    double *Cp = calloc(padded * padded, sizeof(double));
    if (!Ap || !Bp || !Cp) {
        free(Ap); free(Bp); free(Cp);
        matrix_multiply_classic(A, B, C, n);
        return;
    }

    for (size_t i = 0; i < n; i++) {
        memcpy(Ap + i * padded, A + i * n, n * sizeof(double));
        memcpy(Bp + i * padded, B + i * n, n * sizeof(double));
    }

    strassen_recursive(Ap, Bp, Cp, padded);

    for (size_t i = 0; i < n; i++) {
        memcpy(C + i * n, Cp + i * padded, n * sizeof(double));
    }

    free(Ap); free(Bp); free(Cp);
}

// ============================================================================
// CLOSEST PAIR OF POINTS - Cormen S33.4
// ============================================================================

static double point_dist(Point2D a, Point2D b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

static int cmp_x(const void *a, const void *b) {
    double diff = ((const Point2D *)a)->x - ((const Point2D *)b)->x;
    return (diff > 0) ? 1 : (diff < 0) ? -1 : 0;
}

static int cmp_y(const void *a, const void *b) {
    double diff = ((const Point2D *)a)->y - ((const Point2D *)b)->y;
    return (diff > 0) ? 1 : (diff < 0) ? -1 : 0;
}

static ClosestPairResult brute_force_closest(Point2D *points, size_t n) {
    ClosestPairResult best;
    best.distance = DBL_MAX;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            double d = point_dist(points[i], points[j]);
            if (d < best.distance) {
                best.distance = d;
                best.p1 = points[i];
                best.p2 = points[j];
            }
        }
    }
    return best;
}

static ClosestPairResult closest_pair_rec(Point2D *px, Point2D *py, Point2D *temp, size_t n) {
    if (n <= 3) {
        return brute_force_closest(px, n);
    }

    size_t mid = n / 2;
    Point2D mid_point = px[mid];

    size_t li = 0, ri = 0;
    Point2D *pyl = temp;
    Point2D *pyr = temp + mid;
    for (size_t i = 0; i < n; i++) {
        if ((py[i].x < mid_point.x || (py[i].x == mid_point.x && py[i].y < mid_point.y)) && li < mid) {
            pyl[li++] = py[i];
        } else {
            pyr[ri++] = py[i];
        }
    }

    Point2D *temp_next = temp + n;
    ClosestPairResult dl = closest_pair_rec(px, pyl, temp_next, mid);
    ClosestPairResult dr = closest_pair_rec(px + mid, pyr, temp_next, n - mid);

    ClosestPairResult best = (dl.distance <= dr.distance) ? dl : dr;
    double d = best.distance;

    Point2D *strip = temp;
    size_t strip_count = 0;
    for (size_t i = 0; i < n; i++) {
        if (fabs(py[i].x - mid_point.x) < d) {
            strip[strip_count++] = py[i];
        }
    }

    for (size_t i = 0; i < strip_count; i++) {
        for (size_t j = i + 1; j < strip_count && (strip[j].y - strip[i].y) < d; j++) {
            double dist = point_dist(strip[i], strip[j]);
            if (dist < best.distance) {
                best.distance = dist;
                best.p1 = strip[i];
                best.p2 = strip[j];
            }
        }
    }

    return best;
}

ClosestPairResult closest_pair(Point2D *points, size_t n) {
    ClosestPairResult result = { {0,0}, {0,0}, DBL_MAX };
    if (points == NULL || n < 2) return result;

    Point2D *px = malloc(n * sizeof(Point2D));
    Point2D *py = malloc(n * sizeof(Point2D));
    Point2D *temp = malloc(2 * n * sizeof(Point2D));
    if (!px || !py || !temp) {
        free(px); free(py); free(temp);
        return brute_force_closest(points, n);
    }

    memcpy(px, points, n * sizeof(Point2D));
    memcpy(py, points, n * sizeof(Point2D));
    qsort(px, n, sizeof(Point2D), cmp_x);
    qsort(py, n, sizeof(Point2D), cmp_y);

    result = closest_pair_rec(px, py, temp, n);

    free(px); free(py); free(temp);
    return result;
}

// ============================================================================
// KARATSUBA MULTIPLICATION
// ============================================================================

static long long num_digits(long long x) {
    if (x == 0) return 1;
    long long count = 0;
    while (x > 0) { count++; x /= 10; }
    return count;
}

static long long power10(long long n) {
    long long result = 1;
    for (long long i = 0; i < n; i++) result *= 10;
    return result;
}

long long karatsuba_multiply(long long x, long long y) {
    int neg = 0;
    if (x < 0) { x = -x; neg ^= 1; }
    if (y < 0) { y = -y; neg ^= 1; }

    if (x < 10 || y < 10) {
        long long result = x * y;
        return neg ? -result : result;
    }

    long long dx = num_digits(x);
    long long dy = num_digits(y);
    long long n = (dx > dy) ? dx : dy;
    long long half = n / 2;

    long long p = power10(half);

    long long a = x / p;
    long long b = x % p;
    long long c = y / p;
    long long d = y % p;

    long long ac = karatsuba_multiply(a, c);
    long long bd = karatsuba_multiply(b, d);
    long long abcd = karatsuba_multiply(a + b, c + d);

    long long result = ac * power10(2 * half) + (abcd - ac - bd) * p + bd;
    return neg ? -result : result;
}

// ============================================================================
// MAXIMUM SUBARRAY - Cormen S4.1
// ============================================================================

static MaxSubarrayResult max_crossing_subarray(const int *arr, size_t low, size_t mid, size_t high) {
    long long left_sum = LLONG_MIN;
    long long sum = 0;
    size_t max_left = mid;

    for (size_t i = mid + 1; i > low; i--) {
        sum += arr[i - 1];
        if (sum > left_sum) {
            left_sum = sum;
            max_left = i - 1;
        }
    }

    long long right_sum = LLONG_MIN;
    sum = 0;
    size_t max_right = mid + 1;

    for (size_t j = mid + 1; j <= high; j++) {
        sum += arr[j];
        if (sum > right_sum) {
            right_sum = sum;
            max_right = j;
        }
    }

    MaxSubarrayResult r;
    r.sum = left_sum + right_sum;
    r.left = max_left;
    r.right = max_right;
    return r;
}

static MaxSubarrayResult max_subarray_rec(const int *arr, size_t low, size_t high) {
    if (low == high) {
        MaxSubarrayResult r = { arr[low], low, high };
        return r;
    }

    size_t mid = low + (high - low) / 2;
    MaxSubarrayResult left = max_subarray_rec(arr, low, mid);
    MaxSubarrayResult right = max_subarray_rec(arr, mid + 1, high);
    MaxSubarrayResult cross = max_crossing_subarray(arr, low, mid, high);

    if (left.sum >= right.sum && left.sum >= cross.sum) return left;
    if (right.sum >= left.sum && right.sum >= cross.sum) return right;
    return cross;
}

MaxSubarrayResult max_subarray_dc(const int *arr, size_t n) {
    MaxSubarrayResult empty = { 0, 0, 0 };
    if (arr == NULL || n == 0) return empty;
    return max_subarray_rec(arr, 0, n - 1);
}

MaxSubarrayResult max_subarray_kadane(const int *arr, size_t n) {
    MaxSubarrayResult result = { 0, 0, 0 };
    if (arr == NULL || n == 0) return result;

    long long max_ending = arr[0];
    long long max_so_far = arr[0];
    size_t start = 0, end = 0, temp_start = 0;

    for (size_t i = 1; i < n; i++) {
        if (arr[i] > max_ending + arr[i]) {
            max_ending = arr[i];
            temp_start = i;
        } else {
            max_ending += arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
            start = temp_start;
            end = i;
        }
    }

    result.sum = max_so_far;
    result.left = start;
    result.right = end;
    return result;
}

// ============================================================================
// QUICK SELECT - Cormen S9.2
// ============================================================================

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static size_t partition_qs(int *arr, size_t low, size_t high) {
    size_t mid = low + (high - low) / 2;
    if (arr[mid] < arr[low]) swap_int(&arr[mid], &arr[low]);
    if (arr[high] < arr[low]) swap_int(&arr[high], &arr[low]);
    if (arr[mid] < arr[high]) swap_int(&arr[mid], &arr[high]);
    int pivot = arr[high];

    size_t i = low;
    for (size_t j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap_int(&arr[i], &arr[j]);
            i++;
        }
    }
    swap_int(&arr[i], &arr[high]);
    return i;
}

static int quick_select_rec(int *arr, size_t low, size_t high, size_t k) {
    if (low == high) return arr[low];

    size_t pivot_idx = partition_qs(arr, low, high);

    if (k == pivot_idx) return arr[pivot_idx];
    if (k < pivot_idx) return quick_select_rec(arr, low, pivot_idx - 1, k);
    return quick_select_rec(arr, pivot_idx + 1, high, k);
}

int quick_select(int *arr, size_t n, size_t k, bool *found) {
    if (arr == NULL || n == 0 || k >= n) {
        if (found) *found = false;
        return 0;
    }
    if (found) *found = true;
    return quick_select_rec(arr, 0, n - 1, k);
}

int median(int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;
    bool found;
    return quick_select(arr, n, (n - 1) / 2, &found);
}
