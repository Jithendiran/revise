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
    // virtual void non_pure() {} = 0; this not valid

    virtual void nonpure() {
        // non pure, can be over ride
        cout << "Non Pure Abstract \n";
    } 

    ~ Abstract(){
        cout << "Abstract Dest\n";
    }
        
};
void Abstract::non_pure() {
    std::cout << "Abstract non_pure implementation\n";
}
//------------------

struct Abstract2
{
    Abstract2(){
        cout << "Abstract2 default cons\n";
    }
    virtual void pure_1() = 0;  // pure virtual // 	Must be overridden

    virtual ~ Abstract2(){
        cout << "Abstract Dest2\n";
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
    concrete_ch() {
        cout << "concrete_ch cons\n";
    }
    void pure_1(){
        cout << "Pure one in concrete_ch\n";
    }

    void pure_2 () {
        cout << "Pure 2 in concrete_ch\n";
    }
    ~concrete_ch() {
        cout << "concrete_ch des\n";
    }
};

struct concrete_ch2:  Abstract2
{
    concrete_ch2() {
        cout << "concrete_ch2 cons\n";
    }
    void pure_1(){
        cout << "Pure one in concrete_ch2\n";
    }

    ~concrete_ch2() {
        cout << "concrete_ch2 des\n";
    }
};

struct concrete_ch3:  concrete_ch2
{
    concrete_ch3() {
        cout << "concrete_ch3 cons\n";
    }

    ~concrete_ch3() {
        cout << "concrete_ch3 des\n";
    }
};

//-----------------------------------
// in btw virtual
struct st1{
    st1() {
        cout << "st1 cons\n";
    }

    ~st1() {
        cout << "st1 dest\n";
    }
}; // no vtable is created for s1

struct st2: st1{
    st2() {
        cout << "st2 cons\n";
    }

    virtual ~st2() {
        cout << "st2 dest";
    }
};

struct st3:st2 {
    st3() {
        cout << "st3 cons\n";
    }

    ~st3() {
        cout << "st3 dest\n";
    }
};
//----------------------------------

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

    cout << "Delete a\n\n";
    delete a;
    /*
    Abstract Dest
    */
   // when deleting a object using base class pointer, derived class destructor is not getting called without virtual destructor
    // in above case parent class destructor is non virtual so, derived class is not getting called
   cout << "delete a completed \n\n";

   Abstract2 *a2 = new concrete_ch2();
   /*
    Abstract2 default cons
    concrete_ch2 cons
   */

   cout << "Delete ab\n\n";

   delete a2;
   /*
    concrete_ch2 des
    Abstract Dest2
   */
  // here parent class's destructor is virtual

   cout << "Deleted a2\n\n";

   a2 = new concrete_ch3();
   /*
   Abstract2 default cons
    concrete_ch2 cons
    concrete_ch3 cons
   */

   cout << "Deleting a2\n\n";
   delete a2; // delete from bottom child to top parent with Top most parent's destructor 
   /*
    concrete_ch3 des
    concrete_ch2 des
    Abstract Dest2
   */
   cout << "Deleted a2\n\n";

   st1 *s3 = new st3();
   /*
    st1 cons
    st2 cons
    st3 cons
   */
   cout << "Deleting s3\n\n";

   delete s3; // st1 dest
   // Here st1 has no virtual destructor  so st2 and st3 is not deleted, cause memory leak
   cout << "Deleted s3\n";

   st1 *s2 = new st2();
   /*
    st1 cons
    st2 cons
   */
   cout << "delete st2\n";
   delete s2; // st1 des
   // even st2 is virtual destructor but base st1 is non virtual destructor, since st1's reference is used st1 is not deleted
   cout << "deleted st2\n\n";
}