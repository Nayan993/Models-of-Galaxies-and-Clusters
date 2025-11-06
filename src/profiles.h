#ifndef PROFILES_H
#define PROFILES_H

#include "density.h"

double mass_profile(double r, ModelType model, double param1, double param2);

double circular_velocity(double r, ModelType model, double param1, double param2);

void save_profiles(ModelType model, double param1, double param2, 
                   double r_max, const char *filename);
#endif // PROFILES_H