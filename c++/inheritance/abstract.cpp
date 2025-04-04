/**
 * Author: E.K.Jithendiran
 * Date  : 4 Apr 2025
 */
// g++ abstract.cpp -o abstract.o

#include <iostream>
using namespace std;

// This is abstract 
struct Abstract
{
    Abstract(){
        cout << "Abstract default cons\n";
    }
    virtual void pure_1() = 0;  // pure virtual // 	Must be overridden
    virtual void pure_2() = 0;  // pure
    virtual void non_pure();
    virtual void nonpure() {
        // non pure, can be over ride
        cout << "Non Pure Abstract \n";
    } 

    ~ Abstract(){
        cout << "Abstract Dest\n";
    }
        
};

struct Abs2 : Abstract{
    // This is still abstract
    Abs2() {
        cout << "Abs2 cons\n";
    }
    void pure_1(){
        cout << "Pure one in Abs2\n";
    }

    ~Abs2() {
        cout << "Abs2 dest\n";
    }
};

struct Concrete: Abs2 {
    Concrete () {
        cout << "Concrete cons\n";
    }

    void pure_2 () {
        cout << "Pure 2 in Concrete\n";
    }
    
    ~Concrete() {
        cout << "Concerete dest\n";
    }
};

struct Concrete2: Concrete{
    Concrete2(){
        cout << "Concrete2 cons\n";
    }

    void non_pure() {
        cout << "non_pure \n";
    }
   ~Concrete2() {
        cout << "Concrete2 dest\n";
    }
};
 
 
int main()
{
    // Abstract a; //  error: cannot declare variable ‘a’ to be of abstract type ‘Abstract’ pure_1 and pure_2 must ober ride
    // Abs2 a;  // error: cannot declare variable ‘a’ to be of abstract type ‘Abs2, pure_2 must override
    // Concrete c; // Linker error

    Concrete2 c;
}

/*
/usr/bin/ld: /tmp/cchi8aMY.o: warning: relocation against `_ZTV8Abstract' in read-only section `.text._ZN8AbstractD2Ev[_ZN8AbstractD5Ev]'
/usr/bin/ld: /tmp/cchi8aMY.o: in function `Abstract::Abstract()':
abstract.cpp:(.text._ZN8AbstractC2Ev[_ZN8AbstractC5Ev]+0xf): undefined reference to `vtable for Abstract'
/usr/bin/ld: /tmp/cchi8aMY.o: in function `Abstract::~Abstract()':
abstract.cpp:(.text._ZN8AbstractD2Ev[_ZN8AbstractD5Ev]+0xf): undefined reference to `vtable for Abstract'
/usr/bin/ld: /tmp/cchi8aMY.o:(.data.rel.ro._ZTV8Concrete[_ZTV8Concrete]+0x20): undefined reference to `Abstract::non_pure()'
/usr/bin/ld: /tmp/cchi8aMY.o:(.data.rel.ro._ZTV4Abs2[_ZTV4Abs2]+0x20): undefined reference to `Abstract::non_pure()'
/usr/bin/ld: /tmp/cchi8aMY.o:(.data.rel.ro._ZTI4Abs2[_ZTI4Abs2]+0x10): undefined reference to `typeinfo for Abstract'
/usr/bin/ld: warning: creating DT_TEXTREL in a PIE
collect2: error: ld returned 1 exit status
*/