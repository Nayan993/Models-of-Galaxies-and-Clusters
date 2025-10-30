# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
DATA_DIR = data
OUTPUT_DIR = output

# Target
TARGET = /galaxy_sim

# Source files
SOURCES = 
OBJECTS = 

# Colors for output
GREEN = \033[0;32m
NC = \033[0m

# Default target
all: directories 
    @echo "✅ Build complete! Run with: ./"

# Create directories
directories:
    @mkdir -p    

# Link
: 
    @echo "🔗 Linking..."
    @  -o $@ 

# Compile
/%.o: /%.c
    @echo "🔨 Compiling $<..."
    @  -c $< -o $@

# Clean build files
clean:
    @rm -rf  
    @echo "✅ Cleaned build files"

# Clean everything
cleanall: clean
    @rm -f /*.txt /*.dat
    @rm -f /*.png /*.jpg
    @rm -f scripts/*.gnu
    @echo "✅ Cleaned all generated files"

# Run simulation
run: all
    @echo "🚀 Running simulation..."
    @./

# Help
help:
    @echo "Available targets:"
    @echo "  make          - Build the project"
    @echo "  make run      - Build and run simulation"
    @echo "  make clean    - Remove build files"
    @echo "  make cleanall - Remove all generated files"
    @echo "  make help     - Show this help"

.PHONY: all clean cleanall run help directories
