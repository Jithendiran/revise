## Constructor Types

### The Problem Without Constructors
When a variable is declared without initialization, the compiler allocates a memory slot on the stack and moves on. No value is written into that slot. Whatever bit pattern happened to exist at that address from a previous operation becomes the starting value. This is called a **garbage value**.

```cpp
int x;
std::cout << x;   // could print anything — 0, -858993460, 1752392
```
For a primitive, a garbage value is a bug waiting to happen. For a class with multiple members, the situation is worse — every member independently holds garbage.

```cpp
struct BankAccount {
    int id;
    double balance;
};

BankAccount acc;
acc.balance -= 100.0;   // subtracting from garbage — completely undefined result
```

Without a constructor, the programmer must remember to manually set every member to a valid state immediately after creating every object. If one assignment is missed, the object is silently broken. There is no compiler warning. The program runs and produces wrong results.

**A constructor solves this by making it impossible for an object to exist in an uninitialized state.** Initialization is not a separate manual step — it is part of the object's creation.


### What a Constructor Is
A constructor is a special member function that runs automatically the moment an object is created in memory. It has two syntax rules that distinguish it from every other function:

1. The name must be **identical to the class name**
2. It must have **no return type** — not even `void`
3. 
```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount() {          // name matches class, no return type
        id      = 0;
        balance = 0.0;
    }
};

BankAccount acc;   // constructor runs here automatically — no manual call
```
The programmer never calls a constructor by name. The compiler calls it at the point of object creation.

### Default Constructor
A **default constructor** is any constructor that can be called with **no arguments** — either because it declares no parameters, or because every parameter has a default value.

#### User-Defined Default Constructor

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount() {              // no parameters — default constructor
        id      = 0;
        balance = 0.0;
    }
};

BankAccount acc;        // calls BankAccount()
BankAccount arr[3];     // calls BankAccount() for each element
```

#### All-Default-Parameters Version
A constructor where every parameter has a default value also qualifies as a default constructor — it can be called with zero arguments.

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount(int accountId = 0, double initialBalance = 0.0)
        : id(accountId), balance(initialBalance)
    {}
};

BankAccount acc;              // calls with id=0, balance=0.0
BankAccount acc2(101);        // calls with id=101, balance=0.0
BankAccount acc3(101, 500.0); // calls with both supplied
```
Only one default constructor can exist per class. If both `BankAccount()` and `BankAccount(int = 0, double = 0.0)` existed, a zero-argument call would be ambiguous — the compiler would not know which to call.


#### The Compiler-Generated Default Constructor
If a class defines **no constructors of any kind**, the compiler automatically generates a default constructor. This generated constructor:
- Calls the default constructor of every **class-type** data member
- Leaves every **primitive-type** data member (`int`, `double`, pointers)   **uninitialized** — the garbage value problem is not solved

```cpp
class Simple {
    int x;             // primitive — left uninitialized
    std::string name;  // class type — std::string's default constructor runs,
                       // name becomes ""
};

Simple s;   // s.x is garbage, s.name is ""
```

**The compiler stops generating a default constructor the moment a class defines any constructor of any kind.**

```cpp
class BankAccount {
    int id;
public:
    BankAccount(int accountId) { id = accountId; }  // user defined — any constructor
};

// BankAccount acc;    // COMPILE ERROR: no default constructor exists
                       // compiler will NOT generate one
BankAccount acc(101);  // VALID: must supply the argument
```
This is the most common constructor-related compile error. Adding a parameterized constructor silently removes the ability to default-construct the class unless a default constructor is also explicitly written.

### Parameterized Constructor
A **parameterized constructor** accepts one or more required arguments and uses them to initialize the object's members with caller-supplied values.

```cpp
class BankAccount {
    int id;
    double balance;
    std::string holderName;

public:
    BankAccount(int accountId, double initialBalance, const std::string& name)
        : id(accountId), balance(initialBalance), holderName(name)
    {}
};

BankAccount acc(101, 1000.0, "Alice");
```

A class can define multiple parameterized constructors with different parameter combinations — this is constructor overloading. Each overload represents a different valid way to supply the information needed to build the object.

```cpp
class BankAccount {
    int id;
    double balance;
    std::string holderName;

public:
    BankAccount(int accountId, double initialBalance)
        : id(accountId), balance(initialBalance), holderName("Unknown")
    {}

    BankAccount(int accountId, double initialBalance, const std::string& name)
        : id(accountId), balance(initialBalance), holderName(name)
    {}
};

BankAccount a(101, 1000.0);                 // two-argument version
BankAccount b(102, 500.0, "Alice");         // three-argument version
```

### Converting Constructor and `explicit`

#### Converting Constructor
Any constructor callable with **exactly one argument** defines an implicit conversion from that argument's type to the class type. The compiler can invoke it automatically whenever a value of the argument type appears where the class type is expected.

```cpp
class BankAccount {
    int id;
public:
    BankAccount(int accountId) : id(accountId) {}
};

BankAccount acc = 101;   // implicit conversion: 101 → BankAccount(101)
```

This also applies to constructors with multiple parameters where all but one have default values — the constructor can be called with one argument.

```cpp
class BankAccount {
    int id;
    double balance;
public:
    BankAccount(int accountId, double initialBalance = 0.0)
        : id(accountId), balance(initialBalance)
    {}
};

BankAccount acc = 101;   // still a converting constructor — callable with one arg
```
This also applies to constructors with multiple parameters where all but one have default values — the constructor can be called with one argument.

```cpp
class BankAccount {
    int id;
    double balance;
public:
    BankAccount(int accountId, double initialBalance = 0.0)
        : id(accountId), balance(initialBalance)
    {}
};

BankAccount acc = 101;   // still a converting constructor — callable with one arg
```

**The Silent Bug This Creates**

Implicit conversion causes the compiler to silently accept incorrect code:

```cpp
void printAccount(BankAccount acc) {
    // ...
}

printAccount(101);   // compiles silently — 101 converted to BankAccount(101)
                     // was this intended? probably not
```
A function expecting a `BankAccount` silently accepts a plain `int`. The compiler does not warn. The program may produce wrong results because the constructed object has no meaningful `balance`.

#### `explicit` — Blocking the Implicit Conversion
The `explicit` keyword on a constructor disables implicit conversion while leaving direct and brace initialization fully functional.
```cpp
class BankAccount {
    int id;
public:
    explicit BankAccount(int accountId) : id(accountId) {}
};

BankAccount a = 101;               // COMPILE ERROR: implicit conversion blocked
BankAccount b(101);                // VALID: direct initialization
BankAccount c{101};                // VALID: brace initialization
BankAccount d = BankAccount(101);  // VALID: explicit construction

void printAccount(BankAccount acc) {}
printAccount(101);                 // COMPILE ERROR: implicit conversion blocked
printAccount(BankAccount(101));    // VALID: explicit construction
```
**Rule:** Mark every single-argument constructor `explicit` by default. Remove `explicit` only when implicit conversion is a deliberate, well-understood design choice — for example, a `std::string` constructor accepting `const char*` is intentionally implicit so that `"hello"` can be passed wherever a `std::string` is expected.

### Copy Constructor
A copy constructor initializes a **new object** as a duplicate of an **existing object** of the same class. Its signature is fixed:

```cpp
ClassName(const ClassName& other);
```
- The parameter is a **reference** — passing by value would require calling the copy constructor to construct the parameter, which would call the copy   constructor again — infinite recursion

- The parameter is **`const`** — the source object is not modified during copying, and `const` also permits copying from `const` objects

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount(int accountId, double initialBalance)
        : id(accountId), balance(initialBalance)
    {}

    BankAccount(const BankAccount& other)   // copy constructor
        : id(other.id), balance(other.balance)
    {}
};

BankAccount a(101, 1000.0);
BankAccount b = a;    // copy constructor — b is a new object initialized from a
BankAccount c(a);     // copy constructor — same thing, different syntax
```

#### When the Copy Constructor Runs
Four situations trigger it automatically:

```cpp
BankAccount a(101, 1000.0);

BankAccount b = a;                    // 1. copy initialization

BankAccount c(a);                     // 2. direct initialization from existing object

void show(BankAccount acc) {}
show(a);                              // 3. passing by value to a function
                                      //    — a copy is made into the parameter slot

BankAccount makeAccount() {
    BankAccount temp(999, 0.0);
    return temp;                      // 4. returning by value
                                      //    — compiler may elide this copy (see below)
}
```

#### Compiler-Generated Copy Constructor
If no copy constructor is declared, the compiler generates one that performs a **member-by-member copy** — each member is copied from source to destination in declaration order.
```cpp
class Simple {
    int x;
    double y;
};

Simple a;  a.x = 5;  a.y = 2.5;
Simple b = a;   // compiler-generated: b.x = 5, b.y = 2.5
```
This is correct for classes whose members are primitives or well-behaved class types like `std::string`. It becomes a serious defect for classes holding raw pointers to heap memory — both objects end up pointing to the same heap block, and when one is destroyed, the other holds a dangling pointer. This is the **shallow copy problem**

#### When the Copy Constructor Does Not Actually Run
The compiler is permitted (and in C++17, required in specific cases) to skip the copy constructor entirely and construct the object directly in its final destination. This is called **copy elision** or **NRVO/RVO**.

```cpp
BankAccount makeAccount() {
    return BankAccount(101, 1000.0);   // C++17: constructed directly in caller's
}                                       // memory — no copy constructor called

BankAccount acc = makeAccount();       // no copy, no move — direct construction
```

This does not change the correctness requirement: the copy constructor must still be accessible (not deleted or private), even if it is never actually called. It only means that in well-defined cases the compiler avoids the overhead.

### Delegating Constructor

#### The Problem Without Delegation
Multiple constructors with overlapping initialization logic repeat that logic in every overload:

```cpp
class BankAccount {
    int id;
    double balance;
    std::string holderName;

public:
    BankAccount(int accountId)
        : id(accountId), balance(0.0), holderName("Unknown") {}

    BankAccount(int accountId, double initialBalance)
        : id(accountId), balance(initialBalance), holderName("Unknown") {}

    BankAccount(int accountId, double initialBalance, const std::string& name)
        : id(accountId), balance(initialBalance), holderName(name) {}
};
```
`id(accountId)` appears in all three. `holderName("Unknown")` appears in two. If the initialization logic needs to change, every constructor must be updated.

**Solution**

A delegating constructor calls another constructor of the same class from its initializer list. The target constructor runs first and completes fully, then the delegating constructor's body runs.

```cpp
class BankAccount {
    int id;
    double balance;
    std::string holderName;

public:
    // Primary constructor — contains the full logic
    BankAccount(int accountId, double initialBalance, const std::string& name)
        : id(accountId), balance(initialBalance), holderName(name)
    {}

    // Delegates to primary — supplies defaults
    BankAccount(int accountId, double initialBalance)
        : BankAccount(accountId, initialBalance, "Unknown")   // delegation
    {}

    BankAccount(int accountId)
        : BankAccount(accountId, 0.0, "Unknown")              // delegation
    {}
};

BankAccount a(101);                       // delegates twice → primary
BankAccount b(102, 500.0);                // delegates once → primary
BankAccount c(103, 250.0, "Alice");       // calls primary directly
```
**The Delegation Rule**

When a constructor delegates, the delegation call must be the **only** entry in the initializer list. A delegating constructor cannot also directly initialize any members — that work belongs to the constructor being delegated to.

```cpp
BankAccount(int accountId)
    : BankAccount(accountId, 0.0),   // delegation
      holderName("Unknown")          // COMPILE ERROR: cannot mix delegation
                                     // with direct member initialization
{}
```

### Constructor Overload Resolution
When multiple constructors exist, the compiler selects one at the call site based on the number and types of arguments supplied.

```cpp
class BankAccount {
public:
    BankAccount()                                { std::cout << "default\n"; }
    BankAccount(int id)                          { std::cout << "int\n"; }
    BankAccount(int id, double balance)          { std::cout << "int, double\n"; }
    BankAccount(const BankAccount& other)        { std::cout << "copy\n"; }
};

BankAccount a;                 // "default"
BankAccount b(101);            // "int"
BankAccount c(101, 500.0);     // "int, double"
BankAccount d(a);              // "copy"
BankAccount e = a;             // "copy"
```

If two constructors are equally valid matches for a given call, the compiler reports an ambiguity error rather than guessing:


```cpp
class Example {
public:
    Example(int x)    {}
    Example(double x) {}
};

Example e(5);     // AMBIGUOUS: 5 can match int or double equally
                  // COMPILE ERROR: call to constructor is ambiguous
Example e(5.0);   // VALID: 5.0 is double — unambiguous
Example e(5);     // Fix: cast  →  Example e(static_cast(5));
```

### Move Constructor

#### The Problem — Copying When a Copy Is Not Needed

When an object owns heap memory, copying it creates a second heap allocation and copies every byte:


```cpp
class Buffer {
    int* data;
    int  size;

public:
    Buffer(int n) : size(n), data(new int[n]) {}

    Buffer(const Buffer& other)       // copy constructor
        : size(other.size),
          data(new int[other.size])   // NEW heap allocation
    {
        for (int i = 0; i < size; i++)
            data[i] = other.data[i];  // copy every element
    }

    ~Buffer() { delete[] data; }
};

Buffer makeBuffer() {
    Buffer temp(1000);
    return temp;       // without move: allocates 1000 ints, copies all 1000
}

Buffer b = makeBuffer();   // temp is about to be destroyed — why copy it?
```
`temp` is going to be destroyed the moment `makeBuffer` returns. Allocating a new heap block and copying 1000 integers into it — only to then destroy the original — is pure waste. The heap block that `temp` already owns could simply be transferred to `b` directly.


#### What a Move Constructor Does
A move constructor **transfers ownership** of resources from an expiring object (an rvalue) to a new object, without allocating new memory or copying any data.

**Signature:**

```cpp
ClassName(ClassName&& other);
```
- The parameter is an **rvalue reference** (`&&`) — it only binds to temporary or expiring objects
- The parameter is **not `const`** — the source object must be modified (its internal pointer must be set to `nullptr` after the transfer)

```cpp
class Buffer {
    int* data;
    int  size;

public:
    Buffer(int n) : size(n), data(new int[n]) {}

    // Copy constructor — deep copy
    Buffer(const Buffer& other)
        : size(other.size), data(new int[other.size])
    {
        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }

    // Move constructor — ownership transfer
    Buffer(Buffer&& other)
        : size(other.size),
          data(other.data)    // steal the pointer — no new allocation
    {
        other.data = nullptr; // null out the source — prevents double-free
        other.size = 0;
    }

    ~Buffer() { delete[] data; }
};
```
The heap block was never touched. Only the address (8 bytes) moved between the two objects' stack slots.

#### When the Move Constructor Runs
The move constructor runs when an object is initialized from an **rvalue** — a temporary or an object explicitly marked as expiring with `std::move`:

```cpp
Buffer a(100);

Buffer b = Buffer(200);        // move: Buffer(200) is a temporary (prvalue)
Buffer c = std::move(a);       // move: std::move(a) converts a to xvalue
Buffer d = makeBuffer();       // move (or elision): return value is an rvalue
```

When an lvalue is passed, the **copy** constructor runs instead:

```cpp
Buffer e = a;   // copy: a is an lvalue — copy constructor, not move
```
After the move constructor runs, the source object (`other`) is in a **valid but unspecified state**. For class types that manage heap memory, "valid" means the destructor can run safely on it — hence setting `other.data = nullptr` and `other.size = 0`. A null pointer passed to `delete[]` is safe (it does nothing). Without the null-out, the destructor of the source would `delete[]` the same heap block again — a double-free crash.

#### Compiler-Generated Move Constructor

If a class declares no copy constructor, no copy assignment, no move assignment, and no destructor, the compiler generates a move constructor that performs a member-by-member move of each data member.

The moment any of those five is user-declared, the compiler stops generating the move constructor automatically.

Full rules for when each of the five special functions suppresses generation of the others are covered in the **Rule of Five** section.

### `= default` and `= delete`
The compiler generates certain constructors automatically under specific conditions. Those conditions are complex — adding one user-defined constructor silently removes the compiler-generated default. Adding a destructor silently suppresses the move constructor. These silent behaviors cause hard-to-find bugs.


#### `= default` — Explicitly Request Compiler Generation
`= default` tells the compiler: generate this function using the same rules it would apply automatically, even in situations where it would normally suppress generation.

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount(int accountId, double initialBalance)
        : id(accountId), balance(initialBalance)
    {}

    // Adding a parameterized constructor suppressed the compiler-generated default.
    // Restore it explicitly:
    BankAccount() = default;

    // Explicitly request compiler-generated copy constructor:
    BankAccount(const BankAccount&) = default;

    // Explicitly request compiler-generated move constructor:
    BankAccount(BankAccount&&) = default;
};
```
`= default` can also appear **outside the class body** in a `.cpp` file:

Using `= default` makes the intent explicit and visible to every reader. It also produces the most efficient implementation — the compiler-generated version can use memcpy-level optimizations that a hand-written version may not.

#### `= delete` — Explicitly Block a Constructor
`= delete` tells the compiler: this constructor must not exist. Any call that would invoke it is a compile error.

**Use case 1 — Prevent copying of a class that should not be copied:**
```cpp
class NetworkConnection {
public:
    NetworkConnection(const std::string& address) {}

    NetworkConnection(const NetworkConnection&) = delete;   // no copying
    NetworkConnection& operator=(const NetworkConnection&) = delete;
};

NetworkConnection a("192.168.1.1");
NetworkConnection b = a;    // COMPILE ERROR: copy constructor is deleted
```

**Use case 2 — Block specific argument types:**
```cpp
class SafeInt {
public:
    SafeInt(int value) {}
    SafeInt(double) = delete;   // prevents silent narrowing via double → int
};

SafeInt a(5);       // VALID
SafeInt b(5.0);     // COMPILE ERROR: constructor from double is deleted
```
**Use case 3 — Block default construction when it makes no sense:**
```cpp
class FileHandle {
public:
    FileHandle() = delete;                        // must provide a path
    FileHandle(const std::string& path) {}
};

FileHandle f;                   // COMPILE ERROR: no default constructor
FileHandle g("data.txt");       // VALID
```

### Default Member Initialization
Data members can be given a default value directly at the point of declaration inside the class body. This value is used by any constructor that does not explicitly initialize that member in its initializer list.
```cpp
class BankAccount {
    int    id      = 0;            // default value
    double balance = 0.0;          // default value
    std::string holderName = "Unknown";  // default value

public:
    BankAccount() {}    // id=0, balance=0.0, holderName="Unknown" — from defaults

    BankAccount(int accountId)
        : id(accountId)             // id comes from parameter
                                    // balance and holderName from class defaults
    {}

    BankAccount(int accountId, double initialBalance)
        : id(accountId),
          balance(initialBalance)   // holderName still from class default
    {}
};
```

### Execution Order — What Runs and When
When an object is created, the sequence is fixed:

1. Memory is allocated (stack slot reserved or heap block allocated)
2. Member initializer list runs — in declaration order, not list order
3. Constructor body runs
4. Object is ready for use


```cpp
class BankAccount {
    int id;
    double balance;
    std::string holderName;

public:
    BankAccount(int accountId, double initialBalance)
        : id(accountId),            // step 2a — id initialized
          balance(initialBalance),  // step 2b — balance initialized
          holderName("Unknown")     // step 2c — holderName initialized
    {
        // step 3 — body runs, all members already initialized
        std::cout << "BankAccount created\n";
    }
};
```

For delegating constructors the sequence extends:

1. Memory allocated
2. Delegated-to constructor's initializer list runs
3. Delegated-to constructor's body runs
4. Delegating constructor's body runs (its list was replaced by the delegation)
5. Object is ready