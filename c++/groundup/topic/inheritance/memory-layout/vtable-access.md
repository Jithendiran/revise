##  How the vtable Is Viewed and Read in C++

The vtable and vptr are implementation details — the C++ standard does not expose them. However, because the vptr is always at offset 0, raw memory access can be used to read it:

```cpp
#include <iostream>
#include <cstdint>

class Base {
public:
	int    baseData;

	double baseBalance;

	Base(int d, double b) : baseData(d), baseBalance(b) {}

	virtual void show() {
		std::cout << "Base::show\n";
	}
	virtual void identify() {
		std::cout<< "Base::identify\n";
	}

};

class Derived : public Base {
public:
	double derivedRate;

	Derived(int d, double b, double r)
		: Base(d, b), derivedRate(r) {}

	void show() {
		std::cout << "Derived::show\n";
	}
	void identify() {
		std::cout<< "Derived::identify\n";
	}

};

int main() {
	Derived  obj  = Derived(1, 1000.0, 0.01);
	Base     bobj = obj;

	// Cast the object's address to a pointer to uintptr_t
	// uintptr_t is an unsigned integer large enough to hold a pointer address
	// This lets the first 8 bytes be read as a plain integer
	uintptr_t* raw = reinterpret_cast<uintptr_t*>(&obj);

	std::cout<<"vptr :: " << raw << "\n";

	// The first element is the vptr — the address of the vtable
	uintptr_t vtable_address = raw[0];
	std::cout<<"vtable :: " << vtable_address << "\n";

	// Treat the vtable as an array of function pointer addresses
	uintptr_t* vtable = reinterpret_cast<uintptr_t*>(vtable_address);

	// Read each slot — each is the address of one virtual function
	uintptr_t slot0 = vtable[0];   // address of show()
	uintptr_t slot1 = vtable[1];   // address of identify()

	using Fn = void(*)(Base*);             // function pointer type
    /*
    using Fn =: Introduces an alias named Fn (equivalent to typedef void(*Fn)(Base*); in older C++).
    void: The return type of the functions this pointer can point to (meaning they return nothing).
    (*): Indicates that Fn is a pointer to a function.
    (Base*): The parameter list. Any function assigned to this pointer must take a single argument: a pointer to an object of class Base.
    */
	Fn fn = reinterpret_cast<Fn>(slot0);   // cast address to callable pointer
	fn(&obj);
}
```

### Output
```
vptr :: 0x7ffd7bde0230
vtable :: 102931306216696
Derived::show
```

## Important Caveats
While this is an amazing educational exercise for understanding C++ internals, should never write code like this in production:
* Undefined Behavior (UB): The C++ standard explicitly states that inspecting object internals this way, casting arbitrary memory addresses to function pointers, and relying on object layout details invokes undefined behavior.
* Compiler Dependency: Different compilers structure vtables differently. For instance, MSVC handles virtual inheritance, multiple inheritance, and RTTI pointers (type_info) differently than GCC/Clang. Code might work on Linux with GCC, but crash or behave entirely differently on Windows with MSVC.