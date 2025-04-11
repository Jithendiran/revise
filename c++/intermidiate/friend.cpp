/**
 * Author: E.K.Jithendiran
 * Date  : 10 Apr 2025
 */
// g++ friend.cpp -o friend.o

#include<iostream>
using namespace std;

/*
Friend
 Friend function can declared outside of the class can access private and protected members
*/

struct s2; // to avoid un declared error in s1's add function

struct s1{
    public:
        int pu = 1;
        void s(s2 o){
            // o.pr; pr of s2 is not visible to this method
        }
    private:
        int pr = 2;
        friend void pr(s1&); // this only access s1 because it is declared only in s1 struct and it's parameter is only s1
        friend int add(s1, s2); // This can access s1 and s2 because it is declared on both s1 and s2, also it takes 2  params for s1 and s2
    protected:
        int po = 3;
};

void pr(s1& obj){
    // this is the friend function of a class, it can access protected and private members also
    cout << "Public : " <<  obj.pu << endl;
    cout << "Protected : " <<  obj.po << endl;
    cout << "Private : " <<  obj.pr << endl;
}

struct s2{
    private:
     int  pr = 3;
     friend int add(s1, s2); // without this declaration, add function can't access s2 object
};

// if it have s1 param means it should declare as friend function in s1, same for s2
int add(s1 os1, s2 os2){
        return os1.pr + os2.pr;
}

/* friend class
 When a class is declared as friend, all the members function of the friend class become friend function 
*/

struct s4;

struct s3 {
    public:
    int pu = 1;
    private:
    int pr = 2;
    friend class s4; // s4 can access all members of s3 inside s4
    // not possible to access s4's private and protected members in this class, bcz s3 is not a friend of s4 
    protected:
    int po = 3;
};

// s3 can't access s4's member because it is not a friend class

struct s4{
    private:
     int a;
    public:
     int add() {
        s3 o;
        return o.po + o.pr + o.pu;
     }
     int sub();
};

int s4::sub(){
    s3 o;
    return o.po - o.pr - o.pu;
}

// specific method of another class

struct s5;

struct s6{
    public:
    void display(s5& s);
};

struct s5 {
    public:
        int pu = 1;
    private:
        int pr = 3;
        friend void s6::display(s5&);  // only s6's display function is friend of s5, so display function can access s6

};

void s6::display(s5& s) {
    cout << "pu: " << s.pu << endl;
    cout << "pr: " << s.pr << endl; //  private access allowed via friend
}


// inheritance
struct A {
    private:
        int a = 5;
        friend struct B; // struct b can access struct a's private members
};

struct B {
    public:
     void display(){
        A a;
        cout <<a.a<<endl;
     }
};

struct C : public B {
    void display(){
        A a;
        // cout <<a.a<<endl; // A is not a friend of C, even though it is inherited from parent struct 
    }
};

int main() {
    s1 o;
    pr(o);
    /*
    Public : 1
    Protected : 3
    Private : 2
    */
    s2 os2;
    cout << "Add : " << add(o, os2) << endl; // Add : 5

    s4 os4;
    cout << "Add 2 : " << os4.add() << endl; // Add 2 : 6

    B b;
    b.display(); // 5

    C c;
    c.display();
}