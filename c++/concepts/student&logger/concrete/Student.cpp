#include "../Logger/Logger.cpp"
#ifndef MY_HEADER
#include "../header.h"
#endif

class Student
{
    char *name = nullptr;
    int stdno = -1;
    friend void Logger::info(Student &s);
    friend ostream &operator<<(ostream &, Student &); // Student & instead of Student is to avoid copy constructor
public:
    // assigning private values without constructor
    // problem, user can set and unset values any time
    // Object of name size may vary on each time
    Student() = default;
    // assigning private values with constructor
    Student(char *name, int stdno)
    {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->stdno = stdno;
    }
    Student(const Student &s)
    {
        cout << "Copy cons \n";
    }

    void setName(char *name)
    {
        if (this->name != nullptr)
        {
            delete[] this->name;
        }
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }

    void setStdNo(int stdno)
    {
        this->stdno = stdno;
    }

    int getStdNo() const
    {
        return stdno;
    }

    const char *getName() const
    {
        return this->name;
    }

    ~Student()
    {
        cout << "Deleting object " << *this << endl;
        delete[] name;
        stdno = -1;
    }
};


// friend function for accessing private
// Student s in param is calling copy cons, and when goes out of scope will destroy the object, it may impact the results
// so use Student &s, reference operator, it will avoid copy
ostream &operator<<(ostream &cout, Student &s)
{
    if (&s != nullptr)
    {
        cout << "(" << s.name << "," << s.stdno << ")";
    }
    return cout;
}

void printStudent(Student **s)
{

    if (*s == nullptr)
    {
        return;
    }
    cout << "Student : " << **s << endl;
    s++;
    printStudent(s);
}

// These functions should declar under Student class, if declared above student it will throw error bcz it don't aware of the Student class members

void Logger::infoNonFriend(Student & s) {
    if (ins == nullptr) {
        cout << "No logger exists" << endl;
        return ;
    }
    cout << "Logger Non friend : [" << s.getName() << "," << s.getStdNo() << "]" << endl;
}

void Logger::info(Student &s) {
    if (ins == nullptr) {
        cout << "No logger exists" << endl;
        return ;
    }
    cout << "Logger friend : [" << s.name << "," << s.stdno << "]" << endl;
}