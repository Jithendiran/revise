# Object Copy — Slicing, New Object, vptr Reset
```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
	int    baseData;

	double baseBalance;

	Base(int d, double b) : baseData(d), baseBalance(b) {
        std::cout << "--- Inside Base Constructor ---\n";
        
        // Inspect vptr
        uintptr_t* raw = reinterpret_cast<uintptr_t*>(this);
        std::cout << "Base vptr address: " << raw[0] << "\n";
        
        // Test which function vtable points to right now
        uintptr_t* vtable = reinterpret_cast<uintptr_t*>(raw[0]);
        using Fn = void(*)(Base*);
        Fn fn = reinterpret_cast<Fn>(vtable[0]);
        
        std::cout << "Virtual call inside Base constructor: ";
        fn(this); // Calls Base::show
    }

    Base(const Base& other)
        : baseData(other.baseData), baseBalance(other.baseBalance)
    {
        std::cout << "--- Inside Base Copy Constructor ---\n";
        
        // Inspect vptr
        uintptr_t* raw = reinterpret_cast<uintptr_t*>(this);
        std::cout << "Base vptr address: " << raw[0] << "\n";
        
        // Test which function vtable points to right now
        uintptr_t* vtable = reinterpret_cast<uintptr_t*>(raw[0]);
        using Fn = void(*)(Base*);
        Fn fn = reinterpret_cast<Fn>(vtable[0]);
        
        std::cout << "Virtual call inside Base constructor: ";
        fn(this); // Calls Base::show
    }

	virtual void show() {
		std::cout << "Base::show\n";
	}

};

class Derived : public Base {
public:
	double derivedRate;

	Derived(int d, double b, double r)
		: Base(d, b), derivedRate(r) {
        std::cout << "--- Inside Derived Constructor ---\n";
        
        // Inspect vptr
        uintptr_t* raw = reinterpret_cast<uintptr_t*>(this);
        std::cout << "Derived vptr address: " << raw[0] << "\n";
        
        // Test which function vtable points to right now
        uintptr_t* vtable = reinterpret_cast<uintptr_t*>(raw[0]);
        using Fn = void(*)(Base*);
        Fn fn = reinterpret_cast<Fn>(vtable[0]);
        
        std::cout << "Virtual call inside Derived constructor: ";
        fn(this); // Calls Derived::show
    }
    
    Derived(const Derived& other)
        : Base(other), derivedRate(other.derivedRate) {
            std::cout << "--- Inside Derived copy Constructor ---\n";
        
        // Inspect vptr
        uintptr_t* raw = reinterpret_cast<uintptr_t*>(this);
        std::cout << "Derived vptr address: " << raw[0] << "\n";
        
        // Test which function vtable points to right now
        uintptr_t* vtable = reinterpret_cast<uintptr_t*>(raw[0]);
        using Fn = void(*)(Base*);
        Fn fn = reinterpret_cast<Fn>(vtable[0]);
        
        std::cout << "Virtual call inside Derived constructor: ";
        fn(this); // Calls Derived::show
    }

	void show() {
		std::cout << "Derived::show\n";
	}

};

int main(){
    Derived src(4, 4000.0, 0.09);
    Base    sliced = src;
    sliced.show();
    Derived* cast_back = static_cast<Derived*>(&sliced);
    std::cout << "\nCast back" << cast_back->derivedRat <<"\n";
    cast_back->show();
     std::cout << "\n";
    Derived d2 = src;
}
```

**Output**
```
--- Inside Base Constructor ---
Base vptr address: 94984860437792
Virtual call inside Base constructor: Base::show
--- Inside Derived Constructor ---
Derived vptr address: 94984860437768
Virtual call inside Derived constructor: Derived::show
--- Inside Base Copy Constructor ---
Base vptr address: 94984860437792
Virtual call inside Base constructor: Base::show
Base::show

Cast back6.92605e-310
Base::show

--- Inside Base Copy Constructor ---
Base vptr address: 94984860437792
Virtual call inside Base constructor: Base::show
--- Inside Derived copy Constructor ---
Derived vptr address: 94984860437768
Virtual call inside Derived constructor: Derived::show
```

`sliced` is a `Base` object — it is 24 bytes. The `Base` copy constructor is invoked. It copies `baseData` and `baseBalance` from `src`.

The `vptr` is never copied. It is written by the compiler, not by the copy constructor body.

## What Actually Happens Step by Step
* src is a Derived object. Its vptr points to the Derived vtable.
* sliced is a Base object. The Base copy constructor is called.

```
Step 1 — compiler allocates memory for sliced (24 bytes)

Step 2 — compiler writes Base vtable address into sliced's vptr
          THIS HAPPENS BEFORE THE CONSTRUCTOR BODY RUNS
          the compiler does this automatically for every constructor call

Step 3 — Base copy constructor body runs:
          baseData    = other.baseData;      ← member copied
          baseBalance = other.baseBalance;   ← member copied
          vptr        → NOT touched, NOT read from source
```

## Why the Compiler Does This — Not the Body
The vptr is not a data member. It does not appear in the class definition. The programmer cannot write to it. The copy constructor body only sees and copies what is declared as members: baseData and baseBalance.

The compiler treats the vptr separately — as infrastructure. For every constructor call, the compiler inserts a hidden instruction before the body:

## Why the Sliced Object Still Has a vptr


`Base` declared a virtual function. Every `Base` object — regardless of how it was created — has a vptr at offset 0. Slicing does not remove the vptr. It creates a new `Base` object with its own vptr pointing to the Base vtable.

The vptr is a property of the object's type. `sliced` is type `Base`. `Base` has virtual functions. Therefore `sliced` has a vptr.

## Casting the Sliced Object Back to Derived


```cpp
Derived* cast_back = static_cast<Derived*>(&sliced);
```

This compiles. The pointer `cast_back` now points to the 24-byte `sliced` object, but the compiler treats the 8 bytes after offset 24 as if they were `derivedRate`. Those bytes were never written — they contain garbage:`6.84016e-310`

Furthermore, `sliced`'s vptr still points to the Base vtable. Even through a `Derived*`, the virtual dispatch reads the vptr from the object and finds the Base vtable
The cast changed the pointer type. It did not restore the Derived portion of the object — that data was never there. This is **undefined behavior** — reading memory that was never written. It happens to produce a readable output here, but it is not safe or meaningful.

**A sliced object cannot be safely cast back to Derived. The derived data is gone permanently.**

## Derived Copy (No Slicing)
```cpp
Derived d2 = src;
```

The **Derived copy constructor** is called. It calls Base copy constructor first (which sets Base vtable), then the Derived copy constructor overwrites the vptr to Derived vtable — same two-step pattern as normal construction.

## The One Rule

The compiler writes the vptr based on the **type being constructed**, before the constructor body runs. The source object's vptr is never read or copied. In derived construction, the vptr is written twice — once by the base constructor, once by the derived constructor — the derived value is always the final one.