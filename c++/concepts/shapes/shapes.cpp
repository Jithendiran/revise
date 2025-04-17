#include <iostream>
using namespace std;

class AShape{
    protected:
    int n = 0;
    public:
    AShape(){
        cout << "AShape Default constructor" << endl;
    }
    AShape(int i){
        this->n = i;
        cout << "AShape Param constructor" << endl;
    }
    AShape(const AShape & obj){
        cout << "AShape Copy constructor" << endl;
    }
    AShape(const AShape && obj){
        cout << "AShape Move constructor" << endl;
    }
    virtual void shape() = 0;
    virtual ~AShape() {
        cout << "AShape destructor" << endl;
    }
};

class Lprymid: public AShape {
    public:
    Lprymid(int i): AShape(i) {
        cout << "Lprymid param constructor" << endl;
    }

    Lprymid(const Lprymid & obj){
        cout << "Lprymid Copy const" << endl;
    }

    Lprymid(const Lprymid && obj){
        cout << "Lprymid move const" << endl;
    }

    void shape(){
        cout << "Lprymid" << endl;
        for(int i = 0; i < n ; i++){
            for(int j = 1; j<=i+1 ;j++){
                cout<<" * ";
            }
            cout << endl;
        }
    }

    ~Lprymid() {
        cout << "Lprymid destructor" << endl;
    }
};

class Rprymid: public AShape {
    public:
    Rprymid(int i): AShape(i) {
        cout << "Rprymid param constructor" << endl;
    }

    Rprymid(const Rprymid & obj){
        cout << "Rprymid Copy const" << endl;
    }

    Rprymid(const Rprymid && obj){
        cout << "Rprymid move const" << endl;
    }

    void shape(){
        cout << "Rprymid" << endl;
        for(int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++){
                if (j >= n-(i+1)) cout << " * ";
                else cout << "   ";
            }
            cout << endl;
        }
    }

    ~Rprymid() {
        cout << "Rprymid destructor" << endl;
    }
};

class Diamond: public AShape {
    public:
    Diamond(int i): AShape(i) {
        cout << "Diamond param constructor" << endl;
    }

    Diamond(const Diamond & obj){
        cout << "Diamond Copy const" << endl;
    }

    Diamond(const Diamond && obj){
        cout << "Diamond move const" << endl;
    }

    void shape(){
        cout << "Diamond" << endl;
        n = n / 2;
        for(int i = 0; i < n-1; i++) {
            for (int j = 0; j < n; j++){
                if (j >= n-(i+1)) cout << " *  "; // // back one extra space
                else cout << "  ";
            }
            cout << endl;
        }

        for(int i = n-1; i >= 0; i--) {
            for (int j = 0; j < n; j++){
                if (j >= n-(i+1)) cout << " *  "; // last one extra space
                else cout << "  ";
            }
            cout << endl;
        }
    }

    ~Diamond() {
        cout << "Diamond destructor" << endl;
    }
};

void print(AShape *obj){
    if(obj != nullptr){
        obj->shape();
    } else {
        cout << "NO object" << endl;
    }
}

int main(){
    int n = 0;
    cout << "Enter number of lines : ";
    cin >> n;
    AShape * a = new Lprymid(n);
    print(a);
    delete a; // if missed memory leak

    a = new Rprymid(n);
    print(a);
    delete a;

    a = new Diamond(n);
    print(a);
    delete a;
}

/*
OP
Enter number of lines : 8
AShape Param constructor
Lprymid param constructor
Lprymid
 * 
 *  * 
 *  *  * 
 *  *  *  * 
 *  *  *  *  * 
 *  *  *  *  *  * 
 *  *  *  *  *  *  * 
 *  *  *  *  *  *  *  * 
Lprymid destructor
AShape destructor
AShape Param constructor
Rprymid param constructor
Rprymid
                      * 
                   *  * 
                *  *  * 
             *  *  *  * 
          *  *  *  *  * 
       *  *  *  *  *  * 
    *  *  *  *  *  *  * 
 *  *  *  *  *  *  *  * 
Rprymid destructor
AShape destructor
AShape Param constructor
Diamond param constructor
Diamond
       *  
     *   *  
   *   *   *  
 *   *   *   *  
   *   *   *  
     *   *  
       *  
Diamond destructor
AShape destructor
*/