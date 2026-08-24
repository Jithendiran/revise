
## Class Templates

A class template is a blueprint for a class where the stored type and size constants are parameters.

### Syntax

```cpp
template<typename T>
class Stack {
    T*          data;
    std::size_t top;
    std::size_t capacity;

public:
    Stack(std::size_t cap = 16)
        : capacity(cap), top(0), data(new T[cap])
    {}

    ~Stack() { delete[] data; }

    void push(const T& value) {
        if (top < capacity) data[top++] = value;
    }

    T pop() {
        return data[--top];
    }

    bool        empty() const { return top == 0; }
    std::size_t size()  const { return top; }
};
```

### Instantiation

```cpp
Stack<int>         int_stack;
Stack<double>      double_stack;
Stack<std::string> string_stack;

int_stack.push(1);
int_stack.push(2);
int x = int_stack.pop();   // x = 2
```

Each instantiation is a completely separate class. They share no code.

### Multiple Parameters

```cpp
template<typename Key, typename Value, std::size_t Capacity = 16>
class FixedMap {
    Key   keys[Capacity];
    Value values[Capacity];
    int   count;

public:
    FixedMap() : count(0) {}

    void insert(const Key& k, const Value& v) {
        keys[count] = k;  values[count] = v;  count++;
    }

    Value* find(const Key& k) {
        for (int i = 0; i < count; i++)
            if (keys[i] == k) return &values[i];
        return nullptr;
    }
};

FixedMap<std::string, int>   scores;
FixedMap<int, double, 32>    data;
```

## Member Functions of Class Templates

### Defined Inside the Class Body

Member functions defined inside automatically belong to the template:

```cpp
template<typename T>
class Box {
    T value;
public:
    Box(T v) : value(v) {}
    T    get() const { return value; }   // inside — part of template
    void set(T v)    { value = v; }      // inside — part of template
};
```

### Defined Outside the Class Body

Must repeat the template parameter list and qualify with the class template name:

```cpp
template<typename T>
class Box {
    T value;
public:
    Box(T v);
    T get() const;
};

template<typename T>
Box<T>::Box(T v) : value(v) {}

template<typename T>
T Box<T>::get() const { return value; }
```

### Member Function Templates

A member function can itself be a template with its own type parameters:

```cpp
class Converter {
public:
    template<typename To, typename From>
    To convert(From value) {
        return static_cast<To>(value);
    }
};

Converter c;
int    i = c.convert<int>(3.14);     // To = int,    From = double
double d = c.convert<double>(42);    // To = double, From = int
```

## Dependent Types and the `typename` Keyword

### Without template 
**This is base for understanding the concept**
```cpp
#include <iostream>

// A standard, non-template class with a fixed nested type and static value
class Archive {
public:
    using archive_identifier = int; // Fixed nested type
    static const int security_level = 3; // Fixed static value
};

// A regular function that is not a template
void processArchive() {
    Archive::archive_identifier code = 999; // compiler knows it is int
    int level = Archive::security_level;    // compiler knows it is a static const int value

    std::cout << "Archive Code: " << code << "\n";
    std::cout << "Security Level: " << level << "\n";
}

int main() {
    processArchive();
    return 0;
}
```

The compiler knows the exact definition of non-template classes immediately during compilation. Because the identifiers do not depend on any unknown template parameters, no ambiguity exists regarding whether an identifier is a type or a value. Consequently, the compiler forbids the use of `typename` in this context.

```cpp
template<typename Container>
void printFirst(const Container& c) {
    Container::value_type first = *c.begin(); // here comes the ambiguity it can be static value or type
    std::cout << first << "\n";
}
```

### The Core Concept: Dependent Names

A template allows code to work with different data types supplied later. Inside a template, a name that relies on a template parameter is called a **dependent name**.

Consider the expression `Container::value_type`.

* `Container` is a template parameter.
* Therefore, `value_type` depends on whatever type `Container` becomes during compilation.


### The Compiler's Parsing Problem

During the initial phase of compiling a template, the compiler reads the code before knowing the exact definition of `Container`.

Within any class, a nested identifier like `value_type` can represent two completely different entities:

1. **A Data Type:** A specification for creating variables (such as `int`, `double`, or a custom class).
2. **A Static Value:** A constant variable or a static member variable stored inside the class.

Without explicit guidance, the C++ language specification enforces a strict default rule: **any dependent name is assumed to be a static value, not a type.**

If the compiler assumes `Container::value_type` is a static value, it misinterprets a variable declaration as a multiplication mathematical expression (for example pointers Container::value_type a = +var; as + math). This mismatch causes compilation errors.


### The Function of the `typename` Keyword

The `typename` keyword serves as a direct instruction to the compiler.

Placing `typename` immediately before a dependent name (such as `typename Container::value_type`) signals that the identifier is strictly a data type. This enables the compiler to correctly parse variable declarations and instantiations without treating them as mathematical expressions.

### Example
1. `typename`
    ```cpp
    #include <iostream>

    // A template class that defines a nested type named 'element_blueprint'
    template<typename T>
    class Box {
    public:
        using element_blueprint = T; // Nested identifier acting as a type
        T item;
    };

    // A template function that receives the container template
    template<typename Container>
    void printItemType(const Container& box) {
        // typename is required because Container::element_blueprint depends on a template parameter
        // and functions as a type definition for creating a local variable.
        typename Container::element_blueprint stored_value = box.item;
        std::cout << "Stored Item: " << stored_value << "\n";
    }

    int main() {
        Box<int> integer_box;
        integer_box.item = 500;

        printItemType(integer_box);
        return 0;
    }
    ```
2. Static Value
    ```cpp
    #include <iostream>

    // A template class that defines a nested static constant named 'capacity_limit'
    template<typename T>
    class Warehouse {
    public:
        static const int capacity_limit = 500; // Nested identifier acting as a static value
        T inventory_item;
    };

    // A template function that uses the static value from the container template
    template<typename Container>
    void evaluateCapacity(const Container& storage) {
        // No typename is used here because Container::capacity_limit functions 
        // as a static value, not a type. It participates directly in mathematical expressions.
        int doubled_limit = Container::capacity_limit * 2;
        
        std::cout << "Calculated Capacity Limit: " << doubled_limit << "\n";
    }

    int main() {
        Warehouse<double> modern_warehouse;
        modern_warehouse.inventory_item = 10.5;

        evaluateCapacity(modern_warehouse);
        return 0;
    }
    ```

## Explicit Specialization of Class Templates

Provides a completely custom implementation for one specific set of template arguments. The specialization is used instead of generating an instantiation from the primary template.

```cpp
// Primary template — general case
template<typename T>
class Storage {
    T data;
public:
    Storage(T v) : data(v) {}
    T    get()   const { return data; }
    void print() const { std::cout << "generic: " << data << "\n"; }
};

// Full specialization for bool
template<>
class Storage<bool> {
    bool flag;
public:
    Storage(bool v) : flag(v) {}
    bool get()   const { return flag; }
    void print() const { std::cout << "bool: " << (flag ? "true" : "false") << "\n"; }
    void toggle()      { flag = !flag; }   // extra — only in specialization
};

Storage<int>  si(42);     // primary template
Storage<bool> sb(true);   // specialization
sb.toggle();               // VALID: only in specialization
// si.toggle();            // COMPILE ERROR: not in primary template
```

The specialization does not inherit anything from the primary template. It is a completely separate class definition for specific arguments.