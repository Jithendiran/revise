/**
 * Author: E.K.Jithendiran
 * Date  : 13 Apr 2025
 */
// g++ copy.cpp -o copy.o

#include<iostream>
#include<cstring>
using namespace std;

/**
 * Shallow copy/ deep copy
 * 
 * Shallow copy
 *  If pointer is used,  using the same reference to another object, so the underlying object is same.
 *  if one of the object delete the object, it impact the other object all well
 * 
 * Deep copy
 *  It will copy the copy by creating new dynamic memory and update the reference to new object, so the underlying object is different
 *  If one object is deleted another object won't affect 
 */

class shallow {
    public:
    char* name;
     shallow() = delete; // deleting default constructor
     shallow(const char * name) {
        this->name = new char[strlen(name) + 1]; // +1 bcz, strlen does not include null
        strcpy(this->name, name);
    }

    shallow(const shallow &s) {
        cout << "shallow copy\n";
        this->name = s.name;
    }

    ~shallow() {
        cout << "Shallow destructor..\n";
        delete[] this->name;
        this->name = nullptr;
    }
};

class Deep {
    public:
    char* name;
     Deep() = delete; // deleting default constructor
     Deep(const char * name) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }

    Deep(const Deep &s) {
        cout << "Deep copy\n";
        this->name = new char[strlen(s.name) + 1];
        strcpy(this->name, s.name);
    }

    ~Deep() {
        cout << "Deep destructor..\n";
        delete[] this->name;
        this->name = nullptr;
    }
};

int main() {
    shallow * s1 = new shallow("s1");
    cout << "S1 : " << s1->name << endl;
    // S1 : s1
    shallow * s2 = new shallow(*s1);
    // shallow copy
    cout << "S2 : " << s2->name << endl;
    // S2 : s1

    delete s1;
    // Shallow destructor..
    s1 = nullptr; 
    cout << "S2 : " << s2->name << endl; // S2 : (��
    // delete s2; // cause issue bacause s1 already freed the memory

    // deep copy
    Deep * d1 = new Deep("d1");
    cout << "D1 : " << d1->name << endl;
    // D1 : d1
    Deep * d2 = new Deep(*d1);
    // Deep copy
    cout << "D2 : " << d2->name << endl;
    // D2 : d1
    delete d1;
    // Deep destructor..
    s1 = nullptr; 
    cout << "D2 : " << d2->name << endl;
    // D2 : d1
    delete d2; // Deep destructor..
}