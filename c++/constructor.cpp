/**
 * Author: Jithendiran E.K
 * Date  : 1 Apr 2025
 */
// g++ constructor.cpp -o constructor.o
#include <iostream>
using namespace std;
/**
1. Stack vs Heap Memory
Stack Allocation: Memory allocated on the stack is automatically managed by the compiler.

Heap Allocation: Memory allocated on the heap must be manually deallocated using delete.

1. new is used to create in heap


 */
class A {
    // constructor can be private
    public:
        int value; 

        // types of constructor
        // when Lvalue has Class_name obj below constructor is called 
        // new object creation, so constructor is called
        A() {
            // called: A obj
            value = 0;
            cout << "Default Constructor called" << endl;
        }
        A(int a) {
            // called: A obj(1)
            value = a;
            cout << "Parameterized Constructor called" << endl;
        }

        A(A &obj) {
            // called: A o; A o1 = a;
            value = obj.value;
            cout << "Copy Constructor called" << endl;
        }
        
        A(A &&obj) {
            // called: A o; A o1 = std::move(o)
            // after move, obj is reset so no const
            value = obj.value;
            obj.value = 0; // Reset the moved-from object
            // This is important to avoid double free or dangling reference
            cout << "Move Constructor called" << endl;
        }

        //---------------------
        // When value is assigned to existing object below operator over loading is called
        // no new object is created, object value is changing
        A& operator=(int a) {
            // A obj; obj = 1; // like asigning
            value = a;
              cout << "Parameterized = " << endl;   
              return *this;
        }

        // A& operator=(int a, int b){ // error:  = must have exactly one argument
        //     return *this;
        // }

        A& operator=(A &obj) {
            //called: A o, o1(1); o = o1 // like copying
            if (this == &obj) {
                return *this; // Handle self-assignment
            }
            value = obj.value;
            // Clean up any existing resources if necessary
            cout << "Copy = called" << endl;
            return *this;
        }
        A& operator=(A &&obj) {
            // called: A o, o1; o = std::move(o1) 
            if (this == &obj) {
                return *this; // Handle self-assignment
            }
            value = obj.value;
            obj.value = 0; // Reset the moved-from object
            cout << "Move = called" << endl;
            return *this;
        }
        //------------
        // Destructor
        ~A() {
            cout << "Destructor called" << endl;
        }
};

A* createA() {
    cout << "factory called" << endl;
    return new A();
}

int main(){
    A a1; // Default constructor
    cout<< "a1 is : "<< &a1<<endl;    // a1 is :0x7ffc3cc5db78
    cout << "a1 value: " << a1.value << endl; //a1 value: 0

    cout << endl;

    A a2(10); // Parameterized constructor
    cout << "a2 is : "<< &a2<<endl; // a2 is : 0x7ffc3cc5db74
    cout << "a2 value: " << a2.value << endl; //a2 value: 10

    cout << endl;

    A a3 = a2; // Copy constructor
    cout << "a3 is : "<< &a3<<endl; // a3 is : 0x7ffc3cc5db70
    cout << "a3 value: " << a3.value << endl; //a3 value: 10

    cout << endl;

    A a4 = std::move(a2); // Move constructor
    cout << "a4 is : "<< &a4<<endl; // a4 is :0x7ffc3cc5db6c
    cout << "a4 value: " << a4.value << endl; // a4 value: 10
    cout << "a2 value after move: " << a2.value << endl; // a2 value after move: 0
    // a2 is in a valid but unspecified state

    cout << endl;
    // delete &a4; // Destructor called
    // segment fault
    cout << endl;

    A a5; // Default Constructor called
    cout << "a5 is : "<< &a5<<endl; //a5 is :0x7ffedeb604f4
    cout << "a5 value : " << a5.value << endl; //a5 value : 0
    a5 = 1; // Parameterized = 
   
    cout << "a5 is : "<< &a5<<endl; //a5 is :0x7ffedeb604f4
    cout << "a1 is: " << a5.value << endl; //a1 is: 1

    cout << endl;

    a1 = a3; // Copy = called
    cout << "a1 is : "<< &a1<<endl; //a1 is : 0x7ffc3cc5db78
    cout << "a1 value after copy assignment: " << a1.value << endl; //a1 value after copy assignment: 10
    
    cout << endl;

    a1 = std::move(a3); // Move = called
    cout << "a1 is : "<< &a1<<endl; // a1 is : 0x7ffc3cc5db78
    cout << "a1 value after move assignment: " << a1.value << endl; //a1 value after move assignment: 10

    cout << endl;

    //--------------------------------------------
    A a6 = A(); // Default Constructor called

    cout << endl;

    A a7 = A(12); // Parameterized Constructor called
    cout << endl;

    a1 = A(20); //Parameterized Constructor called
    // Move = called
    // Destructor called
    cout << "a1 is : "<< &a1<<endl; //a1 is :0x7ffc3cc5db78
    // a1 is now assigned a new value
    cout << "a1 value after parameterized assignment: " << a1.value << endl; // a1 value after parameterized assignment: 20

    cout << endl;

    //----------------------------
    // custom memory alloction
    // this is allocated in stack, forcing the compiler to allocate 
    char buffer[sizeof(A)]; // allocating some size
    A* obj = new (buffer) A; //Default Constructor called
    // Constructs `A` in pre-allocated memory
    obj->value;
    // obj.value; //error
    cout << "Obj : " << &obj << endl; // Obj : 0x7ffe4f242d10
    obj->~A();                // Manually call destructor

    cout << endl;

    // allocated in heap
    A* obj1 = createA();
    cout << "Obj 1 : " << &obj1 << endl; // Obj : 0x7fff5443c1d0
    /*
    factory called
    Default Constructor called
    */
    delete obj1; // Destructor called
    obj1->~A(); // // Destructor called
    cout << endl;

    // this is dynamic object, created in heap
    A* obj2 = new A(); // Default Constructor called
    obj->~A(); // Destructor called
    delete obj2; //// Destructor called
    cout << endl;

    // Destructor will be called automatically when objects go out of scope
    // or when the program ends
    // Destructor for a1, a2, a3, a4 will be called here

    return 0;
}