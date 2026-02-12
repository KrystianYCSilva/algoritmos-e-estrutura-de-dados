/**
 * @file usage_examples.c
 * @brief Exemplos de uso das estruturas de dados genéricas
 *
 * Este arquivo demonstra como usar cada estrutura de dados implementada
 * no projeto, incluindo tipos primitivos, strings e estruturas customizadas.
 *
 * Para compilar (após implementação):
 * gcc -I../include usage_examples.c ../src/data_structures/*.c -o examples
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures/common.h"
#include "data_structures/queue.h"
#include "data_structures/stack.h"
#include "data_structures/linked_list.h"
#include "data_structures/array_list.h"

// ============================================================================
// ESTRUTURA CUSTOMIZADA PARA EXEMPLOS
// ============================================================================

typedef struct {
    int id;
    char name[50];
    double score;
} Person;

void destroy_person(void *data) {
    // Para este exemplo, Person não tem alocações internas
    // então não há nada para liberar
    (void)data; // Evitar warning de parâmetro não usado
}

void* copy_person(const void *src) {
    Person *new_person = malloc(sizeof(Person));
    if (new_person) {
        memcpy(new_person, src, sizeof(Person));
    }
    return new_person;
}

void print_person(const void *data) {
    const Person *p = (const Person*)data;
    printf("Person{id=%d, name='%s', score=%.2f}", p->id, p->name, p->score);
}

int compare_person_by_id(const void *a, const void *b) {
    const Person *pa = (const Person*)a;
    const Person *pb = (const Person*)b;
    return pa->id - pb->id;
}

// ============================================================================
// EXEMPLOS DE QUEUE
// ============================================================================

void example_queue_integers() {
    printf("\n=== QUEUE DE INTEIROS ===\n");

    // Criar fila de inteiros (array-based)
    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);

    // Enfileirar elementos
    for (int i = 1; i <= 5; i++) {
        queue_enqueue(q, &i);
        printf("Enqueued: %d\n", i);
    }

    printf("Queue size: %zu\n", queue_size(q));

    // Ver o primeiro elemento sem remover
    int front;
    if (queue_front(q, &front) == DS_SUCCESS) {
        printf("Front element: %d\n", front);
    }

    // Desenfileirar elementos
    printf("\nDequeuing:\n");
    while (!queue_is_empty(q)) {
        int value;
        if (queue_dequeue(q, &value) == DS_SUCCESS) {
            printf("Dequeued: %d\n", value);
        }
    }

    queue_destroy(q);
}

void example_queue_strings() {
    printf("\n=== QUEUE DE STRINGS ===\n");

    // Criar fila de strings (linked-based para tamanho dinâmico)
    Queue *q = queue_create(sizeof(char*), QUEUE_LINKED, 0, destroy_string);

    // Enfileirar strings
    char *names[] = {"Alice", "Bob", "Charlie", "David"};
    for (int i = 0; i < 4; i++) {
        char *name = strdup(names[i]);
        queue_enqueue(q, &name);
        printf("Enqueued: %s\n", name);
    }

    // Processar fila
    printf("\nProcessing queue:\n");
    while (!queue_is_empty(q)) {
        char *name;
        if (queue_dequeue(q, &name) == DS_SUCCESS) {
            printf("Processing: %s\n", name);
            free(name); // Liberar string após uso
        }
    }

    queue_destroy(q);
}

// ============================================================================
// EXEMPLOS DE STACK
// ============================================================================

void example_stack_balanced_parentheses() {
    printf("\n=== STACK - VALIDAÇÃO DE PARÊNTESES ===\n");

    const char *expressions[] = {
        "((()))",      // válido
        "(()())",      // válido
        "(()",         // inválido
        "())()",       // inválido
        ""             // válido (vazio)
    };

    for (int i = 0; i < 5; i++) {
        const char *expr = expressions[i];
        Stack *s = stack_create(sizeof(char), STACK_ARRAY, 50, NULL);

        bool valid = true;
        for (size_t j = 0; expr[j] != '\0'; j++) {
            if (expr[j] == '(') {
                stack_push(s, &expr[j]);
            } else if (expr[j] == ')') {
                if (stack_is_empty(s)) {
                    valid = false;
                    break;
                }
                char dummy;
                stack_pop(s, &dummy);
            }
        }

        if (valid && !stack_is_empty(s)) {
            valid = false;
        }

        printf("Expression: '%s' -> %s\n", expr, valid ? "VALID" : "INVALID");
        stack_destroy(s);
    }
}

void example_stack_undo_redo() {
    printf("\n=== STACK - UNDO/REDO ===\n");

    Stack *undo_stack = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);
    Stack *redo_stack = stack_create(sizeof(int), STACK_ARRAY, 10, NULL);

    int state = 0;

    // Executar ações
    printf("Initial state: %d\n", state);

    int actions[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        stack_push(undo_stack, &state); // Salvar estado anterior
        state += actions[i];
        printf("Action +%d: state = %d\n", actions[i], state);
    }

    // Undo
    printf("\nUndo:\n");
    for (int i = 0; i < 2; i++) {
        if (!stack_is_empty(undo_stack)) {
            stack_push(redo_stack, &state); // Salvar para redo
            stack_pop(undo_stack, &state);
            printf("Undo: state = %d\n", state);
        }
    }

    // Redo
    printf("\nRedo:\n");
    if (!stack_is_empty(redo_stack)) {
        stack_push(undo_stack, &state); // Salvar para undo novamente
        stack_pop(redo_stack, &state);
        printf("Redo: state = %d\n", state);
    }

    stack_destroy(undo_stack);
    stack_destroy(redo_stack);
}

// ============================================================================
// EXEMPLOS DE LINKED LIST
// ============================================================================

void example_linkedlist_persons() {
    printf("\n=== LINKED LIST DE PESSOAS ===\n");

    LinkedList *list = list_create(sizeof(Person), LIST_DOUBLY, destroy_person);

    // Adicionar pessoas
    Person people[] = {
        {1, "Alice", 95.5},
        {2, "Bob", 87.3},
        {3, "Charlie", 92.1}
    };

    for (int i = 0; i < 3; i++) {
        list_push_back(list, &people[i]);
        printf("Added: ");
        print_person(&people[i]);
        printf("\n");
    }

    printf("\nList size: %zu\n", list_size(list));

    // Iterar pela lista
    printf("\nIterating:\n");
    ListNode *node = list_begin(list);
    while (node != NULL) {
        Person *p = (Person*)list_node_data(node);
        print_person(p);
        printf("\n");
        node = list_next(node);
    }

    // Buscar pessoa por ID
    printf("\nSearching for person with id=2:\n");
    Person search = {2, "", 0.0};
    ListNode *found = list_find(list, &search, compare_person_by_id);
    if (found) {
        Person *p = (Person*)list_node_data(found);
        printf("Found: ");
        print_person(p);
        printf("\n");
    }

    // Remover primeira pessoa
    Person removed;
    if (list_pop_front(list, &removed) == DS_SUCCESS) {
        printf("\nRemoved first person: ");
        print_person(&removed);
        printf("\n");
    }

    // Inverter lista
    list_reverse(list);
    printf("\nAfter reverse:\n");
    list_print(list, print_person);

    list_destroy(list);
}

// ============================================================================
// EXEMPLOS DE ARRAY LIST
// ============================================================================

void example_arraylist_dynamic_growth() {
    printf("\n=== ARRAY LIST - CRESCIMENTO DINÂMICO ===\n");

    ArrayList *arr = arraylist_create(sizeof(int), 2, NULL); // Capacidade inicial pequena

    printf("Initial capacity: %zu\n", arraylist_capacity(arr));

    // Adicionar elementos para forçar redimensionamento
    for (int i = 1; i <= 10; i++) {
        arraylist_push_back(arr, &i);
        printf("Added %d - Size: %zu, Capacity: %zu\n",
               i, arraylist_size(arr), arraylist_capacity(arr));
    }

    // Acessar elementos por índice
    printf("\nAccessing elements:\n");
    for (size_t i = 0; i < arraylist_size(arr); i++) {
        int value;
        if (arraylist_get(arr, i, &value) == DS_SUCCESS) {
            printf("arr[%zu] = %d\n", i, value);
        }
    }

    // Busca binária (requer array ordenado)
    printf("\nBinary search for 7:\n");
    int search_value = 7;
    ssize_t index = arraylist_binary_search(arr, &search_value, compare_int);
    if (index >= 0) {
        printf("Found at index: %zd\n", index);
    } else {
        printf("Not found\n");
    }

    arraylist_destroy(arr);
}

void example_arraylist_vs_linkedlist() {
    printf("\n=== COMPARAÇÃO: ARRAYLIST vs LINKEDLIST ===\n");

    const int N = 10000;

    // ArrayList: Inserção no final
    printf("ArrayList - %d push_back operations...\n", N);
    ArrayList *arr = arraylist_create(sizeof(int), 100, NULL);
    for (int i = 0; i < N; i++) {
        arraylist_push_back(arr, &i);
    }
    printf("ArrayList size: %zu, capacity: %zu\n",
           arraylist_size(arr), arraylist_capacity(arr));

    // LinkedList: Inserção no final
    printf("\nLinkedList - %d push_back operations...\n", N);
    LinkedList *list = list_create(sizeof(int), LIST_DOUBLY, NULL);
    for (int i = 0; i < N; i++) {
        list_push_back(list, &i);
    }
    printf("LinkedList size: %zu\n", list_size(list));

    // Comparar acesso aleatório
    printf("\nRandom access comparison:\n");
    printf("ArrayList[5000]: ");
    int val;
    if (arraylist_get(arr, 5000, &val) == DS_SUCCESS) {
        printf("%d (O(1))\n", val);
    }

    printf("LinkedList[5000]: ");
    if (list_get(list, 5000, &val) == DS_SUCCESS) {
        printf("%d (O(n))\n", val);
    }

    arraylist_destroy(arr);
    list_destroy(list);
}

// ============================================================================
// APLICAÇÃO REAL: BFS EM GRAFO
// ============================================================================

void example_bfs_graph() {
    printf("\n=== APLICAÇÃO REAL: BFS EM GRAFO ===\n");

    // Grafo simples representado como lista de adjacências
    // 0 -> 1, 2
    // 1 -> 3, 4
    // 2 -> 4
    // 3 -> 5
    // 4 -> 5

    int num_vertices = 6;
    bool visited[6] = {false};

    Queue *q = queue_create(sizeof(int), QUEUE_ARRAY, 10, NULL);

    // Começar BFS do vértice 0
    int start = 0;
    queue_enqueue(q, &start);
    visited[start] = true;

    printf("BFS traversal starting from vertex %d:\n", start);

    // Adjacências simplificadas (em um grafo real, seria mais estruturado)
    int adj[6][5] = {
        {1, 2, -1},      // 0 -> 1, 2
        {3, 4, -1},      // 1 -> 3, 4
        {4, -1},         // 2 -> 4
        {5, -1},         // 3 -> 5
        {5, -1},         // 4 -> 5
        {-1}             // 5 -> (nenhum)
    };

    while (!queue_is_empty(q)) {
        int current;
        queue_dequeue(q, &current);
        printf("Visiting vertex: %d\n", current);

        // Processar vizinhos
        for (int i = 0; adj[current][i] != -1; i++) {
            int neighbor = adj[current][i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue_enqueue(q, &neighbor);
            }
        }
    }

    queue_destroy(q);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    printf("========================================\n");
    printf("  EXEMPLOS DE ESTRUTURAS DE DADOS\n");
    printf("========================================\n");

    // Queue examples
    example_queue_integers();
    example_queue_strings();

    // Stack examples
    example_stack_balanced_parentheses();
    example_stack_undo_redo();

    // LinkedList examples
    example_linkedlist_persons();

    // ArrayList examples
    example_arraylist_dynamic_growth();
    example_arraylist_vs_linkedlist();

    // Real-world application
    example_bfs_graph();

    printf("\n========================================\n");
    printf("  TODOS OS EXEMPLOS CONCLUÍDOS\n");
    printf("========================================\n");

    return 0;
}
