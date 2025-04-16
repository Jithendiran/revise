#include<iostream>
#include<cstring>
using namespace std;
class Student{
    char* name = nullptr;
    int stdno = -1;
    friend ostream& operator<<(ostream&, Student&);
    public:

    // assigning private values without constructor
    // problem, user can set and unset values any time
    // Object of name size may vary on each time 
    Student()=default;
    void setName(char *name){
        if(name != nullptr){
            delete[] name;
        }
        this->name = new char[strlen(name)+1];
        strcpy(this->name, name);
    }

    void setStdNo( int stdno) {
        this->stdno = stdno;
    }

    int getStdNo() const {
        return stdno;
    }

    const char * getName() const {
        return this->name;
    }

    // assigning private values with constructor
    Student(char *name, int stdno){
        this->name = new char[strlen(name)+1];
        strcpy(this->name, name);
        this->stdno = stdno;
    }

    ~Student() {
        cout << "Deleting object " << (this) << endl;
        delete[] name;
        stdno = -1;
    }

};

// friend function for accessing private, witho
ostream& operator<< (ostream& cout, Student &s){
    cout << "(" << s.name << "," << s.stdno << ")";
    return cout;
}

void printStudent(Student **s){

    if(*s == nullptr){
        return;
    }
    cout << "Student : " << **s << endl;
    s++;
    printStudent(s);
}

int main() {
    int n;
    cout  << "Enter no of students : ";
    cin >> n;
    Student *s[n + 1];
    s[n] = nullptr;
    char name[25];
    int stdno = -1;
    for(int i = 0; i < n; i++){
        
        cout << "Enter name of the student (limit 25 char, no space):";
        cin >> name;
        cout << "Enter class no (Only number):";
        cin >> stdno;
        s[i] = new Student(name, stdno);
    }

    cout << "Details : " << endl;

    // Loop
    for(int i = 0; i < n; i++){
        cout << "Student : " << i+1 << " : " << *s[i] << endl;
    }

    // recersive
    // *s is 1st element reference
    // printStudent(s);

    delete[] s[n+1];
}