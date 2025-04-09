/**
 * Author: E.K.Jithendiran
 * Date  : 8 Apr 2025
 */
// g++ enum.cpp -o enum.o

#include<iostream>
using namespace std;
/*
An enum (short for enumeration) is a user-defined type that consists of named integral constants.
It helps you assign readable names to sets of values, usually integers.
*/
enum Fruit { APPLE, MANGO };
// non type safety
// APPLE = 0, MANGO =1
enum Color { RED, GREEN, BLUE };
/*
RED = 0
GREEN = 1
BLUE = 2
*/
enum Status { OK = 200, NOT_FOUND = 404, ERROR = 500 };

enum Animal { DOG, CAT };

// enum class 
enum class Color1 { RED, GREEN, BLUE };
// it's scope is inside enum name
// type safety

// use custom type
enum class Status1 : uint8_t {
    OK = 1, FAIL = 2
};
int main() {

    Color c = GREEN;
    Fruit f = APPLE;
    Animal a = DOG;

    // f = DOG;
    cout << c << endl; // 1
    cout << f << endl; // 0
    cout << a << endl; // 0
    // f = DOG;  //  error: cannot convert ‘Animal’ to ‘Fruit’ in assignment
    Color1 c1 = Color1::RED;
    int z = Color::RED;
    // int x = Color1::RED; //  error: cannot convert ‘Color1’ to ‘int’ in initialization
    int x = static_cast<int>(Color::RED);

    switch (c1)
    {
    case Color1::RED:
        cout << "Red" << endl;
        break;
    case Color1::GREEN:
        cout << "GREEN" << endl;
        break;
    
    default:
        break;
    }
    /*
    Red
    */

}