#include <math.h>
#include <stdio.h>
#include "density.h"

#define PI 3.14159265358979323846

double plummer_density(double r, double rho0, double a) {
    // Calculate (r/a) ratio
    double ratio = r / a;
    
    // Calculate denominator: (1 + (r/a)²)^(5/2)
    double denominator = pow(1.0 + ratio * ratio, 2.5);
    
    // Return density: ρ₀ / denominator
    return rho0 / denominator;
}

double hernquist_density(double r, double M, double a) {
    // Avoid division by zero at r=0
    if (r < 1e-10) {
        return 0.0;
    }
    
    // Calculate ratio r/a
    double ratio = r / a;
    
    // Calculate: (M/2π)
    double mass_factor = M / (2.0 * PI);
    
    // Calculate: (a/r)
    double scale_factor = a / r;
    
    // Calculate: 1/(1+r/a)³
    double falloff = 1.0 / pow(1.0 + ratio, 3.0);
    
    // Combine all factors
    return mass_factor * scale_factor * falloff;
}

double find_max_density(ModelType model, double param1, double param2, double r_max) {
    double max_dens = 0.0;
    int steps = 1000;
    
    // Scan from r=0 to r=r_max to find maximum density
    for (int i = 0; i <= steps; i++) {
        double r = (i * r_max) / steps;
        double dens;
        
        // Calculate density based on model type
        if (model == PLUMMER) {
            dens = plummer_density(r, param1, param2);
        } else {
            dens = hernquist_density(r, param1, param2);
        }
        
        // Update maximum if current is larger
        if (dens > max_dens) {
            max_dens = dens;
        }
    }
    
    // Return max density with 10% safety margin
    return max_dens * 1.1;
}