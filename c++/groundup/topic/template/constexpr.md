## constexpr — Compile-Time Branching (C++17)

### constexpr
```cpp
template<typename T>
bool non_const_var = false; // Regular variable
template<typename T>
bool non_const_var<T*> = true;

template<typename T>
constexpr bool const_var = false; // Compile-time constant
template<typename T>
constexpr bool const_var<T*> = true;

int main() {
    // ERROR: Cannot use a non-constexpr variable for a static_assert or array size
    // static_assert(non_const_var<int*>, "Must be pointer"); 
    // int arr[non_const_var<int*>]; 

    // VALID: constexpr allows compile-time checks
    static_assert(const_var<int*>, "Must be pointer!"); 
    int arr[const_var<int*> ? 5 : 10]; // Works! Array size is determined at compile time.
}
```
1. Without `constexpr` (Runtime Variable)
    Even though the template is instantiated at compile time, the resulting variable acts like a normal global variable.
    - It takes up memory in your final binary.
    - It can be modified at runtime.
    - It cannot be used in compile-time contexts (like static_assert or array sizes).
2. With `constexpr` (Compile-Time Constant)
    Adding `constexpr` tells the compiler that this variable is a fixed value known entirely at compile time.
    - It is usually optimized away completely (embedded directly into the machine code as a literal).
    - It cannot be modified at runtime.
    - It can be used in compile-time contexts.

### The Problem

A runtime `if` in a template compiles both branches for every type:

```cpp
#include <iostream>
void writeInt(int n){
    std::cout<<"Int :: " << n << " \n";
}

void writeString(std::string f){
    std::cout<<"String :: " << f <<  "\n";
}

template<typename T>
void serialize(T value) {
    if (std::is_integral_v<T>) {
        writeInt(value);       //   error: invalid conversion from ‘const char*’ to ‘int’
    } else {
        writeString(value);    //   error: could not convert ‘value’ from ‘int’ to ‘std::string’ 
    }
}

int main()
{
    serialize(42);     
    serialize("Hello");   
    return 0;
}
```

Both branches must compile for every T — impossible when branches require different types.

### if constexpr Solution

`if constexpr` evaluates the condition at compile time. The false branch is discarded — never compiled:

```cpp
#include <iostream>
void writeInt(int n){
    std::cout<<"Int :: " << n << " \n";
}

void writeString(std::string f){
    std::cout<<"String :: " << f <<  "\n";
}

template<typename T>
void serialize(T value) {
    if constexpr (std::is_integral_v<T>) {
        writeInt(value);       //   compiled only when value is integer
    } else {
        writeString(value);    //   compiled only for non integer 
    }
}

serialize(42);     
serialize("Hello"); 

// serialize(3.14); // error because it is non integer, so the compiled code is writeString(...), but it expects string 
//  could not convert ‘value’ from ‘double’ to ‘std::string’ {aka ‘std::__cxx11::basic_string’}
```

### Replacing Recursive Variadic Base Case

```cpp
// Before C++17 — two functions required:
void print() {}
template<typename First, typename... Rest>
void print(First f, Rest... r) { 
    std::cout << f << " "; 
    print(r...); 
}

// C++17 — single function:
template<typename First, typename... Rest>
void print(First f, Rest... r) {
    std::cout << f << " ";
    if constexpr (sizeof...(r) > 0) {
        print(r...);   // only compiled when rest is non-empty
    }
}
```

### if constexpr vs Regular if

| Property               | if           | if constexpr         |
|------------------------|--------------|----------------------|
| Condition evaluated    | At runtime   | At compile time      |
| Discarded branch compiled | YES       | NO                   |
| Condition requirement  | Any bool     | Compile-time constant|
| Branch validity needed | YES          | NO — discarded branch ignored |
| Runtime performance    | Jump instruction | Dead code eliminated |
