/**
 * Author: E.K.Jithendiran
 * Date  : 3 Apr 2025
 */
// g++ single_inh.cpp -o single_inh.o
#include <iostream>
using namespace std;

/*
Constructor order of execution from Top most Parent (Top to bottom)
Destructor order of execution is from bottom child (Bottom to top)
*/

struct A {

    int a;
    inline static int st = 67; 
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

    virtual void virtual_common() {
        cout << "A common virtual" << endl;
    }

    // destructor can be virtual
    // constructor can't be virtual

     /**
     * Comment the non virtual destructor and un coomment the virtual destructor of A
     * when running virtual
     * 
     * Notes
     * Without virtual, only Base’s destructor would be called, causing a memory leak. dervived class is not destroyed
     */

    virtual ~A(){
        cout << "A Destructor " <<endl;
    }
    // ~A() {
    //     cout << "A Destructor " <<endl;
    // }
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

    void virtual_common() override { // override is optional
        cout << "B common virtual" << endl;
    }

    ~B() {
        cout << "B Destructor " <<endl;
    }
};


// class inherit struct

class cls: public A{
public:
    cls() {
        a = 1;
        cout << "cls default const "<<endl;
    }

    cls(int a) {
        this->a = a; // this is object ref
        cout << "cls Param const "<<endl;
    }

    cls(cls& obj){
        this->a = obj.a;
        cout << "cls Copy cons" << endl;
    }

    cls(cls&& obj){
        this->a = obj.a;
        obj.a = 0;
        cout << "cls Move cons" << endl;
    }

    void cls_method(){
        cout << "Cls method" << endl;
    }

    void virtual_common() {
        cout << "cls common virtual" << endl;
    }

    ~cls() {
        cout << "cls Destructor " <<endl;
    }
};

int main(){

    A a;            // A default const
    a.a_method();   // A method
    // A Destructor

    A a0 = A(); // here  temporary object, but move construtor won't call this is because of  Return Value Optimization (RVO)
    // Instead of creating a separate temporary and moving it, the compiler constructs the object directly in a.
    /*
    A default const 
    A Destructor 
    */

   cout << "Parent static from B class : " << B::st << endl; // Parent static from B class : 67
    B b;
    /*
    A default const 
    B default const 
     */
    b.a_method(); // A method
    b.b_method(); 
    /*
    A method
    B method
    */
    b.common_method(); // Common B

    /*
    B Destructor 
    A Destructor 
    */

    B b1(1);
    /**
    A Param const 
    B Param const 
    B Destructor 
    A Destructor
     */

     // upcasting
     A ab = B(); // Object Slicing
    /*
    what is Object Slicing
    When a derived class object is assigned to a base class object, 
    New object for base class is created from derived class, the extra data in the derived class is sliced off.
    */
     ab.a_method(); // A method
     ab.common_method(); // A Common // from Java point of view B's method will be called here B object is lost access 
    //  ab.b_method(); // error: ‘struct A’ has no member named ‘b_method’; did you mean ‘a_method’?
     /*
        A default const   
        B default const
        A Move cons

        B Destructor    // B object deleted because out of scope
        A Destructor 

        A method
        A Common
        A Destructor

        Execution
        1. 1st A's constructor is called
        2. 2nd B's constructor is called
        3. B temp object created and it is considered as rvalue
        4. when it see A ab = rvalue, since it is rvalue, Move constructor will be called, compiler perfer move bcz of temp object, why A's move method. Because 'A ab'
            If no move construtor, copy constructor will be called
        5. While moving only A portion of B is retained
        6. Then B object will be deleted, So B's destructor is called then A's destrutor is called
        here why B is deleted because B goes out of scope, no longer needed, so it is deleted
     */

     B bb0;
     A ab0 = bb0; // object slicing
     // Here bb0 is not in out of scope, no no B's destructor is called
     ab0.a_method();
     ab0.common_method(); 
     /*
    A default const 
    B default const 
    A Copy cons
    A method
    A Common
    A Destructor 
    B Destructor 
    A Destructor

    Execution
    1. As usual A's and B's constructor is called
    2. Here A's copy constructor is called because bb0 is not a rvalue, it is lvale
    3. in above case B's destructor is called, because B's object has no more business. Here B's object scope is there

    '''''''''''
    4. A' destructor is called bcz A's declared last
    5. then B's destructor is called in order of B followed by A
     */


     // using pointers
    // Pointer stores the actual address of derived, without copying anything. So no slicing is happened
     A* ab1 = new B(); // dynamically create memory on heap, new operator return pointer to B*
     // Since it is on heap destrutor did'nt call automatically
     ab1->a_method();
     ab1->common_method();
     /*
        A default const 
        B default const 
        A method
        A Common
     */
    // Doubt: why move constructor didn't called here

    // since new keyword is used need to call destructor manually
    // ab1->~B(); // error: the type being destroyed is ‘A’, but the destructor refers to ‘B’
    // since A* is used only A's resource is available, if A's destructor is virtual then B's destructor will be called
    ab1->~A(); // A Destructor
    delete ab1; //A  

    /*
    static_cast is a compile-time casting operator in C++ used for type conversions that are well-defined and generally safe. It performs conversions between compatible types, such as numeric types, pointers, and references. It is fast
    */
    A ab2 = static_cast<A>(B()); 
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
    bo1->b_method(); 
    /*
    A method
    B method
    */
    // upcast
    A* dy = dynamic_cast<A*>(bo);
    dy -> common_method(); // A Common
    // dy -> b_method(); // error

    cout << "---------------------------------\n";
    // downcast
    B* bo2 = dynamic_cast<B*>(dy); // If A does not have virtual functions, it will throws error

    // alteaset one virtual function is present so no error
    cout << "---------------------------------\n";


    // virtual
    /*
     The virtual keyword enables runtime polymorphism, meaning the function to be called is determined at runtime, not at compile time.
     This allows derived classes to override methods in the base class, and when accessed via a base pointer or reference, the correct (overridden) method is called.

     without virtual key word every decission is on compile time, so when upcast the derived class object, all methods of parent class is called
     
     When virtual keyword is added virtual table is created
     When a class has at least one virtual function, a vtable is created. The vtable stores function pointers to the overridden methods

     comparing with Java, every decission is made at run time so virtual keyword is not required, in c++ when we use virtual that time only it consider taking runtime decisson for polymorphism

     
    */

    /**
     * Comment the non virtual destructor and un coomment the virtual destructor of A
     */

    // slicing
    /*
    When you assign a derived class object to a base class object, the extra data in the derived class is sliced off. 
    Even if the base class has virtual functions, slicing happens before dynamic dispatch, so virtual cannot help.
    */
    A abvb = B();
    abvb.virtual_common(); // A common virtual

    // new
    A* abv = new B();
    abv->virtual_common(); // B common virtual
    abv->~A();
    /*
    B Destructor 
    A Destructor
    */
    

    // static
    B* bov = new B();
    // A* absv = static_cast<A*>(bov);
    // absv->virtual_common();
    // absv->~A();

    // dynamic
    A* dyv = dynamic_cast<A*>(bov);
    dyv->virtual_common(); // B common virtual
    dyv->~A();
    /*
    B Destructor 
    A Destructor
    */

    /*
    Scenario	    Slicing?	Virtual Works?	Explanation
    Base b = d;	     Yes    	 No	            Derived part is lost
    Base* bp = &d;	 No	         Yes	        Pointer keeps full object
    Base& br = d;	 No	         Yes	        Reference binds to full object


    slicing
    Slicing does not destroy or delete the original Derived object. It just creates a new Base object with only the Base parts.
    Just lose access to the derived parts in the base object.
    */


    // Reference
    B br;
    /*
    A default const 
    B default const
    */
    A& ar = br;

    ar.virtual_common(); // B common virtual
    ar.common_method(); // A Common

    /*
    B Destructor 
    A Destructor 
    */

    cls c;
    /*
    A default const 
    cls default const 
    */
    c.a_method();
}

/**
 * As a summary, for run time polymorphism like override, must provide virtual key word in base class
 * When slicing is happened, derived part's access is lost, but still in memory, so no virtual keyword will help
 * Always use pointers (Base*) or references (Base&) when working with polymorphism!
 * 
 * When using pointer or reference no new object is created, just refering to old existing object
 */