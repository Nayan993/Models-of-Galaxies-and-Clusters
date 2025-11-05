#ifndef PROFILES_H
#define PROFILES_H

#include "density.h"

// Gravitational constant (in kpc * (km/s)^2 / Msun)
#define G_kpc_kms2_Msun 4.30091e-6  

// Enumeration for model types
typedef enum {
    PLUMMER,
    HERNQUIST
} profile_model_t;

// Function Prototypes

/**
 * @brief Calculates mass profile M(r) for a given model.
 *
 * @param model      Type of model (PLUMMER or HERNQUIST)
 * @param param1     For Plummer → rho0, For Hernquist → M_total
 * @param param2     Scale length (a)
 * @param r_max      Maximum radius for integration
 * @param filename   Output file to save the profile data
 */
void save_profiles(profile_model_t model, double param1, double param2,
                   double r_max, const char *filename);

/**
 * @brief Calculates circular velocity at given radii.
 *
 * @param mass_values     Array of enclosed masses
 * @param r_values        Array of radii
 * @param velocity_values Array to store computed circular velocities
 * @param n               Number of data points
 */
void calculate_circular_velocity(const double *mass_values,
                                 const double *r_values,
                                 double *velocity_values, int n);

#endif // PROFILES_H
