/**
 * Author: E.K.Jithendiran
 * Date  : 20 Apr 2025
*/
// g++ lambda.cpp -o lambda.o
#include <iostream>
#include <functional>
using namespace std;

template<typename Functor>
void cstm(Functor func) {
    func();
}

template<typename T>
class Greeter {
    std::string name = "Hi";

public:
    std::function<void()> getGreetLambda() {
        return [this]() {
            // capture this
            std::cout << "Hello, " << name << "!" << std::endl;
        };
    }
};

int main() {
    /*
    syntax
    [capture](parameters) -> return_type {
    // body
    };
    // desc
    [capture]	Captures variables from surrounding scope
    (parameters)	Function parameters
    -> return_type	(Optional) specifies return type
    { body }	Function body
    */

    int x = 10;
    auto add = [](int a, int b) -> int {
        // int res = x; //  error: ‘x’ is not captured
        return a + b;
    };
    
    std::cout << add(2, 3) << endl; // Output: 5

    /*Capture modes
        [=]	Capture all by value (copy)
        [&]	Capture all by reference
        [x]	Capture only x by value
        [&x, y]	Capture x by reference, y by value
    */

    auto f = [=](){
        // x++; //  error: increment of read-only variable ‘x’
        cout << "X is : " << x << endl;
    };

    auto f0 = [=]() mutable {
        x++; 
        cout << "X is : " << x << endl;
    };
    f0(); // X is : 11
    cout<< "Outer x = " << x << endl; // Outer x = 10

    //---------------
    auto f1 = [&]()  {
        x++; 
        cout << "X is : " << x << endl;
        return 1;
    };
    f1(); // X is : 11
    cout<< "Outer x = " << x << endl; // Outer x = 11

    cstm([]() { std::cout << "Called!" << std::endl; }); // called

    
    Greeter<void> g;
    auto greetFunc = g.getGreetLambda();
    greetFunc(); // Output: Hello, Hi!

}