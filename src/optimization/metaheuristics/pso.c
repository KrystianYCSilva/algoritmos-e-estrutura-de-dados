/**
 * @file pso.c
 * @brief Implementacao do Particle Swarm Optimization (PSO)
 *
 * PSO classico com inercia constante, linear decreasing ou
 * constriction factor. Velocity clamping e position clamping.
 *
 * Referencias:
 * - Kennedy, J. & Eberhart, R. (1995). "Particle Swarm Optimization".
 * - Clerc, M. & Kennedy, J. (2002). "The Particle Swarm".
 * - Shi, Y. & Eberhart, R. (1998). "A Modified Particle Swarm Optimizer".
 *
 * @author Algoritmos e Heuristicas
 * @date 2025
 */

#include "optimization/metaheuristics/pso.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ============================================================================
// HELPERS
// ============================================================================

static bool pso_is_better(double a, double b, OptDirection dir) {
    return (dir == OPT_MINIMIZE) ? (a < b) : (a > b);
}

static double clamp(double val, double lo, double hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

// ============================================================================
// CONFIGURACAO
// ============================================================================

PSOConfig pso_default_config(void) {
    PSOConfig config;
    config.num_particles = 30;
    config.max_iterations = 500;
    config.w = 0.729;
    config.w_min = 0.4;
    config.c1 = 1.49445;
    config.c2 = 1.49445;
    config.v_max_ratio = 0.1;
    config.inertia_type = PSO_INERTIA_LINEAR_DECREASING;
    config.lower_bound = -5.12;
    config.upper_bound = 5.12;
    config.direction = OPT_MINIMIZE;
    config.seed = 42;
    return config;
}

// ============================================================================
// PSO PRINCIPAL
// ============================================================================

OptResult pso_run(const PSOConfig *config,
                  size_t solution_size,
                  ObjectiveFn objective,
                  const void *context) {
    OptResult result = opt_result_create(config->max_iterations);
    opt_set_seed(config->seed);

    size_t D = solution_size;
    size_t N = config->num_particles;
    size_t element_size = D * sizeof(double);
    double range = config->upper_bound - config->lower_bound;
    double v_max = config->v_max_ratio * range;

    double *positions = malloc(N * element_size);
    double *velocities = calloc(N * D, sizeof(double));
    double *pbest_pos = malloc(N * element_size);
    double *pbest_cost = malloc(N * sizeof(double));
    double *gbest_pos = malloc(element_size);

    if (positions == NULL || velocities == NULL || pbest_pos == NULL ||
        pbest_cost == NULL || gbest_pos == NULL) {
        free(positions);
        free(velocities);
        free(pbest_pos);
        free(pbest_cost);
        free(gbest_pos);
        return result;
    }

    double gbest_cost = (config->direction == OPT_MINIMIZE) ? DBL_MAX : -DBL_MAX;

    for (size_t i = 0; i < N; i++) {
        double *xi = positions + i * D;
        double *vi = velocities + i * D;
        for (size_t d = 0; d < D; d++) {
            xi[d] = config->lower_bound + opt_random_uniform() * range;
            vi[d] = -v_max + opt_random_uniform() * 2.0 * v_max;
        }

        double cost = objective(xi, D, context);
        result.num_evaluations++;

        memcpy(pbest_pos + i * D, xi, element_size);
        pbest_cost[i] = cost;

        if (pso_is_better(cost, gbest_cost, config->direction)) {
            memcpy(gbest_pos, xi, element_size);
            gbest_cost = cost;
        }
    }

    result.best = opt_solution_create(element_size);
    memcpy(result.best.data, gbest_pos, element_size);
    result.best.cost = gbest_cost;

    double w_max = config->w;
    double w_min = config->w_min;
    double chi = 1.0;
    if (config->inertia_type == PSO_INERTIA_CONSTRICTION) {
        double phi = config->c1 + config->c2;
        if (phi > 4.0) {
            chi = 2.0 / fabs(2.0 - phi - sqrt(phi * phi - 4.0 * phi));
        }
    }

    for (size_t iter = 0; iter < config->max_iterations; iter++) {
        double w;
        switch (config->inertia_type) {
            case PSO_INERTIA_CONSTANT:
                w = config->w;
                break;
            case PSO_INERTIA_LINEAR_DECREASING:
                w = w_max - (w_max - w_min) * ((double)iter / (double)config->max_iterations);
                break;
            case PSO_INERTIA_CONSTRICTION:
                w = chi;
                break;
            default:
                w = config->w;
                break;
        }

        for (size_t i = 0; i < N; i++) {
            double *xi = positions + i * D;
            double *vi = velocities + i * D;
            double *pi = pbest_pos + i * D;

            for (size_t d = 0; d < D; d++) {
                double r1 = opt_random_uniform();
                double r2 = opt_random_uniform();

                vi[d] = w * vi[d]
                      + config->c1 * r1 * (pi[d] - xi[d])
                      + config->c2 * r2 * (gbest_pos[d] - xi[d]);

                vi[d] = clamp(vi[d], -v_max, v_max);

                xi[d] = xi[d] + vi[d];
                xi[d] = clamp(xi[d], config->lower_bound, config->upper_bound);
            }

            double cost = objective(xi, D, context);
            result.num_evaluations++;

            if (pso_is_better(cost, pbest_cost[i], config->direction)) {
                memcpy(pi, xi, element_size);
                pbest_cost[i] = cost;

                if (pso_is_better(cost, gbest_cost, config->direction)) {
                    memcpy(gbest_pos, xi, element_size);
                    gbest_cost = cost;
                    memcpy(result.best.data, xi, element_size);
                    result.best.cost = cost;
                }
            }
        }

        if (result.convergence != NULL && iter < result.convergence_size) {
            result.convergence[iter] = result.best.cost;
        }
        result.num_iterations = iter + 1;
    }

    free(positions);
    free(velocities);
    free(pbest_pos);
    free(pbest_cost);
    free(gbest_pos);
    return result;
}
