## Object Lifecycle:

### Constructors
A constructor is a specialized initialization function automatically executed by the system the exact moment an object is created in memory.

**The Problem It Solves**

When a primitive variable or a standard `struct` is declared without initialization, the allocated memory contains whatever random, leftover binary data existed at that hardware address. This is known as garbage value.

If a program attempts to use an object containing garbage values, it will produce unpredictable calculations or crash. Without constructors, programmers had to remember to call a manual initialization function (e.g., `init()`) immediately after creation. If a programmer forgot, the code failed.


#### The Mechanism
A constructor guarantees that an object cannot exist in an uninitialized, invalid state. It forces initialization to occur as part of the allocation process.

Syntax Rules: A constructor must have the exact same name as the `class` and cannot specify any return type (not even `void`).

```cpp
class BankAccount {
private:
    int accountId;
    double balance;

public:
    // Default Constructor
    BankAccount() {
        accountId = 0;
        balance = 0.0;
    }

    // Parameterized Constructor
    BankAccount(int id, double initialBalance) {
        accountId = id;
        balance = initialBalance;
    }
};
```

### Destructors
A destructor is a specialized cleanup function automatically executed by the system the exact moment an object is destroyed (goes out of scope or is explicitly deleted).

**The Problem It Solves**

Objects frequently acquire system resources during their lifetime, such as allocating memory on the heap, opening external files, or establishing network connections. If an object is removed from memory but its acquired resources are not released, those resources remain locked and unusable by the operating system. This defect is called a resource leak (or memory leak).

### The Mechanism
A destructor is a special function inside a class that acts like a cleanup crew. Its only job is to shut down and release those extra resources right before the object itself is deleted.

Syntax Rules: A destructor must have the exact same name as the class preceded by a tilde symbol (~), takes no parameters, and has no return type.

```cpp
class DatabaseConnection {
private:
    FileHandle* filePtr; // Points to an open external system file

public:
    // Constructor opens the file
    DatabaseConnection() {
        filePtr = openSystemFile("data.db");
    }

    // Destructor closes the file automatically when the object dies
    ~DatabaseConnection() {
        if (filePtr != nullptr) {
            closeSystemFile(filePtr);
        }
    }
};
```