/**
 * Author: E.K.Jithendiran
 * Date  : 13 Apr 2025
 */
// g++ type.cpp -o type.o
#include <iostream>
using namespace std;

// old way of type defining
typedef unsigned int uint;
uint a = 1;

// new way of type defining
using uint1 = unsigned int;
uint1 b = 2;
// using is easier to read

// auto lets the compiler deduce the type from the expression.
auto a1 = 5;       // int
auto b1 = 3.14;    // double
auto c1 = "hello"; // const char*

// declare type
int x = 10;
decltype(x) y = 20; // y is int

int a2 = 5, b2 = 10;
decltype(a2 + b2) result; // result is int (type of a+b)

// c++ 20 feature
auto add(auto n1, auto n2)
{
    return n1 + n2;
}

// operator overload
/*
Some operators cannot be overloaded:

:: (scope resolution)

. (member access)

.* (pointer-to-member)

?: (ternary operator)

sizeof, typeid, alignof
*/

class Point
{
    int x, y;

public:
    Point(int a, int b) : x(a), y(b) {}

    // Overload +
    Point operator+(const Point &p)
    {
        // x is current object
        return Point(x + p.x, y + p.y);
    }

    // Overload ==
    bool operator==(const Point &p)
    {
        return x == p.x && y == p.y;
    }

    // Overload << (stream output)
    // why ostream is friend?
    /*
    Because operator<<'s first argument is ostream& — not this class!
    */
    friend ostream &operator<<(ostream &os, const Point &p)
    {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

int main()
{
    cout << "Int add : " << add(1, 2) << endl;           // Int add : 3
    cout << "Float add : " << add(1.23f, 3.56f) << endl; // Float add : 4.79


    Point p1(2, 3), p2(4, 5);

    Point p3 = p1 + p2; // Calls overloaded +

    cout << "p3: " << p3 << endl; // Calls overloaded << // p3: (6, 8)
    cout << "p1 == p2? " << (p1 == p2 ? "Yes" : "No") << endl; // p1 == p2? No
}