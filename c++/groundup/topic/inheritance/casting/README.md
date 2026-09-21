# Casting
Casting is the process of converting a variable or object from one data type into another data type. In object-oriented programming, casting usually applies to pointers and references of related classes, such as base classes and derived classes.

## Types of Casting
* Upcast
    Upcasting is the conversion of a derived class pointer or reference into a base class pointer or reference. Because a derived class contains all the features of a base class, this conversion is completely safe and happens automatically (implicitly) without requiring special syntax.
* Downcast
    Downcasting is the conversion of a base class pointer or reference into a derived class pointer or reference. This conversion is potentially unsafe because a base class object does not automatically contain the specific features of a derived class. Therefore, downcasting requires explicit instructions and often involves runtime checks to verify that the underlying object is actually of the derived type.


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
```

## Upcasting
Upcasting is the conversion of a Derived pointer or reference into a Base pointer or reference.
```cpp
Derived obj(1, 1000.0, 0.01);
Base* basePtr = &obj; // Upcasting

Derived* dptr = new Derived(3, 3000.0, 0.07);
Base*    bptr = dptr; // Upcasting
```
An instance of Derived contains all the memory layout elements of Base (the vptr, baseData, and baseBalance) plus the added derivedRate. Assigning the address of obj to basePtr is safe because Base members exist within the Derived object memory layout.
* Member variables exclusive to Derived (derivedRate) are inaccessible through basePtr.
* Virtual functions (show()) use dynamic dispatch via the vptr. Calling basePtr->show() executes Derived::show() because the object's vptr points to the Derived vtable.

Only the 8-byte pointer value is copied from `dptr` to `bptr`. The object itself is not touched.

Both pointers point to the same 32-byte Derived object. The vptr inside that object was set to the Derived vtable during construction and has not changed, this is why when calling basePtr or bptr ->show() calls derived class function

The pointer type `Base*` is irrelevant to virtual dispatch. The vptr inside the object determines what gets called — not the type of the pointer used to reach it.

## Downcasting
Downcasting is the conversion of a Base pointer or reference into a Derived pointer or reference.
```cpp
Base* basePtr = new Derived(1, 1000.0, 0.01);
Derived* derivedPtr = dynamic_cast<Derived*>(basePtr); // Downcasting
```
A Base pointer may point to a pure Base object or a Derived object. Downcasting explicitly tells the compiler to treat the pointer as a Derived pointer, restoring access to Derived-specific members like derivedRate.

* dynamic_cast uses runtime type information (RTTI) stored via the vtable to verify object types at runtime. If basePtr actually points to a Derived object, the cast succeeds and returns a valid address. If basePtr points to a pure Base object, the cast fails and returns nullptr.
* static_cast performs the conversion at compile-time without runtime checks. If basePtr does not point to a Derived object, accessing derivedRate through the resulting pointer results in undefined behavior.