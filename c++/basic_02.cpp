/**
 * Author: Jithendiran E.K
 * Date  : 1 Apr 2025
 */
// g++ basic_02.cpp -o basic_02.o
#include <iostream>
#include <stdio.h>
using namespace std;

int globe = 1234;

void fun(int a, int *b, int &c){
    // a is value // &a is this func a memory
    cout << a << "\t" << &a << endl; // *a is not possible
    // b is passing memory address, &b is this func memory, *b is value
    cout << b << "\t" << &b << "\t" << *b << endl;

    // c is value, &c is passing memory address
    cout << c << "\t" << &c << endl; // *c is not possible
    // &c is same address of main function variable address, pls check below output

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

// see below desc

void lvalue(int& a){
    // by &a we can avoid copy of lvalue
    printf("Address of a = %p\n",&a);
    a = 27;
}

// void changeaddrlvalue(int& a){
//     a = &globe; // from lvalue it possible to change only vale not address, to change addres we must use pointer
//     // see below
// }

void changeaddrlvalue(int*& a){
    // int*& (pointer to lvalue reference is invalid, but pointer itself is fine)
    // a = 27; // this is error
    // Here pointer is changed 
    printf("Address of a = %p\n",&a);
    printf("Address of globe = %p\n",&globe);

    a = &globe;

    printf("Address of a after change = %p\n",&a);

}


void rvalue(int &&a){
    // by &&a we can avoid copy of rvalue
    printf("Inside rvalue func \n");
    printf("%p, %d\n", &a, a);
    a = 100;
    printf("%p, %d\n", &a, a);
}

void changeaddrrvalue(int*&& a){
    //     int*&& (rvalue reference to a pointer is valid)

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

    // --------------------
    int x = 5;
    // int &y = &x; // error:  invalid conversion from ‘int*’ to ‘int’ [-fpermissive]
    int &y = x;
    // y alias to x
    printf("X is = %d\n",x); // 5
    printf("Y is = %d\n",y); // 5
    y++;
    printf("X is = %d\n",x); // 6
    x++;
    printf("Y is = %d\n",y); //7

    // &y=a; // error: lvalue required as left operand of assignment
    y = a; // here a is copied to x, y is not pointing to a
    printf("X is = %d\n",x); // 1
    a++; // this won't affect x
    printf("X is = %d\n",x); // 1

    // So &y is one time init to a lvalue

    printf("Size of %d, %d, %d\n", sizeof(int) , sizeof(int *), sizeof(int &)); // Size of 4, 8, 4
    // & is a mixture of both normal and pointer
    // *a means it is pointer, storing address
    // & means it not pointer, it don't have actual value, it is refering to other variables like partnering not owning. But it can affect the actual value
    // More use ful in functions

    //--------------------------------------------------------------------------------------------------------------
    // int &&ref = x; // error cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    // int &&ref = &x; // error: invalid conversion from ‘int*’ to ‘int’

    int &&rf = 10; // int&& is an rvalue reference in C++. It is used to bind to temporary (rvalue) objects
    /* This && is use less in regular data type, if a object is big when we want that object in another variable this is useful
       x = obj; 
       x = null;
       here (x = obj) obj content will be copied and stored in x, at this time x and obj have different object with same content
       delete x; to delete the object pointed by x

       instead of those complex operation, we can simply move the reference of obj to x, this is like transfering owner
       x = std::move(obj); this time move constructor is called, in move constructor && is used
    */ 
    rf = rf+1;
    printf("rf is = %d\n",rf); //rf is = 11

    printf("rf is = %p\n",&rf); // 0x7fffbc70bb24
    // printf("rf is = %p\n",&&rf); // error: label ‘rf’ used but not defined
    // it is more helpful in object's for moving resource, after moving still object are valid

    /**
     *       
     * Lvalue
            An lvalue refers to a specific memory location and can appear on the left-hand side of an assignment
            Lvalues are objects that persist beyond a single expression. For example, variables, arrays, and objects are lvalues.
            int x =10;
            x is lvalue, it has memory, can reuse

            lvalue reference
                An lvalue reference is a reference that binds to an lvalue. It allows you to modify the object that the reference is referring to., 
                It only binds to lvalues, means in below code ref = x is valid, ref = 10 is invalid
                
                int x = 10;
                int& ref = x;  // ref is an lvalue reference to x
                ref = 20;      // modifies x through the reference
                std::cout << x << std::endl;  // Output: 20

                int& (lvalue reference)


     * Rvalue
            An rvalue is a temporary object or value that does not persist beyond the expression. Rvalues are typically used on the right-hand side of an assignment.
            int x = 10+5; here 10+5 is r value, result 15 is lvalue because after this expression we can't access 10+5

            Rvalue reference 
                Refers to a temporary object (rvalue). It allows efficient resource transfer, such as in move semantics, where you can "move" the contents of one object to another.
                example move constructor

                int&& (rvalue reference)
      
    ----------------------------------------------------------------------------------------------------------------------
     * Lvalues and Rvalues in Function Parameters:
      
      - Lvalue references (`int&`):
        Used when the function needs to modify or access the original variable.
        Example:
          void modify(int& x) {
              x += 10; // Modifies the original variable.
          }
          int a = 5;
          modify(a); // a becomes 15.
      
      - Rvalue references (`int&&`):
        Used to bind to temporary objects or values, often for optimization (e.g., move semantics).
        Example:
          void process(int&& x) {
              x += 10; // Modifies the temporary object.
          }
          process(5); // Valid, binds to rvalue 5.
        //------------------------------------
          how int&& x is optimized
          void process(int x) {
              x += 10; // also 15
          }
          process(5);
          what happen here is 5 is copied and placed in process function stack, so here two time 5 is present
        
          when we use int&& x, now x is point to rvalue 5, no copy is made

          //----------------------------------------
          int main() {
            int a = 42;
            print(std::move(a)); // Forces `a` to be an rvalue
          }

      
      - Mixing both:
        Functions can overload for both lvalue and rvalue references.
        Example:
          void print(int& x) { std::cout << "Lvalue: " << x << std::endl; }
          void print(int&& x) { std::cout << "Rvalue: " << x << std::endl; }
          int a = 10;
          print(a);   // Calls lvalue version.
          print(20);  // Calls rvalue version.
     */

    // lvalue
    int a1 = 2;
    printf("address a1 = %p\n",&a1); //address a1 = 0x7ffc273fadf0
    lvalue(a1); // Address of a = 0x7ffc273fadf0
    printf("a1 is = %d\n",a1); // a1 is = 27

    // lvalue pointer
    int *ptr = &a1;
    printf("ptr %p, %p\n",ptr, &ptr); // ptr 0x7ffc273fadf0, 0x7ffc273fade8
    changeaddrlvalue(ptr); 
    /**
     *  Address of a = 0x7ffc273fade8
        Address of globe = 0x55a27f37b048
        Address of a after change = 0x7ffc273fade8
     */

    printf("a1 is = %d\n",a1); // a1 is = 27
    printf("ptr %p, %p\n",ptr, &ptr); //ptr 0x55a27f37b048, 0x7ffc273fade8
    printf("*ptr = %d\n", *ptr); // *ptr = 1234

    
    // rvalue
    // rvalue(a); // error
    rvalue(10);
    /**
    Inside rvalue func 
    0x7ffc4b8eb2a4, 10
    0x7ffc4b8eb2a4, 100
     */
    printf("a is = %d\n",a); // a is = 2
    rvalue(std::move(a)); // force a to send as rvalue, rvalue don't do copy
    /*
    Inside rvalue func 
    0x7ffc4b8eb2a0, 2
    0x7ffc4b8eb2a0, 100
    */
    printf("a is = %d\n",a); // a is = 100
    // use case in online they telling effective, need to check more indepth later  

    // rvalue pointer
    // changeaddrrvalue()
    

    return 0;
}