#include <iostream>

// Can be evaluated at compile-time OR runtime
constexpr int square(int x) {
    return x * x;
}

// FORCED compile-time evaluation (Immediate function)
consteval int cube(int x) {
    return x * x * x;
}

int main() {
    // --- WHAT IS ALLOWED ---
    
    // 1. Compile-time evaluation using constexpr
    constexpr int compile_time_sq = square(5); // Evaluated at compile-time
    
    // 2. Runtime evaluation using constexpr (Flexibility!)
    int runtime_input = 5; 
    int runtime_sq = square(runtime_input);    // Works fine; runs at runtime
    
    // 3. Compile-time evaluation using consteval
    constexpr int compile_time_cb = cube(5);   // Evaluated at compile-time

    std::cout << "Square: " << compile_time_sq << ", Cube: " << compile_time_cb << "\n";


    // --- WHAT IS NOT ALLOWED (Will cause compiler errors) ---

    // Error 1: consteval MUST have compile-time constants as arguments
    // int runtime_cb = cube(runtime_input); 
    // ERROR: 'runtime_input' is not a constant expression

    // Error 2: You cannot use runtime-only actions inside constexpr/consteval
    // constexpr int invalid_func() {
    //     std::cout << "Hello"; // ERROR: std::cout is a runtime operation
    //     return 42;
    // }

    return 0;
}