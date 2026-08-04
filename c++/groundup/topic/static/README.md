# Static Members

## The Problem — Data That Belongs to the Class, Not an Object

Every data member established so far belongs to a specific object. When three `BankAccount` objects exist, three separate `balance` variables exist one per object, each at its own memory address.

Some data is not specific to any one object. It belongs to the class itself and is shared across all objects. A count of how many `BankAccount` objects currently exist is not a property of any individual account — it is a property of the class as a whole.

```cpp
class BankAccount {
    int id;
    double balance;
    int totalAccounts;   // WRONG approach — every object gets its own copy
                         // each object tracks its own count independently
                         // no shared state — counts never agree
public:
    BankAccount(int i, double b) : id(i), balance(b) {
        totalAccounts++;   // increments this object's own copy — meaningless
    }
};
```

Without a mechanism for class-level shared data, this problem has no clean solution. Global variables work but pollute the namespace and have no connection to the class. `static` data members solve this.


## Static Data Members

### What a Static Data Member Is

A `static` data member belongs to the **class itself**, not to any individual object. There is exactly **one copy** in memory, shared by all objects of the class and accessible without any object existing at all.

```cpp
class BankAccount {
    int    id;
    double balance;

public:
    static int totalAccounts;   // one copy, shared by all BankAccount objects

    BankAccount(int i, double b) : id(i), balance(b) {
        totalAccounts++;        // increments the single shared counter
    }

    ~BankAccount() {
        totalAccounts--;        // decrements when any object is destroyed
    }
};

// Definition outside the class — allocates the actual memory
int BankAccount::totalAccounts = 0;
```

**Memory layout:**
```
Stack/Heap — each object has its own slot:
┌──────────────┬────────────────────────────┐
│ Object acc1  │ id=101 balance=1000.0      │
├──────────────┼────────────────────────────┤
│ Object acc2  │ id=102 balance=500.0       │
├──────────────┼────────────────────────────┤
│ Object acc3  │ id=103 balance=250.0       │
└──────────────┴────────────────────────────┘

Static segment — one slot for the whole class:
┌──────────────────────────────────────────┐
│ BankAccount::totalAccounts = 3           │
└──────────────────────────────────────────┘
```
`id` and `balance` exist three times — once per object. `totalAccounts` exists once — shared by all.

### Declaration vs Definition

A `static` data member is **declared** inside the class body and **defined** outside it. The declaration announces that the member exists. The definition allocates the actual memory.

```cpp
// Inside the class body — declaration only, no memory allocated
class BankAccount {
public:
    static int totalAccounts;   // declaration
};

// Outside the class body — definition, memory allocated here
int BankAccount::totalAccounts = 0;
```

The definition must appear in **exactly one** `.cpp` file. If it is placed in a header and that header is included in multiple `.cpp` files, the linker sees multiple definitions and reports a duplicate symbol error — an ODR violation.

### Accessing Static Data Members

A static data member can be accessed through the class name directly (preferred), through an object, or through a pointer — all three access the same single memory location:

```cpp
// Through the class name — preferred, makes static nature explicit
BankAccount::totalAccounts = 5;
std::cout << BankAccount::totalAccounts;

// Through an object — valid but misleading, looks like an instance member
BankAccount acc(101, 1000.0);
std::cout << acc.totalAccounts;   // same as BankAccount::totalAccounts

// Through a pointer
BankAccount* p = &acc;
std::cout << p->totalAccounts;    // same memory location
```

Accessing through an object does not require the object to exist in any meaningful state — the object is irrelevant to accessing a static member. The class-name form `BankAccount::totalAccounts` makes this explicit and is the conventional style.


## `inline static` — C++17 In-Class Definition

Before C++17, every static data member required a separate out-of-class definition in a `.cpp` file. C++17 introduced `inline static`, which allows the definition to appear directly inside the class body in a header file without violating the ODR.

```cpp
// C++17 — entire definition in the header, safe to include in multiple files
class BankAccount {
public:
    inline static int totalAccounts = 0;   // declared AND defined here
};

// No separate .cpp definition needed
```

`inline static` is the modern convention for static data members that can be initialized with a constant expression. It removes the need for a separate `.cpp` file entry for each static member.


## `const static` and `constexpr static` Members

### `const static` Integer Members — In-Class Initialization

For `static const` members of integer or enumeration type, the value can be provided directly inside the class body without `inline`:

```cpp
class Config {
public:
    static const int maxConnections = 100;     // in-class initialization allowed
    static const double pi = 3.14159;          // COMPILE ERROR: not integer type
                                                // requires inline or out-of-class
};
```

This in-class initialization is a declaration with an initializer — it does not allocate storage unless the member is **odr-used** (its address is taken or it is bound to a reference). For simple compile-time constant uses, no out-of-class definition is needed.

Historically, C++ allowed static const integer and enumeration types to be initialized inline because the compiler can easily evaluate and substitute integer constants directly into the code (like a #define macro, but with type safety).

For floating-point numbers (double, float) or non-integral objects, the compiler could not guarantee that initialization wouldn't require runtime code or complex storage allocation before C++11.

### 4.2 `constexpr static` Members

`constexpr static` members are implicitly `inline` (established in the ODR document). A single in-class definition is sufficient and safe in headers:

```cpp
class MathConstants {
public:
    static constexpr double pi      = 3.14159265358979;
    static constexpr double e       = 2.71828182845904;
    static constexpr int    maxIter = 1000;
};

// No out-of-class definition needed
// Safe to include in multiple files — inline implied by constexpr
double circumference = 2 * MathConstants::pi * radius;
```

## Static Member Functions

### What a Static Member Function Is

A `static` member function belongs to the class, not to any object. It has no `this` pointer. It cannot access non-static data members or call non-static member functions — because those require an object, and no object is associated with a static member function call.

```cpp
class BankAccount {
    int    id;
    double balance;

public:
    inline static int totalAccounts = 0;

    BankAccount(int i, double b) : id(i), balance(b) { totalAccounts++; }
    ~BankAccount() { totalAccounts--; }

    static int getTotalAccounts() {
        return totalAccounts;   // VALID: accessing static member
        // return balance;      // COMPILE ERROR: balance requires an object
        // return this->id;     // COMPILE ERROR: this does not exist
    }
};
```

### Why There Is No `this`

`this` is the address of the object on which the function was called. A static member function is not called on any object — it is called on the class. There is no object, therefore no address, therefore no `this`.

At the machine level: a regular member function call passes the object's address as a hidden first argument. A static member function call passes no hidden argument. The assembly generated is identical to a free function call.

```asm
; Non-static call: acc.deposit(100)
lea  rdi, [acc]      ; load address of acc — the hidden 'this'
mov  esi, 100        ; argument
call deposit

; Static call: BankAccount::getTotalAccounts()
call getTotalAccounts  ; no hidden address passed — no object involved
```

### Calling Static Member Functions

A static member function can be called through the class name (preferred) or through an object:

```cpp
// Through the class name — preferred
int count = BankAccount::getTotalAccounts();

// Through an object — valid but misleading
BankAccount acc(101, 1000.0);
int count = acc.getTotalAccounts();   // same result — object is irrelevant

// Through a pointer
BankAccount* p = &acc;
int count = p->getTotalAccounts();    // same result
```

### 5.4 What Static Member Functions Cannot Do

| Operation | Allowed | Reason |
|---|---|---|
| Access static data members | YES | No object needed |
| Call other static member functions | YES | No object needed |
| Access non-static data members | NO | Requires an object — no `this` |
| Call non-static member functions | NO | Requires an object — no `this` |
| Be marked `const` | NO | `const` modifies `this` — no `this` exists |
| Be marked `virtual` | NO | Virtual dispatch requires a `this` pointer and vtable |
| Be marked `volatile` | NO | Same reason as `const` |

```cpp
class Example {
    int data;
public:
    static void process() const {}    // COMPILE ERROR: const on static
    static void dispatch() virtual {} // COMPILE ERROR: virtual on static
};
```

## Static Members and the Class Lifecycle

### When Static Members Are Initialized

Static data members with constant initializers are initialized at **compile time** or before `main` runs. Static members requiring runtime initialization (for example, a static `std::string`) are initialized before the first use in the translation unit, following the same static initialization rules established in the Object Allocation document.

```cpp
class Registry {
public:
    inline static int count   = 0;                  // zero at compile time
    inline static std::string name = "default";     // initialized before main
};
```

### Static Members Are Not Part of Object Size

`sizeof` on a class does not include static data members. They live in the static segment, not inside any object:

```cpp
class BankAccount {
    int    id;          // 4 bytes
    double balance;     // 8 bytes

    inline static int totalAccounts = 0;   // NOT included in sizeof
};

sizeof(BankAccount);   // 16 (int + padding + double) — totalAccounts not counted
```

### Static Members Across Translation Units

A static data member defined in a `.cpp` file is shared across all translation units in the program — all `.cpp` files that include the header see the same single memory location. This is different from a `static` local variable or a file-scope `static` variable, which are each private to their translation unit.


## Practical Patterns Using Static Members

### Object Counter

Track how many instances of a class currently exist:

```cpp
class Connection {
    inline static int activeCount = 0;

public:
    Connection()  { activeCount++; }
    ~Connection() { activeCount--; }

    static int getActiveCount() { return activeCount; }
};
```

### Singleton — Construct on First Use

A single shared instance, created lazily the first time it is needed. Thread-safe in C++11 and later (static local initialization is guaranteed thread-safe):

```cpp
class Config {
    std::string configPath;

    Config() : configPath("/etc/app/config") {}   // private constructor

public:
    static Config& getInstance() {
        static Config instance;   // constructed once, on first call
        return instance;
    }

    const std::string& getPath() const { return configPath; }

    // Prevent copying — only one instance should exist
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;
};

Config& cfg = Config::getInstance();   // first call: constructs instance
Config& cfg2 = Config::getInstance();  // subsequent calls: same instance
```

### Class-Level Constants

Compile-time constants that belong conceptually to the class:

```cpp
class Circle {
    double radius;

public:
    static constexpr double pi = 3.14159265358979;

    Circle(double r) : radius(r) {}

    double area()        const { return pi * radius * radius; }
    double circumference() const { return 2 * pi * radius; }
};
```

### Factory with Tracking

```cpp
class Widget {
    int id;
    inline static int nextId = 1;

    Widget(int i) : id(i) {}   // private constructor

public:
    static Widget create() {
        return Widget(nextId++);   // each created widget gets a unique id
    }

    int getId() const { return id; }
};

Widget a = Widget::create();   // id = 1
Widget b = Widget::create();   // id = 2
Widget c = Widget::create();   // id = 3
```

## `static` — Two Different Meanings in C++

The `static` keyword has two completely unrelated meanings in C++ depending on where it appears. This is a known source of confusion.

| Location | Meaning |
|---|---|
| Inside a class (data member or function) | Belongs to the class, not to any object — this document |
| Outside a class at file scope | Internal linkage — the name is private to this translation unit |
| Inside a function (local variable) | Static storage duration — initialized once, persists across calls |

```cpp
static int filePrivate = 0;    // file scope — internal linkage
                                // different from class static entirely

class Example {
    static int classLevel;     // class static — one shared instance
};

void func() {
    static int callCount = 0;  // local static — persists across calls
    callCount++;
}
```

File-scope `static` and local `static` were covered in the Object Allocation document. The class-member meaning is what this document covers.