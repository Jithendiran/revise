## Const falvours

```
[ Writing Code ] ---> [ Compilation Phase ] ---------> [ Loading Program ] ---> [ Runtime Execution ]
                       |                                |                         |
                       * consteval (Must run here)      * constinit               * const (Read-only enforcement)
                       * constexpr (Can run here)       (Guarantees static        * mutable (The const escape hatch)
                       * if consteval                   initialization before     * if consteval (Runtime branch)
                         (Compile-time branch)          dynamic init)             * constexpr (functions)
```

### `const` (The Read-Only Guard)

`const` does **not** mean compile-time. It means **read-only**. It is a contract enforced by the compiler stating, *"Once this variable is given a value, it cannot be modified."*


#### Variables

* **Allowed:** Can be initialized with variables only known at runtime (like user input) or compile-time literals.
* **Not Allowed:** Modifying the variable after its initialization.

```cpp
int x;
std::cin >> x; 
const int runtime_const = x; // ALLOWED: Value locked at runtime.
// runtime_const = 10;       // NOT ALLOWED: Compiler error.
```

### `constexpr` (The Flexible Workhorse)

`constexpr` stands for **Constant Expression**. 
- Applied to a variable, it means the value *must* be known at compile-time. 
- Applied to a function, it means the function *is capable* of being evaluated at compile-time if its inputs are constants or runtime. 

#### Variables

* **Allowed:** Must be initialized with an expression that can be fully computed during compilation.
* **Not Allowed:** Assigning runtime variables or calling non-constexpr functions.

```cpp
int runtime_val = 5;
const int runtime_val2 = 5;
constexpr int a = 10;             // ALLOWED
constexpr int b = a * 5;          // ALLOWED
// constexpr int c = runtime_val;  // NOT ALLOWED: runtime_val is not known at compile time.
constexpr int d =  runtime_val2;    // allowed; bcz it be a optmized at compile time

void fun(const int runtime_val){
    //constexpr int b = runtime_val * 5;    // Not allowed
}
```

#### Functions
* **Allowed:** Can be invoked at compile-time OR runtime. If pass as a runtime variable, it gracefully downshifts into a regular runtime function.
* **Not Allowed (Inside the function):** Cannot execute runtime-only code like `std::cout`, or dynamic allocations that aren't freed before the function returns (C++20 allows transient allocation).

```cpp
constexpr int square(int x) { 
    // std::cout << "A"; // Not allowed because, it required OS level function at run time, compiler cannot execute this
    std::vector<int> v = {1, 2, 3}; // Allowed in C++20! the compiler mimics an entire operating system and heap allocator inside its own memory space while it builds
    /*
    Because the vector is completely destroyed before the function finishes, the memory allocation is transient (temporary). It leaves absolutely zero trace in the final application binary except for the final computed answer (31).
    */
    int * a = new int(3);
    delete a; // without this line funcction won't compile
    return x * x + v[0] + v[1] + v[2];
}

constexpr int compile_time = square(5); // Runs at compile-time.
int input = 5;
int runtime_time = square(input);       // Runs at runtime. (Perfecty allowed!)

```

**Why cout not allowed vecotor or (new) memory operation allowed?**
* When the compiler executes a `std::vector` at compile time, everything it needs exists entirely inside the C++ language rules.
* `std::cout` is not just a variable; it is a complex stream object linked to the Runtime Operating System (OS).


### `consteval` (The Strict Immediate Function)

`consteval` completely replaces runtime flexibility. It forces a function to be an **immediate function**. It **must** execute at compile-time, or the program will fail to build.


### Variables

* **NOT ALLOWED:** `consteval` cannot be used on variables. It is strictly a function keyword.

### Functions

* **Allowed:** Pure logic, loops, branches, and constant inputs.
* **Not Allowed:** Passing regular runtime variables into its arguments, or calling it outside of a compile-time context.

```cpp
consteval int cube(int x) { return x * x * x; }

consteval int square(int x) { 
    // std::cout << "A"; // Not allowed because, it required OS level function at run time, compiler cannot execute this
    std::vector<int> v = {1, 2, 3}; // Allowed in C++20! the compiler mimics an entire operating system and heap allocator inside its own memory space while it builds
    /*
    Because the vector is completely destroyed before the function finishes, the memory allocation is transient (temporary). It leaves absolutely zero trace in the final application binary except for the final computed answer (31).
    */
    int * a = new int(3);
    delete a; // without this line funcction won't compile
    return x * x + v[0] + v[1] + v[2];
}

constexpr int safe = cube(5); // ALLOWED
int input = 5;
// int broken = cube(input);  // NOT ALLOWED: compilation fails instantly.
constexpr int compile_time = square(5); // Runs at compile-time.
// int runtime_time = square(input);       // NOT ALLOWED

```

### `const_cast`

When dealing with "const stuff," how to remove it is just as important as how to apply it. `const_cast` is the only explicit cast allowed to add or strip away `const` or `volatile` qualifiers.

* **The Critical Warning:** Stripping `const` from a variable that was *originally declared* as `const` and modifying it results in **Undefined Behavior**. Stripping `const` from a variable that was originally non-const (but passed via a const-reference) is safe.

```cpp
void legacy_api(char* str); // Old API that forgets to use const char*

void modern_wrapper(const char* str) {
    // Safe only if legacy_api promises not to actually mutate the character array
    legacy_api(const_cast<char*>(str)); 
}

```

## std library

### `std::as_const`
Found in `<utility>`, this function takes a reference and returns it as a `const` reference. It is primarily used to force the compiler to choose a `const` overload of a function when we only have a non-const object.

```cpp
std::string text = "Hello";
void print(std::string& s);       // Overload A
void print(const std::string& s); // Overload B

print(std::as_const(text)); // Explicitly forces Overload B

```

### `std::const_iterator` & `std::as_const` helpers (C++23)

While we've had `cbegin()` and `cend()` since C++11, C++23 introduced `std::make_const_iterator` and generic `std::cbegin` ranges to reliably extract read-only iterators from containers, preserving data safety during iterations.


## 3. The `std::constant_range` (C++23)

In C++20 and C++23 Ranges, type safety extends to collections via concepts. C++23 introduces `std::ranges::constant_range`.

This is a concept used in template programming to verify if a range protects its elements from modification, allowing you to write generic functions that strictly require read-only views.
