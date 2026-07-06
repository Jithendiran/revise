## Member Functions

### What a Member Function Is
A member function is a function declared inside a class body. It differs from a standalone function in one essential way: it receives an implicit `this` pointer, giving it access to the data members of the specific object it was called on.

```cpp
class BankAccount {
private:
    double balance;

public:
    void deposit(double amount) {   // member function
        balance += amount;
    }
};
```
`deposit` is not a free-floating function. It exists only in association with `BankAccount` objects, and every call to it operates on one particular object's `balance`.

### Declaration vs Definition — The Core Distinction
This distinction is fundamental to C++ and applies to every function, member or not. It must be precise before anything else in this block makes sense.
* A declaration tells the compiler that a function exists — its name, parameter types, and return type. It does not provide the function body.
* A definition provides the actual executable body of the function.

```cpp
void deposit(double amount);          // declaration — no body, ends with ;

void deposit(double amount) {         // definition — has a body
    balance += amount;
}
```

A function can be declared many times across a program (as long as each declaration agrees), but it must be defined exactly once across the entire program. This restriction is the One Definition Rule (ODR).

### In-Class Definition vs Out-of-Class Definition
A member function's body can be written in one of two places: directly inside the class body, or outside it using the scope resolution operator `::`.

#### In-Class Definition
```cpp
class BankAccount {
private:
    double balance;

public:
    void deposit(double amount) {   // defined directly inside the class
                                    // implicitly inline — safe to appear in multiple translation units
        balance += amount;
    }
};
```

#### Out-of-Class Definition
```cpp
class BankAccount {
private:
    double balance;

public:
    void deposit(double amount);    // declaration only — inside the class
};

// Definition — outside the class, using ClassName::functionName
void BankAccount::deposit(double amount) { // NOT implicitly inline, no multiple definitions are allowed
    balance += amount;
}
```
The `::` operator here is the scope resolution operator. It tells the compiler: "this deposit function belongs to the `BankAccount` class, not to the global namespace." Without `BankAccount::`, the compiler would treat deposit as an unrelated, standalone global function with no access to balance.

**If class has both in-class definition and an out-of-class definition for the exact same function, the compiler will throw a compilation error.**

### Function Overloading
A class can declare multiple member functions with the same name, provided their parameter lists differ in type or count. This is function overloading, and it applies to member functions exactly as it does to standalone functions.

```cpp
class BankAccount {
    double balance;
public:
    void deposit(double amount) {
        balance += amount;
    }

    void deposit(double amount, const std::string& memo) {   // overload — different parameter list
        balance += amount;
        std::cout << "Memo: " << memo << "\n";
    }
};

BankAccount acc;
acc.deposit(100.0);                 // calls the first version
acc.deposit(100.0, "Paycheck");     // calls the second version
```
The compiler selects the correct overload at compile time based on the number and type of arguments supplied at the call site. This selection process is called overload resolution.


### const Member Functions 
```cpp
class BankAccount {
    double balance;
public:
    double getBalance() const {   // const member function
        return balance;
    }
};
```

The `const` after the parameter list is part of the function's declaration — a `const` and non-const version of a function with otherwise identical signatures are treated by the compiler as two distinct overloads, selected based on whether the calling object itself is const.

```cpp
class Container {
    int data[10];
public:
    int& get(int i)             { return data[i]; }   // non-const version
    const int& get(int i) const { return data[i]; }   // const version — separate overload
};

Container c;
const Container cc{};
c.get(0)  = 5;    // calls non-const version — returns modifiable reference
cc.get(0);        // calls const version — returns read-only reference
```