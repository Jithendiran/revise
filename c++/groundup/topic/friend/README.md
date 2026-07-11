## friend

### The Problem of Data Isolation
In object-oriented programming, encapsulation is a core design principle. Encapsulation mandates that a class hides its internal data (its `private` variables) from the outside world. This restriction prevents external code from accidentally modifying or misinterpreting the internal state of an object.

However, strict encapsulation creates a architectural challenge: occasionally, two distinct, unrelated systems must work together with high efficiency. If System A requires direct access to the `private` data of System B, standard encapsulation rules force the programmer to write public "getter" and "setter" functions. These public functions expose the internal data to all external code, which completely defeats the original purpose of data hiding.

The `friend` keyword solves this specific architectural dilemma. It provides a controlled mechanism to bypass standard access restrictions for highly specific entities without opening the data to the entire program.

`friend` looks like it breaks encapsulation, but used correctly it does not. Encapsulation's purpose is to prevent **arbitrary, uncontrolled** access to implementation details. `friend` grants **specific, named, controlled** access to a **specific entity** that the class designer explicitly chose to trust.


### The Concept of Friend Declarations
Friend is a Compile-Time. A `friend` declaration is an explicit instruction written inside a class definition. This instruction names an external entity and grants that specific entity permission to access the class's `private` and `protected` members.

The external entity can be:
* A standalone function (a global function that does not belong to any class).
* A specific member function belonging to a completely different class.
* An entire external class (which grants all member functions of that external class access).

The `friend` status cannot be claimed from the outside. An external function cannot declare itself a `friend` of a class to steal its data. The permission must be explicitly granted by the class that owns the data. This design ensures that the class retains ultimate control over who can modify its internal state.

```cpp
class SecretHouse {
private:
    int valuables = 5000;

public:
    // The class explicitly grants permission INSIDE its walls
    friend void trustedFriend(SecretHouse& house); 
};

// The external function can now access the data because it was invited
void trustedFriend(SecretHouse& house) {
    house.valuables = 0; // Allowed!
}

// Imagine if C++ allowed this (IT DOES NOT):
friend void strangerFunction(SecretHouse& house) {
    house.valuables = 0; // Trying to steal the data
}
```

### Friend Class
An entire class can be declared a friend. Every member function of the friend class gains access to the private members of the declaring class.

```cpp
class Engine {
private:
    int horsepower;

    friend class Mechanic;   // all Mechanic member functions can access horsepower
};

class Mechanic {
public:
    void inspect(const Engine& e) {
        std::cout << e.horsepower;   // VALID: Mechanic is a friend of Engine
    }

    void tune(Engine& e) {
        e.horsepower += 10;          // VALID: same reason
    }
};
```

Friend class is used more sparingly than friend function. Use it when two classes are tightly coupled and share internal implementation concerns — for example, a container class and its iterator class.


### Four Rules of `friend`

**Rule 1 — Friendship is not inherited.**

If `Derived` inherits from `Base`, and `SomeFunction` is a friend of `Base`, `SomeFunction` does not automatically gain access to the private members of `Derived`.

```cpp
class Base {
private:
    int x;
    friend void helper(Base& b);
};

class Derived : public Base {
private:
    int y;
};

void helper(Base& b) {
    b.x = 10;    // VALID: friend of Base
    // b.y = 20; // COMPILE ERROR: y is Derived's private — not accessible
                 // even when b actually refers to a Derived object
}
```

**Rule 2 — Friendship is not transitive.**
If `A` is a friend of `B`, and `B` is a friend of `C`, `A` is not automatically a friend of `C`.

```cpp
class C {
private:
    int data;
    friend class B;
};

class B {
    friend class A;
public:
    void accessC(C& c) {
        c.data = 5;   // VALID: B is friend of C
    }
};

class A {
public:
    void accessC(C& c) {
        // c.data = 5;   // COMPILE ERROR: A is not a friend of C
    }
};
```

// c -> b, b -> a

**Rule 3 — Friendship is not symmetric.**
If `A` declares `B` as a friend, `A` does not automatically become a friend of `B`.

```cpp
class B;

class A {
private:
    int x;
    friend class B;   // B can access A's private members
};

class B {
private:
    int y;
    // A is NOT automatically a friend of B
};

// For A to access B's private members, B must also declare A as a friend.
```

**Rule 4 — The class granting friendship controls it, not the friend.**

The `friend` declaration is always inside the class whose private members are being opened. The entity being named as a friend cannot grant itself access, only the class being accessed can grant it.

[Hacks](./hacks.md)