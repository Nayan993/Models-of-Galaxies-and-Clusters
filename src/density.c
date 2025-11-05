
#include "density.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


double plummer_density(double r, double rho0, double a)
{
    return rho0 / pow(1.0 + (r*r)/(a*a), 2.5);
}

double hernquist_density(double r, double M, double a)
{
    return (M / (2.0 * M_PI)) * (a / r) * pow(1.0 + (r / a), -3.0);
}


