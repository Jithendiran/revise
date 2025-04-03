/**
 * Author: E.K.Jithendiran
 * Date  : 3 Apr 2025
 */
// g++ single_inh.cpp -o single_inh.o
#include <iostream>
using namespace std;

struct A {

    int a;
    A() {
        a = 1;
        cout << "A default const "<<endl;
    }

    A(int a) {
        this->a = a; // this is object ref
        cout << "A Param const "<<endl;
    }

    A(A& obj){
        this->a = obj.a;
        cout << "A Copy cons" << endl;
    }

    A(A&& obj){
        this->a = obj.a;
        obj.a = 0;
        cout << "A Move cons" << endl;
    }
    // A(A&&) = delete;  // this will delet the constructor, A b = A(); this step will cause error, about deleted constructor
    // A() will create temp(rvalue) object, so internally it will use move constructor
    // A o; is called move constructor won't call. so no error

    void a_method(){
        cout << "A method" << endl;
    }

    void common_method() {
        cout << "A Common" << endl;
    }

    ~A() {
        cout << "A Destructor " <<endl;
    }
};

// default acess specifier is public
struct B: A{
    int b;
    B() {
        a = 1;
        cout << "B default const "<<endl;
    }

    B(int a) : b(a), A(a) //  b(a) called before executing constructor, A(a) is calling parent parameter constructor, without A(a), A's default constructor will be called
    {
        this->a = a; // this is object ref // called during execution of constructor
        cout << "B Param const "<<endl;
    }

    B(B& obj){
        this->a = obj.a;
        cout << "B Copy cons" << endl;
    }

    B(B&& obj){
        this->a = obj.a;
        obj.a = 0;
        cout << "B Move cons" << endl;
    }

    void b_method(){
        A::a_method(); // call parent methods
        cout << "B method" << endl;
    }

    void common_method() {
        cout << "Common B" << endl;
    }

    ~B() {
        cout << "B Destructor " <<endl;
    }
};

int main(){

    // A a;            // A default const
    // a.a_method();   // A method
    // // A Destructor

    // A a0 = A(); // here  temporary object, but move construtor won't call this is because of  Return Value Optimization (RVO)
    // // Instead of creating a separate temporary and moving it, the compiler constructs the object directly in a.
    // /*
    // A default const 
    // A Destructor 
    // */

    // B b;
    // /*
    // A default const 
    // B default const 
    //  */
    // b.a_method(); // A method
    // b.b_method(); 
    // /*
    // A method
    // B method
    // */
    // b.common_method(); // Common B

    // /*
    // B Destructor 
    // A Destructor 
    // */

    // B b1(1);
    // /**
    // A Param const 
    // B Param const 
    // B Destructor 
    // A Destructor
    //  */

    //  // upcasting
    //  A ab = B(); // Object Slicing
    //  ab.a_method(); // A method
    //  ab.common_method(); // A Common // from Java point of view B's method will be called here B object is deleted 
    // //  ab.b_method(); // error: ‘struct A’ has no member named ‘b_method’; did you mean ‘a_method’?
    //  /*

    //     A default const   
    //     B default const
    //     A Move cons
    //     B Destructor 
    //     A Destructor 
    //     A method
    //     A Common
    //     A Destructor

    //     Execution
    //     1. 1st A's constructor is called
    //     2. 2nd B's constructor is called
    //     3. B temp object created and it is considered as rvalue
    //     4. when it see A ab = rvalue, since it is rvalue, Move constructor will be called, compiler perfer move bcz of temp object, why A's move method. Because 'A ab'
    //         If no move construtor, copy constructor will be called
    //     5. While moving only A portion of B is retained
    //     6. Then B object will be deleted, So B's destructor is called then A's destrutor is called
    //  */

    //  // using pointers
    //  A* ab1 = new B(); // dyna,ically create memory on heap, new operator return pointer to B*
    //  // Since it is on heap destrutor did'nt call automatically
    //  ab1->a_method();
    //  ab1->common_method();
    //  /*
    //     A default const 
    //     B default const 
    //     A method
    //     A Common
    //  */
    // // Doubt: why move constructor didn't called here

    // // since new keyword is used need to call destructor manually
    // // ab1->~B(); // error: the type being destroyed is ‘A’, but the destructor refers to ‘B’
    // // since A* is used only A's resource is available, if A's destructor is virtual then B's destructor will be called
    // ab1->~A(); // A Destructor
    // delete ab1; //A  

    /*
    static_cast is a compile-time casting operator in C++ used for type conversions that are well-defined and generally safe. It performs conversions between compatible types, such as numeric types, pointers, and references. It is fast
    */
    // A ab2 = static_cast<A>(B()); 
    /**
     A default const 
    B default const 
    A Move cons
    B Destructor 
    A Destructor 
    A Destructor 
     */

    // upcast, downcast
    B* bo = new B();
    /*
    A default const 
    B default const
    */
    A* abse = static_cast<A*>(bo); // equal to (dsttype) srctype, eg (int)3.2 in c
    bo->common_method(); // Common B
    abse->common_method(); // A Common
    abse->a_method(); //A method
    // abse->b_method(); // error
    cout << "B object arr" << bo << endl; //B object arr0x5567f87c6eb0
    cout << "A object arr" << abse << endl; // A object arr0x5567f87c6eb0

    // downcast
    B* bo1 = static_cast<B*>(abse);
    bo1->common_method(); // Common B
    bo1->b_method(); // B method
    
    // upcast
    A* dy = dynamic_cast<A*>(bo);
    dy -> common_method(); // A Common
    // dy -> b_method(); // error

    // downcast
    // B* bo2 = dynamic_cast<B*>(dy); // A does not have virtual functions, so it throws error
}