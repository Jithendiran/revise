# Object Construction — How the vptr Gets Written

The vptr is set up at the beginning of each constructor's execution (before constrctor body runs)

Here is the exact timeline of what happens during object creation:

1. Memory Allocation: Raw memory is allocated for the entire Derived object (big enough for Base's data, Derived's data, and the vptr).
2. Entering the Base Constructor:
   1. Before the code inside Base constructor body executes, the compiler automatically injects code to point the object's vptr to the Base vtable.
   2. Then, Base constructor body runs.
3. Entering the Derived Constructor:
   1. Before the code inside Derived constructor body executes, the compiler overwrites the vptr to point to the Derived vtable.
   2. Then Derived constructor body runs.

Because of this sequence, an object is never fully a Derived object while the Base constructor is running.

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

	void show() {
		std::cout << "Derived::show\n";
	}

};

int main(){
    Derived  obj  = Derived(1, 1000.0, 0.01);
}
```

### Output
```
--- Inside Base Constructor ---
Base vptr address: 104860265008416
Virtual call inside Base constructor: Base::show
--- Inside Derived Constructor ---
Derived vptr address: 104860265008392
Virtual call inside Derived constructor: Derived::show
```

The Base constructor always sets the vptr to Base's vtable first. The Derived constructor immediately overwrites it with Derived's vtable. After construction, the vptr always reflects the most-derived type.