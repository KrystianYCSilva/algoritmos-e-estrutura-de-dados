---
description: "Pesquisa academica completa sobre heuristicas e meta-heuristicas para otimizacao: taxonomia, algoritmos, pseudocodigos, problemas de benchmark e referencias."
---

# Heuristicas e Meta-Heuristicas - Pesquisa Academica

> Documento de referencia para a Fase 3 do projeto Algoritmos e Heuristicas.
> Cobre taxonomia completa, pseudocodigos, analise de complexidade e referencias bibliograficas.

---

## 1. Introducao e Taxonomia

### 1.1 Metodos Exatos vs Heuristicas vs Meta-Heuristicas

**Metodos Exatos** garantem solucao otima, mas tem complexidade exponencial para problemas NP-hard:
- Branch and Bound, Branch and Cut, Dynamic Programming, Integer Linear Programming

**Heuristicas** sao procedimentos especificos para um problema que encontram solucoes boas (nao necessariamente otimas) em tempo polinomial:
- Construction heuristics (Nearest Neighbor, Greedy)
- Improvement heuristics (2-opt, 3-opt, Hill Climbing)

**Meta-Heuristicas** sao frameworks genericos de alto nivel que guiam heuristicas subordinadas para explorar o espaco de busca de forma eficiente:
- Single-solution (trajectory-based): SA, Tabu, ILS, VNS
- Population-based: GA, PSO, ACO, DE

### 1.2 Quando Usar Cada Abordagem

| Criterio | Exato | Heuristico | Meta-Heuristico |
|----------|-------|------------|-----------------|
| Garantia de otimalidade | Sim | Nao | Nao |
| Tempo computacional | Exponencial | Polinomial | Polinomial |
| Tamanho do problema | Pequeno (n < 100) | Medio | Grande (n > 1000) |
| Qualidade tipica | Otimo | Boa | Muito Boa |
| Generalidade | Especifico | Especifico | Generico |
| Facilidade de implementacao | Alta | Media | Media-Alta |

### 1.3 Taxonomia Completa

```
Optimization Algorithms
|
+-- Exact Methods
|   +-- Branch and Bound
|   +-- Branch and Cut
|   +-- Dynamic Programming
|   +-- Integer Linear Programming (ILP)
|
+-- Approximate Methods
    |
    +-- Approximation Algorithms (com garantia de ratio)
    |
    +-- Heuristics (problem-specific)
    |   +-- Construction Heuristics
    |   |   +-- Nearest Neighbor (TSP)
    |   |   +-- Greedy Construction
    |   |   +-- Insertion Heuristics
    |   +-- Improvement Heuristics (Local Search)
    |       +-- Hill Climbing (Steepest / First Improvement)
    |       +-- 2-opt, 3-opt, Or-opt
    |       +-- Lin-Kernighan
    |
    +-- Meta-Heuristics (generic frameworks)
        |
        +-- Single-Solution (Trajectory-Based)
        |   +-- Simulated Annealing (SA)
        |   +-- Tabu Search (TS)
        |   +-- GRASP
        |   +-- Iterated Local Search (ILS)
        |   +-- Variable Neighborhood Search (VNS)
        |   +-- Large Neighborhood Search (LNS / ALNS)
        |
        +-- Population-Based
        |   +-- Evolutionary Algorithms
        |   |   +-- Genetic Algorithm (GA)
        |   |   +-- Evolution Strategies (ES)
        |   |   +-- Differential Evolution (DE)
        |   |   +-- Estimation of Distribution Algorithms (EDA)
        |   |
        |   +-- Swarm Intelligence
        |   |   +-- Particle Swarm Optimization (PSO)
        |   |   +-- Ant Colony Optimization (ACO)
        |   |
        |   +-- Other
        |       +-- Scatter Search (SS)
        |
        +-- Hybrid
            +-- Memetic Algorithms (GA + Local Search)
            +-- Matheuristics (Meta + Mathematical Programming)
            +-- Hyperheuristics (heuristics choosing heuristics)
```

---

## 2. Heuristicas Classicas

### 2.1 Heuristicas de Construcao

#### Nearest Neighbor (TSP)

Constroi solucao iterativamente escolhendo a cidade mais proxima nao visitada.

```
NEAREST-NEIGHBOR(cities, start):
    tour = [start]
    unvisited = cities - {start}
    current = start
    while unvisited is not empty:
        next = argmin_{c in unvisited} dist(current, c)
        tour.append(next)
        unvisited.remove(next)
        current = next
    return tour
```

- **Complexidade**: O(n^2)
- **Qualidade**: Tipicamente 20-25% acima do otimo
- **Referencia**: Rosenkrantz, Stearns & Lewis (1977)

#### Greedy Construction

Seleciona a melhor opcao local a cada passo sem considerar consequencias futuras. Aplicavel a qualquer problema combinatorio.

#### Insertion Heuristics (TSP)

- **Cheapest Insertion**: Insere cidade que causa menor aumento no custo total
- **Farthest Insertion**: Insere cidade mais distante do tour parcial (melhor diversificacao)
- **Nearest Insertion**: Insere cidade mais proxima do tour parcial

Complexidade: O(n^2) para todas as variantes.

### 2.2 Heuristicas de Melhoria (Local Search)

#### Hill Climbing

```
HILL-CLIMBING(problem):
    current = initial_solution()
    loop:
        neighbor = best_neighbor(current)
        if eval(neighbor) <= eval(current):
            return current
        current = neighbor
```

**Variantes**:
- **Steepest Ascent**: Avalia TODOS os vizinhos, escolhe o melhor. O(|N(s)|) por iteracao.
- **First Improvement**: Aceita o primeiro vizinho melhor encontrado. Mais rapido por iteracao.
- **Random Restart**: Multiplas execucoes com pontos iniciais aleatorios. Mitiga otimos locais.

**Limitacao principal**: Fica preso em otimos locais. Nao tem mecanismo de escape.

#### 2-opt (TSP)

Remove 2 arestas e reconecta o tour de forma cruzada. Vizinhanca de tamanho O(n^2).

```
TWO-OPT(tour):
    improved = true
    while improved:
        improved = false
        for i = 0 to n-2:
            for j = i+2 to n-1:
                delta = dist(tour[i], tour[j]) + dist(tour[i+1], tour[j+1])
                      - dist(tour[i], tour[i+1]) - dist(tour[j], tour[j+1])
                if delta < 0:
                    reverse(tour, i+1, j)
                    improved = true
    return tour
```

- **Complexidade**: O(n^2) por iteracao, tipicamente O(n^2 * k) iteracoes totais
- **Referencia**: Croes (1958)

#### 3-opt e Lin-Kernighan

- **3-opt**: Remove 3 arestas, reconecta. 8 reconexoes possiveis. O(n^3) por iteracao.
- **Or-opt**: Move sequencias de 1, 2 ou 3 cidades consecutivas. Subconjunto do 3-opt, mais rapido.
- **Lin-Kernighan**: Generaliza k-opt com busca adaptativa do k. Estado da arte classico para TSP.
- **Referencia**: Lin, S. & Kernighan, B. W. (1973). "An effective heuristic algorithm for the traveling-salesman problem". Operations Research, 21(2), 498-516.

### 2.3 Heuristicas Problem-Specific

| Algoritmo | Problema | Garantia | Complexidade | Referencia |
|-----------|----------|----------|-------------|------------|
| Christofides | TSP | 1.5 * OPT | O(n^3) | Christofides (1976) |
| Johnson | Scheduling | 4/3 * OPT | O(n log n) | Johnson (1954) |
| First Fit Decreasing | Bin Packing | 11/9 * OPT + 6/9 | O(n log n) | Johnson (1973) |
| Best Fit | Bin Packing | Similar ao FFD | O(n log n) | - |

**Christofides**: Constroi MST, encontra matching perfeito minimo nos vertices de grau impar, combina para formar circuito euleriano, shortcutting para tour hamiltoniano.

---

## 3. Meta-Heuristicas: Single-Solution (Trajectory-Based)

Algoritmos que manipulam uma unica solucao ao longo do tempo, descrevendo uma trajetoria no espaco de busca.

### 3.1 Simulated Annealing (SA)

**Origem**: Kirkpatrick, Gelatt & Vecchi (1983). Analogia com recozimento em metalurgia.

**Ideia**: Aceita solucoes piores com probabilidade decrescente (temperatura), escapando de otimos locais.

```
SIMULATED-ANNEALING(s0, T0, alpha, T_min, L):
    s = s0
    s_best = s
    T = T0
    while T > T_min:
        for i = 1 to L:                          // L = Markov chain length
            s_prime = RANDOM-NEIGHBOR(s)
            delta = f(s_prime) - f(s)             // f = funcao objetivo (minimizar)
            if delta < 0:
                s = s_prime                       // aceita melhoria
            else if random(0,1) < exp(-delta/T):
                s = s_prime                       // aceita piora com probabilidade
            if f(s) < f(s_best):
                s_best = s
        T = alpha * T                             // resfriamento geometrico
    return s_best
```

**Parametros Criticos**:
| Parametro | Descricao | Valor Tipico |
|-----------|-----------|-------------|
| T0 | Temperatura inicial | Calibrar para aceitar ~80% dos movimentos |
| alpha | Taxa de resfriamento | 0.90 - 0.999 |
| T_min | Temperatura final | 0.001 ou 1e-8 |
| L | Iteracoes por temperatura | n ou n^2 (n = tamanho do problema) |

**Cooling Schedules**:
- **Geometrico**: T(k+1) = alpha * T(k). Mais usado, alpha in [0.9, 0.999].
- **Linear**: T(k+1) = T(k) - delta. Simples mas menos flexivel.
- **Logaritmico**: T(k) = T0 / ln(1 + k). Garantia teorica de convergencia, impraticavel.
- **Adaptive**: Ajusta T baseado na taxa de aceitacao atual.

**Convergencia**: Com cooling logaritmico, converge para otimo global com probabilidade 1 (Hajek, 1988). Na pratica, cooling geometrico e suficiente.

**Vantagens**: Simples, poucos parametros, convergencia teorica, genericidade.
**Desvantagens**: Lento para convergir, sensivel ao cooling schedule, dificil calibrar T0.

**Complexidade**: O(L * n_temperatures * custo_vizinho)

**Referencia**: Kirkpatrick, S., Gelatt, C. D., & Vecchi, M. P. (1983). "Optimization by Simulated Annealing". Science, 220(4598), 671-680.

---

### 3.2 Tabu Search (TS)

**Origem**: Glover (1986). Usa memoria explicita para evitar ciclagem e guiar a busca.

```
TABU-SEARCH(s0, max_iter, tabu_tenure):
    s = s0
    s_best = s
    tabu_list = []
    for iter = 1 to max_iter:
        N_s = NEIGHBORHOOD(s)
        // Filtrar movimentos tabu (exceto aspiracao)
        candidates = {s' in N_s : move(s,s') not in tabu_list
                      OR f(s') < f(s_best)}        // aspiration
        s_prime = argmin_{s' in candidates} f(s')
        // Atualizar lista tabu
        tabu_list.add(move(s, s_prime))
        if |tabu_list| > tabu_tenure:
            tabu_list.remove_oldest()
        s = s_prime
        if f(s) < f(s_best):
            s_best = s
    return s_best
```

**Parametros**:
| Parametro | Descricao | Valor Tipico |
|-----------|-----------|-------------|
| tabu_tenure | Duracao do tabu | sqrt(n) a n |
| max_iter | Iteracoes maximas | 1000 - 100000 |
| Tabu attribute | O que e proibido | Move, solucao, ou atributo |

**Estruturas de Memoria**:
- **Short-term (Recency)**: Lista tabu classica. Proibe movimentos recentes.
- **Medium-term (Frequency, Intensification)**: Conta frequencia de bons atributos. Concentra busca em regioes promissoras.
- **Long-term (Frequency, Diversification)**: Penaliza atributos muito frequentes. Forca explorar regioes nao visitadas.

**Aspiration Criteria**: Override do tabu quando:
- Move leva a novo s_best (mais comum)
- Move nao foi feito ha muito tempo
- Todos os vizinhos sao tabu

**Vantagens**: Evita ciclagem, deterministico, muito eficaz, memoria guia a busca.
**Desvantagens**: Mais parametros que SA, design da vizinhanca e critico.

**Referencia**: Glover, F. (1986). "Future Paths for Integer Programming and Links to Artificial Intelligence". Computers & OR, 13(5), 533-549.

---

### 3.3 GRASP (Greedy Randomized Adaptive Search Procedure)

**Origem**: Feo & Resende (1995). Multi-start: construcao aleatorizada + busca local.

```
GRASP(max_iter, alpha):
    s_best = null
    for iter = 1 to max_iter:
        s = GREEDY-RANDOMIZED-CONSTRUCTION(alpha)
        s = LOCAL-SEARCH(s)
        if s_best == null OR f(s) < f(s_best):
            s_best = s
    return s_best

GREEDY-RANDOMIZED-CONSTRUCTION(alpha):
    s = empty
    CL = candidate_list                     // elementos disponiveis
    while s is incomplete:
        g_min = min{g(e) : e in CL}        // melhor custo incremental
        g_max = max{g(e) : e in CL}
        threshold = g_min + alpha * (g_max - g_min)
        RCL = {e in CL : g(e) <= threshold} // Restricted Candidate List
        e = random(RCL)
        s = s + e
        update(CL)
    return s
```

**Parametros**:
| Parametro | Descricao | Valor Tipico |
|-----------|-----------|-------------|
| alpha | Grau de aleatoriedade | 0.0 (greedy) a 1.0 (random) |
| max_iter | Numero de iteracoes | 100 - 10000 |
| Local search | Tipo de busca local | 2-opt, steepest descent |

**Reactive GRASP**: Mantem conjunto de valores alpha, ajusta probabilidade de cada um baseado em performance historica.

**Path Relinking**: Extensao que gera solucoes intermediarias entre duas boas solucoes.

**Vantagens**: Simples, paralelizavel, robusto, nao depende de solucao inicial unica.
**Desvantagens**: Sem memoria entre iteracoes, muitas iteracoes necessarias.

**Referencia**: Feo, T. A. & Resende, M. G. C. (1995). "Greedy Randomized Adaptive Search Procedures". J. Global Optimization, 6(2), 109-133.

---

### 3.4 Iterated Local Search (ILS)

**Origem**: Lourenco, Martin & Stutzle (2003). Perturba solucao local otima e re-otimiza.

```
ILS(s0, max_iter):
    s = LOCAL-SEARCH(s0)
    s_best = s
    for iter = 1 to max_iter:
        s_prime = PERTURBATION(s, history)
        s_double_prime = LOCAL-SEARCH(s_prime)
        s = ACCEPTANCE-CRITERION(s, s_double_prime, history)
        if f(s) < f(s_best):
            s_best = s
    return s_best
```

**Componentes Chave**:
- **Perturbation**: Deve ser forte o suficiente para escapar da bacia de atracao atual, mas fraca o suficiente para manter boa estrutura. Exemplo TSP: 3 double-bridge moves.
- **Local Search**: Qualquer busca local (2-opt, steepest descent, etc.)
- **Acceptance Criterion**:
  - Better: aceita apenas se melhor
  - Random Walk: sempre aceita
  - SA-like: aceita piores com probabilidade
  - Restart: reinicia apos k iteracoes sem melhoria

**Vantagens**: Extremamente simples, muito eficaz, facil de implementar e adaptar.
**Desvantagens**: Sensivel a forca da perturbacao, sem diversificacao explicita.

**Referencia**: Lourenco, H. R., Martin, O. C. & Stutzle, T. (2003). "Iterated Local Search". In Handbook of Metaheuristics, Ch. 11, pp. 320-353.

---

### 3.5 Variable Neighborhood Search (VNS)

**Origem**: Mladenovic & Hansen (1997). Troca sistematicamente entre vizinhancas.

**Principio**: Otimos locais em diferentes vizinhancas geralmente sao diferentes. Explorar vizinhancas crescentes permite escapar.

```
BASIC-VNS(s0, N_1..N_kmax, max_iter):
    s = s0
    for iter = 1 to max_iter:
        k = 1
        while k <= k_max:
            s_prime = SHAKE(s, N_k)              // ponto aleatorio em N_k(s)
            s_double_prime = LOCAL-SEARCH(s_prime)
            if f(s_double_prime) < f(s):
                s = s_double_prime
                k = 1                            // melhoria: volta para N_1
            else:
                k = k + 1                        // sem melhoria: vizinhanca maior
    return s
```

**Variantes**:
| Variante | Descricao |
|----------|-----------|
| Basic VNS (BVNS) | Shaking + Local Search + Move-or-not |
| Reduced VNS (RVNS) | Apenas shaking, sem local search (rapido) |
| VND (Variable Neighborhood Descent) | Apenas local search com multiplas vizinhancas |
| General VNS (GVNS) | VNS com VND como busca local |
| Skewed VNS | Aceita solucoes piores se distantes (diversificacao) |

**Vantagens**: Fundamentacao teorica solida, explora multiplas estruturas de vizinhanca.
**Desvantagens**: Requer definir conjunto de vizinhancas crescentes (N_1, N_2, ..., N_kmax).

**Referencia**: Mladenovic, N. & Hansen, P. (1997). "Variable Neighborhood Search". Computers & OR, 24(11), 1097-1100.

---

### 3.6 Large Neighborhood Search (LNS) / Adaptive LNS (ALNS)

**Origem**: Shaw (1998). Destroi e reconstroi parte significativa da solucao.

```
LNS(s0, max_iter, d):
    s = s0
    s_best = s
    for iter = 1 to max_iter:
        s_prime = DESTROY(s, d)           // remove d% dos elementos
        s_prime = REPAIR(s_prime)         // reconstroi (possivelmente com heuristica)
        if ACCEPT(s, s_prime):            // SA-like ou better
            s = s_prime
        if f(s) < f(s_best):
            s_best = s
    return s_best
```

**Adaptive LNS (ALNS)** - Ropke & Pisinger (2006):
- Multiplos operadores de destroy (random, worst, related, Shaw)
- Multiplos operadores de repair (greedy, regret-k, random)
- Roulette wheel selection adaptativa: peso de cada operador ajustado pela performance

```
ALNS(s0, max_iter):
    s = s0; s_best = s
    weights_d = [1, 1, ..., 1]           // pesos dos operadores destroy
    weights_r = [1, 1, ..., 1]           // pesos dos operadores repair
    for iter = 1 to max_iter:
        d = SELECT-DESTROY(weights_d)     // roulette wheel
        r = SELECT-REPAIR(weights_r)
        s_prime = REPAIR(r, DESTROY(d, s))
        if ACCEPT(s, s_prime):
            s = s_prime
            UPDATE-WEIGHTS(d, r, improvement)
        if f(s) < f(s_best):
            s_best = s
    return s_best
```

**Vantagens**: Vizinhancas muito grandes (exponenciais), eficaz para VRP e scheduling.
**Desvantagens**: Design dos operadores e problem-specific, mais complexo de implementar.

**Referencias**:
- Shaw, P. (1998). "Using Constraint Programming and Local Search Methods to Solve Vehicle Routing Problems". CP-98.
- Ropke, S. & Pisinger, D. (2006). "An Adaptive Large Neighborhood Search Heuristic for the Pickup and Delivery Problem with Time Windows". Transportation Science, 40(4), 455-472.

---

## 4. Meta-Heuristicas: Population-Based

Algoritmos que manipulam um conjunto (populacao) de solucoes simultaneamente.

### 4.1 Genetic Algorithm (GA)

**Origem**: Holland (1975), popularizado por Goldberg (1989). Inspirado em evolucao natural.

```
GENETIC-ALGORITHM(pop_size, max_gen, p_c, p_m):
    P = INITIALIZE-POPULATION(pop_size)
    EVALUATE(P)
    for gen = 1 to max_gen:
        P_new = []
        while |P_new| < pop_size:
            p1 = SELECTION(P)                    // selecao de pais
            p2 = SELECTION(P)
            if random() < p_c:
                (c1, c2) = CROSSOVER(p1, p2)    // recombinacao
            else:
                (c1, c2) = (copy(p1), copy(p2))
            MUTATE(c1, p_m)                      // mutacao
            MUTATE(c2, p_m)
            P_new.add(c1, c2)
        EVALUATE(P_new)
        P = SURVIVOR-SELECTION(P, P_new)         // elitismo ou geracional
    return best(P)
```

**Operadores de Selecao**:
| Operador | Descricao | Pressao Seletiva |
|----------|-----------|-----------------|
| Tournament (k) | Melhor de k individuos aleatorios | Ajustavel via k |
| Roulette Wheel | Proporcional ao fitness | Alta variancia |
| Rank-Based | Proporcional ao ranking | Moderada |
| Stochastic Universal Sampling | Variante da roulette, menos bias | Baixa variancia |

**Operadores de Crossover** (para representacao binaria/inteira):
| Operador | Descricao |
|----------|-----------|
| One-Point | Corta em 1 ponto, troca segmentos |
| Two-Point | Corta em 2 pontos, troca segmento central |
| Uniform | Cada gene vem de um pai com prob 0.5 |
| PMX (Partially Mapped) | Para permutacoes (TSP) |
| OX (Order Crossover) | Para permutacoes, preserva ordem relativa |
| CX (Cycle Crossover) | Para permutacoes, preserva posicoes absolutas |

**Operadores de Mutacao**:
| Operador | Descricao |
|----------|-----------|
| Bit Flip | Inverte bit com prob p_m |
| Swap | Troca duas posicoes |
| Inversion | Inverte subsequencia |
| Insertion | Move elemento para outra posicao |
| Scramble | Embaralha subsequencia |

**Parametros Tipicos**:
| Parametro | Valor Tipico |
|-----------|-------------|
| pop_size | 50 - 200 |
| p_c (crossover) | 0.6 - 0.9 |
| p_m (mutacao) | 0.01 - 0.1 (por gene) |
| max_gen | 100 - 10000 |
| Elitismo | 1-5 melhores preservados |

**Vantagens**: Robusto, paralelizavel, boa exploracao, bem estudado.
**Desvantagens**: Convergencia prematura, muitos parametros, lento sem busca local.

**Referencias**:
- Holland, J. H. (1975). *Adaptation in Natural and Artificial Systems*. University of Michigan Press.
- Goldberg, D. E. (1989). *Genetic Algorithms in Search, Optimization, and Machine Learning*. Addison-Wesley.

---

### 4.2 Differential Evolution (DE)

**Origem**: Storn & Price (1997). Para otimizacao continua. Simples e eficaz.

```
DE-RAND-1-BIN(pop_size, max_gen, F, CR, D):
    // D = dimensoes, F = fator de escala, CR = crossover rate
    P = INITIALIZE-POPULATION(pop_size, D)       // vetores reais
    EVALUATE(P)
    for gen = 1 to max_gen:
        for i = 1 to pop_size:
            // Mutation: seleciona 3 individuos distintos (r1 != r2 != r3 != i)
            v = P[r1] + F * (P[r2] - P[r3])     // donor vector
            // Crossover: binomial
            u = []
            j_rand = random_int(1, D)
            for j = 1 to D:
                if random() < CR OR j == j_rand:
                    u[j] = v[j]                   // do donor
                else:
                    u[j] = P[i][j]                // do target
            // Selection: greedy
            if f(u) <= f(P[i]):
                P[i] = u
    return best(P)
```

**Variantes DE/x/y/z**: x=base vector, y=num differences, z=crossover type
| Variante | Mutacao |
|----------|---------|
| DE/rand/1 | v = x_r1 + F*(x_r2 - x_r3) |
| DE/best/1 | v = x_best + F*(x_r1 - x_r2) |
| DE/rand/2 | v = x_r1 + F*(x_r2 - x_r3) + F*(x_r4 - x_r5) |
| DE/best/2 | v = x_best + F*(x_r1 - x_r2) + F*(x_r3 - x_r4) |
| DE/current-to-best/1 | v = x_i + F*(x_best - x_i) + F*(x_r1 - x_r2) |

**Parametros Tipicos**:
| Parametro | Valor Tipico |
|-----------|-------------|
| pop_size | 5*D a 10*D |
| F (scaling factor) | 0.5 - 0.9 |
| CR (crossover rate) | 0.1 - 1.0 |

**Vantagens**: Poucos parametros, eficaz para otimizacao continua, simples de implementar.
**Desvantagens**: Principalmente para problemas continuos, convergencia pode ser lenta.

**Referencia**: Storn, R. & Price, K. (1997). "Differential Evolution - A Simple and Efficient Heuristic for Global Optimization over Continuous Spaces". J. Global Optimization, 11(4), 341-359.

---

### 4.3 Particle Swarm Optimization (PSO)

**Origem**: Kennedy & Eberhart (1995). Inspirado em comportamento de bandos de passaros.

```
PSO(n_particles, max_iter, w, c1, c2, D):
    // Inicializar posicoes e velocidades
    for i = 1 to n_particles:
        x[i] = random_position(D)
        v[i] = random_velocity(D)
        p_best[i] = x[i]                         // melhor pessoal
    g_best = argmin_{i} f(p_best[i])              // melhor global

    for iter = 1 to max_iter:
        for i = 1 to n_particles:
            // Atualizar velocidade
            for d = 1 to D:
                r1 = random(0, 1)
                r2 = random(0, 1)
                v[i][d] = w * v[i][d]                          // inercia
                        + c1 * r1 * (p_best[i][d] - x[i][d])  // cognitivo
                        + c2 * r2 * (g_best[d] - x[i][d])     // social
            // Atualizar posicao
            x[i] = x[i] + v[i]
            // Atualizar p_best e g_best
            if f(x[i]) < f(p_best[i]):
                p_best[i] = x[i]
                if f(x[i]) < f(g_best):
                    g_best = x[i]
    return g_best
```

**Parametros**:
| Parametro | Descricao | Valor Tipico |
|-----------|-----------|-------------|
| w | Peso de inercia | 0.4 - 0.9 (decrescente) |
| c1 | Coeficiente cognitivo | 1.5 - 2.0 |
| c2 | Coeficiente social | 1.5 - 2.0 |
| n_particles | Tamanho do enxame | 20 - 50 |
| v_max | Velocidade maxima | Limitar ao range do espaco de busca |

**Variantes**:
- **Constriction Factor**: Clerc & Kennedy (2002). chi = 2/|2-phi-sqrt(phi^2-4*phi)| onde phi=c1+c2>4
- **Local Best (lbest)**: Cada particula influenciada apenas por vizinhos topologicos
- **Comprehensive Learning PSO (CLPSO)**: Cada dimensao aprende de diferentes exemplares

**Vantagens**: Muito simples, poucos parametros, convergencia rapida, bom para continuo.
**Desvantagens**: Convergencia prematura, dificil para discreto, sem garantia teorica.

**Referencia**: Kennedy, J. & Eberhart, R. (1995). "Particle Swarm Optimization". Proc. IEEE Int. Conf. Neural Networks, 1942-1948.

---

### 4.4 Ant Colony Optimization (ACO)

**Origem**: Dorigo (1992), Dorigo & Stutzle (2004). Inspirado em formigas depositando feromonio.

```
ANT-SYSTEM(n_ants, max_iter, alpha, beta, rho, Q):
    tau = INITIALIZE-PHEROMONE(tau_0)             // matriz de feromonio
    for iter = 1 to max_iter:
        solutions = []
        for k = 1 to n_ants:
            s_k = CONSTRUCT-SOLUTION(tau, alpha, beta)
            solutions.add(s_k)
        s_best_iter = best(solutions)
        // Evaporacao
        tau = (1 - rho) * tau                     // rho = taxa de evaporacao
        // Deposito
        for k = 1 to n_ants:
            for each edge (i,j) in s_k:
                tau[i][j] += Q / f(s_k)           // deposito proporcional a qualidade
    return s_best_overall

CONSTRUCT-SOLUTION(tau, alpha, beta):
    s = [start_node]
    while s is incomplete:
        // Regra de transicao probabilistica
        for each candidate j:
            p(j) = tau[i][j]^alpha * eta[i][j]^beta
                  / sum_{l in candidates} tau[i][l]^alpha * eta[i][l]^beta
        // eta = informacao heuristica (ex: 1/dist para TSP)
        j = ROULETTE-WHEEL(p)
        s.add(j)
    return s
```

**Variantes Principais**:
| Variante | Deposito | Caracteristica |
|----------|----------|---------------|
| Ant System (AS) | Todas as formigas | Original, convergencia lenta |
| Ant Colony System (ACS) | Apenas melhor global | Pseudo-random proportional rule |
| MAX-MIN AS (MMAS) | Apenas melhor | Limites tau_min, tau_max no feromonio |
| Rank-Based AS | Top-w formigas | Deposito ponderado por ranking |

**Parametros**:
| Parametro | Descricao | Valor Tipico |
|-----------|-----------|-------------|
| n_ants | Numero de formigas | n (num cidades) |
| alpha | Peso do feromonio | 1.0 |
| beta | Peso da heuristica | 2.0 - 5.0 |
| rho | Taxa de evaporacao | 0.1 - 0.5 |
| Q | Constante de deposito | 1.0 |

**Vantagens**: Natural para problemas de roteamento, boa exploracao, adaptavel.
**Desvantagens**: Muitos parametros, convergencia pode ser lenta, complexo de implementar.

**Referencia**: Dorigo, M. & Stutzle, T. (2004). *Ant Colony Optimization*. MIT Press.

---

### 4.5 Evolution Strategies (ES)

**Origem**: Rechenberg (1973), Schwefel (1977). Focadas em otimizacao continua com self-adaptation.

```
(MU+LAMBDA)-ES(mu, lambda, D, max_gen):
    P = INITIALIZE(mu, D)                     // mu pais
    for gen = 1 to max_gen:
        offspring = []
        for i = 1 to lambda:
            p = random_parent(P)
            sigma_new = p.sigma * exp(tau * N(0,1))   // self-adaptation
            x_new = p.x + sigma_new * N(0, I_D)       // mutacao gaussiana
            offspring.add((x_new, sigma_new))
        EVALUATE(offspring)
        // (mu+lambda): seleciona mu melhores de pais + filhos
        // (mu,lambda): seleciona mu melhores APENAS dos filhos
        P = SELECT-BEST(mu, P union offspring)  // ou apenas offspring
    return best(P)
```

**Variantes**:
- **(mu+lambda)-ES**: Elitista - seleciona de pais + filhos. Mais conservador.
- **(mu,lambda)-ES**: Nao-elitista - seleciona apenas de filhos. lambda > mu obrigatorio. Mais explorador.
- **CMA-ES**: Covariance Matrix Adaptation ES. Estado da arte para otimizacao continua black-box.

**Self-Adaptation**: Parametros de mutacao (sigma) evoluem junto com a solucao.
- Learning rate: tau ~ 1/sqrt(2*D)

**Referencia**: Rechenberg, I. (1973). *Evolutionsstrategie: Optimierung technischer Systeme nach Prinzipien der biologischen Evolution*. Frommann-Holzboog.

---

### 4.6 Estimation of Distribution Algorithms (EDA)

**Origem**: Muhlenbein & Paass (1996). Substitui crossover/mutacao por modelo probabilistico.

```
EDA(pop_size, max_gen, select_ratio):
    P = INITIALIZE-POPULATION(pop_size)
    for gen = 1 to max_gen:
        EVALUATE(P)
        S = SELECT-BEST(P, select_ratio * pop_size)
        M = BUILD-PROBABILISTIC-MODEL(S)       // estima distribuicao
        P = SAMPLE(M, pop_size)                // gera nova populacao do modelo
    return best(P)
```

**Tipos de Modelo**:
| Modelo | Dependencias | Exemplo |
|--------|-------------|---------|
| Univariado | Nenhuma | PBIL, UMDA, cGA |
| Bivariado | Pares de variaveis | MIMIC, BMDA |
| Multivariado | Arbitrarias | BOA (Bayesian), ECGA |

**Vantagens**: Captura correlacoes entre variaveis, sem operadores ad-hoc.
**Desvantagens**: Custo computacional do modelo, dificil para problemas continuos de alta dimensao.

**Referencia**: Muhlenbein, H. & Paass, G. (1996). "From Recombination of Genes to the Estimation of Distributions". PPSN IV, pp. 178-187.

---

### 4.7 Scatter Search (SS)

**Origem**: Glover (1977). Combina diversificacao e intensificacao com reference set.

```
SCATTER-SEARCH(P_size, b, max_iter):
    P = DIVERSIFICATION-GENERATION(P_size)
    EVALUATE(P)
    RefSet = SELECT-REFERENCE-SET(P, b)    // b/2 melhores + b/2 mais diversos
    while not converged:
        subsets = SUBSET-GENERATION(RefSet)
        for each S in subsets:
            trial = SOLUTION-COMBINATION(S) // combina solucoes do subset
            trial = IMPROVEMENT(trial)      // busca local
            RefSet = UPDATE-REFSET(RefSet, trial)
    return best(RefSet)
```

**Componentes**:
1. **Diversification Generation**: Gera populacao inicial diversa
2. **Improvement Method**: Busca local em cada solucao
3. **Reference Set Update**: Mantem conjunto pequeno (b~10-20) de solucoes elite e diversas
4. **Subset Generation**: Gera subconjuntos para combinacao (tipicamente pares)
5. **Solution Combination**: Combina solucoes (voting, path relinking, crossover)

**Vantagens**: Populacao pequena (eficiente), combina intensificacao e diversificacao.
**Desvantagens**: Mais complexo de implementar que GA.

**Referencia**: Glover, F. (1977). "Heuristics for Integer Programming Using Surrogate Constraints". Decision Sciences, 8(1), 156-166.

---

## 5. Meta-Heuristicas Nature-Inspired (Perspectiva Critica)

### Nota sobre a Controversia

A proliferacao de algoritmos "inspirados na natureza" (animal-based, physics-based) tem sido criticada pela comunidade academica. Muitos sao variantes de algoritmos existentes com nomes diferentes e pouca contribuicao teorica. Ver Sorensen (2015): "Metaheuristics - the metaphor exposed".

**Problemas identificados**:
- Metaforas biologicas obscurecem os mecanismos reais (exploracao/intensificacao)
- Muitos algoritmos sao equivalent a SA, GA ou DE com operadores diferentes
- Comparacoes experimentais frequentemente injustas (parametros nao tunados para baselines)
- Falta de analise teorica de convergencia

### 5.1 Firefly Algorithm (FA)

**Origem**: Yang (2008). Inspirado em comportamento de vagalumes.

Movimento de cada firefly em direcao a fireflies mais brilhantes (melhor fitness). Atratividade decresce com distancia. Essencialmente um mecanismo de busca local com diversificacao aleatoria.

**Analise**: Equivalente a PSO com topologia especifica ou random search com bias.

### 5.2 Cuckoo Search (CS)

**Origem**: Yang & Deb (2009). Inspirado em parasitismo de ninho de cucos.

Usa Levy flights para gerar novos candidatos. Levy flights produzem steps ocasionalmente grandes (heavy-tailed distribution), o que melhora exploracao.

**Analise**: O componente util (Levy flights) pode ser adicionado a qualquer meta-heuristica.

### 5.3 Grey Wolf Optimizer (GWO)

**Origem**: Mirjalili et al. (2014). Inspirado em hierarquia social de lobos cinzentos.

**Analise**: Mecanismo de busca e equivalente a media ponderada de 3 melhores solucoes com perturbacao aleatoria decrescente. Similar a PSO simplificado.

### 5.4 Whale Optimization Algorithm (WOA)

**Origem**: Mirjalili & Lewis (2016). Inspirado em tecnica de caca de baleias jubarte.

**Analise**: Combina busca local espiral com exploracao aleatoria. Mecanismos nao oferecem vantagem demonstrada sobre DE ou PSO bem tunados.

### 5.5 Recomendacao

Para o projeto academico, **nao recomendamos implementar** estes algoritmos na Fase 3 principal. Recomendamos focar em meta-heuristicas classicas com fundamento teorico solido (SA, Tabu, GA, DE, PSO, ACO) que sao amplamente reconhecidas na literatura.

**Referencia critica**: Sorensen, K. (2015). "Metaheuristics - the metaphor exposed". International Transactions in Operational Research, 22(1), 3-18.

---

## 6. Algoritmos Hibridos

### 6.1 Memetic Algorithms (MA)

**Origem**: Moscato (1989). Combina GA (exploracao global) com busca local (intensificacao).

```
MEMETIC-ALGORITHM(pop_size, max_gen, p_c, p_m):
    P = INITIALIZE-POPULATION(pop_size)
    for i = 1 to pop_size:
        P[i] = LOCAL-SEARCH(P[i])             // melhora cada individuo
    for gen = 1 to max_gen:
        P_new = []
        while |P_new| < pop_size:
            p1, p2 = SELECTION(P)
            c1, c2 = CROSSOVER(p1, p2, p_c)
            MUTATE(c1, p_m); MUTATE(c2, p_m)
            c1 = LOCAL-SEARCH(c1)             // busca local em cada filho
            c2 = LOCAL-SEARCH(c2)
            P_new.add(c1, c2)
        P = SURVIVOR-SELECTION(P, P_new)
    return best(P)
```

**Variantes**:
- **Lamarckian**: Resultado da busca local substitui o individuo (genotipo = fenotipo melhorado)
- **Baldwinian**: Fitness melhorado pela busca local, mas genotipo nao muda

**Vantagens**: Combina o melhor de GA (diversificacao) e busca local (intensificacao).
**Desvantagens**: Custo computacional alto (busca local para cada individuo).

**Referencia**: Moscato, P. (1989). "On Evolution, Search, Optimization, Genetic Algorithms and Martial Arts: Towards Memetic Algorithms". Tech Report C3P 826, Caltech.

### 6.2 Matheuristics

Combinacao de meta-heuristicas com metodos de programacao matematica (LP, MIP).

**Abordagens**:
- **Decomposition**: Meta-heuristica decide decomposicao, solver exato resolve subproblemas
- **Improvement**: Solver MIP usado como operador de busca local (fix-and-optimize, RINS)
- **Construction**: Relaxacao LP guia construcao heuristica

**Exemplo**: Fix-and-Optimize para scheduling:
1. Meta-heuristica fixa subset de variaveis
2. MIP solver otimiza variaveis livres
3. Repete com diferentes subsets fixos

### 6.3 Hyperheuristics

**Origem**: Burke et al. (2003). Heuristicas que selecionam ou geram heuristicas.

```
SELECTION-HYPERHEURISTIC(LLHs, max_iter):
    s = initial_solution()
    for iter = 1 to max_iter:
        llh = SELECT-LOW-LEVEL-HEURISTIC(LLHs, history)
        s_prime = APPLY(llh, s)
        s = MOVE-ACCEPTANCE(s, s_prime)     // accept all, only improving, SA-like
    return s
```

**Dois niveis**:
- **High-level**: Seleciona qual heuristica aplicar (learning mechanism + move acceptance)
- **Low-level heuristics (LLHs)**: Pool de operadores especificos (2-opt, swap, insert, etc.)

**Tipos**:
- **Selection**: Seleciona heuristica de um pool pre-definido
- **Generation**: Gera novas heuristicas (Genetic Programming)

**Vantagens**: Generalidade total, nao precisa expertise no problema, adaptativo.
**Desvantagens**: Performance pode ser inferior a meta-heuristica problem-specific bem tunada.

**Referencia**: Burke, E. K., et al. (2003). "Hyper-heuristics: An Emerging Direction in Modern Search Technology". In Handbook of Metaheuristics, pp. 457-474.

---

## 7. Problemas de Benchmark

### 7.1 Problemas Combinatorios

#### TSP (Traveling Salesman Problem)
- **Descricao**: Encontrar tour de menor custo visitando todas as n cidades exatamente uma vez
- **Complexidade**: NP-hard
- **Benchmark**: TSPLIB (Reinelt, 1991) - instancias de 14 a 85900 cidades
- **Estado da arte**: LKH (Lin-Kernighan-Helsgott), Concorde (exato para instancias medias)
- **URL**: http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/

#### VRP (Vehicle Routing Problem)
- **CVRP**: Capacitated VRP - veiculos com capacidade limitada
- **VRPTW**: VRP with Time Windows - janelas de tempo para entregas
- **Benchmark**: Solomon (1987) instances, Uchoa et al. (2017) new instances
- **Complexidade**: NP-hard (generalizacao do TSP)

#### Knapsack Variants
- **0/1 Knapsack**: Selecionar itens para maximizar valor sem exceder capacidade
- **Multidimensional Knapsack (MKP)**: Multiplas restricoes de capacidade
- **Quadratic Knapsack (QKP)**: Lucro depende de pares de itens selecionados
- **Benchmark**: OR-Library (Beasley, 1990)

#### Job Shop Scheduling (JSP)
- **Descricao**: n jobs, m maquinas, cada job tem sequencia fixa de operacoes
- **Objetivo**: Minimizar makespan (tempo total)
- **Benchmark**: Taillard (1993), Adams, Balas & Zawack (1988)
- **Complexidade**: NP-hard (m >= 2)

#### Flow Shop Scheduling (FSP)
- **Descricao**: Como JSP, mas todos os jobs seguem mesma sequencia de maquinas
- **Benchmark**: Taillard (1993)

#### Graph Coloring
- **Descricao**: Colorir vertices com minimo de cores, sem vertices adjacentes com mesma cor
- **Benchmark**: DIMACS Challenge instances
- **Complexidade**: NP-hard (k >= 3)

#### Set Cover
- **Descricao**: Encontrar menor colecao de subconjuntos que cobre universo
- **Benchmark**: OR-Library
- **Complexidade**: NP-hard, melhor approximation ratio: H_n (harmonic number)

### 7.2 Problemas Continuos

Funcoes de teste padrao para otimizacao continua (minimizacao).

#### Sphere Function
- **Formula**: f(x) = sum_{i=1}^{D} x_i^2
- **Otimo global**: f(0, ..., 0) = 0
- **Caracteristica**: Unimodal, convexa, separavel. Facil - baseline.

#### Rastrigin Function
- **Formula**: f(x) = 10*D + sum_{i=1}^{D} [x_i^2 - 10*cos(2*pi*x_i)]
- **Otimo global**: f(0, ..., 0) = 0
- **Caracteristica**: Multimodal (muitos otimos locais), separavel. Dificil.
- **Dominio tipico**: x_i in [-5.12, 5.12]

#### Rosenbrock Function
- **Formula**: f(x) = sum_{i=1}^{D-1} [100*(x_{i+1} - x_i^2)^2 + (1 - x_i)^2]
- **Otimo global**: f(1, ..., 1) = 0
- **Caracteristica**: Unimodal (D<=3), vale estreito curvo (banana). Dificil de convergir.
- **Dominio tipico**: x_i in [-5, 10]

#### Ackley Function
- **Formula**: f(x) = -20*exp(-0.2*sqrt(1/D * sum x_i^2)) - exp(1/D * sum cos(2*pi*x_i)) + 20 + e
- **Otimo global**: f(0, ..., 0) = 0
- **Caracteristica**: Multimodal, quase plana longe da origem. Testa exploracao.
- **Dominio tipico**: x_i in [-32.768, 32.768]

#### Schwefel Function
- **Formula**: f(x) = 418.9829*D - sum_{i=1}^{D} x_i * sin(sqrt(|x_i|))
- **Otimo global**: f(420.9687, ..., 420.9687) = 0
- **Caracteristica**: Multimodal, otimo global longe dos outros otimos locais. Enganosa.
- **Dominio tipico**: x_i in [-500, 500]

#### Griewank Function
- **Formula**: f(x) = 1 + sum x_i^2/4000 - prod cos(x_i/sqrt(i))
- **Otimo global**: f(0, ..., 0) = 0
- **Caracteristica**: Multimodal, muitos otimos locais regularmente distribuidos.
- **Dominio tipico**: x_i in [-600, 600]

### 7.3 Fontes de Benchmark

| Fonte | Problemas | URL |
|-------|-----------|-----|
| TSPLIB | TSP, ATSP, HCP | comopt.ifi.uni-heidelberg.de/software/TSPLIB95 |
| OR-Library | Knapsack, Set Cover, Scheduling | people.brunel.ac.uk/~mastjjb/jeb/info.html |
| Solomon | VRPTW | w2.uqo.ca/desaulniers/Solomon |
| DIMACS | Graph Coloring, Clique | dimacs.rutgers.edu |
| CEC Competitions | Continuous Optimization | Annual competitions with new test suites |
| Taillard | Flow Shop, Job Shop | mistic.heig-vd.ch/taillard |

---

## 8. Criterios de Avaliacao

### 8.1 Metricas de Qualidade

| Metrica | Descricao | Formula |
|---------|-----------|---------|
| Best found | Melhor solucao encontrada | f(s_best) |
| Mean | Media de multiplas execucoes | (1/R) * sum f(s_best_r) |
| Std Dev | Desvio padrao | Robustez do algoritmo |
| Gap (%) | Distancia ao otimo conhecido | 100 * (f_found - f_opt) / f_opt |
| Hit rate | Frequencia de encontrar otimo | n_optimal / n_runs |
| Time to target | Tempo para atingir solucao de qualidade q | Empirico |

### 8.2 Analise de Convergencia

- **Convergence curve**: f(s_best) vs iteracao/tempo. Mostra velocidade de convergencia.
- **Time-to-target plot**: Probabilidade acumulada de encontrar solucao com gap <= q% vs tempo.
- **Run-length distribution**: Distribuicao do numero de iteracoes para atingir target.

### 8.3 Testes Estatisticos

Para comparar algoritmos de forma rigorosa, testes estatisticos sao obrigatorios.

**Para 2 algoritmos**:
- **Wilcoxon Signed-Rank Test**: Teste nao-parametrico para amostras pareadas. Recomendado sobre t-test por nao assumir normalidade.

**Para k > 2 algoritmos**:
- **Friedman Test**: Teste nao-parametrico para multiplas amostras. Equivalente a ANOVA nao-parametrica.
- **Post-hoc**: Nemenyi test ou Holm correction para comparacoes multiplas.

**Boas Praticas** (Derrac et al., 2011):
1. Minimo 30 execucoes independentes por instancia
2. Reportar media, desvio, melhor, pior, mediana
3. Usar testes nao-parametricos (distribuicoes nao sao normais)
4. Reportar p-values
5. Usar mesmas instancias e orcamento computacional para todos os algoritmos

### 8.4 No Free Lunch Theorem (NFL)

**Wolpert & Macready (1997)**: Para o conjunto de TODOS os problemas de otimizacao possiveis, todos os algoritmos de busca tem a mesma performance media.

**Implicacoes**:
- Nenhum algoritmo e universalmente melhor que outro
- Algoritmos devem ser escolhidos/tunados para a classe de problemas especifica
- Claims de "algoritmo X e melhor que todos" sao fundamentalmente incorretos
- Justifica a existencia de multiplas meta-heuristicas

**Na pratica**: NFL nao impede que algoritmos sejam melhores para CLASSES especificas de problemas. A maioria dos problemas reais tem estrutura exploravel.

**Referencia**: Wolpert, D. H. & Macready, W. G. (1997). "No Free Lunch Theorems for Optimization". IEEE Trans. Evolutionary Computation, 1(1), 67-82.

---

## 9. Referencias Bibliograficas

### Livros de Referencia

1. Talbi, E.-G. (2009). *Metaheuristics: From Design to Implementation*. John Wiley & Sons.
   - Cobertura completa de todas as meta-heuristicas principais com exemplos de implementacao.

2. Gendreau, M. & Potvin, J.-Y. (Eds.) (2010/2019). *Handbook of Metaheuristics* (2nd/3rd ed.). Springer.
   - Referencia definitiva. Capitulos por especialistas em cada meta-heuristica.

3. Luke, S. (2013). *Essentials of Metaheuristics* (2nd ed.). Lulu. [Disponivel gratuitamente online]
   - Introducao acessivel com pseudocodigos claros.

4. Glover, F. & Kochenberger, G. A. (Eds.) (2003). *Handbook of Metaheuristics*. Kluwer Academic.
   - Edicao original do handbook. Capitulos classicos sobre Tabu, GA, SA.

5. De Jong, K. A. (2006). *Evolutionary Computation: A Unified Approach*. MIT Press.
   - Unifica GA, ES, EP, GP sob framework teorico comum.

6. Eiben, A. E. & Smith, J. E. (2015). *Introduction to Evolutionary Computing* (2nd ed.). Springer.
   - Textbook padrao para computacao evolucionaria.

7. Dorigo, M. & Stutzle, T. (2004). *Ant Colony Optimization*. MIT Press.
   - Referencia definitiva para ACO, escrita pelos criadores.

8. Michalewicz, Z. & Fogel, D. B. (2004). *How to Solve It: Modern Heuristics* (2nd ed.). Springer.
   - Abordagem pratica com exemplos e exercicios.

9. Cormen, T. H., Leiserson, C. E., Rivest, R. L. & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
   - Ch. 35: Approximation Algorithms. Fundamentos para heuristicas com garantia.

10. Skiena, S. S. (2020). *The Algorithm Design Manual* (3rd ed.). Springer.
    - Ch. 12: Dealing with Hard Problems. Perspectiva pratica sobre heuristicas.

### Papers Fundamentais

#### Meta-Heuristicas Single-Solution

11. Kirkpatrick, S., Gelatt, C. D. & Vecchi, M. P. (1983). "Optimization by Simulated Annealing". *Science*, 220(4598), 671-680.

12. Cerny, V. (1985). "Thermodynamical approach to the traveling salesman problem: An efficient simulation algorithm". *J. Optimization Theory and Applications*, 45(1), 41-51.

13. Hajek, B. (1988). "Cooling Schedules for Optimal Annealing". *Mathematics of Operations Research*, 13(2), 311-329.

14. Glover, F. (1986). "Future Paths for Integer Programming and Links to Artificial Intelligence". *Computers & OR*, 13(5), 533-549.

15. Glover, F. & Laguna, M. (1997). *Tabu Search*. Kluwer Academic Publishers.

16. Feo, T. A. & Resende, M. G. C. (1995). "Greedy Randomized Adaptive Search Procedures". *J. Global Optimization*, 6(2), 109-133.

17. Lourenco, H. R., Martin, O. C. & Stutzle, T. (2003). "Iterated Local Search". In *Handbook of Metaheuristics*, Ch. 11, pp. 320-353.

18. Mladenovic, N. & Hansen, P. (1997). "Variable Neighborhood Search". *Computers & OR*, 24(11), 1097-1100.

19. Hansen, P. & Mladenovic, N. (2001). "Variable Neighborhood Search: Principles and Applications". *European J. Operational Research*, 130(3), 449-467.

20. Shaw, P. (1998). "Using Constraint Programming and Local Search Methods to Solve Vehicle Routing Problems". *CP-98*, LNCS 1520, pp. 417-431.

21. Ropke, S. & Pisinger, D. (2006). "An Adaptive Large Neighborhood Search Heuristic for the Pickup and Delivery Problem with Time Windows". *Transportation Science*, 40(4), 455-472.

#### Algoritmos Evolucionarios e Populacionais

22. Holland, J. H. (1975). *Adaptation in Natural and Artificial Systems*. University of Michigan Press.

23. Goldberg, D. E. (1989). *Genetic Algorithms in Search, Optimization, and Machine Learning*. Addison-Wesley.

24. Storn, R. & Price, K. (1997). "Differential Evolution - A Simple and Efficient Heuristic for Global Optimization over Continuous Spaces". *J. Global Optimization*, 11(4), 341-359.

25. Kennedy, J. & Eberhart, R. (1995). "Particle Swarm Optimization". *Proc. IEEE Int. Conf. Neural Networks*, pp. 1942-1948.

26. Dorigo, M. (1992). "Optimization, Learning and Natural Algorithms". PhD Thesis, Politecnico di Milano.

27. Rechenberg, I. (1973). *Evolutionsstrategie: Optimierung technischer Systeme nach Prinzipien der biologischen Evolution*. Frommann-Holzboog.

28. Schwefel, H.-P. (1977). *Numerische Optimierung von Computer-Modellen mittels der Evolutionsstrategie*. Birkhauser.

29. Hansen, N. & Ostermeier, A. (2001). "Completely Derandomized Self-Adaptation in Evolution Strategies". *Evolutionary Computation*, 9(2), 159-195. [CMA-ES]

30. Muhlenbein, H. & Paass, G. (1996). "From Recombination of Genes to the Estimation of Distributions". *PPSN IV*, pp. 178-187.

31. Glover, F. (1977). "Heuristics for Integer Programming Using Surrogate Constraints". *Decision Sciences*, 8(1), 156-166. [Scatter Search]

#### Hibridos e Outros

32. Moscato, P. (1989). "On Evolution, Search, Optimization, Genetic Algorithms and Martial Arts: Towards Memetic Algorithms". Tech Report C3P 826, Caltech.

33. Burke, E. K., et al. (2003). "Hyper-heuristics: An Emerging Direction in Modern Search Technology". In *Handbook of Metaheuristics*, pp. 457-474.

34. Wolpert, D. H. & Macready, W. G. (1997). "No Free Lunch Theorems for Optimization". *IEEE Trans. Evolutionary Computation*, 1(1), 67-82.

35. Sorensen, K. (2015). "Metaheuristics - the metaphor exposed". *International Transactions in Operational Research*, 22(1), 3-18.

#### Heuristicas Classicas

36. Lin, S. & Kernighan, B. W. (1973). "An effective heuristic algorithm for the traveling-salesman problem". *Operations Research*, 21(2), 498-516.

37. Christofides, N. (1976). "Worst-Case Analysis of a New Heuristic for the Travelling Salesman Problem". Tech Report 388, CMU.

38. Croes, G. A. (1958). "A Method for Solving Traveling-Salesman Problems". *Operations Research*, 6(6), 791-812.

#### Testes Estatisticos

39. Derrac, J., et al. (2011). "A practical tutorial on the use of nonparametric statistical tests as a methodology for comparing evolutionary and swarm intelligence algorithms". *Swarm and Evolutionary Computation*, 1(1), 3-18.

---

## 10. Roadmap de Implementacao (Fase 3)

Proposta de organizacao para implementacao no projeto C11.

### Phase 3A - Classical (Prioridade Alta)

| # | Algoritmo | Dificuldade | Dependencias | Benchmark |
|---|-----------|------------|-------------|-----------|
| 1 | Hill Climbing | Facil | Nenhuma | TSP, Rastrigin |
| 2 | Simulated Annealing | Media | Nenhuma | TSP, Knapsack |
| 3 | Tabu Search | Media-Alta | Nenhuma | TSP, Graph Coloring |
| 4 | Genetic Algorithm | Media | Nenhuma | TSP, Knapsack |

**Estimativa**: ~2.000 linhas, ~40 testes
**Benchmark comum**: TSP (TSPLIB) + Continuous (Rastrigin, Rosenbrock)

### Phase 3B - Advanced (Prioridade Media)

| # | Algoritmo | Dificuldade | Dependencias | Benchmark |
|---|-----------|------------|-------------|-----------|
| 5 | PSO | Media | Nenhuma | Continuous functions |
| 6 | ACO | Media-Alta | Graph | TSP, VRP |
| 7 | GRASP | Media | Local Search | Scheduling |
| 8 | ILS | Facil | Local Search | TSP |

**Estimativa**: ~2.000 linhas, ~35 testes
**Benchmark adicional**: VRP, Scheduling

### Phase 3C - Specialized (Prioridade Baixa)

| # | Algoritmo | Dificuldade | Dependencias | Benchmark |
|---|-----------|------------|-------------|-----------|
| 9 | Differential Evolution | Media | Nenhuma | Continuous (CEC) |
| 10 | VNS | Media | Multiple neighborhoods | TSP, VRP |
| 11 | Memetic Algorithm | Media | GA + Local Search | TSP |
| 12 | LNS / ALNS | Alta | Destroy/Repair operators | VRP |

**Estimativa**: ~2.000 linhas, ~30 testes

### Arquitetura Proposta

```
include/heuristics/
    common.h           // Tipos: Solution, Objective, Neighborhood, etc.
    local_search.h     // Hill Climbing, 2-opt, first/best improvement
    simulated_annealing.h
    tabu_search.h
    genetic_algorithm.h
    pso.h
    aco.h
    grasp.h
    ils.h
    vns.h
    de.h
    memetic.h
    lns.h

include/benchmarks/
    tsp.h              // TSP: distancias, vizinhanca 2-opt, I/O TSPLIB
    knapsack.h         // Instancias de knapsack
    continuous.h       // Rastrigin, Rosenbrock, Ackley, etc.
    scheduling.h       // Job Shop, Flow Shop

src/heuristics/        // Implementacoes
src/benchmarks/        // Problemas de benchmark
tests/heuristics/      // Testes
tests/benchmarks/      // Testes de benchmark
```

### Ordem de Implementacao Sugerida

1. **common.h + benchmarks**: Definir tipos genericos (Solution, cost function, neighborhood), implementar TSP e funcoes continuas
2. **Hill Climbing + 2-opt**: Base para todas as buscas locais
3. **SA**: Primeiro meta-heuristico, simples de testar
4. **GA**: Primeiro population-based
5. **Tabu Search**: Memory-based, mais complexo
6. **ILS, GRASP**: Variantes de multi-start/perturbacao
7. **PSO, DE**: Population-based para continuo
8. **ACO**: Para TSP/VRP
9. **VNS, Memetic, LNS**: Avancados

---

*Ultima atualizacao: 2026-02-13*
*Documento de pesquisa para Fase 3 - Heuristicas e Meta-Heuristicas*