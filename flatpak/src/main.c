#include <stdio.h>
#include <unistd.h> // For sleep function

// Function to calculate factorial
unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    while (1) {
        for (int i = 1; i <= 20; ++i) {
            printf("Factorial of %d is %llu\n", i, factorial(i));
        }

        printf("Restarting loop...\n\n");
    }

    return 0;
}
