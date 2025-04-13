/**
 * Author: E.K.Jithendiran
 * Date  : 13 Apr 2025
 */
// g++ multiple.cpp -o multiple.o
#include <iostream>
using namespace std;

struct Base {
    public:

    Base() {
        cout << "Base's default constructor" << endl;
    }

    void base_method(){
        cout << "base method" << endl;
    }

    void common_method() {
        cout << "base common method" << endl;
    }

    void common_method1() {
        cout << "base common1 method" << endl;
    }

    virtual ~Base() {
        cout << "Base's  destructor" << endl;
    }
};

struct Base1 {
    public:
    
    Base1() {
        cout << "Base1's default constructor" << endl;
    }

    void base1_method(){
        cout << "base1 method" << endl;
    }

    void common_method1() {
        cout << "base1 common1 method" << endl;
    }

    void common_method() {
        cout << "base1 common method" << endl;
    }

    virtual ~Base1() {
        cout << "Base1's  destructor" << endl;
    }
};

struct Sub : Base , Base1 {
    Sub() {
        cout << "Sub's  default constructor" << endl;
    }

    void common_method1() {
        cout << "Sub common1 method" << endl;
        Base::common_method1();
        Base1::common_method1();
    }
    
    virtual ~Sub() {
        cout << "Sub's  destructor" << endl;
    }
};

// Diamond Problem & Virtual Inheritance
/*
    When parents class have common parent class, it will create a diamond shape inheritance,
    This is know as diamond problem
*/

struct A {
    public:
        A() {cout << "A's  default constructor" << endl;};
        void greet() { cout << "Hello from A\n"; }
        virtual ~A() {cout << "A's  destructor" << endl;}
        virtual void common_method() = 0;
};
    
struct B : public A {
    B() {cout << "B's  default constructor" << endl;};
    void common_method() {
        cout << "B's  common method" << endl;
    }
    virtual ~B() {cout << "B's  destructor" << endl;}
};
struct C : public A {
    C() {cout << "C's  default constructor" << endl;}
    void common_method() {
        cout << "C's  common method" << endl;
    }
    virtual ~C() {cout << "C's  destructor" << endl;}
};

// here two A's object is created for D because of B and C
// Diamond inheritance!
struct D : public B, public C {
    D() {cout << "D's  default constructor" << endl;};
    virtual ~D() {cout << "D's  destructor" << endl;}
};  

//-------------------------------
// Virtual Inheritance
struct B1 : virtual public A {
    B1() {cout << "B1's  default constructor" << endl;};
    void common_method() {
        cout << "B1's  common method" << endl;
    }
    virtual ~B1() {cout << "B1's  destructor" << endl;}
};
struct C1 : virtual public A {
    C1() {cout << "C1's  default constructor" << endl;};
    void common_method() {
        cout << "C1's  common method" << endl;
    }
    virtual ~C1() {cout << "C1's  destructor" << endl;}
};

// Since B1 and C1 are virtual inhetitance only one copy of A is created
struct D1 : public B1, public C1{
    /*  
        When D1 object being created, it will check parent's inheritance
        in parent B1 is mentioned A is base class with virtual inheritance
        since no A's object is created it will create one, since it is virtual it will share the A's object 
        Now it will check for D1's next parent it's C1. When creating C1 object, it's inherited from A class
        and it's inheritance is virtual. Since A's object is created for B, it don't create one and use the existing one  
    */
    D1() {cout << "D1's  default constructor" << endl;};

    // This is Ambiguity, so must define it's own
    void common_method() {
        cout << "D1's  common method" << endl;
    }
    virtual ~D1() {cout << "D1's  destructor" << endl;}
};

// case 2
struct B2 : virtual public A {
    B2() {cout << "B2's  default constructor" << endl;};
    void common_method() {
        cout << "B2's  common method" << endl;
    }
    virtual ~B2() {cout << "B2's  destructor" << endl;}
};
struct C2 : public A {
    C2() {cout << "C2's  default constructor" << endl;};
    void common_method() {
        cout << "C2's  common method" << endl;
    }
    virtual ~C2() {cout << "C2's  destructor" << endl;}
};

struct D2 : public B2, public C2{
    /*
    Here B2's inheritance is virtual so object will be created and shared, 
    if any other class using the same parent class with virtual inheritance it will use the existing A's object
    in C2 case, C2 also have the same parent class but it's inheritance is non virtual, so it is asking for new object for it's use
    C2 won't use shared A object
    */
    D2() {cout << "D2's  default constructor" << endl;};
    virtual ~D2() {cout << "D2's  destructor" << endl;}
};

//

struct Base0 {
    void hello() { cout << "Hello from Base\n"; }
};

struct Base01 {
    void hello() { cout << "Hello from Base1\n"; }
};

struct Sub0 : Base0, Base01 {
    using Base0::hello;  // Resolves ambiguity: now hello() means Base1::hello
};



int main() {
    Sub s;
    // Constructors: Base's -> Base1's -> Sub's
    // parent constructor called from left to right
    // Destructors: Sub's -> Base1's -> Base's
    /*
    Base's default constructor
    Base1's default constructor
    Sub's  default constructor
    */

    s.base_method(); // base method
    s.base1_method(); // base1 method

    // s.common_method(); //error: request for member ‘common_method’ is ambiguous

    // this don't throw ambiguous error, bacause sub class has it's own definition
    s.common_method1();
    /*
    Sub common1 method
    base common1 method
    base1 common1 method
    */

    // parent destructor called from  right to left
    /*
    Sub's  destructor
    Base1's  destructor
    Base's  destructor
    */

    D d; // Multiple A object is created
    /*
    A's  default constructor
    B's  default constructor
    A's  default constructor
    C's  default constructor
    D's  default constructor
    */
    // d.greet(); // error: request for member ‘greet’ is ambiguous

    // work around
    d.B::greet(); // call greet from B's A object 
    // Hello from A
    d.C::greet(); // call greet from C's A object 
    // Hello from A
    cout<<"---------------------------------------------------------------------------------"<<endl;
    // casting will work
    B* bc = static_cast<B*>(&d);
    bc->greet(); // calling from A's object   
    // Hello from A
    // dynamic_cast can use if it has vtable 
    cout<<"---------------------------------------------------------------------------------"<<endl;

    // d.common_method(); // request for member ‘common_method’ is ambiguous
    /*
    D's  destructor
    C's  destructor
    A's  destructor
    B's  destructor
    A's  destructor
    */

    D1 d1; // single A object is created
    /*
    A's  default constructor
    B1's  default constructor
    C1's  default constructor
    D1's  default constructor
    */
    d1.greet(); // Hello from A
    d1.common_method(); // D1's  common method
    /*
    D1's  destructor
    C1's  destructor
    B1's  destructor
    A's  destructor
    */

    cout << endl;
    D2 d2;
    /*
    A's  default constructor
    B2's  default constructor
    A's  default constructor
    C2's  default constructor
    D2's  default constructor
    */
    // d2.greet(); // error: request for member ‘greet’ is ambiguous
    // d2.common_method(); // error: request for member ‘common_method’ is ambiguous
    cout << endl;

    Sub0 s1;
    s1.hello(); // Calls Base0::hello()
    // Hello from Base
}