#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sampling.h"
#include "density.h"

#define PI 3.14159265358979323846

// Original code se ye functions copy karo:
// - rejection_sample_radius()
// - spherical_to_cartesian()
// - generate_stars()

double rejection_sample_radius(ModelType model, double param1, double param2, 
                                double r_max, double max_density) {
    double r, height, density_at_r;
    
    do {
        r = ((double)rand() / RAND_MAX) * r_max;
        height = ((double)rand() / RAND_MAX) * max_density;
        
        if (model == PLUMMER) {
            density_at_r = plummer_density(r, param1, param2);
        } else {
            density_at_r = hernquist_density(r, param1, param2);
        }
        
    } while (height > density_at_r);
    
    return r;
}

Star spherical_to_cartesian(double r) {
    Star s;
    double theta = 2.0 * PI * ((double)rand() / RAND_MAX);
    double phi = acos(2.0 * ((double)rand() / RAND_MAX) - 1.0);
    
    s.x = r * sin(phi) * cos(theta);
    s.y = r * sin(phi) * sin(theta);
    s.z = r * cos(phi);
    
    return s;
}

void generate_stars(Star *stars, int n_stars, ModelType model, 
                    double param1, double param2, double r_max) {
    printf("Generating %d stars for %s model...\n", n_stars, 
           model == PLUMMER ? "Plummer" : "Hernquist");
    
    double max_density = find_max_density(model, param1, param2, r_max);
    
    for (int i = 0; i < n_stars; i++) {
        double r = rejection_sample_radius(model, param1, param2, r_max, max_density);
        stars[i] = spherical_to_cartesian(r);
        
        if ((i + 1) % (n_stars / 10) == 0) {
            printf("Progress: %d%%\n", (i + 1) * 100 / n_stars);
        }
    }
    
    printf("Star generation complete!\n\n");
}