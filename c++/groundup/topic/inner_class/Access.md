## Access Specifiers on Inner Classes
The access specifier controlling the inner class declaration determines who can **name** the inner class from outside. It does not restrict what the inner class itself can access.
```cpp
class Container {
private:
    class InternalNode {         // private — cannot be named outside Container
        int data;
    public:
        InternalNode(int d) : data(d) {}
    };

protected:
    class BaseIterator {         // protected — accessible in derived classes
        InternalNode* ptr;
    public:
        BaseIterator(InternalNode* p) : ptr(p) {}
    };

public:
    class Iterator {             // public — accessible everywhere
        InternalNode* ptr;
    public:
        Iterator(InternalNode* p) : ptr(p) {}
        int  operator*()  const { return ptr->data; }
    };

    Iterator begin();
    Iterator end();
};

Container c;
Container::Iterator   it = c.begin();   // VALID: Iterator is public
// Container::BaseIterator bi;          // only valid in derived classes
// Container::InternalNode n(1);        // COMPILE ERROR: InternalNode is private
```

The access specifier controls who can name the type. It does not prevent the outer class from using `InternalNode` internally — the outer class always has access to its own private members including private inner classes.

## Inner Class `static` Members
An inner class can have `static` data members and `static` member functions of its own. These follow the same rules as all static members. The fully qualified name is `Outer::Inner::staticMember`.

```cpp
class BankAccount {
public:
    class InterestPolicy {
        static double defaultRate;   // static member of inner class
        double        rate;

    public:
        InterestPolicy(double r) : rate(r) {}

        static double getDefaultRate()  { return defaultRate; }
        double        getRate()   const { return rate; }
    };
};

double BankAccount::InterestPolicy::defaultRate = 0.05;

double r = BankAccount::InterestPolicy::getDefaultRate();   // 0.05
BankAccount::InterestPolicy policy(0.07);
double pr = policy.getRate();                               // 0.07
```

### Static class
An inner class cannot be declared static in c++. The compiler is explicit: a storage class (static) can only be specified for objects and functions — not for type declarations.

The "static inner class" concept comes from Java, where it has a specific meaning. To understand why C++ has no such thing, both languages must be compared
*  In Java, a non-static inner class always carries a hidden reference to the enclosing outer object. A static inner class removes that hidden reference.
* Every C++ nested class is already what Java calls a "static nested class." There is no hidden outer object reference. There is nothing to remove with static. The keyword has no meaning on a type declaration.

## Anonymous struct and union (Nested)
An unnamed `struct` or `union` defined as a member of a class. Members of the anonymous type are directly accessible as if they were members of the outer class.

```cpp
class Pixel {
public:
    union {             // anonymous union — no name
        uint32_t rgba;
        struct {        // anonymous struct inside union
            uint8_t r, g, b, a;
        };
    };
};

Pixel p;
p.rgba = 0xFF0000FF;   // access union member directly
p.r = 255;             // access struct member directly — no intermediate name
p.g = 0;
```