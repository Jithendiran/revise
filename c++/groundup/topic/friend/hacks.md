### symmetric binary operators
The primary motivation for `friend` is **symmetric binary operators**.

A member function always has the class object as the left operand (via `this`). This breaks when a built-in type appears on the left side:

```cpp
class Vector2D {
    double x, y;
public:
    // Member: handles  v * 2.0  (Vector2D on the left)
    Vector2D operator*(double scalar) const {
        return Vector2D{x * scalar, y * scalar};
    }
};

Vector2D v{1.0, 2.0};
Vector2D a = v * 2.0;    // VALID: translates to v.operator*(2.0)
Vector2D b = 2.0 * v;    // COMPILE ERROR: double has no operator*(Vector2D)
```

A non-member function takes both operands explicitly, solving the problem:

```cpp
class Vector2D {
    double x, y;
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    friend Vector2D operator*(const Vector2D& v, double scalar);
    friend Vector2D operator*(double scalar, const Vector2D& v);
};

Vector2D operator*(const Vector2D& v, double scalar) {
    return Vector2D{v.x * scalar, v.y * scalar};   // accesses private x, y
}

Vector2D operator*(double scalar, const Vector2D& v) {
    return v * scalar;   // reuse the first form
}

Vector2D v{1.0, 2.0};
Vector2D a = v * 2.0;    // VALID
Vector2D b = 2.0 * v;    // VALID
```

### The access section where `friend` is declared does not matter
A `friend` declaration can appear inside `public:`, `private:`, or `protected:` and the result is identical. The access section has no effect on friendship. This confuses readers who expect `private: friend void foo()` to be more restricted than `public: friend void foo()`.

```cpp
class BankAccount {
private:
    double balance;
    friend void auditAccount(const BankAccount& acc);   // same result...

public:
    friend void printAccount(const BankAccount& acc);   // ...as this
};
// Both auditAccount and printAccount have identical access to balance
```

### Friending a specific member function of another class (not the whole class)
Instead of declaring an entire class as `friend`, a single member function of another class can be named. This requires the other class to be fully defined before the `friend` declaration is written, because the compiler must verify the specific function's signature exists.
```cpp
class Auditor {
public:
    void inspect(const BankAccount& acc);   // forward declaration not enough —
                                            // full definition needed before
                                            // BankAccount can friend this
};

class BankAccount {
private:
    double balance;
    friend void Auditor::inspect(const BankAccount& acc);  // only THIS function
                                                            // not all of Auditor
};
```

When a `friend` function or class is declared before the compiler has seen any declaration of it, a forward declaration is required first.

```cpp
// Forward declaration — tells compiler 'auditAccount' exists
void auditAccount(const BankAccount& acc);

class BankAccount {
private:
    double balance;
    friend void auditAccount(const BankAccount& acc);   // VALID: seen above
};

void auditAccount(const BankAccount& acc) {
    std::cout << acc.balance;
}
```
For friend classes:
```cpp
class Auditor;   // forward declaration

class BankAccount {
private:
    double balance;
    friend class Auditor;   // VALID: Auditor forward-declared above
};

class Auditor {   // full definition can come later
public:
    void inspect(const BankAccount& acc) {
        std::cout << acc.balance;
    }
};
```

### `this`
A `friend` function does not have a `this` pointer — ever.

`this` exists only inside member functions. A `friend` function is not a member function. It has no implicit object. It has no hidden first argument. The compiler does not inject this into it.

```cpp
class BankAccount {
private:
    double balance;
    friend void auditAccount(const BankAccount& acc);
};

void auditAccount(const BankAccount& acc) {
    // this->balance;   // COMPILE ERROR: 'this' is undefined in this context
    acc.balance;        // VALID: access through the explicit parameter
}
```

###  Cannot be `const`, `virtual`, `static`, or `override`
These qualifiers modify the hidden `this` pointer or the dispatch mechanism. A friend function has no `this` and is not part of the class, so none of these apply.
```cpp
class BankAccount {
    friend void foo() const;     // COMPILE ERROR: const on non-member
    friend virtual void bar();   // COMPILE ERROR: virtual on non-member
    friend static void baz();    // COMPILE ERROR: static on non-member
};
```

### Cannot be called on the class without an object
A member function can be called through an object or pointer. A friend function is just a free function — it is called by its name, not through an object.

```cpp
BankAccount acc;
acc.auditAccount();          // COMPILE ERROR: auditAccount is not a member
auditAccount(acc);           // VALID: called as a free function
```