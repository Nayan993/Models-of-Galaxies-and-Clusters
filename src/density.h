// src/density.h
#ifndef DENSITY_H
#define DENSITY_H

// Structure to store model parameters
typedef struct {
    double M;   // total mass
    double a;   // scale radius
    double rho0; // central density (for Plummer)
} ModelParams;

// Function declarations
double plummer_density(double r, ModelParams params);
double hernquist_density(double r, ModelParams params);
double find_max_density(double (*density_func)(double, ModelParams), ModelParams params);

#endif
