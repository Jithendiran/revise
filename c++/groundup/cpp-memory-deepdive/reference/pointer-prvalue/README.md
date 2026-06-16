## Pointer PRvalue Reference
An prvalue reference to a pointer allows the capture and modification of a temporary pointer value (a prvalue).

### 1. Binding (Initialization)
Binding happens only when the prvalue reference to the pointer is created. It must bind to a temporary pointer expression (a prvalue pointer) or an explicit rvalue.
```cpp
int x = 10;
int &&r = 5; // regular rvalue reference
int *&&ref_to_ptr = &x;
```
* What happens: The expression `&x` generates a temporary, un-named pointer value containing the address of `x`. The compiler allocates a hidden, temporary slot in memory to store this address, and then binds `ref_to_ptr` to that hidden slot.
* The Rule: `ref_to_ptr` can only bind to temporary pointer values (prvalues). Once bound, the lifetime of that hidden temporary pointer slot is extended to match the lifetime of `ref_to_ptr`.

### 2. Assignment (Two Levels of Manipulation)
Once the prvalue reference to the pointer is initialized, it behaves like a named variable (an lvalue expression). It can manipulate either the pointer's destination or the underlying data.

**Level A: Changing the Address (Modifying the Temporary Pointer)**
```cpp
int y = 20;
ref_to_ptr = &y;
```
* What happens: The compiler overwrites the memory address inside the hidden temporary pointer slot with the address of `y`.
* The Result: The reference has switched its target. The hidden pointer slot no longer holds the address of `x`; it now holds the address of `y`.

**Level B: Changing the Value (Dereferencing)**
```cpp
*ref_to_ptr = 30;
```
* What happens: The compiler looks at the address stored inside the hidden temporary pointer slot (which is now the address of `y`) and overwrites that memory location with `30`.
* The Result: The value of `y` becomes `30`.

### What Can Bind to int *&&?
It can only bind to temporary pointer values that lack a permanent address in memory (prvalues).
```cpp
int x = 10;
int *&&ref1 = &x;      // Allowed. '&x' is a prvalue pointer generated on the fly.
int *&&ref2 = nullptr; // Allowed. 'nullptr' is a literal prvalue pointer.
```

### What Cannot Bind to int *&&?
It cannot bind to an existing pointer variable that has a permanent name and memory location (lvalues).
```cpp
int x = 10;
int *ptr = &x;

// int *&&ref = ptr; // Error! 'ptr' is an lvalue pointer variable.
```

### Function Arguments and Returns

**1. Accepting Temporary Pointers Safely**
An prvalue reference to a pointer can be used to intercept dynamically allocated pointers directly from a function call or a temporary expression.
```cpp
void inspectAddress(int *&&p) {
    // p holds the temporary address passed into the function
    if (p != nullptr) {
        *p = 50; 
    }
}

// In main:
int x = 10;
inspectAddress(&x); // Allowed. '&x' is a prvalue. 'x' becomes 50.
```
**2. The Trap: Returning a Pointer PRvalue Reference**
Returning an `int *&&` from a function to a local temporary pointer results in undefined behavior once the function returns.
```cpp
int*&& getInvalidPointerReference() {
    int x = 10;
    return &x; // INVALID/DANGEROUS! Returns a reference to a temporary pointer 
               // that points to a local variable which is about to be destroyed.
}

int *&&ref = getInvalidPointerReference(); // Undefined Behavior!
```

### Member Variable Binding in a Constructor
Binding a temporary pointer to an `int *&&` member inside a constructor does not extend the lifetime of the temporary pointer slot past the closing brace of the constructor.
```cpp
struct Foo {
    int *&&ptr_ref;
    Foo(int *p) : ptr_ref(p + 1) {} 
    // The temporary pointer calculated by 'p + 1' is destroyed when the constructor ends.
    // 'ptr_ref' becomes a dangling reference to a dead temporary slot.
};
```

## Pointer and const
When dealing with prvalue references to pointers (`*&&`), qualifiers can be placed in two distinct positions to control mutability:
* To the left of the asterisk (`const int *`): Controls whether the data being pointed to can be modified.
* To the right of the asterisk (`* const &&`): Controls whether the reference itself can be assigned a new address after initialization (though, as an prvalue reference, it still must bind to a prvalue initially).

Here are all four combinations of `const`, `*`, and `&&`.

### 1. Non-Const Pointer to Non-Const Data (int *&&)
Both the temporary pointer's destination address and the underlying data can be modified freely.
```cpp
int x = 10;
int y = 20;

int *&&ref = &x; // Binds to a prvalue pointer pointing to x

ref = &y;        // ALLOWED! The temporary pointer slot now points to y.
*ref = 30;       // ALLOWED! The value of y becomes 30.
```

### 2. Non-Const Pointer to Const Data (const int *&&)
The temporary pointer can be changed to point to a different address, but the underlying data is read-only when accessed through this reference.

```cpp
int x = 10;
int y = 20;

const int *&&ref = &x; // Binds to a prvalue pointer pointing to x

ref = &y;              // ALLOWED! The temporary pointer slot now points to y.
// *ref = 30;          // ERROR! The data is const. Cannot modify the destination.
```

### 3. Const Pointer to Non-Const Data (int * const &&)
The underlying data can be modified, but the temporary pointer slot itself is marked const. The address it holds cannot be changed after initialization.
```cpp
int x = 10;
int y = 20;

int * const &&ref = &x; // Binds to a prvalue pointer pointing to x

*ref = 30;              // ALLOWED! The value of x becomes 30.
// ref = &y;            // ERROR! The pointer component is const. Cannot change the address.
```

### 4. Const Pointer to Const Data (const int * const &&)
This creates a completely read-only window. The address held by the temporary pointer slot cannot be changed, and the underlying data cannot be modified.
```cpp
int x = 10;
int y = 20;

const int * const &&ref = &x; // Binds to a prvalue pointer pointing to x

// ref = &y;                  // ERROR! The pointer component is const.
// *ref = 30;                 // ERROR! The data component is const.
```

### Pointer to reference
**Pointer to reference is not allowed**

```cpp
int &&*              // pointer to rvalue ref  - illegal
int&& &&             // ref to rvalue ref
int& &&               //illegal in written code (but reference collapsng exists in template)    
```