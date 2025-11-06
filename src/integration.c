#include <math.h>
#include "integration.h"
#include "density.h"

#define PI 3.14159265358979323846

double mass_integrand(double r, ModelType model, double param1, double param2) {
    double density;
    
    // Get density at radius r
    if (model == PLUMMER) {
        density = plummer_density(r, param1, param2);
    } else {
        density = hernquist_density(r, param1, param2);
    }
    
    // Return: ρ(r) × 4πr²
    return 4.0 * PI * r * r * density;
}

double simpson_integrate(double r_min, double r_max, ModelType model, 
                         double param1, double param2) {
    
    // Number of intervals (must be even for Simpson's rule)
    int n = INTEGRATION_STEPS;
    if (n % 2 == 1) {
        n++;
    }
    
    // Step size
    double h = (r_max - r_min) / n;
    
    // Initialize sum with first and last terms (coefficient 1)
    double sum = mass_integrand(r_min, model, param1, param2) + 
                 mass_integrand(r_max, model, param1, param2);
    
    // Add odd-indexed terms with coefficient 4
    for (int i = 1; i < n; i += 2) {
        double r = r_min + i * h;
        sum += 4.0 * mass_integrand(r, model, param1, param2);
    }
    
    // Add even-indexed terms with coefficient 2
    for (int i = 2; i < n; i += 2) {
        double r = r_min + i * h;
        sum += 2.0 * mass_integrand(r, model, param1, param2);
    }
    
    // Multiply by h/3 to get final result
    return (h / 3.0) * sum;
}
