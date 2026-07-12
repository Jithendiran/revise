## Member Initializer List

### The Problem — What Constructor Body Assignment Actually Does

Consider a class with a constructor that sets members in the body:

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount(int accountId, double initialBalance) {
        id      = accountId;        // assignment
        balance = initialBalance;   // assignment
    }
};
```
The members get the right values. But two separate operations are happening for every member, not one.

**Step 1 — Before the constructor body runs:**

The compiler must first bring each data member into existence. For primitive types (`int`, `double`), this leaves them with garbage values. For class-type members, this calls their **default constructor** automatically.

**Step 2 — Inside the constructor body:**

The assignments overwrite those initial values with the intended ones.

For primitives this is only slightly wasteful — one write to set garbage, one write to overwrite it. 
For class-type members it is more expensive — the default constructor runs a full initialization, then the assignment overwrites that work entirely.

### The Solution — Member Initializer List
A member initializer list initializes each member **directly** at the moment it is constructed — skipping the default initialization step entirely.

**Syntax:** A colon `:` after the constructor's parameter list, followed by comma-separated `member(value)` pairs, before the opening brace `{`.

```cpp
class BankAccount {
    int id;
    double balance;

public:
    BankAccount(int accountId, double initialBalance)
        : id(accountId),            // direct initialization — one operation
          balance(initialBalance)   // direct initialization — one operation
    {
        // constructor body — all members already initialized
    }
};
```
The body can still contain logic — it just no longer contains initialization. Initialization belongs in the list. Logic belongs in the body.

### Cases Where the Initializer List Is Mandatory
For primitives and class types with default constructors, body assignment is wasteful but functional. For three specific cases, body assignment does not compile at all. The initializer list is the only option.

#### `const` Member Variables
A `const` member must be initialized at the point it is constructed. It cannot be assigned to afterward — `const` blocks all writes after initialization. Body assignment happens after construction, so it is already too late.

```cpp
class Config {
    const int maxConnections;

public:
    Config(int max)
        : maxConnections(max)   // VALID: direct initialization
    {
        // maxConnections = max; //  COMPILE ERROR: assignment of read-only member
    }

    // Body assignment version:
    // Config(int max) {
    //     maxConnections = max;   // COMPILE ERROR: assignment to const member
    // }
};
```

#### Reference Member Variables
A reference must be **bound** at the moment it is created. Binding is permanent — a reference cannot be rebound to a different object later. Body assignment does not bind; it writes through the reference to whatever it already points at. Since an unbound reference cannot exist, the compiler rejects construction without an initializer list entry.

```cpp
class Logger {
    int& streamId;   // reference member

public:
    Logger(int& id)
        : streamId(id)   // VALID: binds the reference at construction
    {}

    // Body assignment version:
    // Logger(int& id) {
    //     streamId = id;   // COMPILE ERROR: streamId is unbound — cannot exist
    // }
};
```

####  Class-Type Members With No Default Constructor
If a data member is a class type and that class has no default constructor, the compiler cannot default-construct it in Step 1. There is no fallback. The member must be initialized directly via the initializer list.

```cpp
class Engine {
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) {}
    // No default constructor — Engine() does not exist
};

class Car {
    Engine engine;   // Engine has no default constructor

public:
    Car(int hp)
        : engine(hp)   // VALID: directly constructs Engine with hp
    {}

    // Body assignment version:
    // Car(int hp) {
    //     engine = hp;   // COMPILE ERROR: Engine cannot be default-constructed
    //                    // before the body runs
    // }
};
```

### Initialization Order — Declaration Order, Not List Order
Members are initialized in the order they are **declared in the class body**, not in the order they appear in the initializer list. The compiler silently reorders the initializer list to match the declaration order.

This becomes a bug when one member's initializer references another member that has not been initialized yet.


```cpp
class Range {
    int size;   // declared FIRST now
    int min;
    int max;

public:
    Range(int lo, int hi)
        : min(lo),
          max(hi),
          size(max - min)   // BUG: size is initialized FIRST by declaration order
                            // max and min are garbage at this point
    {}
};
```

**corrected version**

```cpp
class Range {
    int min;
    int max;
    int size;   // declared third

public:
    Range(int lo, int hi)
        : max(hi),         // listed first
          min(lo),         // listed second
          size(max - min)  // listed third — but is max initialized yet?
    {}
};
```
The declaration order is: `min`, `max`, `size`. The compiler initializes in that order regardless of what the list says. So the actual execution is:
1. min  = lo          (OK)
2. max  = hi          (OK)
3. size = max - min   (OK — both are already initialized by declaration order)

###  `this` in the Initializer List
`this` is accessible inside the initializer list, but the object is only partially constructed at that point. Only members that have already been initialized (those declared before the current one in declaration order) are safe to access through `this`.

```cpp
class Node {
    int value;
    int doubled;
    Node* self;

public:
    Node(int v)
        : value(v),
          doubled(value * 2),    // VALID: value is already initialized
          self(this)             // VALID: stores address of the object,
                                 // but only value and doubled are safe
                                 // to access through self at this point
    {}
};
```

Passing `this` to an external function from the initializer list is dangerous. If that function accesses members that have not yet been initialized in the list, the behavior is undefined.