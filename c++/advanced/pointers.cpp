/**
 * Author: E.K.Jithendiran
 * Date  : 14 Apr 2025
 */
// g++ pointers.cpp -o pointers.o
#include <memory>
#include <iostream>
using namespace std;

struct MyClass {
    MyClass() { cout << "MyClass created\n"; }
    ~MyClass() { cout << "MyClass destroyed\n"; }
};

// circular ref
struct B; // forward declaration

struct A {
    shared_ptr<B> b_ptr;
    ~A() { cout << "A destroyed\n"; }
};

struct B {
    // uncomment shared_ptr, comment weak_ptr and check for destructor call for A and B and vice versa
    // shared_ptr<A> a_ptr; // this will cause circular problem 
    weak_ptr<A> a_ptr; // doesn't increase ref count, so when object goes out of scope destructor will called
    ~B() { cout << "B destroyed\n"; }
};

struct Base {
    virtual ~Base() {
        cout << "Base destroyed\n";
    }
};

struct Derived : Base {
    ~Derived() {
        cout << "Derived destroyed\n";
    }
};

// only smart pointer object creation
struct Foo : std::enable_shared_from_this<Foo> {
    void print() {
        auto self = shared_from_this(); // get shared_ptr<Foo> from `this`
        cout << "Inside print: use_count = " << self.use_count() << endl;
    }
};

int main() {
    // raw pointers
    int* ptr = new int(5);
    int* ptrdup = ptr; // 2nd owner
    *ptrdup = 1;
    cout << *ptr << endl;  //  1
    delete ptr;            // manual cleanup
    // delete ptrdup;      // error: because ptr already freed the memory

    /*
    Problems
    * manually call delete
    * Easy leak memory
    * Multiple owner
    */

    // Smart Pointers — C++11
    //------------------------

    // unique pointers at a time only one owner is available
    std::unique_ptr<int> uptr = std::make_unique<int>(10);
    cout << *uptr << endl; // 10

    std::unique_ptr<MyClass> ptr1 = std::make_unique<MyClass>();
    //Op: MyClass created

    // std::unique_ptr<MyClass> ptr2 = ptr1;  //  Not allowed (copying is not allowed)
    std::unique_ptr<MyClass> ptr2 = std::move(ptr1); // allowed
    if (!ptr1)
        cout << "ptr1 no longer owns the object.\n"; // ptr1 no longer owns the object.
    
    // delete &ptr1; // don't work like normal delete
    // destructor: MyClass destroyed
    /*
    Advantage
    * Auto-deletes when uptr goes out of scope
    * Not copyable, only moveable
    */

    // shared pointers, multiple pointers shares ownership
    std::shared_ptr<int> sp1 = std::make_shared<int>(20);
    std::shared_ptr<int> sp2 = sp1;  // shared
    *sp2 = 2;
    cout << *sp1 << " " << *sp2 << endl; // 2 2
    cout << "shared_ptr use_count: " << sp1.use_count() << " " <<  sp2.use_count() << endl; // shared_ptr use_count: 2 2
    // sp1 and sp2 are refered to one memory location, that has two owners so, that memory reference count is 2
    // when reference count becomes 0 mean, it is out of scope and it will be freed
    // delete sp1;  // don't work like normal delete

    auto a = make_shared<A>();
    auto b = make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a; // Circular reference
    // Memory leak: A and B are never destroyed, Reference counts never drop to 0, Destructors not called
    // here reference count of a is 1 and b is 1, a only free when b release and b only free when a release. It is like dead lock
    /*
    Advantage
    * Deleted when last reference dies.
    */
    /*
    Disadvantage
    * circular reference
    */
    /*
     Two way to avoid circular reference
      1. after work complete clear one pointer manually, like a->b_ptr = nullptr;
      2. use weak_ptr, no need of mauanl clear
    */
//    a->b_ptr = nullptr; // manual clear 
    
    // weak_pointer
    /*
    weak_ptr don't own thw memory, So it won't increase the reference 
    It only observe the object
    Since it don't own, before use of the weak_ptr must need to lock the object and then do the function 
    */
   // correct usage is

    shared_ptr<int> sp = make_shared<int>(42);
    weak_ptr<int> wp = sp; // doesn't increase ref count

    cout << "shared_ptr use_count: " << sp.use_count() << endl; // shared_ptr use_count: 1
    cout << "weak_ptr expired? " << wp.expired() << endl;  // weak_ptr expired? 0

    if (auto locked = wp.lock()) {
        cout << "Value: " << *locked << endl; // Value: 42
    }
    sp.reset(); // now object is destroyed
    cout << "weak_ptr expired? " << wp.expired() << endl;       // weak_ptr expired? 1

    if (auto locked = wp.lock()) {
        cout << "Value: " << *locked << endl;
    } else {
        cout << "Object no longer exists\n"; // Object no longer exists
    }

    // object creation 
    std::unique_ptr<Base> p = std::make_unique<Derived>();
    // without virtual keyword, only base class destructor is called

    //p------------------------------------------------------------------
    auto f = std::make_shared<Foo>();
    cout << "Outside: use_count = " << f.use_count() << endl; // Outside: use_count = 1
    f->print(); // Inside print: use_count = 2
    cout << "After print: use_count = " << f.use_count() << endl; // After print: use_count = 1

    Foo* raw = new Foo();
    // raw->print();
    /*
    error
    terminate called after throwing an instance of 'std::bad_weak_ptr'
    what():  bad_weak_ptr
    Aborted (core dumped)
    */
}