/**
 * Author: E.K.Jithendiran
 * Date  : 14 Apr 2025
 */
// g++ exception.cpp -o exception.o
#include <iostream>
using namespace std;
/*
std::exception
    The parent class of all C++ exceptions.
std::runtime_error
    Run time exception
std::bad_alloc
    Thrown when a dynamic memory allocation fails.
std::bad_cast
    Thrown by C++ when an attempt is made to perform a dynamic_cast to an invalid type.
std::bad_exception
    Typically thrown when an exception is thrown and it cannot be rethrown.
*/

// custom exception
struct MyError {
    string msg;
    MyError(string m) : msg(m) {}
};

// custom exception inherited the exception class
struct MyError2 : public std::exception {
    string msg;
    MyError2(string m) : msg(m) {}
};

// telling the compiler: I promise, this function will not throw any exceptions
// Performance boost
void safeFunc() noexcept {
    // no exceptions allowed here
    cout << "Safe func" << endl;
    // the program terminates immediately 
    // throw 1; 
}

int main() {
    int index = 1; // 4, 1, -1
    try
    {
        safeFunc();
        cout << "In Try \n";
        if (index >= 4)
            throw "Error: string!"; 

        if (index < 0)
            throw 0;

        if(index == 1) 
            throw MyError("something something");
    }
    // catch custom exception, when throw in string
    catch (const char* msg) {
        cout << msg << endl;
    }
    // catch custom exception, when throw in number
    catch (int num) {
        cout << "Error: number " << num << endl;
    }
    
    catch(const std::exception& e)
    {
        cout << "In error: exception\n";
        std::cerr << e.what() << '\n';
    }

    catch (MyError& e) {
        cout << "Custom Error: " << e.msg << endl;
    }

     // catch any other exception
     // this always should be in last
     catch (...) {
        cout << "Unexpected exception!" << endl;
    }
    
}