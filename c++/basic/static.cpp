/**
 * Author: E.K.Jithendiran
 * Date  : 8 Apr 2025
 */
// g++ static.cpp -o static.o

#include<iostream>
using namespace std;
int static hidvar = 0; // it's life time is end of program, but only visible to this file
// global static is initilized, when program starts

// static in filescope
static void filescope(){
    std::cout << "Only visible inside this file\n";
}

// static in function

void count_static() {
    static int c = 0;
    // It is initialized only once and lives for the entire program lifetime, but is visible only inside the function.
    // init happened at 1st time func call
    c++;
    std::cout << "count_static : " << c << std::endl;
}

void count_static_2() {
    static int c = 0; // above function static var is different from this
    c++;
    std::cout << "count_static_2 : " << c << std::endl;
}

// static class member
struct static_cls_1 {
    // static function can't access non-static (object) members, but object member's can access static members
    // static is initilized when 1st time class used
    int x = 4;
    static int cls_member; // = 0; //error: a member with an in-class initializer must be constC/C++
    // the above is because, only object member's can assign inside struct or class

    // const and constexpr is valid
    const static int cls_member_1 = 1; 
    constexpr static int cls_member_2 = 2;

    inline static int obj_member = 1; // when defined inline no need to define outside
    static_cls_1(){
        cls_member++;
    }

    static void static_func(){
        // x++; // error: non static is accessed within static reference
        cout << "Static function" << endl;
    }

    void obj_fun(){
        // object reference can access static
        cout << "x = " << x << endl;
        static_func();
        cout << "obj_member = " << obj_member << endl;
    }
};
// static is accessed by class name followed by reference (::) operator

int static_cls_1::cls_member = 0; // without this line, linker will cause undefined reference to cls_member variable

int main(){
    count_static(); // count_static : 1
    count_static(); // count_static : 2
    count_static_2(); // count_static_2 : 1

    // cls member
    static_cls_1 a0, a1, a2;
    cout << "static_cls_1 : " << static_cls_1::cls_member<<endl; // static_cls_1 : 3

    a0.static_func(); // Static function
    // this is possible in c, bcz compiler knows that function is static
    // object is not used here

    static_cls_1::static_func(); // Static function
    // this is preffered

    // static_cls_1::x ;//error: invalid use of non-static data member ‘static_cls_1::x’
}

/*
static_cls_1
         ┌──────────────────────────────┐
         │         Static Area          │
         ├──────────────────────────────┤
         │ cls_member = 3               │   shared, updated in constructor
         │ cls_member_1 = 1             │   const, inline
         │ cls_member_2 = 2             │   constexpr, compile-time constant
         │ obj_member = 1               │   inline static (since C++17)
         └──────────────────────────────┘

         ┌────────────┐ ┌────────────┐ ┌────────────┐
         │ Object a0  │ │ Object a1  │ │ Object a2  │
         ├────────────┤ ├────────────┤ ├────────────┤
         │ x = 4      │ │ x = 4      │ │ x = 4      │   Each object has its own `x`
         └────────────┘ └────────────┘ └────────────┘

*/