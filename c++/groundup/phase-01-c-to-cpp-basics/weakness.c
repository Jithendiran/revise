#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// WEAKNESS 2: No Encapsulation
// Any rogue piece of code can bypass business logic and corrupt this data.
// ============================================================================
struct BankAccount {
    char holder[20];
    double balance;
};

// ============================================================================
// WEAKNESS 3: Macro Pollution
// A blind text-replacer that triggers unexpected double-evaluation bugs.
// ============================================================================
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// ============================================================================
// WEAKNESS 1: Lack of Type Safety
// Using void* to force the compiler to ignore mismatched data types.
// ============================================================================
void print_as_integer(void* ptr) {
    int* int_ptr = (int*)ptr; // Blindly assuming this raw memory holds an int
    printf("[1. Type Safety Failure] Intercepted raw memory value: %d\n", *int_ptr);
}

// ============================================================================
// WEAKNESS 4: Manual Resource Lifecycle
// An early exit path causes a silent, hazardous memory leak.
// ============================================================================
void leak_memory_demonstration() {
    int* leaked_array = (int*)malloc(1000 * sizeof(int)); // Grab memory
    
    int security_alert = 1; 
    if (security_alert) {
        printf("[4. Resource Leak] Error triggered! Exiting function early...\n");
        return; // DANGER: Function exits, but 'leaked_array' memory is trapped!
    }

    free(leaked_array); // This line is skipped entirely
}

// ============================================================================
// BONUS WEAKNESS 5: Missing Out-of-Bounds & Buffer Protection
// C arrays do not check boundaries. Writing too much data spills into other variables.
// ============================================================================
void buffer_overflow_demonstration() {
    // We intentionally declare these next to each other
    char target_secret[8] = "SECRET";
    char short_buffer[4] = "AAA"; 

    printf("[5. Buffer Overflow] Before overflow:\n");
    printf("    short_buffer: %s\n", short_buffer);
    printf("    target_secret: %s\n", target_secret);

    // DANGER: Copying 7 characters into a buffer that can only hold 4 characters.
    // It will spill over and corrupt 'target_secret'!
    strcpy(short_buffer, "OVERFLOW"); 

    printf("    After overflow:\n");
    printf("    short_buffer: %s\n", short_buffer);
    printf("    target_secret: %s (CRITICAL: Secret data corrupted!)\n", target_secret);
}

// ============================================================================
// MAIN EXECUTION
// ============================================================================
int main() {
    printf("=== THE ULTIMATE C VULNERABILITY PLAYGROUND ===\n\n");

    // ------------------------------------------------------------------------
    // CASE 1: Lack of Type Safety
    // ------------------------------------------------------------------------
    float pi = 3.14159f;
    // We pass a float address into an int reader. Zero compiler warnings!
    print_as_integer(&pi); 
    printf("\n");

    // ------------------------------------------------------------------------
    // CASE 2: No Encapsulation
    // ------------------------------------------------------------------------
    struct BankAccount account = {"Alice", 500.00};
    account.balance = -99999.00; // Directly altering data without verification
    printf("[2. Encapsulation Failure] %s's balance corrupted to: $%.2f\n\n", account.holder, account.balance);

    // ------------------------------------------------------------------------
    // CASE 3: Macro Pollution
    // ------------------------------------------------------------------------
    int x = 10;
    int y = 5;
    // Expands to: ((x++) > (y) ? (x++) : (y)) -> 'x' increments twice!
    int max_val = MAX(x++, y); 
    printf("[3. Macro Pollution] Expected max: 10, Actual max returned: %d\n", max_val);
    printf("    Expected 'x' to be 11, but 'x' is actually: %d\n\n", x);

    // ------------------------------------------------------------------------
    // CASE 4: Manual Resource Lifecycle
    // ------------------------------------------------------------------------
    leak_memory_demonstration();
    printf("    Result: Memory was left permanently un-freed in the system.\n\n");

    // ------------------------------------------------------------------------
    // CASE 5: Buffer Overflow
    // ------------------------------------------------------------------------
    buffer_overflow_demonstration();
    printf("\n===============================================\n");

    return 0;
}