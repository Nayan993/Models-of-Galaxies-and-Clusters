/**
 * Purpose:
 *   Input/Output utilities and Gnuplot automation helpers.
 *
 * Features:
 *   - Save star coordinates to CSV
 *   - Auto-generate Gnuplot scripts for 3D and 2D plots
 *   - Execute Gnuplot from C using system() call
 *   - Create comparison plots between two models
 *   - Ensure required folders exist automatically
 *
 * Written By: Himanshu Thakur + Abhishek Kapoor
 */

#include "utils.h"     // Own fucntion declaration
#include <stdlib.h>    // system(), memory, process utils
#include <string.h>    // String Manupulation Functions
#include <sys/stat.h>  // stat() to check folder existence
#include <sys/types.h> // Needed for stat()/mkdir() types
#include <errno.h>     // Used with perror() for error info
#include <stdio.h>     // File I/O (fopen, printf)

// Platform-specific includes for mkdir()
#ifdef _WIN32
#include <direct.h> // for _mkdir() on Windows
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h> // for POSIX mkdir()
#define MKDIR(path) mkdir(path, 0755)
#endif

/**  ============================================================================
* @breif: Safely open a file and check for errors
* @param filename Path to the file
* @param mode File mode ("r", "w").
* @return: FILE* pointer if successful, NULL otherwise
* = ===========================================================================
*/
static FILE *open_file(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);
    if (!fp)
    {
        fprintf(stderr, "[ERROR] Failed to open file: %s\n", filename);
    }
    return fp;
}

void ensure_dir_exists(const char *path)
{
    // Check if directory exists
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        // Create directory if not found
        if (MKDIR(path) == 0)
        {
            printf("[INFO] Created folder: %s\n", path);
        }
        else
        {
            perror("[ERROR] mkdir failed");
        }
    }
}

int save_stars(const Star *stars, int N, const char *filename)
{
    ensure_dir_exists("data"); // Make sure data folder exists

    // Open file for writing
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        fprintf(stderr, "[ERROR] Failed to open file: %s\n", filename);
        return -1;
    }

    fprintf(fp, "x,y,z\n"); // Header row for Excel

    // Loop through all stars and write their coordinates to file
    for (int i = 0; i < N; i++)
    {
        // Print each star's (x, y, z) with 6 decimal precision
        fprintf(fp, "%.6f,%.6f,%.6f\n", stars[i].x, stars[i].y, stars[i].z);
    }

    fclose(fp); // Close the file to free resources
    printf("[INFO] Saved %d stars to %s Excel\n", N, filename);
    return 0;
}

int create_gnuplot_script(const char *model, const char *scriptfile, const char *datafile, const char *outputfile)
{
    // Make sure folders exist for scripts and outputs
    ensure_dir_exists("scripts");
    ensure_dir_exists("output");

    // Open script file for writing
    FILE *fp = open_file(scriptfile, "w");
    if (!fp)
        return -1;

    // Write Gnuplot commands into the script
    fprintf(fp,
            "# Auto-generated gnuplot script for %s model\n"
            "set terminal pngcairo size 1200,800 enhanced\n" // Output format and image size 1200*800
            "set output '%s'\n" 
            "\n"
            "set title '3D Star Distribution: %s Model'\n"     // Title on plot
            "set xlabel 'X'\n"                                 // X-axis label
            "set ylabel 'Y'\n"                                 // Y-axis label
            "set zlabel 'Z'\n"                                 // Z-axis label
            "set grid\n"
            "set view 60, 30, 1.0, 1.0\n"                      // 3D view angle
            "set datafile separator ','\n"                     // CSV separator
            "\n"
            "splot '%s' using 1:2:3 with points "
            "pointtype 7 pointsize 0.6 lc rgb 'blue' title '%s'\n",
            model, outputfile, model, datafile, model);

    fclose(fp);
    printf("[INFO] Generated 3D gnuplot script: %s\n", scriptfile);
    return 0;
}

int execute_gnuplot(const char *scriptfile)
{
    char command[512];  // Buffer to hold the full command
    // Format the command string as: "gnuplot scriptfile"
    snprintf(command, sizeof(command), "gnuplot %s", scriptfile); 

    int result = system(command); // execute it
    if (result == 0)
    {
        printf("[INFO] Gnuplot executed successfully: %s\n", scriptfile);
    }
    else
    {
        printf("[ERROR] Failed to execute gnuplot script: %s\n", scriptfile);
        printf("[INFO] Try running manually: gnuplot %s\n", scriptfile);
    }

    return 0;
}

int create_comparison_script(const char *scriptfile, const char *datafile1, const char *datafile2, const char *model1_name, const char *model2_name, const char *outputfile)
{
    // Make sure folders exist for scripts and outputs
    ensure_dir_exists("scripts");
    ensure_dir_exists("output");

    // Open file for writing the comparison script
    FILE *fp = open_file(scriptfile, "w");
    if (!fp)
        return -1;

    // Write Gnuplot commands for two subplots (multiplot layout)
    fprintf(fp,
            "# Auto-generated comparison plot\n"
            "set terminal pngcairo size 1200,800 enhanced\n"
            "set output '%s'\n"
            "\n"
            "# Title with empty line below\n"
            "set multiplot layout 1,2 title 'Model Comparison: %s vs %s\\n'\n"
            "set datafile separator ','\n"
            "\n"
            "# Left: Model 1\n"
            "set title '%s'\n"
            "set xlabel 'X'\n"
            "set ylabel 'Y'\n"
            "set grid\n"
            // Plot only X and Y columns (2D projection)
            "plot '%s' using 1:2 with points "
            "pointtype 7 pointsize 0.6 lc rgb 'red' notitle\n"
            "\n"
            "# Right: Model 2\n"
            "set title '%s'\n"
            "plot '%s' using 1:2 with points "
            "pointtype 7 pointsize 0.6 lc rgb 'blue' notitle\n"
            "\n"
            "unset multiplot\n",
            outputfile, model1_name, model2_name,
            model1_name, datafile1,
            model2_name, datafile2);

    fclose(fp); // Close the file
    printf("[INFO] Generated comparison gnuplot script: %s\n", scriptfile);
    return 0;
}
