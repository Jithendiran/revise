## Initialization

### Why Multiple Initialization Syntaxes Exist
C++ accumulated multiple initialization syntaxes over decades. Each one was introduced to solve a specific problem that existed at the time. Reading them without that history makes them look redundant. With it, each form has a clear, distinct reason for existing.

The order in this document follows that history — each syntax introduced as the direct answer to a limitation of the previous one.

### Default Initialization — No Initializer at All
```cpp
int x;                  // default initialization
BankAccount acc;        // default initialization
```

For primitive types (int, double, pointers): The compiler allocates the stack slot and does nothing else. Whatever bit pattern existed at that address becomes the variable's value. This is the garbage value.

```cpp
int x;
std::cout << x;   // UNDEFINED — could print anything
```

For class types: The default constructor runs. If a default constructor exists (either user-defined or compiler-generated), the object is properly initialized according to that constructor's logic. If no default constructor exists, the code does not compile (eithier compiler or user has to define).

```cpp
BankAccount acc;   // BankAccount's default constructor runs
```

Default initialization is the baseline. It solves the class-type case (constructor runs automatically) but leaves the primitive-type case completely unsolved.


### Copy Initialization `T obj = value`

Copy initialization was the first syntax available for giving a variable a value at the point of declaration.

```cpp
int x = 5;
BankAccount acc = BankAccount(101, 1000.0); // move constructor
BankAccount acc1 = acc;                     // copy constructor
```
What actually happens is not a simple assignment. Despite the `=` symbol, this is not the assignment operator. For a class type, this invokes a constructor.

###  Direct Initialization `T obj(args)`
Direct initialization calls the constructor directly with arguments, with no implicit conversion step involved.

to get know about implicit conversion refer [constructors](../constructor/README.md)

```cpp
BankAccount acc(101, 1000.0);    // calls parameterized constructor directly
int x(5);                        // valid — initializes x to 5
```

**Why it exists — two advantages over copy initialization:**
1. Multiple arguments can be expressed directly without constructing a temporary
2. `explicit` constructors are callable — direct initialization is not blocked by `explicit`

```cpp
class BankAccount {
public:
    explicit BankAccount(int id) {}
};

BankAccount a(101);    // VALID: direct initialization bypasses explicit restriction
```

**The Fatal Flaw of `()`**
Direct initialization has one severe, documented flaw. When the argument list is empty, the syntax is identical to a function declaration. The compiler always chooses the function declaration interpretation.

```cpp
BankAccount acc();   // INTENDED: default-construct acc
                     // ACTUAL: declares a function named acc
                     //         that takes no arguments and returns BankAccount
```

```cpp
acc.deposit(100);    // COMPILE ERROR: acc is a function, not an object
                     // a function has no member 'deposit'
```
This is called the **most vexing parse**

It cannot occur with arguments:

```cpp
BankAccount acc(101);   // unambiguously an object — cannot be a function
                        // declaration with an integer argument
```
This single flaw is the primary motivation for brace initialization.


### Brace Initialization — `T obj{args}`
Brace initialization was introduced in C++11 to solve two problems: the most vexing parse and silent narrowing conversions.

#### Solving the Most Vexing Parse

```cpp
BankAccount acc{};   // unambiguously default-constructs acc
                     // cannot be parsed as a function declaration
```

`{}` has no function-declaration interpretation in the grammar. The ambiguity is eliminated at the syntax level.

#### Solving Silent Narrowing Conversions
A **narrowing conversion** is a conversion that may lose information - for example, `double` to `int` truncates the fractional part. With `()` and `=`, narrowing happens silently:

```cpp
int x(3.14);      // VALID: x becomes 3 — fractional part lost silently
int y = 3.14;     // VALID: y becomes 3 — fractional part lost silently
```

With `{}`, narrowing is a compile error:

```cpp
int z{3.14};      // COMPILE ERROR: narrowing conversion from double to int
```

The compiler rejects the code rather than silently discarding data. This converts a class of hard-to-find runtime bugs into immediate compile errors.
```cpp
class BankAccount {
    int accountId;
public:
    BankAccount(int id) : accountId(id) {}
};

BankAccount acc{3.14};   // COMPILE ERROR: narrowing double → int in constructor arg
BankAccount acc(3.14);   // VALID: silently truncates — no warning by default
```

**Brace Initialization With Constructors**
For ordinary constructor calls where no narrowing is involved, `{}` and `()` produce identical results:
```cpp
BankAccount acc{101, 1000.0};   // calls parameterized constructor
BankAccount acc(101, 1000.0);   // identical result
```
The distinction matters only when narrowing is possible, or when the most vexing parse is a risk.

### Value Initialization `T obj{}`
Empty braces produce **value initialization** — a well-defined state with no arguments.

**For primitive types:** zero-initialized to `0`, `0.0`, `false`, or `nullptr` depending on the type.
```cpp
int    a{};     // a = 0
double b{};     // b = 0.0
bool   c{};     // c = false
int*   p{};     // p = nullptr
```

**For class types:** the default constructor runs (same as `T obj;`).

**Comparison with `T obj;`:**

```cpp
int x;      // default initialization — x is garbage
int y{};    // value initialization — y is 0
```

`T obj{}` is the safe version of `T obj;` for primitives. The empty braces guarantee zero for primitives, eliminating the garbage value problem without requiring a named value.

**Comparison with `T obj()`:**

```cpp
int x();    // most vexing parse — function declaration, not int
int y{};    // value initialization — y = 0
```

`T obj{}` is the only zero-argument initialization syntax that is unambiguously an object and correctly value-initializes primitives.

### Aggregate Initialization
An **aggregate** is a class or struct with:
- No user-declared constructors
- No private or protected non-static data members
- No virtual functions
- No base classes

Aggregates can be initialized by listing values for their members directly in declaration order, using either `=` with braces or braces alone.
```cpp
struct Point {
    double x;
    double y;
};

Point p1{3.0, 4.0};        // x = 3.0, y = 4.0
Point p2 = {3.0, 4.0};    // identical — copy-list-initialization form
Point p3{3.0};              // x = 3.0, y = 0.0 — remaining members value-initialized
Point p4{};                 // x = 0.0, y = 0.0 — all members value-initialized
```

**No constructor is called.** The compiler directly initializes each member in declaration order using the corresponding brace value. This connects to the struct vs class convention — aggregate initialization is naturally suited to Plain Old Data structs that have no invariants to enforce.

### Array Initialization — `[]` and `{}`

`[]` is not an initialization operator. It is the array declaration and subscript syntax. The actual initialization of array elements is always performed by `{}`.

```cpp
int arr[5];                   // default initialization — all elements garbage
int arr[5] = {1, 2, 3};      // elements: 1, 2, 3, 0, 0 — remaining zero-initialized
int arr[]  = {1, 2, 3, 4};   // size deduced from list — int[4]
int arr[5]{1, 2, 3};          // same as = {} form — 1, 2, 3, 0, 0
int arr[5]{};                 // all elements value-initialized to 0
```

**Size deduction:** When `[]` is left empty and an initializer list is supplied, the compiler counts the elements and sets the array size to match. This removes the need to keep an explicit size in sync with the list.

```cpp
int arr[] = {10, 20, 30};   // compiler sets size to 3 automatically
// equivalent to:
int arr[3] = {10, 20, 30};
```

**Arrays of class objects:**

```cpp
class BankAccount {
public:
    BankAccount(int id, double balance) { /* ... */ }
};

BankAccount accounts[3] = {
    BankAccount(101, 1000.0),
    BankAccount(102,  500.0),
    BankAccount(103,  250.0)
};
```

Each element is constructed using the corresponding braced sub-expression, following the same constructor resolution rules as single-object construction.
