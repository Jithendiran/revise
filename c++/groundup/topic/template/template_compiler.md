## Template Instantiation — How the Compiler Generates Code

### Implicit Instantiation

When a template is used, the compiler automatically generates code:

```cpp
template<typename T>
T square(T x) { return x * x; }

square(5);    // generates: int    square(int    x) { return x * x; }
square(2.5);  // generates: double square(double x) { return x * x; }
```

### Explicit Instantiation

Forces code generation in one translation unit — reduces compile times and binary size:

```cpp
// In a .cpp file — generate here
template int    square<int>(int);
template double square<double>(double);

// In a header — tell other TUs not to instantiate
extern template int    square<int>(int);
extern template double square<double>(double);
```

### Only Used Members Are Instantiated

For class templates, only called member functions are compiled:

```cpp
template<typename T>
class Box {
    T value;
public:
    Box(T v) : value(v) {}

    void impossible() {
        value.nonExistentMethod();   // error in body
    }
};

Box<int> b(42);   // VALID: impossible() never called — never instantiated
b.impossible();   // NOW compile error: function instantiated here
```

### Templates Must Be Visible at Point of Use

Unlike regular functions, template definitions must be in headers because the compiler needs the full definition to instantiate:

* The requirement comes down to how the C++ compilation model works. Unlike regular functions, templates are not actual code; they are blueprints.
* The compiler doesn't generate machine code for a template when it reads the template definition. It waits until it sees an instantiation (like `Box<int> myBox;`).
* With regular functions, if `main.cpp` just sees a declaration (`void foo();`), it assumes the function exists somewhere else, compiles anyway, and leaves a placeholder for the linker to connect later. Templates don't work this way. The linker cannot instantiate templates; only the compiler can, and the compiler needs the full source code to do it not the blueprint code.

```cpp
// CORRECT: full definition in header
// box.h
template<typename T>
class Box { /* full definition */ };

// WRONG for templates: definition hidden in .cpp
// box.h
template<typename T>
class Box;   // declaration only — other .cpp files cannot instantiate
```

## SFINAE — Substitution Failure Is Not An Error

### What It Is

When the compiler substitutes deduced types into a template and the substitution produces an invalid type or expression, the compiler does not report an error — it silently discards that template from the candidate set and looks for other overloads.

### Why It Exists

```cpp
template<typename T>
typename T::value_type front(T container) {
    return container.front();
}

front(std::vector<int>{1, 2, 3});
// T = vector<int>, T::value_type = int — valid

front(42);
// T = int, int::value_type does not exist — invalid substitution
// WITHOUT SFINAE: compile error
// WITH SFINAE: template silently discarded, compiler looks for other overloads
// since no overload found `error: no matching function for call to ‘front(int)’` will be thrown 
```

SFINAE is meant to be used when code has multiple templates. If one fails, it is silently discarded, but the compiler successfully finds a different one to use instead, resulting in zero compiler errors.

### Where SFINAE Applies and Does Not Apply

SFINAE applies to substitution failures in the immediate context — the template *parameter list* and *function signature*.
  * Immediate Context (SFINAE applies): This includes the template parameter list, the function's return type, and the function's parameter types. If a substitution error happens here, the compiler silently drops the template.
SFINAE does NOT apply to errors in the function body:
  * The Function Body (SFINAE does NOT apply): Once the compiler successfully passes the function signature and decides to instantiate the function, any error found inside the `{ ... }` curly braces is a hard compilation error. 

```cpp
template<typename T>
void process(T x) {
    x.nonExistent();   // body error — NOT SFINAE — always compile error
}
```