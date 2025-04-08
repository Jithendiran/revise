/**
 * Author: E.K.Jithendiran
 * Date  : 8 Apr 2025
 */
// g++ reference.cpp -o reference.o

#include<iostream>
using namespace std;

/*
:: is scope resolution operator
It is used to access names (variables, functions, types) defined in a specific scope, like:

* The global scope
* A namespace
* A class or struct
* Base class members in inheritance
* Definition
--------------- need to cover
* enum
* nested class
* template
*/

// global
int a = 10;

//---------------------------------------------------------------------------------------------------------------------
// namespace
/*
A namespace is a container for identifiers — such as variables, functions, classes — that helps avoid name conflicts.

Types of name space
* named namespace
* Anonymous namespace
* Nested namespace
* Inline namespace
*/

// named namespace
namespace cs_space {
    int a = 15;
}

namespace A {
    void print() { std::cout << "A print\n"; }
}

namespace B {
    void print() { std::cout << "B print\n"; }
}

// Anonymous namespace
// Only visible in this file (like `static` at file scope)
namespace {
    int hidden = 42;
}

// nested
namespace outer {
    namespace inner {
        void fun();
    }
}
void outer::inner::fun(){
    std::cout << "fun\n";
}

// inline
inline namespace v1 {
    void run() {
        cout << "Run inline namespace" << std::endl;
    }
}

namespace Shapes {
    class Shape {
    public:
        void draw() {
            std::cout << "Drawing a shape\n";
        }
    };

    class Circle : public Shape {
    public:
        void draw() {
            std::cout << "Drawing a circle\n";
        }
    };
}

//---------------------------------------------------------------------------------------------------------------

// struct or class
struct MyClass {
    static int val;
    int val2 = 3;
    static void display() {
        std::cout << "Inside MyClass\n";
    }
};

int MyClass::val = 5; // Definition outside

// inheritance
struct child : MyClass {

    void say() {
        MyClass::display();
        std::cout << "Derived class" << std::endl;
    }
};

// definition
class Car {
    public:
        void drive();
};
    
void Car::drive() { // linker will link with class
    std::cout << "Driving the car" << std::endl;
}
    

int main(){

    int a = 20;
    cout << "Local : " << a << endl; // Local : 20
    cout << "Global : " << ::a << endl; //Global : 10
    cout << "custom space : " << cs_space::a << endl; // custom space : 15

    A::print(); //  A print
    B::print(); //  B print
    outer::inner::fun(); // fun
    cout << "Hidden : " << hidden << endl; // 42
    run(); //Run inline namespace
    v1::run(); // Run inline namespace

    Shapes::Circle cr;
    cr.draw(); // Drawing a circle
    cr.Shapes::Shape::draw(); // Drawing a shape

    // when using namespace, mentioned namespace is included in current context, so no need to access like before
    using namespace Shapes;
    Circle cr1;
    cr1.draw();
    
    // In C++, using namespace cannot be "undone" or removed once declared in a scope.
    //But you can control its effect by carefully scoping it. 
    {
        // this namespace scope is only inside the braces 
        using namespace outer::inner;
        fun();
    }

    MyClass::display();  // Inside MyClass   // Access static function
    // MyClass::val2; //  error: invalid use of non-static data member ‘MyClass::val2’
    // only static can access by class name
    std::cout << MyClass::val << std::endl; // 5 // Access static var
    // cout can be used by both cout or std::cout
    // std::cout is specify cout from std

    child c;
    c.say();
    /*
    Op
    Inside MyClass
    Derived class
    */

    Car car;
    car.drive(); // Driving the car
}