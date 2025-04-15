/**
 * Author: Jithendiran E.K
 * Date  : 31 Mar 2025
 */
// g++ basic_01.cpp -o basic_01.o
#include <iostream>
using namespace std;

class Empty {};

class A
{
    private:
        int a; // size calculated
        
    public:
        int c; // size calculated

        static int stat; // size not calc

        void fuc() { // size not calc
            cout<<"Member fun \n";
        }
};

class B{
    // access specifier is private by default
    int a = 4;
};

class C: public B{

};

class D {
    int a,b,c,d,e,f; // 24 bytes for int
    // 8 bytes for vtable pointer
    virtual void fun(){

    }
};

int main()
{
    cout<<"Size of class : "<<sizeof(Empty)<<endl; 
    Empty o = Empty();
    cout<<"Size of object : "<<sizeof(o)<<endl;
    /**
    Size of class : 1
    Size of object : 1
     */

    cout<<"Size of class : "<<sizeof(A)<<endl; 
    A ao = A();
    cout<<"Size of object : "<<sizeof(ao)<<endl;
    /**
     * op
    Size of class : 8
    Size of object : 8
     */

     cout<<"Size of class : "<<sizeof(B)<<endl; 
     B bo = B();
     cout<<"Size of object : "<<sizeof(bo)<<endl;
     /**
      * 
      Size of class : 4
      Size of object : 4
      */

     cout<<"Size of class : "<<sizeof(C)<<endl; 
     C co = C();
     cout<<"Size of object : "<<sizeof(co)<<endl;
     /*
     Size of class : 4
     Size of object : 4
     */

     cout<<"Size of class : "<<sizeof(D)<<endl; // Size of class : 32
     //D has virtual table, so it's size is equal to size of pointer
     D do1 = D();
     cout<<"Size of object : "<<sizeof(do1)<<endl; //Size of object : 32

    return 0;
}