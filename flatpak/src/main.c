#include <stdio.h>

unsigned long long factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1); // Recursive factorial
}

int main() {
    int n = 20; // Large input to cause overflow
    unsigned long long result = factorial(n);
    printf("Factorial of %d is %llu\n", n, result);
    return 0;
}