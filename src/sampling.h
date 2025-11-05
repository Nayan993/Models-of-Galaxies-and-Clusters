#ifndef SAMPLING_H
#define SAMPLING_H

#include "density.h"

typedef struct {
    double x;
    double y;
    double z;
} Star;

double rejection_sample_radius(ModelType model, double param1, double param2, 
                                double r_max, double max_density);

Star spherical_to_cartesian(double r);

void generate_stars(Star *stars, int n_stars, ModelType model, 
                    double param1, double param2, double r_max);

#endif // SAMPLING_H
