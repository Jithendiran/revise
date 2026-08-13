# C++ Attributes

## What Attributes Are

An attribute is a standardized annotation placed on a declaration, statement, or entity that provides additional information to the compiler.

Attributes do not change the meaning of the program — they communicate intent, optimization hints, or warnings to the compiler and to readers of the code.

**Syntax:** Double square brackets `[[attribute]]` placed before or after the entity being annotated.

```cpp
[[nodiscard]] int compute();          // on a function declaration
[[maybe_unused]] int debugCounter;    // on a variable
[[likely]] if (condition) { }         // on a branch
```

Attributes were standardized in C++11. Before that, compilers had their own non-portable annotation syntax (`__attribute__((x))` in GCC, `__declspec(x)` in MSVC). Standard attributes replaced those for the most common use cases.

## `[[likely]]` and `[[unlikely]]` (C++20)
### The Problem — Branch Prediction

Modern CPUs execute instructions faster when they can predict which branch of an `if` statement will be taken. The CPU has a **branch predictor** — hardware logic that guesses the outcome before the condition is evaluated and starts executing the guessed path speculatively.

When the prediction is wrong (a branch misprediction), the CPU must discard the speculatively executed instructions and restart from the correct path. This costs 10–20 clock cycles per misprediction — negligible once but significant in tight loops called millions of times.

By default, the compiler has no information about which branch is taken more often. `[[likely]]` and `[[unlikely]]` provide that information.

### What They Do

- `[[likely]]` tells the compiler: this branch is expected to be taken most of the time.
- `[[unlikely]]` tells the compiler: this branch is rarely taken.

The compiler uses this information to:
1. Lay out the machine code so the likely path has no jumps (sequential execution is fastest)
2. Inform the CPU's branch predictor through code structure
3. Apply optimizations that assume the likely path is the common case

```cpp
void process(int value) {
    if (value > 0) [[likely]] {
        // this runs almost always
        normalProcessing(value);
    } else [[unlikely]] {
        // this is the rare error case
        handleNegative(value);
    }
}
```

### Syntax Positions

`[[likely]]` and `[[unlikely]]` apply to the **statement** following them, not the condition. They can appear in several positions:

**On an `if` branch:**

```cpp
if (condition) [[likely]] {
    // common case
} else [[unlikely]] {
    // rare case
}
```

**On a single-statement branch:**

```cpp
if (errorOccurred) [[unlikely]]
    handleError();
```

**In a `switch` case:**

```cpp
switch (code) {
    [[likely]]   case 200: handleSuccess(); break;
    [[unlikely]] case 404: handleNotFound(); break;
    [[unlikely]] case 500: handleServerError(); break;
}
```

**In a loop condition (applied to the loop continuation):**

```cpp
while (hasMoreData()) [[likely]] {
    processNext();
}
```
### When to Use Them

`[[likely]]` and `[[unlikely]]` are worth using when:

- The branch frequency is known from profiling or domain knowledge
- The code is in a hot path — called frequently in a performance-critical loop
- The imbalance between branches is significant (one branch taken 95%+ of the time)

```cpp
// Good use: error checking in a hot path
// Under normal operation, the error branch is never taken
bool parsePacket(const uint8_t* data, int len) {
    if (len < HEADER_SIZE) [[unlikely]] {
        return false;   // malformed packet — rare
    }
    // ... normal packet processing — runs almost always
    return true;
}
```

**When NOT to use them:**

- When branches are taken roughly equally often
- When the code is not performance-critical
- Without profiling data — guessing incorrectly can harm performance


## `[[nodiscard]]`

### What It Does

`[[nodiscard]]` on a function declaration tells the compiler to warn when the return value is discarded — when the function is called but its return value is not used.

```cpp
[[nodiscard]] int openFile(const char* path);

openFile("data.txt");           // WARNING: ignoring return value
int fd = openFile("data.txt");  // VALID: return value used
```

### Why It Exists

Some functions exist purely for their return value. Calling them and discarding the result is almost always a bug:

```cpp
[[nodiscard]] bool isEmpty() const;

if (container.isEmpty());     // BUG: semicolon — condition checked but nothing done
                              // without nodiscard: silent, no warning
                              // with nodiscard: compiler warns immediately
```

### On Classes and Structs

`[[nodiscard]]` on a class means: every function that returns this type should have its return value checked.

```cpp
[[nodiscard]] class ErrorCode {
    int code;
public:
    ErrorCode(int c) : code(c) {}
    bool failed() const { return code != 0; }
};

ErrorCode write(const char* data);

write("hello");    // WARNING: ErrorCode return value discarded
                   // error may have gone unhandled
```

### With a Message (C++20)

```cpp
[[nodiscard("check for allocation failure")]]
void* allocate(std::size_t bytes);

allocate(1024);   // WARNING: check for allocation failure
```

## `[[maybe_unused]]`

### What It Does

`[[maybe_unused]]` suppresses compiler warnings about unused variables, functions, or parameters. It communicates: this is intentionally unused in some configurations — do not warn about it.

```cpp
[[maybe_unused]] int debugCounter = 0;

void process([[maybe_unused]] int debugMode) {
    // debugMode only used in debug builds
#ifdef DEBUG
    if (debugMode > 0) logVerbose();
#endif
    // in release builds, debugMode is unused — no warning with attribute
}
```

### Common Use Cases

**Variables only used in assertions:**

```cpp
bool result = initialize();
assert(result);   // result used only in debug builds (assert is no-op in release)

// Without attribute: "unused variable 'result'" warning in release builds
// With attribute:
[[maybe_unused]] bool result = initialize();
assert(result);   // no warning
```

**Function parameters unused in some overrides:**

```cpp
class Base {
public:
    virtual void process(int value, int flags) = 0;
};

class Derived : public Base {
public:
    void process(int value, [[maybe_unused]] int flags) override {
        // this implementation ignores flags
        doWork(value);
    }
};
```
## `[[fallthrough]]`

### The Switch Fallthrough Problem

In a `switch` statement, execution falls through from one `case` to the next if no `break` is present. This is sometimes intentional but often a bug. The compiler may warn about implicit fallthrough.

```cpp
switch (command) {
    case 'q':
    case 'Q':
        quit();
        break;

    case 'r':
        reset();
        // MISSING break — falls through to 'p' case
        // compiler warns: implicit fallthrough
    case 'p':
        print();
        break;
}
```

### What `[[fallthrough]]` Does

`[[fallthrough]]` marks an intentional fallthrough — silencing the warning and communicating to readers that the fallthrough is deliberate.

```cpp
switch (errorLevel) {
    case 3:
        logCritical();
        [[fallthrough]];   // intentional — critical errors also need
                            // the same handling as severe errors
    case 2:
        logSevere();
        [[fallthrough]];   // intentional
    case 1:
        logWarning();
        break;
    case 0:
        break;
}
```

`[[fallthrough]]` must appear as a statement on its own, immediately before the next `case` label. It cannot appear after the last case or where there is a `break`.

## `[[deprecated]]`

### What It Does

`[[deprecated]]` marks a declaration as deprecated — still available but scheduled for removal in a future version. The compiler emits a warning whenever the deprecated entity is used.

```cpp
[[deprecated]] void oldFunction();

[[deprecated("use newFunction() instead")]]
void anotherOldFunction();

oldFunction();           // WARNING: 'oldFunction' is deprecated
anotherOldFunction();    // WARNING: use newFunction() instead
```

### On Classes and Members

```cpp
class [[deprecated("use NewAccount instead")]] OldAccount {
    // ...
};

class BankAccount {
    [[deprecated("use getBalance() instead")]]
    double balance;   // direct access deprecated — use getter

public:
    double getBalance() const { return balance; }
};
```

## `[[noreturn]]`

### What It Does

`[[noreturn]]` on a function declaration tells the compiler that the function never returns to its caller — it always throws, calls `std::exit`, or enters an infinite loop.

```cpp
[[noreturn]] void fatalError(const std::string& message) {
    std::cerr << "FATAL: " << message << "\n";
    std::terminate();   // program ends here — function never returns
}

[[noreturn]] void infiniteLoop() {
    while (true) {
        processEvents();
    }
}
```

### Why the Compiler Needs to Know

Without `[[noreturn]]`, the compiler may generate warnings about missing return values in calling code, or miss optimization opportunities:

```cpp
int compute(bool valid) {
    if (!valid) {
        fatalError("invalid state");  // without [[noreturn]]: compiler warns
                                       // "control reaches end of non-void function"
                                       // because it doesn't know fatalError never returns
    }
    return 42;
}
```

With `[[noreturn]]` on `fatalError`, the compiler knows the `if` path never returns to `compute` — no warning, and the code after the call is recognized as unreachable.

## `[[no_unique_address]]` (C++20)

### The Problem — Empty Class Members Have Non-Zero Size

In C++, every object must have a unique address. Even an empty class (no data members) has a size of at least 1 byte to ensure different objects have different addresses.

When an empty class is used as a data member, this forces the containing class to be larger than necessary:

```cpp
struct EmptyAllocator {};   // empty class — size = 1

struct Container {
    EmptyAllocator alloc;   // takes 1 byte + padding
    int data;               // 4 bytes
    // sizeof(Container) = 8 (not 4)
};
```

### What `[[no_unique_address]]` Does

`[[no_unique_address]]` allows the compiler to make an empty member share its address with another member or with the base class. The empty member occupies zero additional space.

```cpp
struct Container {
    [[no_unique_address]] EmptyAllocator alloc;   // no space taken
    int data;
    // sizeof(Container) = 4
};
```

This is used in standard library implementations of containers that store an allocator as a member — when the allocator is stateless (empty), it should consume no space in the container object.

## `[[assume]]` (C++23)

### What It Does

`[[assume(expression)]]` tells the compiler to assume the given expression is always true at that point in the code. The compiler is permitted to use this assumption for optimization — generating code that would be incorrect if the assumption were false.

```cpp
void processPositive(int* arr, int n) {
    [[assume(n > 0)]];       // tell compiler n is always positive
    [[assume(arr != nullptr)]]; // tell compiler arr is never null

    for (int i = 0; i < n; i++) {
        arr[i] *= 2;
    }
}
```

**If the assumption is violated at runtime, the behavior is undefined.**
`[[assume]]` is not an assertion — it does not check the condition. It tells the compiler "trust me, this is always true" and the compiler removes safety checks based on that trust.

Use `[[assume]]` only when:
- The condition is genuinely always true at that point
- Profiling shows the optimization is meaningful
- The assumption has been thoroughly verified