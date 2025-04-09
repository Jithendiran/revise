/**
 * Author: E.K.Jithendiran
 * Date  : 9 Apr 2025
 */
// g++ nestedclass.cpp -o nestedclass.o

#include<iostream>
using namespace std;

struct Outer {
    public:
        inline static int pu = 1;
        Outer() {
            cout << "Outer cons\n";
        }
        void outerfunction() {
            cout << "Inside outer class\n";
        }
        void access_inner() {
            // inner class variable is not accessible
            // cout<< "Inner pu "<< inpu << endl;
            // cout<< "Outer po "<< inpo << endl;
            // cout<< "Outer pr "<< inpr << endl;
            Inner in; // Inner object creation
            cout<< "Inner pu "<< in.inpu << endl;
            // cout<< "Outer po "<< in.inpo << endl;
            // cout<< "Outer pr "<< in.inpr << endl;
        }
        struct Inner {
            public:
                inline static int inpu = 11;
                Inner() {
                    cout << "Inner cons\n";
                }
                void show() {
                    std::cout << "Inside Inner class\n";
                }
                void access_outer(){
                    cout<< "Outer pu "<< pu << endl;
                    cout<< "Outer po "<< po << endl;
                    cout<< "Outer pr "<< pr << endl;

                    Outer ot; // Outer object creation
                    ot.outerfunction();
                }
                ~Inner() {
                    cout << "Inner dest\n";
                }
            protected:
                inline static int inpo = 12;
            private:
                inline static int inpr = 13;
        };
        ~Outer() {
            cout << "Outer dest\n";
        }
    protected:
        inline static int po = 2;
    
    private:
        inline static int pr = 3;
        struct PrivateInn {};
};

struct Outer1{
    // error: a storage class can only be specified for objects and functions
    // static class is not available in c++
    // static is usable only for storage class specifier like (int, void,..), not for class or struct declaration or definition.
    // static struct inner {
    // };
};

int main(){
    // to access inner class
    // Inner in; // error
    // Outer::PrivateInn; // error not access able
    Outer::Inner inobj; // object created only for inner class
    /*
    op
    Inner cons
    */
    inobj.show();
    // op: Inside Inner class
    // Inner dest

    // inobj.outerfunction(); // error: ‘class Outer::Inner’ has no member named ‘outerfunction’

    inobj.access_outer();
    /*
    OP:
    Outer pu 1
    Outer po 2
    Outer pr 3
    Outer cons
    Inside outer class
    Outer dest
    */
   // from inner outside obj created and deleted when scope end

    Outer otobj;
    // op Outer cons
    otobj.outerfunction();
    // OP: Inside outer class
    // impossible to access inner class members with outer class obj
    otobj.access_inner();
    /*
    Op:
    Inner cons
    Inner pu 11
    Inner dest
    */
   // from outer inner object created and deleted when scope ends
}

/*
Summary
Outside class to create inner object need to use (::) reference operator
Outer::Inner inobj;

Outer class
    From Outer class object creation of inner class is direct `Inner in;` no need to (::) reference operator, when scope end destructor will be called automatically
    From Outer class can access only Inner class static public members without object

Inner class
    From Inner class object creation of outer calss is direct `Outer ot;` no need to (::) reference operator, when scope end destructor will be called automatically
    From Inner class can access all member (public, protected and private) of Outer class static members, without object. This is due to Inner class is inside outer
*/