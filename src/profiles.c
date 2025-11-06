#include <stdio.h>
#include <math.h>
#include "profiles.h"
#include "integration.h"
#include "density.h"

double mass_profile(double r, ModelType model, double param1, double param2) {
    // Integrate from 0 to r to get total mass within radius r
    return simpson_integrate(0.0, r, model, param1, param2);
}

double circular_velocity(double r, ModelType model, double param1, double param2) {
    // Handle r=0 case (avoid division by zero)
    if (r < 1e-10) {
        return 0.0;
    }
    
    // Calculate mass within radius r
    double M_r = mass_profile(r, model, param1, param2);
    
    // Calculate circular velocity: Vc = √(M/r)
    return sqrt(M_r / r);
}

void save_profiles(ModelType model, double param1, double param2, 
                   double r_max, const char *filename) {
    
    // Open file for writing
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Cannot create file %s\n", filename);
        return;
    }
    
    // Write header
    fprintf(fp, "# r density mass_profile velocity\n");
    
    // Number of sample points
    int steps = 100;
    
    // Calculate and write profiles at each radius
    for (int i = 1; i <= steps; i++) {
        double r = (i * r_max) / steps;
        double density;
        
        // Calculate density at this radius
        if (model == PLUMMER) {
            density = plummer_density(r, param1, param2);
        } else {
            density = hernquist_density(r, param1, param2);
        }
        
        // Calculate mass profile M(r)
        double M_r = mass_profile(r, model, param1, param2);
        
        // Calculate circular velocity Vc(r)
        double V_c = circular_velocity(r, model, param1, param2);
        
        // Write data: r, density, M(r), Vc(r)
        fprintf(fp, "%.6f %.6f %.6f %.6f\n", r, density, M_r, V_c);
    }
    
    // Close file
    fclose(fp);
    printf("Profiles saved to %s\n", filename);
}