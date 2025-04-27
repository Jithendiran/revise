/**
 * Author: Jithendiran E.K
 * Date  : 27 Apr 2025
 */
// g++ custom_pointers.cpp -o custom_pointers.o
#include <iostream>
using namespace std;

// custom pointer
/*
* Single owner
* Auto delete when goes out of scope
*/
template <typename T>
struct MyPointers{
    
    MyPointers(): ptr(new T()){
        cout << "My pointer constructor\n";
    }

    ~MyPointers(){
        cout << "Deleting My pointers\n";
        delete ptr;
    }

    T& operator*() {
        cout << "operator *" << endl;
        return *ptr;
    }

    // Arrow operator
    T* operator->() {
        cout << "operator ->" << endl;
        return ptr;
    }

    MyPointers(const MyPointers& obj) = delete;
    MyPointers& operator=(const  MyPointers&) = delete;
    private:
    T *ptr;
};

struct cls {
    void show() {
        cout << "In cls show"<<endl;
    }
};

int main(){
    MyPointers<cls> p;
    // My pointer constructor

    /*
    p is stack object and content of p is cls object which is created in heap
    Since p is stack object, when object goes out of scope stack will be cleared so content of p's object also cleared 
    */

    p->show();
    /*
    operator ->
    In cls show
    */
    (*p).show();
    /*
    operator *
    In cls show
    */

    // MyPointers<cls>p1 = p; 
    /*
    custom_pointers.cpp:63:25: error: use of deleted function ‘MyPointers<T>::MyPointers(const MyPointers<T>&) [with T = cls]’
   63 |     MyPointers<cls>p1 = p;
    */

    // Deleting My pointers
}

/**
 * To check memory leak used `valgrind` software
 * $ valgrind ./custom_pointers.o
==7057== Memcheck, a memory error detector
==7057== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==7057== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==7057== Command: ./custom_pointers.o
==7057== 
My pointer constructor
operator ->
In cls show
operator *
In cls show
Deleting My pointers
==7057== 
==7057== HEAP SUMMARY:
==7057==     in use at exit: 0 bytes in 0 blocks
==7057==   total heap usage: 3 allocs, 3 frees, 73,729 bytes allocated
==7057== 
==7057== All heap blocks were freed -- no leaks are possible
==7057== 
==7057== For lists of detected and suppressed errors, rerun with: -s
==7057== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */