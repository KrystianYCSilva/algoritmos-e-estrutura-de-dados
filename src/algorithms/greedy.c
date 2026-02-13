/**
 * @file greedy.c
 * @brief Implementacao de 3 algoritmos gulosos classicos
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 16.1-16.3
 * - Huffman (1952), "A method for the construction of minimum-redundancy codes"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/greedy.h"

#include <stdlib.h>
#include <string.h>

// ============================================================================
// ACTIVITY SELECTION - Cormen S16.1
// ============================================================================

typedef struct {
    int start;
    int finish;
    size_t original_index;
} Activity;

static int compare_activities(const void *a, const void *b) {
    const Activity *aa = (const Activity *)a;
    const Activity *bb = (const Activity *)b;
    if (aa->finish != bb->finish) return aa->finish - bb->finish;
    return aa->start - bb->start;
}

ActivityResult greedy_activity_selection(const int *start, const int *finish, size_t n) {
    ActivityResult result = { NULL, 0 };
    if (start == NULL || finish == NULL || n == 0) return result;

    Activity *activities = malloc(n * sizeof(Activity));
    if (activities == NULL) return result;

    for (size_t i = 0; i < n; i++) {
        activities[i].start = start[i];
        activities[i].finish = finish[i];
        activities[i].original_index = i;
    }

    qsort(activities, n, sizeof(Activity), compare_activities);

    result.selected = malloc(n * sizeof(size_t));
    if (result.selected == NULL) {
        free(activities);
        return result;
    }

    result.selected[0] = activities[0].original_index;
    result.count = 1;
    size_t last = 0;

    for (size_t i = 1; i < n; i++) {
        if (activities[i].start >= activities[last].finish) {
            result.selected[result.count++] = activities[i].original_index;
            last = i;
        }
    }

    free(activities);
    return result;
}

void activity_result_destroy(ActivityResult *result) {
    if (result == NULL) return;
    free(result->selected);
    result->selected = NULL;
    result->count = 0;
}

// ============================================================================
// HUFFMAN CODING - Cormen S16.3
// ============================================================================

static HuffmanNode *huffman_node_create(char ch, int freq) {
    HuffmanNode *node = malloc(sizeof(HuffmanNode));
    if (node == NULL) return NULL;
    node->character = ch;
    node->frequency = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void huffman_tree_destroy(HuffmanNode *node) {
    if (node == NULL) return;
    huffman_tree_destroy(node->left);
    huffman_tree_destroy(node->right);
    free(node);
}

typedef struct {
    HuffmanNode **nodes;
    size_t size;
    size_t capacity;
} MinHeap;

static MinHeap *minheap_create(size_t cap) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) return NULL;
    h->nodes = malloc(cap * sizeof(HuffmanNode *));
    if (h->nodes == NULL) { free(h); return NULL; }
    h->size = 0;
    h->capacity = cap;
    return h;
}

static void minheap_destroy(MinHeap *h) {
    if (h == NULL) return;
    free(h->nodes);
    free(h);
}

static void minheap_swap(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void minheap_sift_up(MinHeap *h, size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (h->nodes[parent]->frequency > h->nodes[i]->frequency) {
            minheap_swap(&h->nodes[parent], &h->nodes[i]);
            i = parent;
        } else {
            break;
        }
    }
}

static void minheap_sift_down(MinHeap *h, size_t i) {
    while (true) {
        size_t smallest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left < h->size && h->nodes[left]->frequency < h->nodes[smallest]->frequency)
            smallest = left;
        if (right < h->size && h->nodes[right]->frequency < h->nodes[smallest]->frequency)
            smallest = right;
        if (smallest == i) break;
        minheap_swap(&h->nodes[i], &h->nodes[smallest]);
        i = smallest;
    }
}

static void minheap_insert(MinHeap *h, HuffmanNode *node) {
    h->nodes[h->size] = node;
    minheap_sift_up(h, h->size);
    h->size++;
}

static HuffmanNode *minheap_extract(MinHeap *h) {
    if (h->size == 0) return NULL;
    HuffmanNode *min = h->nodes[0];
    h->size--;
    h->nodes[0] = h->nodes[h->size];
    minheap_sift_down(h, 0);
    return min;
}

static void huffman_build_codes(HuffmanNode *node, char *code, int depth, char *codes[256]) {
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        code[depth] = '\0';
        codes[(unsigned char)node->character] = malloc((size_t)(depth + 1));
        if (codes[(unsigned char)node->character] != NULL) {
            strcpy(codes[(unsigned char)node->character], code);
        }
        return;
    }

    code[depth] = '0';
    huffman_build_codes(node->left, code, depth + 1, codes);
    code[depth] = '1';
    huffman_build_codes(node->right, code, depth + 1, codes);
}

HuffmanResult greedy_huffman(const char *chars, const int *freqs, size_t n) {
    HuffmanResult result;
    memset(&result, 0, sizeof(result));
    if (chars == NULL || freqs == NULL || n == 0) return result;

    if (n == 1) {
        result.root = huffman_node_create(chars[0], freqs[0]);
        result.codes[(unsigned char)chars[0]] = malloc(2);
        if (result.codes[(unsigned char)chars[0]] != NULL) {
            result.codes[(unsigned char)chars[0]][0] = '0';
            result.codes[(unsigned char)chars[0]][1] = '\0';
        }
        return result;
    }

    MinHeap *heap = minheap_create(n);
    if (heap == NULL) return result;

    for (size_t i = 0; i < n; i++) {
        HuffmanNode *node = huffman_node_create(chars[i], freqs[i]);
        if (node == NULL) {
            for (size_t j = 0; j < heap->size; j++) free(heap->nodes[j]);
            minheap_destroy(heap);
            return result;
        }
        minheap_insert(heap, node);
    }

    while (heap->size > 1) {
        HuffmanNode *left = minheap_extract(heap);
        HuffmanNode *right = minheap_extract(heap);
        HuffmanNode *parent = huffman_node_create('\0', left->frequency + right->frequency);
        if (parent == NULL) {
            huffman_tree_destroy(left);
            huffman_tree_destroy(right);
            while (heap->size > 0) huffman_tree_destroy(minheap_extract(heap));
            minheap_destroy(heap);
            return result;
        }
        parent->left = left;
        parent->right = right;
        minheap_insert(heap, parent);
    }

    result.root = minheap_extract(heap);
    minheap_destroy(heap);

    char code_buf[256];
    huffman_build_codes(result.root, code_buf, 0, result.codes);

    return result;
}

void huffman_result_destroy(HuffmanResult *result) {
    if (result == NULL) return;
    huffman_tree_destroy(result->root);
    result->root = NULL;
    for (int i = 0; i < 256; i++) {
        free(result->codes[i]);
        result->codes[i] = NULL;
    }
}

// ============================================================================
// FRACTIONAL KNAPSACK - Cormen S16.2
// ============================================================================

typedef struct {
    double weight;
    double value;
    double ratio;
    size_t original_index;
} FracItem;

static int compare_frac_items(const void *a, const void *b) {
    const FracItem *ia = (const FracItem *)a;
    const FracItem *ib = (const FracItem *)b;
    if (ib->ratio > ia->ratio) return 1;
    if (ib->ratio < ia->ratio) return -1;
    return 0;
}

double greedy_fractional_knapsack_value(const double *weights, const double *values,
                                        size_t n, double capacity) {
    if (weights == NULL || values == NULL || n == 0 || capacity <= 0.0) return 0.0;

    FracItem *items = malloc(n * sizeof(FracItem));
    if (items == NULL) return 0.0;

    for (size_t i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (weights[i] > 0.0) ? values[i] / weights[i] : 0.0;
        items[i].original_index = i;
    }

    qsort(items, n, sizeof(FracItem), compare_frac_items);

    double total_value = 0.0;
    double remaining = capacity;

    for (size_t i = 0; i < n && remaining > 0.0; i++) {
        if (items[i].weight <= remaining) {
            total_value += items[i].value;
            remaining -= items[i].weight;
        } else {
            total_value += items[i].ratio * remaining;
            remaining = 0.0;
        }
    }

    free(items);
    return total_value;
}

FractionalKnapsackResult greedy_fractional_knapsack(const double *weights, const double *values,
                                                     size_t n, double capacity) {
    FractionalKnapsackResult result = { 0.0, NULL, n };
    if (weights == NULL || values == NULL || n == 0 || capacity <= 0.0) return result;

    FracItem *items = malloc(n * sizeof(FracItem));
    if (items == NULL) return result;

    for (size_t i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (weights[i] > 0.0) ? values[i] / weights[i] : 0.0;
        items[i].original_index = i;
    }

    qsort(items, n, sizeof(FracItem), compare_frac_items);

    result.fractions = calloc(n, sizeof(double));
    if (result.fractions == NULL) {
        free(items);
        return result;
    }

    double remaining = capacity;

    for (size_t i = 0; i < n && remaining > 0.0; i++) {
        size_t orig = items[i].original_index;
        if (items[i].weight <= remaining) {
            result.fractions[orig] = 1.0;
            result.max_value += items[i].value;
            remaining -= items[i].weight;
        } else {
            result.fractions[orig] = remaining / items[i].weight;
            result.max_value += items[i].ratio * remaining;
            remaining = 0.0;
        }
    }

    free(items);
    return result;
}

void fractional_knapsack_result_destroy(FractionalKnapsackResult *result) {
    if (result == NULL) return;
    free(result->fractions);
    result->fractions = NULL;
    result->max_value = 0.0;
}
