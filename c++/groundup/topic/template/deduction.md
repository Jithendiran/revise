## Function Template Overloading and Specialization

### Overloading Function Templates

A function template can coexist with other templates and non-template functions. The compiler selects the best match:

```cpp
template<typename T>
void print(T value) {
    std::cout << "template: " << value << "\n";
}

void print(const char* s) {              // non-template overload
    std::cout << "c-string: " << s << "\n";
}

template<typename T>
void print(T* ptr) {                     // pointer overload
    std::cout << "pointer: " << *ptr << "\n";
}

print(42);         // template: 42
print("hello");    // c-string: hello  (non-template preferred for exact match)
int x = 5;
print(&x);         // pointer: 5
```

Overload resolution priority:
1. Non-template functions that are exact matches
2. Template specializations that are exact matches
3. Template instantiations with conversions

### Explicit Specialization of Function Templates

Provides a custom implementation for one specific type. The primary template handles the general case.

```cpp
// Primary template
template<typename T>
void swap_values(T& a, T& b) {
    T temp = a;  a = b;  b = temp;
}

// Explicit specialization for const char*
template<>
void swap_values<const char*>(const char*& a, const char*& b) {
    const char* temp = a;  a = b;  b = temp;
}
```

* `template<>` with an empty list signals explicit specialization.
* If code only has `template<>` function without `template<typename T>` code will not compile

### Specialization vs Overloading

For function templates, overloading is almost always preferred over explicit specialization. A non-template overload is simpler and behaves more predictably:

```cpp
// Preferred: non-template overload
void process(int v)  { /* special for int */ }

template<typename T>
void process(T v)    { /* general */ }
```

## Partial Specialization

Provides a custom implementation for a subset of template arguments — some parameters remain generic.

Partial specialization is only available for class templates and variable templates — NOT for function templates.

### For Pointer Types

```cpp
// Primary template
template<typename T>
class Storage {
    T data;
public:
    Storage(T v) : data(v) {}
    T    get()   const { return data; }
    void print() const { std::cout << "value: " << data << "\n"; }
};

// Partial specialization — for ANY pointer type
template<typename T>
class Storage<T*> {
    T* ptr;
public:
    Storage(T* p) : ptr(p) {}
    T*   get()    const { return ptr; }
    T&   deref()  const { return *ptr; }
    void print()  const { std::cout << "pointer to: " << *ptr << "\n"; }
};

Storage<int>   si(42);            // primary: T = int
Storage<int*>  sp(new int(99));   // partial spec: T = int
sp.deref();    // VALID: only in pointer specialization
```

### When Both Parameters Are the Same Type

```cpp
// Primary: two independent types
template<typename T, typename U>
class Pair {
    T first;  U second;
public:
    Pair(T t, U u) : first(t), second(u) {}
    void print() const {
        std::cout << "different: " << first << ", " << second << "\n";
    }
};

// Partial: both types are the same
template<typename T>
class Pair<T, T> {
    T first;  T second;
public:
    Pair(T a, T b) : first(a), second(b) {}
    T    sum()  const { return first + second; }   // extra — only when same type
    void print() const {
        std::cout << "same: " << first << ", " << second << "\n";
    }
};

Pair<int, double>  p1(1, 2.0);   // primary
Pair<int, int>     p2(3, 4);     // partial spec
p2.sum();    // VALID: T = int
// p1.sum(); // COMPILE ERROR: not in primary
```

### For const Types

```cpp
template<typename T>
class Wrapper {
    T value;
public:
    void set(T v) { value = v; }
};

template<typename T>
class Wrapper<const T> {
    const T value;
public:
    Wrapper(T v) : value(v) {}
    const T& get() const { return value; }
    // no set() — const value cannot be modified
};

Wrapper<int>       wi;       // primary
Wrapper<const int> wci(42);  // const specialization
wci.get();     // VALID
// wi.get();   // COMPILE ERROR: primary has no get()
```

## Class Template Argument Deduction — CTAD (C++17)

Before C++17, class template arguments had to be specified explicitly:

```cpp
// C++14:
std::pair<int, double> p(1, 2.0);   // must specify <int, double>
auto p = std::make_pair(1, 2.0);    // workaround: helper function deduces
```

C++17 — CTAD deduces class template arguments from constructor arguments:

```cpp
std::pair   p(1, 2.0);     // deduces std::pair<int, double>
std::vector v{1, 2, 3};    // deduces std::vector<int>
std::array  a{1, 2, 3};    // deduces std::array<int, 3>
```

### CTAD for User-Defined Templates

```cpp
template<typename T>
class Box {
    T value;
public:
    Box(T v) : value(v) {}
    T get() const { return value; }
};

Box b1(42);        // deduces Box<int>
Box b2(3.14);      // deduces Box<double>
Box b3("hello");   // deduces Box<const char*>
```

### Deduction Guides

When constructor argument types do not directly map to class template parameters, deduction guides tell the compiler how to deduce:

```cpp
template<typename T>
class Container {
    T*          data;
    std::size_t size;
public:
    Container(T* ptr, std::size_t n) : data(ptr), size(n) {}
};

// Deduction guide
template<typename T>
Container(T*, std::size_t) -> Container<T>;

int arr[5] = {1,2,3,4,5};
Container c(arr, 5);   // deduces Container<int> via the guide
```
The template parameter` typename T` belongs to the `class Container<T>`.

Even though both use the letter `T`, the compiler treats them as separate entities during template argument deduction. When `Container c(arr, 5);` the compiler attempts to deduce template arguments for the class template, but by default, it does not automatically look inside the constructor parameters to solve the class template parameters.

```cpp
template<typename T>
Container(T*, std::size_t) -> Container<T>;
```

* Left of `->:` It specifies a function signature matching the constructor's parameter types (`T*` and `std::size_t`).
* Right of `->:` It explicitly tells the compiler which class template specialization to instantiate (`Container<T>`).
