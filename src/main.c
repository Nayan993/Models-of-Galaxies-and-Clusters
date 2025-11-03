#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "density.h"
#include "sampling.h"
#include "profiles.h"
#include "utils.h"

int main() {
    srand(time(NULL));
    
    printf("====================================================\n");
    printf("   GALAXY CLUSTER SIMULATION\n");
    printf("   Models: Plummer & Hernquist\n");
    printf("====================================================\n\n");
    
    // Parameters
    int n_stars = 10000;
    double r_max = 10.0;
    
    // Plummer parameters
    double rho0 = 1.0;
    double a_plummer = 1.0;
    
    // Hernquist parameters
    double M_total = 100.0;
    double a_hernquist = 1.0;
    
    // Allocate memory
    Star *plummer_stars = (Star *)malloc(n_stars * sizeof(Star));
    Star *hernquist_stars = (Star *)malloc(n_stars * sizeof(Star));
    
    if (!plummer_stars || !hernquist_stars) {
        printf("Error: Memory allocation failed!\n");
        return 1;
    }
    
    // PLUMMER MODEL
    printf("\n========== PLUMMER MODEL ==========\n");
    generate_stars(plummer_stars, n_stars, PLUMMER, rho0, a_plummer, r_max);
    save_stars(plummer_stars, n_stars, "data/plummer_stars.txt");
    save_profiles(PLUMMER, rho0, a_plummer, r_max, "data/plummer_profiles.txt");
    create_gnuplot_script("plummer");
    generate_plots("plummer");
    
    // HERNQUIST MODEL
    printf("\n========== HERNQUIST MODEL ==========\n");
    generate_stars(hernquist_stars, n_stars, HERNQUIST, M_total, a_hernquist, r_max);
    save_stars(hernquist_stars, n_stars, "data/hernquist_stars.txt");
    save_profiles(HERNQUIST, M_total, a_hernquist, r_max, "data/hernquist_profiles.txt");
    create_gnuplot_script("hernquist");
    generate_plots("hernquist");
    
    // Cleanup
    free(plummer_stars);
    free(hernquist_stars);
    
    printf("\n====================================================\n");
    printf("   SIMULATION COMPLETE!\n");
    printf("====================================================\n\n");
    
    return 0;
}

/*

## 🔗 How Everything Connects:
```
main.c
  ├── calls → density.h/c (get density values)
  ├── calls → sampling.h/c (generate stars)
  ├── calls → integration.h/c (calculate integrals)
  ├── calls → profiles.h/c (calculate M(r), Vc(r))
  └── calls → utils.h/c (save files, plot)

density.c
  └── provides functions to sampling.c, profiles.c

sampling.c
  ├── uses → density.c (to get density values)
  └── provides Star positions to main.c

integration.c
  ├── uses → density.c (for integrand)
  └── provides integration results to profiles.c

profiles.c
  ├── uses → integration.c (to calculate mass)
  ├── uses → density.c (for density values)
  └── provides M(r), Vc(r) to main.c

utils.c
  └── handles file I/O and plotting

  */