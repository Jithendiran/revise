## Class - Struct
In the C++ programming language, the keywords `class` and `struct` serve the exact same foundational purpose: they define the blueprint for an object, specifying both the data fields and the functions that manipulate that data.

### Historical Necessity: Backward Compatibility
The simultaneous existence of both keywords is a result of historical design requirements rather than technical necessity.
* The C Legacy: Before the creation of C++, the C language utilized `struct` solely as a passive container to group related data variables. It could not contain functions or enforce access restrictions.
* The Migration Strategy: When C++ was developed as an extension of C, a primary goal was absolute backward compatibility. Millions of lines of legacy C code relied on the existing behavior of `struct`.
* The Language Decision: Instead of altering or replacing the traditional `struct` keyword—which would break existing programs—the language designers retained `struct` but upgraded its capabilities to match the newly introduced `class` keyword. Consequently, both keywords were granted the ability to contain functions, implement inheritance, and utilize access modifiers.

### The Sole Technical Difference: Default Access Control
From a technical compilation standpoint, `class` and `struct` are identical with exactly one exception: the default access modifier applied when `PUBLIC` or `PRIVATE` labels are omitted.
* `struct` Default: When variables or functions are declared inside a `struct` without an explicit access label, the compiler treats them as `PUBLIC`. They are entirely accessible and modifiable by any external function in the program.
* `class` Default: When variables or functions are declared inside a `class` without an explicit access label, the compiler treats them as `PRIVATE`. They are instantly locked behind a protective barrier and can only be accessed by functions defined inside that specific `class`.

## Standard Usage Conventions: When to Use What
Because the technical differences are minimal, the choice between `class` and `struct` in modern software development is governed by professional convention to communicate architectural intent.
### When to Use struct (Plain Old Data)
A `struct` is selected for Plain Old Data (POD). It acts as a passive container where variables are independent, meaning any combination of values is valid. Direct external modification carries no risk of breaking the structure.
```cpp
struct Point {
    double x;
    double y;
};
```
Data Independence: If `x` is `5` and `y` is `-1000`, the data remains valid. No rules dictate a required relationship between `x` and `y`.

### When to Use class (Domain Entities)
A `class` is selected for components where the data is interdependent or bound by strict logical rules. If external code modifies these variables directly, the object can enter an invalid, corrupted state.
```cpp
class RationalNumber {
private:
    int numerator;
    int denominator; // Invariant: Must never be 0, and must remain reduced.

public:
    RationalNumber(int num, int den) {
        if (den == 0) throw std::invalid_argument("Denominator cannot be zero.");
        int g = std::gcd(num, den);
        numerator = num / g;
        denominator = den / g;
    }

    void setDenominator(int den) {
        if (den == 0) throw std::invalid_argument("Denominator cannot be zero.");
        // Logic to re-reduce the fraction occurs here
    }
};
```
Data Interdependence: The fields `numerator` and `denominator` must maintain a precise mathematical relationship. Setting `denominator = 0` directly would corrupt the object state and cause runtime errors.