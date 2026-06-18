## Type Modifiers and Qualifiers
### Concept Definition
Type modifiers and qualifiers change how the compiler treats memory locations. It is not a fundamental data type like (int, float), they dictate the access rules and optimization boundaries for that data.

### Mechanism of Action
1. `const` (Constant): This qualifier tells the compiler that a variable's value is read-only after initialization.
    Logic: If code attempts to rewrite a `const` variable, the compiler halts and throws an error. Because the compiler knows this value cannot change, it optimizes the program by replacing the variable name directly with the literal value in machine code, bypassing slower memory lookups.
2. `volatile`: This qualifier tells the compiler that a variable's value can be changed by factors outside the program's direct control (such as hardware signals or operating system interrupts).
   Logic: Normally, compilers optimize code by storing frequently used variables in high-speed CPU registers instead of slower system RAM. If a variable is marked volatile, the compiler is forced to read the value directly from the actual RAM address every single time it is referenced, ensuring the program always uses the most current external data.

## Compile-Time Evaluation (constexpr, consteval)

### Concept Definition
Compile-time evaluation shifts computational work away from the period when the user runs the program (runtime) to the period when the code is being built (compile-time).

### The "Why": Purpose and Logic
Every calculation performed while a program runs consumes CPU cycles, takes time, and uses battery power. If a calculation relies entirely on values known before the program runs, performing that calculation at runtime is inefficient. Pre-computing these values makes the final application faster and smaller.

### Mechanism of Action
* `constexpr` (Constant Expression): This keyword specifies that a variable or function can be evaluated at *compile-time* if all inputs are known constants. If the inputs are only known at runtime, it behaves like a normal *runtime function*.
* `consteval` (Immediate Function): Introduced in modern C++, this keyword forces a function to *only execute at compile-time*.
    Logic: If the compiler cannot fully resolve a `consteval` function at compile-time (because it relies on unpredictable user input, for example), compilation fails immediately. This guarantees zero runtime performance cost for that specific logic.