# Basic Syntax: Base and Derived Class
## The Inheritance Syntax
A derived class is declared by placing a colon after the class name, followed by an access specifier and the base class name:
```cpp
class Base {
    // base class members
};

class Derived : public Base {
    // derived class members
};
```

The colon `:` means "inherits from." 

The `public` before `Base` is the **inheritance access specifier** — it controls how the inherited members are visible in the derived class. The three options (`public`, `protected`, `private`). For now, `public` inheritance is used throughout — it is the most common form and preserves the IS-A relationship.

## What the Derived Class Receives

Every non-private member of the base class becomes part of the derived class. The derived class can use them as if it had declared them itself.

```cpp
class BankAccount {
    int    accountId;     // private — NOT directly accessible in derived
    double balance;       // private — NOT directly accessible in derived

public:
    BankAccount(int id, double bal)
        : accountId(id), balance(bal) {}

    void   deposit(double amount)  { balance += amount; }
    void   withdraw(double amount) { balance -= amount; }
    double getBalance() const      { return balance; }
    int    getId()      const      { return accountId; }
};

class SavingsAccount : public BankAccount {
    double interestRate;   // own member — specific to SavingsAccount

public:
    SavingsAccount(int id, double bal, double rate)
        : BankAccount(id, bal),   // base constructor
          interestRate(rate)
    {}

    void applyInterest() {
        deposit(getBalance() * interestRate);   // uses inherited functions
    }
};
```
`SavingsAccount` never declared `deposit`, `withdraw`, `getBalance`, or `getId`. It received them from `BankAccount` through inheritance.

```cpp
SavingsAccount sa(1, 1000.0, 0.05);

sa.deposit(200.0);        // VALID: inherited from BankAccount
sa.withdraw(50.0);        // VALID: inherited from BankAccount
sa.getBalance();          // VALID: inherited from BankAccount
sa.applyInterest();       // VALID: declared in SavingsAccount itself
```
## What the Derived Class Does NOT Receive

Not everything transfers through inheritance:

| Not inherited | Reason |
|---|---|
| Private members of the base | Private means accessible only within the base class itself — derived class cannot access them directly |
| Constructors | Each class constructs its own portion — the derived class must call the base constructor explicitly |
| Destructor | Each class destroys its own portion — called automatically in order |
| Copy constructor | Not automatically inherited |
| Copy assignment operator | Not automatically inherited |
| `friend` declarations | Friendship is not transitive — a friend of the base is not a friend of the derived |

**Private members — the key distinction:**

Private members of the base class **do exist** inside the derived object in memory. They are part of the object's layout. But the derived class code cannot name them directly:

```cpp
class SavingsAccount : public BankAccount {
    void example() {
        balance = 100;      // COMPILE ERROR: balance is private in BankAccount
        deposit(100);       // VALID: deposit() is public in BankAccount
        getBalance();       // VALID: getBalance() is public in BankAccount
    }
};
```

`balance` exists in every `SavingsAccount` object. The derived class just cannot access it by name — it must go through the public interface.

## Adding New Members in the Derived Class

The derived class can declare its own members — data and functions — on top of everything it inherits. These are unique to the derived class and do not exist in the base:

```cpp
class SavingsAccount : public BankAccount {
    double interestRate;        // own data member
    int    withdrawalCount;     // own data member

public:
    SavingsAccount(int id, double bal, double rate)
        : BankAccount(id, bal), interestRate(rate), withdrawalCount(0)
    {}

    void applyInterest() {              // own function
        deposit(getBalance() * interestRate);
    }

    void withdraw(double amount) {      // own function — same name as base
        withdrawalCount++;              // track withdrawals
        BankAccount::withdraw(amount);  // call base version 
    }

    int getWithdrawalCount() const { return withdrawalCount; }   // own function
};
```

The derived class is a strict superset of the base class — it has everything the base has, plus whatever it adds.

## Multiple Levels of Inheritance

A derived class can itself be a base class for another class. There is no limit to the depth of the chain:

```cpp
class BankAccount {                                     // level 1
    double balance;
public:
    BankAccount(double b) : balance(b) {}
    void deposit(double amount) { balance += amount; }
    double getBalance() const   { return balance; }
};

class SavingsAccount : public BankAccount {             // level 2
    double interestRate;
public:
    SavingsAccount(double bal, double rate)
        : BankAccount(bal), interestRate(rate) {}

    void applyInterest() {
        deposit(getBalance() * interestRate);
    }
};

class PremiumSavingsAccount : public SavingsAccount {   // level 3
    int bonusPoints;
public:
    PremiumSavingsAccount(double bal, double rate)
        : SavingsAccount(bal, rate), bonusPoints(0) {}

    void addBonus(int points) { bonusPoints += points; }
    int  getBonus()     const { return bonusPoints; }
};
```

`PremiumSavingsAccount` receives:
- From `BankAccount`: `deposit`, `getBalance`
- From `SavingsAccount`: `applyInterest`
- Its own: `addBonus`, `getBonus`, `bonusPoints`

```cpp
PremiumSavingsAccount psa(1000.0, 0.05);
psa.deposit(200.0);       // from BankAccount (two levels up)
psa.applyInterest();      // from SavingsAccount (one level up)
psa.addBonus(100);        // own
```

## `struct` and Inheritance

`struct` participates in inheritance exactly as `class` does, with one difference: the default inheritance access specifier for `struct` is `public`, and for `class` it is `private`.

```cpp
struct Base {
    int x;
};

struct Derived : Base {      // implicitly public inheritance
    int y;
};

// Equivalent to:
struct Derived : public Base {
    int y;
};
```

```cpp
class Derived : Base {       // implicitly PRIVATE inheritance for class
    int y;
};
```

Base is struct and Derived is class, the inheritance is determined by the derived class, derived class is a class so by default inheritance access specifiers is is private. 

This is the only syntactic difference between `struct` and `class` in the context of inheritance — consistent with the general rule that `struct` defaults to `public` and `class` defaults to `private`.


## Terminology

| Term | Meaning |
|---|---|
| Base class | The class being inherited from (also called parent class or superclass) |
| Derived class | The class that inherits (also called child class or subclass) |
| Inheritance chain | A sequence of base → derived → more derived classes |
| Direct base | The class immediately inherited from |
| Indirect base | A class further up the chain |

For `PremiumSavingsAccount`:
- Direct base: `SavingsAccount`
- Indirect base: `BankAccount`