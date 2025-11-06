#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#include <profiles.h>

// Forward declaration of integration function
// This should be implemented in integration.c
double simpson_integrate(double (*func)(double, void*), double a, double b, int n, void *params);

// Structure to pass parameters to integration function
typedef struct {
    ModelType model;
    double param1;
    double param2;
} IntegrationParams;

// Integrand for mass calculation: ρ(r) * 4π * r²
static double mass_integrand(double r, void *params) {
    IntegrationParams *p = (IntegrationParams *)params;
    double rho = calculate_density(p->model, r, p->param1, p->param2);
    return 4.0 * M_PI * r * r * rho;
}

void calculate_mass_profile(ModelType model, double param1, double param2,
                           double *r_values, double *mass_values, int n_points) {
    IntegrationParams params;
    params.model = model;
    params.param1 = param1;
    params.param2 = param2;
    
    printf("Calculating mass profile...\n");
    
    for (int i = 0; i < n_points; i++) {
        double r = r_values[i];
        
        if (r < 1e-10) {
            mass_values[i] = 0.0;
            continue;
        }
        
        // M(r) = ∫₀ʳ ρ(r') * 4π * r'² dr'
        int n_steps = 500;
        mass_values[i] = simpson_integrate(mass_integrand, 0.0, r, n_steps, &params);
        
        // Progress indicator
        if (n_points >= 10 && (i + 1) % (n_points / 10) == 0) {
            printf("  Progress: %d/%d (%.0f%%)\n", 
                   i + 1, n_points, 100.0 * (i + 1) / n_points);
        }
    }
    
    printf("Mass profile complete.\n");
}

void calculate_circular_velocity(double *mass_values, double *r_values,
                                double *velocity_values, int n_points) {
    printf("Calculating circular velocity...\n");
    
    for (int i = 0; i < n_points; i++) {
        double r = r_values[i];
        double M = mass_values[i];
        
        if (r < 1e-10) {
            velocity_values[i] = 0.0;
            continue;
        }
        
        // Vc(r) = sqrt(G * M(r) / r)
        double Vc_kpc_gyr = sqrt(G_CONST * M / r);
        
        // Convert to km/s
        velocity_values[i] = Vc_kpc_gyr * (KPC_TO_KM / GYR_TO_S);
        
        // Progress indicator
        if (n_points >= 10 && (i + 1) % (n_points / 10) == 0) {
            printf("  Progress: %d/%d (%.0f%%)\n", 
                   i + 1, n_points, 100.0 * (i + 1) / n_points);
        }
    }
    
    printf("Circular velocity complete.\n");
}

void save_profile_data(const char *filename, double *r_values, 
                      double *mass_values, double *velocity_values, int n_points) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Cannot open file '%s'\n", filename);
        return;
    }
    
    // Write header
    fprintf(fp, "# Galaxy Profile Data\n");
    fprintf(fp, "# r(kpc)         M(Msun)          Vc(km/s)\n");
    fprintf(fp, "#----------------------------------------------\n");
    
    // Write data
    for (int i = 0; i < n_points; i++) {
        fprintf(fp, "%.8e  %.8e  %.8e\n", 
                r_values[i], mass_values[i], velocity_values[i]);
    }
    
    fclose(fp);
    printf("Saved to: %s\n", filename);
}

void save_profiles(ModelType model, double param1, double param2, 
                  double r_max, const char *filename) {
    const int N_POINTS = 200;
    
    // Allocate arrays
    double *r_values = (double *)malloc(N_POINTS * sizeof(double));
    double *mass_values = (double *)malloc(N_POINTS * sizeof(double));
    double *velocity_values = (double *)malloc(N_POINTS * sizeof(double));
    
    if (r_values == NULL || mass_values == NULL || velocity_values == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        if (r_values) free(r_values);
        if (mass_values) free(mass_values);
        if (velocity_values) free(velocity_values);
        return;
    }
    
    // Generate logarithmic radial grid
    double r_min = r_max / 1000.0;
    for (int i = 0; i < N_POINTS; i++) {
        double log_r_min = log10(r_min);
        double log_r_max = log10(r_max);
        double log_r = log_r_min + (log_r_max - log_r_min) * i / (N_POINTS - 1);
        r_values[i] = pow(10.0, log_r);
    }
    
    // Print header
    printf("\n--- %s Model ---\n", model == PLUMMER ? "PLUMMER" : "HERNQUIST");
    
    // Calculate profiles
    calculate_mass_profile(model, param1, param2, r_values, mass_values, N_POINTS);
    calculate_circular_velocity(mass_values, r_values, velocity_values, N_POINTS);
    
    // Print statistics
    printf("\nStatistics:\n");
    printf("  r_min = %.6e kpc, r_max = %.6e kpc\n", r_values[0], r_values[N_POINTS-1]);
    printf("  M_total = %.6e Msun\n", mass_values[N_POINTS-1]);
    printf("  Vc(r_max) = %.2f km/s\n", velocity_values[N_POINTS-1]);
    
    // Find peak velocity
    double max_vel = 0.0;
    double r_peak = 0.0;
    for (int i = 0; i < N_POINTS; i++) {
        if (velocity_values[i] > max_vel) {
            max_vel = velocity_values[i];
            r_peak = r_values[i];
        }
    }
    printf("  Vc_max = %.2f km/s at r = %.6e kpc\n", max_vel, r_peak);
    
    // Save to file
    save_profile_data(filename, r_values, mass_values, velocity_values, N_POINTS);
    
    // Free memory
    free(r_values);
    free(mass_values);
    free(velocity_values);
}