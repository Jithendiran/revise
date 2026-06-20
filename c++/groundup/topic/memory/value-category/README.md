## Value Categories in c
By analyzing how the CPU uses temporary registers (rax, rbx,..) and persistent RAM addresses (stack, heap), a clear boundary can be drawn between two categories of values in C. These categories dictate what can be done with a value in code.

### Value Categories: lvalue vs. prvalue
Every expression in a C program evaluates to a value that falls into one of two fundamental categories based on its memory persistence.

* lvalue (Locator Value)
  An lvalue is an expression that refers to a persistent memory location (RAM).
  * Characteristics: It has a identifiable, accessible memory address. Because it occupies a stable home on the Stack or the Heap, its address can be taken using the address-of operator (&).
  * Example: In `int a = 4;`, the variable `a` is an lvalue. It names a specific, persistent location in RAM that can be read from or written to repeatedly.
  
  Variables aren't the only lvalues. Anything that dereferences a pointer or looks up memory results in an lvalue:
  * Pointer Dereferencing: `*ptr` is an lvalue because it points to a specific spot in RAM.
  * Array Indexing: arr[5] is an lvalue.
  * Struct Members: `user.age` or `ptr->age` are lvalues.
  * This is also lvalue, not an rvalue. When define "Hello", the compiler doesn't put that in a CPU register; it allocates it in the read-only data segment of RAM.
    ```c
    char *ptr = &"Hello"[0]; // Legal! can take the address of a character in a string literal.
    ```
* prvalue (Pure Rvalue)
  A prvalue is an expression that represents a temporary, transient value.
  * Characteristics: It does not have a persistent memory address. It exists strictly inside a temporary CPU register or is baked directly into the machine code instruction itself. Its address cannot be taken (& is illegal) because it has no location in RAM.
  * Example: In `a = 1 + 2 + 3;`, the literal numbers 1, 2, 3, and their calculated sum 6 are all prvalues. They exist momentarily in flight within the CPU registers before being discarded or copied out.
  * If a function returns a value (e.g., `int posX()`), that returned value is a prvalue. It is handed back in a CPU register (like rax).
  * In C, both `i++` and `++i` result in prvalues (unlike C++, where ++i is an lvalue). 
  