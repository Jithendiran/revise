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
    static AbsLog& getInstance();
    static void deleteInstance();
    virtual ~AbsLog(){
        cout << "AbsLog destrutor" << endl;
    }
    protected:
    inline static AbsLog* ins = nullptr;
};

// TODO
/*
After template chapter
call the derived class getInstance based on the type from Abstract class, same for deleteInstance
*/