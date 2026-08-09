# Namespaces
## The Problem — Name Collisions in Large Programs
In a small program with one file, every function and variable name is unique. As programs grow across multiple files, multiple libraries, and multiple developers, the same name gets used for different things.

```cpp
// file: geometry.h
double area(double radius) {
    return 3.14159 * radius * radius;   // area of a circle
}

// file: agriculture.h
double area(double hectares) {
    return hectares * 10000;   // area in square meters
}

// file: main.cpp
#include "geometry.h"
#include "agriculture.h"

area(5.0);   // COMPILE ERROR: which area? ambiguous — two definitions
```
Both libraries define `area`. Neither knows about the other. The compiler cannot distinguish them. This is called a **name collision**.

Before namespaces, the only defense was long, descriptive prefixes:

```cpp
double geometry_area(double radius) { ... }
double agriculture_area(double hectares) { ... }
```

This works but is verbose, inconsistent, and not enforced by the compiler. Any developer can forget the prefix. Namespaces solve the collision problem at the language level.

## What a Namespace Is

A namespace is a named scope that groups related declarations together and prevents their names from colliding with names in other scopes.

```cpp
namespace Geometry {
    double area(double radius) {
        return 3.14159 * radius * radius;
    }

    double perimeter(double radius) {
        return 2 * 3.14159 * radius;
    }
}

namespace Agriculture {
    double area(double hectares) {
        return hectares * 10000;
    }
}

Geometry::area(5.0);       // unambiguous — Geometry's area
Agriculture::area(2.5);    // unambiguous — Agriculture's area
```

`::` is the scope resolution operator. `Geometry::area` means "the name `area` inside the `Geometry` namespace."

## Declaring and Defining Namespace Members

### Everything Inside the Namespace Block

The simplest form — declare and define everything inside the namespace:

```cpp
namespace BankSystem {

    class BankAccount {
        double balance;
    public:
        BankAccount(double b) : balance(b) {}
        void deposit(double amount);
    };

    void BankAccount::deposit(double amount) {
        balance += amount;
    }

    double calculateInterest(double principal, double rate) {
        return principal * rate;
    }
}
```

### Declare in Namespace, Define Outside

A function can be declared inside the namespace and defined outside it using the namespace prefix:

```cpp
namespace BankSystem {
    void processTransaction(double amount);  // declaration
}

// Definition outside — must use the namespace prefix
void BankSystem::processTransaction(double amount) {
    // ...
}
```

### Namespace Across Multiple Files

A namespace does not have to be defined in one place. The same namespace can be opened and extended across multiple files. All declarations across all files become part of the same namespace.

```cpp
// accounts.h
namespace BankSystem {
    class BankAccount { /* ... */ };
}

// transactions.h
namespace BankSystem {           // same namespace — extends it
    void processTransaction(double amount);
}

// main.cpp
#include "accounts.h"
#include "transactions.h"

BankSystem::BankAccount acc(1000.0);     // from accounts.h
BankSystem::processTransaction(100.0);   // from transactions.h
```

Both declarations belong to `BankSystem`. The namespace was opened twice across two files — this is intentional and correct.


## Accessing Names in a Namespace

### Fully Qualified Name — `Namespace::name`

The explicit, unambiguous form. Always works. Always clear.

```cpp
Geometry::area(5.0);
std::cout << "hello";
std::vector<int> v;
```

`std::cout` and `std::vector` use this form — `std` is the namespace for the entire C++ standard library.

### `using` Declaration — Import One Name

A `using` declaration brings one specific name from a namespace into the current scope. Only that name is affected.

```cpp
using Geometry::area;   // only 'area' is imported

area(5.0);              // VALID: area is now directly accessible
// perimeter(5.0);      // COMPILE ERROR: perimeter was not imported
```

**Inside a function — scoped import:**

```cpp
void computeCircle(double r) {
    using Geometry::area;       // only in scope inside this function
    using Geometry::perimeter;

    double a = area(r);
    double p = perimeter(r);
}

// area(5.0);   // COMPILE ERROR: using declaration gone — out of scope
```
### `using namespace` Directive — Import Everything

A `using namespace` directive brings all names from a namespace into the current scope at once.

```cpp
using namespace Geometry;

area(5.0);          // VALID
perimeter(5.0);     // VALID
```

**Why `using namespace std` in headers is a problem:**

A `using namespace` directive affects every file that includes the header. The programmer including the header did not choose to import all of `std` — the header forced it on them. This reintroduces the name collision problem that namespaces were designed to solve.

```cpp
// mylib.h
using namespace std;   // DANGEROUS in a header

// user_code.cpp
#include "mylib.h"
// now every std name is in scope — collisions become possible
// the user of mylib.h never asked for this
```

**Rule:**
- Never write `using namespace` in a header file
- In implementation files (`.cpp`), `using namespace` is acceptable but `using` declarations for specific names are preferred — they import only what is needed

## Nested Namespaces

Namespaces can be nested inside other namespaces to create a hierarchy:

```cpp
namespace Bank {
    namespace Account {
        class Savings { /* ... */ };
        class Checking { /* ... */ };
    }

    namespace Transaction {
        void process(double amount) { /* ... */ }
    }
}

Bank::Account::Savings s;
Bank::Transaction::process(100.0);
```
**C++17 nested namespace shorthand:**

```cpp
// Before C++17:
namespace Bank {
    namespace Account {
        namespace Premium {
            class GoldAccount { /* ... */ };
        }
    }
}

// C++17 and later:
namespace Bank::Account::Premium {
    class GoldAccount { /* ... */ };
}
```

Both forms are identical. The C++17 form removes the indentation nesting for deeply hierarchical namespaces.

## Anonymous Namespace — File-Private Names

An anonymous namespace has no name. Everything declared inside it has **internal linkage** — it is private to the translation unit (`.cpp` file) it is defined in. Other files cannot access it, and the linker treats each file's anonymous namespace as independent.

```cpp
// file: processing.cpp

namespace {
    // These names are private to processing.cpp
    // No other file can access them
    int helperCounter = 0;

    void helperFunction() {
        helperCounter++;
    }
}

void publicFunction() {
    helperFunction();   // VALID: same file
}
```

**Anonymous namespace vs `static`:**

In C, `static` at file scope gave internal linkage to a name. In C++, the anonymous namespace is the preferred equivalent. Both prevent the name from being visible outside the translation unit.

```cpp
// C style — still valid in C++
static int filePrivate = 0;

// C++ preferred style
namespace {
    int filePrivate = 0;
}
```

The anonymous namespace is preferred because it works for all declarations including class definitions — `static` cannot be applied to a class.

## Inline Namespace (C++11)

An inline namespace makes its contents visible in the enclosing namespace without qualification, while still allowing the full qualified name.

Its primary use is **library versioning** — allowing multiple versions of an API to coexist while the default version is accessible without specifying a version.

```cpp
namespace BankSystem {

    inline namespace v2 {
        void processTransaction(double amount) {
            // new implementation
        }
    }

    namespace v1 {
        void processTransaction(double amount) {
            // old implementation
        }
    }
}

BankSystem::processTransaction(100.0);    // uses v2 — inline version
BankSystem::v2::processTransaction(100.0); // explicit v2
BankSystem::v1::processTransaction(100.0); // explicit v1 — old behavior
```

Callers using the unqualified `BankSystem::processTransaction` automatically get the current default version. Code that explicitly requests `v1` continues to work without modification.

## Namespace Aliases

A long or deeply nested namespace name can be aliased to a shorter name:

```cpp
namespace BSAP = Bank::System::Account::Premium;

BSAP::GoldAccount g;   // same as Bank::System::Account::Premium::GoldAccount
```

Aliases are particularly useful when working with third-party library namespaces that have verbose hierarchical names. The alias only affects the current scope — it does not rename the original namespace.

## Argument-Dependent Lookup (ADL)

ADL (also called Koenig lookup) is a name lookup rule that allows the compiler to find a function in a namespace without an explicit namespace prefix, based on the types of the function's arguments.

When a function is called without a namespace qualifier, the compiler searches not only the current scope but also **the namespaces associated with the argument types**.

```cpp
namespace Geometry {
    struct Point { double x; double y; };

    void print(const Point& p) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }
}

Geometry::Point p{1.0, 2.0};
print(p);   // VALID via ADL: p is of type Geometry::Point
            // compiler searches Geometry namespace for 'print'
            // finds Geometry::print — no explicit prefix needed
```

**Why ADL exists — operator overloading:**

ADL is the mechanism that makes `std::cout << value` work for user-defined types. The `<<` operator for a class `MyClass` is typically defined in the same namespace as `MyClass`. When the compiler sees `std::cout << obj`, it searches both the `std` namespace (for the stream) and the namespace of `obj`'s type. It finds the correct `operator<<` without any explicit namespace prefix.

```cpp
namespace Graphics {
    struct Color { int r, g, b; };

    std::ostream& operator<<(std::ostream& os, const Color& c) {
        return os << "rgb(" << c.r << "," << c.g << "," << c.b << ")";
    }
}

Graphics::Color red{255, 0, 0};
std::cout << red;   // ADL finds Graphics::operator<< automatically
```

**ADL and `std::swap`:**

ADL is also the reason the correct pattern for calling `swap` in generic code is:

```cpp
using std::swap;
swap(a, b);
```

The `using std::swap` makes the standard `swap` a candidate. The unqualified `swap(a, b)` call also triggers ADL — if `a` and `b` have a custom `swap` in their namespace, it is found and preferred over the standard version.

## The Global Namespace

Code written outside any namespace lives in the **global namespace**. Its names are accessible from anywhere without a prefix. When a name collision exists between a local scope and the global namespace, the `::` prefix with no left side explicitly refers to the global namespace:

```cpp
int value = 100;   // global namespace

void process() {
    int value = 5;         // local variable — shadows global

    std::cout << value;    // local value: 5
    std::cout << ::value;  // global value: 100 — :: with nothing on left
}
```

`::value` means "the name `value` in the global namespace."

## `std` — The Standard Library Namespace

The entire C++ standard library lives in the `std` namespace. Every standard type, function, and object is accessed as `std::name`.

```cpp
std::cout          // standard output stream
std::string        // string type
std::vector<int>   // dynamic array
std::sort(...)     // sorting algorithm
std::move(...)     // cast to xvalue
std::swap(...)     // value exchange
```

This is why `using namespace std;` in headers is dangerous — it imports hundreds of names into global scope. Any user-defined name that happens to match a standard library name silently collides.

```cpp
// Inside the C++ Standard Library header <iostream>
namespace std {
    // Declaration of the standard output stream object
    extern ostream cout; 
    
    // Declaration of the standard input stream object
    extern istream cin; 
}

// inside <vector>
namespace std {
    template <typename T, typename Allocator = allocator<T>>
    class vector {
    private:
        T* _data;
        size_t _size;
        size_t _capacity;
    public:
        // Constructors, destructors, methods like push_back, size, etc.
        void push_back(const T& value);
        // ...
    };
}

// inside <swap>
namespace std {
    template <typename T>
    void swap(T& a, T& b) noexcept(/* conditions */) {
        T temp = static_cast<T&&>(a);
        a = static_cast<T&&>(b);
        b = static_cast<T&&>(temp);
    }
}
```
