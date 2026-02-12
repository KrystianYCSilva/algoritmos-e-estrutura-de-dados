/**
 * @file common.h
 * @brief Definições comuns e tipos genéricos para estruturas de dados
 *
 * Este arquivo define tipos e funções auxiliares para suportar genericidade
 * em estruturas de dados em C, permitindo armazenar tipos primitivos,
 * strings, ponteiros, vetores e matrizes.
 *
 * Referências:
 * - Kernighan & Ritchie, "The C Programming Language", 2nd ed.
 * - ISO/IEC 9899:2011 (C11 Standard)
 */

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Tipo de função para comparação entre elementos
 *
 * @param a Ponteiro para o primeiro elemento
 * @param b Ponteiro para o segundo elemento
 * @return int < 0 se a < b, 0 se a == b, > 0 se a > b
 *
 * Exemplos:
 * - Para inteiros: return *(int*)a - *(int*)b;
 * - Para strings: return strcmp((char*)a, (char*)b);
 */
typedef int (*CompareFn)(const void *a, const void *b);

/**
 * @brief Tipo de função para copiar um elemento
 *
 * @param src Ponteiro para o elemento a ser copiado
 * @return void* Ponteiro para a cópia alocada (caller deve liberar)
 *
 * Nota: Se NULL, será usada cópia memcpy simples
 */
typedef void* (*CopyFn)(const void *src);

/**
 * @brief Tipo de função para destruir/liberar um elemento
 *
 * @param data Ponteiro para o elemento a ser destruído
 *
 * Nota: Se NULL, nenhuma ação de limpeza será executada
 * Use para liberar memória alocada dinamicamente dentro do elemento
 */
typedef void (*DestroyFn)(void *data);

/**
 * @brief Tipo de função para imprimir um elemento
 *
 * @param data Ponteiro para o elemento a ser impresso
 *
 * Útil para debugging e visualização de estruturas
 */
typedef void (*PrintFn)(const void *data);

/**
 * @brief Tipo de função hash para elementos
 *
 * @param data Ponteiro para o elemento
 * @return size_t Hash value do elemento
 */
typedef size_t (*HashFn)(const void *data);

/**
 * @brief Códigos de erro comuns
 */
typedef enum {
    DS_SUCCESS = 0,           /**< Operação bem-sucedida */
    DS_ERROR_NULL_POINTER,    /**< Ponteiro nulo fornecido */
    DS_ERROR_OUT_OF_MEMORY,   /**< Falha na alocação de memória */
    DS_ERROR_EMPTY,           /**< Estrutura vazia */
    DS_ERROR_FULL,            /**< Estrutura cheia */
    DS_ERROR_NOT_FOUND,       /**< Elemento não encontrado */
    DS_ERROR_INVALID_INDEX,   /**< Índice inválido */
    DS_ERROR_INVALID_PARAM    /**< Parâmetro inválido */
} DataStructureError;

/**
 * @brief Estrutura para armazenar dados genéricos com metadados
 */
typedef struct {
    void *data;              /**< Ponteiro para os dados */
    size_t element_size;     /**< Tamanho de cada elemento em bytes */
    CopyFn copy;            /**< Função de cópia customizada (opcional) */
    DestroyFn destroy;      /**< Função de destruição customizada (opcional) */
} GenericContainer;

// ============================================================================
// FUNÇÕES AUXILIARES PARA TIPOS COMUNS
// ============================================================================

/**
 * @brief Função de comparação padrão para inteiros
 */
int compare_int(const void *a, const void *b);

/**
 * @brief Função de comparação padrão para floats
 */
int compare_float(const void *a, const void *b);

/**
 * @brief Função de comparação padrão para doubles
 */
int compare_double(const void *a, const void *b);

/**
 * @brief Função de comparação padrão para strings (char*)
 */
int compare_string(const void *a, const void *b);

/**
 * @brief Função de cópia padrão para strings (char*)
 * @return Nova string alocada (caller deve liberar com free)
 */
void* copy_string(const void *src);

/**
 * @brief Função de destruição padrão para strings (char*)
 */
void destroy_string(void *data);

/**
 * @brief Função de impressão padrão para inteiros
 */
void print_int(const void *data);

/**
 * @brief Função de impressão padrão para floats
 */
void print_float(const void *data);

/**
 * @brief Função de impressão padrão para doubles
 */
void print_double(const void *data);

/**
 * @brief Função de impressão padrão para strings
 */
void print_string(const void *data);

/**
 * @brief Função hash padrão para inteiros
 */
size_t hash_int(const void *data);

/**
 * @brief Função hash padrão para strings
 */
size_t hash_string(const void *data);

#endif // COMMON_H
