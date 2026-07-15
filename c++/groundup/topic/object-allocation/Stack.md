## Automatic Storage Duration — The Stack
An object declared inside a function, block, or scope is placed on the stack. The compiler determines its address at compile time. The object is constructed when execution reaches its declaration and destroyed automatically when the enclosing scope exits. No manual cleanup is required.

```cpp
void processAccount() {
    BankAccount acc(101, 1000.0);   // constructed here — stack slot allocated
    acc.deposit(100.0);
}   // acc destroyed here automatically — destructor called, stack slot released
```

### Why the Stack Is Fast
The stack is a contiguous block of memory managed by a single pointer called the **stack pointer**. Allocating stack memory is a single CPU instruction — subtract from the stack pointer to reserve space. No searching for free blocks, no bookkeeping, no system calls. The entire stack frame for a function is reserved in one operation when the function is entered.

```asm
; Function entry:
sub rsp, 0x20    ; reserve 32 bytes — one instruction allocates all local variables
; Function exit:
add rsp, 0x20    ; release all — one instruction frees everything
```

### Stack Size Limit

The stack is fixed in size — typically 1 MB to 8 MB depending on the platform and operating system settings. Exceeding this limit causes a **stack overflow**, which terminates the program.

Large objects — arrays of millions of elements, deeply nested recursive calls, large structs — must not live on the stack. They belong on the heap.

```cpp
int bigArray[1000000];   // 4 MB on the stack — likely stack overflow
```

### Scope and Lifetime
Every stack object is tied to a scope — the pair of `{` `}` braces that contains its declaration. The object is destroyed when execution leaves that scope, regardless of how it is left (normal exit, `return`, `break`, or exception).
```cpp
void example() {
    {
        BankAccount inner(101, 500.0);   // constructed
    }                                    // inner destroyed here — scope exits

    // inner is gone — cannot be accessed here
}
```
Multiple objects in the same scope are destroyed in **reverse declaration
order** — last declared, first destroyed.

```cpp
void example() {
    BankAccount a(101, 100.0);   // constructed first
    BankAccount b(102, 200.0);   // constructed second
    BankAccount c(103, 300.0);   // constructed third
}
// destroyed: c first, then b, then a — reverse order
```

### All Ways to Create a Stack Object
A stack object is created simply by declaring it. There is no separate "create" operation — the declaration is the creation.
```cpp
BankAccount acc;                           // default constructor
BankAccount acc(101, 1000.0);              // parameterized constructor
BankAccount acc{101, 1000.0};              // brace initialization (see Initialization doc)
BankAccount acc = BankAccount(101, 1000.0); // copy initialization (elided in practice)

const BankAccount acc(101, 1000.0);        // const object — all modifying calls blocked
BankAccount arr[3];                        // array of 3 — default constructor for each
```

**Why would a new object be created mid-scope?**

A new scope can be opened anywhere using `{` `}`. This is done intentionally to control exactly when an object's destructor runs — before the enclosing function exits.

```cpp
void processFile() {
    // ... setup work ...

    {
        FileHandle fh("data.txt");    // constructor opens the file
        fh.readAll();
    }   // destructor closes the file here — not at end of processFile()

    // file is guaranteed closed here, before the rest of the function runs
}
```

This is the core of **RAII (Resource Acquisition Is Initialization)** — a resource is tied to an object's lifetime, and that lifetime is controlled by scope. When the scope exits, the destructor runs and the resource is released. No manual cleanup, no possibility of forgetting to release.
