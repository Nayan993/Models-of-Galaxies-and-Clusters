#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "profiles.h"
#include "density.h"

#define M_PI 3.14159265358979323846

// Gravitational constant
#ifndef G_kpc_kms2_Msun
#define G_kpc_kms2_Msun 4.30091e-6
#endif

// -----------------------------------------------------------------------------
// 1️⃣ Helper function: numerical integration for total mass M(r)
// -----------------------------------------------------------------------------
static double integrate_mass(profile_model_t model, double param1, double param2, double r) {
    int steps = 1000;
    if (r <= 0.0) return 0.0;

    double dr = r / steps;
    double mass = 0.0;

    for (int i = 0; i < steps; ++i) {
        double r1 = i * dr;
        double r2 = (i + 1) * dr;
        double mid = 0.5 * (r1 + r2);

        double rho_mid = (model == PLUMMER)
            ? plummer_density(mid, param1, param2)
            : hernquist_density(mid, param1, param2);

        // dM = 4πr²ρ(r)dr
        mass += 4.0 * M_PI * mid * mid * rho_mid * dr;
    }

    return mass;
}

// -----------------------------------------------------------------------------
// 2️⃣ Helper function: compute circular velocity
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// 3️⃣ Helper function: safely write data to a file
// -----------------------------------------------------------------------------
int write_profile_to_file(const char *filename,
                          const double *r_values,
                          const double *mass_values,
                          const double *velocity_values,
                          int n)
{
    if (!filename || !r_values || !mass_values || n <= 0) return -1;

    FILE *fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "# r(kpc)\tM(Msun)\tVc(km/s)\n");
    for (int i = 0; i < n; ++i) {
        double vc = (velocity_values) ? velocity_values[i] : 0.0;
        fprintf(fp, "% .8e\t% .12e\t% .8e\n",
                r_values[i], mass_values[i], vc);
    }

    fclose(fp);
    return 0;
}

// -----------------------------------------------------------------------------
// 4️⃣ Main function: compute and save profiles (r, M(r), Vc(r))
// -----------------------------------------------------------------------------
void save_profiles(profile_model_t model, double param1, double param2,
                   double r_max, const char *filename)
{
    int n_points = 200;
    if (r_max <= 0.0 || filename == NULL) {
        printf("Error: Invalid arguments to save_profiles().\n");
        return;
    }

    double dr = r_max / n_points;

    double *r_values = (double *)malloc(n_points * sizeof(double));
    double *mass_values = (double *)malloc(n_points * sizeof(double));
    double *vc_values = (double *)malloc(n_points * sizeof(double));

    if (!r_values || !mass_values || !vc_values) {
        printf("Error: Memory allocation failed in save_profiles().\n");
        free(r_values); free(mass_values); free(vc_values);
        return;
    }

    // Compute M(r)
    for (int i = 0; i < n_points; ++i) {
        double r = (i + 1) * dr;
        r_values[i] = r;
        mass_values[i] = integrate_mass(model, param1, param2, r);
    }

    // Compute Vc(r)
    calculate_circular_velocity(mass_values, r_values, vc_values, n_points);

    // Save all data safely
    int status = write_profile_to_file(filename, r_values, mass_values, vc_values, n_points);
    if (status == 0)
        printf("✅ Profile saved to %s\n", filename);
    else
        printf("❌ Error saving profile (code %d)\n", status);

    free(r_values);
    free(mass_values);
    free(vc_values);
}
