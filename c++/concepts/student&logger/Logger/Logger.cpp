#include "./AbsLog.cpp"
#ifndef MY_HEADER
#include "../header.h"
#endif

class Student;

class Logger : public AbsLog {
    public:
        static Logger& getInstance(){
            if(ins == nullptr ){
                cout << "No OBJ " << endl;
                ins = new Logger();
            } else {
                cout << "Using cached OBJ " << endl;
            }
            return *ins;
        }

        void info(Student & s);

        void infoNonFriend(Student & s);

        static void deleteInstance(){
            if(ins == nullptr ){
                cout << "No OBJ no delete" << endl;
            } else {
                delete ins;
                cout << "Deleted Logger" << endl;
            }
        }
     
    private:
        inline static Logger* ins = nullptr;
        Logger(){
            cout << "Logger Initiated" << endl;
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        ~Logger() {
            cout << "Logger Destroying" << endl;
            ins = nullptr;
            cout << "Logger Destroyed" << endl;
        }
        friend ostream& operator<< (ostream& cout, Logger& log);
        friend ostream& operator<< (ostream& cout, Logger* log);

};

// These functions should not define here it should define in student class
// void Logger::infoNonFriend(Student & s) {
//     if (ins == nullptr) {
//         cout << "No logger exists" << endl;
//         return ;
//     }
//     cout << "Logger Non friend : [" << s.getName() << "," << s.getStdNo() << "]" << endl;
// }

// void Logger::info(Student &s) {
//     if (ins == nullptr) {
//         cout << "No logger exists" << endl;
//         return ;
//     }
//     cout << "Logger friend : [" << s.name << "," << s.stdno << "]" << endl;
// }
