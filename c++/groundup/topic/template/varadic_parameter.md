# Variadic template

## Step 1: Motivation and Syntax of Parameter Packs
### The Problem
Prior to C++11, every function template and class template required a fixed, explicit number of template parameters defined at compile time.

Consider a function designed to log items of varying types. Without the ability to accept an arbitrary number of parameters, distinct overloads must be written for every possible quantity of arguments:

```cpp
// Fixed overloads required prior to C++11
template <typename T1>
void log_items(T1 a);

template <typename T1, typename T2>
void log_items(T1 a, T2 b);

template <typename T1, typename T2, typename T3>
void log_items(T1 a, T2 b, T3 c);
```

This approach breaks down when the quantity of arguments is unknown or unbounded, forcing developers to rely on type-unsafe arrays or container allocations.

### The Mechanism
A variadic template introduces a template parameter pack, which is a template parameter that accepts zero or more template arguments.
An ellipsis `(...)` placed to the left of the parameter name designates it as a pack.
```cpp
template <typename... Args>
void process(Args... args);
```
* `typename... Args`: A template parameter pack holding a compile-time sequence of zero or more types (e.g., {int, double, char}).
* `Args... args`: A function parameter pack holding a runtime sequence of zero or more values corresponding to the types in Args.

### Compiler Processing
When the compiler encounters an instantiation call such as `process(10, 3.14, 'a');`:
1. The compiler inspects the types of the provided function arguments: `int`, `double`, and `char`.
2. The compiler binds the template parameter pack `Args` to the type list `[int, double, char]`.
3. The compiler binds the function parameter pack `args` to the value list `[10, 3.14, 'a']`.
4. The compiler generates a concrete, overloaded function signature: 
    ```cpp
    void process<int, double, char>(int, double, char);
    ```
## Step 2: Accessing Types from a Parameter Pack Directly
### The Problem
A template parameter pack (`Args...`) is an ordered collection of types evaluated at compile time. However, unlike a standard array or vector, direct index-based subscripting (such as `Args[0]`) is syntactically invalid in C++ templates.

When function parameter names are omitted or when operating purely at the type level (without concrete function arguments), extracting an individual type requires explicit compile-time mechanisms.

### The Mechanisms
1. Counting the Elements: `sizeof...`
   Before extracting a type, the total number of elements in the pack is queried using the `sizeof...` operator, which evaluates to a compile-time constant `std::size_t`.
   ```cpp
    template <typename... Args>
    void inspect() {
        constexpr std::size_t count = sizeof...(Args);
    }
   ```
   `sizeof...(Args)`: Returns the exact integer count of types stored within the pack `Args`.
2. Type Extraction via `std::tuple_element`
    To extract a specific type at a known compile-time index without relying on function parameters, the standard library utility `std::tuple_element` is utilized.
    ```cpp
    #include <tuple>

    template <typename... Args>
    void examine_first() {
        // Extracts the type at index 0 from the parameter pack packaged inside a std::tuple
        using FirstType = typename std::tuple_element<0, std::tuple<Args...>>::type;
    }
    ```
    * `std::tuple<Args...>`: A standard template structure that temporarily wraps the parameter pack into an indexed structure, assigning sequential zero-based integer indices to each type.
    * `std::tuple_element<I, Tuple>`: A compile-time helper struct containing a nested type definition named type that corresponds to the type at index I.
  
    **Compiler Processing**
    1. The compiler evaluates `std::tuple<Args...>` with a concrete pack, creating a concrete type layout: `std::tuple<int, double, char>`.
    2. The compiler evaluates `std::tuple_element<0, std::tuple<int, double, char>>`.
    3. The internal compiler resolution accesses the zeroth element of the tuple structure, resolving `type` to `int`.

## Step 3: Accessing and Using Values from a Function Parameter Pack
### The Problem
When a function template accepts an arbitrary number of arguments via a function parameter pack (`Args... args`), those values reside inside a single packed entity.
```cpp
template <typename... Args>
void print_all(Args... args) {
    // How to access individual values inside 'args' without a variable name like args[0]?
}
```
Direct index-based access (such as `args[0]`) is syntactically invalid because a parameter pack is not a standard array; it is a compile-time expansion construct.

### The Mechanism: Pack Expansion and Indexing via std::get
To extract a specific value by index at runtime or compile time without naming each variable individually, the parameter pack is forwarded into a `std::tuple` or `std::initializer_list`.

Using `std::forward_as_tuple` wraps the runtime values into a tuple of references, allowing index-based access via `std::get<I>`.

```cpp
#include <tuple>
#include <iostream>

template <typename... Args>
void process_third_value(Args&&... args) {
    // Wrap the parameter pack into a std::tuple of references
    auto t = std::forward_as_tuple(args...);
    
    // Access the value at index 2 (the third element)
    std::cout << std::get<2>(t) << "\n";
}
```
* `std::forward_as_tuple(args...)`: Constructs a `std::tuple` consisting of references to the elements in the function parameter pack `args`, preserving value categories (lvalues and rvalues).
* `std::get<I>(t)`: A template function that extracts the element at zero-based compile-time index `I` from the tuple `t`.

### Compiler Processing
1. The compiler receives a call: `process_third_value(10, 3.14, 'a', "hello");`.
2. The compiler expands `args...` inside `std::forward_as_tuple` into individual arguments: `std::forward_as_tuple(10, 3.14, 'a', "hello")`.
3. The compiler generates a concrete `std::tuple<int&, double&, char&, const char(&)[6]>`.
4. The compiler resolves `std::get<2>(t)` to a direct reference to the third element ('a') stored within the tuple memory layout.

## Step 4: Processing All Values Using Fold Expressions
### The Problem
Prior to C++17, performing an operation across every element of a function parameter pack (such as summing numbers or writing multiple items to an output stream) required writing recursive function templates with a base case.

Consider a function to sum multiple integers without fold expressions:
```cpp
// Base case for recursion termination
inline int sum_all() {
    return 0;
}

// Recursive step
template <typename T, typename... Rest>
int sum_all(T first, Rest... rest) {
    return first + sum_all(rest...);
}
```
This pattern requires a separate zero-argument overload to terminate the recursion and introduces deep template instantiation chains, increasing compilation time and code verbosity.

### The Mechanism
A fold expression evaluates a binary operator over all elements of a parameter pack in a single, direct expression without requiring recursion or base cases.

A fold expression uses an ellipsis `(...)` combined with a binary operator.
```cpp
template <typename... Args>
auto sum_all(Args... args) {
    // Unary right fold
    return (args + ...);
}
```
`(args + ...)`: A unary right fold. The compiler expands this expression by placing the binary operator `+` between every element of the parameter pack `args` from right to left.

### Fold Expression Variants
1. Unary Right Fold: `(pack + ...)` expands to `arg1 + (arg2 + (arg3 + arg4))`
2. Unary Left Fold: `(... + pack)` expands to `((arg1 + arg2) + arg3) + arg4`
3. Binary Right Fold: `(pack + ... + init)` expands to` arg1 + (arg2 + (arg3 + init))`
4. Binary Left Fold: `(init + ... + pack)` expands to `((init + arg1) + arg2) + arg3`

### Compiler Processing
1. The compiler receives a call: `sum_all(1, 2, 3, 4);`.
2. The compiler binds the parameter pack `args` to the values `[1, 2, 3, 4]`.
3. The compiler encounters the fold expression `(args + ...)` and expands the operator across the pack: `1 + (2 + (3 + 4))`
4. The compiler evaluates the expression at compile time (if operands are constexpr) or generates the corresponding arithmetic machine instructions.
    ```
    Pack Input:        [1, 2, 3, 4]
    Fold Operator:     + (Unary Right)
    Expansion Trace:   1 + (2 + (3 + 4))
    ---------------------------------------------------------
    Final Result:      10
    ```
## Step 5: Pack Expansion in Expression Lists
### The Problem
Fold expressions require a binary operator (such as `+`, `<<`, or `&&`) to connect elements. When an operation involves calling a function that returns `void` for each element in a parameter pack, standard binary fold expressions cannot be used directly because `void` operands cannot participate in binary arithmetic or logical operations.

Consider a requirement to invoke a logging function on every individual argument within a parameter pack sequentially from left to right:

```cpp
void log_value(int x);
void log_value(double x);

template <typename... Args>
void process_all(Args... args) {
    // How to invoke log_value on each argument sequentially without a binary operator?
}
```

### The Mechanism
An initializer list expansion forces the compiler to evaluate an expression for every element in a parameter pack sequentially by embedding the pack expansion inside an initializer list braces syntax combined with the comma operator.
```cpp
template <typename T>
void log_value(const T& val) {
    // Concrete handling function for a single item
}

template <typename... Args>
void process_all(Args&&... args) {
    // Unpack into an array initializer list
    int dummy[] = { (log_value(args), 0)... };
}
```
* `(log_value(args), 0)`: A comma expression. The left side (`log_value(args)`) executes as a side effect. The right side evaluates to the integer literal `0`.
* `...`: The pack expansion operator applied to the entire comma expression.
* `int dummy[]`: A temporary array created strictly to force evaluation. The initializer list evaluates every element sequentially from left to right, populating the array with zeros.

### Why It Exists
The initializer list expansion guarantees strict left-to-right evaluation order. The C++ standard mandates that initializer list elements are evaluated sequentially in the order they appear in the comma-separated list. This ensures that the first argument is processed before the second argument, maintaining deterministic execution order for side effects like logging, I/O operations, or resource acquisition.
### Compiler Processing
1. The compiler receives a call: `process_all(10, 3.14);`.
2. The compiler binds the parameter pack args to `[10, 3.14]`.
3. The compiler expands the pattern inside the initializer list braces for each element in the pack:
    ```cpp
    int dummy[] = { (log_value(10), 0), (log_value(3.14), 0) };
    ```
    When expanding a parameter pack inside an array initializer list (like `int dummy[] = { (log_value(args), 0)... };`), every element in the array requires a valid value (in this case, an `int` like `0`). Because calling `log_value(args)` returns `void`—which cannot be used to initialize an array element directly—pairing it with `, 0` ensures the function executes its side-effect first, while still providing a valid `0` to satisfy the array initialization syntax.

    **when function returns proper value**
    ```cpp
        #include <iostream>

        template<typename T>
        T log(T v){
            std::cout<<v<<"\n";
            return v;
        }

        template<typename... T>
        void process(T... args){
            int dummy[] = { log(args)... }; // valid because log returns T
        }

        int main()
        {
            process(1,2,3);
            return 0;
        }
    ```

4. The compiler generates code that executes `log_value(10)`, discards the result `0`, then executes `log_value(3.14)`, discards the result `0`, and allocates the temporary dummy array in stack memory.

    ```
    Pack Input:        [10, 3.14]
    Pattern:           (log_value(args), 0)
    Expanded Array:    [(log_value(10), 0), (log_value(3.14), 0)]
    ---------------------------------------------------------
    Execution Order:   1. log_value(10)
                    2. log_value(3.14)
                    3. Allocate dummy array [0, 0]
    ```

## Step 6: Perfect Forwarding with Parameter Packs

### The Problem

When a function template accepts a parameter pack as standard values or lvalue references, the original value category (whether an argument is a temporary rvalue or a persistent lvalue) is lost.

Consider a wrapper function that passes its parameter pack to an underlying constructor:

```cpp
template <typename... Args>
void wrapper(Args... args) {
    // Calling a target function treats every element in 'args' as an lvalue
    target_function(args...);
}

```

If a caller passes a temporary rvalue (such as a newly constructed string or an integer literal), the parameter pack binds to it, but inside `wrapper`, `args` becomes a named lvalue variable. Consequently, any subsequent function call receives an lvalue, forcing an expensive copy operation instead of a cheap move operation, or failing to compile if the target function requires rvalue references.

### The Mechanism

To preserve the exact value category (lvalue or rvalue, constness, and volatility) of each argument as it passes through a template layer, a parameter pack must use **universal references** (`Args&&...`) paired with **pack expansion using `std::forward`**.

```cpp
template <typename... Args>
void wrapper(Args&&... args) {
    // Forward each argument individually while preserving its value category
    target_function(std::forward<Args>(args)...);
}

```

* **`Args&&...`**: A sequence of universal (or forwarding) reference template parameters that can bind to any lvalue or rvalue argument.
* **`std::forward<Args>(args)...`**: The pack expansion operator applied to `std::forward`. The compiler expands this syntax into individual `std::forward<T_i>(arg_i)` expressions for every element in the pack.

### Why It Exists

This mechanism powers standard container creation utilities (such as `std::make_unique` and `std::make_shared`) and wrapper callbacks. It ensures zero overhead by eliminating redundant object copies during intermediate function delegations, allowing resources to be moved directly into destination memory.

### Compiler Processing

1. The compiler receives a call: `wrapper(10, std::string("text"));`.
2. The compiler binds `Args` to `[int, std::string]` and `args` to `[int&, std::string&&]` due to reference collapsing rules.
3. The compiler expands the expression `std::forward<Args>(args)...` into:
```cpp
target_function(std::forward<int>(/* arg 1 */), std::forward<std::string>(/* arg 2 */));

```


4. The first argument is forwarded as an integer rvalue/value, and the second argument is forwarded as an rvalue reference, enabling a move constructor invocation for the string without copying heap memory.

```text
Input Call:        wrapper(10, temporary_string)
Pack Types (Args): [int, std::string]
Pack Values (args):[int&, std::string&&]
Expansion Trace:   target_function(std::forward<int>(arg1), std::forward<std::string>(arg2))
---------------------------------------------------------
Result:            Exact value categories preserved; zero unnecessary copies.

```

## Comprehensive Summary of Variadic Templates
A variadic template is a template mechanism that accepts a zero-to-infinite sequence of template arguments or function arguments. It replaces fixed-length overloads with a unified structure that processes arbitrary type lists and value lists at compile time.
```
+---------------------------------------------------------------------------------+
|                           VARIADIC TEMPLATE ARCHITECTURE                        |
+---------------------------------------------------------------------------------+
| Component Name         | Syntax Example           | Core Purpose                |
+------------------------+--------------------------+-----------------------------+
| Template Parameter Pack| typename... Args         | Stores a compile-time type  |
|                        |                          | sequence.                   |
+------------------------+--------------------------+-----------------------------+
| Function Parameter Pack| Args... args             | Stores a runtime value      |
|                        |                          | sequence.                   |
+------------------------+--------------------------+-----------------------------+
| Pack Size Query        | sizeof...(Args)          | Returns the exact element   |
|                        |                          | count as a constant integer.|
+------------------------+--------------------------+-----------------------------+
| Type Extraction        | std::tuple_element       | Retrieves an individual     |
|                        |                          | type at index I.            |
+------------------------+--------------------------+-----------------------------+
| Value Indexing         | std::get<I>(tuple)       | Retrieves a runtime value   |
|                        |                          | from a wrapped pack.        |
+------------------------+--------------------------+-----------------------------+
| Fold Expression        | (args + ...)             | Applies a binary operator   |
|                        |                          | across all pack elements.   |
+------------------------+--------------------------+-----------------------------+
| Initializer Expansion  | { (func(args), 0)... }   | Forces strict sequential    |
|                        |                          | left-to-right evaluation.   |
+------------------------+--------------------------+-----------------------------+
| Perfect Forwarding     | std::forward<Args>(args).| Preserves exact lvalue/     |
|                        |                          | rvalue value categories.    |
+------------------------+--------------------------+-----------------------------+
```