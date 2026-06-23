## assert
Software programs execute instructions based on specific assumptions. For example, a function that divides two numbers assumes the divisor is not zero. A function that retrieves data from a specific memory address assumes that the pointer to that address is valid.

If these assumptions are violated during execution, the program enters an undefined or invalid state, which can lead to data corruption, incorrect results, or unpredictable crashes. To prevent this, developers need a mechanism to verify that these foundational assumptions hold true while the code is running.

**What is an Assertion?**

An assertion is a systematic check embedded within source code to verify that a specific condition evaluates to true at a precise point in execution.
If the condition is true, the program continues running normally. If the condition is false, it means a foundational assumption of the program has been violated. The assertion immediately halts the program and prints an error message containing the file name, line number, and the condition that failed.

**The Purpose of Assertions ("The Why")**
* Early Failure: Finding a bug at the exact moment an assumption is violated is highly efficient. Without assertions, a program might continue running with corrupted data and crash much later, making the original cause difficult to trace.

### Runtime Assertions: assert
The `assert` macro is used to check conditions that can only be evaluated while the program is running (runtime).
```cpp
#include <cassert>

assert(condition); //condition: Any expression that evaluates to a boolean value (true or false).
```
1. If the condition is non-zero (true), execution moves to the next line of code.
2. If the condition is zero (false), the program prints a diagnostic message to the standard error stream (`stderr`) and calls `std::abort()`, terminating the program immediately.

### Disabling Assertions in Production
Evaluating assertions requires processing power. While beneficial during development and testing, running these checks in a final, production-ready software release can slow down performance.

The C++ standard allows all assert statements to be removed automatically during compilation by defining the macro `NDEBUG` (No Debug) before including `<cassert>`.
```cpp
#define NDEBUG
#include <cassert>

// Because NDEBUG is defined, this line is completely ignored by the compiler.
assert(count > 0);
```

Critical Rule: Because assertions can be disabled completely, *never place executable code* that changes the program state *inside an assert* statement. For example, `assert(update_score() > 0);` will fail to execute `update_score()` entirely if `NDEBUG` is defined.

## Compile-Time Assertions: static_assert
Some conditions can be verified before the program ever runs, during the compilation phase. C++ provides `static_assert` for this purpose.

### The why
Different computer architectures allocate different amounts of memory to data types (e.g., an int might be 2 bytes on an embedded microcontroller but 4 bytes on a desktop computer). If a program relies strictly on a data type being an exact size, running it on an incompatible system will cause silent failures. `static_assert` stops the compilation process entirely, forcing the developer to fix the issue before an executable can even be generated.

```cpp
static_assert(constant_expression, "Error message string");
```
* `constant_expression`: A condition that the compiler can evaluate fully during compilation. It cannot depend on runtime variables or user input.
* "Error message string": The custom message displayed by the compiler if the check fails.

### Permissible Expressions inside static_assert
For an expression to be valid inside a `static_assert`, it must qualify as a compile-time constant expression (formally known in C++ as a constant expression). This means the compiler must be able to completely evaluate the expression and determine its outcome (`true` or `false`) before the program runs.

If the expression relies on any information that is only known at runtime, the compiler will generate an error.

* Not all `const` variables are valid in a `static_assert`. To be valid, a `const` variable must be initialized with a constant expression at the point of declaration. Variables declared with `constexpr` are always valid because the keyword guarantees compile-time evaluation.
    ```cpp
    // Valid: The value is known at compile time.
    const int max_users = 100;
    static_assert(max_users > 50, "Error: Max users allocation is too low.");

    // Valid: constexpr forces compile-time evaluation.
    constexpr int buffer_size = 1024;
    static_assert(buffer_size >= 512, "Error: Buffer size is insufficient.");

    // INVALID: Even though 'system_id' is const, its value depends on a runtime function.
    const int system_id = get_runtime_id(); 
    static_assert(system_id > 0, "Error: Invalid system ID."); // Compile error
    ```
* `consteval` and `constexpr` are allowed
    ```cpp
    // A consteval function (C++20) explicitly runs only at compile time.
    consteval int square(int x) {
        return x * x;
    }

    // A constexpr function can run at compile time if given constant arguments.
    constexpr int cube(int x) {
        return x * x * x;
    }

    // Valid: Both functions return values known entirely to the compiler.
    static_assert(square(4) == 16, "Error: Square calculation failed.");
    static_assert(cube(3) == 27, "Error: Cube calculation failed.");
    ```
* Type Information and the sizeof Operator
    The memory size of data types and objects is determined during compilation. Therefore, sizeof expressions are completely valid.
    ```cpp
    struct DataPacket {
        int id;
        double timestamp;
    };

    // Valid: Evaluates the size of the structure during compilation.
    static_assert(sizeof(DataPacket) == 16, "Error: DataPacket size mismatch due to padding.");
    ```