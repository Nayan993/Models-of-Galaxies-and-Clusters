#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "density.h"
#include "sampling.h"
#include "profiles.h"
#include "utils.h"

void print_banner(const char *title) {
    printf("\n");
    printf("====================================================\n");
    printf("   %s\n", title);
    printf("====================================================\n\n");
}

int main() {
    // Seed random number generator with current time
    srand(time(NULL));
    
    // Welcome banner
    print_banner("GALAXY CLUSTER SIMULATION");
    printf("Models: Plummer (Globular Clusters) & Hernquist (Galaxy Bulges)\n");
    printf("Developed by: Galaxy Simulation Team\n");
    printf("Team Members: 11\n\n");
    
    // Simulation parameters
    printf("Setting up simulation parameters...\n\n");
    
    int n_stars = 10000;
    printf("Number of stars per model: %d\n", n_stars);
    
    double r_max = 10.0;
    printf("Maximum radius: %.1f\n", r_max);
    
    // Plummer model parameters
    double rho0 = 1.0;
    double a_plummer = 1.0;
    printf("\nPlummer parameters:\n");
    printf("  - Central density rho0: %.1f\n", rho0);
    printf("  - Scale radius a: %.1f\n", a_plummer);
    
    // Hernquist model parameters
    double M_total = 100.0;
    double a_hernquist = 1.0;
    printf("\nHernquist parameters:\n");
    printf("  - Total mass M: %.1f\n", M_total);
    printf("  - Scale radius a: %.1f\n", a_hernquist);
    
    // Allocate memory for stars
    printf("\nAllocating memory for %d stars...\n", n_stars * 2);
    
    Star *plummer_stars = (Star *)malloc(n_stars * sizeof(Star));
    if (!plummer_stars) {
        printf("Error: Memory allocation failed for Plummer stars!\n");
        return 1;
    }
    
    Star *hernquist_stars = (Star *)malloc(n_stars * sizeof(Star));
    if (!hernquist_stars) {
        printf("Error: Memory allocation failed for Hernquist stars!\n");
        free(plummer_stars);
        return 1;
    }
    
    printf("Memory allocated successfully!\n");
    
    // PLUMMER MODEL
    print_banner("PLUMMER MODEL (Globular Cluster)");
    
    generate_stars(plummer_stars, n_stars, PLUMMER, rho0, a_plummer, r_max);
    save_stars(plummer_stars, n_stars, "data/plummer_stars.txt");
    
    printf("Calculating profiles (M(r), Vc(r))...\n");
    save_profiles(PLUMMER, rho0, a_plummer, r_max, "data/plummer_profiles.txt");
    
    printf("Creating gnuplot script...\n");
    create_gnuplot_script("plummer");
    
    printf("Generating plots...\n");
    generate_plots("plummer");
    
    // HERNQUIST MODEL
    print_banner("HERNQUIST MODEL (Galaxy Bulge)");
    
    generate_stars(hernquist_stars, n_stars, HERNQUIST, M_total, a_hernquist, r_max);
    save_stars(hernquist_stars, n_stars, "data/hernquist_stars.txt");
    
    printf("Calculating profiles (M(r), Vc(r))...\n");
    save_profiles(HERNQUIST, M_total, a_hernquist, r_max, "data/hernquist_profiles.txt");
    
    printf("Creating gnuplot script...\n");
    create_gnuplot_script("hernquist");
    
    printf("Generating plots...\n");
    generate_plots("hernquist");
    
    // COMPARISON
    print_banner("GENERATING COMPARISON PLOTS");
    
    printf("Creating comparison script...\n");
    create_comparison_script();
    
    printf("Generating comparison plots...\n");
    int result = system("gnuplot scripts/plot_comparison.gnu");
    if (result == 0) {
        printf("Comparison plot generated: output/comparison.png\n");
    } else {
        printf("Run manually: gnuplot scripts/plot_comparison.gnu\n");
    }
    
    // Cleanup
    printf("\nCleaning up memory...\n");
    free(plummer_stars);
    free(hernquist_stars);
    printf("Memory freed successfully!\n");
    
    // Summary
    print_banner("SIMULATION COMPLETE!");
    
    printf("Generated Files:\n");
    printf("\nData Files:\n");
    printf("  - data/plummer_stars.txt      (Star positions)\n");
    printf("  - data/hernquist_stars.txt    (Star positions)\n");
    printf("  - data/plummer_profiles.txt   (M(r), Vc(r))\n");
    printf("  - data/hernquist_profiles.txt (M(r), Vc(r))\n");
    
    printf("\nVisualization Files:\n");
    printf("  - output/plummer_visualization.png\n");
    printf("  - output/hernquist_visualization.png\n");
    printf("  - output/comparison.png\n");
    
    printf("\nScripts:\n");
    printf("  - scripts/plot_plummer.gnu\n");
    printf("  - scripts/plot_hernquist.gnu\n");
    printf("  - scripts/plot_comparison.gnu\n");
    
    printf("\nAll tasks completed successfully!\n");
    printf("\n====================================================\n\n");
    
    return 0;
}