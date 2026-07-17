## Placement `new` — Constructing at a Specific Address

Placement `new` constructs an object **at a pre-allocated memory address** without allocating new memory. It is used when the memory itself is managed separately — for example, in memory pools, custom allocators, or shared memory regions.

```cpp
#include <new>

char buffer[sizeof(BankAccount)];    // raw memory — no object yet

BankAccount* p = new(buffer) BankAccount(101, 1000.0);
//                  ↑           ↑
//                  address     constructor called at this address
//                  to use      — no heap allocation

p->deposit(100.0);   // VALID: object properly constructed at buffer's address
```

**Destruction with placement `new`:** Because the memory was not allocated by `new`, it must not be freed by `delete`. The destructor must be called **explicitly**, then the memory is managed separately.

```cpp
p->~BankAccount();   // explicit destructor call — required for placement new objects
// delete p;         // WRONG: do not use delete — memory was not new-allocated
// buffer goes out of scope and its memory is released automatically (it is on the stack)
```

Explicit destructor calls are only valid and necessary for objects created with placement `new`. For all normally-constructed objects, the destructor is called automatically — calling it explicitly would run it twice, causing undefined behavior.

## `alignas` — Controlling Alignment
Every type has an alignment requirement — the addresses at which it can validly be placed in memory must be multiples of its alignment value.

- `int` typically requires 4-byte alignment, `double` 8-byte alignment.
- `alignas` overrides the default alignment of a variable or type:

```cpp
alignas(64) int cache_line_var;   // aligned to 64-byte boundary
                                   // useful for preventing cache false sharing
                                   // in concurrent code

struct alignas(16) SimdVector {   // struct aligned to 16 bytes
    float x, y, z, w;             // required for SIMD CPU instructions
};
```

`alignof` queries the alignment requirement of a type:

```cpp
std::cout << alignof(int);      // typically 4
std::cout << alignof(double);   // typically 8
std::cout << alignof(char);     // 1
```


```cpp
#include <iostream>
#include <new>      // Required for placement new
#include <string>

class Dog {
private:
    std::string name;

public:
    // Constructor
    Dog(std::string n) : name(n) {
        std::cout << name << " is born! (Constructor called)\n";
    }

    // Destructor
    ~Dog() {
        std::cout << name << " is destroyed. (Destructor called)\n";
    }

    void bark() {
        std::cout << name << " says Woof!\n";
    }
};

int main() {
    // 1. Allocate raw byte memory on the stack (big enough for a Dog)
    alignas(Dog) char memoryBuffer[sizeof(Dog)];

    // 2. Construct the Dog object inside that specific memory
    Dog* myDog = new (memoryBuffer) Dog("Buddy");

    // 3. Use the object normally
    myDog->bark();

    // 4. MANUALLY CALL THE DESTRUCTOR
    // This cleans up resources (like the std::string) without freeing the memory.
    myDog->~Dog(); 

    // The 'memoryBuffer' array goes out of scope here and is cleaned up automatically,
    // but the Dog object inside it has already been safely dismantled.
    return 0;
}
```