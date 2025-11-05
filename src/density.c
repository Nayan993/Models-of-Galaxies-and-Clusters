#include <stdio.h>
#include <math.h>
#include "density.h"

// Define PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ------------------------------------------------------------
// Plummer Model:
// ρ(r) = ρ0 / [ (1 + (r/a)^2)^(5/2) ]
// ------------------------------------------------------------
double plummer_density(double r, ModelParams params) {
    double x = r / params.a;
    return params.rho0 / pow(1.0 + x * x, 2.5);
}

// ------------------------------------------------------------
// Hernquist Model:
// ρ(r) = (M / (2π)) * (a / r) * (1 / (1 + r/a)^3)
// ------------------------------------------------------------
double hernquist_density(double r, ModelParams params) {
    if (r == 0.0) r = 1e-6; // avoid division by zero
    double a = params.a;
    double M = params.M;
    return (M / (2.0 * M_PI)) * (a / r) * pow(1.0 + (r / a), -3.0);
}

// ------------------------------------------------------------
// Finds approximate maximum density for sampling algorithm
// ------------------------------------------------------------
double find_max_density(double (*density_func)(double, ModelParams), ModelParams params) {
    double max_rho = 0.0;
    for (double r = 0.0; r <= 10.0 * params.a; r += 0.01 * params.a) {
        double rho = density_func(r, params);
        if (rho > max_rho)
            max_rho = rho;
    }
    return max_rho;
}
