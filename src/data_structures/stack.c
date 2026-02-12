/**
 * @file stack.c
 * @brief Implementação de Stack (Pilha LIFO) genérica
 *
 * Implementa duas variantes:
 * - STACK_ARRAY: Array dinâmico com crescimento automático
 * - STACK_LINKED: Lista encadeada (push/pop no head)
 *
 * Referências:
 * - Cormen et al. (2009), Chapter 10.1 - Stacks and Queues
 * - Sedgewick & Wayne (2011), Section 1.3
 *
 * @author Algoritmos e Heurísticas
 * @date 2025
 */

#include "data_structures/stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURA INTERNA DA STACK
// ============================================================================

/**
 * @brief Nó da pilha encadeada (para STACK_LINKED)
 */
typedef struct StackNode {
    void *data;
    struct StackNode *next;
} StackNode;

/**
 * @brief Estrutura interna da pilha
 */
struct Stack {
    StackType type;           // Tipo de implementação
    size_t element_size;      // Tamanho de cada elemento
    size_t size;              // Número de elementos (= top + 1 para array)
    DestroyFn destroy;        // Função de destruição

    // Para STACK_ARRAY
    void *array;              // Array de elementos
    size_t capacity;          // Capacidade do array
    size_t top;               // Índice do topo (size - 1)

    // Para STACK_LINKED
    StackNode *head;          // Topo da pilha (primeiro nó)
};

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

/**
 * @brief Redimensiona o array da pilha
 *
 * Dobra a capacidade quando necessário.
 *
 * Complexidade: O(n)
 */
static DataStructureError stack_resize_array(Stack *stack) {
    size_t new_capacity = stack->capacity * 2;
    if (new_capacity < 4) new_capacity = 4;

    void *new_array = realloc(stack->array, new_capacity * stack->element_size);
    if (new_array == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    stack->array = new_array;
    stack->capacity = new_capacity;

    return DS_SUCCESS;
}

// ============================================================================
// CRIAÇÃO E DESTRUIÇÃO
// ============================================================================

/**
 * @brief Cria uma nova pilha
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * Inicializa S.top = 0
 *
 * Complexidade: O(1) para LINKED, O(capacity) para ARRAY
 */
Stack* stack_create(size_t element_size, StackType type, size_t initial_capacity, DestroyFn destroy) {
    if (element_size == 0) {
        return NULL;
    }

    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->element_size = element_size;
    stack->type = type;
    stack->size = 0;
    stack->destroy = destroy;

    if (type == STACK_ARRAY) {
        if (initial_capacity == 0) initial_capacity = 16;

        stack->array = malloc(initial_capacity * element_size);
        if (stack->array == NULL) {
            free(stack);
            return NULL;
        }

        stack->capacity = initial_capacity;
        stack->top = 0;
        stack->head = NULL;
    } else {
        // STACK_LINKED
        stack->array = NULL;
        stack->capacity = 0;
        stack->top = 0;
        stack->head = NULL;
    }

    return stack;
}

/**
 * @brief Destrói a pilha
 *
 * Complexidade: O(n)
 */
void stack_destroy(Stack *stack) {
    if (stack == NULL) {
        return;
    }

    stack_clear(stack);

    if (stack->type == STACK_ARRAY && stack->array != NULL) {
        free(stack->array);
    }

    free(stack);
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS - ARRAY
// ============================================================================

/**
 * @brief Push para STACK_ARRAY
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * PUSH(S, x)
 *   S.top = S.top + 1
 *   S[S.top] = x
 *
 * Complexidade: O(1) amortizado
 */
static DataStructureError stack_push_array(Stack *stack, const void *data) {
    // Verificar se precisa redimensionar
    if (stack->size >= stack->capacity) {
        DataStructureError err = stack_resize_array(stack);
        if (err != DS_SUCCESS) {
            return err;
        }
    }

    // Inserir no topo
    void *dest = (char*)stack->array + (stack->size * stack->element_size);
    memcpy(dest, data, stack->element_size);

    stack->size++;
    stack->top = stack->size - 1;

    return DS_SUCCESS;
}

/**
 * @brief Pop para STACK_ARRAY
 *
 * Pseudocódigo (Cormen et al., 2009, p. 233):
 * POP(S)
 *   if STACK-EMPTY(S)
 *       error "underflow"
 *   else S.top = S.top - 1
 *       return S[S.top + 1]
 *
 * Complexidade: O(1)
 */
static DataStructureError stack_pop_array(Stack *stack, void *output) {
    if (stack->size == 0) {
        return DS_ERROR_EMPTY;
    }

    // Copiar elemento do topo
    void *src = (char*)stack->array + (stack->top * stack->element_size);
    if (output != NULL) {
        memcpy(output, src, stack->element_size);
    }

    // Chamar destrutor se necessário
    if (stack->destroy != NULL) {
        stack->destroy(src);
    }

    stack->size--;
    if (stack->size > 0) {
        stack->top = stack->size - 1;
    } else {
        stack->top = 0;
    }

    return DS_SUCCESS;
}

/**
 * @brief Top para STACK_ARRAY
 *
 * Complexidade: O(1)
 */
static DataStructureError stack_top_array(const Stack *stack, void *output) {
    if (stack->size == 0) {
        return DS_ERROR_EMPTY;
    }

    void *src = (char*)stack->array + (stack->top * stack->element_size);
    memcpy(output, src, stack->element_size);

    return DS_SUCCESS;
}

// ============================================================================
// OPERAÇÕES PRINCIPAIS - LINKED
// ============================================================================

/**
 * @brief Push para STACK_LINKED
 *
 * Inserção no head (topo) da lista.
 *
 * Complexidade: O(1)
 */
static DataStructureError stack_push_linked(Stack *stack, const void *data) {
    // Criar novo nó
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    if (node == NULL) {
        return DS_ERROR_OUT_OF_MEMORY;
    }

    node->data = malloc(stack->element_size);
    if (node->data == NULL) {
        free(node);
        return DS_ERROR_OUT_OF_MEMORY;
    }

    memcpy(node->data, data, stack->element_size);

    // Inserir no head (novo topo)
    node->next = stack->head;
    stack->head = node;

    stack->size++;
    return DS_SUCCESS;
}

/**
 * @brief Pop para STACK_LINKED
 *
 * Remoção do head (topo) da lista.
 *
 * Complexidade: O(1)
 */
static DataStructureError stack_pop_linked(Stack *stack, void *output) {
    if (stack->size == 0 || stack->head == NULL) {
        return DS_ERROR_EMPTY;
    }

    StackNode *node = stack->head;

    // Copiar dados
    if (output != NULL) {
        memcpy(output, node->data, stack->element_size);
    }

    // Chamar destrutor se necessário
    if (stack->destroy != NULL) {
        stack->destroy(node->data);
    }

    // Remover nó
    stack->head = node->next;

    free(node->data);
    free(node);
    stack->size--;

    return DS_SUCCESS;
}

/**
 * @brief Top para STACK_LINKED
 *
 * Complexidade: O(1)
 */
static DataStructureError stack_top_linked(const Stack *stack, void *output) {
    if (stack->size == 0 || stack->head == NULL) {
        return DS_ERROR_EMPTY;
    }

    memcpy(output, stack->head->data, stack->element_size);
    return DS_SUCCESS;
}

// ============================================================================
// INTERFACE PÚBLICA (dispatch para array ou linked)
// ============================================================================

DataStructureError stack_push(Stack *stack, const void *data) {
    if (stack == NULL || data == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (stack->type == STACK_ARRAY) {
        return stack_push_array(stack, data);
    } else {
        return stack_push_linked(stack, data);
    }
}

DataStructureError stack_pop(Stack *stack, void *output) {
    if (stack == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (stack->type == STACK_ARRAY) {
        return stack_pop_array(stack, output);
    } else {
        return stack_pop_linked(stack, output);
    }
}

DataStructureError stack_top(const Stack *stack, void *output) {
    if (stack == NULL || output == NULL) {
        return DS_ERROR_NULL_POINTER;
    }

    if (stack->type == STACK_ARRAY) {
        return stack_top_array(stack, output);
    } else {
        return stack_top_linked(stack, output);
    }
}

// ============================================================================
// CONSULTAS E UTILITÁRIOS
// ============================================================================

bool stack_is_empty(const Stack *stack) {
    return (stack == NULL || stack->size == 0);
}

size_t stack_size(const Stack *stack) {
    return (stack == NULL) ? 0 : stack->size;
}

size_t stack_capacity(const Stack *stack) {
    if (stack == NULL || stack->type != STACK_ARRAY) {
        return 0;
    }
    return stack->capacity;
}

void stack_clear(Stack *stack) {
    if (stack == NULL) {
        return;
    }

    if (stack->type == STACK_ARRAY) {
        // Chamar destrutor para cada elemento se necessário
        if (stack->destroy != NULL) {
            for (size_t i = 0; i < stack->size; i++) {
                void *elem = (char*)stack->array + (i * stack->element_size);
                stack->destroy(elem);
            }
        }
        stack->size = 0;
        stack->top = 0;
    } else {
        // STACK_LINKED
        while (stack->head != NULL) {
            stack_pop_linked(stack, NULL);
        }
    }
}

void stack_print(const Stack *stack, PrintFn print) {
    if (stack == NULL || print == NULL) {
        return;
    }

    printf("Stack(size=%zu, capacity=%zu) [TOP", stack->size, stack_capacity(stack));

    if (stack->type == STACK_ARRAY) {
        // Imprimir do topo (último) para a base (primeiro)
        for (size_t i = stack->size; i > 0; i--) {
            printf(" -> ");
            void *elem = (char*)stack->array + ((i - 1) * stack->element_size);
            print(elem);
        }
    } else {
        // STACK_LINKED
        StackNode *node = stack->head;
        while (node != NULL) {
            printf(" -> ");
            print(node->data);
            node = node->next;
        }
    }

    printf(" -> BASE]\n");
}

void stack_reverse(Stack *stack) {
    if (stack == NULL || stack->size <= 1) {
        return;
    }

    if (stack->type == STACK_ARRAY) {
        // Reverter array: trocar elementos das extremidades
        size_t left = 0;
        size_t right = stack->size - 1;

        void *temp = malloc(stack->element_size);
        if (temp == NULL) return;

        while (left < right) {
            void *left_elem = (char*)stack->array + (left * stack->element_size);
            void *right_elem = (char*)stack->array + (right * stack->element_size);

            // Swap
            memcpy(temp, left_elem, stack->element_size);
            memcpy(left_elem, right_elem, stack->element_size);
            memcpy(right_elem, temp, stack->element_size);

            left++;
            right--;
        }

        free(temp);
    } else {
        // STACK_LINKED: reverter lista encadeada
        StackNode *prev = NULL;
        StackNode *current = stack->head;
        StackNode *next = NULL;

        while (current != NULL) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }

        stack->head = prev;
    }
}

// TODO: Implementar stack_clone e stack_to_array quando necessário
Stack* stack_clone(const Stack *stack, CopyFn copy) {
    (void)stack;
    (void)copy;
    // TODO: Implementar
    return NULL;
}

DataStructureError stack_to_array(const Stack *stack, void **array, size_t *size) {
    (void)stack;
    (void)array;
    (void)size;
    // TODO: Implementar
    return DS_ERROR_NOT_FOUND;
}
