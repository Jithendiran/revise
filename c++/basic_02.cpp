/**
 * Author: Jithendiran E.K
 * Date  : 1 Apr 2025
 */
// g++ basic_02.cpp -o basic_02.o
#include <iostream>
#include <stdio.h>
using namespace std;

void fun(int a, int *b, int &c){
    // a is value // &a is this func a memory
    cout << a << "\t" << &a << endl; // *a is not possible
    // b is passing memory address, &b is this func memory, *b is value
    cout << b << "\t" << &b << "\t" << *b << endl;

    // c is value, &c is passing memory address
    cout << c << "\t" << &c << endl; // *c is not possible

    // a is equal to c

    a++;
    (*b)++;
    c++;

}

// c++ support function overloading
int add(int a, int b){
    return a + b;
}

// c++ support default arguments
int add(int a, int b, int c, int d = 1){
    return a + b + c + d;
}

double add(double a, double b){
    return a + b;
}

int main(){
    int a = 1, b = 2, c = 3;
    // int &ap; // error required initialize
    int &ap = a; // both ap and a points to same address
    // ap is reference not pointer, so it can store a not &ap
    cout << ap << "\t" << &ap <<endl; // 1       0x7fff2d768c74
    cout << &a << "\t" << &b << "\t" << &c << endl; // 0x7ffd899c679c  0x7ffd899c6798  0x7ffd899c6794
    fun(a, &b, c);
    /**
     * Op
        1       0x7ffd899c677c
        0x7ffd899c6798  0x7ffd899c6770  2
        3       0x7ffd899c6794
     */
    cout << a << "\t" << b << "\t" << c << endl; // 1       3       4

    // fun(a, *b, *c); // b and c are error
    // fun(a, b, c);  // b is wrong
    // fun(a, &b, &c); // c is wrong

    // summary in function's param &c means is a pointer


    printf("Add : %d\n", add(1,2)); //Add : 3
    printf("Add : %d\n", add(1,2, 3)); //Add : 7
    printf("Add : %lf\n", add(1.1,2.2)); //Add : 3.300000

    int x = 5;
    // int &y = &x; // error:  invalid conversion from ‘int*’ to ‘int’ [-fpermissive]
    int &y = x;
    printf("X is = %d\n",x); // 5
    printf("Y is = %d\n",y); // 5
    y++;
    printf("X is = %d\n",x); // 6
    x++;
    printf("Y is = %d\n",y); //7

    printf("Size of %d, %d, %d\n", sizeof(int) , sizeof(int *), sizeof(int &)); // Size of 4, 8, 4
    // & is a mixture of both normal and pointer
    // *a means it is pointer, storing address
    // & means it not pointer, it don't have actual value, it is refering to other variables like partnering not owning. But it can affect the actual value
    return 0;
}