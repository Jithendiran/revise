# Memory Layout: What the Object Looks Like in RAM
## Why Memory Layout Matters
Understanding what a derived object looks like in memory explains:
- Why a derived pointer can be used where a base pointer is expected
- Why object slicing happens
- Why virtual functions need a vtable
- What `sizeof` returns for derived objects
- Why casting between base and derived pointers works without data copying

## The Core Rule — One Contiguous Block
A derived object is a **single, contiguous block of memory** that contains the base class portion first, followed by the derived class portion.

There are no gaps between them (subject to alignment padding). There is no separate allocation for the base portion — it is embedded directly inside the derived object.

```cpp
class BankAccount {
    int    accountId;   // 4 bytes
    double balance;     // 8 bytes
};
// sizeof(BankAccount) = 16 (4 + 4 padding + 8)

class SavingsAccount : public BankAccount {
    double interestRate;   // 8 bytes
};
// sizeof(SavingsAccount) = 24 (16 from BankAccount + 8 for interestRate)
```

```
┌─────────────────────────────────────────────────────┐
│ BankAccount portion                                 │
│ ┌──────────────┬──────────────┬───────────────┐     │
│ │ accountId    │ padding      │ balance       │     │
│ │ (4 bytes)    │ (4 bytes)    │ (8 bytes)     │     │
│ └──────────────┴──────────────┴───────────────┘     │
│ Offset 0 Offset 15                                  │
├─────────────────────────────────────────────────────┤
│ SavingsAccount portion                              │
│ ┌────────────────────────────────────────────┐      │
│ │ interestRate                               │      │
│ │ (8 bytes)                                  │      │
│ └────────────────────────────────────────────┘      │
│ Offset 16 Offset 23                                 │
└─────────────────────────────────────────────────────┘
```
The base class portion always comes first. This is a language guarantee, not an implementation detail

## Address Relationship — Base Pointer From Derived Object

Because the base portion is at the beginning of the derived object, the address of the derived object and the address of its base portion are the **same address**:

```cpp
SavingsAccount sa(1, 1000.0, 0.05);

BankAccount*    bptr = &sa;   // points to the BankAccount portion
SavingsAccount* sptr = &sa;   // points to the whole object

std::cout << bptr;   // prints: 0x1000
std::cout << sptr;   // prints: 0x1000  ← same address
```
A `BankAccount*` pointing to a `SavingsAccount` object is valid because the `BankAccount` portion is genuinely at that address. No data is copied. No conversion happens. The base pointer simply refers to the first part of the object.

## Three Levels Deep — Cumulative Layout

```cpp
class BankAccount {
    int    accountId;     // 4 bytes
    double balance;       // 8 bytes
};
// sizeof = 16

class SavingsAccount : public BankAccount {
    double interestRate;  // 8 bytes
};
// sizeof = 24

class PremiumSavingsAccount : public SavingsAccount {
    int bonusPoints;      // 4 bytes
};
// sizeof = 32 (24 + 4 + 4 padding)
```

```
Memory layout of PremiumSavingsAccount at address 0x1000:
┌──────────────────────────────────────────────────────────┐
│ BankAccount portion (offset 0, size 16)                  │
│ ┌────────────┬────────────┬─────────────────────────┐    │
│ │ accountId  │ padding    │ balance                 │    │
│ │ 4 bytes    │ 4 bytes    │ 8 bytes                 │    │
│ └────────────┴────────────┴─────────────────────────┘    │
├──────────────────────────────────────────────────────────┤
│ SavingsAccount portion (offset 16, size 8)               │
│ ┌──────────────────────────────────────────────────┐     │
│ │ interestRate                                     │     │
│ │ 8 bytes                                          │     │
│ └──────────────────────────────────────────────────┘     │
├──────────────────────────────────────────────────────────┤
│ PremiumSavingsAccount portion (offset 24, size 8)        │
│ ┌────────────────────┬────────────────────────────┐      │
│ │ bonusPoints        │ padding                    │      │
│ │ 4 bytes            │ 4 bytes                    │      │
│ └────────────────────┴────────────────────────────┘      │
└──────────────────────────────────────────────────────────┘
Total size: 32 bytes
```

Address relationships:

```cpp
BankAccount* bptr = &psa; // 0x1000 (base is at the start)
SavingsAccount* sptr = &psa; // 0x1000 (same — SavingsAccount
// starts at same address)
PremiumSavingsAccount* pptr = &psa; // 0x1000 (all three same)
```
For single inheritance, all three pointers hold the same address — each level's portion starts at the same base address because they stack sequentially.


## Verifying With Code

```cpp
#include <iostream>

class BankAccount {
    int    accountId;
    double balance;
public:
    BankAccount(int id, double bal) : accountId(id), balance(bal) {}
};

class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(int id, double bal, double rate)
        : BankAccount(id, bal), interestRate(rate) {}
};

class PremiumSavingsAccount : public SavingsAccount {
    int bonusPoints;
public:
    PremiumSavingsAccount(int id, double bal, double rate)
        : SavingsAccount(id, bal, rate), bonusPoints(0) {}
};

int main() {
    PremiumSavingsAccount psa(1, 1000.0, 0.05);

    PremiumSavingsAccount* pptr = &psa;
    SavingsAccount*        sptr = &psa;
    BankAccount*           bptr = &psa;

    std::cout << "sizeof BankAccount:            "
              << sizeof(BankAccount)            << "\n";
    std::cout << "sizeof SavingsAccount:         "
              << sizeof(SavingsAccount)         << "\n";
    std::cout << "sizeof PremiumSavingsAccount:  "
              << sizeof(PremiumSavingsAccount)  << "\n";

    std::cout << "PremiumSavingsAccount address: " << pptr << "\n";
    std::cout << "SavingsAccount address:        " << sptr << "\n";
    std::cout << "BankAccount address:           " << bptr << "\n";
}
```
Output:
```
sizeof BankAccount:            16
sizeof SavingsAccount:         24
sizeof PremiumSavingsAccount:  32
PremiumSavingsAccount address: 0x7ffcc936d810
SavingsAccount address:        0x7ffcc936d810
BankAccount address:           0x7ffcc936d810
```
All three pointers print the same address — the object starts at one location and all three class portions begin there.

## What sizeof Shows

`sizeof` on a derived type always returns the total size including all inherited portions:

```cpp
sizeof(BankAccount)           // 16 — accountId + padding + balance
sizeof(SavingsAccount)        // 24 — BankAccount (16) + interestRate (8)
sizeof(PremiumSavingsAccount) // 32 — SavingsAccount (24) + bonusPoints (4) + padding (4)
```

The derived class size is always greater than or equal to the base class size. It can never be smaller — the derived class always contains the complete base class.

## Stack Allocation vs Heap Allocation — Same Layout

The contiguous layout applies regardless of where the object lives:

**Stack:**
```cpp
SavingsAccount sa(1, 1000.0, 0.05);
// One contiguous stack slot — BankAccount portion + SavingsAccount portion
```

**Heap:**
```cpp
SavingsAccount* sa = new SavingsAccount(1, 1000.0, 0.05);
// One contiguous heap block — same internal layout
```

## What Changes With Virtual Functions

When a class has at least one virtual function, the compiler adds a hidden pointer — the **vptr** (virtual pointer) — to the object's memory layout. This pointer points to a table of function pointers called the **vtable**.

### Case 1: virtual without inheritance

```cpp
class BankAccount {
    int    accountId;
    double balance;
public:
    virtual void printStatement() { }   // virtual function added
};
```

Memory layout WITH virtual function:
```
┌──────────────────────────────────────────────────────┐
│ vptr (8 bytes on 64-bit) ← hidden, added by          │
│ the compiler                                         │
├──────────────────────────────────────────────────────┤
│ accountId (4 bytes)                                  │
├──────────────────────────────────────────────────────┤
│ padding (4 bytes)                                    │
├──────────────────────────────────────────────────────┤
│ balance (8 bytes)                                    │
└──────────────────────────────────────────────────────┘
sizeof(BankAccount) = 24 (was 16 without virtual)
```

Adding a virtual function increases `sizeof` by the size of one pointer (8 bytes on 64-bit systems). Adding more virtual functions does NOT increase the size further — one vptr is shared across all virtual functions of a class.

### Case 2: virtual on derived class
```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
    int    baseData;
    double baseBalance;

    Base(int d, double b) : baseData(d), baseBalance(b) {}
    // NO virtual function in Base
};

class Derived : public Base {
public:
    double derivedRate;

    Derived(int d, double b, double r)
        : Base(d, b), derivedRate(r) {}

    virtual void show() {   // virtual only in Derived
        std::cout << "Derived::show\n";
    }
};

int main() {
    Derived obj(1, 1000.0, 0.05);

    std::cout << "sizeof Base:    " << sizeof(Base)    << "\n";
    std::cout << "sizeof Derived: " << sizeof(Derived) << "\n\n";

    // Print addresses of each member
    uintptr_t obj_addr = reinterpret_cast<uintptr_t>(&obj);

    std::cout << "obj address:              " << &obj             << "\n";
    std::cout << "obj.baseData address:     " << &obj.baseData    << "\n";
    std::cout << "obj.baseBalance address:  " << &obj.baseBalance << "\n";
    std::cout << "obj.derivedRate address:  " << &obj.derivedRate << "\n\n";

    // Offsets from start of object
    std::cout << "offset of baseData:    "
              << reinterpret_cast<uintptr_t>(&obj.baseData)
               - obj_addr << "\n";
    std::cout << "offset of baseBalance: "
              << reinterpret_cast<uintptr_t>(&obj.baseBalance)
               - obj_addr << "\n";
    std::cout << "offset of derivedRate: "
              << reinterpret_cast<uintptr_t>(&obj.derivedRate)
               - obj_addr << "\n";
}
```
#### Output
```
sizeof Base:    16
sizeof Derived: 32

obj address:              0x7fff6fdfcf60
obj.baseData address:     0x7fff6fdfcf68
obj.baseBalance address:  0x7fff6fdfcf70
obj.derivedRate address:  0x7fff6fdfcf78

offset of baseData:    8
offset of baseBalance: 16
offset of derivedRate: 24
```

```
Memory layout of Derived (32 bytes total):
┌───────────────────────────────────────────┐
│  offset 0   │  vptr        │  8 bytes     │ ← inserted at the very start
├───────────────────────────────────────────┤
│  offset 8   │  baseData    │  4 bytes     │ ← Base portion PUSHED DOWN
│  offset 12  │  padding     │  4 bytes     │
│  offset 16  │  baseBalance │  8 bytes     │
├───────────────────────────────────────────┤
│  offset 24  │  derivedRate │  8 bytes     │ ← Derived portion
└───────────────────────────────────────────┘

sizeof Base     = 16  (Base itself has no vptr — unchanged)
sizeof Derived  = 32  (16 Base + 8 vptr + 8 derivedRate)
```

The `vptr` is at offset 0 — before even the base class members. The entire Base portion was pushed 8 bytes down to make room. Base itself is still 16 bytes — it has no virtual functions and no vptr inside its own definition.

### Case 3: Virtual in Both Base and Derived
```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
    int    baseData;
    double baseBalance;

    Base(int d, double b) : baseData(d), baseBalance(b) {}
    virtual void show() {   // virtual 
        std::cout << "Base::show\n";
    }
};

class Derived : public Base {
public:
    double derivedRate;

    Derived(int d, double b, double r)
        : Base(d, b), derivedRate(r) {}

    void show() override {
        std::cout << "Derived::show\n";
    }
    virtual void log() { // virtual
        std::cout << "Derived::log\n";
    }
};

int main() {
    Derived obj(1, 1000.0, 0.05);

    std::cout << "sizeof Base:    " << sizeof(Base)    << "\n";
    std::cout << "sizeof Derived: " << sizeof(Derived) << "\n\n";

    // Print addresses of each member
    uintptr_t obj_addr = reinterpret_cast<uintptr_t>(&obj);

    std::cout << "obj address:              " << &obj             << "\n";
    std::cout << "obj.baseData address:     " << &obj.baseData    << "\n";
    std::cout << "obj.baseBalance address:  " << &obj.baseBalance << "\n";
    std::cout << "obj.derivedRate address:  " << &obj.derivedRate << "\n\n";

    // Offsets from start of object
    std::cout << "offset of baseData:    "
              << reinterpret_cast<uintptr_t>(&obj.baseData)
               - obj_addr << "\n";
    std::cout << "offset of baseBalance: "
              << reinterpret_cast<uintptr_t>(&obj.baseBalance)
               - obj_addr << "\n";
    std::cout << "offset of derivedRate: "
              << reinterpret_cast<uintptr_t>(&obj.derivedRate)
               - obj_addr << "\n";
}
```

#### Output
```
sizeof Base:    24
sizeof Derived: 32

obj address:              0x7ffdc0250a70
obj.baseData address:     0x7ffdc0250a78
obj.baseBalance address:  0x7ffdc0250a80
obj.derivedRate address:  0x7ffdc0250a88

offset of baseData:    8
offset of baseBalance: 16
offset of derivedRate: 24
```

```
Memory layout of Derived (32 bytes total):
┌───────────────────────────────────────────┐
│  offset 0   │  vptr        │  8 bytes     │ ← Base's vptr
├───────────────────────────────────────────┤
│  offset 8   │  baseData    │  4 bytes     │ ← Base portion
│  offset 12  │  padding     │  4 bytes     │
│  offset 16  │  baseBalance │  8 bytes     │
├───────────────────────────────────────────┤
│  offset 24  │  derivedRate │  8 bytes     │ ← Derived portion
└───────────────────────────────────────────┘

sizeof Base     = 24  (16 data + 8 vptr — Base already has vptr)
sizeof Derived  = 32  (16 Base + 8 vptr + 8 derivedRate)
```

Only one vptr exists — the one Base introduced. Derived does not add a second vptr. It replaces the function pointer entry in the same vtable. This is why `sizeof(Derived)` = 32 and not 40 — overriding  and adding new virtual in derived does not create new vtable infrastructure.

### Difference in case 2 and case 3
In Case 2, `Base` has no virtual functions. The vptr does not belong to `Base`. When `sizeof(Base)` is measured, it measures `Base` alone — no vptr is inside.

In Case 3, `Base` has a virtual function. The vptr belongs to `Base` itself. When `sizeof(Base)` is measured, it measures `Base`  including its own vptr.

## Layout understanding

### 1. Why a Derived Pointer Can Be Used Where a Base Pointer Is Expected
A `SavingsAccount` object in memory looks like this:
```
Address 0x1000:
┌──────────────────────────────┐
│   BankAccount portion        │ ← a complete, valid BankAccount lives HERE
│   accountId + balance        │
├──────────────────────────────┤
│   SavingsAccount portion     │
│   interestRate               │
└──────────────────────────────┘
```
A `BankAccount*` pointing to `0x1000` is valid because a complete, genuine `BankAccount` actually exists at that address. It is not a simulation or a copy — it is the real base portion of the derived object. Every member a `BankAccount*` would ever read or write is physically present and correctly laid out starting at `0x1000`.

```cpp
SavingsAccount sa(1, 1000.0, 0.05);
BankAccount* bp = &sa;      // valid — a real BankAccount IS at &sa

bp->deposit(100);           // reads/writes balance — which is at offset 8
                            // the base pointer knows balance is at offset 8
                            // and it is — the layout guarantees it
```
The compiler hardcodes member offsets at compile time. balance is always at offset 8 from the start of a BankAccount. Since the base portion is always at the start of the derived object, offset 8 from the derived object's start is also offset 8 from the base pointer — same address, same data.

`bp` has type `BankAccount*`. The compiler uses the pointer type to determine what members and functions are accessible — not what the object actually is in memory.

`BankAccount` has no interestRate member and no `getInterestRate()` function. The compiler has no knowledge of `interestRate` when looking through a `BankAccount*`. It does not exist in `BankAccount's` definition.

```cpp
SavingsAccount sa(1, 1000.0, 0.05);
BankAccount* bp = &sa;

bp->deposit(100);          // VALID:   deposit is in BankAccount
bp->getBalance();          // VALID:   getBalance is in BankAccount
bp->interestRate;          // COMPILE ERROR: BankAccount has no interestRate
bp->applyInterest();       // COMPILE ERROR: BankAccount has no applyInterest
```

The Data IS There in Memory — But Unreachable Through `bp`

```
Address 0x1000:
┌──────────────────────────────────────┐
│  BankAccount portion                 │ ← bp can see and use this
│  accountId + balance                 │
├──────────────────────────────────────┤
│  SavingsAccount portion              │ ← bp is BLIND to this
│  interestRate    ← physically here   │    even though it exists
└──────────────────────────────────────┘
```

**How to Access interestRate Through a Base Pointer**
The pointer must be cast back to the derived type first:
```cpp
SavingsAccount* sp = static_cast<SavingsAccount*>(bp);
sp->applyInterest();    // VALID: now compiler sees SavingsAccount
sp->interestRate;       // VALID: now accessible
```
`static_cast` tells the compiler: "trust me, the object at this address is actually a SavingsAccount." The address does not change — only the type the compiler uses to interpret it. This is a downcast — going from base pointer back to derived pointer.

The safe version using `dynamic_cast`. it verifies at runtime that the object actually is a `SavingsAccount` before allowing the cast, rather than blindly trusting the programmer.

### 2. Why Object Slicing Happens
Object slicing happens when a derived object is copied by value into a base object. Only the base portion fits — the derived portion is cut off.
```cpp
SavingsAccount sa(1, 1000.0, 0.05);
BankAccount ba = sa;   // copy by value — ba is a BankAccount, not a SavingsAccount
```

```
sa in memory (24 bytes):               ba in memory (16 bytes):
┌──────────────────────┐               ┌──────────────────────┐
│  BankAccount portion │  ──copied──►  │  BankAccount portion │
│  accountId, balance  │               │  accountId, balance  │
├──────────────────────┤               └──────────────────────┘
│  SavingsAccount part │  ──LOST────►  (no room — ba is only 16 bytes)
│  interestRate        │
└──────────────────────┘
```

`ba` is exactly 16 bytes — the size of `BankAccount`. There is physically no space for interestRate. It is not copied. It is gone. The `SavingsAccount`-specific behavior is sliced off because the destination object is smaller than the source.

This never happens with pointers or references — a pointer is always the same size (8 bytes) regardless of what it points to. No slicing, no copying, no size mismatch.

### 3. Why Virtual Functions Need a vtable
Without virtual functions, the compiler decides at compile time which function to call based purely on the pointer type:
```cpp
BankAccount* bp = new SavingsAccount(1, 1000.0, 0.05);
bp->printStatement();
// Compiler sees: bp is BankAccount*
// Compiler calls: BankAccount::printStatement — always, no matter what
// Even if bp actually points to a SavingsAccount — wrong function called
```

The compiler cannot look inside the object at runtime to check what it actually is. It only knows the pointer type at compile time.

To call the correct function based on what the object actually is at runtime, some piece of information must be stored inside the object that identifies which function to call. That information is the `vptr` — a pointer stored at offset 0 of the object, pointing to a vtable that contains the correct function pointers for that object's actual type.

```
At runtime: bp->printStatement()

Step 1: read 8 bytes at offset 0 of the object → gets the vptr
Step 2: vptr points to the vtable
Step 3: look up printStatement's slot in the vtable
Step 4: call whatever function pointer is in that slot

If actual object is BankAccount   → vtable has BankAccount::printStatement
If actual object is SavingsAccount → vtable has SavingsAccount::printStatement
```
The vtable is necessary because the decision must be made at runtime using information stored in the object — the pointer type alone is not enough.

### 4. What `sizeof` Returns for Derived Objects
`sizeof(Derived)` = sum of all members across the entire inheritance chain.

alignment padding + vptr if any virtual function exists anywhere in the chain.

```
sizeof(BankAccount)    = 16    (int + padding + double)
sizeof(SavingsAccount) = 24    (16 from BankAccount + 8 for interestRate)

sizeof(Base)           = 24    (int + padding + double + 8 vptr)
sizeof(Derived)        = 32    (24 from Base3 + 8 for derivedRate)
```

### 5. Why Casting Between Base and Derived Pointers Works Without Data Copying
A cast between related pointer types is just a reinterpretation of the same address — no data moves, no object is constructed.
```
SavingsAccount* sptr = new SavingsAccount(1, 1000.0, 0.05);
BankAccount*    bptr = sptr;   // upcast — derived to base
Before cast:         After cast:
sptr = 0x1000        bptr = 0x1000   ← same address
                                         no data moved
                                         no new object
```

The pointer value is just copied — the same 8-byte address. The cast changes what the compiler thinks the pointer points to, not where it actually points.

**Downcast — base pointer back to derived:**
```cpp
BankAccount*    bptr = new SavingsAccount(1, 1000.0, 0.05);
SavingsAccount* sptr = static_cast<SavingsAccount*>(bptr);
```

Again — same address, no data copied. `static_cast` here is the programmer telling the compiler "trust me, the object at this address really is a SavingsAccount." The compiler changes the pointer type. The address stays the same.

```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
    int    baseData; // 4 
                    // 4 padding
    double baseBalance; // 8

    Base(int d, double b) : baseData(d), baseBalance(b) {}
    
    virtual void show() {   // 8 bytes for vtable
        std::cout << "Base::show\n";
    }
    
    // 4 + 4 + 8 + 8 = 24
};

class Derived : public Base {
public:
    double derivedRate;

    Derived(int d, double b, double r)
        : Base(d, b), derivedRate(r) {}

    void show() {   
        std::cout << "Derived::show\n";
    }
};

int main() {
    Derived obj(1, 1000.0, 0.05);
    Derived * ptr = new Derived(2, 1500.0, 0.02);
    
    Base ba = obj;
    Base * bptr = ptr;
    ba.show();  // Base::show
    bptr->show(); // Derived::show
    
    std::cout<<"Regular Derived: "<< sizeof(obj) <<"\n"; 
    std::cout<<"Pointer Derived: "<<sizeof(*ptr) <<"\n"; 
    std::cout<<"Regular Base: "<<sizeof(ba) <<"\n";
    std::cout<<"Pointer Base: "<<sizeof(*bptr) <<"\n";
}
```

```
Base::show
Derived::show
Regular Derived: 32
Pointer Derived: 32
Regular Base: 24
Pointer Base: 24
```

Up cast on regular stack object  copied only `Base` object, even with vtable only calls base class `show` because dervived object were not copied.