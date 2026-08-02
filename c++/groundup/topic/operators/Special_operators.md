# New and Changed Operators

### [spaceship](./spaceship_operator/README.md)

### `co_await` and `co_yield` (New)

Part of the coroutines system (C++20). Both are operators in the precedence table but their full behavior requires the coroutine framework to be understood.
- `co_await` (precedence 3): suspends a coroutine until an awaitable completes
- `co_yield` (precedence 16): yields a value from a coroutine and suspends
These are covered in the Coroutines section of the curriculum.


### Changes 
**Right-shift of signed integers is now defined:**

In C++20, right shift of a signed negative integer performs an arithmetic right shift (sign-extending) — previously this was implementation-defined.

#### Positive Integer Right Shift Example
A bitwise right shift operation moves all binary digits of a number a specified number of positions to the right.
```
8 >> 2
8 = 0000 0000  0000 0000  0000 0000  0000 1000

8 >> 2
_ _ 00 0000  0000 0000  0000 0000  0000 0010
```

For positive integers, the vacated position on the leftmost side is always filled with 0.

#### Negative Integer Right Shift Example
Negative integers are represented in binary using two's complement notation, where the leftmost bit (the sign bit) is always 1.
```
-8 >> 2
-8 = 1111 1111  1111 1111  1111 1111  1111 1000

-8 >> 2
_ _ 11 1111  1111 1111  1111 1111  1111 0010
```
Depending on how the compiler handles this vacant position, two distinct methods emerge.

#### Shift Variations for Negative Integers
**Arithmetic Shift**
* Definition: An arithmetic shift fills the vacant leftmost position with a copy of the original sign bit (1 for negative numbers).
* result is `1111 1111  1111 1111  1111 1111  1111 0010` = `-2`

**Logical Shift**
* Definition: A logical shift always fills the vacant leftmost position with 0, regardless of whether the original number was positive or negative.
* result is `0011 1111  1111 1111  1111 1111  1111 1110` = `1073741822`

#### The C++20 Standard Definition
Prior to C++20, the C++ standard classified right-shifting of negative signed integers as implementation-defined. Different hardware architectures and compilers could execute either an arithmetic shift or a logical shift, making cross-platform code unpredictable.  
* The Mandate: C++20 explicitly mandates that a right shift on a signed integer must perform an arithmetic shift.  
* The Logic: The standard aligns software behavior with universal hardware practices. Developers are guaranteed that negative signed integers will consistently perform sign extension, yielding predictable mathematical division by powers of 2 without risking unexpected positive conversions.