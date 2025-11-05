#ifndef DENSITY_H
#define DENSITY_H

#include <math.h>

// ------------------------------------------------------------
// Enum for model types — main.c expects these names
// ------------------------------------------------------------
typedef enum {
    PLUMMER,
    HERNQUIST
} ModelType;

// ------------------------------------------------------------
// Structure for holding model parameters
// ------------------------------------------------------------
typedef struct {
    double rho0;  // Central density (Plummer)
    double M;     // Total mass (Hernquist)
    double a;     // Scale radius
} ModelParams;

// ------------------------------------------------------------
// Function declarations used by other modules
// ------------------------------------------------------------

// Calculate Plummer model density ρ(r)
double plummer_density(double r, ModelParams params);

// Calculate Hernquist model density ρ(r)
double hernquist_density(double r, ModelParams params);

// Compute maximum density (for rejection sampling)
double find_max_density(double (*density_func)(double, ModelParams), ModelParams params);

#endif
