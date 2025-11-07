#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void save_stars(Star *stars, int n_stars, const char *filename) {
    // Open file for writing
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Cannot create file %s\n", filename);
        return;
    }
    
    // Write header line
    fprintf(fp, "# x y z\n");
    
    // Write each star's position
    for (int i = 0; i < n_stars; i++) {
        fprintf(fp, "%.6f %.6f %.6f\n", 
                stars[i].x, stars[i].y, stars[i].z);
    }
    
    // Close file
    fclose(fp);
    printf("Stars saved to %s\n", filename);
}

void create_gnuplot_script(const char *model_name) {
    // Create script filename
    char filename[100];
    sprintf(filename, "scripts/plot_%s.gnu", model_name);
    
    // Open script file for writing
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Warning: Cannot create gnuplot script %s\n", filename);
        return;
    }
    
    // Set output format and file
    fprintf(fp, "# Gnuplot script for %s model\n", model_name);
    fprintf(fp, "set terminal png size 1200,800 enhanced font 'Arial,12'\n");
    fprintf(fp, "set output 'output/%s_visualization.png'\n\n", model_name);
    
    // Create 2x2 grid of plots
    fprintf(fp, "set multiplot layout 2,2 title '%s Model Visualization'\n\n", 
            model_name);
    
    // Plot 1: XY Projection
    fprintf(fp, "# Plot 1: XY Projection (top view)\n");
    fprintf(fp, "set title 'XY Projection (Top View)'\n");
    fprintf(fp, "set xlabel 'X'\n");
    fprintf(fp, "set ylabel 'Y'\n");
    fprintf(fp, "set size square\n");
    fprintf(fp, "plot 'data/%s_stars.txt' using 1:2 with points pt 7 ps 0.3 lc rgb 'blue' notitle\n\n", 
            model_name);
    
    // Plot 2: XZ Projection
    fprintf(fp, "# Plot 2: XZ Projection (side view)\n");
    fprintf(fp, "set title 'XZ Projection (Side View)'\n");
    fprintf(fp, "set xlabel 'X'\n");
    fprintf(fp, "set ylabel 'Z'\n");
    fprintf(fp, "set size square\n");
    fprintf(fp, "plot 'data/%s_stars.txt' using 1:3 with points pt 7 ps 0.3 lc rgb 'red' notitle\n\n", 
            model_name);
    
    // Plot 3: Density and Mass Profile
    fprintf(fp, "# Plot 3: Density and Mass Profiles\n");
    fprintf(fp, "set title 'Density and Mass Profile'\n");
    fprintf(fp, "set xlabel 'Radius r'\n");
    fprintf(fp, "set ylabel 'Density / Mass'\n");
    fprintf(fp, "unset size\n");
    fprintf(fp, "plot 'data/%s_profiles.txt' using 1:2 with lines lw 2 lc rgb 'green' title 'Density', \\\n", 
            model_name);
    fprintf(fp, "     'data/%s_profiles.txt' using 1:3 with lines lw 2 lc rgb 'orange' title 'Mass M(r)'\n\n", 
            model_name);
    
    // Plot 4: Circular Velocity
    fprintf(fp, "# Plot 4: Circular Velocity Curve\n");
    fprintf(fp, "set title 'Circular Velocity Curve'\n");
    fprintf(fp, "set xlabel 'Radius r'\n");
    fprintf(fp, "set ylabel 'Velocity Vc(r)'\n");
    fprintf(fp, "plot 'data/%s_profiles.txt' using 1:4 with lines lw 2 lc rgb 'purple' notitle\n\n", 
            model_name);
    
    // End multiplot
    fprintf(fp, "unset multiplot\n");
    
    // Close file
    fclose(fp);
    printf("Gnuplot script created: %s\n", filename);
}

void generate_plots(const char *model_name) {
    // Construct gnuplot command
    char command[200];
    sprintf(command, "gnuplot scripts/plot_%s.gnu", model_name);
    
    // Execute command
    printf("Executing: %s\n", command);
    int result = system(command);
    
    // Check result
    if (result == 0) {
        printf("Plots generated successfully: output/%s_visualization.png\n\n", 
               model_name);
    } else {
        printf("Note: Install gnuplot to generate plots automatically.\n");
        printf("Or run manually: gnuplot scripts/plot_%s.gnu\n\n", model_name);
    }
}

void create_comparison_script(void) {
    // Open file for writing
    FILE *fp = fopen("scripts/plot_comparison.gnu", "w");
    if (!fp) {
        printf("Warning: Cannot create comparison script\n");
        return;
    }
    
    // Header
    fprintf(fp, "# Comparison plot: Plummer vs Hernquist\n");
    fprintf(fp, "set terminal png size 1600,500 enhanced font 'Arial,12'\n");
    fprintf(fp, "set output 'output/comparison.png'\n\n");
    
    // Create 1x3 grid
    fprintf(fp, "set multiplot layout 1,3 title 'Plummer vs Hernquist Comparison'\n\n");
    
    // Plot 1: Density Comparison
    fprintf(fp, "# Density Profile Comparison\n");
    fprintf(fp, "set title 'Density Profile'\n");
    fprintf(fp, "set xlabel 'Radius r'\n");
    fprintf(fp, "set ylabel 'Density'\n");
    fprintf(fp, "set logscale y\n");
    fprintf(fp, "plot 'data/plummer_profiles.txt' using 1:2 with lines lw 2 lc rgb 'blue' title 'Plummer', \\\n");
    fprintf(fp, "     'data/hernquist_profiles.txt' using 1:2 with lines lw 2 lc rgb 'red' title 'Hernquist'\n\n");
    
    // Plot 2: Mass Profile Comparison
    fprintf(fp, "# Mass Profile Comparison\n");
    fprintf(fp, "set title 'Mass Profile'\n");
    fprintf(fp, "set xlabel 'Radius r'\n");
    fprintf(fp, "set ylabel 'Mass M(r)'\n");
    fprintf(fp, "unset logscale y\n");
    fprintf(fp, "plot 'data/plummer_profiles.txt' using 1:3 with lines lw 2 lc rgb 'blue' title 'Plummer', \\\n");
    fprintf(fp, "     'data/hernquist_profiles.txt' using 1:3 with lines lw 2 lc rgb 'red' title 'Hernquist'\n\n");
    
    // Plot 3: Velocity Curve Comparison
    fprintf(fp, "# Velocity Curve Comparison\n");
    fprintf(fp, "set title 'Circular Velocity'\n");
    fprintf(fp, "set xlabel 'Radius r'\n");
    fprintf(fp, "set ylabel 'Velocity Vc(r)'\n");
    fprintf(fp, "plot 'data/plummer_profiles.txt' using 1:4 with lines lw 2 lc rgb 'blue' title 'Plummer', \\\n");
    fprintf(fp, "     'data/hernquist_profiles.txt' using 1:4 with lines lw 2 lc rgb 'red' title 'Hernquist'\n\n");
    
    // End multiplot
    fprintf(fp, "unset multiplot\n");
    
    // Close file
    fclose(fp);
    printf("Comparison script created: scripts/plot_comparison.gnu\n");
}
