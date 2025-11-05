#ifndef PROFILES_H
#define PROFILES_H

// ==========================================================
// profiles.h
// ----------------------------------------------------------
// This header file declares functions used to calculate the
// dynamical properties of galaxies and star clusters.
//
// Author: (Your Name)
// Project: Galaxy Models - Plummer & Hernquist
// ==========================================================

// Gravitational constant (in kpc * (km/s)^2 / Msun)
// This constant is used for astrophysical calculations.
#define G_kpc_kms2_Msun 4.30091e-6  

// ----------------------------------------------------------
// Function: calculate_circular_velocity
// Purpose : Given radius values (r_values) and enclosed mass
//           values (mass_values), this function calculates
//           the circular velocity Vc(r) using the formula:
//           Vc = sqrt(G * M(r) / r)
// ----------------------------------------------------------
// Parameters:
//   mass_values[]     -> Array containing the enclosed mass within each radius (in Msun)
//   r_values[]        -> Array containing the radius values (in kiloparsecs, kpc)
//   velocity_values[] -> Output array where the resulting circular velocity (in km/s) will be stored
//   n                 -> Total number of data points
// ----------------------------------------------------------
// Return:
//   This function does not return a value (void).
//   The computed circular velocities are stored in velocity_values[].
// ----------------------------------------------------------
void calculate_circular_velocity(const double *mass_values,
                                 const double *r_values,
                                 double *velocity_values, int n);

#endif  // PROFILES_H
