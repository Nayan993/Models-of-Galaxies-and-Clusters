#ifndef UTILS_H
#define UTILS_H

#include "sampling.h"
#include "density.h"

void save_stars(Star *stars, int n_stars, const char *filename);

void create_gnuplot_script(const char *model_name);

void generate_plots(const char *model_name);

void create_comparison_script(void);

#endif // UTILS_H
