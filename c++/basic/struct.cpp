/**
 * Author: Jithendiran E.K
 * Date  : 1 Apr 2025
 */
// g++ struct.cpp -o struct.o
#include <iostream>
#include<stdio.h>
using namespace std;


/**
 * Construtor call from Top to Down
 * Destructor call from Bottom to Top
 */
struct a{
    int i = 12;
    void fun(){
        cout << "In func : " << i << endl;
    }
};

struct A{
    int i = 10;
    A() {
        cout << "Default constructor called" << endl;
    }
    A(int a) {
        i = a;
        cout << "Constructor called" << endl;
    }

    ~A() {
        cout << "Destructor called" << endl;
    }

    // // opertor overloading
    // void operator<<(ostream &os, A &a) {
    //     os << "A: " << a.i;
    // }
    //  error: ‘void A::operator<<(std::ostream&, A&)’ must have exactly one argument
};

// << operator is overloaded
// ostream must passed to be reference, streams don't copy
void operator<<(ostream os, A a) {
        os << "A: " << a.i; // not overloadnig, signature not matched
}

struct A1{
    int i = 10;
    A1() {
        cout << "Default constructor called" << endl;
    }
    A1(int a) {
        i = a;
        cout << "Constructor called" << endl;
    }

    ~A1() {
        cout << "Destructor called" << endl;
    }

    // opertor overloading
    void operator<<(A1& a) { // inside struct or class one param is required
        cout << "A1: " << a.i; // It don't throw error but also it won't execute, bcz operator<< must required 2 params
    }
};

struct B{
    int i = 20;
    B() {
        cout << "B constructor called" << endl;
    }

    B(int a) {
        i = a;
        cout << "B param constructor called" << endl;
    }

    ~B() {
        cout << "B destructor called" << endl;
    }
};

// This expect b << cout 
// void operator<<(B& b, ostream& os) {
//     os << "B: " << b.i;
// }

// this expect cout << b
void operator<<(ostream& os, B b) {
    os << "B: " << b.i;
}

struct C{
    int i = 2;

    C() {
        cout << "C constructor" << endl;
    }
    ~C() {
        cout << "C destructor" << endl;
    }
};

ostream& operator<<(ostream& os, C& c) {
    // if 2nd para of C& c is written as C c, it cause unnecessary copies and print destructor or other printing
    os << "C: " << c.i;
    return os;
}

struct D {
    private:
    int a;
};

// struct support inheritance
struct c: public a, private D
{

};


struct M{
    int i;
    const float f;
    M() :i(1), f(2.9) // :i (1)  member initializer, It is used to initialize member variables before the constructor body executes. this->i(1) will throw error because, no object is created at that time
    { //this->i = 1; // here this can be used because object is created, now assigning the member values
    }

    /**
     * 
    const members -> const int i; must be initialized at declaration.

    Reference members -> int& ref; must be initialized at declaration.

    Objects -> To avoid extra default construction.


     */
    M(int i, float f): f(f){ 
        this->i = i; // this points to current object
        (*this).i = i; // this can work
        // this is always pointer in c++, so this.i will not work
    }

    M (M &&obj): f(obj.f){
         // // without constructor, move constructor throws error, bcz required valid way to initilize the value
        // any one constructor is required
        // obj is rvalue, meaning it don't copy value to this stack space instead refering 
        
        // reseting of f is not possible because of const
        cout << "Move cons of M" << endl;
        i = obj.i;
        obj.i = 0;
    }
};

ostream& operator<<(ostream& os, M& m) {
    os << "M: i: " << m.i << " f : " << m.f;
    return os;
}


int main() {

    struct a aa;

    printf("%d\n", aa.i); // 12
    aa.fun(); // In func : 12

    struct a aa1 = {1};
    printf("%d\n", aa1.i); // 1
    aa1.fun(); // In func : 1
    // aa1->fun(); // error: base operand of ‘->’ has non-pointer type ‘a’

    //----------------------------

    struct a *ap; // ap is a dangling pointer (it points to an undefined memory location).
    printf("%d \n",ap->i); //1994462232
    printf("%d \n",(*ap).i); //1994462232
    ap->fun(); // In func : 1994462232

    ap = &aa;
    printf("%d \n",ap->i); //12
    printf("%d \n",(*ap).i); //12
    ap->fun(); // In func : 12
    //---------------------------------

    // struct a &ar = ap// error
    struct a &ar = *ap;
    printf("%d \n",ar.i); //12
    ar.fun(); // In func : 12

    //-----------------------------------

    // size of struct
    printf("Size of struct %d and object %d\n", sizeof(struct a), sizeof(aa)); // Size of struct 4 and object 4

    //------------------------------------

    A a1; // Default constructor
    // cout << a1; // error: use of deleted function ‘std::basic_ostream<_CharT, _Traits>::basic_ostream(const std::basic_ostream<_CharT, _Traits>&) [with _CharT = char; _Traits = std::char_traits<char>]’
    // cout << a1; // no overload function
    cout << &a1 << endl; // 0x7ffcae2819a4 // overload not called, printed address
    cout<< endl;

    A1 a2(20); // Constructor constructor
    // cout << a2; //no overload function
    cout << &a2; // 0x7ffd59344bf8, just addr
    cout<< endl;

    B b1; // B constructor called
    cout << b1; // B: 20B destructor called // why 'B destructor called' printed, because operator<< function's 2nd param is non reference so it's behaviour is undefined
    // cout << b1 << endl;  //  error: invalid operands of types ‘void’ and ‘<unresolved overloaded function type>’ to binary ‘operator<<’
    cout<< endl;

    B b2(30); // B param constructor called
    cout << b2; //B: 30B destructor called
    cout<< endl;

    // To print multiple things in same line our operator<< should return ostream
    C c1; //C constructor
    cout << c1 << endl; // C: 2

    D d;
    // cout << d.a << endl; // error: int D::a’ is private within this context 

    M m(27,1.0f);
    cout << m << endl; // M: i: 27 f : 1

    M m1 = std::move(m); // Move cons of M
    cout << m1 << endl; // M: i: 27 f : 1
    cout << m << endl; // M: i: 0 f : 1

    /*
    C destructor
    B destructor called
    B destructor called
    Destructor called
    Destructor called
    */
    return 0;
}
