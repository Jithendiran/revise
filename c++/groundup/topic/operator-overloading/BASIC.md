# Operator Overloading — Full System

## What Was Already Established

The [Overload](./README.md) was covered in the Assignment Operators document:
- What operator overloading is — compiler translates `a op b` to a function call named `operator op`
- What can and cannot be overloaded
- Member vs non-member function form — when each is required
- `friend` for non-member operators that need private access
- `operator=` copy assignment — full coverage
- `operator=` move assignment — full coverage

This document covers every remaining operator category. The same rules about member vs non-member, `friend`, and return types apply throughout.

## Return Type Conventions — Complete Reference

Before individual operators are covered, the return type rule for each category must be established. Returning the wrong type breaks chaining, assignment semantics, or lvalue requirements.

| Operator category | Return type | Reason |
|---|---|---|
| Arithmetic `+` `-` `*` `/` `%` | `T` by value | New temporary — not a modification of either operand |
| Compound assignment `+=` `-=` etc. | `T&` | Returns modified self — enables chaining |
| Prefix `++` `--` | `T&` | Returns modified self — `++a` IS `a` after increment |
| Postfix `++` `--` | `T` by value | Returns old value — cannot return reference to temporary |
| Comparison `==` `!=` `<` etc. | `bool` | Result of comparison is true or false |
| Spaceship `<=>` | ordering type | Covered in C++20 document |
| Stream `<<` `>>` | `std::ostream&` / `std::istream&` | Enables chaining: `cout << a << b` |
| Subscript `[]` non-const | `T&` | Must be assignable: `arr[i] = value` |
| Subscript `[]` const | `const T&` | Read-only access from const objects |
| Function call `()` | Any — purpose-dependent | No fixed convention |
| Dereference `*` | `T&` | Must refer to the pointed-at object |
| Arrow `->` | `T*` | Compiler applies `->member` to the returned pointer |
| Type conversion | The target type | Produces the converted value |


## Arithmetic Operators — `+`, `-`, `*`, `/`, `%`

### The Canonical Pattern — Implement `+=`, Derive `+`

Arithmetic operators produce a new value without modifying either operand. The standard pattern implements the compound assignment operator first (as a member), then derives the arithmetic operator from it (as a non-member). This keeps the logic in one place.

```cpp
class Vector2D {
    double x, y;
public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Step 1: implement += as member — modifies self, returns self
    Vector2D& operator+=(const Vector2D& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    // Step 2: derive + from += as non-member friend
    // lhs is taken BY VALUE — it becomes the working copy
    friend Vector2D operator+(Vector2D lhs, const Vector2D& rhs) {
        lhs += rhs;   // reuses += — no logic duplication
        return lhs;   // return the modified copy
    }
};

Vector2D a{1.0, 2.0}, b{3.0, 4.0};
Vector2D c = a + b;   // a and b unchanged, c = {4.0, 6.0}
a += b;               // a modified to {4.0, 6.0}
```

**Why `lhs` is passed by value in `operator+`:**

The function must return a new object without modifying `a` or `b`. Taking `lhs` by value creates a local copy. `+=` is applied to the copy. The copy is returned. The original `a` is untouched.

**Deriving all five arithmetic operators:**

```cpp
friend Vector2D operator-(Vector2D lhs, const Vector2D& rhs) {
    return lhs -= rhs;
}

friend Vector2D operator*(Vector2D lhs, double scalar) {
    return lhs *= scalar;
}

// Handles: scalar * vector (built-in double on the left)
friend Vector2D operator*(double scalar, const Vector2D& rhs) {
    return rhs * scalar;   // reuse the above
}

friend Vector2D operator/(Vector2D lhs, double scalar) {
    return lhs /= scalar;
}
```

## Compound Assignment Operators — `+=`, `-=`, `*=`, `/=`, `%=`

Compound assignment operators modify the left operand in place and return a reference to it. They **must be member functions** — they modify `this`.

```cpp
class Vector2D {
    double x, y;
public:
    Vector2D& operator+=(const Vector2D& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};
```

**Why return `T&` and not `void`:**

Returning `T&` enables compound assignment chaining:

```cpp
Vector2D a, b, c;
(a += b) += c;   // a += b returns a&, then a += c applies
```

Returning `void` would break this. Returning `T` by value would create a temporary — `(a += b) += c` would modify the temporary, not `a`.

## Relational Operators — `==`, `!=`, `<`, `>`, `<=`, `>=`

### The Minimal Implementation Strategy

Define `operator==` and `operator<` with actual logic. Derive everything else from these two. No logic is duplicated.

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    // Primary equality — actual logic here
    friend bool operator==(const Temperature& lhs, const Temperature& rhs) {
        return lhs.celsius == rhs.celsius;
    }

    // Derived from ==
    friend bool operator!=(const Temperature& lhs, const Temperature& rhs) {
        return !(lhs == rhs);
    }

    // Primary ordering — actual logic here
    friend bool operator<(const Temperature& lhs, const Temperature& rhs) {
        return lhs.celsius < rhs.celsius;
    }

    // All derived from 
    friend bool operator>(const Temperature& lhs, const Temperature& rhs) {
        return rhs < lhs;             // a > b  ≡  b < a
    }

    friend bool operator<=(const Temperature& lhs, const Temperature& rhs) {
        return !(rhs < lhs);          // a <= b  ≡  NOT (b < a)
    }

    friend bool operator>=(const Temperature& lhs, const Temperature& rhs) {
        return !(lhs < rhs);          // a >= b  ≡  NOT (a < b)
    }
};
```

**In C++20:** Define `operator<=>` and `operator==` only. The compiler generates all six automatically. Covered in the C++20 operators document.

### Floating-Point Equality Warning

```cpp
double a = 0.1 + 0.2;
a == 0.3   // likely false — IEEE 754 representation error
           // 0.1 + 0.2 = 0.30000000000000004 in binary floating-point
```

For types containing `double` or `float` members, direct `==` comparison is unreliable. Use an epsilon comparison:

```cpp
friend bool operator==(const Temperature& lhs, const Temperature& rhs) {
    return std::abs(lhs.celsius - rhs.celsius) < 1e-9;
}
```

## Increment and Decrement — `++` and `--`

Prefix and postfix forms have different signatures. The compiler distinguishes them by a dummy `int` parameter in the postfix form.

### Prefix — Increment First, Return Modified Object

```cpp
class Counter {
    int value;
public:
    Counter(int v = 0) : value(v) {}

    // Prefix ++: increment, return reference to self
    Counter& operator++() {
        ++value;
        return *this;
    }

    // Prefix --:
    Counter& operator--() {
        --value;
        return *this;
    }
};

Counter c(5);
Counter& ref = ++c;   // c.value = 6, ref refers to c
```

### Postfix — Save Old Value, Increment, Return Old Value

```cpp
class Counter {
    int value;
public:
    // Postfix ++: dummy int parameter distinguishes from prefix
    Counter operator++(int) {
        Counter old = *this;   // save current state
        ++value;               // modify
        return old;            // return saved state — by value, not reference
    }

    Counter operator--(int) {
        Counter old = *this;
        --value;
        return old;
    }
};

Counter c(5);
Counter old = c++;   // old.value = 5, c.value = 6
```

**Why postfix cannot return a reference:**

The old value is stored in a local variable `old`. Returning a reference to a local variable is undefined behavior — the local is destroyed when the function returns. Postfix must return by value.

**Performance — prefer prefix over postfix:**

Prefix: one operation — modify and return.
Postfix: three operations — copy, modify, return copy.

For complex types, the extra copy in postfix is measurable overhead. Use prefix (`++i`) unless the old value is explicitly needed.

## Unary Operators — `-`, `+`, `!`, `~`

Unary operators take one operand — the object itself. They are member functions with no parameters and typically return a new value without modifying the object.

```cpp
class Vector2D {
    double x, y;
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    // Unary minus — returns negated copy, does not modify self
    Vector2D operator-() const {
        return Vector2D{-x, -y};
    }

    // Unary plus — returns unchanged copy (rarely useful)
    Vector2D operator+() const {
        return *this;
    }
};

Vector2D a{3.0, -4.0};
Vector2D b = -a;    // b = {-3.0, 4.0}, a unchanged
```

**Logical NOT — typically returns `bool`:**

```cpp
class FileHandle {
    bool open;
public:
    FileHandle() : open(false) {}

    bool operator!() const {
        return !open;   // true means "not open"
    }
};

FileHandle fh;
if (!fh) { fh.openFile(); }
```

**Bitwise NOT — for types representing bit patterns:**

```cpp
class BitMask {
    unsigned int bits;
public:
    BitMask(unsigned int b) : bits(b) {}

    BitMask operator~() const {
        return BitMask{~bits};
    }
};
```

## Stream Operators — `<<` and `>>`

Stream operators connect user-defined types to the C++ I/O system. They **must be non-member functions** — the left operand is always a stream object (`std::ostream`, `std::istream`), which is a standard library class whose member functions cannot be modified.

### Output Operator `<<`

```cpp
class Vector2D {
    double x, y;
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;   // return the stream — REQUIRED for chaining
    }
};

Vector2D a{1.0, 2.0}, b{3.0, 4.0};
std::cout << a << " and " << b << "\n";
// Output: (1, 2) and (3, 4)
```

**Why return `std::ostream&`:**

`std::cout << a << b` is parsed as `(std::cout << a) << b`. The expression `std::cout << a` must evaluate to `std::cout` (the stream) so that `<< b` can be applied to it. Returning `os` by reference achieves this.

**The second parameter is `const T&`:**

The stream operator reads the object. It does not modify it. `const` prevents accidental modification and allows printing `const` objects.

### Input Operator `>>`

```cpp
friend std::istream& operator>>(std::istream& is, Vector2D& v) {
    is >> v.x >> v.y;

    if (!is) {
        v = Vector2D{0.0, 0.0};   // reset to valid state on failure
    }

    return is;
}

Vector2D v;
std::cin >> v;   // reads two doubles from standard input
```

**The second parameter is non-const reference:**

Input fills the object — modification is required. The parameter cannot be `const`.

**Check stream state after extraction:**

`is >> v.x` may fail if the input is not a valid double. Checking `!is` after extraction and resetting to a valid default prevents a partially initialized object from propagating.

## Subscript Operator — `[]`

The subscript operator provides index-based element access. It **must be a member function**. Two overloads are always required — one for mutable objects, one for `const` objects.

```cpp
class IntArray {
    int*        data;
    std::size_t size;

public:
    IntArray(std::size_t n) : size(n), data(new int[n]{}) {}
    ~IntArray() { delete[] data; }

    // Non-const: returns reference — allows assignment arr[i] = value
    int& operator[](std::size_t i) {
        return data[i];
    }

    // Const: returns const reference — read-only access from const objects
    const int& operator[](std::size_t i) const {
        return data[i];
    }
};

IntArray arr(5);
arr[2] = 10;               // non-const version — returns int&

const IntArray cArr(5);
int val = cArr[2];         // const version — returns const int&
// cArr[2] = 10;           // COMPILE ERROR: const int& is not assignable
```

**Why two overloads are necessary:**

```cpp
void readOnly(const IntArray& arr) {
    int v = arr[2];         // calls const version — required
    // arr[2] = 5;          // COMPILE ERROR: const version returns const int&
}

void mutable(IntArray& arr) {
    arr[2] = 5;             // calls non-const version — returns int&
}
```

Without the const overload, `const IntArray` objects cannot use `[]` at all — the non-const version requires a non-const `this`.

**Bounds checking:**

```cpp
int& operator[](std::size_t i) {
    if (i >= size) throw std::out_of_range("index out of bounds");
    return data[i];
}
```

Standard convention: `operator[]` does not bounds-check (matches built-in array behavior). A named function like `at()` is the conventional location for bounds-checked access — this is the pattern `std::vector` uses.


## Function Call Operator — `()`

Overloading `operator()` makes a class instance **callable** — it can be used with `()` syntax as if it were a function. A class with `operator()` is called a **functor** or **function object**.

```cpp
class Multiplier {
    double factor;
public:
    Multiplier(double f) : factor(f) {}

    double operator()(double value) const {
        return value * factor;
    }
};

Multiplier double_it(2.0);
double result = double_it(5.0);   // calls operator()(5.0) → 10.0
double result2 = double_it(7.0);  // calls operator()(7.0) → 14.0
```

**Why functors are useful over plain functions — they carry state:**

```cpp
class Threshold {
    double limit;
public:
    Threshold(double l) : limit(l) {}

    bool operator()(double value) const {
        return value > limit;
    }
};

Threshold above10(10.0);
above10(15.0);   // true — 15 > 10
above10(5.0);    // false — 5 not > 10
above10(20.0);   // true — limit persists across calls
```

A plain function cannot carry state without a global variable. A functor carries its state as member data — different instances carry different state.

**`operator()` can be overloaded with different signatures:**

```cpp
class Transform {
public:
    double operator()(double x) const         { return x * 2.0; }
    double operator()(double x, double y) const { return x + y; }
    std::string operator()(const std::string& s) const { return s + s; }
};

Transform t;
t(5.0);          // → 10.0
t(3.0, 4.0);     // → 7.0
t("hello");      // → "hellohello"
```

**Connection to lambdas:**

Every lambda expression is compiled into an anonymous class with `operator()`. The functor pattern is the foundational mechanism that lambda syntax automates. This connection is covered fully in the Lambda document.


## Dereference and Arrow Operators — `*` and `->`
These operators are overloaded to implement **smart pointers** and **iterators** — objects that behave like pointers but provide additional behavior such as resource management, bounds checking, or logging.

### Dereference Operator `*`

Returns a reference to the **object** being pointed at. Two overloads are required — one for mutable access, one for const access.
```cpp
class IntPtr {
    int* raw;
public:
    explicit IntPtr(int* p) : raw(p) {}
    ~IntPtr() { delete raw; }

    // Non-const: allows modification through the pointer
    int& operator*() {
        return *raw;
    }

    // Const: read-only access when IntPtr itself is const
    const int& operator*() const {
        return *raw;
    }
};
IntPtr p(new int(42));
int val = *p;    // calls operator*() → returns int& → reads 42
*p = 99;         // calls operator*() → returns int& → writes 99

const IntPtr cp(new int(5));
int v = *cp;     // calls const operator*() → const int& → read only
// *cp = 10;     // COMPILE ERROR: const int& cannot be assigned to
```
### Arrow Operator `->`

Returns a **pointer**. The compiler then automatically applies `->member` to whatever pointer is returned.

```cpp
struct Point { int x; int y; };

class PointPtr {
    Point* raw;
public:
    explicit PointPtr(Point* p) : raw(p) {}
    ~PointPtr() { delete raw; }

    Point* operator->() {
        return raw;   // returns raw pointer
    }

    const Point* operator->() const {
        return raw;
    }
};

PointPtr p(new Point{3, 4});
int x = p->x;    // Step 1: p.operator->() returns Point*
                  // Step 2: compiler applies ->x to Point*
                  // result: (*raw).x = 3

p->x = 10;       // same two steps — writes through the pointer
```
**Why `->` must return a pointer:**
When the compiler encounters `p->x`, it performs exactly two steps:
```
p->x
Step 1: call p.operator->() → must return a pointer
Step 2: apply ->x to that pointer → (*ptr).x
```

If step 1 returns a raw pointer, the compiler stops and resolves the member access directly. If step 1 returns a class type instead of a raw pointer, the compiler repeats step 1 on the returned object — this is **arrow proxy chaining**.

### Arrow Proxy Chaining

Arrow proxy chaining is the rule that when `operator->()` returns a class type rather than a raw pointer, the compiler keeps calling `operator->()` on the result until a raw pointer is reached.

**Why this exists:** Some designs need to intercept member access at multiple levels — logging access, reference counting at intermediate layers, or iterator adapters that wrap other iterators. Each layer defines its own `operator->()` that returns the next layer, until the innermost layer returns the actual raw pointer.
**Concrete example:**

```cpp
struct Point { int x; int y; };

// Inner layer — holds the actual raw pointer
// operator->() returns a raw pointer — this ends the chain
class PtrProxy {
    Point* ptr;
public:
    explicit PtrProxy(Point* p) : ptr(p) {}

    Point* operator->() {
        std::cout << "PtrProxy::operator->() called\n";
        return ptr;   // RAW POINTER — chain stops here
    }
};

// Outer layer — holds a PtrProxy, not a raw pointer
// operator->() returns a CLASS TYPE — chain continues
class SmartPtr {
    PtrProxy proxy;
public:
    explicit SmartPtr(Point* p) : proxy(PtrProxy{p}) {}

    PtrProxy operator->() {
        std::cout << "SmartPtr::operator->() called\n";
        return proxy;   // CLASS TYPE — chain continues
    }
};

Point pt{10, 20};
SmartPtr sp(&pt);

sp->x = 99;
```
**What the compiler does step by step:**
```
sp->x = 99

Step 1: call sp.operator->()
returns PtrProxy (a class type — not a raw pointer)
→ chain continues

Step 2: call proxy.operator->() (on the returned PtrProxy)
returns Point* (a raw pointer — chain stops)
→ base case reached

Step 3: apply ->x to Point*
→ (*ptr).x = 99
```
**Output when run:**
```
SmartPtr::operator->() called
PtrProxy::operator->() called
```

Two `operator->()` calls happened transparently for a single `sp->x` expression. The programmer wrote one `->`, the compiler generated two function calls.

**The chain rule — stated precisely:**
The chain terminates only when `operator->()` returns a raw pointer. If no class in the chain ever returns a raw pointer, the compiler reports an error — the recursion has no base case.

**Practical use case — logging wrapper:**

```cpp
struct BankAccount {
    double balance;
    void deposit(double amount) { balance += amount; }
};

class LoggingProxy {
    BankAccount* account;
public:
    explicit LoggingProxy(BankAccount* a) : account(a) {}

    BankAccount* operator->() {
        std::cout << "Accessing BankAccount member\n";
        return account;   // raw pointer — chain ends
    }
};

class AuditedPtr {
    LoggingProxy proxy;
public:
    explicit AuditedPtr(BankAccount* a) : proxy(LoggingProxy{a}) {}

    LoggingProxy operator->() {
        std::cout << "AuditedPtr intercepted\n";
        return proxy;   // class type — chain continues to LoggingProxy
    }
};

BankAccount acc{1000.0};
AuditedPtr ptr(&acc);

ptr->deposit(100.0);
// Output:
// AuditedPtr intercepted
// Accessing BankAccount member
// (deposit runs on acc)
```

Every member access through `AuditedPtr` passes through both layers of logging automatically. The calling code writes `ptr->deposit(100.0)` — exactly as if `ptr` were a raw pointer.

### Const Correctness for Both Operators

A const smart pointer should give read-only access to the pointed-at object. Two overloads of each operator cover both cases:

```cpp
class PointPtr {
    Point* raw;
public:
    explicit PointPtr(Point* p) : raw(p) {}

    // Non-const PointPtr: full access
    Point* operator->()             { return raw; }
    Point& operator*()              { return *raw; }

    // Const PointPtr: read-only access to pointed-at object
    const Point* operator->() const { return raw; }
    const Point& operator*()  const { return *raw; }
};

PointPtr p(new Point{3, 4});
p->x = 10;           // non-const operator-> — writable

const PointPtr cp(new Point{1, 2});
int v = cp->x;       // const operator-> — returns const Point* — readable
// cp->x = 5;        // COMPILE ERROR: const Point* member is read-only
```

## Type Conversion Operators
Type conversion operators define how a class object converts to another type — allowing the class object to appear wherever the target type is expected.
### Implicit Conversion Operator

```cpp
class Celsius {
    double degrees;
public:
    Celsius(double d) : degrees(d) {}

    operator double() const {
        return degrees;
    }
};

Celsius temp(100.0);
double d = temp;              // implicit conversion: calls operator double()
std::cout << temp + 0.5;      // Celsius implicitly converted to double
```
**Syntax:** No return type is written before the function name. The function name IS the return type.
### `explicit` Conversion Operator

Implicit conversions cause silent bugs. `explicit` prevents implicit conversion while allowing explicit casts and certain special contexts:

```cpp
class SmartBool {
    bool value;
public:
    SmartBool(bool b) : value(b) {}

    explicit operator bool() const {
        return value;
    }
};

SmartBool sb(true);
// bool b = sb;               // COMPILE ERROR: implicit conversion blocked
bool b = static_cast<bool>(sb);  // VALID: explicit cast
if (sb) { }                       // VALID: if() is an allowed implicit context
!sb;                              // VALID: logical operators are allowed contexts
```

`if`, `while`, `!`, `&&`, `||` are permitted to use `explicit operator bool()` implicitly. All other contexts require an explicit cast.

**Rule:** Mark all single-argument constructors and all conversion operators `explicit` by default. Remove `explicit` only when implicit conversion is a deliberate, documented design decision.

## `new` and `delete` Operators

Classes can override the global `new` and `delete` to control memory allocation for objects of that class — for memory pools, custom allocators, or allocation tracking.

### Overriding `operator new`

```cpp
class PoolObject {
public:
    static void* operator new(std::size_t size) {
        std::cout << "Custom new: " << size << " bytes\n";
        void* ptr = std::malloc(size);
        if (!ptr) throw std::bad_alloc();
        return ptr;
    }

    static void operator delete(void* ptr) noexcept {
        std::cout << "Custom delete\n";
        std::free(ptr);
    }
};

PoolObject* p = new PoolObject;   // custom operator new called
delete p;                          // custom operator delete called
```

**`static` is implicit:** Even without the keyword, `operator new` and `operator delete` are always static member functions. They do not receive a `this` pointer — the object does not exist yet when `new` runs, and has already been destroyed when `delete` runs.

### Array Versions

```cpp
class PoolObject {
public:
    static void* operator new[](std::size_t size) {
        return std::malloc(size);
    }

    static void operator delete[](void* ptr) noexcept {
        std::free(ptr);
    }
};

PoolObject* arr = new PoolObject[10];   // operator new[] called
delete[] arr;                            // operator delete[] called
```

`new[]` and `delete[]` must be paired. Mixing them with the non-array versions is undefined behavior.