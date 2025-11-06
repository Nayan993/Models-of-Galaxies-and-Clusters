#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "density.h"

// Number of integration steps (Simpson’s rule)
#define INTEGRATION_STEPS 1000

// Function prototypes
double mass_integrand(double r, ModelType model, double param1, double param2);
double simpson_integrate(double r_min, double r_max, ModelType model,
                         double param1, double param2);

#endif
