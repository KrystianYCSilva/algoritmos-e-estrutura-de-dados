/**
 * @file common.c
 * @brief Implementação de funções auxiliares para tipos genéricos
 *
 * Este arquivo implementa funções de comparação, cópia, destruição,
 * impressão e hash para tipos comuns, facilitando o uso de estruturas
 * de dados genéricas.
 *
 * Referências:
 * - Kernighan & Ritchie, "The C Programming Language", 2nd ed.
 * - Knuth TAOCP Vol 3, Section 6.4 (Hash Functions)
 * - http://www.cse.yorku.ca/~oz/hash.html (djb2)
 * - http://www.isthe.com/chongo/tech/comp/fnv/ (FNV-1a)
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// FUNÇÕES DE COMPARAÇÃO
// ============================================================================

/**
 * @brief Compara dois inteiros
 *
 * @return < 0 se a < b, 0 se a == b, > 0 se a > b
 *
 * Complexidade: O(1)
 */
int compare_int(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }

    int ia = *(const int*)a;
    int ib = *(const int*)b;

    // Evitar overflow: não usar ia - ib diretamente
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

/**
 * @brief Compara dois floats
 *
 * Usa epsilon para lidar com imprecisão de ponto flutuante.
 *
 * Complexidade: O(1)
 */
int compare_float(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }

    float fa = *(const float*)a;
    float fb = *(const float*)b;

    // Epsilon para comparação de ponto flutuante
    const float epsilon = 1e-6f;
    float diff = fa - fb;

    if (fabs(diff) < epsilon) return 0;
    return (diff > 0) ? 1 : -1;
}

/**
 * @brief Compara dois doubles
 *
 * Usa epsilon para lidar com imprecisão de ponto flutuante.
 *
 * Complexidade: O(1)
 */
int compare_double(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }

    double da = *(const double*)a;
    double db = *(const double*)b;

    // Epsilon para comparação de ponto flutuante
    const double epsilon = 1e-9;
    double diff = da - db;

    if (fabs(diff) < epsilon) return 0;
    return (diff > 0) ? 1 : -1;
}

/**
 * @brief Compara duas strings (char*)
 *
 * Usa strcmp da biblioteca padrão.
 *
 * Complexidade: O(min(len(a), len(b)))
 */
int compare_string(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }

    const char *sa = *(const char**)a;
    const char *sb = *(const char**)b;

    if (sa == NULL || sb == NULL) {
        return (sa == NULL) - (sb == NULL);
    }

    return strcmp(sa, sb);
}

// ============================================================================
// FUNÇÕES DE CÓPIA
// ============================================================================

/**
 * @brief Cria cópia de uma string (char*)
 *
 * @param src Ponteiro para char* (string)
 * @return char* Nova string alocada (caller deve liberar com free)
 *
 * Complexidade: O(n) onde n = strlen(src)
 */
void* copy_string(const void *src) {
    if (src == NULL) {
        return NULL;
    }

    const char *str = *(const char**)src;
    if (str == NULL) {
        return NULL;
    }

    // Alocar e copiar
    size_t len = strlen(str);
    char *copy = (char*)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, str, len + 1);
    return &copy;  // Retornar ponteiro para o ponteiro
}

// ============================================================================
// FUNÇÕES DE DESTRUIÇÃO
// ============================================================================

/**
 * @brief Libera memória de uma string (char*)
 *
 * @param data Ponteiro para char* (string)
 *
 * Complexidade: O(1)
 */
void destroy_string(void *data) {
    if (data == NULL) {
        return;
    }

    char *str = *(char**)data;
    if (str != NULL) {
        free(str);
    }
}

// ============================================================================
// FUNÇÕES DE IMPRESSÃO
// ============================================================================

/**
 * @brief Imprime um inteiro
 *
 * Complexidade: O(1)
 */
void print_int(const void *data) {
    if (data == NULL) {
        printf("NULL");
        return;
    }
    printf("%d", *(const int*)data);
}

/**
 * @brief Imprime um float
 *
 * Complexidade: O(1)
 */
void print_float(const void *data) {
    if (data == NULL) {
        printf("NULL");
        return;
    }
    printf("%.2f", *(const float*)data);
}

/**
 * @brief Imprime um double
 *
 * Complexidade: O(1)
 */
void print_double(const void *data) {
    if (data == NULL) {
        printf("NULL");
        return;
    }
    printf("%.4f", *(const double*)data);
}

/**
 * @brief Imprime uma string
 *
 * Complexidade: O(n) onde n = strlen(string)
 */
void print_string(const void *data) {
    if (data == NULL) {
        printf("NULL");
        return;
    }

    const char *str = *(const char**)data;
    if (str == NULL) {
        printf("NULL");
    } else {
        printf("\"%s\"", str);
    }
}

// ============================================================================
// FUNÇÕES HASH
// ============================================================================

/**
 * @brief Hash simples para inteiros
 *
 * Usa multiplicação por número primo e XOR shift.
 *
 * Complexidade: O(1)
 */
size_t hash_int(const void *data) {
    if (data == NULL) {
        return 0;
    }

    int value = *(const int*)data;

    // Multiplicação por primo e XOR shift
    size_t hash = (size_t)value;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;

    return hash;
}

/**
 * @brief Hash djb2 para strings (Dan Bernstein)
 *
 * Algoritmo: hash = hash * 33 + c
 * Uma das funções hash mais populares e eficientes.
 *
 * Referência: http://www.cse.yorku.ca/~oz/hash.html
 *
 * Complexidade: O(n) onde n = strlen(string)
 */
size_t hash_string(const void *data) {
    if (data == NULL) {
        return 0;
    }

    const char *str = *(const char**)data;
    if (str == NULL) {
        return 0;
    }

    // djb2 algorithm
    size_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash;
}

/**
 * @brief Hash djb2 para dados arbitrários (implementação alternativa)
 *
 * Esta é a versão exposta no header como hash_djb2.
 *
 * Complexidade: O(n)
 */
size_t hash_djb2(const void *data) {
    return hash_string(data);
}

/**
 * @brief Hash FNV-1a para dados binários
 *
 * FNV (Fowler-Noll-Vo) é uma família de funções hash não-criptográficas
 * conhecida por sua simplicidade e boa distribuição.
 *
 * Algoritmo FNV-1a (64-bit):
 * hash = FNV_offset_basis
 * for each byte:
 *     hash = hash XOR byte
 *     hash = hash * FNV_prime
 *
 * Referência: http://www.isthe.com/chongo/tech/comp/fnv/
 *
 * Complexidade: O(size)
 */
size_t hash_fnv1a(const void *data, size_t size) {
    if (data == NULL || size == 0) {
        return 0;
    }

    const unsigned char *bytes = (const unsigned char*)data;

    // FNV-1a parameters for 64-bit
    // offset_basis = 14695981039346656037
    // prime = 1099511628211

    // Para 32-bit ou 64-bit dependendo do sistema
#if SIZE_MAX == UINT64_MAX
    // 64-bit
    size_t hash = 14695981039346656037ULL;
    const size_t prime = 1099511628211ULL;
#else
    // 32-bit
    size_t hash = 2166136261U;
    const size_t prime = 16777619U;
#endif

    for (size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= prime;
    }

    return hash;
}

/**
 * @brief Hash multiplicativa (Knuth)
 *
 * Usa a constante de ouro φ = (√5 - 1) / 2 ≈ 0.618...
 * Multiplicação: (value * A) mod 1, onde A é relacionado a φ
 *
 * Referência: Knuth TAOCP Vol 3, Section 6.4
 *
 * Complexidade: O(1)
 */
size_t hash_multiplicative(const void *data) {
    if (data == NULL) {
        return 0;
    }

    int value = *(const int*)data;

    // Constante relacionada à razão áurea
    // A = 2^w * φ onde φ = (√5 - 1) / 2
    // Para 64-bit: 11400714819323198485
    // Para 32-bit: 2654435769

#if SIZE_MAX == UINT64_MAX
    const size_t multiplier = 11400714819323198485ULL;
#else
    const size_t multiplier = 2654435769U;
#endif

    size_t hash = (size_t)value * multiplier;

    // Pegar bits mais significativos
    return hash;
}
