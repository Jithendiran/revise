# `noexcept`

## 1. The Problem — What Happens When a Destructor Throws

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


## 2. What `noexcept` Is

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

## 3. `noexcept` on Destructors

### 3.1 Destructors Are Implicitly `noexcept`

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

### 3.2 Writing It Explicitly

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

### 3.3 Handling Errors Inside a `noexcept` Destructor

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


## 4. `noexcept` on Other Functions

`noexcept` is not limited to destructors. It applies to any function.

### 4.1 Regular Functions

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

### 4.2 Move Constructor and Move Assignment

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
        std::cout << "[MOVE] Fast pointer swap.\n";
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
[MOVE] Fast pointer swap.  <-- (Call 1: Buffer(100) enters vector)

--- Inserting second element (Forces Vector Resize) ---
[MOVE] Fast pointer swap.  <-- (Call 2: Vector transfers Buffer(100) to new memory)
[MOVE] Fast pointer swap.  <-- (Call 3: Buffer(200) enters vector)
```

**without noexcept**
```
--- Inserting first element ---
[MOVE] Fast pointer swap.

--- Inserting second element (Forces Vector Resize) ---
[MOVE] Fast pointer swap.
[COPY] Deep copying 100 elements.
```