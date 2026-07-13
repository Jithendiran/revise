## Access Specifiers

### The Problem Access Specifiers Solve
A `struct` in C, and an unrestricted class member, is directly accessible from any code that has the object. Nothing prevents external code from setting a `BankAccount` object's `balance` to an invalid or dangerous value.

```cpp
struct BankAccount {
    double balance;
};

BankAccount acc;
acc.balance = -999999.0;   // no protection — any code can corrupt the object
```

OOP was designed to bind data and the functions that operate on it together, and to prevent external code from bypassing the rules those functions enforce. Access specifiers are the language mechanism that makes this enforcement possible. Without them, encapsulation is only a naming convention — with them, it is a compiler-enforced rule.

### The Three Access Specifiers
C++ provides three keywords that control which code can access a class member:
* `public`: Any code that has the object
* `private`: Only member functions of the same class
* `protected`: Member functions of the same class, and member functions of derived classes

```cpp
class BankAccount {
private:
    double balance;          // only BankAccount's own functions can touch this

public:
    void deposit(double amount) {
        balance += amount;   // VALID: this is a member function of BankAccount
    }

    double getBalance() const {
        return balance;      // VALID: same class
    }
};

BankAccount acc;
acc.deposit(100.0);          // VALID: deposit is public
// acc.balance = 500.0;      // COMPILE ERROR: balance is private
```

###  How Access Control Is Enforced
Access control is a compile-time check, not a runtime check. The compiler examines every member access expression and verifies that the code performing the access is permitted to touch that member. There is no runtime cost — no flags are checked, no permissions are looked up while the program executes. Once code compiles, the access rules have already been fully validated and no trace of them remains in the generated machine instructions.
This is worth stating precisely because it explains an important fact: `private` is not a security mechanism against a determined attacker who can read raw memory or use pointer arithmetic to bypass the type system. It is a discipline enforcement mechanism for correct, well-behaved code — it prevents accidental misuse, not deliberate circumvention at the machine level.

### Default Access — class vs struct
The default access level applied to members with no explicit specifier.
```cpp
class Example {
    int x;   // private by default — class defaults to private
};

struct Example2 {
    int x;   // public by default — struct defaults to public
};
```
This default applies only until the next access specifier label appears. Multiple labels can appear in any order, any number of times, inside the same class body.

```cpp
class Mixed {
    int a;          // private (class default)
public:
    int b;          // public
    int b1;
private:
    int c;          // private (explicit)
public:
    void show() {}  // public
};
```

### Accessor and Mutator Functions (Getters and Setters)
Since private members cannot be read or written directly from outside the class, controlled access is provided through public member functions — conventionally called accessors (read access) and mutators (write access).
```cpp
class BankAccount {
private:
    double balance;

public:
    // Accessor ("getter") — read-only access
    double getBalance() const {
        return balance;
    }

    // Mutator ("setter") — controlled write access
    void setBalance(double newBalance) {
        if (newBalance < 0) {
            throw std::invalid_argument("Balance cannot be negative");
        }
        balance = newBalance;
    }
};
```

A raw public member allows any value, including invalid ones, with zero validation. A mutator function is a *checkpoint* — every write request passes through validation logic before the member is actually modified. This is the entire purpose of encapsulation: not to prevent access, but to force access through a controlled gateway, exactly as covered in the earlier encapsulation material.

## Access Is Per-Class, Not Per-Object
`private` restricts access based on which **class** the accessing code belongs to — not which **object** it belongs to. A member function of `BankAccount` can access the private members of **any** `BankAccount` object, not only the object it was called on.

```cpp
class BankAccount {
private:
    double balance;

public:
    BankAccount(double b) : balance(b) {}

    bool hasMoreThan(const BankAccount& other) const {
        return balance > other.balance;   // VALID: accessing other.balance
                                          // 'other' is a different object,
                                          // but same class — access is allowed
    }
};

BankAccount a(100.0);
BankAccount b(50.0);
a.hasMoreThan(b);   // a's member function reads b's private balance directly
```

This is why the copy constructor and copy assignment operator can directly copy the private members of the source object — both objects belong to the same class.

### `protected` — Access for Derived Classes

#### What `protected` Does
`protected` sits between `private` and `public`. A `protected` member is:

- **Not accessible** from arbitrary external code (same restriction as `private`)
- **Accessible** from member functions of the same class (same as `private`)
- **Accessible** from member functions of derived classes (not available to`private`)

```cpp
class Vehicle {
private:
    int engineSerialNumber;   // only Vehicle's own functions can access this

protected:
    int speed;                // Vehicle's functions AND derived class functions
                              // can access this

public:
    void accelerate(int amount) {
        speed += amount;      // VALID: same class
    }
};

class Car : public Vehicle {
public:
    void turboBoost() {
        speed *= 2;           // VALID: Car is a derived class — protected access
        // engineSerialNumber = 0;  // COMPILE ERROR: private, not accessible
    }
};

Vehicle v;
// v.speed = 100;   // COMPILE ERROR: protected is not public
```

### Friend as an Access Exception 
`private` and `protected` restrict access to member functions of the class. One documented exception exists: a class can explicitly grant access to a specific outside function or class using the friend keyword.

```cpp
class BankAccount {
private:
    double balance;
    friend void auditAccount(const BankAccount& acc);   // grants access
};

void auditAccount(const BankAccount& acc) {
    std::cout << acc.balance;   // VALID: auditAccount is a friend
}
```