## Pointer to Member Function

### What It Is and Why It Exists
A regular function pointer holds the memory address of a standalone (free) function. However, a non-static member function cannot be called without an object because it relies on an implicit `this` pointer to access object state.

A pointer to a member function (PMF) stores an offset to a member function within a class. It represents an unattached function signature that can later be invoked on any valid instance of that class.

```cpp
// Regular function pointer (does NOT work for non-static member functions)
void (*funcPtr)(double); 

// Pointer to member function (specifically tied to BankAccount class)
void (BankAccount::*memFuncPtr)(double);
```

### Syntax and Declaration

Because member functions belong to a class scope, the class type must be explicitly included in the pointer's type declaration using `ClassName::*`.

```cpp
ReturnType (ClassName::*pointerName)(ParameterTypes...);
```

### Assigning an Address
To get the address of a member function, must use the fully qualified name with the address-of operator `&`. Unlike regular functions, implicit conversion to a pointer does not happen.
```cpp
class BankAccount {
public:
    void deposit(double amount) { /* ... */ }
    void withdraw(double amount) { /* ... */ }
};

// Store address of deposit
void (BankAccount::*action)(double) = &BankAccount::deposit;

// Reassign to withdraw (signatures match)
action = &BankAccount::withdraw;
```

### Invoking via Object: The `.*` and `->*` Operators
To execute a pointer to a member function, must bind it to a specific object instance using dedicated C++ operators:
* `.*` — Pointer-to-member operator for objects/references
* `->*` — Pointer-to-member operator for pointers to objects
Note on Precedence: The function call operator `()` has higher precedence than `.*` and `->*`. Parentheses around (`object.*ptr`) are mandatory.

```cpp
BankAccount acc;
BankAccount* accPtr = &acc;

void (BankAccount::*action)(double) = &BankAccount::deposit;

// 1. Invoking on an object instance using .*
(acc.*action)(150.0);       // Equivalent to: acc.deposit(150.0)

// 2. Invoking on an object pointer using ->*
(accPtr->*action)(50.0);    // Equivalent to: accPtr->deposit(50.0)
```

### `const` Correctness in PMFs
The `const` qualifier on a member function is part of its type signature. A pointer to a `const` member function can only point to const member functions, but it can be called on both `const` and non-const objects.

```cpp
class BankAccount {
public:
    double getBalance() const;
    void deposit(double amount);
};

// Must include 'const' at the end of the type declaration
double (BankAccount::*getter)() const = &BankAccount::getBalance;

const BankAccount acc;
(acc.*getter)(); // Valid: const function called on const object

// Error: deposit is non-const, cannot assign to const member pointer
// double (BankAccount::*invalidPtr)() const = &BankAccount::deposit;
```

### Modern Alternatives (std::invoke and auto)
The raw pointer-to-member syntax is notoriously difficult to read and write. Modern C++ provides cleaner abstractions:
```cpp
#include <functional>

auto action = &BankAccount::deposit; // Compiler deduces type

BankAccount acc;
std::invoke(action, acc, 100.0);     // Modern, clean invocation (C++17)
```

## Reference to Member Function
In C++, there is no such thing as a reference to a member function.

While program can have references to regular standalone functions (`void (&ref)()` is valid), language grammar does not permit syntax like `void (BankAccount::&ref)(double)`.

### Why the Language Disallows Them
References in C++ are non-rebinding aliases to existing objects or functions in memory. A non-static member function does not exist as an independent, invokable entity in memory by itself—it requires an instance (`this`) to give it context.
Because a member function cannot be called without an object, an unassigned reference to a class member function would be meaningless.

### Alternatives to achieve "Reference" Behavior
If needed a reference-like variable that aliases a member function bound to an object, C++ provides two standard ways to achieve this:

1. `std::function` with `std::bind`
    ```cpp
    #include <functional>

    BankAccount acc;

    // Binds the member function specifically to the 'acc' instance
    std::function<void(double)> depositRef = std::bind(&BankAccount::deposit, &acc, std::placeholders::_1);

    depositRef(200.0); // Directly modifies 'acc'
    ```
2. Lambda Capturing by Reference
    ```cpp
    BankAccount acc;

    // Lambda aliases the function call bound to 'acc'
    auto depositRef = [&acc](double amount) {
        acc.deposit(amount);
    };

    depositRef(200.0); // Invokes acc.deposit(200.0)
    ```