# Base Constructor Initialization — Initializer List Requirement

## The Problem — Base Has No Default Constructor

When a derived object is constructed, the base portion must be initialized first. If the derived constructor does not explicitly say how to initialize the base, the compiler calls the base's **default constructor** automatically.

This fails when the base has no default constructor:

```cpp
class BankAccount {
    int    accountId;
    double balance;
public:
    BankAccount(int id, double bal)   // parameterized only — no default
        : accountId(id), balance(bal) {}
};

class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(int id, double bal, double rate)
        : interestRate(rate)           // forgot to initialize base
    {}
    // COMPILE ERROR: no default constructor for BankAccount
    // compiler tried BankAccount() — does not exist
};
```

The base constructor must be called explicitly — in the initializer list.

## The Solution — Call Base Constructor in Initializer List

```cpp
class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(int id, double bal, double rate)
        : BankAccount(id, bal),   // explicitly calls base constructor
          interestRate(rate)
    {}
};

SavingsAccount sa(1, 1000.0, 0.05);
// BankAccount(1, 1000.0) runs first
// interestRate = 0.05 initialized after
// SavingsAccount body runs last
```

The base class name is used as the function name in the initializer list. Arguments passed to it are forwarded to the matching base constructor.

## What Happens if Base Has a Default Constructor

If the base has a default constructor, omitting the base in the initializer list is valid — the default constructor runs silently:

```cpp
class BankAccount {
    int    accountId;
    double balance;
public:
    BankAccount() : accountId(0), balance(0.0) {}          // default
    BankAccount(int id, double bal)
        : accountId(id), balance(bal) {}                   // parameterized
};

class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(double rate)
        : interestRate(rate)   // BankAccount() called automatically
    {}
};
```

This compiles — but it may not be the intended behavior. The base is initialized to 0/0.0 when specific values were likely needed.

## Choosing Which Base Constructor to Call

Any constructor of the base can be called from the initializer list. The compiler selects the matching overload based on the arguments:

```cpp
class BankAccount {
public:
    BankAccount()                   { std::cout << "default\n"; }
    BankAccount(int id)             { std::cout << "int\n"; }
    BankAccount(int id, double bal) { std::cout << "int,double\n"; }
};

class SavingsAccount : public BankAccount {
public:
    SavingsAccount() : BankAccount()        {}   // calls default
    SavingsAccount(int id) : BankAccount(id) {}  // calls int version
    SavingsAccount(int id, double bal, double rate)
        : BankAccount(id, bal) {}                // calls int,double version
};
```

## Multiple Levels — Each Level Calls Its Direct Base

Each constructor calls only its **direct base** — not the grandparent. The chain propagates automatically:

```cpp
class A {
    int x;
public:
    A(int val) : x(val) {
        std::cout << "A(" << val << ")\n";
    }
};

class B : public A {
    int y;
public:
    B(int a, int b) : A(a), y(b) {      // B calls A directly
        std::cout << "B(" << b << ")\n";
    }
};

class C : public B {
    int z;
public:
    C(int a, int b, int c) : B(a, b), z(c) {  // C calls B directly
        std::cout << "C(" << c << ")\n";       // B calls A internally
    }
};

C obj(1, 2, 3);
```
`C` does not call `A` directly. `C` calls `B`, which calls `A`. Each level is responsible only for its direct base.

```cpp
class C : public B {
    int z;
public:
    C(int a, int b, int c) : B(a, b), A(a), z(c) {  // C calls B directly
        std::cout << "C(" << c << ")\n";       // B calls A internally
    }
};

main.cpp: In constructor ‘C::C(int, int, int)’:
main.cpp:30:39: error: type ‘A’ is not a direct base of ‘C’
   30 |     C(int a, int b, int c) : B(a, b), A(a), z(c) {  // C calls B directly
      |                                       ^
```

## Multiple Inheritance — Each Base Called Separately

When a class has multiple bases, each is initialized separately in the initializer list:

```cpp
class Engine {
    int horsepower;
public:
    Engine(int hp) : horsepower(hp) {
        std::cout << "Engine(" << hp << ")\n";
    }
};

class Chassis {
    std::string material;
public:
    Chassis(std::string m) : material(m) {
        std::cout << "Chassis(" << m << ")\n";
    }
};

class Car : public Engine, public Chassis {
    std::string color;
public:
    Car(int hp, std::string mat, std::string col)
        : Engine(hp),        // initialize first base
          Chassis(mat),      // initialize second base
          color(col)         // initialize own member
    {
        std::cout << "Car(" << col << ")\n";
    }
};

Car c(200, "steel", "red");
```

## The Initializer List Order Warning

Construction follows declaration order, not initializer list order. This applies to base classes too:

```cpp
class Car : public Engine, public Chassis {
public:
    Car(int hp, std::string mat)
        : Chassis(mat), Engine(hp)   // Chassis listed first here
    {}
    // BUT Engine still constructs first — it is first in the declaration
    // g++ warns: -Wreorder
};
```

Always write the initializer list in the same order as the class declaration to avoid confusion and compiler warnings.
