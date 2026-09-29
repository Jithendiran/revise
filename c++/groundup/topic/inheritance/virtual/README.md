# vtable and vptr Internals
Already covered the vtable/slicing document:
- vtable is an array of function pointers, one per class, in the read-only code segment
- vptr is a hidden 8-byte pointer at offset 0 of every polymorphic object
- vptr is written by the constructor before the body runs
- In derived construction, base constructor writes Base vtable first, derived constructor overwrites it with Derived vtable
- Object copy resets the vptr to the constructed type's vtable
- Pointer assignment does not touch the vptr
This section covers what was not yet covered: how multiple virtual functions fill the vtable, memory overhead for a virtual call.

## vtable Slot Assignment

Each virtual function gets a fixed slot number. The slot number is assigned by the base class and never changes across the hierarchy. Derived classes use the same slot numbers — they replace the address in the slot, not the slot itself.
```cpp
class BankAccount {
public:
    virtual void printStatement() {}   // slot 0
    virtual void getType()        {}   // slot 1
    virtual void calcInterest()   {}   // slot 2
    virtual ~BankAccount()        {}   // slot 3
};
```

```
BankAccount vtable:
┌──────────┬─────────────────────────────────────────┐
│  slot 0  │  address of BankAccount::printStatement │
│  slot 1  │  address of BankAccount::getType        │
│  slot 2  │  address of BankAccount::calcInterest   │
│  slot 3  │  address of BankAccount::~BankAccount   │
└──────────┴─────────────────────────────────────────┘
```

```cpp
class SavingsAccount : public BankAccount {
public:
    void printStatement() override {}  // slot 0 — replaced
    void calcInterest()   override {}  // slot 2 — replaced
    // getType not overridden          // slot 1 — same as BankAccount
    // ~SavingsAccount                 // slot 3 — replaced (always)
};
```

Slot 1 holds `BankAccount::getType` in the `SavingsAccount` vtable — `SavingsAccount` did not override it so the base address is inherited into the slot.

### New Virtual Function in Derived
When a derived class introduces a new virtual function that does not exist in the base, it gets a new slot added at the end of the derived vtable:
```cpp
class BankAccount {
public:
    virtual void printStatement() {}   // slot 0
    virtual void getType()        {}   // slot 1
    virtual ~BankAccount()        {}   // slot 2
};

class SavingsAccount : public BankAccount {
public:
    void printStatement() override {}  // slot 0 — replaces base
    virtual void applyInterest()   {}  // slot 3 — NEW slot, only in SavingsAccount
};
```
```
BankAccount vtable:
┌──────────┬────────────────────────────────────┐
│  slot 0  │  BankAccount::printStatement       │
│  slot 1  │  BankAccount::getType              │
│  slot 2  │  BankAccount::~BankAccount         │
└──────────┴────────────────────────────────────┘

SavingsAccount vtable:
┌──────────┬────────────────────────────────────┐
│  slot 0  │  SavingsAccount::printStatement    │ ← replaced
│  slot 1  │  BankAccount::getType              │ ← inherited
│  slot 2  │  SavingsAccount::~SavingsAccount   │ ← replaced
│  slot 3  │  SavingsAccount::applyInterest     │ ← NEW — only here
└──────────┴────────────────────────────────────┘
```

**The consequence through a base pointer:**
```cpp
BankAccount* bp = new SavingsAccount();
bp->applyInterest();   // COMPILE ERROR
// BankAccount has no slot 3
// compiler cannot generate the vtable lookup
// applyInterest is invisible through BankAccount*
```
`applyInterest` can only be called through a `SavingsAccount*` or `SavingsAccount&`. It is not part of the base class interface.

**For multilevel inheritance — slot numbers keep extending:**
```cpp
class PremiumSavings : public SavingsAccount {
public:
    void applyInterest() override {}   // slot 3 — replaces SavingsAccount's
    virtual void addBonus()        {}  // slot 4 — NEW, only in PremiumSavings
};
```
```
PremiumSavings vtable:
┌──────────┬────────────────────────────────────┐
│  slot 0  │  SavingsAccount::printStatement    │ ← inherited from SA
│  slot 1  │  BankAccount::getType              │ ← inherited from BA
│  slot 2  │  PremiumSavings::~PremiumSavings   │ ← replaced
│  slot 3  │  PremiumSavings::applyInterest     │ ← replaced
│  slot 4  │  PremiumSavings::addBonus          │ ← NEW
└──────────┴────────────────────────────────────┘
```
Each level can add new slots at the end. Existing slot numbers never change — a base pointer can always find slots 0, 1, 2 reliably.

## Memory Overhead

Adding virtual functions has two costs:

**Per class — one vtable:** sizeof vtable = number of virtual functions × 8 bytes (on 64-bit)
- BankAccount vtable = 4 slots × 8 = 32 bytes
- SavingsAccount vtable = 4 slots × 8 = 32 bytes

The vtable is shared across all instances of a class. One million `BankAccount` objects share one vtable. The vtable cost is per class, not per object.

**Per object — one vptr:** sizeof vptr = 8 bytes (on 64-bit)
- Every polymorphic object pays 8 bytes for the vptr — regardless of how many virtual functions the class has.
- Without virtual: sizeof BankAccount = 16 (int + padding + double)
- With virtual: sizeof BankAccount = 24 (8 vptr + int + padding + double)

Adding more virtual functions does NOT increase object size — the vptr is already there and does not change size. Only the vtable grows, and the vtable is shared.