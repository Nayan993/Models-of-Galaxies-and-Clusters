#ifndef DENSITY_H
#define DENSITY_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Model identifiers
typedef enum {
    PLUMMER = 0,
    HERNQUIST = 1
} ModelType;

// Star structure for 3D positions
typedef struct {
    double x;
    double y;
    double z;
    double r;  // radial distance from origin
} Star;

// Density function prototypes
double plummer_density(double r, double rho0, double a);
double hernquist_density(double r, double M, double a);

// Generic density function wrapper
double density(double r, ModelType model, double param1, double param2);

#endif // DENSITY_H