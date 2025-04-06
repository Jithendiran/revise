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
    // virtual void non_pure(); this is not valid
    // virtual void non_pure() {} = 0; this not valid

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
    
    virtual void non_pure() = 0;

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
 

void func (Abstract &a){
    // `Abstract a` param like this won't work because, for Abstract can't create object
    a.pure_1();

}

struct concrete_ch:  Abstract
{
    void pure_1(){
        cout << "Pure one in concrete_ch\n";
    }

    void pure_2 () {
        cout << "Pure 2 in concrete_ch\n";
    }
};


int main()
{
    // Abstract a; //  error: cannot declare variable ‘a’ to be of abstract type ‘Abstract’ pure_1 and pure_2 must ober ride
    Abstract *a;
    // Abs2 a;  // error: cannot declare variable ‘a’ to be of abstract type ‘Abs2, pure_2 must override
    // Concrete c; // cannot declare variable ‘c’ to be of abstract type ‘Concrete’

    Concrete2 c;
    cout << endl;
    /*
    Abstract default cons
    Abs2 cons
    Concrete cons
    Concrete2 cons

    Concrete2 dest
    Concerete dest
    Abs2 dest
    Abstract Dest
    */
   func(c); // Pure one in Abs2

   c.pure_1(); // Pure one in Abs2
   c.pure_2(); // Pure 2 in Concrete
   c.nonpure(); // Non Pure Abstract
   c.non_pure(); // non_pure

   concrete_ch ch;

   func(ch); // Pure one in concrete_ch

//    a = new Abstract(); //  error: invalid new-expression of abstract class type ‘Abstract’
    a = new concrete_ch();

    a->nonpure(); //Non Pure Abstract
}