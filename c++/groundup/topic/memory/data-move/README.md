## Copying Data: Stack vs. Heap
When data needs to be duplicated or transferred from one variable to another, the performance and mechanism depend entirely on where the underlying data resides.

To be precise at the hardware level, the CPU always copies whatever bits are stored inside the memory slot of the source variable, regardless of what those bits represent.

### Copying Stack Data
Variables on the stack have fixed, explicit sizes known at compile time.
```c
int a = 10;
int b = a; // Stack copy
```
When the variable is a primitive (`int b = a;`)
* The slot for `a` on the stack holds the actual integer value (e.g., the number 10).
* The CPU reads those bits from a's stack location into a register and writes them into b's stack location.
* Result: The integer value itself is duplicated on the stack.

### Copying Heap Data: Shallow Copy vs. Deep Copy
When dealing with dynamically allocated heap memory via pointers, copying becomes a multi-layered problem. Consider a pointer `p1` pointing to a block of heap memory containing data:

```c
int *p1 = malloc(sizeof(int) * 1000); // Heap allocation of 1000 integers
```

There are two distinct ways to "copy" this setup into a second pointer, p2.

#### Shallow Copy
```c
int *p2 = p1; // Shallow copy
```
* The slot for `p1` on the stack does not hold the target data; it holds a  memory address (the pointer value).
* The CPU reads those address bits from `p1`'s stack location into a register and writes them into `p2`'s stack location.
* Result: The memory address is duplicated on the stack. The underlying data on the heap is never touched by the CPU during this operation; both pointers simply end up containing the same address bits.

#### Deep Copy
A deep copy duplicates the entire underlying block of data on the heap.
```c
int *p2 = malloc(sizeof(int) * 1000); // Allocate a brand new heap block
for(int i = 0; i < 1000; i++) {
    p2[i] = p1[i];                    // Copy every single element across
}
```

#### Array on stack
When working with primitives and pointers, assignment copies the bits from one stack slot to another. This naturally leads to the question of what happens when attempting to assign one array to another:
```c
int a[] = {1, 2};
int b[2];
b = a; // Compilation Error
```
If the code above is compiled, the compiler will throw an error such as: `error: assignment to expression with array type`

To understand why, `a` is not a regular variable, when declared as array variable name is considered as an address of the starting location `a == &a[0]`

### The Concept of "Moving" a Value in C
What happens if a program no longer needs the data inside p1, but wants p2 to take over ownership of that large heap allocation?

Performing a Deep Copy is highly wasteful here because the program would allocate a new block and copy 1,000 integers, only to delete the original block immediately after.

To solve this, the concept of a Move is used. In pure C, a move is accomplished manually by manipulating pointers:

```c
int *p1 = malloc(sizeof(int) * 1000); // p1 owns the heap resource

// The Move Operation
int *p2 = p1;  // Step 1: Hand over the heap address to p2 (Shallow copy)
p1 = NULL;     // Step 2: Sever p1's connection to the heap address
```