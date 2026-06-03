#include <stdio.h>
#include "config.h" // Textually copies the struct definition and function prototype

int main() {
    printf("=== PHASE 1.2: UNDERSTANDING THE LINKER AND TUs ===\n\n");

    struct DatabaseConfig my_db = { 5432 };
    
    // At this stage, the compiler doesn't know *how* print_config works, 
    // it just trusts config.h that it exists.
    print_config(&my_db); 

    return 0;
}

/*
gcc -E main.c -o /tmp/main.i // open this file and see
gcc -c main.c -o /tmp/main.o

gcc -E config.c -o /tmp/config.i
gcc -c config.c -o /tmp/config.o

gcc /tmp/main.o /tmp/config.o -o /tmp/compilation_test
/tmp/compilation_test
*/