#include <stdio.h>
#include <math.h>
#include<string.h>
#include "integration.h"
#include "density.h"  // Provided by your teammates

#define PI 3.141592653589793

// Internal integrand for mass calculation
static double mass_integrand(double r, void *params) {
    IntegrationParams *p = (IntegrationParams *)params;
    double rho = 0.0;

    // Select model based on type
    if (strcmp(p->model, "PLUMMER") == 0) {
        rho = plummer_density(r, p->rho0, p->a);
    } else if (strcmp(p->model, "HERNQUIST") == 0) {
        rho = hernquist_density(r, p->M_total, p->a);
    } else {
        fprintf(stderr, "Unknown model type in mass_integrand: %s\n", p->model);
        return 0.0;
    }

    // Mass integrand: 4πr²ρ(r)
    return 4.0 * PI * r * r * rho;
}

// Generic Simpson’s rule implementation
double simpson_integrate(IntegrandFunc f, double a, double b, int n, void *params) {
    if (n % 2 != 0) n++;  // Ensure even number of intervals
    double h = (b - a) / n;
    double sum = f(a, params) + f(b, params);

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        double fx = f(x, params);
        sum += (i % 2 == 0) ? 2 * fx : 4 * fx;
    }

    return (h / 3.0) * sum;
}

// Calculate total mass up to radius r
double calculate_mass(double r, const char *model, double rho0, double a, double M_total) {
    IntegrationParams params;
    params.a = a;
    params.rho0 = rho0;
    params.M_total = M_total;
    params.model = model;

    // Integrate 0 → r for M(r) = ∫ 4πr²ρ(r) dr
    double M_r = simpson_integrate(mass_integrand, 0.0, r, 1000, &params);
    return M_r;
}
