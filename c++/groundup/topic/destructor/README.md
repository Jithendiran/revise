# Destructor

## The Problem Without a Destructor
An object frequently acquires resources during its lifetime that exist outside the object itself — a heap memory block, an open file, a network connection, a database handle. When the object is destroyed, those external resources are not automatically released. They remain locked, allocated, and unavailable to the rest of the program.

```cpp
void processFile() {
    FILE* file = fopen("data.txt", "r");   // acquires resource
    // ... work ...
    return;   // file is never closed — resource leaks
}
```
For a single call this may be harmless. In a loop called ten thousand times, ten thousand file handles remain open. The operating system has a fixed limit on open file handles. The program fails.

The same problem exists for heap memory:

```cpp
class DataBuffer {
    int* data;
public:
    DataBuffer(int size) {
        data = new int[size];   // acquires heap memory
    }
    // no destructor — heap block never freed
};

void process() {
    DataBuffer buf(1000);
}   // buf's stack slot released — but the heap block it pointed to is still
    // allocated and now unreachable — memory leak
```

A destructor solves this. It is a guaranteed cleanup point that runs automatically when an object's lifetime ends, regardless of how that lifetime ends.

##  What a Destructor Is
A destructor is a member function that runs automatically the moment an object is destroyed. It has three syntax rules:
1. The name is the class name preceded by a tilde `~`
2. No parameters — a destructor takes nothing
3.  No return type — not even `void`

```cpp
class DataBuffer {
    int* data;
    int  size;

public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    ~DataBuffer() {               // destructor
        delete[] data;            // releases heap memory
        data    = nullptr;        // defensive null — prevents accidental reuse
        size    = 0;
    }
};

void process() {
    DataBuffer buf(1000);         // constructor runs — heap allocated
    // ... work ...
}   // destructor runs here automatically — heap freed
```

The programmer never calls a destructor by name in normal usage. The compiler calls it at the correct point automatically.

## When the Destructor Runs
The destructor runs in every situation that ends an object's lifetime, not just normal scope exit.

### Stack Object — Scope Exit
The destructor runs when execution leaves the scope containing the object's declaration, regardless of how that scope is exited.

```cpp
void example() {
    DataBuffer buf(100);

    if (someCondition) {
        return;              // destructor runs here on early return
    }

    // ... more work ...
}   // destructor runs here on normal exit
```
Both paths trigger the destructor. There is no way to exit a scope containing a stack object without its destructor running.

### Heap Object — `delete`

```cpp
DataBuffer* p = new DataBuffer(1000);
// ... work ...
delete p;   // destructor runs here, then memory freed
```

For heap objects, the destructor only runs when `delete` is called. If `delete` is never called, the destructor never runs and the resource leaks.

### Exception — Stack Unwinding
When an exception is thrown and propagates up the call stack, every stack object in every scope that is being exited has its destructor called. This process is called **stack unwinding**.

```cpp
void inner() {
    DataBuffer buf(100);          // constructor runs
    throw std::runtime_error("failed");
    // exception propagates — buf's destructor runs automatically
    // even though no 'return' or closing brace was reached normally
}

void outer() {
    DataBuffer outerBuf(200);     // constructor runs
    inner();                      // throws — inner's buf is cleaned up
    // exception continues propagating — outerBuf's destructor runs
}
```

This is the most important property of stack objects under exceptions. Every resource tied to a stack object is guaranteed to be released even when the program takes an unexpected path through an exception. This is why resource management through stack objects is safe by construction.

### Temporary Object — End of Full Expression
A temporary object (prvalue or xvalue) is destroyed at the end of the full expression that created it — typically at the semicolon.
```cpp
std::string("hello") + " world";
//                              ↑
// the temporary std::string created by "hello" is destroyed here
// its destructor runs at the semicolon
```
Exception: a temporary bound to a `const T&` or `T&&` reference has its lifetime extended to match the reference (covered in the References section).

### Global and Static Objects — Program Exit
Global and static-duration objects have their destructors called when the program exits — after `main` returns. They are destroyed in **reverse order of construction**.

```cpp
BankAccount globalAccount(999, 0.0);   // constructed before main
static DataBuffer sharedBuf(100);       

// When program exits:
// sharedBuf destructor runs first (constructed last)
// globalAccount destructor runs second
```

here static is not meant by lazy initialization, if we need a object which only constructs at the time of execution 
```cpp
BankAccount globalAccount(999, 0.0); 
void useBuffer() {
    static DataBuffer sharedBuf(100); // Constructed on FIRST USE
    // ...
}
```

```cpp
#include <iostream>
using namespace std;

class A{
    int num;
  public:
    A(int i): num(i){
        cout<< "A cons :: "<< num << "\n";
    }
    ~A() {
        cout << "A des :: "<< num << "\n";
    }
};


A a1(1);  //----------------------- created before main because of global
void useBuffer() {
    static A a2(2);     //---------------because of static only once it would create when the outer function is called
}

int main()
{
    A a3(3);            //--------------- local object
    std::cout<<"Hello World \n";
    useBuffer();
    return 0;
    //--------------------local obj deleted
}
//---------------- static and global dete in rev order of construction
```

```bash
A cons :: 1
A cons :: 3
Hello World 
A cons :: 2
            <----- main() hits 'return 0;'
A des :: 3  
            <----- main()'s local stack resources are cleared / main() exits
            #-----------------------------------------------------------#
            #  THE RUNTIME CLEANUP PHASE (Still inside the program!)   #
            #-----------------------------------------------------------#
A des :: 2
A des :: 1
            <------ PROGRAM EXIT (Control goes back to the OS)
```

## Destruction Order
### Stack Objects — Reverse Declaration Order

Within a scope, objects are destroyed in the reverse order they were declared. The last object declared is the first destroyed.
```cpp
void example() {
    BankAccount a(101, 100.0);   // constructed 1st
    DataBuffer  b(50);           // constructed 2nd
    BankAccount c(102, 200.0);   // constructed 3rd
}
// destroyed: c (3rd→1st), then b (2nd), then a (1st→last)
```
This reverse order matters when objects depend on each other. An object that depends on another is declared after it, and therefore destroyed before it — the dependency is always outlived by what it depends on.

###  Data Members — Reverse Declaration Order
When an object's destructor runs, its body executes first. Then the data members are destroyed in reverse declaration order automatically.

```cpp
class Connection {
    Socket     socket;    // declared 1st
    DataBuffer buffer;    // declared 2nd
    Logger     logger;    // declared 3rd

public:
    ~Connection() {
        // destructor body runs first
        logger.log("Connection closing");
    }
    // then: logger destroyed (3rd→1st)
    // then: buffer destroyed
    // then: socket destroyed (1st→last)
};
```
The destructor body runs before member destructors. This means the destructor body can still safely use all members — they have not been destroyed yet when the body executes.

**NOTE**
In the above code snippet `socket`, `DataBuffer` and `Logger` are data member value types *not a pointer type*, so programmer should not delete them using `delete`

**Rule**
* `delete` used only for allocated using `new`
* If regular data member c++ internally manages them   

Think of an object like a brick building:
* The Destructor Body is the foreman walking through the building, saving files, and turning off the power.
* The Automatic Destruction Phase is the wrecking ball that actually knocks the walls down.

### Inheritance Chain — Derived First, Base Last
When a derived class object is destroyed, the derived destructor runs first, then the base class destructor runs automatically. Construction order is base-first, so destruction order is derived-first — always reverse.
```cpp
class Base {
public:
    ~Base() { std::cout << "Base destroyed\n"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived destroyed\n"; }
};

Derived d;
// output when d goes out of scope:
// "Derived destroyed"
// "Base destroyed"
```

## What Is Valid Inside the Destructor Body
When the destructor body executes, the object is still fully alive. All data members hold their last-assigned values. `this` is valid. All member functions can be called.

```cpp
class DatabaseConnection {
    int    connectionId;
    bool   isOpen;
    Logger logger;

public:
    ~DatabaseConnection() {
        if (isOpen) {                          // VALID: member is accessible
            logger.log("Closing connection");  // VALID: member function call
            closeConnection(connectionId);     // VALID: member used as argument
            isOpen = false;                    // VALID: member can be modified
        }
    }
};
```

After the destructor body completes, the members are destroyed in reverse declaration order, then the object's memory is released. The destructor body executes before any of that cleanup — it is the last opportunity to use the object in a coherent state.

## The Compiler-Generated Destructor
If a class declares no destructor, the compiler generates one automatically. The generated destructor has an empty body — it does nothing itself. However, it still triggers the automatic destruction of all data members in reverse declaration order, and for class-type members, calls their own destructors.

```cpp
class Simple {
    int x;
    std::string name;   // std::string has its own destructor
};

// Compiler generates:
// ~Simple() {}
// After the empty body:
//   name's destructor runs (std::string frees its heap buffer)
//   x is destroyed (primitive — nothing to do)
```
The generated destructor is sufficient for classes whose members manage their own resources. It becomes incorrect for classes that hold **raw pointers to heap memory** — the generated destructor destroys the pointer variable (the stack slot holding the address) but does not call `delete` on the heap block it points to.

```cpp
class DataBuffer {
    int* data;   // raw pointer to heap

public:
    DataBuffer(int n) : data(new int[n]) {}
    // compiler-generated destructor:
    //   ~DataBuffer() {}
    //   data (the pointer variable) is destroyed
    //   but delete[] data is NEVER called — heap block leaks
};
```

This is the direct motivation for writing a destructor manually: whenever a class owns a raw pointer, the compiler-generated destructor is wrong.

## RAII — The Design Pattern the Destructor Enables
**RAII** stands for **Resource Acquisition Is Initialization**. It is the central resource management pattern in C++ and is entirely built on the guarantee that destructors run automatically.

The principle:
- Acquire a resource in the constructor
- Release the resource in the destructor
- Tie the resource's lifetime to the object's lifetime
- Let scope control the object's lifetime — and therefore the resource's+

```cpp
class FileHandle {
    FILE* file;

public:
    FileHandle(const char* path) {
        file = fopen(path, "r");
        if (!file) throw std::runtime_error("Cannot open file");
    }

    ~FileHandle() {
        if (file) fclose(file);   // always runs — scope exit or exception
    }

    // ... read methods ...
};

void processFile() {
    FileHandle fh("data.txt");    // file opened here
    fh.readAll();
    // fh.doMoreWork();
    // ... any path through this function ...
}   // file closed here — always, without fail, even on exception
```
Without RAII, every function that uses a resource must have explicit cleanup on every exit path — every `return`, every `throw`, every `break`. As functions grow, paths are missed. With RAII, the destructor is the single cleanup point. It cannot be bypassed.

The standard library is built on RAII:
- `std::vector` — heap buffer freed in destructor
- `std::string` — heap buffer freed in destructor
- `std::fstream` — file closed in destructor
- `std::unique_ptr` — `delete` called in destructor
- `std::lock_guard` — mutex unlocked in destructor

## Destructors Must Not Throw
If a destructor throws an exception and that destructor was already called as part of stack unwinding from another exception, the program calls `std::terminate` immediately. Two simultaneous exceptions cannot be handled.

```cpp
class Dangerous {
public:
    ~Dangerous() {
        throw std::runtime_error("error in destructor");  // DANGEROUS
    }
};

void example() {
    Dangerous d;
    throw std::runtime_error("first exception");
    // stack unwinding calls d's destructor
    // destructor throws a second exception
    // std::terminate() — program crashes
}
```
**Rule:** Destructors must never throw. All cleanup operations inside a destructor must either be inherently non-throwing or be wrapped in a try/catch that swallows the exception internally.

```cpp
class Safe {
    FILE* file;
public:
    ~Safe() noexcept {                  // noexcept documents the guarantee
        try {
            if (file) fclose(file);
        } catch (...) {
            // log the error, but do not propagate — destructor must not throw
        }
    }
};
```
### `noexcept` on Destructors
Destructors are implicitly `noexcept` in C++11 and later — the compiler marks them `noexcept` automatically unless the destructor explicitly throws or calls a function that may throw.

```cpp
class MyClass {
public:
    ~MyClass() {}             // implicitly noexcept — standard behavior
    ~MyClass() noexcept {}    // explicit — same result, self-documenting
};
```
The standard library uses `noexcept` on destructors to enable move optimizations. `std::vector` will use move operations (instead of copy) when resizing only if the move constructor and move assignment of the contained type are `noexcept`. A throwing destructor signals an unreliable type and disables these optimizations.

## `= default` and `= delete` for Destructors
### `= default`
Explicitly requests the compiler-generated destructor. Used to document intent and to restore the generated destructor when it would otherwise be suppressed.

### `= delete`
Deletes the destructor. An object of this type cannot be destroyed — it cannot be allocated on the stack (stack exit would call the destructor) and cannot be `delete`d from the heap.

```cpp
class Permanent {
public:
    ~Permanent() = delete;
};

// Permanent p;           // COMPILE ERROR: destructor is deleted — cannot be on stack
Permanent* p = new Permanent();   // heap allocation is valid
// delete p;              // COMPILE ERROR: destructor is deleted
```
This is used in rare architectural cases where object lifetime is controlled entirely by a custom mechanism (such as a memory pool that manages destruction explicitly).

