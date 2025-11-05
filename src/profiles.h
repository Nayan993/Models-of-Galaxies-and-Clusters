#ifndef PROFILES_H
#define PROFILES_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Model selection */
typedef enum {
    MODEL_PLUMMER = 0,
    MODEL_HERNQUIST = 1
} profile_model_t;

/* Compute density ρ(r) for given model:
   r: radius (kpc)
   M: total mass (Msun)
   a: scale radius (kpc)
   returns density in Msun / kpc^3
*/
double density_profile(profile_model_t model, double r, double M, double a);

/* Compute mass profile M(r) at n radii.
   r_values: array of radii (kpc) of length n (must be non-decreasing, r_values[0] >= 0)
   mass_values: output array (Msun) length n
   n: number of radii
   nsteps: number of integration steps used for Simpson (per integration). If <=0 a default is used.
*/
int calculate_mass_profile(profile_model_t model,
                           double M, double a,
                           const double *r_values, double *mass_values,
                           int n, int nsteps);

/* Compute circular velocity Vc(r) given enclosed mass array:
   mass_values: array of M(r) in Msun
   r_values: array of radii in kpc
   velocity_values: output array (km/s)
   n: length
*/
void calculate_circular_velocity(const double *mass_values,
                                 const double *r_values,
                                 double *velocity_values, int n);

/* Save profiles to a plain text file with header:
   # r(kpc)  M(Msun)  Vc(km/s)
*/
int save_profiles(const char *filename,
                  const double *r_values, const double *mass_values,
                  const double *velocity_values, int n);

#ifdef __cplusplus
}
#endif

#endif /* PROFILES_H */
