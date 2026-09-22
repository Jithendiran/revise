# Access Under Inheritance — The 3×3 Matrix
## What Changes Under Inheritance
The derived class receives all public and protected members of the base. The **inheritance access specifier** — the keyword written between `:` and the base class name — controls how those inherited members appear inside the derived class and to the outside world.

```cpp
class Derived : public    Base { };   // public inheritance
class Derived : protected Base { };   // protected inheritance
class Derived : private   Base { };   // private inheritance
```

This specifier does not change what the base class contains. It changes the **maximum visibility** of inherited members as seen through the derived class.

## The Rule — Access Is Capped

The inheritance specifier acts as a ceiling. Each inherited member's access in the derived class is the **more restrictive** of:
- its original access in the base class
- the inheritance specifier

```
effective access = min(original access, inheritance specifier)

where: public > protected > private (public is least restrictive)
```
## The 3×3 Matrix

Rows = original access in base class
Columns = inheritance specifier
Cell = resulting access in derived class

| Base member | `public` inheritance | `protected` inheritance | `private` inheritance |
|---|---|---|---|
| `public` | `public` | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private` | inaccessible | inaccessible | inaccessible |

**Private members are always inaccessible in the derived class**, regardless of the inheritance specifier. They exist in memory but cannot be named in derived class code.

### Public Inheritance — IS-A

```cpp
class BankAccount {
public:
    void deposit(double amount);      // public
protected:
    double balance;                   // protected
private:
    int accountId;                    // private
};

class SavingsAccount : public BankAccount {
    // deposit   → public    (public  in base, public  ceiling = public)
    // balance   → protected (protected in base, public ceiling = protected)
    // accountId → inaccessible (private in base — always inaccessible)
};

SavingsAccount sa;
sa.deposit(100);    // VALID:   public in derived
// sa.balance;      // INVALID: protected — not accessible from outside
// sa.accountId;    // INVALID: inaccessible
```

Public inheritance preserves the original access of all non-private members. The IS-A relationship holds — a `SavingsAccount` object can be used wherever a `BankAccount` is expected through a pointer or reference:

```cpp
BankAccount* bp = new SavingsAccount();   // VALID: IS-A holds
```
### Protected Inheritance — Rarely Used

```cpp
class SavingsAccount : protected BankAccount {
    // deposit   → protected (public  in base, protected ceiling = protected)
    // balance   → protected (protected in base, protected ceiling = protected)
    // accountId → inaccessible
};

SavingsAccount sa;
// sa.deposit(100);   // INVALID: deposit is now protected — not accessible outside
```

Public members of the base become protected in the derived class. They are accessible inside `SavingsAccount`'s member functions and inside any class that derives from `SavingsAccount` — but not from outside code.

What can and cannot access it?
* From outside (like `main()`):
    A derived class object (`sa`) cannot access protected members. Calling  `sa.deposit(100)` from `main()` will result in a compiler error saying the member is protected/inaccessible.
*  From inside SavingsAccount:
    Member functions or friend functions inside `SavingsAccount` can still call `deposit()` because protected members are accessible to derived classes.
    ```cpp
    #include <iostream>

    class BankAccount {
    public:
        void deposit(double amount) {
            balance += amount;
            std::cout << "Successfully deposited: " << amount << std::endl;
        }
    protected:
        double balance;                   // protected
    private:
        int accountId;                    // private
    };

    class SavingsAccount : protected BankAccount {

        public:
        void test(){
            BankAccount::deposit(100l); // accessable
        }
    };


    int main()
    {
        SavingsAccount sa;
        // sa.deposit(200); // not accessable
        sa.test();    
        return 0;
    }
    ```
**The IS-A relationship does NOT hold with protected inheritance:**

```cpp
BankAccount* bp = new SavingsAccount();   // COMPILE ERROR
BankAccount ba = sa; // COMPILE ERROR
// SavingsAccount is not publicly a BankAccount
// the relationship is hidden from outside code
```
In C++, converting a pointer from a derived class to a base class (`SavingsAccount*` to `BankAccount*`) is only allowed if the inheritance is public.

When using protected BankAccount, the programmer tells the compiler as : "Only SavingsAccount and its own children should know that it inherits from BankAccount. Keep this relationship hidden from `main()` and the rest of the program". `BankAccount` can be accessed inside `SavingsAccount` class, not using the object

Because `main()` is not allowed to see that `SavingsAccount` is a type of BankAccount, the compiler blocks the pointer conversion and throws that error.

### Private Inheritance — HAS-A via Inheritance

```cpp
class SavingsAccount : private BankAccount {
    // deposit   → private (public  in base, private ceiling = private)
    // balance   → private (protected in base, private ceiling = private)
    // accountId → inaccessible
};

SavingsAccount sa;
// sa.deposit(100);   // INVALID: deposit is private — not accessible outside
```

All inherited members become private in the derived class. They are only accessible inside `SavingsAccount`'s own member functions — not from outside, not from further derived classes.

```cpp
#include <iostream>

class BankAccount {
public:
	void deposit(double amount) {
		balance += amount;
		std::cout << "Successfully deposited: " << amount << std::endl;
	}
protected:
	double balance;                   // protected
private:
	int accountId;                    // private
};

class SavingsAccount : private BankAccount {

	public:
	 void test(){
	     BankAccount::deposit(100l); // It can access
	 }
};


int main()
{
	SavingsAccount sa;

	sa.test();    
	return 0;
}
```

If `BankAccount::deposit(100l);` accessable inside `SavingsAccount` class then what is the difference between protected and private?
* It worked because private inheritance only blocks access from outside the class, not from inside it.
* The difference only matters when create a sub-derived class (a child of SavingsAccount).
    ```cpp
    class BankAccount {
    public:
        void deposit(double amount) {}
    };

    // PRIVATE inheritance
    class SavingsAccount : private BankAccount {
    public:
        void test() {
            deposit(100); // VALID: Inside SavingsAccount, it can access it.
        }
    };

    // Trying to inherit from SavingsAccount
    class SuperSavings : public SavingsAccount {
    public:
        void test2() {
            // deposit(100); // ERROR! Because SavingsAccount used PRIVATE inheritance, 
                            // deposit is now private to SavingsAccount and hidden from SuperSavings.
        }
    };
    ```
* When inherited as protected, All public and protected members of the base class become protected members inside the derived class.
* When inherited as private, All public and protected members of the base class become private members inside the derived class.
**The IS-A relationship does NOT hold:**

```cpp
BankAccount* bp = new SavingsAccount();   // COMPILE ERROR
``` 

**Why private inheritance exists — HAS-A via inheritance:**

Private inheritance is a way to use the base class's implementation internally without exposing it. It is similar to composition (member variable) but gives access to protected members and allows overriding virtual functions. In practice, composition is preferred. Private inheritance is rare.

> What Is Composition
>
> Composition means having an object of another class as a data member instead of inheriting from it.
    ```cpp
    // Inheritance (IS-A)
    class SavingsAccount : private BankAccount { };

    // Composition (HAS-A) — BankAccount is a MEMBER
    class SavingsAccount {
        BankAccount account;   // composition — owns a BankAccount internally
    };
    ```
> Both give `SavingsAccount` access to `BankAccount`'s functionality. The difference:
> * Private inheritance — SavingsAccount IS secretly a BankAccount
>   - Can access protected members of BankAccount directly
>   - Can override BankAccount's virtual functions
> *  Composition — SavingsAccount HAS a BankAccount
>   - Can only access BankAccount's PUBLIC members
>   - Cannot override BankAccount's virtual functions
>   - Cannot access balance (protected) directly
>
> Private inheritance only wins over composition in two narrow situations:
> * need to access the base's `protected` members
> * need to override the base's virtual functions
> In all other cases composition is cleaner — it makes the HAS-A relationship explicit and does not carry the complications of inheritance.

## How Inheritance Propagates Further Down the Chain
When a third class inherits from the derived class, the access seen in the derived class is what the third class works with — not the original base class access.

```cpp
class BankAccount {
public:    void deposit();
protected: double balance;
private:   int accountId;
};

class SavingsAccount : protected BankAccount {
    // deposit → protected
    // balance → protected
};

class PremiumSavings : public SavingsAccount {
    // deposit → protected  (was protected in SavingsAccount, public ceiling = protected)
    // balance → protected  (same)
    // accountId → inaccessible
    void example() {
        deposit(100);   // VALID: protected is accessible in member functions
        balance = 0;    // VALID: protected is accessible in member functions
    }
};

PremiumSavings ps;
// ps.deposit();   // INVALID: deposit is protected — not accessible from outside
```

## `using` to Restore Access

Inside a derived class, a `using` declaration can restore a specific inherited member to a higher access level — up to but not exceeding its original access in the base.

```cpp
#include <iostream>

class BankAccount {
public:
	void deposit(double amount) {
		balance += amount;
		std::cout << "Successfully deposited: " << amount << std::endl;
	}
protected:
	double balance;                   // protected
private:
	int accountId;                    // private
};

// PRIVATE inheritance
class SavingsAccount : private BankAccount {
public:
	using BankAccount::deposit;
};

class SuperSavings : private SavingsAccount {
public:
	using BankAccount::deposit;
};
int main()
{
	SavingsAccount sa;
	sa.deposit(100);

    SuperSavings   su;
    // su.deposit(100); The compilation error occurs because BankAccount is completely private to SavingsAccount.
    // When SuperSavings inherits privately from SavingsAccount, it cannot reach past SavingsAccount to access BankAccount directly. The base class relationship is hidden.
	return 0;
}
```

`using` cannot promote beyond the original access — a private base member cannot be made public through `using`.

## Default Inheritance Specifier

When no specifier is written, the default depends on the keyword used:

```cpp
class Derived : Base { };     // private inheritance (class default)
struct Derived : Base { };    // public  inheritance (struct default)
```

Same rule as member access defaults: `class` defaults to private, `struct` defaults to public.