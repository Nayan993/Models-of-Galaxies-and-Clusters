#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sampling.h"
#include "density.h"

#define PI 3.14159265358979323846

double rejection_sample_radius(ModelType model, double param1, double param2, 
                                double r_max, double max_density) {
    double r, height, density_at_r;
    
    do {
        // Generate random radius in [0, r_max]
        r = ((double)rand() / RAND_MAX) * r_max;
        
        // Generate random height in [0, max_density]
        height = ((double)rand() / RAND_MAX) * max_density;
        
        // Calculate actual density at radius r
        if (model == PLUMMER) {
            density_at_r = plummer_density(r, param1, param2);
        } else {
            density_at_r = hernquist_density(r, param1, param2);
        }
        
        // Accept if height <= density, else reject and retry
    } while (height > density_at_r);
    
    return r;
}

Star spherical_to_cartesian(double r) {
    Star s;
    
    // Random azimuthal angle [0, 2π]
    double theta = 2.0 * PI * ((double)rand() / RAND_MAX);
    
    // Random polar angle with uniform sphere distribution
    double phi = acos(2.0 * ((double)rand() / RAND_MAX) - 1.0);
    
    // Convert to Cartesian coordinates
    s.x = r * sin(phi) * cos(theta);
    s.y = r * sin(phi) * sin(theta);
    s.z = r * cos(phi);
    
    return s;
}

void generate_stars(Star *stars, int n_stars, ModelType model, 
                    double param1, double param2, double r_max) {
    
    printf("Generating %d stars for %s model...\n", n_stars, 
           model == PLUMMER ? "Plummer" : "Hernquist");
    
    // Find maximum density for rejection sampling
    double max_density = find_max_density(model, param1, param2, r_max);
    
    // Generate each star
    for (int i = 0; i < n_stars; i++) {
        // Generate radius using rejection sampling
        double r = rejection_sample_radius(model, param1, param2, r_max, max_density);
        
        // Convert to (x,y,z) coordinates
        stars[i] = spherical_to_cartesian(r);
        
        // Progress indicator every 10%
        if ((i + 1) % (n_stars / 10) == 0) {
            printf("Progress: %d%%\n", (i + 1) * 100 / n_stars);
        }
    }
    
    printf("Star generation complete!\n\n");
}
