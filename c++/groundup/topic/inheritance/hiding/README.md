# Function Hiding — Name Lookup Before Signature Matching
## The Problem to Understand
When a derived class declares a function with the same name as a base class function, something unexpected happens — all base class functions with that name become inaccessible through the derived class. This is not overriding. This is **function hiding**, and it happens before the compiler even looks at parameter types.

## How the Compiler Looks Up a Name

When `obj.process(42)` is written, the compiler follows a strict two-step sequence:

### Steps
1. Name lookup: Search the type of obj for a member named 'process'. Start in the derived class
    * If found there → STOP searching (do not look in base)
    * If not found → move up to the base class and repeat
2. Overload resolution (only runs if Step 1 found the name):
    Among all functions found at the scope where the name was found, select the best match for the given arguments
       * If the compiler is evaluating `obj.process(42)`, it first checks the Derived class. The moment it finds any function named process in the Derived class, that class becomes the final scope. The compiler freezes its search right there and refuses to check the Base class.
       * Once the scope is locked to the Derived class, the compiler looks only at the functions found in that specific scope and tries to find the best overload for `42`. It completely ignores whatever is in the Base class, even if a Base class function has a much better signature.
   ```cpp
    class Base {
        public:
            void process(int x) { 
                std::cout << "Base: int version\n"; 
            }
    };

    class Derived : public Base {
        public:
            void process(double x) { 
                std::cout << "Derived: double version\n"; 
            }
    };

    int main() {
        Derived obj;
        obj.process(42); // Derived: double version
    }
    ```
    The critical rule: **Step 1 stops at the first scope where the nameis found.** If the derived class has any function named `process`, the compiler stops there — it never looks at the base class functions named `process`, regardless of their signatures.

## The Surprising Consequence — Overloads Are Hidden
```cpp
class Base {
public:
    void process(int x) {
        std::cout << "Base::process(int)\n";
    }
    void process(double x) {
        std::cout << "Base::process(double)\n";
    }
};

class Derived : public Base {
public:
    void process(std::string s) {    // different parameter — different signature
        std::cout << "Derived::process(string)\n";
    }
};

Derived d;
d.process("hello");   // VALID:   Derived::process(string)
d.process(42);        // COMPILE ERROR: no matching function
d.process(3.14);      // COMPILE ERROR: no matching function
```
The compiler found `process` in `Derived` in Step 1 and stopped. It never reached `Base::process(int)` or `Base::process(double)`. The two base overloads are hidden — invisible through `d`.

This is not intuitive. The programmer added a new overload with a completely different signature. But the name `process` existing in the derived class shadows every `process` in the base.

## Hiding vs Overriding — The Distinction

These two are completely different:

| | Hiding | Overriding |
|---|---|---|
| Requires `virtual`? | NO | YES |
| Signature must match? | NO — any signature hides the name | YES — exact same signature |
| Base functions visible? | NO — all hidden | YES — same slot replaced |
| Dispatch | Always compile-time | Runtime through vtable |
| Called through base pointer? | Calls base version | Calls derived version |


```cpp
class Base {
public:
    void show()          { std::cout << "Base::show\n"; }       // non-virtual
    virtual void display() { std::cout << "Base::display\n"; }  // virtual
};

class Derived : public Base {
public:
    void show()    { std::cout << "Derived::show\n"; }          // HIDES Base::show
    void display() { std::cout << "Derived::display\n"; }       // OVERRIDES Base::display
};

int main() {
    Derived d;
    Base* bp = &d;
    
    d.show();                                                   // Derived::show   — called on Derived object directly
    bp->show();                                                 // Base::show      ← hiding — bp is Base*, compile-time dispatch
                                                                //                   calls Base version regardless of actual object
    
    d.display();                                                // Derived::display
    bp->display();                                              // Derived::display ← overriding — virtual, runtime dispatch
                                                                //                   calls Derived version through vtable
}
```
Hiding only affects access through the derived type. Through a base pointer, the base version is always called for non-virtual functions — this is static binding.

## Hiding Across Multiple Levels

Hiding propagates down the chain. If `B` hides a name from `A`, and `C` inherits from `B`, `C` also cannot see `A`'s version:

```cpp
class A {
public:
    void process(int x)    { std::cout << "A::process(int)\n"; }
    void process(double x) { std::cout << "A::process(double)\n"; }
};

class B : public A {
public:
    void process(std::string s) { std::cout << "B::process(string)\n"; }
    // hides A::process(int) and A::process(double)
};

class C : public B {
    // no process defined here
    // name lookup finds process in B → stops
    // A::process still hidden
};

C obj;
obj.process("hi");   // VALID: B::process(string) found in B
// obj.process(42);  // COMPILE ERROR: only B's process in scope, no (int) overload
```

## Explicit Base Call — Bypassing Hiding

A hidden base function can always be called explicitly using the scope resolution operator:

```cpp
Derived d;
d.Base::process(42);    // VALID: bypasses name lookup, calls Base directly
d.Base::process(3.14);  // VALID: same
```

This calls the base version directly, bypassing the derived class entirely. This is always available even when the name is hidden.

## `using` Declarations — Restoring Hidden Names
A single `using` declaration brings **all** overloads of that name from the base class into the derived class scope. One line restores everything:

```cpp
class Base {
public:
    void process(int x) {
        std::cout << "Base::process(int)\n";
    }
    void process(double x) {
        std::cout << "Base::process(double)\n";
    }
};

class Derived : public Base {
public:
    using Base::process;   // restores ALL Base::process overloads

    void process(std::string s) { std::cout << "Derived::process(string)\n"; }
};

Derived d;
d.process(42);        // VALID: Base::process(int)
d.process(3.14);      // VALID: Base::process(double)
d.process("hello");   // VALID: Derived::process(string)
```

###  Placement — Access Section Matters
The `using` declaration is placed inside the class body. The access section where it appears controls the access of the restored names:

```cpp
class Base {
public:
    void show(int x)    {}
    void show(double x) {}
};

class Derived : public Base {
private:
    using Base::show;   // restores show — but as PRIVATE
    void show(std::string s) {}
};

Derived d;
// d.show(42);    // COMPILE ERROR: show is private in Derived
```

```cpp
class Derived : public Base {
public:
    using Base::show;   // restores show — as PUBLIC
    void show(std::string s) {}
};

Derived d;
d.show(42);    // VALID: public
```

### `using` Cannot Selectively Restore One Overload

A `using` declaration names a function — not a specific overload. All overloads of that name are brought in together. There is no syntax to restore only `process(int)` while leaving `process(double)` hidden.

### `using` With Multiple Levels

`using` can reach any ancestor — not just the direct base:

```cpp
class A {
public:
    void work(int x)    { std::cout << "A::work(int)\n"; }
    void work(double x) { std::cout << "A::work(double)\n"; }
};

class B : public A {
public:
    void work(std::string s) { std::cout << "B::work(string)\n"; }
    // A::work hidden here
};

class C : public B {
public:
    using A::work;   // reaches past B, restores A::work overloads
    void work(char c) { std::cout << "C::work(char)\n"; }
};

C obj;
obj.work(42);        // A::work(int)    — restored from A
obj.work(3.14);      // A::work(double) — restored from A
obj.work('x');       // C::work(char)   — own version
obj.work("hi");      // B::work(string) — not accessable
```

**Fix**
```cpp
class C : public B {
public:
	using A::work;   
	using B::work;   // restores B::work(string)
	void work(char c) {
		std::cout << "C::work(char)\n";
	}
};
```