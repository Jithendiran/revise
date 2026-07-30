# C++ Operators and Precedence — Complete Reference (C++20)

## What Operator Precedence Is
When an expression contains multiple operators, the compiler must determine which operations to perform first. Precedence defines this evaluation order. Higher precedence operators bind more tightly to their operands than lower precedence operators.

```cpp
int result = 2 + 3 * 4;
//                 ↑
//           * has higher precedence than +
//           evaluated as: 2 + (3 * 4) = 2 + 12 = 14
//           NOT as:       (2 + 3) * 4 = 5 * 4 = 20
```
**Associativity** determines evaluation order when multiple operators of the **same** precedence appear in sequence.

```cpp
int a = b = c = 5;
// = is right-to-left associative
// evaluated as: a = (b = (c = 5))

int x = 10 - 3 - 2;
// - is left-to-right associative
// evaluated as: (10 - 3) - 2 = 7 - 2 = 5
// NOT as:       10 - (3 - 2) = 10 - 1 = 9
```

**Important rule:** Precedence and associativity determine **grouping** only. They do not determine the **order of evaluation** of subexpressions. The order in which operands are evaluated is a separate concern and is often unspecified

## Complete Operator Precedence Table (C++20)

Precedence 1 is the highest (binds most tightly).
Precedence 17 is the lowest (binds least tightly).

### Precedence 1 — Scope Resolution
 Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `::` | Scope resolution | `Class::member` | Left to right |

```cpp
std::cout          // namespace::name
BankAccount::id    // class::member
::globalVar        // ::name — global scope
```

`::` has the highest precedence of all operators. It always resolves before anything else in the expression.

### Precedence 2 — Postfix / Suffix

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `a++` | Postfix increment | `expr++` | Left to right |
| `a--` | Postfix decrement | `expr--` | Left to right |
| `()` | Function call | `func(args)` | Left to right |
| `[]` | Subscript | `arr[i]` | Left to right |
| `.` | Member access | `obj.member` | Left to right |
| `->` | Pointer member access | `ptr->member` | Left to right |
| `typeid` | Type identification | `typeid(expr)` | Left to right |
| `const_cast` | Const cast | `const_cast<T>(expr)` | Left to right |
| `dynamic_cast` | Dynamic cast | `dynamic_cast<T>(expr)` | Left to right |
| `reinterpret_cast` | Reinterpret cast | `reinterpret_cast<T>(expr)` | Left to right |
| `static_cast` | Static cast | `static_cast<T>(expr)` | Left to right |

```cpp
obj.method()       // . then ()
ptr->method()      // -> then ()
arr[i]++           // [] then ++
```
Left-to-right associativity means chained member access and calls evaluate left to right:

```cpp
obj.getAccount().deposit(100);
// evaluates as: (obj.getAccount()).deposit(100)
// left to right: obj.getAccount() first, then .deposit(100) on the result
```

### Precedence 3 — Prefix Unary

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `++a` | Prefix increment | `++expr` | Right to left |
| `--a` | Prefix decrement | `--expr` | Right to left |
| `+a` | Unary plus | `+expr` | Right to left |
| `-a` | Unary minus (negation) | `-expr` | Right to left |
| `!` | Logical NOT | `!expr` | Right to left |
| `~` | Bitwise NOT | `~expr` | Right to left |
| `(T)` | C-style cast | `(Type)expr` | Right to left |
| `*` | Dereference | `*ptr` | Right to left |
| `&` | Address-of | `&var` | Right to left |
| `sizeof` | Size of object | `sizeof(expr)` | Right to left |
| `sizeof...` | Size of parameter pack | `sizeof...(pack)` | Right to left |
| `new` | Dynamic allocation | `new T` | Right to left |
| `new[]` | Dynamic array allocation | `new T[n]` | Right to left |
| `delete` | Dynamic deallocation | `delete ptr` | Right to left |
| `delete[]` | Dynamic array deallocation | `delete[] ptr` | Right to left |
| `co_await` | Coroutine await (C++20) | `co_await expr` | Right to left |

```cpp
*ptr++     // * is precedence 3, ++ is postfix precedence 2
           // evaluated as: *(ptr++) — increment pointer, dereference old value

++*ptr     // prefix ++ and * are both precedence 3, right-to-left
           // evaluated as: ++(*ptr) — dereference, then increment the value

&arr[i]    // [] is precedence 2, & is precedence 3
           // evaluated as: &(arr[i]) — address of the i-th element
```

### Precedence 4 — Pointer-to-Member Access

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `.*` | Pointer-to-member of object | `obj.*memptr` | Left to right |
| `->*` | Pointer-to-member of pointer | `ptr->*memptr` | Left to right |

```cpp
// pointer-to-member represents an offset, only available in c++
struct Point { int x; int y; };

int Point::*px = &Point::x;   // pointer to member x
Point p{3, 4};

p.*px       // access member x of p via pointer-to-member
            // result: 3
```

These operators are rarely used in everyday code. They appear in generic callback systems and certain template patterns.

### Precedence 5 — Multiplicative

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `*` | Multiplication | `a * b` | Left to right |
| `/` | Division | `a / b` | Left to right |
| `%` | Modulo (remainder) | `a % b` | Left to right |

```cpp
10 / 2 * 3    // left to right: (10 / 2) * 3 = 5 * 3 = 15
              // NOT: 10 / (2 * 3) = 10 / 6 = 1
```

**Integer division truncates toward zero:**

```cpp
7 / 2     // 3  — not 3.5
-7 / 2    // -3 — truncates toward zero (C++11 and later)
7 % 2     // 1  — remainder
-7 % 2    // -1 — remainder has same sign as dividend (C++11)
```

### Precedence 6 — Additive

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `+` | Addition | `a + b` | Left to right |
| `-` | Subtraction | `a - b` | Left to right |

```cpp
10 - 3 - 2    // left to right: (10 - 3) - 2 = 7 - 2 = 5
```

### Precedence 7 — Bitwise Shift

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `<<` | Left shift | `a << b` | Left to right |
| `>>` | Right shift | `a >> b` | Left to right |

```cpp
1 << 3      // 1 shifted left 3 bits = 8 (2^3)
16 >> 2     // 16 shifted right 2 bits = 4 (16 / 2^2)
```

**Shifting rules:**
- Left shift `<<`: multiplies by 2 per shift (for non-negative values)
- Right shift `>>`: divides by 2 per shift for unsigned types; for signed types, behavior is implementation-defined in C++03, arithmetic shift (sign-extending) in C++20

**Important note:** `<<` and `>>` are also overloaded for stream operations. When used with `std::cout` or `std::cin`, they are stream insertion and extraction operators, not bit shifts. The compiler distinguishes by the types of the operands.

### Precedence 8 — Three-Way Comparison (C++20)

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `<=>` | Spaceship (three-way comparison) | `a <=> b` | Left to right |

```cpp
#include <compare>

int a = 5, b = 10;
auto result = a <=> b;
// result is std::strong_ordering::less

if ((a <=> b) < 0) { /* a < b */ }
if ((a <=> b) > 0) { /* a > b */ }
if ((a <=> b) == 0) { /* a == b */ }
```
`<=>` returns an ordering type, not a `bool`. Three return types exist:

| Return type | Meaning | Example types |
|---|---|---|
| `std::strong_ordering` | Total order, no equivalent-but-unequal values | `int`, `long` |
| `std::weak_ordering` | Total order, equivalent values may be unequal | Case-insensitive string |
| `std::partial_ordering` | Some pairs are incomparable | `float`, `double` (NaN) |

**Why `<=>` has its own precedence level between shift and relational:**

Placing it here means `a + b <=> c + d` groups as `(a + b) <=> (c + d)`, which is the expected mathematical meaning. If it had the same precedence as `<` and `>`, mixed expressions would require parentheses.

**`= default` generates all six comparisons:**

```cpp
class Point {
    int x, y;
public:
    auto operator<=>(const Point&) const = default;
    // compiler generates ==, !=, <, >, <=, >= using member-by-member <=>
};
```

### Precedence 9 — Relational

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `<` | Less than | `a < b` | Left to right |
| `>` | Greater than | `a > b` | Left to right |
| `<=` | Less than or equal | `a <= b` | Left to right |
| `>=` | Greater than or equal | `a >= b` | Left to right |

```cpp
1 < 2 < 3   // left to right: (1 < 2) < 3
            // = true < 3
            // = 1 < 3   (true converts to 1)
            // = true
            // THIS IS NOT A MATHEMATICAL RANGE CHECK
            // Use: 1 < x && x < 3
```

The chained comparison trap: `1 < x < 3` does not check if `x` is between 1 and 3. It evaluates `(1 < x)` first (producing `true` or `false`), then compares that boolean with `3`. Always use `&&` for range checks.

### Precedence 10 — Equality

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `==` | Equal to | `a == b` | Left to right |
| `!=` | Not equal to | `a != b` | Left to right |

```cpp
a == b == c    // left to right: (a == b) == c
               // compares the bool result of (a == b) with c
               // NOT: checks if a, b, and c are all equal
               // Use: a == b && b == c
int a = 5;
int b = 5;
int c = 1;

a == b == c; // true; (5 == 5) -> return true is 1, then compare 1 == 1
```

**Floating-point equality warning:**

```cpp
double x = 0.1 + 0.2;
// The stored binary approximation for 0.1 is actually a tiny microscopic fraction greater than 0.1.
// The stored binary approximation for 0.2 is also a tiny microscopic fraction greater than 0.2.
// When the computer adds those two slightly oversized binary numbers together, the result is also slightly oversized.
// The true mathematical sum should be 0.3.
// Because of the accumulated binary error, the actual value sitting in memory is infinitesimally larger than 0.3 (roughly 0.3000000000000000166...). When standard output formatting prints this out rounded to 17 decimal places, the closest displayable decimal representation that the computer can generate is 0.30000000000000004.  
// That 4 isn't a random glitch—it is the exact mathematical digit required to represent that specific tiny binary overshoot when translated back into our base-10 system.
x == 0.3    // likely false — floating-point representation error
            // 0.1 + 0.2 = 0.30000000000000004 in IEEE 754
            // Use: std::abs(x - 0.3) < 1e-9
```

### Precedence 11 — Bitwise AND

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `&` | Bitwise AND | `a & b` | Left to right |

```cpp
0b1010 & 0b1100    // = 0b1000 = 8
                   // each bit: AND of corresponding bits

// Common use: masking
int flags = 0b0111;
bool isSet = (flags & 0b0100) != 0;   // check if bit 2 is set
```

Note: `&` at precedence 11 is bitwise AND — different from unary `&` (address-of) at precedence 3. The compiler distinguishes by context (unary vs binary usage).

### Precedence 12 — Bitwise XOR

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `^` | Bitwise XOR (exclusive OR) | `a ^ b` | Left to right |

```cpp
0b1010 ^ 0b1100    // = 0b0110 = 6
                   // each bit: 1 if bits differ, 0 if same

// XOR swap (classic — use std::swap in practice)
a ^= b;
b ^= a;
a ^= b;
```

### Precedence 13 — Bitwise OR

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `\|` | Bitwise OR | `a \| b` | Left to right |

```cpp
0b1010 | 0b1100    // = 0b1110 = 14
                   // each bit: 1 if either bit is 1

// Common use: setting flags
int flags = 0;
flags |= 0b0100;   // set bit 2
flags |= 0b0001;   // set bit 0
// flags = 0b0101
```

**Bitwise operator precedence trap:**

```cpp
if (a & 0xFF == 0)    // WRONG: == has higher precedence than &
                       // evaluated as: a & (0xFF == 0) = a & false = a & 0
                       // always evaluates to 0

if ((a & 0xFF) == 0)  // CORRECT: parentheses force the intended grouping
```

This trap applies to all bitwise operators (`&`, `^`, `|`). Their precedence is lower than the relational and equality operators, which is counterintuitive. Always parenthesize bitwise operations when combined with comparisons.


### Precedence 14 — Logical AND

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `&&` | Logical AND | `a && b` | Left to right |

```cpp
true && false    // false
true && true     // true
```

**Short-circuit evaluation:** If the left operand is `false`, the right operand is **not evaluated**. The result is already determined to be `false`.

```cpp
int* p = nullptr;
if (p != nullptr && p->value > 0) {
    // p->value is only evaluated if p != nullptr
    // without short-circuit, p->value on nullptr would crash
}
```

This is a guaranteed language behavior, not a compiler optimization.

### Precedence 15 — Logical OR

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `\|\|` | Logical OR | `a \|\| b` | Left to right |

```cpp
false || true    // true
false || false   // false
```

**Short-circuit evaluation:** If the left operand is `true`, the right operand is **not evaluated**. The result is already `true`.

```cpp
if (cache.has(key) || database.fetch(key)) {
    // database.fetch(key) is only called if cache.has(key) is false
    // avoids expensive database call when cache hit occurs
}
```

### Precedence 16 — Ternary, Assignment, and Throw

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `?:` | Ternary conditional | `cond ? a : b` | Right to left |
| `=` | Direct assignment | `a = b` | Right to left |
| `+=` | Add and assign | `a += b` | Right to left |
| `-=` | Subtract and assign | `a -= b` | Right to left |
| `*=` | Multiply and assign | `a *= b` | Right to left |
| `/=` | Divide and assign | `a /= b` | Right to left |
| `%=` | Modulo and assign | `a %= b` | Right to left |
| `<<=` | Left shift and assign | `a <<= b` | Right to left |
| `>>=` | Right shift and assign | `a >>= b` | Right to left |
| `&=` | Bitwise AND and assign | `a &= b` | Right to left |
| `^=` | Bitwise XOR and assign | `a ^= b` | Right to left |
| `\|=` | Bitwise OR and assign | `a \|= b` | Right to left |
| `throw` | Throw expression | `throw expr` | Right to left |
| `co_yield` | Coroutine yield (C++20) | `co_yield expr` | Right to left |

**Right-to-left associativity of assignment:**

```cpp
a = b = c = 5;
// right to left: a = (b = (c = 5))
// c = 5 runs first, returns 5
// b = 5 runs next, returns 5
// a = 5 runs last
// all three variables become 5
```

**Ternary operator:**

```cpp
int max = (a > b) ? a : b;
// if a > b: max = a
// else:     max = b
```

Ternary is right-to-left, so chaining works:

```cpp
int classify = (x < 0) ? -1 : (x > 0) ? 1 : 0;
// right to left: (x < 0) ? -1 : ((x > 0) ? 1 : 0)
```

### Precedence 17 — Comma

| Operator | Name | Syntax | Associativity |
|---|---|---|---|
| `,` | Comma (sequence) | `a, b` | Left to right |

The comma operator evaluates the left expression, discards the result,
evaluates the right expression, and returns the right result.

```cpp
int x = (a++, b++, c);
// a++ evaluated and discarded
// b++ evaluated and discarded
// c evaluated and returned
// x = c
```

The comma operator in function calls and initializer lists is **not** the comma operator — it is a separator with no defined precedence behavior.

The comma operator is rarely used intentionally in modern C++. It is most commonly seen in C-style `for` loops:

```cpp
for (int i = 0, j = 10; i < j; ++i, --j) {
    // ++i, --j uses comma to advance two variables per iteration
}
```

[Order of evaluation](./Order_of_evaluation.md)