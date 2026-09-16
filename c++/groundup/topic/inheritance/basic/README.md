#  Why Inheritance Exists
## The Problem — Duplicated Structure Across Related Types

Consider a system that models different types of bank accounts. A savings account and a checking account share a large amount of common structure:

```cpp
class SavingsAccount {
    int    accountId;
    double balance;
    std::string holderName;

public:
    SavingsAccount(int id, double bal, std::string name)
        : accountId(id), balance(bal), holderName(name) {}

    void deposit(double amount)  { balance += amount; }
    void withdraw(double amount) { balance -= amount; }
    double getBalance()    const { return balance; }
    std::string getName()  const { return holderName; }
    void printStatement()  const {
        std::cout << holderName << ": " << balance << "\n";
    }

    // Savings-specific
    void applyInterest(double rate) { balance *= (1 + rate); }
};

class CheckingAccount {
    int    accountId;
    double balance;
    std::string holderName;

public:
    CheckingAccount(int id, double bal, std::string name)
        : accountId(id), balance(bal), holderName(name) {}

    void deposit(double amount)  { balance += amount; }
    void withdraw(double amount) { balance -= amount; }
    double getBalance()    const { return balance; }
    std::string getName()  const { return holderName; }
    void printStatement()  const {
        std::cout << holderName << ": " << balance << "\n";
    }

    // Checking-specific
    void issueCheck(double amount) { balance -= amount; }
};
```
Every common member — `accountId`, `balance`, `holderName`, `deposit`, `withdraw`, `getBalance`, `getName`, `printStatement` — is duplicated exactly. Adding a `LoanAccount` would duplicate them a third time.

### Three concrete problems this creates:

* **Problem 1 — Maintenance burden.** A bug fix or change in `deposit` must be applied to every class separately. Miss one and the classes diverge.

* **Problem 2 — No common interface.** A function that processes any account cannot exist:

    ```cpp
    void processAccount(????? account) {
        account.deposit(100);
        account.printStatement();
    }
    // What type does the parameter have?
    // SavingsAccount and CheckingAccount are unrelated types
    // No single type covers both
    ```
* **Problem 3 — Scaling fails.** Ten account types means ten copies of the same core code. Real banking systems have dozens of account types.

## IS-A vs HAS-A — The Conceptual Distinction

Before the solution is introduced, a precise test must be established for when inheritance is the right tool.

* **IS-A Relationship**
    A new type is a more specific version of a base type. Every instance of the new type also functions as an instance of the base type.

    * A savings account is a bank account.
    * A checking account is a bank account.
    * A car is a vehicle.
    * A dog is an animal.
  
    Every feature of the general item automatically applies to the specific item. 

* **HAS-A Relationship**
    A type contains or uses another type as a component. The contained type is a separate entity rather than a subcategory.

    * A bank account has an account holder name.
    * A car has an engine.
    * A dog has a name.

    An engine is a separate component inside a car, not a subcategory of a car. Components use storage variables instead of inheritance. 


HAS-A is expressed through member variables. IS-A is expressed through inheritance.

**The test — apply the sentence literally:**
```
"A SavingsAccount is a BankAccount" — makes sense → inheritance
"A BankAccount has a string" — makes sense → member variable
"A string is a BankAccount" — nonsense → never use inheritance here
"An Engine is a Car" — nonsense → member variable, not inheritance
```
If the IS-A sentence sounds wrong, inheritance is the wrong tool. This test prevents the most common misuse of inheritance.

## The Solution — A Common Base Class

The shared structure is extracted into one class. The specific classes derive from it, gaining all the shared members automatically.

```cpp
// All shared structure lives here — defined ONCE
class BankAccount {
    int         accountId;
    double      balance;
    std::string holderName;

public:
    BankAccount(int id, double bal, std::string name)
        : accountId(id), balance(bal), holderName(name) {}

    void deposit(double amount)  { balance += amount; }
    void withdraw(double amount) { balance -= amount; }
    double      getBalance() const { return balance; }
    std::string getName()    const { return holderName; }
    void printStatement()    const {
        std::cout << holderName << ": " << balance << "\n";
    }
};

// SavingsAccount IS-A BankAccount
// Gets everything BankAccount has, adds its own specific behavior
class SavingsAccount : public BankAccount {
public:
    SavingsAccount(int id, double bal, std::string name)
        : BankAccount(id, bal, name) {}

    void applyInterest(double rate) {
        deposit(getBalance() * rate);     // uses inherited deposit and getBalance
    }
};

// CheckingAccount IS-A BankAccount
class CheckingAccount : public BankAccount {
public:
    CheckingAccount(int id, double bal, std::string name)
        : BankAccount(id, bal, name) {}

    void issueCheck(double amount) {
        withdraw(amount);                 // uses inherited withdraw
    }
};
```

**What each derived class automatically receives from BankAccount:**
- All data members: `accountId`, `balance`, `holderName`
- All member functions: `deposit`, `withdraw`, `getBalance`, `getName`,
  `printStatement`

**What each derived class adds:**
- `SavingsAccount`: `applyInterest`
- `CheckingAccount`: `issueCheck`

**What changed in maintenance terms:**
- `deposit` exists in exactly one place — `BankAccount`
- A bug fix in `deposit` fixes it for all account types simultaneously
- Adding `LoanAccount` means writing only the loan-specific behavior

## The Common Interface Problem — Also Solved

Because `SavingsAccount` IS-A `BankAccount` and `CheckingAccount` IS-A `BankAccount`, both can be treated as `BankAccount` through a pointer or reference:

```cpp
void processAccount(BankAccount& account) {
    account.deposit(100.0);
    account.printStatement();
}

SavingsAccount sa(1, 500.0, "Alice");
CheckingAccount ca(2, 300.0, "Bob");

processAccount(sa);   // VALID: SavingsAccount is a BankAccount
processAccount(ca);   // VALID: CheckingAccount is a BankAccount
```

One function handles all account types. This is possible because the IS-A relationship guarantees that a `SavingsAccount` has everything a `BankAccount` has — including the `deposit` and `printStatement` functions `processAccount` calls.

## What Inheritance Is NOT For

**Inheritance is not for code reuse alone.** If the only reason to inherit is to avoid rewriting functions — without a genuine IS-A relationship — the design is wrong.

```cpp
// WRONG — a Stack is not a Vector
class Stack : public std::vector<int> {
public:
    void push_stack(int v) { push_back(v); }
    int  pop_stack()       { int v = back(); pop_back(); return v; }
};
```

`Stack` inherits from `std::vector<int>` only to reuse `push_back` and `back`. But a Stack IS NOT a vector. The result exposes all vector operations (`insert`, `erase`, `operator[]`) on the Stack — operations that should not exist on a Stack. The Stack's invariant (only access the top) is broken.

The correct design uses composition (HAS-A):

```cpp
// CORRECT — Stack HAS-A vector internally
class Stack {
    std::vector<int> storage;   // member variable — HAS-A
public:
    void push(int v)  { storage.push_back(v); }
    int  pop()        { int v = storage.back(); storage.pop_back(); return v; }
    int  top()  const { return storage.back(); }
    bool empty() const { return storage.empty(); }
    // vector's other operations are NOT exposed — invariant preserved
};
```