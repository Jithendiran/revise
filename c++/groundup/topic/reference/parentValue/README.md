## GLvalue

A **glvalue** (generalized lvalue) is a broad, high-level category in the C++ type system. It represents any expression that evaluates to an **identifiable memory location** (an address).

To understand glvalues, it helps to look at the C++ value category family tree. The compiler splits expressions into two fundamental properties:

1. **Has identity?** (Does it reside at a specific, trackable memory address?)
2. **Is it eligible for moving?** (Can we safely steal its resources because it is expiring?)

A **glvalue** is the answer to the first question. It is any expression that **has identity**, regardless of whether it is expiring or permanent.


## The Composition of glvalues

The glvalue category is not a concrete category you code with directly; rather, it is the parent category that encompasses two distinct sub-categories:

$$\text{glvalue} = \text{lvalue} + \text{xvalue}$$

### 1. lvalues (Identity + Cannot Be Moved)

These are your classic, named variables. They have a permanent address in memory and are not about to expire.

```cpp
int x = 10;
// 'x' is a glvalue (specifically, an lvalue). 
// It has a clear address, and it is not expiring.

```

### 2. xvalues (Identity + Can Be Moved)

These are expiring objects. They still have a trackable address in memory, but they are marked as safe to move from.

```cpp
std::string s = "hello";
// 'std::move(s)' is a glvalue (specifically, an xvalue).
// It still points to the memory address of 's', but it is marked for expiration.

```

---

## Why does the compiler care about "glvalues"?

The compiler uses the term **glvalue** whenever a rule applies equally to both normal variables (lvalues) and expiring variables (xvalues). The common denominator is always: **"Does this expression refer to an actual object in memory?"**

Here are the scenarios where the glvalue classification matters:

### 1. Polymorphism and Dynamic Types

Polymorphic behavior (using a base class pointer/reference to call a `virtual` function of a derived class) requires a concrete object in memory to look up the virtual table (vtable). Therefore, the expression must be a glvalue.

```cpp
struct Base { virtual void run() {} };
struct Derived : Base { void run() override {} };

Derived d;
Base& ref1 = d;                 // 'd' is an lvalue (glvalue) -> Works
Base&& ref2 = std::move(d);    // 'std::move(d)' is an xvalue (glvalue) -> Works

```

Because both expressions are glvalues, the compiler can safely locate the object's memory to determine its dynamic type.

### 2. Lvalue Reference Binding Rules

A standard, mutable lvalue reference (`T&`) requires an identity to bind to, but it strictly forbids binding to expiring resources.

* It requires a **glvalue** (must have an address).
* It rejects **rvalues** (cannot be moving/expiring).

By looking at the taxonomy, the only category that is a glvalue but *not* an rvalue is an **lvalue**.

## Rvalue
## rvalue (The Parent Category)

Before breaking down **prvalue**, it is essential to look at the other half of the C++ value category tree. The counterpart to a glvalue is an **rvalue**.

An rvalue is any expression that represents an **expiring resource or a pure literal value**. It is the answer to the question: *"Is this value safe to move from?"*

The rvalue category splits into two sub-categories:

$$\text{rvalue} = \text{xvalue} + \text{prvalue}$$

While an xvalue has an identity (an address in memory), a **prvalue** does not.

---

## What is a prvalue (Pure rvalue)?

A **prvalue** (pure rvalue) is an expression that **computes a value but does not have an identifiable memory address** in the context of the running program. It represents pure data initialization before it is materialized into an actual memory slot.

A prvalue has two defining characteristics:

1. **No Identity:** The program cannot locate its address using the address-of operator (`&`).
2. **Eligible for Moving:** Its resources can be completely stolen or optimized away because it has no permanent home.

---

## Common Examples of prvalues

### 1. Literals (Except String Literals)

Basic literals are pure values loaded directly into CPU registers; they do not live in a dedicated RAM address.

```cpp
42;      // prvalue: an integer literal
true;    // prvalue: a boolean literal
1.23f;   // prvalue: a float literal

// Exception: "hello" is an lvalue because string literals 
// must reside permanently in the static data segment of RAM.

```

### 2. Function Calls Returning Non-References

When a function returns an object by value, that returned expression is a prvalue.

```cpp
int getValue() { return 100; }
std::string getName() { return "Alice"; }

getValue(); // prvalue
getName();  // prvalue

```

### 3. Arithmetic and Logical Expressions

The temporary results computed on the fly by operators are prvalues.

```cpp
int x = 10;
int y = 20;

x + y;   // prvalue: The result (30) exists temporarily in a CPU register
x < y;   // prvalue: The boolean result is temporary

```

### 4. Temporary Objects Created Explicitly

Instantiating an un-named object directly inside an expression creates a prvalue.

```cpp
struct Widget { int id; };

Widget{}; // prvalue: A temporary object with no name and no persistent address

```

---

## The Modern Role of prvalues: Temporary Materialization

In modern C++ (C++17 and newer), a prvalue is not viewed as a "temporary object" right away. Instead, it is viewed as a **recipe for initialization**.

A prvalue does not actually occupy memory until the compiler is absolutely forced to create a physical object for it. This transition from a pure value recipe to a physical address is called **Temporary Materialization**.

```cpp
struct Resource { /* ... */ };
Resource getResource() { return Resource{}; }

// Inside main:
Resource r = getResource(); 

```

### The Materialization Process

1. `Resource{}` inside the function is a **prvalue**.
2. `return Resource{}` passes that **prvalue** recipe out of the function.
3. `Resource r = getResource();` evaluates the **prvalue**.

Because of modern compiler rules (Guaranteed Copy Elision), the compiler uses the prvalue recipe to construct the object `r` **directly inside its final memory destination**. No temporary object is ever created on the stack, and no copy or move constructors are even called. The prvalue simply materializes straight into `r`.

```
                  [ Expression ]
                   /          \
                  /            \
          [ glvalue ]        [ rvalue ]
           /       \          /       \
          /         \        /         \
   [ lvalue ]      [ xvalue ]      [ prvalue ]
```