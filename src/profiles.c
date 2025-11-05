#include <stdio.h>
#include <math.h>

// gravitational constant (in kpc * (km/s)^2 / Msun)
#define G_kpc_kms2_Msun 4.30091e-6  

// Function to calculate circular velocity
// calculate_circular_velocity(mass_values, r_values, velocity_values, n);
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