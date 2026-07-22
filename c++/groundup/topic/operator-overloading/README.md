# Operator Overloading and Assignment Operators

## The Problem — Operators Only Work on Built-in Types
C++ operators like `+`, `-`, `*`, `=`, `==`, `<<` work on primitive types by definition. When a user-defined class is introduced, these operators have no meaning for it. The compiler does not know what `+` means for a `BankAccount` or what `=` means when copying one `DataBuffer` to another.

```cpp
DataBuffer a(5);
DataBuffer b(10);

DataBuffer c = a;   // what does this mean for DataBuffer?
                    // the compiler does not know — no rule exists yet
```

Without a mechanism to define operator behavior for classes, every operation on a class object requires a named function call:

```cpp
DataBuffer c = copyBuffer(a);
addBuffers(a, b);
printBuffer(a);
```

This is verbose, inconsistent with how primitive types work, and breaks the goal of making user-defined types behave like built-in types.

## What Operator Overloading Is
Operator overloading is the mechanism that allows operators to be defined for user-defined types. The compiler translates every operator expression into a function call. Defining that function gives the operator its meaning for the class.

**The compiler translation:**

Every operator expression has a function form:
```cpp
a + b          →    operator+(a, b)          // non-member form
a + b          →    a.operator+(b)           // member form
a = b          →    a.operator=(b)           // member form
a[i]           →    a.operator[](i)          // member form
++a            →    a.operator++()           // prefix, member form
a++            →    a.operator++(0)          // postfix, dummy int argument
std::cout << a →    operator<<(std::cout, a) // non-member form
```
The function is named with the `operator` keyword followed by the symbol. Defining this function gives the operator its behavior for the class.

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    // Define what + means for Temperature
    Temperature operator+(const Temperature& other) const {
        return Temperature{celsius + other.celsius};
    }
};

Temperature a(20.0);
Temperature b(5.0);
Temperature c = a + b;   // compiler calls a.operator+(b)
                          // c.celsius = 25.0
```

## What Can and Cannot Be Overloaded
### Operators That Can Be Overloaded

Operators that work on values (+, -, ==, []). If overload performed architectural and design of language is not violated

* Arithmetic: +, -, *, /, %
* Assignment: =, +=, -=, *=, /=, %=, <<=, >>=, &=, ^=, |=
* Increment / Decrement: ++, --
* Relational & Comparison: ==, !=, <, >, <=, >=, <=> (C++20 spaceship operator)
* Logical: !, &&, ||
* Bitwise: ~, &, |, ^, <<, >>
* Subscript & Member Access: [], ->, * (dereference), ->*
* Function Call & Conversion: (), type conversion operators (e.g., operator int())
* Memory Management: new, new[], delete, delete[]
* Other: , (comma), "" (user-defined literal)

### Operators That Cannot Be Overloaded

Operators that the compiler relies on to find code, measure memory, or control basic syntax (., ::, sizeof, ?:). if overload supported this architectural and design of language is violated. example if sizeof is allowed, would provide a wrong size compiler depend on this to allocate memory, may mess up the things

* Scope Resolution: ::
* Direct Member Selection: .
* Member Pointer Selector: .*
* Ternary Conditional: ?:
* Size Query: sizeof
* Type Information: typeid
* Compile-time Constant Evaluation: requires (C++20)

### Fundamental Rules
1. **Cannot create new operators.** Only existing C++ operator symbols can be overloaded. `**` for exponentiation does not exist as an operator.
2. **Cannot change arity.** Unary operators remain unary. Binary operators remain binary. The number of operands is fixed by the operator.
3. **Cannot change precedence or associativity.** `a + b * c` always evaluates `*` before `+`, regardless of what `+` and `*` do for the type.
4. **At least one operand must be a user-defined type.** Overloading  `int + int` is prohibited — built-in behavior cannot be replaced.

## Return Type Conventions
The return type of an overloaded operator determines whether results can be chained and whether they behave consistently with built-in operators. 

### chainable
```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    Temperature operator+(const Temperature& other) const {
        return Temperature{celsius + other.celsius};
    }
};

Temperature a(20.0), b(5.0), c (10.5);
a+b+c; // it is valid
```

### Non chainable
```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    void operator+(const Temperature& other) const {
        Temperature tmp = Temperature{celsius + other.celsius};
        std::cout<<tmp.celsius<<"\n";
    }
};

Temperature a(20.0), b(5.0), c (10.5);
a+b+c; // it is not-valid
a+b; // valid but won't return
```

## Member Function vs Non-Member Function
Every overloaded operator can be a member function of the class or a non-member function defined outside it. The choice is not stylistic — it determines which operand combinations are valid.

### Member function — left operand is `this`:
The left operand is the implicit object (`this`). The right operand is the explicit parameter.

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    Temperature operator+(const Temperature& other) const {
        return Temperature{celsius + other.celsius};
    }
};

Temperature a(20.0), b(5.0);
a + b;   // translates to: a.operator+(b)
         // left operand = *this = a
         // right operand = other = b
```

Because the left operand is always `*this`, the left operand must always be the class type. The compiler looks up `operator+` on the left operand's type. If the left operand is not a `Temperature`, no member function is found.

```cpp
Temperature a(20.0);
a + 5.0;    // VALID: translates to a.operator+(Temperature{5.0})
            // if a converting constructor exists
5.0 + a;    // COMPILE ERROR: 5.0 is a double — double has no operator+(Temperature)
            // cannot call a.operator+ because the left operand is not Temperature
```

### Non-Member Function Form

Both operands are explicit parameters. Neither is bound to `this`.


```cpp
Temperature operator+(const Temperature& lhs, const Temperature& rhs) {
    return Temperature{lhs.celsius + rhs.celsius};
}

Temperature a(20.0), b(5.0);
a + b;      // translates to: operator+(a, b)
5.0 + a;    // translates to: operator+(5.0, a)
            // VALID if a converting constructor exists for double → Temperature
```

Because both operands are explicit, either side can be the class type. The compiler searches both member functions of the left operand and non-member functions in scope. This solves the symmetry problem — operators like `+`, `==`, and `<<` work regardless of which side the class object appears on.

**This works only for  Non-explicit constructor**

1. **Implicit Constructor (Without explicit)**
   ```cpp
   Temperature(double c) : celsius(c) {} // Non-explicit
   ```
   * Expression: `5.0 + a`
   * Compiler Action: The compiler sees that `operator+(const Temperature&, const Temperature&)` expects a `Temperature` as its left argument. Because the constructor allows implicit conversions, the compiler silently constructs a temporary `Temperature` object from `5.0` (i.e., `Temperature{5.0}`).
2. **Explicit Constructor (With explicit)**
   ```cpp
   explicit Temperature(double c) : celsius(c) {} // Explicit
   ```
   * Expression: `5.0 + a`
   * Compiler Action: The compiler attempts to find a matching function for `double` + `Temperature` because implicit conversions are strictly forbidden..
     * It looks for `operator+(double, Temperature)`. None exists.
   
   **Solution**
   1. Cast the double at the call site so the compiler doesn't have to guess:
   ```cpp
   auto result = Temperature{5.0} + a; // Explicit conversion works fine
   ```
   2. Provide Overloads for double
   ```cpp
    // Temperature + double
    Temperature operator+(const Temperature& lhs, double rhs) {
        return Temperature{lhs.getCelsius() + rhs};
    }

    // double + Temperature
    Temperature operator+(double lhs, const Temperature& rhs) {
        return Temperature{lhs + rhs.getCelsius()};
    }
   ```
**Accessing Private Members From a Non-Member Function**
A non-member function has no `this` pointer and is not a member of the class. It cannot access `private` members directly:
```cpp
Temperature operator+(const Temperature& lhs, const Temperature& rhs) {
    return Temperature{lhs.celsius + rhs.celsius};
    //                    ↑
    // COMPILE ERROR: celsius is private — non-member cannot access it
}
```

**Two solutions:**
1. **Solution 1 — `friend` declaration:**
    The class grants the non-member function access to its private members:
    ```cpp
    class Temperature {
        double celsius;
    public:
        Temperature(double c) : celsius(c) {}

        friend Temperature operator+(const Temperature& lhs, const Temperature& rhs);
    };

    Temperature operator+(const Temperature& lhs, const Temperature& rhs) {
        return Temperature{lhs.celsius + rhs.celsius};   // VALID: friend access
    }
    ```
2. **Solution 2 — Use public accessors:**
   If the class provides a public getter, no `friend` is needed:

    ```cpp
    class Temperature {
        double celsius;
    public:
        Temperature(double c) : celsius(c) {}
        double getCelsius() const { return celsius; }
    };

    Temperature operator+(const Temperature& lhs, const Temperature& rhs) {
        return Temperature{lhs.getCelsius() + rhs.getCelsius()};   // VALID: public access
    }
    ```
`friend` is the conventional choice for operators because it keeps the operator implementation clean and avoids exposing internal data through public getters that exist solely to serve the operator.

## The Symmetry Problem — Why Some Operators Must Be Non-Members
This is the core reason the distinction matters.
Consider a `Scalar` class with a member `operator*`:
```cpp
class Scalar {
    double value;
public:
    Scalar(double v) : value(v) {}

    Scalar operator*(double factor) const {
        return Scalar{value * factor};
    }
};

Scalar s(3.0);
Scalar r1 = s * 2.0;    // VALID: translates to s.operator*(2.0)
Scalar r2 = 2.0 * s;    // COMPILE ERROR: 2.0 is a double
                          // double has no operator*(Scalar)
                          // member function requires Scalar on the left
```

`s * 2.0` works. `2.0 * s` does not. This is inconsistent. For mathematical types, both forms must work identically.
Non-member `friend` functions solve this by providing both forms explicitly:

```cpp
class Scalar {
    double value;
public:
    explicit Scalar(double v) : value(v) {}

    Scalar operator*(double factor); // s * 2.0 // this is member function, it can access this
    // or it is possible to write as non member function
    // friend Scalar operator*(const Scalar& s, double factor); //  s * 2.0

    // this must be a non member function
    friend Scalar operator*(double factor, const Scalar& s); // 2.0 * s
};

Scalar operator*(const Scalar& s, double factor) {
    return Scalar{s.value * factor};
}

Scalar operator*(double factor, const Scalar& s) {
    return s * factor;   // reuse the first form
}

Scalar s(3.0);
s * 2.0;    // VALID: operator*(s, 2.0)
2.0 * s;    // VALID: operator*(2.0, s)
```

### Operators That Must Be Member Functions
The C++ standard requires four operators to always be member functions. Defining them as non-members is a compile error:
| Operator | Why it must be a member |
|---|---|
| `operator=` | Assignment must operate on `this` — the object being assigned to |
| `operator[]` | Subscript must know which object is being indexed |
| `operator()` | Function call must know which object is being called |
| `operator->` | Arrow must know which object to dereference |

the reason is `a = ;`, `[1]a`;, `()func` `a->this` all of this are invalid

```cpp
class Container {
public:
    int& operator[](int i);         // must be member
    void operator()();              // must be member
    Container& operator=(const Container&);   // must be member
};
```

### Operators That Should Be Non-Member Functions
Operators where either operand could be on the left should be non-members to support both orderings:

| Operator category | Examples | Reason for non-member |
| Stream | `<<`, `>>` | Left operand is `std::ostream`/`std::istream` — cannot be a member of those |


`<<` and `>>` are a special case: the left operand is always a stream object (`std::ostream`, `std::istream`). Since these are standard library classes whose member functions cannot be modified, stream operators must always be non-member functions.


```cpp
class Temperature {
    double celsius;
public:
    friend std::ostream& operator<<(std::ostream& os, const Temperature& t);
};

std::ostream& operator<<(std::ostream& os, const Temperature& t) {
    os << t.celsius << "°C";
    return os;   // return the stream — enables chaining: cout << a << b
}

Temperature t(100.0);
std::cout << t << "\n";   // operator<<(operator<<(std::cout, t), "\n")
```