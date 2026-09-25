# Constructor and Destructor Order
## The Rule
* When a derived object is created, constructors run **base first, derived last**.
* When a derived object is destroyed, destructors run **derived first, base last**.
* Destruction is always the exact reverse of construction.

## Why This Order Exists

The derived class depends on the base class. The derived constructor may use base class members — they must already be initialized when the derived constructor body runs. Calling a base member from the derived constructor before the base is constructed would read garbage.

Destruction reverses this: the derived portion is destroyed first because it may depend on the base portion still being valid during its cleanup. The base destructor runs last — after all derived cleanup is done.

## Single Level

```cpp
class Base {
public:
    Base()  { std::cout << "Base constructor\n"; }
    ~Base() { std::cout << "Base destructor\n";  }
};

class Derived : public Base {
public:
    Derived()  { std::cout << "Derived constructor\n"; }
    ~Derived() { std::cout << "Derived destructor\n";  }
};

int main() {
    Derived d;
}
```
**Output**
```
Base constructor
Derived constructor
Derived destructor
Base destructor
```
## Multiple Levels

```cpp
class A {
public:
    A()  { std::cout << "A constructor\n"; }
    ~A() { std::cout << "A destructor\n";  }
};

class B : public A {
public:
    B()  { std::cout << "B constructor\n"; }
    ~B() { std::cout << "B destructor\n";  }
};

class C : public B {
public:
    C()  { std::cout << "C constructor\n"; }
    ~C() { std::cout << "C destructor\n";  }
};

int main() {
    C obj;
}
```
**Output**
```
A constructor
B constructor
C constructor
C destructor
B destructor
A destructor
```

## Multiple Inheritance Order
When a class inherits from multiple bases, constructors run in the order the bases are listed in the class declaration — not the order they appear in the initializer list.
```cpp
class A {
public:
    A()  { std::cout << "A constructor\n"; }
    ~A() { std::cout << "A destructor\n";  }
};

class B {
public:
    B()  { std::cout << "B constructor\n"; }
    ~B() { std::cout << "B destructor\n";  }
};

class C : public A, public B {   // A listed first, B second
public:
    C()  { std::cout << "C constructor\n"; }
    ~C() { std::cout << "C destructor\n";  }
};

int main() { C obj; }
```
**Output**
```
A constructor
B constructor
C constructor
C destructor
B destructor
A destructor
```
The initializer list order does not matter — only the declaration order. Even if written as  `: B(), A()` in the initializer list. `A` still constructs first — declaration order wins.

## Member Objects — Also Part of the Order
If the derived class has member objects of class type, those are constructed after the base class but before the derived constructor body. Destroyed in reverse — after the derived body but before the base.

```cpp
class Engine {
public:
    Engine()  { std::cout << "Engine constructor\n"; }
    ~Engine() { std::cout << "Engine destructor\n";  }
};

class Vehicle {
public:
    Vehicle()  { std::cout << "Vehicle constructor\n"; }
    ~Vehicle() { std::cout << "Vehicle destructor\n";  }
};

class Car : public Vehicle {
    Engine engine;   // member object
public:
    Car()  { std::cout << "Car constructor\n"; }
    ~Car() { std::cout << "Car destructor\n";  }
};

int main() {
    Car c;
}
```

**Output**
```
Vehicle constructor
Engine constructor
Car constructor
Car destructor
Engine destructor
Vehicle destructor
```
In C++, member objects are fully constructed before the body of the derived class's constructor begins to run.

Even though `Engine` is a member of `Car`, its construction happens outside and prior to the opening brace `{}` of the `Car` constructor. 

C++ requires all member objects to be initialized before the containing object's constructor body executes. If programmer don't explicitly initialize it in a member initializer list (e.g., `Car() : engine()`), the compiler automatically calls the default constructor of the member right here.

**Full multiple inheritance order:**
```
Construction: bases in declaration order → members → derived body
Destruction:  derived body → members (reverse) → bases in reverse declaration order
```