#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <stdio.h>

// Function pointer type for integrands like density(r)
typedef double (*IntegrandFunc)(double r, void *params);

// Structure to pass multiple parameters to integration function
typedef struct {
    double a;        // Scale radius
    double rho0;     // Central density (for Plummer)
    double M_total;  // Total mass (for Hernquist)
    const char *model; // Model name
} IntegrationParams;

// Simpson’s rule integration
double simpson_integrate(IntegrandFunc f, double a, double b, int n, void *params);

// Calculate mass profile M(r) using density function
double calculate_mass(double r, const char *model, double rho0, double a, double M_total);

#endif