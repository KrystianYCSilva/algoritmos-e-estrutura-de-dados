/**
 * @file dynamic_programming.c
 * @brief Implementacao de 8 problemas classicos de programacao dinamica
 *
 * Referencias:
 * - Cormen et al. (2009), Chapters 15.1-15.4
 * - Bellman (1957), "Dynamic Programming"
 * - Levenshtein (1966), "Binary codes capable of correcting deletions"
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "algorithms/dynamic_programming.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ============================================================================
// FIBONACCI - Cormen S15.1
// ============================================================================

static long long fib_memo_helper(int n, long long *memo) {
    if (memo[n] != -1) return memo[n];
    memo[n] = fib_memo_helper(n - 1, memo) + fib_memo_helper(n - 2, memo);
    return memo[n];
}

long long dp_fibonacci_memo(int n) {
    if (n < 0) return -1;
    if (n <= 1) return n;

    long long *memo = malloc((size_t)(n + 1) * sizeof(long long));
    if (memo == NULL) return -1;

    for (int i = 0; i <= n; i++) memo[i] = -1;
    memo[0] = 0;
    memo[1] = 1;

    long long result = fib_memo_helper(n, memo);
    free(memo);
    return result;
}

long long dp_fibonacci_tab(int n) {
    if (n < 0) return -1;
    if (n <= 1) return n;

    long long prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// ============================================================================
// LCS - Cormen S15.4
// ============================================================================

size_t dp_lcs_length(const char *x, const char *y) {
    if (x == NULL || y == NULL) return 0;

    size_t m = strlen(x);
    size_t n = strlen(y);

    if (m == 0 || n == 0) return 0;

    size_t *prev = calloc(n + 1, sizeof(size_t));
    size_t *curr = calloc(n + 1, sizeof(size_t));
    if (prev == NULL || curr == NULL) {
        free(prev);
        free(curr);
        return 0;
    }

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (x[i - 1] == y[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = (prev[j] > curr[j - 1]) ? prev[j] : curr[j - 1];
            }
        }
        size_t *tmp = prev;
        prev = curr;
        curr = tmp;
        memset(curr, 0, (n + 1) * sizeof(size_t));
    }

    size_t result = prev[n];
    free(prev);
    free(curr);
    return result;
}

LCSResult dp_lcs(const char *x, const char *y) {
    LCSResult result = { 0, NULL };
    if (x == NULL || y == NULL) return result;

    size_t m = strlen(x);
    size_t n = strlen(y);

    if (m == 0 || n == 0) return result;

    size_t **dp = malloc((m + 1) * sizeof(size_t *));
    if (dp == NULL) return result;

    for (size_t i = 0; i <= m; i++) {
        dp[i] = calloc(n + 1, sizeof(size_t));
        if (dp[i] == NULL) {
            for (size_t k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return result;
        }
    }

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    result.length = dp[m][n];

    if (result.length > 0) {
        result.sequence = malloc(result.length + 1);
        if (result.sequence != NULL) {
            size_t idx = result.length;
            result.sequence[idx] = '\0';
            size_t i = m, j = n;
            while (i > 0 && j > 0) {
                if (x[i - 1] == y[j - 1]) {
                    idx--;
                    result.sequence[idx] = x[i - 1];
                    i--;
                    j--;
                } else if (dp[i - 1][j] > dp[i][j - 1]) {
                    i--;
                } else {
                    j--;
                }
            }
        }
    }

    for (size_t i = 0; i <= m; i++) free(dp[i]);
    free(dp);
    return result;
}

void dp_lcs_result_destroy(LCSResult *result) {
    if (result == NULL) return;
    free(result->sequence);
    result->sequence = NULL;
    result->length = 0;
}

// ============================================================================
// KNAPSACK 0/1 - Cormen S16.2 / Kleinberg S6.4
// ============================================================================

int dp_knapsack_value(const int *weights, const int *values, size_t n, int capacity) {
    if (weights == NULL || values == NULL || n == 0 || capacity <= 0) return 0;

    int *prev = calloc((size_t)(capacity + 1), sizeof(int));
    int *curr = calloc((size_t)(capacity + 1), sizeof(int));
    if (prev == NULL || curr == NULL) {
        free(prev);
        free(curr);
        return 0;
    }

    for (size_t i = 0; i < n; i++) {
        for (int w = 0; w <= capacity; w++) {
            curr[w] = prev[w];
            if (weights[i] <= w) {
                int with_item = values[i] + prev[w - weights[i]];
                if (with_item > curr[w]) curr[w] = with_item;
            }
        }
        int *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    int result = prev[capacity];
    free(prev);
    free(curr);
    return result;
}

KnapsackResult dp_knapsack(const int *weights, const int *values, size_t n, int capacity) {
    KnapsackResult result = { 0, NULL, n };
    if (weights == NULL || values == NULL || n == 0 || capacity <= 0) return result;

    int **dp = malloc((n + 1) * sizeof(int *));
    if (dp == NULL) return result;

    for (size_t i = 0; i <= n; i++) {
        dp[i] = calloc((size_t)(capacity + 1), sizeof(int));
        if (dp[i] == NULL) {
            for (size_t k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return result;
        }
    }

    for (size_t i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            if (weights[i - 1] <= w) {
                int with_item = values[i - 1] + dp[i - 1][w - weights[i - 1]];
                if (with_item > dp[i][w]) dp[i][w] = with_item;
            }
        }
    }

    result.max_value = dp[n][capacity];

    result.selected = calloc(n, sizeof(bool));
    if (result.selected != NULL) {
        int w = capacity;
        for (size_t i = n; i > 0; i--) {
            if (dp[i][w] != dp[i - 1][w]) {
                result.selected[i - 1] = true;
                w -= weights[i - 1];
            }
        }
    }

    for (size_t i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return result;
}

void dp_knapsack_result_destroy(KnapsackResult *result) {
    if (result == NULL) return;
    free(result->selected);
    result->selected = NULL;
    result->max_value = 0;
}

// ============================================================================
// EDIT DISTANCE - Cormen Problem 15-5
// ============================================================================

size_t dp_edit_distance(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return 0;

    size_t m = strlen(s1);
    size_t n = strlen(s2);

    if (m == 0) return n;
    if (n == 0) return m;

    size_t *prev = malloc((n + 1) * sizeof(size_t));
    size_t *curr = malloc((n + 1) * sizeof(size_t));
    if (prev == NULL || curr == NULL) {
        free(prev);
        free(curr);
        return 0;
    }

    for (size_t j = 0; j <= n; j++) prev[j] = j;

    for (size_t i = 1; i <= m; i++) {
        curr[0] = i;
        for (size_t j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                size_t ins = curr[j - 1] + 1;
                size_t del = prev[j] + 1;
                size_t sub = prev[j - 1] + 1;
                curr[j] = ins;
                if (del < curr[j]) curr[j] = del;
                if (sub < curr[j]) curr[j] = sub;
            }
        }
        size_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    size_t result = prev[n];
    free(prev);
    free(curr);
    return result;
}

// ============================================================================
// LIS - Cormen Problem 15-4
// ============================================================================

static size_t lis_binary_search(const int *tails, size_t len, int target) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (tails[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

size_t dp_lis_length(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;

    int *tails = malloc(n * sizeof(int));
    if (tails == NULL) return 0;

    size_t len = 0;
    for (size_t i = 0; i < n; i++) {
        size_t pos = lis_binary_search(tails, len, arr[i]);
        tails[pos] = arr[i];
        if (pos == len) len++;
    }

    free(tails);
    return len;
}

LISResult dp_lis(const int *arr, size_t n) {
    LISResult result = { 0, NULL, 0 };
    if (arr == NULL || n == 0) return result;

    size_t *dp = malloc(n * sizeof(size_t));
    size_t *parent = malloc(n * sizeof(size_t));
    if (dp == NULL || parent == NULL) {
        free(dp);
        free(parent);
        return result;
    }

    for (size_t i = 0; i < n; i++) {
        dp[i] = 1;
        parent[i] = (size_t)-1;
    }

    size_t max_len = 1;
    size_t max_idx = 0;

    for (size_t i = 1; i < n; i++) {
        for (size_t j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        if (dp[i] > max_len) {
            max_len = dp[i];
            max_idx = i;
        }
    }

    result.length = max_len;
    result.seq_len = max_len;
    result.sequence = malloc(max_len * sizeof(int));
    if (result.sequence != NULL) {
        size_t idx = max_len;
        size_t k = max_idx;
        while (k != (size_t)-1 && idx > 0) {
            idx--;
            result.sequence[idx] = arr[k];
            k = parent[k];
        }
    }

    free(dp);
    free(parent);
    return result;
}

void dp_lis_result_destroy(LISResult *result) {
    if (result == NULL) return;
    free(result->sequence);
    result->sequence = NULL;
    result->length = 0;
    result->seq_len = 0;
}

// ============================================================================
// ROD CUTTING - Cormen S15.1
// ============================================================================

int dp_rod_cutting_value(const int *prices, size_t n) {
    if (prices == NULL || n == 0) return 0;

    int *r = calloc(n + 1, sizeof(int));
    if (r == NULL) return 0;

    for (size_t j = 1; j <= n; j++) {
        int q = INT_MIN;
        for (size_t i = 1; i <= j; i++) {
            int val = prices[i - 1] + r[j - i];
            if (val > q) q = val;
        }
        r[j] = q;
    }

    int result = r[n];
    free(r);
    return result;
}

RodCutResult dp_rod_cutting(const int *prices, size_t n) {
    RodCutResult result = { 0, NULL, 0 };
    if (prices == NULL || n == 0) return result;

    int *r = calloc(n + 1, sizeof(int));
    size_t *s = calloc(n + 1, sizeof(size_t));
    if (r == NULL || s == NULL) {
        free(r);
        free(s);
        return result;
    }

    for (size_t j = 1; j <= n; j++) {
        int q = INT_MIN;
        for (size_t i = 1; i <= j; i++) {
            int val = prices[i - 1] + r[j - i];
            if (val > q) {
                q = val;
                s[j] = i;
            }
        }
        r[j] = q;
    }

    result.max_revenue = r[n];

    size_t num_cuts = 0;
    size_t temp = n;
    while (temp > 0) {
        num_cuts++;
        temp -= s[temp];
    }

    result.cuts = malloc(num_cuts * sizeof(int));
    if (result.cuts != NULL) {
        result.num_cuts = num_cuts;
        size_t idx = 0;
        temp = n;
        while (temp > 0) {
            result.cuts[idx++] = (int)s[temp];
            temp -= s[temp];
        }
    }

    free(r);
    free(s);
    return result;
}

void dp_rod_cut_result_destroy(RodCutResult *result) {
    if (result == NULL) return;
    free(result->cuts);
    result->cuts = NULL;
    result->max_revenue = 0;
    result->num_cuts = 0;
}

// ============================================================================
// MATRIX CHAIN MULTIPLICATION - Cormen S15.2
// ============================================================================

long long dp_matrix_chain_value(const int *dims, size_t n) {
    if (dims == NULL || n <= 1) return 0;

    long long **m_table = malloc(n * sizeof(long long *));
    if (m_table == NULL) return 0;

    for (size_t i = 0; i < n; i++) {
        m_table[i] = calloc(n, sizeof(long long));
        if (m_table[i] == NULL) {
            for (size_t k = 0; k < i; k++) free(m_table[k]);
            free(m_table);
            return 0;
        }
    }

    for (size_t l = 2; l <= n; l++) {
        for (size_t i = 0; i <= n - l; i++) {
            size_t j = i + l - 1;
            m_table[i][j] = LLONG_MAX;
            for (size_t k = i; k < j; k++) {
                long long q = m_table[i][k] + m_table[k + 1][j] +
                              (long long)dims[i] * dims[k + 1] * dims[j + 1];
                if (q < m_table[i][j]) {
                    m_table[i][j] = q;
                }
            }
        }
    }

    long long result = m_table[0][n - 1];
    for (size_t i = 0; i < n; i++) free(m_table[i]);
    free(m_table);
    return result;
}

static void build_parens(size_t **s, size_t i, size_t j, char *buf, size_t *pos) {
    if (i == j) {
        buf[*pos] = 'A';
        (*pos)++;
        if (i < 10) {
            buf[*pos] = (char)('0' + i);
        } else {
            buf[*pos] = (char)('0' + i / 10);
            (*pos)++;
            buf[*pos] = (char)('0' + i % 10);
        }
        (*pos)++;
    } else {
        buf[*pos] = '(';
        (*pos)++;
        build_parens(s, i, s[i][j], buf, pos);
        build_parens(s, s[i][j] + 1, j, buf, pos);
        buf[*pos] = ')';
        (*pos)++;
    }
}

MatrixChainResult dp_matrix_chain(const int *dims, size_t n) {
    MatrixChainResult result = { 0, NULL };
    if (dims == NULL || n <= 1) return result;

    long long **m_table = malloc(n * sizeof(long long *));
    size_t **s_table = malloc(n * sizeof(size_t *));
    if (m_table == NULL || s_table == NULL) {
        free(m_table);
        free(s_table);
        return result;
    }

    for (size_t i = 0; i < n; i++) {
        m_table[i] = calloc(n, sizeof(long long));
        s_table[i] = calloc(n, sizeof(size_t));
        if (m_table[i] == NULL || s_table[i] == NULL) {
            for (size_t k = 0; k <= i; k++) {
                free(m_table[k]);
                free(s_table[k]);
            }
            free(m_table);
            free(s_table);
            return result;
        }
    }

    for (size_t l = 2; l <= n; l++) {
        for (size_t i = 0; i <= n - l; i++) {
            size_t j = i + l - 1;
            m_table[i][j] = LLONG_MAX;
            for (size_t k = i; k < j; k++) {
                long long q = m_table[i][k] + m_table[k + 1][j] +
                              (long long)dims[i] * dims[k + 1] * dims[j + 1];
                if (q < m_table[i][j]) {
                    m_table[i][j] = q;
                    s_table[i][j] = k;
                }
            }
        }
    }

    result.min_operations = m_table[0][n - 1];

    result.parenthesization = malloc(n * 10);
    if (result.parenthesization != NULL) {
        size_t pos = 0;
        build_parens(s_table, 0, n - 1, result.parenthesization, &pos);
        result.parenthesization[pos] = '\0';
    }

    for (size_t i = 0; i < n; i++) {
        free(m_table[i]);
        free(s_table[i]);
    }
    free(m_table);
    free(s_table);
    return result;
}

void dp_matrix_chain_result_destroy(MatrixChainResult *result) {
    if (result == NULL) return;
    free(result->parenthesization);
    result->parenthesization = NULL;
    result->min_operations = 0;
}

// ============================================================================
// COIN CHANGE - Cormen Problem 16-1
// ============================================================================

int dp_coin_change_value(const int *coins, size_t num_coins, int amount) {
    if (coins == NULL || num_coins == 0 || amount < 0) return -1;
    if (amount == 0) return 0;

    int *dp = malloc((size_t)(amount + 1) * sizeof(int));
    if (dp == NULL) return -1;

    for (int i = 0; i <= amount; i++) dp[i] = INT_MAX;
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (size_t j = 0; j < num_coins; j++) {
            if (coins[j] <= i && dp[i - coins[j]] != INT_MAX) {
                int val = dp[i - coins[j]] + 1;
                if (val < dp[i]) dp[i] = val;
            }
        }
    }

    int result = (dp[amount] == INT_MAX) ? -1 : dp[amount];
    free(dp);
    return result;
}

CoinChangeResult dp_coin_change(const int *coins, size_t num_coins, int amount) {
    CoinChangeResult result = { -1, NULL, 0 };
    if (coins == NULL || num_coins == 0 || amount < 0) return result;
    if (amount == 0) { result.min_coins = 0; return result; }

    int *dp = malloc((size_t)(amount + 1) * sizeof(int));
    int *from = malloc((size_t)(amount + 1) * sizeof(int));
    if (dp == NULL || from == NULL) {
        free(dp);
        free(from);
        return result;
    }

    for (int i = 0; i <= amount; i++) {
        dp[i] = INT_MAX;
        from[i] = -1;
    }
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (size_t j = 0; j < num_coins; j++) {
            if (coins[j] <= i && dp[i - coins[j]] != INT_MAX) {
                int val = dp[i - coins[j]] + 1;
                if (val < dp[i]) {
                    dp[i] = val;
                    from[i] = coins[j];
                }
            }
        }
    }

    if (dp[amount] == INT_MAX) {
        free(dp);
        free(from);
        return result;
    }

    result.min_coins = dp[amount];

    result.coins_used = malloc((size_t)dp[amount] * sizeof(int));
    if (result.coins_used != NULL) {
        size_t idx = 0;
        int rem = amount;
        while (rem > 0) {
            result.coins_used[idx++] = from[rem];
            rem -= from[rem];
        }
        result.num_coins_used = idx;
    }

    free(dp);
    free(from);
    return result;
}

void dp_coin_change_result_destroy(CoinChangeResult *result) {
    if (result == NULL) return;
    free(result->coins_used);
    result->coins_used = NULL;
    result->min_coins = -1;
    result->num_coins_used = 0;
}
