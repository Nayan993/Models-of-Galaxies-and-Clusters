#include <stdio.h>
#include <math.h>
#include "integration.h"
#include "density.h"

// 4πr²ρ(r)
double mass_integrand(double r, ModelType model, double param1, double param2) {
    double rho = 0.0;

    if (model == PLUMMER) {
        rho = plummer_density(r, param1, param2);   // rho0, a
    } else if (model == HERNQUIST) {
        rho = hernquist_density(r, param1, param2); // M_total, a
    } else {
        fprintf(stderr, "Error: Unknown model type!\n");
        return 0.0;
    }

    return 4.0 * M_PI * r * r * rho;
}

// Simpson’s rule integration
double simpson_integrate(double r_min, double r_max,
                         ModelType model, double param1, double param2) {
    int n = INTEGRATION_STEPS;
    if (n % 2 != 0) n++;
    double h = (r_max - r_min) / n;

    double sum = mass_integrand(r_min, model, param1, param2)
               + mass_integrand(r_max, model, param1, param2);

    for (int i = 1; i < n; i++) {
        double r = r_min + i * h;
        double weight = (i % 2 == 0) ? 2.0 : 4.0;
        sum += weight * mass_integrand(r, model, param1, param2);
    }

    return (h / 3.0) * sum;
}
