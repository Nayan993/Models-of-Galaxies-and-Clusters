#include "density.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Plummer density profile
// rho(r) = rho0 / (1 + (r/a)^2)^(5/2)
// Used for globular clusters
double plummer_density(double r, double rho0, double a)
{
    return rho0 / pow(1.0 + (r*r)/(a*a), 2.5);
}

// Hernquist density profile  
// rho(r) = (M/(2*pi)) * (a/r) * (1 + r/a)^(-3)
// Used for galaxy bulges
double hernquist_density(double r, double M, double a)
{
    if(r == 0.0) r = 1e-8;  // Avoid division by zero
    return (M / (2.0 * M_PI)) * (a / r) * pow(1.0 + (r / a), -3.0);
}

// Generic density function wrapper
// Calls appropriate model based on ModelType
double density(double r, ModelType model, double param1, double param2)
{
    switch(model) {
        case PLUMMER:
            return plummer_density(r, param1, param2);
        case HERNQUIST:
            return hernquist_density(r, param1, param2);
        default:
            return 0.0;
    }
}