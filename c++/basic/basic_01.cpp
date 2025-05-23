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

class E : public D {};

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
    /*
    * GDB 
    > p do1
    $1 = {_vptr.D = 0x555555557d50 <vtable for D+16>, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0}

    > x/16x 0x555555557d50
    0x555555557d50 <_ZTV1D+16>:	0x5555556a	0x00005555	0xf7e1dc30	0x00007fff
    0x555555557d60 <_ZTI1E+8>:	0x55556027	0x00005555	0x55557d70	0x00005555
    0x555555557d70 <_ZTI1D>:	0xf7e1cfa0	0x00007fff	0x5555602a	0x00005555
    0x555555557d80:	0x00000001	0x00000000	0x0000019c	0x00000000
    */
    E eo = E();
    cout<<"Size of object : "<<sizeof(eo)<<endl; //Size of object : 32

    /*
    GDB from VSCODE
    -exec p eo
    $2 = {<D> = {_vptr.D = 0x555555557d38 <vtable for E+16>, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0}, <No data fields>}


    -exec x/16x 0x555555557d38
    0x555555557d38 <_ZTV1E+16>:	0x5555556a	0x00005555	0x00000000	0x00000000
    0x555555557d48 <_ZTV1D+8>:	0x55557d70	0x00005555	0x5555556a	0x00005555
    0x555555557d58 <_ZTI1E>:	0xf7e1dc30	0x00007fff	0x55556027	0x00005555
    0x555555557d68 <_ZTI1E+16>:	0x55557d70	0x00005555	0xf7e1cfa0	0x00007fff
    */
    return 0;
}