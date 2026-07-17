## Static Storage Duration
An object with static storage duration exists for the **entire lifetime of the program**. It is constructed once (before it is first used) and destroyed once (when the program exits). There is one instance shared across all function calls, all threads, and all translation units (with linkage rules).

Three kinds of objects have static storage duration:
- Global variables (declared outside any function)
- `static` local variables (declared inside a function with `static`)
- `static` data members of a class (covered in the Static Members section)

### Global Variables
A global variable is declared outside all functions at namespace scope. It is initialized before `main` begins and destroyed after `main` returns.

```cpp
int globalCounter = 0;          // zero-initialized, then default-initialized
BankAccount defaultAccount;     // default constructor runs before main()

int main() {
    globalCounter++;            // accessing global from main
    defaultAccount.deposit(100);
}
// defaultAccount destructor runs after main() returns
```
**Initialization order across translation units:**
* Within a single translation unit (`.cpp` file), global objects are initialized in declaration order. 
* cross multiple translation units, the order is **unspecified**.

This creates the **static initialization order fiasco**: if global object `A` in `file1.cpp` depends on global object `B` in `file2.cpp`, `B` may or may not be initialized when `A`'s constructor runs — depending on link order.

```cpp
// file1.cpp
extern BankAccount masterAccount;
BankAccount branchAccount(masterAccount);   // DANGEROUS: masterAccount
                                             // may not be initialized yet
// file2.cpp
BankAccount masterAccount(999, 0.0);
```


**Solution — Construct On First Use:**

Wrap the global in a function that returns a static local reference. Static locals are initialized exactly once, the first time the function is called — guaranteed to be after the program has started.
```cpp
BankAccount& getMasterAccount() {
    static BankAccount master(999, 0.0);   // initialized on first call only
    return master;
}

// Any code that needs masterAccount calls getMasterAccount() instead
// Initialization is deferred until first use — no ordering dependency
```

### `static` Local Variables

A `static` local variable is declared inside a function but retains its value across calls. It is initialized the first time execution reaches its declaration and destroyed when the program exits.
```cpp
int callCount() {
    static int count = 0;   // initialized to 0 on first call only
    count++;
    return count;
}

callCount();   // returns 1
callCount();   // returns 2
callCount();   // returns 3
// count is not reset between calls — it lives for the program's lifetime
```


**Initialization is thread-safe (C++11 and later):**

The C++11 standard guarantees that static local initialization is thread-safe. If two threads call a function with a static local for the first time simultaneously, only one initializes it — the other waits.

```cpp
Config& getConfig() {
    static Config instance;   // constructed once, thread-safely
    return instance;
}
```

This pattern is the standard implementation of the **Singleton** design pattern in modern C++.

### `constinit` — Guaranteed Compile-Time Initialization
`constinit` (C++20) verifies at compile time that a static-duration variable is initialized with a constant expression. It does not make the variable `const` — the variable can still be modified at runtime.

```cpp
constinit int maxConnections = 100;   // guaranteed initialized at compile time
                                       // NOT const — can be changed at runtime

maxConnections = 200;   // VALID: constinit does not block modification
```

`constinit` solves the static initialization order fiasco for variables that can be initialized at compile time — their initialization happens before any runtime code runs, with no ordering dependency on other objects.

### `thread_local` Storage Duration
A `thread_local` variable is a separate instance per thread. Each thread gets its own copy, initialized when the thread starts and destroyed when the thread exits.

```cpp
thread_local int requestId = 0;   // each thread has its own requestId

void handleRequest() {
    requestId++;    // modifies only this thread's copy
                    // other threads' requestId values are unaffected
}
```

`thread_local` can be combined with `static`:

```cpp
void logMessage(const std::string& msg) {
    thread_local static int lineNumber = 0;   // one counter per thread,
    lineNumber++;                              // persists across calls within
                                               // the same thread
}
```