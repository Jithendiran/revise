#ifndef MY_HEADER
#include "../header.h"
#endif

class Student;
class AbsLog{
    public:
    AbsLog(){
        cout << "Default AbsLog cons" << endl;
    }
    virtual void info(Student & s) = 0;
    
    virtual void infoNonFriend(Student & s) = 0;

    virtual ~AbsLog(){
        cout << "AbsLog destrutor" << endl;
    }
};