#  Copy Assignment Operator — `operator=`
## What It Is and Why It Exists
The copy assignment operator defines what happens when one **existing** object is assigned the value of another **existing** object using `=`.

This is distinct from the copy constructor, which initializes a **new** object from an existing one. The difference is whether the destination object already exists:

```cpp
DataBuffer a(5);
DataBuffer b = a;    // COPY CONSTRUCTOR — b does not exist yet, being initialized
DataBuffer c(10);
c = a;               // COPY ASSIGNMENT — c already exists, being assigned to
c.operator=(a);      // Above is short hand for this, both calls the same function 
```

Without a user-defined copy assignment operator, the compiler generates one that performs a **member-by-member copy** — it will do shallow copy. For classes with raw pointer members, this is wrong for exactly the same reasons.

## Signature
```cpp
ClassName& operator=(const ClassName& rhs);
//       ↑                            ↑
//       returns reference to self    rhs = right-hand side
```
- Returns `T&` (reference to self) to support chaining (`a = b = c`)
- Parameter is `const T&` — source is not modified
- Must be a **member function** — the language requires it

## The Self-Assignment Problem
Before writing any copy assignment operator, the first thing to handle is self-assignment: what happens when an object is assigned to itself.

```cpp
DataBuffer a(5);
a = a;    // self-assignment — should do nothing
```
Without a self-assignment check, a naive implementation destroys its own data before copying from it:
```cpp
DataBuffer& operator=(const DataBuffer& rhs) {
    delete[] data;             // frees the heap block
    size = rhs.size;
    data = new int[rhs.size];  // rhs IS this — rhs.size was just invalidated
    for (int i = 0; i < size; i++)
        data[i] = rhs.data[i]; // reading from freed memory — undefined behavior
    return *this;
}
```
After `delete[] data`, `data` and `rhs.data` point to freed memory (they are the same pointer). Reading `rhs.data[i]` is undefined behavior.

**Self-assignment check:**

```cpp
DataBuffer& operator=(const DataBuffer& rhs) {
    if (this == &rhs) {        // same object — do nothing
        return *this;
    }
    // ... rest of implementation
    return *this;
}
```

### Canonical Implementation
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

    DataBuffer& operator=(const DataBuffer& rhs) { // pass by reference
        // Step 1: Self-assignment check
        if (this == &rhs) return *this;

        // Step 2: Release current resources
        delete[] data;

        // Step 3: Allocate new resources
        size = rhs.size;
        data = new int[rhs.size];

        // Step 4: Copy contents
        for (int i = 0; i < size; i++) data[i] = rhs.data[i];

        // Step 5: Return reference to self
        return *this;
    }
};
```

**Usage:**
```cpp
DataBuffer a(5);
DataBuffer b(10);

b = a;    // copy assignment: b's old heap block freed, new block allocated,
          // a's contents copied into it

DataBuffer c(3);
c = b = a;   // chains: b = a runs first (returns b&), then c = b runs
```

### The Copy-and-Swap Idiom — Exception-Safe Alternative
The canonical implementation above has a weakness: if `new int[rhs.size]` throws `std::bad_alloc`, `data` is already deleted and `size` may be partially updated. The object is left in an invalid state.

The **copy-and-swap idiom** eliminates this by using the copy constructor and `std::swap`:

```cpp
#include <utility>   // for std::swap

DataBuffer& operator=(DataBuffer rhs) {   // rhs is a COPY — passed by value, not an reference
    // till thid rhs has new memory, data has old memory 
    std::swap(data, rhs.data);            // swap this object's pointer with copy's
    // now data has new memory, rhs has old memory
    std::swap(size, rhs.size);            // swap sizes
    return *this;
    // rhs (now holding the old data) is destroyed here — old resources freed
}
```

Approach is like create a duplicate then delete the old one
**How it works:**
1. `rhs` is passed by value — the copy constructor runs on the argument, creating an independent copy. If this copy throws, nothing has changed in `*this` yet — exception safety guaranteed.
2. `std::swap` exchanges the internals — no allocation, no throw possible.
3. When `rhs` goes out of scope at the end of the function, it holds the old data and its destructor frees it.

**Self-assignment is automatically handled:**

```cpp
a = a;
// rhs is a copy of a (made by copy constructor)
// swap exchanges a's internals with the copy
// copy (now holding a's old data) is destroyed
// a is left unchanged — correct
```

No explicit self-assignment check is needed with copy-and-swap.

**Trade-off:** Copy-and-swap always makes a full copy even when not needed, such as when the destination already has a buffer of the right size. The canonical implementation can reuse the existing buffer in such cases. For learning purposes, copy-and-swap is simpler and always correct.

## `= default` and `= delete` for Copy Assignment
```cpp
class Config {
public:
    Config& operator=(const Config&) = default;   // explicitly request compiler version
};
```
### Compiler-Generated Copy Assignment
If no copy assignment operator is declared, the compiler generates one. The generated version performs a **member-by-member copy assignment** — calling `operator=` for each member in declaration order. For primitive types, this copies the value. For class-type members, this calls their own copy assignment.

For classes with raw pointer members, the compiler-generated copy assignment is a shallow copy — the same defect as the compiler-generated copy constructor.


```cpp
class Singleton {
public:
    Singleton& operator=(const Singleton&) = delete;  // prevent assignment
};

Singleton a, b;
a = b;   // COMPILE ERROR: copy assignment is deleted
```

## Move Assignment Operator
### The Problem — Copying When the Source Is Expiring
When the right-hand side of an assignment is a temporary or an object about to be destroyed, copy assignment does unnecessary work — it allocates new memory and copies every byte, only to then destroy the source's memory:

```cpp
DataBuffer a(1000);
a = DataBuffer(2000);   // DataBuffer(2000) is a temporary
                         // copy assignment:
                         // 1. allocate 2000 ints — new heap block
                         // 2. copy 2000 ints from temporary
                         // 3. destroy old a block
                         // 4. temporary destroyed — its block freed
                         // total: 2 allocations, 2000 copies, 2 frees
```
The temporary already owns a heap block with 2000 ints. That block could be stolen directly — no allocation, no copying:

What should happen:
1. free a's old block
2. steal the temporary's pointer
3. null the temporary's pointer
total: 1 free, 3 integer assignments

The move assignment operator implements this transfer.
### Signature
```cpp
ClassName& operator=(ClassName&& rhs) ;
//                            ↑↑
//                  rvalue reference — only binds to temporaries
//                  and objects explicitly marked expiring with std::move
```

- Parameter is `T&&` (rvalue reference) — binds only to rvalues
- Parameter is **not `const`** — the source must be modified (its pointer must be nulled after the transfer)
- Returns `T&` — same chaining requirement as copy assignment

**Functions which evaluates noexcept**
```cpp
ClassName& operator=(ClassName&& rhs) noexcept;
```
- `noexcept` is prefered for move because vector like std implementation check for is `noexcept`, if `noexcept` is available they it will perform move, else it would fall back to copy
- `noexcept` should only performs pointer swaps and integer assignments, which cannot throw. Without `noexcept`, standard containers fall back to copy instead of move (covered in the `noexcept` document)

### Canonical Implementation
```cpp
class DataBuffer {
    int* data;
    int  size;

public:
    DataBuffer(int n) : size(n), data(new int[n]) {}

    ~DataBuffer() { delete[] data; }

    DataBuffer& operator=(DataBuffer&& rhs) noexcept {
        // Step 1: Self-assignment check
        if (this == &rhs) return *this;

        // Step 2: Release current resources
        delete[] data;

        // Step 3: Steal the source's resources
        data = rhs.data;
        size = rhs.size;

        // Step 4: Null out the source — prevents double free
        rhs.data = nullptr;
        rhs.size = 0;

        // Step 5: Return reference to self
        return *this;
    }
};
```

**Optmized with swap**


```cpp
DataBuffer& operator=(DataBuffer&& rhs) noexcept {
    std::swap(data, rhs.data);
    std::swap(size, rhs.size);
    return *this;
    // rhs (now holding the old data) destroyed here
}
```
The self-assignment check is not strictly needed here — swapping with self leaves both values unchanged. It is often omitted from move assignment for this reason.

### The Source Object After Move Assignment
After move assignment, the source object (`rhs`) is in a **valid but unspecified state**. The only guarantee is that its destructor can safely run — hence the `nullptr` assignment to `rhs.data`. A null pointer passed to `delete[]` is a defined no-op.

After a move, the source must not be read from. It may be safely assigned to again (bringing it back to a defined state) or simply destroyed.

```cpp
DataBuffer a(100);
DataBuffer b(200);

b = std::move(a);

// a is now valid but unspecified:
// a.data == nullptr, a.size == 0
// safe to destroy: a's destructor runs delete[] nullptr — no-op
// safe to reassign: a = DataBuffer(50) — brings a back to defined state
// NOT safe to read: a.data[0] — undefined
```

### Compiler-Generated Move Assignment
If no move assignment is declared and the class has no user-declared destructor, copy constructor, or copy assignment, the compiler generates a move assignment that performs a **member-by-member move** — calling `std::move` on each member.

## When Each Assignment Operator Is Invoked
The compiler selects copy or move assignment based on the value category of the right-hand side.
| Right-hand side | Value category | Operator invoked |
|---|---|---|
| Named variable: `b = a` | lvalue | Copy assignment |
| Temporary: `b = DataBuffer(5)` | prvalue | Move assignment |
| `std::move`: `b = std::move(a)` | xvalue | Move assignment |
| Named rvalue ref: `DataBuffer&& r = ...; b = r` | lvalue (named) | Copy assignment |

The last row is the named rvalue reference trap established in the Value Categories document: giving a variable a name makes it an lvalue in subsequent code, regardless of its declared type. `b = r` copies even though `r` is declared `DataBuffer&&`.

```cpp
DataBuffer a(5);
DataBuffer b(10);
DataBuffer c(3);

b = a;                   // copy assignment — a is lvalue
b = DataBuffer(20);      // move assignment — temporary is prvalue
b = std::move(a);        // move assignment — std::move produces xvalue

DataBuffer&& r = DataBuffer(7);
b = r;                   // COPY assignment — r is named, therefore lvalue
b = std::move(r);        // move assignment — std::move on r produces xvalue
```

## `= default` and `= delete`
```cpp
class Widget {
public:
    // Restore compiler-generated versions after they were suppressed
    Widget& operator=(const Widget&) = default;
    Widget& operator=(Widget&&) noexcept = default;
};

class Unique {
public:
    // Prevent copying — must be moved or not transferred
    Unique& operator=(const Unique&) = delete;
    Unique& operator=(Unique&&) noexcept = default;   // move still allowed
};

Unique a, b;
b = a;               // COMPILE ERROR: copy assignment deleted
b = std::move(a);    // VALID: move assignment allowed
```