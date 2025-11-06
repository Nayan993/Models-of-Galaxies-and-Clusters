#ifndef DENSITY_H
#define DENSITY_H

typedef enum {
    PLUMMER,
    HERNQUIST
} ModelType;

double plummer_density(double r, double rho0, double a);

double hernquist_density(double r, double M, double a);

double find_max_density(ModelType model, double param1, double param2, double r_max);

#endif // DENSITY_H