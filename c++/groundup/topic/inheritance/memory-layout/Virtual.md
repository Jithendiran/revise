# C++ Polymorphism, Memory Layout, and Virtual Dispatch Anatomy Reference

```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
    int    baseData;       // 4 bytes
                           // 4 bytes padding (for alignment)
    double baseBalance;    // 8 bytes

    Base(int d, double b) : baseData(d), baseBalance(b) {}
    
    virtual void show() {   
        std::cout << "Base::show\n";
    }
    // 4 + 4 + 8 + 8 (vptr) = 24
};

class Derived : public Base {
public:
    double derivedRate;    // 8 bytes

    Derived(int d, double b, double r)
        : Base(d, b), derivedRate(r) {}

    void show() {   
        std::cout << "Derived::show\n";
    }
    // 24 (includes vptr) + 8 = 32
};

int main(){
  Derived  obj  = Derived(1, 1000.0, 0.01);
  Base     bobj = obj;

  obj.show();
  bobj.show();

  Derived* ptr  = new Derived(2, 1500.0, 0.02);
  Base*    bptr = ptr;                          

  ptr->show();
  bptr.show();

  std::cout<<"Size of Derived : " << sizeof(obj) << "\n";
  std::cout<<"Size of Base    : " << sizeof(bobj) << "\n";

  std::cout<<"Size of Derived*: " << sizeof(*ptr) << "\n";
  std::cout<<"Size of Base*   : " << sizeof(*bptr) << "\n";
}
```

**Output**
```
Derived::show
Base::show        //---------------------->*
Derived::show
Derived::show     //---------------------->*
Size of Derived : 32
Size of Base    : 24
Size of Derived*: 32
Size of Base*   : 24
```

This document covers two distinct behaviors that appear when a derived object is assigned to a base type — one through a regular object copy, the other through a pointer.

```cpp
Derived  obj  = Derived(1, 1000.0, 0.01);
Base     bobj = obj;     // object copy

Derived* ptr  = new Derived(2, 1500.0, 0.02);
Base*    bptr = ptr;     // pointer assignment
```

The output reveals the difference immediately:

```
obj.show()  → Derived::show     // correct — called on the derived object directly
bobj.show() → Base::show        // ← Base version called, not Derived
ptr->show() → Derived::show     // correct
bptr->show()→ Derived::show     // ← still Derived, even through a Base pointer

sizeof(obj) = 32 sizeof(bobj) = 24
sizeof(*ptr) = 32 sizeof(*bptr) = 24
```

Question arise from this output:

**Question :** `bobj` is a `Base` object of 24 bytes — it has a vptr. `sizeof` confirms it. Yet it dispatches to `Base::show`, not `Derived::show`. The vptr is present but points to the wrong vtable. How did this happen?

This document answers question by examining exactly what occurs in memory when an object is constructed, copied, and sliced — and contrasting that with what happens when a pointer is assigned instead.
