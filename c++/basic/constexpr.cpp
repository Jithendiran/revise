/**
 * Author: Jithendiran E.K
 * Date  : 11 Apr 2025
 */
// g++ constexpr.cpp -o constexpr.o
#include <iostream>
#include <stdio.h>
using namespace std;

// constexpr means a value or function can be evaluated at compile-time.

/*
It ensures:
* The function can be used in constant expressions.
    eg:  constexpr int result = square(5); 
* The function behaves like inline and must return something computable at compile-time, when value known in compile time
    eg:  square(5);
* If value is found at run time, it will be executed in run time, eg: below cin case
*/

// constexpr is used in both variable and function, where as const is only for variable
constexpr int square(int x) {
    return x * x;
}

int sq(int x){
    return x*x;
}

constexpr int cube(int x) { return x * x * x; }

constexpr void c(int a) {} // it is allowed but not a valid one

constexpr int c1(int a) {
    if(a > 0){
        return 1;
    }
    return -1;
}

// ----------------------------------------------------------------
struct Point {
    int x, y;
    mutable int  z;

    // constexpr constructor
    // constexpr constructors allow creating constexpr objects.
    constexpr Point(int x_val, int y_val) : x(x_val), y(y_val), z(2) {}

    // constexpr method
    constexpr int sum() const {
        // since function is const
        // x = 1; //  error: assignment of member ‘Point::x’ in read-only object, 
        // You can't modify members inside const functions.
        //  z = 6; // it is allowed because of mutable, but constexpr is used not allowed to modify even though it is mutable

        return x + y;
    }

    int mul() {
        return x * y;
    }

    constexpr int sub() {
        x = 6; // it will throw error when function called, error is compile time error
        return x - y;
    }

    static constexpr int square(int x) {
        return x * x;
    }
};


// mutable
struct Logger {
    mutable int count = 0;

    void log() const {
        ++count; //  allowed because `count` is mutable
        std::cout << "Logged " << count << " times\n";
    }
};


int main() {
    // constexpr int result = sq(2);  //  error: call to non-‘constexpr’ function ‘int sq(int)’
    constexpr int result = square(5); // compile-time evaluated
    std::cout << result << std::endl; // 25


    constexpr int i = square(7); // compile time
    constexpr int y = square(i); // compile time
    std::cout << "Square of 49 = " << y << std::endl;  // Square of 49 = 2401

    cout << "Enter a number: ";
    int num;
    // take integer input
    cin >> num; // 3

    // constexpr int z = square(num); //  error: the value of ‘num’ is not usable in a constant expression
    cout << "Square of input number: " << square(num) << endl; // run time
    //Square of input number: 9
    // the above square called at run time, even though square function is constexpr

    constexpr int c = cube(2);  //  Compile-time
    int x = 5;
    int r = cube(x);    // run time

    constexpr int x1 = 5;    // compile-time constant
    int r1 = cube(x1);        // compile-time execution

    //------------------------------------------------------
    constexpr Point p(3, 4); // evaluated at compile-time
    constexpr int total = p.sum();

    cout << "Sum = " << total << endl; // sum = 7

    // is this run time or compile time?
    Point *pt = new Point(5,4); // This is run time

    // constexpr Point *pt1 = new Point(4,4); // Can’t use malloc/new/IO in constexpr functions (only compile-time valid ops allowed). 

    static_assert(square(5) == 25, "Square logic is broken!");
    // static_assert(sq(5) == 25, "Square logic is broken 2!"); //  error: non-constant condition for static assertion
    
    // static_assert(square(num) == 9, "Square logic is broken!"); // error: error: non-constant condition for static assertion

    const Logger log;
    log.log();  // Logged 1 times
    log.log();  // Logged 2 times
}

// how to verify this is really compile time or not? using static_asset
/*
static_assert is a compile-time assertion. 
static_assert(condition, "Error message");
condition: Must be a constant expression (evaluated at compile time).
*/