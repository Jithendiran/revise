## Dynamic Storage Duration — The Heap
### Why the Heap Exists
The stack has two limitations the heap solves:

1. **Objects that must outlive their creation scope.** A function that creates a `BankAccount` and returns it to the caller cannot return a stack object — the stack frame is destroyed when the function returns. A heap object exists until explicitly deleted, independent of any scope.

2. **Objects whose size is not known at compile time.** Stack allocation requires knowing the size at compile time (the compiler must subtract the right amount from the stack pointer). Heap allocation happens at runtime and can use any size computed at that moment.


```cpp
// Stack: size must be known at compile time
int arr[5];       // VALID: 5 is a compile-time constant
// int arr[n];    // Invalid in standard C++ — n is runtime (use vector instead)

// Heap: size can be a runtime value
int n = getUserInput();
int* arr = new int[n];   // VALID: n is a runtime value
```

### `new` — Allocation and Construction

`new` performs two operations in sequence:
1. Requests a block of memory from the heap large enough to hold the object
2. Calls the appropriate constructor to initialize the object at that address

The result is a **pointer** to the constructed object.

```cpp
BankAccount* p = new BankAccount(101, 1000.0);
//                 ↑                ↑
//                 allocates heap   calls parameterized constructor
//                 memory for one   with these arguments
//                 BankAccount

// p holds the heap address
// *p is the BankAccount object
// p->deposit(100.0) — access via arrow operator
```

### `new` vs `malloc`

`malloc` (from C) only allocates memory — it does not call any constructor. Using `malloc` for C++ class objects produces uninitialized memory with no constructor having run. This is undefined behavior for any non-trivial class.

```cpp
BankAccount* p = (BankAccount*)malloc(sizeof(BankAccount));
// memory allocated — but no constructor ran
// p->deposit(100.0);   // undefined behavior — object not properly constructed

BankAccount* q = new BankAccount(101, 1000.0);
// memory allocated AND constructor ran — object is properly initialized
```

`new` is the correct mechanism for C++ objects. `malloc` is for raw byte buffers in C-compatible code.

### `delete` — Destruction and Deallocation

`delete` performs two operations in sequence:
1. Calls the object's destructor
2. Returns the memory block to the heap

```cpp
BankAccount* p = new BankAccount(101, 1000.0);
p->deposit(100.0);
delete p;   // destructor runs, then memory is freed
            // p is now a dangling pointer — holds the old address
            // that address no longer belongs to the program
p = nullptr; // set to nullptr immediately after delete — prevents accidental use
```

**`delete` vs `free`:** `free` (from C) only releases memory — it does not call the destructor. Using `free` on a C++ object skips cleanup, leaking any resources the destructor would have released (open files, nested heap allocations, etc.).


### Array Allocation — `new[]` and `delete[]`

Allocating an array on the heap requires `new[]`, and the corresponding release requires `delete[]`. Using `delete` (without `[]`) on a heap array
is undefined behavior — it calls the destructor only for the first element.

```cpp
int n = 10;
BankAccount* accounts = new BankAccount[n];   // default constructor for each element
                                               // n must have a default constructor

accounts[0].deposit(100.0);
accounts[1].deposit(200.0);

delete[] accounts;   // destructor for each element in reverse order, then memory freed
// delete accounts;  // WRONG: undefined behavior — only destructs first element
```
**Caveat**

* `delete[]` knows the array size, when `new` is called size is stored internally
* when using array in heap programmer are forced to use default constructor

**Workarounds to use constructors**
1. Array Initializer List
    ```cpp
    int n = 3;

    // Allocates array AND constructs each element with arguments
    BankAccount* accounts = new BankAccount[3]{
        BankAccount(100.0),
        BankAccount(200.0),
        BankAccount(300.0)
    };

    accounts[0].deposit(50.0);

    // Standard array delete handles cleanup for all 3
    delete[] accounts;
    ```
2. Array of Pointers
    ```cpp
    int n = 10;

    // Step 1: Allocate an array of BankAccount POINTERS (no constructors called yet)
    BankAccount** accounts = new BankAccount*[n];

    // Step 2: Allocate each BankAccount individually with any arguments want
    for (int i = 0; i < n; ++i) {
        accounts[i] = new BankAccount((i + 1) * 100.0); // Parameterized constructor
    }

    // Access elements using '->' (pointer to object)
    accounts[0]->deposit(50.0);

    // Step 3: Cleanup requires a loop (delete each object, then the pointer array)
    for (int i = 0; i < n; ++i) {
        delete accounts[i];  // Calls destructor for each BankAccount
    }
    delete[] accounts;      // Frees the array of pointers
    ```
### Memory Leaks

A memory leak occurs when a heap object is never deleted. The memory block remains allocated for the lifetime of the program — unavailable for reuse — even though no pointer to it exists.

```cpp
void leak() {
    BankAccount* p = new BankAccount(101, 1000.0);
    // ... work ...
    return;   // p goes out of scope — heap memory never freed
              // the BankAccount object still exists on the heap
              // but nothing points to it — it can never be freed
}
```
Repeated leaks accumulate, exhausting available memory. The program either crashes or the operating system terminates it.

### 3.7 Dangling Pointers

A dangling pointer holds an address to memory that has already been freed. Dereferencing it is undefined behavior — the memory may have been reused for a different object, or may contain garbage.

```cpp
BankAccount* p = new BankAccount(101, 1000.0);
delete p;

p->deposit(100.0);   // UNDEFINED BEHAVIOR: the object no longer exists
                     // the memory may now hold anything
```

**Prevention:** Immediately set the pointer to `nullptr` after `delete`. Dereferencing `nullptr` produces a defined crash (segmentation fault) rather than silent corruption.

```cpp
delete p;
p = nullptr;
// p->deposit(100.0);   // crash immediately — nullptr dereference is detectable
```

### `new` Failure — `std::bad_alloc`

If the heap cannot satisfy the request (insufficient memory), `new` throws `std::bad_alloc`. If not caught, the program terminates.

```cpp
try {
    int* p = new int[1000000000];   // may fail if not enough memory
} catch (const std::bad_alloc& e) {
    std::cerr << "Allocation failed: " << e.what() << "\n";
}
```

The **nothrow** form returns `nullptr` instead of throwing:

```cpp
int* p = new(std::nothrow) int[1000000000];
if (p == nullptr) {
    // allocation failed — handle it
}
```