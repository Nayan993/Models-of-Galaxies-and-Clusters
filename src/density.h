
#ifndef DENSITY_H
#define DENSITY_H
#define PLUMMER 0
#define HERNQUIST 1

#include <math.h>

double plummer_density(double r, double rho0, double a);
double hernquist_density(double r, double M, double a);

#endif
