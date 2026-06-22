## Inline
To understand the purpose of the `inline` keyword, the standard mechanism by which a computer executes a function call must first be understood.

Normally, code execution follows a sequential path. When a function is called, the CPU must pause its current progress and jump to a completely different location in memory where the function's instructions are stored. This process involves precise hardware overhead:

1. `Arguments Placement`: The current function arguments are copied into CPU registers or onto the system memory stack.
2. `Instruction Pointer Jump`: The CPU stores the address of the next instruction (the return address) and jumps to the target function's memory address.
3. `Execution`: The instructions inside the called function are executed.
4. `Clean-up and Return`: The return value is saved, the local function memory is cleaned up, and the CPU jumps back to the stored return address to resume the original sequence.
For small functions (such as a simple one-line mathematical calculation), the time spent performing this structural "jumping" overhead can actually take longer than executing the function's core instructions themselves.

### The Definition and Purpose of `inline`
The `inline` keyword is a hint given to the C++ compiler. It requests that the compiler replace a function call site directly with the actual body code of the function, eliminating the hardware jump entirely.

```cpp
inline int square(int x) {
    return x * x;
}

int main() {
    int result = square(5);
    return 0;
}
```
When the compiler processes this code with inline optimizations enabled, it transforms the logic during compilation so that the final binary executes as if it were written like this:
```cpp
int main() {
    // The function call 'square(5)' is deleted and replaced with the body
    int result = 5 * 5; 
    return 0;
}
```

The fundamental purpose of inlining is to eliminate the runtime overhead of function calls for small, frequently used functions. This increases execution speed.

However, it comes with a trade-off called code bloat. If a large function is marked inline and called in 100 different places throughout a program, the compiler will duplicate that large chunk of machine code 100 times into the final binary file, drastically increasing the size of the executable program.
