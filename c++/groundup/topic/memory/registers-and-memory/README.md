## Hardware Mechanics — Registers, Memory, and Persistence

Let's break down exactly what happens under the hood with these two lines of code:

```c
int a = 4;
int b = 1 + 2 + 3;
int *p = malloc(sizeof(int));
```

### Code Segment

Before  program runs, it is compiled into machine code (binary instructions) and loaded into a specific area of RAM called the Code Segment (or Text Segment).

#### Line 1

`int a = 4;`

The number `4` in  code is not a variable yet. It is just a hardcoded piece of data written directly inside the machine instruction itself. It is text data baked into the executable program.

When the CPU executes this line, it performs two distinct steps:
1. **Moving to a Register**
    The CPU cannot move data directly from the Code Segment into a specific RAM variable location in one step. It first executes an instruction like:

    ```asm
    mov rax, 4
    ```

    * The `rax` Register: This is a high-speed, internal memory slot inside the CPU.
    * The Value is a Copy: The CPU reads the `4` from the code segment and creates a copy of it inside the `rax` register. The original `4` stays in the code segment.
    * Why Registers are Temporary: The `rax` register cannot be accessed directly using a variable name or memory address. If the CPU performs another calculation next, the data in `rax` is overwritten. Once overwritten, that specific `4` is gone permanently due to a complete lack of persistence.
  
2. **Saving to the Stack**
    To make this data persistent so program can use it later, the CPU must copy it out of the register and into RAM. It uses the Stack frame pointer (`rbp`) to find the variable's address:

    ```asm
    mov [rbp - <variable_start_address>], rax
    ```
    For now keep `variable_start_address` as 0x0

    ```asm
    mov [rbp - 0x0], rax
    ```

    * Now, the value `4` is written into a dedicated memory slot on the Stack.
    * What is an Address? An address is a unique numerical index for a byte in RAM. Because this stack location has a permanent address, the compiler maps the name a to this address. Program can read, modify, and reference a anytime later because its location in RAM remains fixed.
  
### Line 2

```c
int b = 1 + 2 + 3;
```
When evaluating mathematical expressions, the CPU uses registers as a temporary scratchpad.

```asm
mov rax, 1      ; Copies 1 into register rax
add rax, 2      ; Adds 2 to rax (rax is now 3)
add rax, 3      ; Adds 3 to rax (rax is now 6)
```

1. While this calculation is happening, the individual numbers `1`, `2`, and `3` only exist inside the CPU instructions and temporary registers.
2. When the calculation finishes, the final sum `6` exists only inside the temporary `rax` register.
3. Finally, the CPU copies the 6 from the register into the stack memory address reserved for b

    (size of int is 4, bytes where a occupies bytes 0-3 and b occupies bytes 4-7)
    ```asm
    mov [rbp - 0x4], rax
    ```

4. Only after this step does the value `6` become persistent and accessible via the variable `b`.

### Line 3

```c
int *p = malloc(sizeof(int));
```

This line creates a variable that stores a memory address.
1. The Request: The CPU executes the malloc function. malloc finds an available 4-byte block of memory in a region of RAM called the Heap.
2. Temporary Address Storage: malloc takes the starting memory address of that Heap block and returns it. The CPU immediately places this address into a temporary register (usually `rax`). At this exact moment, the address is only held in temporary memory.
3. The Persistence Copy: To save this address, the CPU executes a move instruction to copy the address out of the register and into the stack memory location reserved for the pointer variable `p`
    ```asm
    mov [rbp - 0x8], rax
    ```
4. The Result: The pointer variable `p` lives on the Stack and has its own stack address. The value stored inside `p` is the address pointing to the Heap memory block.