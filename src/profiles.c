#include "profiles.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Gravitational constant G in units: kpc * (km/s)^2 / Msun */
static const double G_kpc_kms2_Msun = 4.30091e-6;

/* Small epsilon to avoid division-by-zero */
static const double SMALL_EPS = 1e-10;

/* -------------------------------------------------------------
   Plummer density profile:
   ρ(r) = (3M) / (4πa^3) * (1 + (r/a)^2)^(-5/2)
   Units: Msun / kpc^3
------------------------------------------------------------- */
static double plummer_density(double r, double M, double a) {
    double x = r / a;
    double denom = pow(1.0 + x * x, 2.5);
    double rho0 = 3.0 * M / (4.0 * M_PI * pow(a, 3));
    return rho0 / denom;
}

/* -------------------------------------------------------------
   Hernquist density profile:
   ρ(r) = (M a) / (2π r (r + a)^3)
   Units: Msun / kpc^3
------------------------------------------------------------- */
static double hernquist_density(double r, double M, double a) {
    double rr = (r < SMALL_EPS) ? SMALL_EPS : r;
    double denom = 2.0 * M_PI * rr * pow(rr + a, 3);
    return (M * a) / denom;
}

/* -------------------------------------------------------------
   Density profile selector
------------------------------------------------------------- */
double density_profile(profile_model_t model, double r, double M, double a) {
    if (r < 0.0) r = 0.0;
    switch (model) {
        case MODEL_PLUMMER:
            return plummer_density(r, M, a);
        case MODEL_HERNQUIST:
            return hernquist_density(r, M, a);
        default:
            return 0.0;
    }
}

/* -------------------------------------------------------------
   Integrand for total enclosed mass:
   f(r) = 4π r^2 ρ(r)
------------------------------------------------------------- */
static double integrand(profile_model_t model, double r, double M, double a) {
    double rho = density_profile(model, r, M, a);
    return 4.0 * M_PI * r * r * rho;
}

/* -------------------------------------------------------------
   Composite Simpson’s rule integration from r=0 to r=r_max
------------------------------------------------------------- */
static double integrate_mass_to_r(profile_model_t model,
                                  double r_max, double M, double a, int nsteps) {
    if (r_max <= 0.0) return 0.0;
    if (nsteps <= 0) nsteps = 1000;
    if (nsteps % 2 == 1) nsteps++;  // Simpson’s rule needs even intervals

    double h = r_max / (double)nsteps;
    double sum = 0.0;

    double f0 = integrand(model, SMALL_EPS, M, a);
    double fn = integrand(model, r_max, M, a);
    sum = f0 + fn;

    for (int i = 1; i < nsteps; ++i) {
        double r_i = h * i;
        double fi = integrand(model, r_i, M, a);
        if (i % 2 == 0)
            sum += 2.0 * fi;
        else
            sum += 4.0 * fi;
    }

    double integral = (h / 3.0) * sum;
    return integral;
}

/* -------------------------------------------------------------
   Compute mass profile M(r)
------------------------------------------------------------- */
int calculate_mass_profile(profile_model_t model,
                           double M_total, double a,
                           const double *r_values, double *mass_values,
                           int n, int nsteps)
{
    if (!r_values || !mass_values || n <= 0) return -1;

    for (int i = 0; i < n; ++i) {
        if (r_values[i] < 0.0) return -2;
        mass_values[i] = integrate_mass_to_r(model, r_values[i], M_total, a, nsteps);
        if (mass_values[i] > M_total) mass_values[i] = M_total;  // cap for numeric overflow
    }
    return 0;
}

/* -------------------------------------------------------------
   Compute circular velocity:
   Vc(r) = sqrt(G * M(r) / r)
------------------------------------------------------------- */
void calculate_circular_velocity(const double *mass_values,
                                 const double *r_values,
                                 double *velocity_values, int n)
{
    if (!mass_values || !r_values || !velocity_values || n <= 0) return;

    for (int i = 0; i < n; ++i) {
        double r = r_values[i];
        double m = mass_values[i];
        if (r <= 0.0 || m <= 0.0) {
            velocity_values[i] = 0.0;
        } else {
            double vc2 = G_kpc_kms2_Msun * m / r;
            velocity_values[i] = (vc2 > 0.0) ? sqrt(vc2) : 0.0;
        }
    }
}

/* -------------------------------------------------------------
   Save profiles to a text file
------------------------------------------------------------- */
int save_profiles(const char *filename,
                  const double *r_values, const double *mass_values,
                  const double *velocity_values, int n)
{
    if (!filename || !r_values || !mass_values || n <= 0) return -1;
    FILE *fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "# r(kpc)\tM(Msun)\t\tVc(km/s)\n");
    for (int i = 0; i < n; ++i) {
        double vc = (velocity_values) ? velocity_values[i] : 0.0;
        fprintf(fp, "% .8e\t% .12e\t% .8e\n",
                r_values[i], mass_values[i], vc);
    }

    fclose(fp);
    return 0;
}
