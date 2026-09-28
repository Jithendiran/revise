# Function Overriding
## What Overriding Is
Overriding is when a derived class provides a new implementation for a virtual function declared in the base class. The derived function must have the **exact same signature** — same name, same parameter types, same const qualification.

When called through a base pointer or reference, the derived version is selected at runtime through the vtable — not the base version. This is what makes polymorphism work.

## Requirements for Overriding
All four must be true for a function to override:
1. The base function must be declared virtual
2. Same function name
3. Same parameter list (type and order)
4. Same const qualification on the function
If any one of these is not met, the result is hiding — not overriding.

```cpp
#include <iostream>

class Base {
public:
    // 1. Virtual function (Candidate for Overriding)
    virtual void process(int x) const {
        std::cout << "Base::process(int) const\n";
    }
    
    // 2. Virtual function (Candidate for Overriding)
    virtual void display() const {
        std::cout << "Base::display() const\n";
    }

    // 3. Non-virtual function (Candidate for Hiding only)
    void process(float f) {
        std::cout << "Base::process(float) [Non-virtual]\n";
    }
};

class Derived : public Base {
public:
    // --- CATEGORY A: PROPER OVERRIDING (Virtual + Same Signature & Const) ---
    
    void process(int x) const {
        std::cout << "Derived::process(int) const -> [OVERRIDDEN]\n";
    }

    void display() const {
        std::cout << "Derived::display() const -> [OVERRIDDEN]\n";
    }


    // --- CATEGORY B: HIDING BY NON-VIRTUAL MATCH ---
    
    // Hides Base::process(float) because Base's version is non-virtual 
    // (and Derived declares a function with the same name/parameter).
    void process(float f) {
        std::cout << "Derived::process(float) -> [HIDDEN (Non-virtual base)]\n";
    }


    // --- CATEGORY C: HIDING BY PARAMETER MISMATCH ---
    
    // Takes double instead of int. Doesn't override; creates a new overload 
    // that hides all 'process' names from Base in this scope.
    void process(double x) {
        std::cout << "Derived::process(double) -> [HIDDEN (Parameter mismatch)]\n";
    }


    // --- CATEGORY D: HIDING BY CONST MISMATCH ---
    
    // Missing 'const'. Fails to override Base::display() const and 
    // instead hides it for objects of type Derived.
    void display() {
        std::cout << "Derived::display() (non-const) -> [HIDDEN (Const mismatch)]\n";
    }
};

int main() {
    Derived d;
    Base* b = &d;
    const Base* cb = &d;

    std::cout << "=== 1. DEMONSTRATING OVERRIDING (Polymorphism via Base Pointer) ===\n";
    b->process(42);   // Resolves to Derived::process(int) const (Virtual)
    b->process(1.2f); // Resolves to Base::process(float) (Non-virtual, uses static type)
    b->display();     // Resolves to Derived::display() const (Virtual)

    std::cout << "\n=== 2. DEMONSTRATING HIDING (Directly on Derived Object) ===\n";
    d.process(3.14);  // Calls Derived::process(double)
    d.process(1.2f);  // Calls Derived::process(float) (shadows Base's float version)
    d.process(10);    // Calls Derived::process(int) const (found via standard overload resolution)
    d.display();      // Calls Derived::display() (non-const version due to hiding)

    std::cout << "\n=== 3. DEMONSTRATING THE CONST QUALIFIER HIDING TRAP ===\n";
    cb->display();    // Resolves to Derived::display() const because cb is a const Base*

    return 0;
}
```
**Output**
```
=== 1. DEMONSTRATING OVERRIDING (Polymorphism via Base Pointer) ===
Derived::process(int) const -> [OVERRIDDEN]
Base::process(float) [Non-virtual]
Derived::display() const -> [OVERRIDDEN]

=== 2. DEMONSTRATING HIDING (Directly on Derived Object) ===
Derived::process(double) -> [HIDDEN (Parameter mismatch)]
Derived::process(float) -> [HIDDEN (Non-virtual base)]
Derived::process(int) const -> [OVERRIDDEN]
Derived::display() (non-const) -> [HIDDEN (Const mismatch)]

=== 3. DEMONSTRATING THE CONST QUALIFIER HIDING TRAP ===
Derived::display() const -> [OVERRIDDEN]
```

### Hiding vs Overriding — Behavior Difference

The difference becomes visible when calling through a base pointer:

```cpp
class Base {
public:
    void    hide()    { std::cout << "Base::hide\n"; }     // non-virtual
    virtual void over()    { std::cout << "Base::over\n"; } // virtual
};

class Derived : public Base {
public:
    void hide()    { std::cout << "Derived::hide\n"; }   // hides
    void over()    { std::cout << "Derived::over\n"; }   // overrides
};

Derived d;
Base* bp = &d;

bp->hide();   // Base::hide    ← hiding — compile-time, pointer type decides
bp->over();   // Derived::over ← overriding — runtime, vtable decides

d.hide();     // Derived::hide ← called on derived object directly
d.over();     // Derived::over ← same
```

* Hiding: the pointer type decides at compile time.
* Overriding: the actual object type decides at runtime.

### Return Type — Covariant Return Types

The return type must match — with one exception. If the base function returns a pointer or reference to a class, the derived override can return a pointer or reference to a **derived** version of that class. This is called a covariant return type.

```cpp
#include <iostream>

class Base {
public:
    virtual Base* clone() { 
        std::cout << "Base\n";
        return new Base(); 
    }
    virtual ~Base() {} // Good practice for polymorphic classes
};

class Derived : public Base {
public:
    Derived* clone() { 
        std::cout << "Derived\n";
        return new Derived(); 
    }
};

int main() {
    Base* bp = new Derived();
    bp->clone();
    delete bp;
    return 0;
}
```

Any other change to the return type breaks the override and becomes a compile error (with `override` keyword) or silent hiding (without it).

## `override` Keyword (C++11)

Without `override`, a typo or signature mismatch silently creates a hiding function instead of an override — no warning, wrong behavior
at runtime.

```cpp
#include <iostream>

class Base {
public:
    virtual void process(int x) {}
};

class Derived : public Base {
public:
    void process(int x)         {
    // intended override — works
        std::cout<<"process int";
    }   
    void process(double x)      {
    // intended override — silently hides
        std::cout<<"process double";
    }   
    void processs(int x)        {
    // typo — silently hides a nonexistent name
        std::cout<<"processs int";
    }   
};

int main()
{
    Derived dp;
    dp.process(34);
    return 0;
}
```

`override` tells the compiler to verify that this function actually overrides something. If it does not, the compiler reports an error:

```cpp
#include <iostream>

class Base {
public:
    virtual void process(int x) {}
};

class Derived : public Base {
public:
    void process(int x) override {
    // VALID: confirmed override
        std::cout<<"process int";
    }   
    void process(double x) override {
    // COMPILE ERROR: no virtual match in Base
        std::cout<<"process double";
    }   
    void processs(int x) override {
    // COMPILE ERROR: no virtual match in Base
        std::cout<<"processs int";
    }   
};

int main()
{
    Derived dp;
    dp.process(34);
    return 0;
}
```

`override` is not required but should always be written on every function that is intended to override a base class virtual function.

### `final` on a Function

`final` prevents further overriding of a virtual function in any class that derives from this one:

```cpp
class Base {
public:
    virtual void process() {}
};

class Middle : public Base {
public:
    void process() override final {}   // overrides Base, blocks further override
};

class Derived : public Middle {
public:
    void process() override {}   // COMPILE ERROR: process is final in Middle
};
```
### `final` on a Class

`final` on a class prevents any class from inheriting from it:

```cpp
class Leaf final : public Base {
public:
    void process() override {}
};

class Further : public Leaf {};   // COMPILE ERROR: Leaf is final
```