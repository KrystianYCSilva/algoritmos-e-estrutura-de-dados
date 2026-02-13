/**
 * @file divide_conquer.h
 * @brief Algoritmos de Divisao e Conquista (Divide and Conquer)
 *
 * Implementacao de 5 algoritmos classicos de divisao e conquista:
 * 1. Strassen Matrix Multiplication
 * 2. Closest Pair of Points
 * 3. Karatsuba Multiplication
 * 4. Maximum Subarray (Kadane + D&C)
 * 5. Quick Select (k-th smallest)
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 4, 9
 * - Strassen (1969), "Gaussian elimination is not optimal"
 * - Karatsuba & Ofman (1962), "Multiplication of many-digital numbers"
 * - Shamos & Hoey (1975), "Closest-point problems"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#ifndef DIVIDE_CONQUER_H
#define DIVIDE_CONQUER_H

#include <stddef.h>
#include <stdbool.h>

// ============================================================================
// STRASSEN MATRIX MULTIPLICATION
// ============================================================================

/**
 * @brief Multiplica duas matrizes n x n usando Strassen
 *
 * Para n pequeno (<=64), usa multiplicacao classica.
 * Matrizes armazenadas em row-major order.
 *
 * @param A Matriz A (n x n)
 * @param B Matriz B (n x n)
 * @param C Matriz resultado C = A*B (n x n, pre-alocada)
 * @param n Dimensao das matrizes
 *
 * Complexidade: O(n^2.807) vs O(n^3) classico
 * Espaco: O(n^2 log n)
 *
 * Referencia: Cormen S4.2 (pseudocodigo STRASSEN p. 79-80)
 */
void strassen_multiply(const double *A, const double *B, double *C, size_t n);

/**
 * @brief Multiplicacao classica de matrizes n x n
 *
 * Complexidade: O(n^3)
 */
void matrix_multiply_classic(const double *A, const double *B, double *C, size_t n);

// ============================================================================
// CLOSEST PAIR OF POINTS
// ============================================================================

/**
 * @brief Ponto 2D
 */
typedef struct {
    double x;
    double y;
} Point2D;

/**
 * @brief Resultado do closest pair
 */
typedef struct {
    Point2D p1;
    Point2D p2;
    double distance;
} ClosestPairResult;

/**
 * @brief Encontra o par de pontos mais proximo
 *
 * @param points Array de pontos
 * @param n Numero de pontos (deve ser >= 2)
 * @return ClosestPairResult com os dois pontos e a distancia
 *
 * Complexidade: O(n log n)
 * Espaco: O(n)
 *
 * Referencia: Cormen S33.4 (pseudocodigo CLOSEST-PAIR p. 1039)
 */
ClosestPairResult closest_pair(Point2D *points, size_t n);

// ============================================================================
// KARATSUBA MULTIPLICATION
// ============================================================================

/**
 * @brief Multiplicacao de inteiros grandes usando Karatsuba
 *
 * Para numeros que cabem em long long. Para numeros maiores,
 * seria necessario BigInteger.
 *
 * @param x Primeiro numero
 * @param y Segundo numero
 * @return long long x * y
 *
 * Complexidade: O(n^1.585) onde n = numero de digitos
 * Referencia: Karatsuba & Ofman (1962); Cormen Problem 30-1
 */
long long karatsuba_multiply(long long x, long long y);

// ============================================================================
// MAXIMUM SUBARRAY
// ============================================================================

/**
 * @brief Resultado do maximum subarray
 */
typedef struct {
    long long sum;
    size_t left;
    size_t right;
} MaxSubarrayResult;

/**
 * @brief Maximum subarray usando Divide and Conquer
 *
 * @param arr Array de inteiros
 * @param n Tamanho do array
 * @return MaxSubarrayResult com soma maxima e indices
 *
 * Complexidade: O(n log n)
 * Referencia: Cormen S4.1 (pseudocodigo FIND-MAXIMUM-SUBARRAY p. 71)
 */
MaxSubarrayResult max_subarray_dc(const int *arr, size_t n);

/**
 * @brief Maximum subarray usando Kadane (linear)
 *
 * Complexidade: O(n)
 * Referencia: Kadane (1984); Cormen Exercise 4.1-5
 */
MaxSubarrayResult max_subarray_kadane(const int *arr, size_t n);

// ============================================================================
// QUICK SELECT
// ============================================================================

/**
 * @brief Encontra o k-esimo menor elemento (0-indexed)
 *
 * Usa particionamento estilo quicksort com pivot aleatorio.
 *
 * @param arr Array de inteiros (sera modificado!)
 * @param n Tamanho do array
 * @param k Indice do elemento desejado (0 = menor, n-1 = maior)
 * @param found Ponteiro para bool indicando sucesso
 * @return int O k-esimo menor elemento
 *
 * Complexidade: O(n) esperado, O(n^2) pior caso
 * Referencia: Cormen S9.2 (pseudocodigo RANDOMIZED-SELECT p. 216)
 */
int quick_select(int *arr, size_t n, size_t k, bool *found);

/**
 * @brief Mediana de um array
 *
 * @param arr Array de inteiros (sera modificado!)
 * @param n Tamanho do array
 * @return int Mediana (elemento do meio para n impar, menor dos dois centrais para n par)
 */
int median(int *arr, size_t n);

#endif // DIVIDE_CONQUER_H
