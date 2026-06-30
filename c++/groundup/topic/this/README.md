## This

### The Problem this Solves
1. Variables are unique per object — every object gets its own separate memory slot for data members.
2. Functions are shared — only one copy of the function instructions exists in memory, shared by all objects.

This creates an immediate problem. Consider three bank accounts calling the same deposit function:

```cpp
class BankAccount {
public:
    double balance;
    void deposit(double amount) {
        balance += amount;   // Which object's balance?
    }
};

BankAccount acc1, acc2, acc3;
acc1.deposit(100);
acc2.deposit(200);
acc3.deposit(300);
```
```
Memory:
┌──────────────┬──────────────────────┐
│  acc1        │  balance = 0.0       │  ← at 0x1000
├──────────────┼──────────────────────┤
│  acc2        │  balance = 0.0       │  ← at 0x1020
├──────────────┼──────────────────────┤
│  acc3        │  balance = 0.0       │  ← at 0x1040
└──────────────┴──────────────────────┘

Function deposit():        ← single copy, shared by all three objects
  balance += amount        ← the word "balance" here is ambiguous
```

The function has one copy. When `acc2.deposit(200)` runs, the function must know it is operating on `acc2`'s `balance` at `0x1020`, not `acc1`'s balance at `0x1000`. The function has no name for the object that called it.

**`this` is the solution.** Every time a non-static member function is called on an object, the compiler secretly passes the memory address of that object to the function. This hidden address is accessible inside the function through the keyword `this`.

## What `this` Is at the Machine Level
`this` is a hidden pointer parameter added by the compiler to every non-static member function. The programmer does not write it. The caller does not pass it explicitly. The compiler inserts it automatically at both the call site and the function definition.

**Source code the programmer writes:**
 
```cpp
acc2.deposit(200);
```

**What the compiler actually generates (conceptually):**
 
```cpp
deposit(&acc2, 200);    // address of acc2 is passed as a hidden first argument
```

**Inside the function, source code:**
 
```cpp
void deposit(double amount) {
    balance += amount;
}
```
 
**What the compiler actually generates (conceptually):**
 
```cpp
void deposit(BankAccount* const this, double amount) {
    this->balance += amount;   // 'balance' is rewritten as this->balance
}
```

### The Exact Type of `this`

Inside a non-const member function of a class named `ClassName`, the type of `this` is:
```cpp
ClassName* const
```

This is a *constant pointer to the object*

```
ClassName* const this
↑                ↑
points to       the pointer itself
ClassName        is constant
```

Two separate facts from this type:
 
1. **`ClassName*` — pointer to ClassName:** The pointer targets the object. The object's data members can be read and modified through this pointer.
2. **`const` after `*` — the pointer is constant:** The pointer itself cannot be reassigned to point to a different object. `this` is permanently bound to the object that triggered the function call.
```cpp
class BankAccount {
    double balance;
public:
    void deposit(double amount) {
 
        // this has type: BankAccount* const
 
        this->balance += amount;    // VALID: modifying data through the pointer
        // this = nullptr;          // COMPILE ERROR: the pointer itself is const
        // this = &other;           // COMPILE ERROR: cannot reassign this
    }
};
```

### Value Category of `this` — It Is a Prvalue
`this` is a **prvalue** (pure right value).

**The test — can the address of `this` be taken?**
 
```cpp
void BankAccount::deposit(double amount) {
    BankAccount** pp = &this;   // COMPILE ERROR: cannot take address of 'this'
}
```
`&this` fails. `this` has no stack slot of its own that can be addressed. It is a compiler-injected value that exists only during the function call — it lives in a CPU register, not as a named stack variable.
 
| Expression | Has fixed address | Category |
|---|---|---|
| `acc1` (the object) | YES — permanent stack slot | lvalue |
| `this` (inside member function) | NO — lives in register | prvalue |
| `*this` (the object this points to) | YES — same slot as `acc1` | lvalue |

### Can `this` Be Modified?
Two separate questions must be answered:

**Question 1: Can the `this` pointer itself be changed to point elsewhere?**
 
No. The pointer is `ClassName* const` — the `const` after `*` locks the pointer. Any attempt to reassign `this` is a compile error.
 
```cpp
void BankAccount::someFunction() {
    BankAccount other;
    // this = &other;    // COMPILE ERROR: assignment of read-only parameter 'this'
}
```
**Question 2: Can the data of the object `this` points to be modified?**
 
Yes — in a non-const member function. The left side of `*` is `ClassName` (no const), meaning the target data is freely writable through the pointer.
 
```cpp
void BankAccount::deposit(double amount) {
    this->balance += amount;   // VALID: modifying the object's data
}
```

### Assignment — `*this = cc`

```cpp
void deposit(BankAccount cc, double amount) {
    *this = cc;
}
```

This statement does not move or rebind the pointer. It overwrites the current object's data by invoking the **copy assignment operator**.

**Step 1 — Dereference produces the object.**

* `this` is a `BankAccount* const` — a pointer to the current object. Applying `*` to it produces the actual object at that address.
* `this`    --->  BankAccount* const  --->  a pointer (holds an address)
* `*this `  ---> ` BankAccount&`        --->  the actual object at that address (lvalue)

**Step 2 — The assignment calls the copy assignment operator.**

`*this = cc` is an assignment between two `BankAccount` objects. The compiler translates this into a call to the copy assignment operator:

```cpp
*this = cc;
// identical to:
this->operator=(cc);
```

**Step 3 — All data members are overwritten.**

Every data member of `cc` is copied into the corresponding slot of `*this`.
```
Before:                          After:

this       = 0x1000              this       = 0x1000   ← address unchanged

*this      = { balance: 0.0 }    *this      = { balance: 500.0 }

cc         = { balance: 500.0 }  cc         = { balance: 500.0 }
```
The pointer `this` never changed. The address `0x1000` remained fixed throughout.
Only the data stored at that address was overwritten.

**Distinction from rebinding:**

```cpp
this = &cc;     // ILLEGAL  — attempts to move the pointer to cc's address
*this = cc;     // LEGAL    — copies cc's data into the current object's address
```

The first line tries to change where `this` points.
The second line leaves the pointer unchanged and overwrites what it points to.

#### lvalue
```cpp
void BankAccount::someFunction() {
    BankAccount& ref = *this;   // VALID: *this is an lvalue, can bind to T&
    ref.balance = 100.0;        // modifies the same object
}
```
 
`ref` and `*this` refer to the exact same memory slot. Modifying through `ref` modifies the object directly.
 

###  Explicit vs Implicit Use of `this`
Inside any non-static member function, every access to a data member is implicitly rewritten by the compiler as a `this->member` access. Explicit use of `this->` is optional unless disambiguation is required.

**Implicit (what the programmer writes):**
 
```cpp
class BankAccount {
    double balance;
public:
    void deposit(double amount) {
        balance += amount;      // implicit this->balance
    }
};
```
 
**Explicit (identical behavior):**
 
```cpp
void BankAccount::deposit(double amount) {
    this->balance += amount;   // explicit — same machine code produced
}
```
 
Both forms compile to the same assembly instruction. The explicit form is used in two situations:
 
1. When a parameter name shadows a member name.
2. When the pointer or the object itself needs to be passed or returned.

### Disambiguating Member Names from Parameter Names
When a function parameter has the same name as a data member, the parameter name shadows the member name inside the function body. Without explicit `this->`, both the assignment source and destination resolve to the parameter — the member is never updated.

**The shadowing problem:**
 
```cpp
class BankAccount {
    int accountId;
    double balance;
public:
    void setBalance(double balance) {   // parameter named 'balance' shadows member 'balance'
        balance = balance;              // BUG: assigns parameter to itself, member unchanged
    }
};
```
 
**Solution using explicit `this->`:**
 
```cpp
void BankAccount::setBalance(double balance) {
    this->balance = balance;   // left side: member (via this)
                               // right side: parameter
}
```
 
The compiler resolves `this->balance` to the object's member slot and `balance` (unqualified) to the parameter. The assignment now correctly updates the member.

### Returning `this` — Returning a Pointer to Self
A member function can return `this` to give the caller a pointer to the object. The return type must be `ClassName*`.
 
```cpp
class BankAccount {
    double balance;
public:
    BankAccount* getPointerToSelf() {
        return this;   // returns the address of this object
    }
};
```
 
**Usage:**
 
```cpp
BankAccount acc;
BankAccount* p = acc.getPointerToSelf();
// p and &acc now hold the same address
```
 
**When this pattern is used:**
 
Factory functions, registry patterns, and certain container implementations return `this` to give external code a pointer to the object without requiring a separate `&acc` expression.
 
### Returning `*this` — Method Chaining

Returning `*this` returns the object itself by reference. The return type is `ClassName&`. This enables **method chaining** — multiple function calls on the same object in a single expression.
 
```cpp
class BankAccount {
    double balance;
public:
    BankAccount& deposit(double amount) {
        balance += amount;
        return *this;             // returns the object itself by reference
    }

    // this is valid, but it copies the data, it will give a brand new data of BankAccount
    BankAccount deposit1(double amount) {
        balance += amount;
        return *this;           // Note: This returns a COPY, not a reference!
    }
 
    BankAccount& withdraw(double amount) {
        balance -= amount;
        return *this;
    }
 
    BankAccount& applyInterest(double rate) {
        balance *= (1.0 + rate);
        return *this;
    }
};
```
 
**Without method chaining:**
 
```cpp
BankAccount acc;
acc.deposit(500.0);
acc.withdraw(100.0);
acc.applyInterest(0.05);
```
 
**With method chaining:**
 
```cpp
BankAccount acc;
acc.deposit(500.0).withdraw(100.0).applyInterest(0.05);
```
 
Each function returns `*this` — a reference to the same object. The next call in the chain is applied to the same object returned by the previous call.
 
**Memory behavior — no copies involved:**
 
```
acc at 0x1000
 
acc.deposit(500.0)    → this = &acc, modifies acc.balance, returns *this = acc (at 0x1000)
.withdraw(100.0)      → this = &acc (same address), modifies acc.balance, returns *this
.applyInterest(0.05)  → this = &acc (same address), modifies acc.balance, returns *this
```
 
All three calls operate on the same object. No temporary copies are created. This is different from returning by value (`return *this` with return type `ClassName`) which would copy the object at each step.

```cpp
BankAccount acc; // balance = 0
acc.deposit1(500.0).withdraw(100.0);
```
This breaks method chaining, `acc` will not be modified