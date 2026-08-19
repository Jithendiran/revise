# Forwarding References
## The Problem — Losing Value Category When Passing Arguments

Consider a wrapper function that takes an argument and passes it to anotherfunction. The goal is for the inner function to receive the argument exactly as the caller passed it — same type, same value category.

**What is the problem with `void wrapper(int&& arg)`?**
Even if the caller passes a temporary *rvalue*, the moment it enters wrapper and gets named `arg`, it *loses its temporary tag* and turns into a *permanent-looking lvalue*. When wrapper calls `process(arg)`, process always thinks it received an lvalue. The rvalue information is destroyed.

```cpp
void process(int& x)  { std::cout << "lvalue\n"; }    // for lvalues
void process(int&& x) { std::cout << "rvalue\n"; }    // for rvalues

void wrapper(T&& arg) {
                    // arg now become lvalue, arg own's the content
    process(arg);   // how to forward arg without losing its value category?
}

int a = 5;
wrapper(a);            // caller passes lvalue — process should receive lvalue
wrapper(std::move(a)); // caller passes rvalue — process should receive rvalue
```

**What is the type can be used instead of `T&&` to preserve the type?**

*The parameter type problem:*

```cpp
// If wrapper takes by value:

// Note: It can take lvalue, rvalue and xvalue
void wrapper(int arg) { process(arg); }
// The catch is what happens to it once it's inside the wrapper function.
// arg is always a named variable — always lvalue inside wrapper
// process(arg) always calls lvalue overload — rvalue information lost

// If wrapper takes lvalue reference:
void wrapper(int& arg) { process(arg); }
wrapper(std::move(a));  // COMPILE ERROR: cannot bind lvalue ref to rvalue

// If wrapper takes rvalue reference:
void wrapper(int&& arg) { process(arg); }
wrapper(a);             // COMPILE ERROR: cannot bind rvalue ref to lvalue

// If wrapper takes const lvalue reference:
/*
Note:
In C++, a const lvalue reference (const T&) has a special superpower: it can bind to temporaries (rvalues).
    * When call `wrapper(a)` where `a` is an lvalue variable, `arg` binds to `a` normally.
    * When call `wrapper(5)` where `5` is a temporary rvalue, `const int&` is allowed to bind to it (which extends the temporary's lifetime).
*/
void wrapper(const int& arg) { process(arg); }
// binds to both — but process(arg) always calls lvalue overload
// rvalue information permanently lost
```

None of the four approaches work. A forwarding reference solves this.

## What Makes T&& a Forwarding Reference
```cpp
template<typename T>
void wrapper(T&& arg) { process(arg); }
```
**This is Forwarding Reference**

`T&&` is a forwarding reference only when both conditions are met:
* Condition 1: `T` is a type parameter being DEDUCED by the compiler
* Condition 2: The form is exactly `T&&` — no other qualifiers
**FORWARDING REFERENCE**
In C++, a forwarding reference (`T &&`) isn't a separate type; it's a magic trick performed by the compiler using Template Type Deduction and Reference Collapsing
```cpp
// T is deduced from the call
template<typename T>
void foo(T&& param);

// FORWARDING REFERENCE — auto is deduced from the initializer
auto&& x = someExpression;
```

**Not FORWARDING REFERENCE**
```cpp
// NOT forwarding references:
void bar(int&& param);             // int is concrete — rvalue reference, become l value
/*
Why it is not a forward reference?
There is no template (typename T) here. The compiler doesn't need to deduce anything. Because it explicitly says int&&, this function only accepts rvalues (temporaries). If you pass an lvalue, it throws a compilation error.
*/ 

template<typename T>
void baz(const T&& param);         // const present — rvalue reference
/*
Why it is not a forward reference?
A forwarding reference must be naked (T&& with nothing else attached). The moment you add const (const T&&), you tell the compiler: "I only want to accept things that are const rvalues." It loses the ability to cleanly bind to regular lvalues and non-const rvalues.
*/

template<typename T>
void qux(std::vector<T>&& param);  // T is deduced but vector<T> is not T directly
                                    // rvalue reference
/*
Why it is not a forward reference?
Even though there is a template (typename T), the parameter type is std::vector<T>&&, not T&& directly. When you pass a vector to this function, the compiler deduces what T is (e.g., int), but the parameter itself is strictly an rvalue reference to a vector (std::vector<int>&&). You cannot pass an lvalue vector here.
*/

template<typename T>
class MyClass {
    void method(T&& param);        // T fixed when class instantiated, not deduced here
};                                  // rvalue reference
/*
Why it is not a forward reference?
In a class template like template<typename T> class MyClass, T is usually locked in when you create the object (e.g., MyClass<int> obj;), not when you call the method. By the time you call obj.method(arg), T is already permanently known as int. Therefore, inside the method, T&& acts just like int&& (a regular rvalue reference), and it will reject lvalues.
*/
```

##  Reference Collapsing
The compiler cannot represent `int& &&` directly — no reference-to-reference exists in valid C++ user code. The compiler resolves it using four fixed rules:


| Left Qualifier (`T`) | Right Qualifier (Declaration) | Result | Notes |
| --- | --- | --- | --- |
| `&` | `&` | `&` |  |
| `&` | `&&` | `&` | $\leftarrow$ lvalue wins |
| `&&` | `&` | `&` | $\leftarrow$ lvalue wins |
| `&&` | `&&` | `&&` |  |


**The single principle:** lvalue reference (`&`) is dominant. Any combination containing a `&` collapses to `&`. Only `&&` + `&&` produces `&&`.

```
int& && → & (case: lvalue passed, T = int&)
int&& → && (case: rvalue passed, T = int)
```

## How Deduction Works — The Two Cases

When a forwarding reference parameter receives an argument, the compiler deduces T based on the **value category** of the argument:

### Case 1 — Lvalue Argument

The compiler deduces T as **lvalue reference** (`T&`):

```cpp
template<typename T>
void foo(T&& param);

int a = 5;
foo(a);   // a is lvalue(&), param is rvalue(&&); & + && = & 
```
`T` $\rarr$ `T&` + `&&` = `&`

`param` becomes `int&` — an lvalue reference to `a`. The lvalue category is preserved.

### Case 2 — Rvalue Argument
The compiler deduces T as a **plain type** (no reference):

```cpp
foo(5);            // 5 is prvalue
foo(std::move(a)); // xvalue  && + && = &&
```

`T` $\rarr$ `T&&` + `&&` = `&&`

`param` becomes `int&&` — an rvalue reference. The rvalue category is preserved.

## The Named Variable Problem

After a forwarding reference receives an argument, `param` has a name. A named variable is always an lvalue in subsequent code — regardless of its declared type.

```cpp
template<typename T>
void wrapper(T&& param) {
    // What is param here?
    // param has a NAME — named variables are ALWAYS lvalues in the function body
    // Even if T&& collapsed to int&&, param itself is an lvalue

    process(param);   // ALWAYS calls lvalue overload of process
                      // rvalue-ness of the original argument is LOST
}
```
```
Caller passes rvalue: wrapper(5)
↓
T deduces as int T&& = int&&; param type is int&&
↓
Inside wrapper:
param has a name — param is an LVALUE
process(param) → lvalue overload called
original rvalue information is gone
```
This is the exact problem that `std::forward` solves.

## std::forward — Restoring the Original Value Category

`std::forward<T>(param)` restores the original value category of the argument based on what T was deduced as:

- If T was deduced as `int&` (lvalue was passed) → `std::forward<int&>(param)` returns `int&` (lvalue reference)
- If T was deduced as `int` (rvalue was passed) → `std::forward<int>(param)` returns `int&&` (rvalue reference)

```cpp
template<typename T>
void wrapper(T&& param) {
    process(std::forward<T>(param));   // preserves original value category
}

int a = 5;
wrapper(a);            // T = int&  → forward returns int&  → lvalue overload
wrapper(5);            // T = int   → forward returns int&& → rvalue overload
wrapper(std::move(a)); // T = int   → forward returns int&& → rvalue overload
```