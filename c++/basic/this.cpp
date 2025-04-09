/**
 * Author: E.K.Jithendiran
 * Date  : 9 Apr 2025
 */
// g++ this.cpp -o this.o

#include<iostream>
using namespace std;
/*
this
-----
This is pointer to current object, *this is dereferenced

*/

struct myclass{
    int a;
    myclass(int a){
        this->a = a;
        // here `int a` is parameter, this->a is current object's a
        // why -> because `this` is pointer, in class or struct we will be using -> when to dereference
        (*this).a = a; // this also valid
    }

    myclass& set(int v) {
        this->a = v;
        return *this; // returning the current object in dereferenced form
    }

    myclass& doubleVal() {
        a *= 2;
        return *this; // when current object is returned we can do chaining
    }

    void print() {
        cout << "Address : " << this << endl;
        std::cout << "a = " << a << std::endl;
    }

    void compare(myclass &other) {
        if (this == &other) // this and  &other is address
            cout << "Same object\n";
        else
            cout << "Different object\n";
    }

    static void statfun(){
        // this; // error: this is not available in static context
        cout << "Static function " << endl;
    }
};

// final
struct base final {};// struct named base is final, it can't inherit

struct final final {}; // ok

// struct ch: base{}; // error

struct parent {
    virtual void fun() final { // this function can't override
        // without virtual keyword it will throw error
        cout << "Final member function" << endl; 
    } 
    virtual void show() {
        std::cout << "Base show()\n";
    }
};

struct child final: parent { // child is final
    // void fun() { 
    //     cout << "Child fun" << endl;
    // }
    //  error: virtual function ‘virtual void child::fun()’ overriding final function

    void show() override { // correct override
        std::cout << "Derived show()\n";
    }
};

int main(){

    myclass o(123);
    o.set(2).doubleVal().print(); // chaining 
     /*
    op
    Address : 0x7fff85294344
    a = 4
    */
    myclass o1(10), o2(10);
    o1.compare(o1); // Same object
    o1.compare(o2); // Different object
}