# `noexcept`

## The Problem — What Happens When a Destructor Throws

To understand why `noexcept` exists, the exact failure it prevents must be understood first.

When an exception is thrown, the runtime begins **stack unwinding** — it walks back up the call stack, destroying every local object in every scope it exits, calling each destructor in reverse order.

Now consider what happens if one of those destructors throws its own exception during unwinding:

```cpp
class Resource {
public:
    ~Resource() {
        throw std::runtime_error("cleanup failed");  // throws during unwinding
    }
};

void process() {
    Resource r;
    throw std::runtime_error("original error");  // starts unwinding
    // unwinding calls r's destructor
    // destructor throws a second exception
    // C++ runtime: two live exceptions simultaneously — impossible to handle
    // result: std::terminate() — program crashes immediately
}
```

C++ has no mechanism for handling two simultaneous exceptions. The moment a second exception is thrown during stack unwinding, `std::terminate` is called and the program ends. There is no recovery.

This is not a hypothetical edge case. It is a fundamental constraint of the exception model. A destructor that throws is capable of terminating any program that uses it, including programs that otherwise handle all their exceptions correctly.


## What `noexcept` Is

`noexcept` is a specifier placed on a function declaration that makes a guarantee: **this function will never throw an exception**.

```cpp
void cleanup() noexcept {
    // guaranteed: no exception will propagate out of this function
}
```

If a `noexcept` function does throw internally and that exception attempts to propagate out, the runtime calls `std::terminate` immediately — the same crash, but now it is a deliberate, documented contract violation rather than a silent accident.

`noexcept` does two things:

1. **Documents the guarantee** — the function signature itself states the contract. A caller reading the declaration knows no exception handling is needed.
2. **Enables compiler optimizations** — the compiler can generate more efficient code when it knows a function cannot throw, because it does not need to generate exception unwinding tables for those call sites.

## `noexcept` on Destructors

### Destructors Are Implicitly `noexcept`

In C++11 and later, **every destructor is implicitly `noexcept`** unless the destructor's body or a base class/member destructor explicitly violates this. The compiler marks destructors `noexcept` automatically.

```cpp
class Simple {
public:
    ~Simple() {}   // implicitly noexcept — compiler adds it automatically
};
```
This is a language-level decision. Destructors must not throw. Making them implicitly `noexcept` means the compiler enforces this even when the programmer forgets to write it.

#### How can be violated
1. Explicit Violation (The Direct Way)
```cpp
class ExplicitThrower {
public:
    // Explicitly overriding the implicit noexcept
    ~ExplicitThrower() noexcept(false) {
        if (something_went_wrong) {
            throw std::runtime_error("Exception in destructor!");
        }
    }
};
```
2. Implicit Violation (The Chain Reaction)
```cpp
class BadClass {
public:
    ~BadClass() noexcept(false) {} // This one explicitly allows throwing
};

class Violator {
    BadClass member; // Member can throw on destruction!
public:
    // ~Violator() is IMPLICITLY noexcept(false) because of 'member'
};
```
3. Throwing from a Function Called Inside the Destructor
```cpp
void riskyFunction() {
    throw std::runtime_error("Oops");
}

class ContractBreaker {
public:
    ~ContractBreaker() { // Implicitly noexcept(true)
        riskyFunction(); // This will compile, but...
    }
};
```

If a destructor throws an exception while the program is already processing another exception (stack unwinding), C++ will immediately crash program via `std::terminate()`.

### Writing It Explicitly

Writing `noexcept` explicitly on a destructor is self-documenting — it makes the guarantee visible in the code without requiring the reader to know the implicit rule:

```cpp
class FileHandle {
    FILE* file;
public:
    ~FileHandle() noexcept {
        if (file) fclose(file);
    }
};
```

### Handling Errors Inside a `noexcept` Destructor

Cleanup operations can fail. A file close can fail. A network disconnect can fail. Inside a `noexcept` destructor, these failures must be absorbed — the exception must not escape.

```cpp
class NetworkConnection {
    int socket;
public:
    ~NetworkConnection() noexcept {
        try {
            closeSocket(socket);    // might throw internally
        } catch (...) {
            // absorb — log if possible, but never propagate
            // letting it escape would call std::terminate
        }
    }
};
```

The `catch (...)` catches every possible exception type. The error can be logged but must not be rethrown.


## `noexcept` on Other Functions

`noexcept` is not limited to destructors. It applies to any function.

### Regular Functions

```cpp
int add(int a, int b) noexcept {
    return a + b;   // arithmetic — cannot throw
}

void swapInts(int& a, int& b) noexcept {
    int temp = a;
    a = b;
    b = temp;
}
```

### Move Constructor

When a `std::vector` runs out of capacity, it must allocate a larger internal array and transfer all existing elements from the old array to the new one.

To maintain the strong exception guarantee, `std::vector` ensures that if an error occurs during this transfer, the operation can be aborted without losing or corrupting the original data.

#### The Performance Penalty of Omitting noexcept
The choice between a move operation and a copy operation depends entirely on the presence of the `noexcept` keyword.

1. With `noexcept` (Fast: $O(1)$ per element)
   * When the move constructor is marked `noexcept`, `std::vector` knows the transfer cannot fail. It transfers ownership of each element by simply swapping internal pointers.
   * Cost: Copying a pointer takes constant time ($O(1)$), regardless of how much data the pointer points to.

2. Without noexcept (Slow: $O(n)$ per element)
   * If the move constructor lacks `noexcept`, `std::vector` assumes the operation might throw an exception. If an exception occurred halfway through moving elements, the old array would be partially destroyed, resulting in data loss.
   * To prevent this risk, the vector falls back to using the copy constructor. Cost: Instead of swapping pointers, the program must allocate entirely new memory blocks and duplicate all the underlying data. For a buffer containing $n$ items, a deep copy requires linear time ($O(n)$).

```cpp
#include <iostream>
#include <vector>
#include <utility>

class Buffer {
private:
    int* data;
    size_t size;

public:
    // Constructor
    Buffer(size_t s) : size(s) {
        data = new int[size];
    }

    // Destructor
    ~Buffer() {
        delete[] data;
    }

    // Copy Constructor (Deep Copy)
    Buffer(const Buffer& other) : size(other.size) {
        std::cout << "[COPY] Deep copying " << size << " elements.\n";
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Move Constructor
    // TOGGLE NOEXCEPT HERE: Remove 'noexcept' to see the vector switch to COPY during resize.
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        std::cout << "[MOVE] Fast pointer swap -- size " << other.size << "\n";
        other.data = nullptr;
        other.size = 0;
    }
};

int main() {
    std::vector<Buffer> buffers;
    
    // Set initial capacity to 1 to force a reallocation on the second push
    buffers.reserve(1); 

    std::cout << "--- Inserting first element ---\n";
    buffers.push_back(Buffer(100));

    std::cout << "\n--- Inserting second element (Forces Vector Resize) ---\n";
    // The vector is full. It must allocate a larger block and transfer the first element.
    buffers.push_back(Buffer(200)); 

    return 0;
}
```

**with noexcept**

```
--- Inserting first element ---
[MOVE] Fast pointer swap -- size 100 <-- (Call 1: Buffer(100) is a rvalue so calls move, enters vector)

--- Inserting second element (Forces Vector Resize) ---
[MOVE] Fast pointer swap -- size 200 <-- (Call 2: Directly construct the new element in the new allocated memory)
[MOVE] Fast pointer swap -- size 100 <-- (Call 3: Move the 1st element from old memory to new memory)
```

**without noexcept**
```
--- Inserting first element ---
[MOVE] Fast pointer swap -- size 100

--- Inserting second element (Forces Vector Resize) ---
[MOVE] Fast pointer swap -- size 200
[COPY] Deep copying 100 elements.
```

## The `noexcept` Operator — Checking at Compile Time

`noexcept` also exists as an **operator** (distinct from the specifier) that queries at compile time whether an expression is guaranteed not to throw. It returns `true` or `false` as a compile-time boolean.

```cpp
int add(int a, int b) noexcept { return a + b; }
int risky(int a, int b) { return a / b; }   // could throw

static_assert(noexcept(add(1, 2)));      // true — function is noexcept
static_assert(!noexcept(risky(1, 2)));   // false — function is not noexcept
```

## `noexcept` vs `throw()` — Old Syntax

Before C++11, a function that promised not to throw used `throw()`:

```cpp
void cleanup() throw() {}   // C++03 — do not throw
```

`throw()` was deprecated in C++11 and removed in C++17. `noexcept` replaces it entirely. `noexcept` is more efficient — `throw()` required the runtime to catch any thrown exception and call `std::unexpected`, adding overhead. `noexcept` directly calls `std::terminate` with zero overhead.


## What Happens When `noexcept` Is Violated

If a `noexcept` function allows an exception to escape, the runtime calls `std::terminate` immediately. The stack is **not** unwound — destructors for local objects in the `noexcept` function may not run.

```cpp
void guaranteed() noexcept {
    Resource r;                              // constructor runs
    throw std::runtime_error("oops");        // exception escapes noexcept boundary
    // std::terminate() called immediately
    // r's destructor may NOT run — undefined
}
```

This is intentional. `noexcept` is a hard contract. Violating it is a program error, not a recoverable condition. The immediate termination prevents the program from continuing in an undefined state.