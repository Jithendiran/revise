## Lvalue
Understanding references requires separating the moment a reference is created from the moment it is used. *Reference don't have pointers*

### 1. Binding (Initialization)
Binding happens only when a reference is created.

```cpp
int x = 10;
int &ref = x;
```
* What happens: The compiler takes the memory address of `x` and stores that address inside the memory allocated for `ref`.
* The Rule: This binding is permanent. The address stored inside `ref` can never be changed to point to another box.

### 2. Assignment
Assignment happens when using an already existing reference on the left side of an equal sign.

```cpp
ref = 20;
```
* What happens: The compiler sees `ref`. Because `ref` is a reference, the compiler automatically generates a dereference instruction. It looks at the address stored inside `ref` (which is the address of `x`) and writes the value `20` into that address.
* The Result: The value of `x` becomes `20`. The address stored inside `ref` remains exactly the same.

### What Can Bind to T&?
It can only bind to an expression that has a permanent, identifiable address in memory. This means it must be an lvalue.

```cpp
int x = 10; 
int &r = x; // allowed, x has a permanent address
int *p = &x;
int &r = *p; // Allowed, *p resolves to the permanent address of x
```

### What Cannot Bind to T&?
It cannot bind to anything temporary that lacks a permanent address in memory (rvalues).
```cpp
int &r = 5; // Error. `5` is a raw value, not a memory box)
int &r = x + 1; //(Error. The result of x + 1 is held in a CPU register or a temporary slot that disappears immediately)
*r = 7; // Error
```

### Const Lvalue References (const T&)
Adding `const` changes how the compiler handles binding and memory lifetime.

```cpp
const int& r = 5; // allowed
```
1. Temporary Creation: The compiler allocates a hidden, temporary slot in memory.
2. Value Copy: The value `5` is copied into that hidden memory slot.
3. Address Binding: The address of that hidden slot is stored inside `r`
4. Lifetime Extension: The compiler guarantees that this hidden memory slot will not be destroyed until `r` goes out of scope.
   
### What Can Bind to const T&?
Because of this hidden mechanism, a const lvalue reference can bind to absolutely anything of the matching type:
```cpp
const int& r = x; // lvalue, allowed
const int& r = 5; // prvalue, allowed
// HOW THE COMPILER REWRITES IT:
// int __temp1 = 20;          <- Secretly creates a hidden temporary variable
// const int& r = __temp1; <- Binds the reference to it, extending __temp1's lifetime

const int& r = x + 5; // prvalue, allowed
// HOW THE COMPILER REWRITES IT:
// int __temp1 = x + 5;          <- Secretly evaluates the expression into a hidden temporary
// const int& r = __temp1; <- Binds the reference to it, extending __temp1's lifetime
```
*compiler rewrite is just for easy understading, in real may not work like this*

**carefull here**
```cpp
const int& getInvalidReference() {
    return 42; // INVALID/DANGEROUS!
}

const int& ref = getInvalidReference(); // Undefined Behavior!
```

```cpp
const int& getInvalidReference() {
    int __hidden_temp = 42; // Created inside the function's stack frame
    return __hidden_temp;   // Returns a reference to this local variable
}

const int& ref = getInvalidReference(); // __hidden_temp is dead
```

```cpp
int getValidValue() { // Notice: No ampersand '&'
    return 42; 
}

// In main:
const int& ref = getValidValue(); // VALID! Lifetime extension works perfectly here.

//-------
int __hidden_temp = getValidValue();
const int& ref = __hidden_temp;
```

*`const &` member variable bound in a constructor:*

Binding a temporary to a `const &` member inside a constructor does not extend the lifetime of the temporary past the closing brace of the constructor. The member becomes a dangling reference the moment the constructor exits.

```cpp
struct Foo {
    const int& ref;
    Foo() : ref(42) {}
    // The temporary '42' is destroyed when the constructor ends.
    // 'ref' is a dangling reference inside every Foo object.
};

Foo f;
// f.ref;   // undefined behavior — refers to destroyed memory
```

> Lifetime extension is a **compile-time** decision. The operating system is not involved.

**Effect:** Data accessed through a `const &` cannot be modified through that reference.