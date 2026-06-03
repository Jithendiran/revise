#include <stdio.h>
#include "config.h" // Includes its own definition to verify consistency

// The actual code for the function
void print_config(struct DatabaseConfig* cfg) {
    printf("[1.2 Linker Check] Database is running on port: %d\n", cfg->port);
}