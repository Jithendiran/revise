/**
 * Author: Jithendiran E.K
 * Date  : 1 Apr 2025
 */
// g++ constructor.cpp -o constructor.o
#include <iostream>
using namespace std;

class A {
    // constructor can be private
    public:
        int value; 

        // types of constructor
        A() {
            value = 0;
            cout << "Default Constructor called" << endl;
        }
        A(int a) {
            value = a;
            cout << "Parameterized Constructor called" << endl;
        }

        A(const A &obj) {
            // don't change value of obj so it is const
            value = obj.value;
            // obj.value = 1; // error read only value
            cout << "Copy Constructor called" << endl;
        }
        
        A(A &&obj) {
            // after move, obj is reset so no const
            value = obj.value;
            obj.value = 0; // Reset the moved-from object
            // This is important to avoid double free or dangling reference
            cout << "Move Constructor called" << endl;
        }

        //---------------------

        A operator=(int a) {
            value = a;
              cout << "Parameterized = const" << endl;   
              return *this;
          }

        A& operator=(const A &obj) {
            if (this == &obj) {
                return *this; // Handle self-assignment
            }
            value = obj.value;
            // Clean up any existing resources if necessary
            cout << "Copy Assignment Operator called" << endl;
            return *this;
        }
        A& operator=(A &&obj) {
            if (this == &obj) {
                return *this; // Handle self-assignment
            }
            value = obj.value;
            obj.value = 0; // Reset the moved-from object
            cout << "Move Assignment Operator called" << endl;
            return *this;
        }
        //------------
        // Destructor
        ~A() {
            cout << "Destructor called" << endl;
        }
};

int main(){
    A a1; // Default constructor
    cout<< "a1 is : "<< &a1<<endl;    
    cout << "a1 value: " << a1.value << endl;

    // A a2(10); // Parameterized constructor
    // cout << "a2 is : "<< &a2<<endl;
    // cout << "a2 value: " << a2.value << endl;

    // A a3 = a2; // Copy constructor
    // cout << "a3 is : "<< &a3<<endl;
    // cout << "a3 value: " << a3.value << endl;

    // A a4 = std::move(a2); // Move constructor
    // cout << "a4 is : "<< &a4<<endl;
    // cout << "a4 value: " << a4.value << endl;

    // cout << "a2 value after move: " << a2.value << endl; // a2 is in a valid but unspecified state

    // a1 = a3; // Copy assignment operator
    // cout << "a1 is : "<< &a1<<endl;
    // cout << "a1 value after copy assignment: " << a1.value << endl;
    
    // a1 = a4; // Copy assignment operator
    // cout << "a1 is : "<< &a1<<endl;
    // cout << "a1 value after copy assignment: " << a1.value << endl;

    // a1 = std::move(a3); // Move assignment operator
    // cout << "a1 is : "<< &a1<<endl;
    // cout << "a1 value after move assignment: " << a1.value << endl;

    // a1 = A(20); // Parameterized constructor
    // cout << "a1 is : "<< &a1<<endl;
    // // a1 is now assigned a new value
    // cout << "a1 value after parameterized assignment: " << a1.value << endl;

    // Destructor will be called automatically when objects go out of scope
    // or when the program ends
    // Destructor for a1, a2, a3, a4 will be called here

    return 0;
}

 /**
     * op
Default Constructor called
a1 is : 0x7ffda07ec0c8
a1 value: 0
Parameterized Constructor called
a2 is : 0x7ffda07ec0c4
a2 value: 10
Copy Constructor called
a3 is : 0x7ffda07ec0c0
a3 value: 10
Move Constructor called
a4 is : 0x7ffda07ec0bc
a4 value: 10
a2 value after move: 0
Copy Assignment Operator called
a1 is : 0x7ffda07ec0c8
a1 value after copy assignment: 10
Copy Assignment Operator called
a1 is : 0x7ffda07ec0c8
a1 value after copy assignment: 10
Move Assignment Operator called
a1 is : 0x7ffda07ec0c8
a1 value after move assignment: 10
Parameterized Constructor called
Move Assignment Operator called
Destructor called
a1 is : 0x7ffda07ec0c8
a1 value after parameterized assignment: 20
Destructor called
Destructor called
Destructor called
Destructor called
     */