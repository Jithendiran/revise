## PRvalue
Understanding Prvalue references requires separating the concept of a temporary object from a permanent memory location.

### 1. Binding (Initialization)
Binding happens only when an prvalue reference is created. It is declared using &&.
```cpp
int &&ref = 5;
```
* What happens: The compiler allocates a hidden, temporary slot in memory, places the value `5` inside it, and stores the address of that temporary slot inside `ref`.
* The Rule: An prvalue reference can only bind to prvalues (temporaries). Once bound, the reference itself acts like a normal variable; the binding to that specific temporary memory slot is permanent.

### 2. Assignment
Assignment happens when using an already existing prvalue reference on the left side of an equal sign.
```cpp
ref = 20;
```
* What happens: The compiler treats the named prvalue reference `ref` as an lvalue expression because it has a name. It modifies the value inside the temporary memory slot that `ref` is currently bound to.
* The Result: The value in the hidden temporary slot becomes `20`. The reference does not switch to a different memory location.

### What Can Bind to T&&?
An prvalue reference can only bind to an expression that lacks a permanent, identifiable address in memory (prvalues). This includes literals, temporary objects, and the results of expressions returned by value.
```cpp
int &&r1 = 42;       // Allowed. 42 is a literal (prvalue)
int &&r2 = x + 5;    // Allowed. The result of x + 5 is a temporary value
```

### What Cannot Bind to T&&?
It cannot bind to an lvalue (a variable with a permanent, named address in memory).
```cpp
int x = 10;
int &&r1 = x;        // Error. x is an lvalue
int *p = &x;
int &&r2 = *p;       // Error. *p resolves to the permanent address of x
```

### Lifetime Extension with T&&
Just like `const T&`, binding a temporary (an prvalue) to a non-const prvalue reference T&& extends the lifetime of that temporary object to match the scope of the reference.
```cpp
int &&r = 10 + 20; 
```
Key Difference from `const T&`: The data accessed through a `T&&` can be modified.

### Dangerous Pitfalls: Returning PRvalue References
Returning an prvalue reference from a function to a temporary local variable results in undefined behavior. The lifetime extension rule does not cross function return boundaries.
```cpp
int&& getInvalidReference() {
    return 42; // INVALID/DANGEROUS!
}

int&& ref = getInvalidReference(); // Undefined Behavior!
```

```cpp
int&& getInvalidReference() {
    int __hidden_temp = 42;  // Created inside the function's stack frame
    return __hidden_temp;    // Error/Warning: Cannot bind lvalue to prvalue reference, 
                             // and the storage disappears when the function exits.
}
```

### The Correct Approach for Function Returns
To safely capture a temporary from a function, the function must return by value, and the caller can then bind it to an prvalue reference.
```cpp
int getValidValue() { // Notice: No ampersand '&&'
    return 42; 
}

// In main:
int&& ref = getValidValue(); // VALID! Lifetime extension works perfectly here.
```

### Member Variable Binding in a Constructor
Binding a temporary to an `&&` member inside a constructor does not extend the lifetime of the temporary past the closing brace of the constructor.
```cpp
struct Foo {
    int&& ref;
    Foo() : ref(42) {} 
    // The temporary '42' is destroyed when the constructor ends.
    // 'ref' is a dangling reference inside every Foo object.
};

Foo f;
// f.ref;   // Undefined behavior — refers to destroyed memory
```
Lifetime extension via references (`const T&` and `T&&`) only works for local stack variables. It does not extend lifetimes when bound to class member variables.