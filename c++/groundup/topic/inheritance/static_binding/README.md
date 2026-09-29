# Static Binding and the Problem It Creates

## What Binding Means
Binding is the process of connecting a function call in code to the actual machine code that runs. The question is: **when** does the compiler decide which function to call?
* Static binding — decision made at COMPILE TIME
* Dynamic binding — decision made at RUNTIME

## Static Binding — How It Works
Without `virtual`, the compiler decides which function to call based purely on the **static type** of the pointer or reference — the type written in the declaration. It does not matter what the pointer actually points to at runtime.
```cpp
class BankAccount {
public:
    void printStatement() {
        std::cout << "BankAccount::printStatement\n";
    }
};

class SavingsAccount : public BankAccount {
public:
    void printStatement() {
        std::cout << "SavingsAccount::printStatement\n";
    }
};

BankAccount*    bp = new SavingsAccount();
bp->printStatement();
```

* Compiler sees: `bp` is `BankAccount*`
* Compiler decides at compile time: call `BankAccount::printStatement`
* Runtime: does not matter that bp points to `SavingsAccount`
* Output: `BankAccount::printStatement` ← wrong

The pointer `bp` genuinely points to a `SavingsAccount` object. The `SavingsAccount` version exists. But it is never called — the compiler locked in the decision at compile time based on the pointer type.

### The Problem — The Wrong Function Is Always Called

Static binding makes polymorphism impossible. Consider a function that processes any account:

```cpp
void process(BankAccount* account) {
    account->printStatement();   // always calls BankAccount version
                                  // regardless of actual object type
}

SavingsAccount sa(1, 1000.0, 0.05);

process(&sa);   // prints BankAccount::printStatement — WRONG
sa.printStatement(); // prints SavingsAccount::printStatement — CORRECT
```

Every call goes to the base version. The derived class versions are compiled into the binary but never reached through a base pointer.

Direct calls on the object work correctly — the compiler knows the exact type. Through a base pointer, every call goes to the base version — the compiler only sees `BankAccount*`.

### Why Static Binding Exists at All

Static binding is the default because it is fast — zero runtime cost. The compiler resolves the call at compile time and generates a direct jump instruction to the function address. No table lookup, no indirection.

For non-virtual functions that are never meant to be overridden, static binding is correct and efficient. The problem only appears when the goal is to write code that works correctly for any derived type through a base pointer — which is the entire point of polymorphism.

## The Solution — `virtual`
Adding `virtual` to the base class function switches the compiler to dynamic binding for that function. The decision is deferred to runtime using the vtable mechanism.

```cpp
class BankAccount {
public:
    virtual void printStatement() {         // virtual added
        std::cout << "BankAccount::printStatement\n";
    }
};

class SavingsAccount : public BankAccount {
public:
    void printStatement() override {        // overrides the virtual
        std::cout << "SavingsAccount::printStatement\n";
    }
};

class CheckingAccount : public BankAccount {
public:
    void printStatement() override {
        std::cout << "CheckingAccount::printStatement\n";
    }
};

void process(BankAccount* account) {
    account->printStatement();   // now calls correct version at runtime
}

int main() {
    SavingsAccount  sa;
    CheckingAccount ca;

    process(&sa);   // SavingsAccount::printStatement
    process(&ca);   // CheckingAccount::printStatement
}
```

### What `virtual` Changes
* Without `virtual`:
  1. `bp->printStatement()`, compiler sees: `bp` is `BankAccount*`
  2. compiler generates: direct call to `BankAccount::printStatement`, fixed at compile time — never changes
* With `virtual`:
  1. `bp->printStatement()`, compiler sees: `bp` is `BankAccount*`, function is virtual
  2. compiler generates: read vptr from object → look up vtable slot → call, decision made at runtime based on actual object type

The compiler no longer generates a direct call to a known address. It generates instructions to read the vptr, follow it to the vtable, and call whatever function pointer is in the correct slot.

### `virtual` Only Needs to Be Written in the Base

Once a function is declared `virtual` in a base class, it remains virtual in all derived classes automatically — even if `virtual` is not repeated in the derived class.

```cpp
class A {
public:
    virtual void show() { std::cout << "A::show\n"; }
};

class B : public A {
public:
    void show() { std::cout << "B::show\n"; }
    // virtual automatically — no need to write virtual again
};

class C : public B {
public:
    void show() { std::cout << "C::show\n"; }
    // still virtual — propagates all the way down
};

A* ap = new C();
ap->show();   // C::show — virtual dispatch works through all levels
```

Writing `virtual` again in derived classes is allowed but redundant. Writing `override` is preferred — it confirms the function is virtual and overrides a base version.


### `virtual` Does NOT Work on Direct Object Calls

Virtual dispatch only activates through a pointer or reference. When called directly on an object, the type is known at compile time and static binding is used:

```cpp
#include <iostream>

class BankAccount {
public:
    virtual void printStatement() {
        std::cout << "BankAccount::printStatement\n";
    }
};

class SavingsAccount : public BankAccount {
public:
    void printStatement() {
        std::cout << "SavingsAccount::printStatement\n";
    }
};

int main()
{
    BankAccount bp =  SavingsAccount();
    bp.printStatement(); // BankAccount::printStatement
    return 0;
}
```
### Calling the Base Version Explicitly

Even when a function is virtual and overridden, the base version can be called explicitly through scope resolution:

```cpp
class SavingsAccount : public BankAccount {
public:
    void printStatement() override {
        BankAccount::printStatement();   // explicitly calls base version
        std::cout << "interest rate: " << interestRate << "\n";
    }
};
```

This bypasses virtual dispatch entirely. The call goes directly to `BankAccount::printStatement` regardless of the object's actual type. Useful when the derived version wants to extend rather than completely replace the base behavior.

### `virtual` Propagates Through the Vtable

Each class in the hierarchy has its own vtable. When a derived class overrides a virtual function, its vtable slot for that function holds
the derived version's address. When it does not override, its vtable slot holds the same address as the base class slot.

```cpp
class A {
public:
    virtual void one() { std::cout << "A::one\n"; }
    virtual void two() { std::cout << "A::two\n"; }
};

class B : public A {
public:
    void one() override { std::cout << "B::one\n"; }
    // two() not overridden — B's vtable slot for two() = A::two address
};

A* ap = new B();
ap->one();   // B::one   — B overrode slot 0
ap->two();   // A::two   — B did not override slot 1, A's version used
```