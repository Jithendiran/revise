# Exception Handling
## The Problem — Error Codes Are Not Enough
Before exceptions, errors were communicated through return values. Every function that could fail returned a status code. The caller checked the code and decided what to do.
```cpp
int openFile(const char* path) {
    // returns 0 on success, -1 on failure
}

int readData(int fileHandle, char* buffer, int size) {
    // returns bytes read, -1 on failure
}

int processData(char* buffer, int size) {
    // returns 0 on success, -1 on failure
}

// Using these functions:
int handle = openFile("data.txt");
if (handle == -1) {
    // handle error
    return -1;   // propagate error up
}

int bytes = readData(handle, buffer, 1024);
if (bytes == -1) {
    closeFile(handle);   // must remember to clean up
    return -1;
}

if (processData(buffer, bytes) == -1) {
    closeFile(handle);   // must remember to clean up again
    return -1;
}
```
Three problems with this approach:

1. **Error propagation is manual.** Every function in the call chain must check the return value and pass the error upward. Missing one check silently ignores the error.

2. **Cleanup is manual and error-prone.** Every early return requires releasing every resource acquired up to that point. As the function grows, tracking all resources becomes unmanageable.

3. **Return values are overloaded.** A function cannot return both a useful value and an error code in the same return statement. The error and the result compete for the same channel.

Exceptions solve all three problems.

## What an Exception Is

An exception is a value that signals an error condition. When a function cannot complete its intended operation, it **throws** the exception. The exception propagates up the call stack automatically until a handler is found that can deal with it. If no handler is found, the program terminates.

The three keywords:

| Keyword | Purpose |
|---|---|
| `throw` | Signals that an error has occurred — launches the exception |
| `try` | Marks a block of code to monitor for exceptions |
| `catch` | Handles an exception of a specific type |

## `throw` — Signaling an Error

`throw` followed by a value launches an exception. Any type can be thrown — an integer, a string, or an object. The conventional and correct practice is to throw objects of types derived from `std::exception`.

```cpp
#include <stdexcept>

void err(){
    int code;
    if (code == 1) {
        throw 404; // Throws an integer
    } 
    else if (code == 2) {
        throw std::string("Disk full error!"); // Throws a std::string
    } 
    else if (code == 3) {
        throw DatabaseError("Connection failed!"); // Throws a custom object
    }
}

double divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
        // execution stops here — the line below never runs
    }
    return a / b;
}
```

When `throw` executes:
1. The thrown value is copied into a special storage area managed by the runtime (not on the regular stack)
2. Normal execution of the current function stops immediately
3. The runtime begins searching for a matching `catch` block

## `try` and `catch` — Handling an Exception

A `try` block wraps code that might throw. One or more `catch` blocks follow it, each handling a specific exception type.

```cpp
try {
    double result = divide(10.0, 0.0);
    std::cout << result;   // never reached if divide throws
}
catch (int errCode) {
        std::cout << "Caught an integer exception: Error code " << errCode << "\n";
} 
catch (const std::string& errMsg) {
    std::cout << "Caught a string exception: " << errMsg << "\n";
} 
catch (const DatabaseError& dbErr) {
    std::cout << "Caught a custom object exception: " << dbErr.message << "\n";
} 
catch (const std::invalid_argument& e) {
    std::cout << "Error: " << e.what() << "\n";
    // execution continues here after the catch block
}

std::cout << "After try-catch\n";   // always reached
```
**Execution flow:**
1. try block starts
2. `divide(10.0, 0.0)` called
3. `throw std::invalid_argument("Division by zero")` executed
4. `divide()` exits immediately — no return value
5. remaining try block skipped
6. `catch (const std::invalid_argument& e)` matched
7. handler body runs
8. prints: `"Error: Division by zero"`
9. execution continues after the catch block
10. prints: `"After try-catch"`

## Stack Unwinding

When an exception is thrown and no `catch` block exists in the current function, the exception propagates to the caller. The current function exits immediately and all its local stack objects have their destructors called. This process — walking back up the call stack, destroying local objects along the way — is called **stack unwinding**.

```cpp
void inner() {
    DataBuffer buf(100);              // constructor runs
    throw std::runtime_error("fail"); // throw — buf's destructor runs
}                                      // automatically during unwinding

void middle() {
    DataBuffer buf2(200);             // constructor runs
    inner();                          // throws — buf2's destructor runs
}                                      // automatically during unwinding

void outer() {
    try {
        middle();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";  // caught here
    }
}
```
**Execution flow:**
1. `outer()` calls `middle()`
2. `middle()` creates `buf2`
3. `middle()` calls `inner()`
4. `inner()` creates `buf`
5. `inner()` throws
6. `buf` destroyed (destructor called) ← stack unwinding
7. `inner()` exits
7. `buf2` destroyed (destructor called) ← stack unwinding
8. `middle()` exits
9. exception arrives at `outer()`'s catch block
10. handler runs

**Stack unwinding guarantees RAII works correctly under exceptions.**
* Every resource tied to a stack object is released even when the function exits via an exception. This is the fundamental reason RAII is safe by construction — the destructor always runs.
* Stack unwinding does not automatically clean up raw heap memory
```cpp
void riskyFunction() {
    int* heapData = new int[1000]; // Allocated on the heap

    // Imagine an exception is thrown here (e.g., out of memory or custom logic)
    throw std::runtime_error("Something went wrong!"); 

    delete[] heapData; // Never reached! Memory leak occurs.
}
```

## The `std::exception` Hierarchy

The standard library provides a hierarchy of exception types in `<stdexcept>` and `<exception>`. All derive from `std::exception`.
```
std::exception
 ├── std::logic_error (Logic & Programming Errors, Predictable)
 │    ├── std::invalid_argument
 │    ├── std::out_of_range
 │    ├── std::length_error
 │    └── std::domain_error
 │
 ├── std::runtime_error (Runtime & Environmental Errors, Unpredictable)
 │    ├── std::range_error
 │    ├── std::overflow_error
 │    └── std::underflow_error
 │
 ├── std::bad_alloc      (Thrown by `new` when memory allocation fails)
 ├── std::bad_cast       (Thrown by `dynamic_cast` when a type cast fails)
 ├── std::bad_typeid     (Thrown by `typeid` on a null pointer)
 └── std::bad_exception  (Used to handle unexpected exception types)
```

logic_error are not thows at compile time, it does not mean the compiler finds the bug. By keeping a conditional statements we can find the bugs

Interestingly, modern C++ (`constexpr`) can actually catch some of these logic errors at compile time if force the code to run during compilation.

For example, using `std::array` or `std::vector` inside a constexpr context:
```cpp
constexpr int getElement() {
    std::array<int, 3> arr = {1, 2, 3};
    return arr.at(5); // Out of bounds!
}

int main() {
    constexpr int val = getElement(); // COMPILE ERROR! 
}
```

**`std::exception::what()`:**

Every `std::exception` has a `what()` member function that returns a `const char*` describing the error:

```cpp
try {
    throw std::out_of_range("index 10 is out of range [0, 5)");
} catch (const std::exception& e) {
    std::cout << e.what();
    // prints: "index 10 is out of range [0, 5)"
}
```

## Catching by Reference — Why It Is Required

Exception objects should always be caught by **`const` reference**, never by value.

```cpp
// WRONG — catch by value
catch (std::exception e) {
    e.what();   // calls std::exception::what() — not the derived class version
                // object is SLICED — derived class information is lost
}

// CORRECT — catch by const reference
catch (const std::exception& e) {
    e.what();   // virtual dispatch — calls the correct derived class version
}
```

**Why slicing happens with catch-by-value:**

If a `std::runtime_error` is thrown and caught as `std::exception` by value, the runtime_error part is sliced off — only the `std::exception` base portion is copied into the catch parameter. The derived class's `what()` message may be lost. Catching by reference avoids the copy and preserves the full derived type.

## 8Multiple `catch` Blocks — Ordering Matters

Multiple `catch` blocks handle different exception types. They are checked in order from top to bottom. The first matching catch block runs. Others are skipped.

```cpp
try {
    riskyOperation();
} catch (const std::invalid_argument& e) {
    // handles invalid_argument specifically
    std::cout << "Invalid argument: " << e.what() << "\n";
} catch (const std::runtime_error& e) {
    // handles runtime_error and anything derived from it
    std::cout << "Runtime error: " << e.what() << "\n";
} catch (const std::exception& e) {
    // handles any std::exception not caught above
    std::cout << "Exception: " << e.what() << "\n";
}
```

**The ordering rule — most derived first:**

```cpp
// WRONG ORDER
catch (const std::exception& e) { }         // catches everything
catch (const std::runtime_error& e) { }     // NEVER REACHED — std::exception above already caught it
catch (const std::invalid_argument& e) { }  // NEVER REACHED

// CORRECT ORDER — most derived types first
catch (const std::invalid_argument& e) { }  // most specific
catch (const std::runtime_error& e) { }     // less specific
catch (const std::exception& e) { }         // least specific — catch-all for std exceptions
```

## `catch(...)` — Catch Everything

A `catch(...)` block catches any exception regardless of type. No information about the exception is available inside the block.

```cpp
try {
    riskyOperation();
} catch (const std::exception& e) {
    std::cout << "Standard exception: " << e.what() << "\n";
} catch (...) {
    std::cout << "Unknown exception caught\n";
    // cannot access the exception object — type is unknown
}
```

`catch(...)` is used as a last resort when a function must not allow any exception to escape — for example inside a destructor or a C callback that cannot propagate C++ exceptions.

## Re-throwing an Exception

A `catch` block can re-throw the current exception using `throw;` with no argument. This forwards the original exception — with its original type and message — to the next enclosing handler.

```cpp
void processFile(const std::string& path) {
    try {
        openAndRead(path);
    } catch (const std::exception& e) {
        std::cerr << "processFile failed for: " << path << "\n";
        throw;   // re-throw — original exception propagates upward
                  // does NOT create a new exception — original type preserved
    }
}
```

**Why `throw;` instead of `throw e;`:**

`throw e;` would throw a new copy of the exception as the static type of `e` (which is `std::exception`) — losing the derived type. `throw;` always rethrows the original exception object with its original dynamic type.

## Custom Exception Classes

Custom exception classes provide structured, type-safe error information specific to the domain of the code.

```cpp
#include <stdexcept>
#include <string>

class InsufficientFundsError : public std::runtime_error {
    double available;
    double requested;

public:
    InsufficientFundsError(double avail, double req)
        : std::runtime_error("Insufficient funds"),
          available(avail),
          requested(req)
    {}

    double getAvailable()  const { return available; }
    double getRequested()  const { return requested; }
};

class BankAccount {
    double balance;
public:
    BankAccount(double b) : balance(b) {}

    void withdraw(double amount) {
        if (amount > balance) {
            throw InsufficientFundsError(balance, amount);
        }
        balance -= amount;
    }
};
```

**Catching the custom exception:**

```cpp
BankAccount acc(100.0);

try {
    acc.withdraw(200.0);
} catch (const InsufficientFundsError& e) {
    std::cout << e.what() << "\n";
    std::cout << "Available: " << e.getAvailable() << "\n";
    std::cout << "Requested: " << e.getRequested() << "\n";
} catch (const std::exception& e) {
    // fallback for any other standard exception
    std::cout << e.what() << "\n";
}
```

**Rules for custom exception classes:**
- Always derive from `std::exception` or one of its subclasses
- Use `std::runtime_error` or `std::logic_error` as the immediate base for most cases
- Override `what()` or pass the message to the base constructor

## Exception Safety Guarantees
A function's **exception safety guarantee** describes what state the program is left in if the function throws. There are three levels:

### No-Throw Guarantee

The function never throws. If an internal operation fails, the error is handled internally and never propagates.

```cpp
void DataBuffer::swap(DataBuffer& other) noexcept {
    std::swap(data, other.data);   // pointer swap — cannot throw
    std::swap(size, other.size);   // integer swap — cannot throw
}
```

Marked with `noexcept`. This is the strongest guarantee. Destructors, move operations, and `swap` should provide this guarantee.

### Strong Guarantee

If the function throws, the program state is exactly as it was before the function was called. The operation either completes fully or has no effect — like a database transaction with rollback.

```cpp
DataBuffer& DataBuffer::operator=(const DataBuffer& rhs) {
    // Copy-and-swap provides strong guarantee:
    DataBuffer temp(rhs);           // if this throws, *this is unchanged
    swap(temp);                     // noexcept — cannot throw
    return *this;
    // if new() in DataBuffer(rhs) throws, temp was never swapped in
    // *this is exactly as it was before the assignment
}

// or
DataBuffer& DataBuffer::operator=(DataBuffer rhs) { // 1. Pass by value: invokes copy constructor
    swap(rhs);                                      // 2. Non-throwing swap with the temporary
    return *this;                                   // 3. rhs is destroyed, cleaning up old resources
}
```

### Basic Guarantee

The basic guarantee states that if a function throws an exception, the program remains in a valid, safe state, but the exact contents of the data may have changed.

```cpp
void DataBuffer::append(int value) {
    // If resize throws, the buffer may be partially grown
    // but no memory is leaked and the object remains destructible
    resize(size + 1);
    data[size - 1] = value;
}
```

A safe, valid state does not mean the data remains identical to how it was before the function started. Instead, it means the internal rules and structures of the program remain mathematically and logically consistent, preventing crashes, memory leaks, and unpredictable behavior.

#### The Distinction Between "Valid" and "Unchanged"
* Unchanged Data (Strong Guarantee): The data looks exactly as it did before the function call (complete rollback).
* Valid Data (Basic Guarantee): The data might be altered or incomplete, but its internal structural rules are strictly obeyed.

#### What Makes a State Unsafe?
An unsafe or corrupted state occurs when internal rules (called invariants) are broken. Examples of unsafe states include:
* Dangling Pointers: A pointer references memory that has already been deleted.
* Memory Leaks: Memory was allocated, but the pointer to it was lost, making it impossible to free.
* Mismatched Trackers: An internal size variable states a container holds 10 items, but the memory buffer only actually holds 5 items, causing the program to read garbage memory or crash.

### No Guarantee (Avoid)

If the function throws, the program may be in an invalid state — memory leaked, invariants violated, pointers dangling. This is the situation the Rule of Three and Rule of Five were designed to prevent.

## The levels in order:
* No-throw — function never throws
* Strong — throws or no-change (commit or rollback)
* Basic — throws but program remains valid
* No guarantee — throws and may corrupt state ← avoid

## Exceptions in Constructors
When a constructor throws, the object is considered never to have been constructed. The destructor is **not called** for the partially-constructed object — because the object never fully existed.

However, any data members that were fully constructed before the throw **do** have their destructors called:

```cpp
class TwoResources {
    DataBuffer first;   // constructed first
    DataBuffer second;  // constructed second

public:
    TwoResources(int a, int b)
        : first(a),    // succeeds
          second(b)    // throws std::bad_alloc
    {
        // constructor body never reached
    }

    ~TwoResources() {
        // NEVER CALLED: object was never fully constructed
    }
};

// When second(b) throws:
//   'second' was never constructed — no destructor for it
//   'first' was fully constructed — its destructor IS called
//   TwoResources destructor is NOT called
```

### Automatic Destructor
* The destructors for fully constructed member variables are called automatically by the C++ runtime when an exception interrupts a constructor.
* This automatic behavior exists to prevent memory leaks and resource locking. If a constructor fails halfway through initializing a class, any resources (such as memory blocks, file handles, or network connections) that were successfully acquired up to that point would remain trapped in memory permanently unless cleaned up.

### How the Mechanism Operates
1. Order of Construction: Member variables inside a class are always constructed in the exact order they are listed in the class definition, from top to bottom.
2. Tracking Success: The C++ runtime tracks which individual members have finished constructing successfully during the initialization process.
3. The Cleanup Process: If a constructor throws an exception, the runtime halts execution and travels backward through the list of successfully constructed members, destroying each one in the reverse order of their creation.

**This is why RAII members (like `std::unique_ptr`) are safe in constructors** — if the constructor throws partway through, already- constructed members clean up their resources automatically.

```cpp
#include <iostream>
#include <stdexcept>

class DatabaseConnection {
public:
    DatabaseConnection() {
        std::cout << "[LOG] DatabaseConnection resource acquired.\n";
    }
    
    ~DatabaseConnection() {
        std::cout << "[LOG] DatabaseConnection destructor executed (Cleaned up).\n";
    }
};

class ServerManager {
private:
    DatabaseConnection * db; 
    DatabaseConnection db2;  // Member object created first

public:
    ServerManager(): db(new DatabaseConnection()) {
        std::cout << "[LOG] ServerManager constructor body started.\n";
        std::cout << "[LOG] Error detected! Throwing exception inside constructor...\n";
        
        // This exception stops construction midway
        throw std::runtime_error("Critical Initialization Failure");
    }

    ~ServerManager() {
        // THIS WILL NEVER EXECUTE
        delete db;
        db = nullptr;
        std::cout << "[LOG] ~ServerManager() destructor executed.\n";
    }
};

int main() {
    std::cout << "=== Program Started ===\n";
    
    try {
        std::cout << "Attempting to create ServerManager on the heap...\n";
        
        // Attempting heap allocation
        ServerManager* server = new ServerManager();
        
        // This line is never reached because an exception is thrown
        delete server; 
                
    } catch (const std::exception& e) {
        std::cout << "[LOG Caught Exception]: " << e.what() << "\n";
    }

     try {
        std::cout << "Attempting to create ServerManager on the stack...\n";
        
        // stack
        ServerManager server1;
        
    } catch (const std::exception& e) {
        std::cout << "[LOG Caught Exception]: " << e.what() << "\n";
    }
    
    std::cout << "=== Program Finished Safely ==\n";
    return 0;
}
```
**output**
```
=== Program Started ===
Attempting to create ServerManager on the heap...
[LOG] DatabaseConnection resource acquired.
[LOG] DatabaseConnection resource acquired.
[LOG] ServerManager constructor body started.
[LOG] Error detected! Throwing exception inside constructor...
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG Caught Exception]: Critical Initialization Failure
Attempting to create ServerManager on the stack...
[LOG] DatabaseConnection resource acquired.
[LOG] DatabaseConnection resource acquired.
[LOG] ServerManager constructor body started.
[LOG] Error detected! Throwing exception inside constructor...
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG Caught Exception]: Critical Initialization Failure
=== Program Finished Safely ==
```

As we observe the output only DatabaseConnection's stack object is cleared, raw object is still in the heap cause memory leak
1. The object's destructor (`~ServerManager()`) is NEVER called because the object was never fully constructed.
2. Fully constructed sub-objects and member variables (in this case, `db2`, the stack member) are automatically cleaned up in the reverse order of their construction.
3. Raw pointers (db pointing to heap memory allocated via new) are NOT automatically cleaned up. C++ only manages the memory of the pointer variable itself, not the resource it points to. Because the ServerManager destructor never runs, the delete db; statement inside it is skipped, resulting in a memory leak.

```cpp
class ServerManager {
private:
    DatabaseConnection * db; 
    DatabaseConnection db2;  

public:
    // Notice the try-catch block wrapping the constructor initializer list and body
    ServerManager() try : db(new DatabaseConnection()) {
        std::cout << "[LOG] ServerManager constructor body started.\n";
        std::cout << "[LOG] Error detected! Throwing exception inside constructor...\n";
        
        throw std::runtime_error("Critical Initialization Failure");
    } 
    catch (...) {
        // This catch block executes if ANY exception is thrown during 
        // member initialization or inside the constructor body.
        
        // Explicitly clean up the raw heap allocation
        delete db;
        db = nullptr;
        
        std::cout << "[LOG] Caught exception in constructor try-block. Cleaned up heap memory.\n";
        
        // Note: The caught exception is automatically rethrown by C++ here.
    }

    ~ServerManager() {
        delete db;
        db = nullptr;
        std::cout << "[LOG] ~ServerManager() destructor executed.\n";
    }
};
```
**output**
```
=== Program Started ===
Attempting to create ServerManager on the heap...
[LOG] DatabaseConnection resource acquired.
[LOG] DatabaseConnection resource acquired.
[LOG] ServerManager constructor body started.
[LOG] Error detected! Throwing exception inside constructor...
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG] Caught exception in constructor try-block. Cleaned up heap memory.
[LOG Caught Exception]: Critical Initialization Failure
Attempting to create ServerManager on the stack...
[LOG] DatabaseConnection resource acquired.
[LOG] DatabaseConnection resource acquired.
[LOG] ServerManager constructor body started.
[LOG] Error detected! Throwing exception inside constructor...
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG] DatabaseConnection destructor executed (Cleaned up).
[LOG] Caught exception in constructor try-block. Cleaned up heap memory.
[LOG Caught Exception]: Critical Initialization Failure
=== Program Finished Safely ==
```

## Exceptions in Destructors — Recap

**destructors must not throw**.

If a destructor throws while another exception is propagating (during stack unwinding), `std::terminate` is called immediately. Destructors are implicitly `noexcept` in C++11 and later.

```cpp
class Safe {
public:
    ~Safe() noexcept {
        try {
            riskyCleanup();
        } catch (...) {
            // absorb — log if possible, never propagate
        }
    }
};
```

## `std::terminate` and `std::terminate_handler`

`std::terminate` is called when exception handling cannot continue:
- An exception propagates out of `main`
- An exception throws during stack unwinding (destructor throws)
- A `noexcept` function allows an exception to escape
- `throw;` is used outside a catch block

By default, `std::terminate` calls `std::abort`, which terminates the program immediately without further stack unwinding or destructor calls.

A custom terminate handler can be installed:

```cpp
#include <exception>

void myTerminate() {
    std::cerr << "Unhandled exception — terminating\n";
    std::abort();
}

int main() {
    std::set_terminate(myTerminate);
    // ...
}
```

## Performance Considerations

Exception handling has costs:

1. **Zero-cost when no exception is thrown (modern compilers):** The compiler generates exception tables in a separate section of the binary. When no exception occurs, those tables are never consulted — normal execution has no overhead from the presence of `try`/`catch`.

2. **Significant cost when an exception is thrown:** Stack unwinding, type matching, and handler lookup have real runtime cost. Exceptions are not suitable for normal control flow or frequently occurring conditions.

**Rule:** Use exceptions for genuinely exceptional conditions — errors that are unexpected and not part of normal operation. Use return values or `std::optional` for expected failure modes (user not found, file does not exist, parse failed on user input).
