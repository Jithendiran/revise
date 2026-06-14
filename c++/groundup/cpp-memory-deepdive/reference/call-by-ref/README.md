## Function call by reference
Every time a function is called, a new frame is pushed onto the Stack. How data moves into that frame determines whether a program is fast, slow, safe, or complex.

### Pass-by-Value (Stack Duplication)
By default, C passes arguments by value. When a variable is passed into a function, the CPU duplicates the bits from the caller's stack frame into the callee's stack frame.
```c
void modify_value(int local_x) {
    local_x = 99; 
}

int main() {
    int a = 10;
    modify_value(a);
    // a is still 10 here
}
```

**Hardware Mechanics:**
1. The stack slot for `a` contains the integer value `10`.
2. When `modify_value(a)` is called, the CPU copies the value `10` into a temporary register, then writes it into a brand new stack slot allocated for `local_x`.
3. Inside the function, `local_x = 99` overwrites the memory slot of `local_x`.
4. The original variable `a` remains completely untouched because it resides at an entirely different stack address.

If the argument is a massive `struct` instead of a 4-byte integer, the CPU must sequentially copy every single byte across the stack. This introduces massive, unnecessary performance overhead.

### Pass-by-Pointer
To avoid copying large structures, or to allow a function to modify an outside variable, C programmers must pass a memory address instead of the value.

```c
void modify_with_pointer(int *p) {
    if (p != NULL) {
        *p = 99; // Dereferencing to access the original stack slot
    }
}

int main() {
    int a = 10;
    modify_with_pointer(&a); 
    // a is now 99
}
```

**Hardware Mechanics:**
1. `&a` evaluates to the persistent stack address of `a`.
2. This address is copied into the stack slot allocated for the pointer variable `p` inside the function's frame.
3. To modify the value, the CPU must read the address stored in p, use it to look back at the caller's stack frame, and overwrite the memory slot of `a`.

### References
A Reference (`&`) was introduced in C++ as an implicit, secure abstraction over a pointer. It provides the exact performance benefits of pass-by-pointer but uses the clean syntax of pass-by-value.

```c
void modify_with_reference(int &ref) {
    ref = 99; // Clean syntax: no '*' needed
}

int main() {
    int a = 10;
    modify_with_reference(a); // Clean syntax: no '&' needed
    // a is now 99
}
```
**Hardware Mechanics:**

At the hardware level, a reference does not exist. The compiler converts a reference into a constant pointer (`type *const`) that is automatically dereferenced every time it is accessed. The assembly instructions generated for `modify_with_reference` are identical to `modify_with_pointer`.