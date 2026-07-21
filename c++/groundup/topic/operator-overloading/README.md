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