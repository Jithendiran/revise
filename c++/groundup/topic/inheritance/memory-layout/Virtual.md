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

## What a vtable Is
A vtable (virtual table) is an array of function pointers. One vtable exists per class that has virtual functions. The vtable contains the address of the actual machine code for each virtual function declared in that class.

The vtable is created by the **compiler** — before the program runs. It lives in the **read-only code segment** of the binary file (the same region as function code). It never changes at runtime. It is not allocated on the stack or heap. It is a fixed table baked into the program.

Binary file — read-only code segment:
```
Base vtable (at address 0x564d751c1d20):
┌───────────────────────────────────────────┐
│ slot 0 │ address of Base::show()          │
│ slot 1 │ address of Base::identify()      │
│ slot 2 │ address of Base::~Base()         │
└───────────────────────────────────────────┘

Derived vtable (at address 0x564d751c1cf0):
┌───────────────────────────────────────────┐
│ slot 0 │ address of Derived::show()       │
│ slot 1 │ address of Derived::identify()   │
│ slot 2 │ address of Derived::~Derived()   │
└───────────────────────────────────────────┘
```

The slot order matches the order in which virtual functions are declared in the base class. When a derived class overrides a function, its vtable slot for that function holds the derived class's address instead of the base class's address. Slot numbers do not change across the hierarchy.

Two vtables. Two different addresses. This is confirmed by the program:
```
Base vtable: 0x564d751c1d20
Derived vtable: 0x564d751c1cf0
```
* A vtable (virtual table) is created per class, not per object.
* In a vtable, only virtual functions are present, not every function.

## What a vptr Is
A vptr (virtual pointer) is a hidden 8-byte pointer stored **inside every object** of a class that has virtual functions. It lives at offset 0 — the very first bytes of the object. The vptr holds the address of the vtable for that object's actual type.

The vptr is written by the **constructor** at runtime. It is not part of the class definition the programmer writes — the compiler inserts it automatically.

vptr only present when a class has vtable

```
Object in memory (at runtime):

offset 0 │ vptr │ 8 bytes → points to the vtable
offset 8 │ data members begin
```
The vptr is the runtime link between an object and its vtable. When a virtual function is called, the CPU reads the vptr, follows it to the vtable, reads the correct slot, and jumps to the function at that address.

* Per Class (vtable): There is only one vtable generated for a class that has virtual functions. It contains the memory addresses of the virtual functions for that specific class and is shared by all instances of that class.
* Per Object (vptr): Every individual object of that class contains a hidden pointer (usually called the vptr or virtual pointer) that points to the shared vtable of its class.