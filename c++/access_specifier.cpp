/**
 * Author: Jithendiran E.K
 * Date  : 31 Mar 2025
 */
// g++ access_specifier.cpp -o access_specifier.o
#include <iostream>
using namespace std;

// strct is public by default
// class is private by default
/*
    * Access specifier
    * 1. private
    * 2. protected
    * 3. public
    *
    * Default access specifier is private for class and public for struct.
    * Private members are not accessible outside the class.
    * Protected members are accessible inside the class and child class but not outside the class.
        Can't be accessed by object of class.
    * Public members are accessible outside the class.
        Can be accessed with object of class.

    C++ does not have a `public class A` like Java.
    c++'s protected member are not accessible by object of the class, but Java can access by object of class.
    c++ does not support `static class A` like java, static member and methods are possible
*/

class A {
    // default access specifier is private
    // only accessible inside the class not even by object or inside child class
    int a = 4;
    protected:
        // protected members are accessible inside the class and child class
        // but not accessible outside the class, also not accessible with object of A
        int b = 5;
        void protected_fun(){
            cout<<"Protected function \n";
        }

    public:
        // only public members are accessible outside the class, means objects can access this part
        int c = 6;
        void public_fun(){
            cout<<"Public function \n";
        }
};

class B: A // Inheritance is private by default
{
    // what ever inherited from A is private to B
    // Public members (c, public_fun()) become private in B. (Like c and public_fun() are B's members declared as private) 
    // Protected members (b, protected_fun()) become private in B.
    // Private members (a) are never inherited.

    int ba = 2;
    protected:
        int bb = 3;
    public:
        int bc = 4;
        void access(){
            cout<<"Inside B\n";
            // cout<<a<<endl; // error: 'int A::a' is private within this context
            cout<<b<<endl; // This bacame private in class B
            cout<<c<<endl; // This became private in class B
            public_fun(); // This bacame private in class B
            protected_fun(); // This bacame private in class B
        }
};

class C: protected A {
    // what ever inherited from A is protected to C
    // Public members (c, public_fun()) become protected in C.
    // Protected members (b, protected_fun()) remain protected in C.
    // Private members (a) are never inherited.

    int ca = 2;
    protected:
        int cb = 3;
    public:
        int cc = 4;
        void access_c(){
            cout<<"Inside c\n";
            // cout<<a<<endl; // error: 'int A::a' is private within this context
            cout<<b<<endl; 
            cout<<c<<endl; 
            public_fun(); 
            protected_fun(); 
        }
};

class D: public A{
    // what ever inherited from A is public to D
    // It means what ever parent access specifier is inherited as it is
    // Public members (c, public_fun()) remain public in D.
    // Protected members (b, protected_fun()) remain protected in D.
    // Private members (a) are never inherited.
    int da = 2;
    protected:
        int db = 3;
    public:
        int dc = 4;
        void access_d(){
            // cout<<a<<endl; // error: 'int A::a' is private within this context
            cout<<"Inside D\n";
            cout<<b<<endl;
            cout<<c<<endl;
        }
};

class E: private A{
    // A's member are private to E
    // b and c are private to E
    private:
        int ea = 2; // only accessible inside E
    protected:
        int eb = 3; // only accessible inside E and child class
    public:
        int ec = 4; // accessible inside E, child class and with Object

        void access_e(){
            // a; // error : private of A
            b;
            c;
            ea;
            eb;
            ec;
        }
};

class F: protected E{
    // b, c are not accessible
    // ea is not accessible
    // ec, eb is accessible inside this class not by object or outside

    private:
        int fa = 2; // only accessible inside F
    protected:
        int fb = 3; // only accessible inside F and child class
    public:
        int fc = 4;

        void access_f(){
            // a; // error : private of A
            // b; // error: private of E
            // c; // error: private of E
            // ea;//error: private of E
            eb;
            ec;
        }
};

class G: public F{
    // b, c are not accessible
    // ea is not accessible
    // ec, eb is accessible inside this class not by object or outside
    // fa, fb are not accessible

    private:
        int ga = 2; // only accessible inside G
    protected:
        int gb = 3; // only accessible inside G and child class
    public:
        int gc = 4; 

        void access_g(){
            // a; // error : private of A
            // b; // error: private of E
            // c; // error: private of E

            // ea;//error: private of E
            eb; //protected of E, F and G
            ec; //public of E, protected of F and G

            // fa; // private of F
            fb; // protected of F and G
            fc; // public of F and G

            ga; // private of G
            gb; // protected of G
            gc; // public of G
        }
        
};


int main(){
    A a = A();

    // public members are accessible
    cout<<a.c<<endl; // 6
    a.public_fun(); // Public function
    /**
    6
    Public function 
    */

    // protected members are only accessible inside It's class and child class
    // protected members are not accessible outside the class
    // cout<<a.b<<endl; // error: 'int A::b' is protected within this context
    // a.protected_fun(); // error: 'void A::protected_fun()' is protected within this context
    
    // private members are only accessible inside class
    // cout<<a.a<<endl; // error: 'int A::a' is private within this context

    B b = B();
    b.access();
    /**
    cout<<"Inside A\n";
    5
    6
    Public function 
    Protected function
     */
    // because of inheritance private inheritance, parent's public members become private in child class
    // b.public_fun();  // error: ‘void A::public_fun()’ is inaccessible within this context
    // b.protected_fun(); // error: ‘void A::protected_fun()’ is protected within this context


    C c = C();
    // c.public_fun(); //error: ‘A’ is not an accessible base of ‘C’
    // c.protected_fun(); // error: ‘A’ is not an accessible base of ‘C’
    c.access_c();
    /**
     Inside c
    5
    6
    Public function 
    Protected function 
    */

    D d = D();
    d.access_d();
    /**
     Inside D
     5
     6
     */
    d.public_fun(); // Public function
    // d.protected_fun(); // error: not accessable
    // d.a; // error: ‘int A::a’ is inaccessible



    // multilevel
    E e = E();
    // e.a; // error: A's private
    // e.b; // error: A's protected
    // e.c; // error: public of A and private of E

    // e.ea;// error: E's private
    // e.eb;// error : E's protected
    e.ec;

    F f = F();
    // f.a; // error: A's private
    // f.b; // error: A's protected
    // f.c; // error: public of A and private of E
    // f.ea;// error: E's private
    // f.eb;// error : E's protected
    // f.ec;// error : E's public and protected of F
    // f.fa; // error: F's private
    // f.fb; // error: F's protected
    f.fc; // F's public

    G g = G();
    // g.a; // error: A's private
    // g.b; // error: A's protected
    // g.c; // error: public of A and private of E
    // g.ea; // error: E's private
    // g.eb; // error : E's protected
    // g.ec; // error : E's public and protected of F
    // g.fa; // error: F's private
    // g.fb; // error: F's protected
    g.fc; // error: F's public
    // g.ga; // error: G's private
    // g.gb; // error: G's protected
    g.gc; // G's public
    return 0;
}