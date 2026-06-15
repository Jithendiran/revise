## Understanding xvalue (eXpiring value)
An xvalue is an object near the end of its lifetime whose resources can be moved/stealed. It occupies a middle ground in the C++ value category system:
* It has an identifiable memory address (like an lvalue).
* It is treated as temporary and eligible to be moved from (like a prvalue).

Together, xvalues and prvalues make up the broader category of **rvalues**.
It is moving the moving the resource from one pointer to another, it is same as 
```cpp
int *x = malloc(4);
*x = 5;
int *y = x;
x = NULL;
```

### Xvalues
An xvalue is a fundamental category recognized by the compiler whenever an expression yields an rvalue that still retains a memory address (like &var_name).

Here are the primary ways an xvalue is generated:
1. Explicit Casts to an `Rvalue` Reference (The `std::move` Category)
   `std::move(x)` does not actually move anything at execution time. It is purely a compile-time cast that converts an lvalue into an rvalue reference (`T&&`). Any expression that explicitly casts an object to an rvalue reference results in an xvalue.
   ```cpp
   int x = 10;

    // All three expressions below produce an xvalue:
    static_cast<int&&>(x);
    (int&&)x;
    std::move(x); // Inside the standard library, std::move is just a static_cast<T&&>
   ```
2. A Function Call Returning an Rvalue Reference
   If a function is declared to return a `T&&`, invoking that function yields an xvalue.
   ```cpp
   int global_var = 100;

    int&& getXValue() {
        return std::move(global_var);
    }

    // In main:
    getXValue(); // This function call expression is an xvalue
    ```
3. Member Access of an xvalue
   If an object is an xvalue, accessing its non-static data member also results in an xvalue. The expiration status of the parent object propagates down to its components.
   ```cpp
    struct Resource {
        int data;
    };

    Resource getResource() { return Resource{42}; }

    // 1. getResource() is a prvalue.
    // 2. Resource{42}.data is an xvalue because it accesses a member of a temporary.
    int val = getResource().data; 

    // 3. Moving a struct turns its members into xvalues:
    Resource r;
    std::move(r).data; // This expression is an xvalue
   ```
4. Rvalue Array Subscripting
   Accessing an element of an rvalue array via the subscript operator [] results in an xvalue.
   ```cpp
    using IntArray = int[3];

    // IntArray{1, 2, 3} is a prvalue array.
    // IntArray{1, 2, 3}[0] is an xvalue expression.
    int first_element = IntArray{1, 2, 3}[0];
   ```

### Mechanics: Binding and Behavior
Because an xvalue is an rvalue with a physical address, it obeys specific binding rules.

**What Can Bind to an xvalue?**
An xvalue can bind to `const T&` (which binds to anything) and `T&&` (rvalue references). It cannot bind to a standard mutable `T&`.
```cpp
int x = 10;

int cp = std::move(x);
int &&r1 = std::move(x);       // Allowed. xvalue binds to rvalue reference.
const int &r2 = std::move(x);  // Allowed. const lvalue reference extends lifetime.

// int &r3 = std::move(x);     // ERROR! Non-const lvalue reference cannot bind to an xvalue.
```

### 1. Why getResource() is a prvalue

When `getResource()` is called, it returns a Resource object by value.
```cpp
Resource getResource() { return Resource{42}; }
```
* The Reality: At the moment of the function call, this object is a pure temporary. It does not have a permanent name, and it does not have an identifiable, persistent address in memory yet. It is a value floating in transition.
* The Category: This fits the exact definition of a prvalue (pure rvalue). It is a literal or a temporary object (address) returned by a function before it is bound to anything.

### 2. Why getResource().data becomes an xvalue
The moment the member access operator `.` is used to look inside that temporary object (it returned the address from that address `.` is applied), the rules change.
```cpp
getResource().data;
```
* The Reality: The parent object (`Resource`) is a temporary that is about to expire. Therefore, its internal member (`data`) is also about to expire. However, unlike the abstract temporary object returned by the function, `data` represents a specific, identifiable chunk of memory inside that temporary object. It occupies a concrete physical offset within the `Resource` struct.
* The Category: Because it has an identifiable memory location (like an lvalue) but is also tracking an object that is about to die (like a prvalue), it becomes an xvalue (expiring value).
  
### Why the Distinction Matters
Unlike a prvalue (e.g., the literal 5), which does not have a memory address until bound, an xvalue already resides at an address in memory. When bound to a T&&, the compiler does not need to allocate a secret hidden temporary slot; it simply points the reference directly to the existing address of the object being expired.


### Copy vs Move
* For a user-defined class (like `std::string` or `std::vector`), the compiler would look at the xvalue and say: "This is an rvalue, so I will invoke the Move Constructor to steal its resources."
* For a primitive type like `int`, there are no constructors, pointers to heap memory, or resources to steal. An `int` is just a raw 4-byte block of memory on the stack.
Because primitives cannot be "moved from" in a structural sense, the compiler falls back to a standard bitwise copy.

### Complex data type
Substituting a `std::string` for a primitive `int` completely changes how the compiler handles the initialization. Because `std::string` manages dynamic heap memory, it can actively participate in move semantics.
Here is the exact behavior of all three scenarios when using `std::string x = "hi";`.

* Scenario 1: `string cp = std::move(x);`
    This triggers Move-Initialization. It uses the xvalue to invoke the move constructor of `std::string`.

    **The Mechanics**
    1. The Expression: `std::move(x)` is an xvalue representing an expiring string.
    2. The Target: `string cp` is a completely new, independent object. (`x` is different object, `cp` is different object)
    3. The Action: The compiler calls the `std::string` move constructor. `cp` steals the internal heap pointer and data boundaries directly from x. It does not duplicate the characters on the heap.
    4. The Cleanup: The internals of `x` are reset to a safe, empty state (`nullptr`).

    **Current State**
    * `cp` contains "hi".
    * `x` is left empty (""). Its resource was stolen.
   
* Scenario 2: `string &&r1 = std::move(x);`
    This triggers Reference Binding to a non-const rvalue reference.

    **The Mechanics**
    1. The Expression: `std::move(x)` is an xvalue.
    2. The Target: `string &&r1` is a reference (an alias). It is not a new object. (`x` is an object, for `r1` no object created)
    3. The Action: No constructor is called. No memory is allocated, and no pointers are transferred. The compiler simply binds the reference `r1` directly to the existing memory address of `x`.
    4. The Cleanup: Because no resources were stolen, `x` remains completely untouched.

    **Current State**
    * `r1` is now just a direct alternative name for `x`.
    * `x` still contains "hi".
    * Because `r1` is a non-const reference, it can be used later to modify or move from `x` (e.g., `string y = std::move(r1);` would finally steal the data).
  
* Scenario 3: `const string &r2 = std::move(x);`
    This triggers Reference Binding to a const lvalue reference.

    **The Mechanics**
    1. The Expression: `std::move(x)` is an xvalue.
    2. The Target: `const string &r2` is a read-only reference.
    3. The Action: A `const T&` reference is highly permissive and can bind to any value category, including xvalues. Like Scenario 2, this is just a reference binding. It points directly to the memory address of x. No data is copied, and no data is moved.
   
    **Current State**
    * `r2` is a read-only alias for `x`.
    * `x` still contains "hi".
    * The data cannot be modified or moved through `r2` because of the const restriction.