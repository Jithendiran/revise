/**
 * Author: E.K.Jithendiran
 * Date  : 13 Apr 2025
 */
// g++ multilevel.cpp -o multilevel.o
#include <iostream>
using namespace std;

struct A {

    A() {
        cout << "A's default constructor" << endl;
    }

    void a_method() {
        cout << "A's method" << endl;
    }

    virtual void common_method(){
        cout << "A's common method" << endl;
    }

    virtual ~A() {
        cout << "A's default destructor" << endl;
    }
};

struct B:A {

    B() {
        cout << "B's default constructor" << endl;
    }

    void b_method() {
        cout << "B's method" << endl;
    }

    virtual void common_method(){
        cout << "B's common method" << endl;
    }

    virtual ~B() {
        cout << "B's default destructor" << endl;
    }
};

struct C:B {

    C() {
        cout << "C's default constructor" << endl;
    }

    void c_method() {
        cout << "C's method" << endl;
    }

    virtual void common_method(){
        cout << "C's common method" << endl;
        B::common_method();
        A::common_method();
    }

    virtual ~C() {
        cout << "C's default destructor" << endl;
    }
};

int main() {
    C c;
    /*
    A's default constructor
    B's default constructor
    C's default constructor
    */
    // c.a_method(); // A's method
    // c.b_method(); // B's method
    // c.c_method(); // c.c_method
    // c.common_method();
    /*
    C's common method
    B's common method
    A's common method
    */

   

    A ac = c;
    ac.common_method(); // A's common method 
    // object slicing

    // A's default destructor

    A* apc = new C();
    /*
    A's default constructor
    B's default constructor
    C's default constructor
    */
    apc->a_method(); // A's method
    apc->common_method();
    /*
    C's common method
    B's common method
    A's common method
    */
    // apc->~A(); this is not recommanded 
    delete apc; // This is recommanded
    /*
    C's default destructor
    B's default destructor
    A's default destructor
    */
    /*
    C's default destructor
    B's default destructor
    A's default destructor
    */
}