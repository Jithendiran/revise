
## Compile conditional

### Compile time IF

1. **Regular if (Runtime)**
    The compiler builds both branches into final program. The CPU decides which path to take when the program runs.
    ```cpp
    if (condition) {
        // Both of these are compiled into your binary
    } else {
        // and take up space.
    }
    ```

2. **if constexpr (Compile-time)**
    The compiler evaluates the condition immediately(inside compiler). It keeps one branch and deletes the other from the final binary.
    ```cpp
    if constexpr (condition) {
        // If true, ONLY this code exists in final application.
        // The else block is completely deleted by the compiler.
    }
    ```


### `std::is_constant_evaluated()` (C++20)

Found in `<type_traits>`, this function returns `true` if the current execution thread is running within a compile-time evaluation context.

```cpp
#include <type_traits>
constexpr double power(double base, int exp) {
    if (std::is_constant_evaluated()) {
        // Compile-time path: Simple loop acceptable during compilation
        double res = 1.0;
        for(int i = 0; i < exp; ++i) res *= base;
        return res;
    } else {
        // Runtime path: Call a highly optimized hardware math library
        return __builtin_pow(base, exp); 
    }
}

// compile time
// 1. Assigning to a 'constexpr' or 'constinit' variable
constexpr double static_val = power(2.0, 10); 

// 2. Using it inside a template argument
std::array<int, static_cast<int>(power(2.0, 4))> my_array; 

// 3. Using it in a static_assert
static_assert(power(2.0, 3) == 8.0);

// run time
// 1. Passing dynamic variables (variables that can change)
double b = 2.0;
int e = 10;
double result1 = power(b, e); // RUNTIME (Value isn't known yet)

// 2. Passing normal numbers to a normal variable
double result2 = power(2.0, 10); // RUNTIME! 
// Even though 2.0 and 10 are constants, 'result2' is a normal variable.
// The compiler chooses the optimized runtime math library branch here.
```

### `if consteval` (C++23)

C++23 introduces a cleaner, more robust syntax that acts as a compile-time branch, fixing subtle ordering bugs present in the C++20 function approach.

* **The Use Case:** Providing a highly optimized, hardware-specific assembly path at runtime, but keeping a pure C++ fallback path for compile-time calculation.

```cpp
#include <type_traits>

constexpr double power(double base, int exp) {
    if consteval {
        // Compile-time path: Simple loop acceptable during compilation
        double res = 1.0;
        for(int i = 0; i < exp; ++i) res *= base;
        return res;
    } else {
        // Runtime path: Call a highly optimized hardware math library
        return __builtin_pow(base, exp); 
    }
}

```

### Critical note
```cpp
if constexpr (std::is_constant_evaluated()) {
    return compile_time_loop(base, exp);
} else {
    return __builtin_pow(base, exp); 
}
```

**The Step-by-Step Compilation Failure Mechanics**
1. Enforced Compile-Time Context: When the compiler encounters the keyword sequence `if constexpr (...)`, the language rules state that whatever is inside the parentheses must be evaluated right then and there (at compile-time).
2. The Context Shift: Because the compiler is forced to evaluate the condition during compilation, the evaluation environment inside those parentheses becomes, by definition, a compile-time context.
3. The Function Query: The function `std::is_constant_evaluated()` executes its check. It asks: "Am I currently being looked at during the compilation phase?"
4. The Inevitable True: Because step 1 and step 2 forced a compile-time context, the answer is always yes. `std::is_constant_evaluated()` evaluates to `true`.
5. The Elimination of the Runtime Path: Because the condition is permanently `true` during compilation, the mechanism of `if constexpr` triggers. The compiler preserves the true branch (`compile_time_loop`) and permanently deletes the `else` branch (`__builtin_pow`) from the final compiled application.

**The Consequences at Runtime**
Because the `else` branch was completely stripped out during compilation, the machine code for `__builtin_pow` does not exist in the final binary file.