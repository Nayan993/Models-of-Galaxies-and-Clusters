#ifndef PROFILES_H
#define PROFILES_H

// Model type enumeration
typedef enum {
    PLUMMER,
    HERNQUIST
} ModelType;

// Physical constants for unit conversions
#define G_CONST 4.302e-6     // Gravitational constant in (kpc³)/(M☉·Gyr²)
#define KPC_TO_KM 3.086e16   // 1 kpc in km
#define GYR_TO_S 3.154e16    // 1 Gyr in seconds

// Function prototypes

/**
 * Calculate mass profile M(r) for a given model
 * Integrates density to find enclosed mass at each radius
 */
void calculate_mass_profile(ModelType model, double param1, double param2,
                           double *r_values, double *mass_values, int n_points);

/**
 * Calculate circular velocity Vc(r) from mass profile
 * Uses Vc = sqrt(G*M(r)/r)
 */
void calculate_circular_velocity(double *mass_values, double *r_values,
                                double *velocity_values, int n_points);

/**
 * Save profiles to text file in columnar format
 */
void save_profile_data(const char *filename, double *r_values, 
                      double *mass_values, double *velocity_values, int n_points);

/**
 * Main function: computes and saves complete profile data
 * Called from main.c as: save_profiles(model, param1, param2, r_max, filename)
 */
void save_profiles(ModelType model, double param1, double param2, 
                  double r_max, const char *filename);

/**
 * Calculate density at radius r for given model
 * This should be implemented in density.c
 */
double calculate_density(ModelType model, double r, double param1, double param2);

#endif // PROFILES_H