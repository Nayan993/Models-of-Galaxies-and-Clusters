#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "sampling.h" 

/**
 * @brief Save star coordinates to a CSV file.
 *
 * @param stars Pointer to array of Star structures.
 * @param N Number of stars to write.
 * @param filename Output CSV file path (e.g., "data/plummer_stars.csv").
 * @return 0 on success, -1 on failure
 */
int save_stars(
    const Star *stars, 
    int N, 
    const char *filename
);


/**
 * @brief Generate a 3D Gnuplot script for visualizing a model's stars.
 * Produces a script to plot x, y, z coordinates in 3D using points.
 *
 * @param model Model name (e.g., "Plummer" or "Hernquist").
 * @param scriptfile Output path for the generated Gnuplot script. (e.g., "scripts/plot_plummer.gnu")
 * @param datafile Path to CSV data file containing star positions. (e.g., "data/plummer_stars.csv")
 * @param outputfile Path to save the rendered PNG output. (e.g., "output/plummer_visualization.png")
 */
int create_gnuplot_script(
    const char *model,
    const char *scriptfile,
    const char *datafile,
    const char *outputfile
);

/**
 * @brief Execute a Gnuplot script file using the system shell.
 *
 * @param scriptfile Path to the .gnu script file.
 */
int execute_gnuplot(const char *scriptfile);

/**
 * @brief Generate a side-by-side 2D comparison Gnuplot script.
 * Creates a multiplot (1x2 layout) comparing two models on XY projection.
 * Example: scripts/plot_comparison.gnu
 * 
 * @param scriptfile Output path for generated Gnuplot script. (e.g., "scripts/plot_comparison.gnu")
 * @param datafile1 CSV path for Plummer Model data. (e.g., "data/plummer_stars.csv")
 * @param datafile2 CSV path for Hernquist Model data. (e.g., "data/hernquist_stars.csv")
 * @param model1_name Display name for Model 1 (left plot title -> "Plummer").
 * @param model2_name Display name for Model 2 (right plot title -> "Hernquist").
 * @param outputfile Path for PNG output (comparison image). (e.g.,  "output/comparison.png")
 */
int create_comparison_script(
    const char *scriptfile,
    const char *datafile1,
    const char *datafile2,
    const char *model1_name,
    const char *model2_name,
    const char *outputfile
);

void ensure_dir_exists(const char *path);

#endif
