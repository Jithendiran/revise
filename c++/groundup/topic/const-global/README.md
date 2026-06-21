## External linkage

A global variable is declared outside of all functions, usually at the top of a file.

### The C vs C++ Difference

```c
const int a = 9;
int b = 9;
```

#### C
Global `const` variables have external linkage by default.
* Any file in the project can access the variable.
* To use it in another file, declare it with `extern const int a;`.
* To restrict the variable to its own file (internal linkage), the `static` keyword is required:
  ```c
  static const int a = 9; // Restricted to this file
  ```
#### C++
Global `const` variables have internal linkage by default.
* The variable is automatically restricted to the file where it is defined.
* To make it accessible to other files (external linkage), the `extern` keyword must be used during definition:
  ```c
  extern const int a = 9; // Accessible to other files
  ```

In C++, understanding how variables are shared across multiple files (translation units) and when their values are determined (compile-time versus runtime) is essential for efficient memory management.


Compile-Time (`constexpr`): The compiler calculates the value of the variable during **compilation**. This requires the value to be explicitly known at the moment of compilation.

Any function or static data member declared with `constexpr` (or `consteval`) is implicitly `inline`. This means we can safely declare them in header files without causing "duplicate symbol" errors during linking.

### The Default Behavior Trap
When a standard global variable is declared, it defaults to *external linkage*. However, adding `const` or `constexpr` automatically changes the default behavior to *internal linkage*.

```cpp
// file1.cpp
constexpr int max_users = 100; // Has INTERNAL linkage by default
// due to constexpr, programmer must include `= <value>`
```

```cpp
// for external linkage
//file1.cpp
extern  constexpr int max_users = 100;

// file2.cpp
extern constexpr int max_users; // ERROR: constexpr variable must be initialized!
extern const int max_users; // allowed

```

issues with `extern const int max_users; // allowed` this is 
* We lose compile-time optimization: In file2.cpp, the compiler only knows that an integer named max_users exists somewhere in the program, but it doesn't know its value is 100 during compilation. The actual value is linked later.
* No constant expressions: Because the value is unknown to file2.cpp at compile-time, we cannot use `max_users` for things that require a compile-time constant, such as:
  * Setting the size of a standard array (`int arr[max_users];`).
  * As a template argument (`std::array<int, max_users>`).
  * In a switch case label.


# C++ Linkage and Compile-Time Constants: Reference Guide

In C++, understanding how variables are shared across multiple files (translation units) and when their values are determined (compile-time versus runtime) is essential for efficient memory management.

## 1. Core Concepts: Linkage and Compile-Time Availability

### Linkage

Linkage determines the visibility of a variable across different source files (`.cpp` files). There are two primary types of linkage for global variables:

* **External Linkage (Default for standard variables):** The variable is visible to the entire program. Multiple files can access and share the exact same variable in memory.
* **Internal Linkage:** The variable is isolated within the specific file where it is defined. Other files cannot see or access it.

### Compile-Time vs. Runtime

* **Runtime:** The variable receives its value when the program executes.
* **Compile-Time (`constexpr`):** The compiler calculates the value of the variable during compilation. This requires the value to be explicitly known at the moment of compilation.


## 2. The Default Behavior Trap

When a standard global variable is declared, it defaults to external linkage. However, adding `const` or `constexpr` automatically changes the default behavior to **internal linkage**.

```cpp
// file1.cpp
constexpr int max_users = 100; // Has INTERNAL linkage by default

```

### The Issue with Headers and Internal Linkage

If a `constexpr` variable is placed in a header file (`.h`) to be shared, every `.cpp` file that includes that header will create its own independent copy of the variable.

* **Memory Duplication:** If the variable is a large data structure (e.g., an array or lookup table), every translation unit duplicates that data. This increases the final executable file size.
* **No Shared Memory Address:** Each file interacts with a different memory location, meaning the variable is not truly shared.


## 3. The Traditional Solution: `extern const` and `extern constexpr`

To ensure a compile-time constant is shared globally without duplicating memory, the `extern` keyword must be used to force external linkage. This requires a two-step process split between a header file and a source file.

### Step 1: The Declaration in the Header File (`constants.h`)

The header file informs all tracking files that a global constant exists, without defining its value.

```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const int max_users; // Declaration: Promises the variable exists elsewhere

#endif

```

* **Why `extern const` is used here:** A `constexpr` variable requires an immediate value assignment (e.g., `= 100`). Because the value is not provided in the declaration, the `constexpr` keyword cannot be used here. It must be declared as `const`.

### Step 2: The Definition in the Source File (`constants.cpp`)

The source file allocates the actual memory and defines the value.

```cpp
// constants.cpp
#include "constants.h"

extern constexpr int max_users = 100; // Definition: Allocates memory and assigns value

```

* **Why `extern constexpr` is used here:** The value `= 100` is provided, allowing `constexpr` to be applied for compile-time optimization. The `extern` keyword overrides the default internal linkage, forcing the variable to be visible to the declaration in the header file.


## 4. The Modern Solution: `inline constexpr` (C++17 and Newer)

Managing separate declarations and definitions via `extern` is complex. C++17 introduced **inline variables** to resolve this issue.

By applying the `inline` keyword to a `constexpr` variable in a header file, the two-step process is reduced to a single line.

```cpp
// constants.h
#pragma once

inline constexpr int max_users = 100; // Single definition in the header file

```

### Mechanism of `inline constexpr`

1. **Compile-Time Availability:** The value (`100`) is visible to every file that includes the header, satisfying the requirements for `constexpr`.
2. **Single Instance Guarantee:** The `inline` keyword instructs the linker to merge all duplicate definitions across different translation units into a single instance in memory. No memory duplication occurs.

---

## 5. Summary Reference Table

| Approach | File Location | Linkage Type | Memory Allocation | Minimum C++ Version |
| --- | --- | --- | --- | --- |
| `constexpr int x = 10;` | Header File | **Internal** | Separate copy per `.cpp` file | C++11 |
| `extern const` / `extern constexpr` | Split between Header and `.cpp` | **External** | Single shared instance | C++11 |
| `inline constexpr int x = 10;` | Header File | **External** (via inline mechanism) | Single shared instance | C++17 |


When a compiler sees `constexpr`, it automatically treats that function or static variable as `inline`.

To understand what this means, we have to look at how the C++ **linker** works and why the **One Definition Rule (ODR)** exists.
