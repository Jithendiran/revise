# Rule of Three
##  The Problem — Three Functions, One Shared Responsibility
| Function | Responsibility |
|---|---|
| Destructor | Release the resource when the object ends |
| Copy constructor | Create a new independent copy of the resource |
| Copy assignment | Replace an existing object's resource with a copy of another's |

When a class owns a raw pointer to heap memory, all three functions must be correct for the class to behave safely. The problem is that the compiler generates default versions of all three — and the compiler-generated versions are correct only for classes that do not own raw resources.

```cpp
class DataBuffer {
    int* data;
    int  size;
public:
    DataBuffer(int n) : size(n), data(new int[n]) {}
    // No destructor — heap block never freed
    // No copy constructor — shallow copy, both objects share one block
    // No copy assignment — shallow copy assignment, same problem
};
```

If any one of these three is absent or wrong, the class is broken — not in an obvious way that produces a compile error, but in a silent way that produces memory leaks, double frees, use-after-free access, and undefined behavior.

## The Rule
> **If a class requires a user-defined destructor, copy constructor, or  copy assignment operator, it almost certainly requires all three.**

The logic behind this rule: the need for any one of these three signals that the class manages a resource that the compiler's default behavior cannot handle correctly. That same resource requires correct behavior from all three special functions. Defining one without the others leaves the resource exposed to mismanagement through the other two paths.

## What Goes Wrong When Each Is Missing
### Destructor Present, Copy Constructor Missing
The user-defined destructor correctly frees the heap block. But the compiler-generated copy constructor performs a shallow copy — both the original and the copy point to the same heap block. When either is destroyed, it frees the block. The other then holds a dangling pointer.

```cpp
class DataBuffer {
    int* data;
    int  size;
public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    ~DataBuffer() { delete[] data; }   // correct

    // copy constructor NOT defined — compiler generates shallow copy
};

{
    DataBuffer a(5);
    DataBuffer b = a;       // shallow copy: b.data == a.data (same address)
}   // b destroyed first: delete[] b.data (0x1000 freed)
    // a destroyed second: delete[] a.data (0x1000 — DOUBLE FREE — crash)
```

The destructor being correct is not enough. The copy constructor must also be correct, because objects are copied in many situations — passed by value, returned by value, inserted into containers.

### Copy Constructor Present, Destructor Missing
The user-defined copy constructor correctly allocates a new heap block for each copy. But the missing destructor means neither the original nor any copy ever frees their heap blocks. Every object creation leaks memory.
```cpp
class DataBuffer {
    int* data;
    int  size;
public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    DataBuffer(const DataBuffer& other)       // correct deep copy
        : size(other.size), data(new int[other.size])
    {
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }

    // destructor NOT defined — compiler generates empty destructor
    // heap blocks are NEVER freed
};

void process() {
    DataBuffer a(1000);   // allocates 4000 bytes
    DataBuffer b = a;     // allocates another 4000 bytes
}   // both objects destroyed — compiler destructor runs — nothing freed
    // 8000 bytes leaked on every call to process()
```

### Copy Assignment Missing, Others Present

The destructor and copy constructor are correct. But when an existing object is assigned from another, the compiler-generated copy assignment performs a shallow copy — overwriting the destination pointer without freeing the original heap block first, and then pointing both objects at the same block.

```cpp
class DataBuffer {
    int* data;
    int  size;
public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    ~DataBuffer() { delete[] data; }

    DataBuffer(const DataBuffer& other)
        : size(other.size), data(new int[other.size])
    {
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }

    // copy assignment NOT defined — compiler generates shallow copy assignment
};

DataBuffer a(5);    // a.data = 0x1000
DataBuffer b(10);   // b.data = 0x2000

b = a;
// compiler-generated copy assignment:
//   b.data = a.data    → b.data = 0x1000
//   b.size = a.size    → b.size = 5
//
// PROBLEMS:
//   1. b's original heap block (0x2000, 10 ints) was NEVER freed — memory leak
//   2. b.data and a.data now both point to 0x1000 — double free on destruction
```

### All Three Present but Inconsistent

The most subtle failure: all three functions are defined, but they do not agree on the ownership model.

```cpp
class DataBuffer {
    int* data;
    int  size;
public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    ~DataBuffer() { delete[] data; }   // frees the block

    DataBuffer(const DataBuffer& other)
        : size(other.size), data(other.data)   // WRONG: shallow copy
    {}

    DataBuffer& operator=(const DataBuffer& rhs) {
        if (this == &rhs) return *this;
        delete[] data;
        size = rhs.size;
        data = new int[rhs.size];
        for (int i = 0; i < size; i++) data[i] = rhs.data[i];
        return *this;
    }
};
```

The copy assignment is correct but the copy constructor is wrong. The three functions must implement the same ownership model consistently. Having two correct and one wrong produces bugs only on the code paths that trigger the wrong one — making them hard to find.

## The Complete, Correct Implementation
All three functions follow from the same ownership decision: this class owns its heap block exclusively. Every object gets its own block. The block is freed when the object is destroyed.

```cpp
class DataBuffer {
    int* data;
    int  size;

public:
    // Regular constructor
    DataBuffer(int n)
        : size(n), data(new int[n])
    {
        for (int i = 0; i < n; i++) data[i] = 0;
    }

    // 1. DESTRUCTOR — release the resource
    ~DataBuffer() {
        delete[] data;
        data = nullptr;
        size = 0;
    }

    // 2. COPY CONSTRUCTOR — create an independent copy of the resource
    DataBuffer(const DataBuffer& other)
        : size(other.size),
          data(new int[other.size])
    {
        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }

    // 3. COPY ASSIGNMENT — replace this object's resource with a copy
    DataBuffer& operator=(const DataBuffer& rhs) {
        // Self-assignment check
        if (this == &rhs) return *this;

        // Release current resource
        delete[] data;

        // Acquire new resource
        size = rhs.size;
        data = new int[rhs.size];

        // Copy contents
        for (int i = 0; i < size; i++)
            data[i] = rhs.data[i];

        return *this;
    }
};
```

**Verification — every scenario is handled correctly:**

```cpp
DataBuffer a(5);            // constructor: 5-element block at 0x1000

DataBuffer b = a;           // copy constructor: new 5-element block at 0x2000
                             // b.data[i] == a.data[i], separate blocks

b.data[0] = 99;             // modifies b only — a.data[0] still 0

DataBuffer c(3);             // constructor: 3-element block at 0x3000
c = a;                       // copy assignment:
                             //   delete[] 0x3000 (c's old block freed)
                             //   new 5-element block at 0x4000
                             //   contents of a copied into 0x4000

a = a;                       // self-assignment: if check, return *this — correct
```

## The Copy-and-Swap Alternative

The copy-and-swap idiom implements copy assignment by reusing the copy constructor and destructor. This ensures the three functions are automatically consistent — copy assignment is implemented in terms of the other two rather than independently:

```cpp
#include <utility>   // for std::swap

DataBuffer& operator=(DataBuffer rhs) {   // parameter is a copy
    std::swap(data, rhs.data);
    std::swap(size, rhs.size);
    return *this;
}   // rhs (holding old data) destroyed here by the destructor
```

If the destructor and copy constructor are correct, copy assignment is automatically correct. The three functions cannot become inconsistent because two of them are written directly in terms of the third.


## `= delete` — Preventing Copying Entirely

Sometimes a resource cannot or must not be copied. A network connection cannot be duplicated. A unique ownership handle cannot have two owners.

In these cases, the correct Rule of Three implementation is to delete the copy operations — making copying a compile error — while still providing a correct destructor.

```cpp
class NetworkConnection {
    int socketFd;
public:
    NetworkConnection(const std::string& address)
        : socketFd(openSocket(address))
    {}

    ~NetworkConnection() {
        closeSocket(socketFd);
    }

    // Copying a socket connection makes no sense — delete both
    NetworkConnection(const NetworkConnection&) = delete;
    NetworkConnection& operator=(const NetworkConnection&) = delete;
};

NetworkConnection a("192.168.1.1");
NetworkConnection b = a;    // COMPILE ERROR: copy constructor is deleted
NetworkConnection c("10.0.0.1");
c = a;                       // COMPILE ERROR: copy assignment is deleted
```

Deleting the copy operations is an explicit statement of the ownership model: this object exclusively owns this resource, and that exclusive ownership cannot be transferred by copying.

## Connection to the Rule of Five

The Rule of Three was the complete rule before C++11. C++11 introduced move semantics, adding two more special functions to the set:

- Move constructor
- Move assignment operator

A class that needs the Rule of Three almost certainly also needs correct move operations — otherwise, the move operations either do not exist (missing an optimization opportunity) or the compiler generates shallow move operations (the same problem as shallow copy, applied to moves).

The Rule of Three extended to five functions is called the **Rule of Five**, covered in the Move Semantics block. The dependency chain is:
```
Rule of Three (this document)
↓ requires
Move Constructor
Move Assignment
↓ together form
Rule of Five
```

## Connection to the Rule of Zero
The correct long-term solution to the Rule of Three is to eliminate the need for it entirely. If a class uses **only well-managed member types** — `std::string`, `std::vector`, `std::unique_ptr` — rather than raw pointers, the compiler-generated versions of all three functions are correct by construction. No user-defined destructor, copy constructor, or copy assignment is needed.

```cpp
class DataBuffer {
    std::vector<int> data;   // std::vector manages the heap block

public:
    DataBuffer(int n) : data(n, 0) {}

    // No destructor — std::vector's destructor frees the heap block
    // No copy constructor — std::vector's copy constructor deep-copies
    // No copy assignment — std::vector's copy assignment deep-copies correctly

    // All compiler-generated versions are correct
    // This is the Rule of Zero
};
```

The Rule of Zero — define none of the five special functions when members manage their own resources — is covered in the Rule of Five document.

## Contents
1. [Rule 5](./5.md) 
2. [Rule 0](./0.md)