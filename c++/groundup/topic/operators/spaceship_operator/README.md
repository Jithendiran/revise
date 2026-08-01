# Spaceship operator
## The Problem — Six Functions for One Concept
Before C++20, comparing two objects of a user-defined class required writing up to six separate operator functions. Every comparison a class needed had to be implemented individually:

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    bool operator==(const Temperature& rhs) const { return celsius == rhs.celsius; }
    bool operator!=(const Temperature& rhs) const { return !(*this == rhs); }
    bool operator< (const Temperature& rhs) const { return celsius < rhs.celsius; }
    bool operator> (const Temperature& rhs) const { return rhs < *this; } // will calll the `operator<` `rhs` becomes `this` and this becomes `rhs` 
    bool operator<=(const Temperature& rhs) const { return !(rhs < *this); }
    bool operator>=(const Temperature& rhs) const { return !(*this < rhs); }
};
```
Six functions. One concept: ordering. 

Every function contains either a duplication of the core comparison logic or a derivation from another operator. If the internal representation of `Temperature` changes — for example, from `double celsius` to `double kelvin` — all six functions must be found and updated. Missing any one of them produces a class that behaves inconsistently.

The same pattern had to be repeated for every class that needed comparison. Thousands of lines of boilerplate across a real codebase.

C++20 introduced the **spaceship operator** `<=>` to collapse all of this into a single function — and in most cases, a single line.

## The Three-Way Comparison Concept
Before the spaceship operator syntax is established, the concept it implements must be understood.

A two-way comparison returns a `bool` — either true or false. This requires two separate questions to fully describe the relationship between two values:

```cpp
a < b    // is a less than b?
a == b   // is a equal to b?
```
A **three-way comparison** answers both questions in a single operation. It returns a value that encodes one of three outcomes:

| Outcome | Meaning |
|---|---|
| Negative (less than zero) | Left operand is less than right |
| Zero (equal) | Left operand is equal to right |
| Positive (greater than zero) | Left operand is greater than right |

This is the same concept used by `strcmp` in C:

```cpp
int result = strcmp("apple", "banana");
// result < 0: "apple" < "banana","apple" comes before "banana"
// result == 0: "apple" == "apple", strings are equal
// result > 0: "banana" > "apple", "banana" comes after "apple"
```

C++20 formalizes this concept with a dedicated operator and a set of return types that express the kind of ordering being performed.


## The Spaceship Operator `<=>`

### Syntax

```cpp
auto result = a <=> b;
```

The result is not a `bool`. It is an **ordering type** from the `<compare>` header. The ordering type is then compared to zero to extract meaning:

```cpp
#include <compare>

int a = 5, b = 10;
auto result = a <=> b;

if (result < 0)  { /* a < b */ }
if (result == 0) { /* a == b */ }
if (result > 0)  { /* a > b */ }
```

### Equal vs Equivalent
* Equal (Strong Ordering): The two items are 100% identical in every measurable way. They are completely interchangeable.
* Equivalent (Weak Ordering): The two items occupy the same spot in a sort/comparison, but they are not identical inside.

#### Example A: Student Records (Sorting by Age)
Imagine a C++ Student struct with a name and an age, where you only compare students by age:
```cpp
struct Student {
    std::string name;
    int age;
};
```
Let's compare two objects
* Alice (Age 20)
* Bob (Age 20)

If compare Alice <=> Bob based on age:
* Are they equivalent in age? Yes ($20 == 20$). They take the exact same position when sorted by age.
* Are they equal? No! Alice is not Bob. One has the name "Alice", the other has "Bob"

#### Example B: Mathematical 
* $f_1 = \frac{1}{2}$
* $f_2 = \frac{2}{4}$
Mathematically, both fractions represent the value $0.5$.
* Are they equivalent in math value? Yes, $1/2 == 2/4$.
* $f_2.\text{numerator}$ is 2. If you print their numerators, they output different numbers. They hold different internal data.

#### Example C: Case-Insensitive Search
"NOTES.TXT" and "notes.txt" refer to the same file location.
* Are they equivalent for file sorting? Yes, they sort into the same position.
* Are they equal? No. One contains uppercase letters ('N', 'O'), and the other contains lowercase letters ('n', 'o'). Their raw binary data is different.

### The Three Return Types
C++20 introduced three categories because it wants to know how strict your comparison is:

|Category|Meaning|Example|
|--------|-------|-------|
|std::strong_ordering|Strict Equality: Equivalent means 100% identical and interchangeable.|Comparing integers (5 <=> 5).|
|std::weak_ordering|Equivalence Only: Same sorting rank, but objects might hold different data.|Comparing Student by age, or case-insensitive text.|
|std::partial_ordering| Equivalence + Uncomparable: Some values cannot even be compared.|Floating-point numbers with NaN (Not a Number).|

**What is a "Total Order"?**
> A total order simply means every single value can be compared to every other value, and they always fall into a strict sequence: Less than, Equal to, or Greater than. For example, take integers: 3, 5, and 7. It is always possible to tell if one is smaller, larger, or equal to another. There is no confusion or missing information.

* `equivalent-but-unequalvalues`: Relationship happens when two distinct values are treated as "equal" by a comparison function, even though their internal representations are different. eg: `"Apple"` == `"apple"`
* `no equivalent-but-unequalvalues`: Relationship happens when two distinct values are treated as "equal" and "equivalent" by a comparison function. eg: `"apple"` == `"apple"`

#### std::strong_ordering
Used when the ordering is a **total order with no equivalent-but-unequalvalues**. Every pair of values is comparable. If `a <=> b == 0`, then `a` and `b` are genuinely identical in every meaningful sense.

Used for: `int`, `long`, `char`, `bool`, pointer types, any type where equality and equivalence are the same thing.

```cpp
int a = 5, b = 5;
auto r = a <=> b;
// r is std::strong_ordering::equal
// a and b are not just equivalent — they are truly equal in all respects
```

Possible values:
- `std::strong_ordering::less`
- `std::strong_ordering::equal`
- `std::strong_ordering::greater`

#### std::weak_ordering
Used when the ordering is a **total order but equivalent values may not be identical**. Every pair is comparable, but `a <=> b == 0` means "equivalent for ordering purposes" — not necessarily "identical in all respects."

Used for: case-insensitive string comparison (two strings can be "equivalent" for ordering without having the same characters), or any type where the sort key is different from full equality.


```cpp
// Case-insensitive string comparison
// "Hello" and "hello" are equivalent for ordering (weak_ordering::equivalent)
// but they are not equal (different characters)
```

Possible values:
- `std::weak_ordering::less`
- `std::weak_ordering::equivalent`
- `std::weak_ordering::greater`


#### std::partial_ordering
Partial ordering adds a fourth possibility: Uncomparable (or "unordered"). It means: “It is mathematically impossible to say whether $a$ is less than, greater than, or equal to $b$. They just don't relate.”

Suppose  have:
```cpp
double a = 5.0;
double b = NAN; //(Not a Number)
```
* Is 5.0 < NAN? False.
* Is 5.0 > NAN? False.
* Is 5.0 == NAN? False. (In fact, NaN is not even equal to itself!).

Because none of those three standard questions (<, ==, >) return true, standard ordering breaks down completely.

This creates a partial order: some numbers can be ordered (5.0, 1.2,..) against each other normally, but NaN breaks the rules, leaving the comparison unordered (or uncomparable).

##### Example Comparing People by Two Traits
Imagine trying to rank people based on both Height and Age combined, but comaprision only want to say someone is "greater than" someone else if they are taller AND older.
* Person A: 180 cm tall, 25 years old.
* Person B: 170 cm tall, 30 years old.

If comparision between Person A and Person B:
* Person A is taller than B ($180 > 170$).
* But Person A is younger than B ($25 < 30$).
Who is "greater"? There is no correct answer!
* A isn't strictly greater than B.
* B isn't strictly greater than A.
* They aren't equal either.
They are uncomparable in that combined multi-trait system. That is a partial ordering.

##### Result of Partial Ordering
std::partial_ordering, the result of a <=> b can be one of four things:
1. less ($a < b$)
2. equivalent ($a == b$)
3. greater ($a > b$)
4. unordered ($a$ and $b$ cannot be compared at all)

Used when **some pairs of values are incomparable**. Not every pair has a defined ordering relationship.

Used for: `float` and `double`, because `NaN` (Not a Number) is not less than, equal to, or greater than any other value — including itself.

```cpp
#include <compare>
#include <cmath>

double a = std::numeric_limits<double>::quiet_NaN();
double b = 5.0;

auto r = a <=> b;
// r is std::partial_ordering::unordered
// NaN is incomparable to anything
```
Possible values:
- `std::partial_ordering::less`
- `std::partial_ordering::equivalent`
- `std::partial_ordering::greater`
- `std::partial_ordering::unordered`

### Ordering Type Hierarchy
The three types form a hierarchy from most restrictive to least:

```
std::strong_ordering
↓ (implicitly converts to)
std::weak_ordering
↓ (implicitly converts to)
std::partial_ordering
```
A `strong_ordering` can be used wherever a `weak_ordering` or `partial_ordering` is expected. A `weak_ordering` can be used where a `partial_ordering` is expected. The reverse is not true.


### Writing `<=>` for a User-Defined Class
For most value types, the `<=>` implementation delegates directly to the comparison of one or more data members:

```cpp
#include <compare>

class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    std::partial_ordering operator<=>(const Temperature& rhs) const {
        return celsius <=> rhs.celsius;
        // double uses partial_ordering — handles NaN correctly
    }

    bool operator==(const Temperature& rhs) const {
        return celsius == rhs.celsius;
    }
};

Temperature a(20.0), b(30.0), c(20.0);

a <=> b;   // std::partial_ordering::less
a <=> c;   // std::partial_ordering::equivalent
b <=> a;   // std::partial_ordering::greater

std::partial_ordering result = a <=> b;

if (result < 0) {
    // a is less than b
} else if (result == 0) {
    // a is equivalent to b
} else if (result > 0) {
    // a is greater than b
} else {
    // a and b are unordered (e.g., one is NaN)
}

if (result == std::partial_ordering::less) {
    // a is less than b
} 
else if (result == std::partial_ordering::equivalent) {
    // a is equivalent to b
} 
else if (result == std::partial_ordering::greater) {
    // a is greater than b
} 
else if (result == std::partial_ordering::unordered) {
    // a and b cannot be compared (NaN)
}
```

**Why `operator==` must still be written manually when using `double`:**

`<=>` returning `partial_ordering` generates `<`, `>`, `<=`, `>=` — but does NOT automatically generate `==` and `!=`. This is because for `double`, `NaN == NaN` is `false` by IEEE 754 convention, but `NaN <=> NaN` is `unordered`. The two concepts are not equivalent for floating-point types. C++20 keeps them separate to avoid silently generating incorrect `==` behavior.

For integer-based types returning `strong_ordering`, `==` IS generated automatically.

###  `= default` — The One-Line Solution
For classes where members already support `<=>`, the entire comparison system can be generated automatically with a single declaration:

```cpp
#include <compare>

class Point {
    int x;
    int y;
public:
    Point(int x, int y) : x(x), y(y) {}

    auto operator<=>(const Point&) const = default;
};
```

**What `= default` generates:**

1. A `<=>` that compares members **lexicographically in declaration order**: first compare `x`, if equal then compare `y`
2. An `operator==` that tests member-by-member equality
3. From those two: `==`, `!=`, `<`, `>`, `<=`, `>=` — all six comparisons

```cpp
Point a{1, 2}, b{1, 3}, c{1, 2};

a < b;    // VALID: x equal (1==1), then y: 2 < 3 → true
a == c;   // VALID: x equal, y equal → true
a != b;   // VALID: generated from == → true
a > b;    // VALID: generated from <=> → false
```

#### Lexicographic Comparison
The default comparison compares members in declaration order. The first member that differs determines the result. Only if all preceding members are equal does comparison proceed to the next member.

```cpp
class Version {
    int major;
    int minor;
    int patch;
public:
    Version(int major, int minor, int patch)
        : major(major), minor(minor), patch(patch) {}

    auto operator<=>(const Version&) const = default;
};

Version v1{2, 10, 3};
Version v2{2, 11, 0};
Version v3{2, 10, 3};

v1 < v2;    // major equal (2==2), minor: 10 < 11 → true (patch not examined)
v1 == v3;   // all members equal → true
v2 > v1;    // major equal, minor: 11 > 10 → true
```

#### When `= default` Is Not Appropriate
`= default` is not suitable when:

1. The comparison key is different from the stored representation

```cpp
class CaseInsensitiveString {
    std::string value;
public:
    // Cannot use = default: comparison must ignore case
    // but storage preserves case
    std::weak_ordering operator<=>(const CaseInsensitiveString& rhs) const {
        // custom comparison that lowercases before comparing
    }
};
```
2. Only some members participate in ordering

```cpp
class Employee {
    int    employeeId;    // ordering key
    std::string name;     // NOT part of ordering — two employees with the
                          // same ID are the same employee regardless of name
public:
    auto operator<=>(const Employee& rhs) const {
        return employeeId <=> rhs.employeeId;
        // name deliberately excluded
    }
};
```

**Return type deduction:**

When `auto` is used as the return type with `= default`, the compiler deduces the return type from the `<=>` of each member, taking the weakest ordering that applies. For `int` members, this is `strong_ordering`. If any member used `double`, the deduced type would be `partial_ordering`.

## Automatic Generation of `!=` From `==` (C++20)
### The Problem
Before C++20, defining `operator==` did not automatically provide `operator!=`. Both had to be written:
```cpp
// C++17 and earlier — both required
bool operator==(const Temperature& rhs) const { return celsius == rhs.celsius; }
bool operator!=(const Temperature& rhs) const { return !(*this == rhs); }
```

`operator!=` contains no independent logic — it is always `!(operator==)`. Writing it manually is pure boilerplate.

### The C++20 Rule
In C++20, if `operator==` is defined, the compiler **automatically generates `operator!=`** as its logical negation.

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    bool operator==(const Temperature& rhs) const {
        return celsius == rhs.celsius;
    }
    // operator!= is automatically available — no definition needed
};

Temperature a(20.0), b(30.0);
a != b;   // VALID in C++20: compiler generates !(a == b)
```

**This is a source-breaking change from C++17 in one edge case:** if a class explicitly defined `operator!=` with behavior different from `!(operator==)`, that custom behavior is still used — the explicit definition takes priority over the generated one. The generation only applies when no explicit `operator!=` exists.

### Rewritten Candidates — Reverse Argument Lookup
Before C++20, a symmetric comparison required defining both argument orders:
```cpp
// C++17 — must define both
bool operator==(const Temperature& lhs, const Temperature& rhs);
bool operator==(double lhs, const Temperature& rhs);       // double on left
bool operator==(const Temperature& lhs, double rhs);       // double on right
```
In C++20, when the compiler cannot find an `operator==` matching a given call, it also considers the **reversed** form — swapping left and right operands. If `a == b` fails, the compiler tries `b == a`.

```cpp
class Temperature {
    double celsius;
public:
    Temperature(double c) : celsius(c) {}

    bool operator==(const Temperature& rhs) const {
        return celsius == rhs.celsius;
    }
};

Temperature t(20.0);
t == 20.0;    // VALID: t.operator==(Temperature{20.0}) — direct match
20.0 == t;    // C++17: COMPILE ERROR — no match
              // C++20: VALID — compiler tries t == 20.0 (reversed)
              //        finds t.operator==(Temperature{20.0}) — succeeds
```

The same rewriting applies to `<=>` and the operators generated from it. If `a < b` cannot be resolved directly, the compiler tries `b <=> a` and checks whether the result is greater-than.

```cpp
20 > t; // changes to t < 20;
```

This eliminates the need to define both argument-order variants of symmetric operators for mixed-type comparisons.