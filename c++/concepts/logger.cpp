/**
 * Author: E.K.Jithendiran
 * Date  : 12 Apr 2025
 */
// g++ logger.cpp -o logger.o

#include<iostream>
using namespace std;

class Logger {
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

        void info(string s) {
            if (ins == nullptr) {
                cout << "No logger exists" << endl;
                return ;
            }
            cout << s << endl;
        }

        static void deleteInstance(){
            if(ins == nullptr ){
                cout << "No OBJ no delete" << endl;
            } else {
                delete ins;
                cout << "Deleted" << endl;
            }
        }
     
    private:
        // NULL is just 0 integer, NULL == 0   
        // nullptr it is type safety, it is nullptr != 0
        inline static Logger* ins = nullptr;
        Logger(){
            cout << "Logger Initiated" << endl;
        }

        // delete copy constructor
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

// address of Logger::ins will always same because of static nature
ostream& operator<< (ostream& cout, Logger& log) /*const*/ { // using const is allowed inside class or struct
    cout << "Called << log &";
    // Logger::ins or log.ins; same result
    cout << " " << &(Logger::ins) << " content " << static_cast<void*>(log.ins);
    return cout;
}

ostream& operator<< (ostream& cout, Logger* log) { 
    cout << "Called << log *" ;
    /*
    error
    cout << log->ins; // this will call this function `operator<<` recursively till memory fill up
    */
    cout << " " << &(Logger::ins) << " content " << static_cast<void*>(log->ins);
    return cout;
}

//--------------------Non static
class LoggerNs {
    public:
    LoggerNs(){
        cout << "LoggerNs Initiated" << endl;
    }

    // delete copy constructor
    LoggerNs(const LoggerNs&) = delete;
    LoggerNs& operator=(const LoggerNs&) = delete;

    void info(string s) {
        cout << s << endl;
    }

    ~LoggerNs() {
        cout << "LoggerNs Destroyed" << endl;
    }
    friend ostream& operator<< (ostream& cout, LoggerNs& log);
    friend ostream& operator<< (ostream& cout, LoggerNs* log);

};

ostream& operator<< (ostream& cout, LoggerNs& log) {
    cout << "Called << log &";
    cout << " " << static_cast<void*>(&log);
    return cout;
}

ostream& operator<< (ostream& cout, LoggerNs* log) { 
    cout << "Called << log *" ;
    cout << " " << static_cast<void*>(log);
    return cout;
}

int main(){
    Logger& l1 = Logger::getInstance();
    /*
    No OBJ 
    Logger Initiated
    */
    Logger& l2 = Logger::getInstance();
    // Using cached OBJ 

    // Logger* l3 = new Logger();

    cout << "L1 is : " << l1 << endl;
    // L1 is : Called << log & 0x6536956f9158 content 0x6536b34332c0
    cout << "L2 is : " << l2 << endl;
    // L2 is : Called << log & 0x6536956f9158 content 0x6536b34332c0
    l2.info("print test"); // print test

    l2.deleteInstance();
    /*
    Logger Destroying
    Logger Destroyed
    Deleted
    */

    cout << "L1 is : " << &l1 << endl;
    // L1 is : Called << log * 0x6536956f9158 content 0
    cout << "L2 is : " << &l2 << endl;
    // L2 is : Called << log * 0x6536956f9158 content 0
    cout << "L1 is : " << l1 << endl;
    // L1 is : Called << log & 0x6536956f9158 content 0

    // cout << "L1 is : " << *l1 << endl; // not valid

    l2.info("print test"); // No logger exists
    // the above l1 and l2 is now dangling pointer
    cout << endl;


    Logger* log = & Logger::getInstance();
    /*
    No OBJ 
    Logger Initiated
    */
    log->info("Hello");
    // cout << "L1 is : " << log << endl;

    cout << "log is : " << log << endl;
    // log is : Called << log * 0x6536956f9158 content 0x6536b34332c0
    cout << "log is : " << *log << endl;
    // log is : Called << log & 0x6121c72f6158 content 0x6536b34332c0
    /*
    why still 0x6536b34332c0 same as old object, might be because of heap don't content, just reference lost the access
    after delete c++ don't return the memory to OS, it use for future purpose of `new` allocation
    */

    Logger::deleteInstance();
    /*
    Logger Destroying
    Logger Destroyed
    Deleted
    */
    // delete log; // error: ‘Logger::~Logger()’ is private within this context
    log = nullptr; // why still accessable?
    log->info("crash?");  // No logger exists
    cout << "log is : " << log << endl;
    // log is : Called << log * 0x6121c72f6158 content 0
    cout << "log is : " << *log << endl;
    // log is : Called << log & 0x6121c72f6158 content 0

    /*
    Always use 
    if (log != nullptr) {
        log->info("safe call");
    }
    */
   
    
    cout << "----------------------Non static" << endl;
    // cout << "Logger NS : " << &(LoggerNs);
    LoggerNs * ns = new LoggerNs(); // LoggerNs Initiated
    cout << "Non logger is " << ns << endl; // Non logger is Called << log * 0x5d2d980492c0
    // ns = nullptr; // with this destructor won't call and object won't properly freed
    delete ns; // LoggerNs Destroyed

    LoggerNs * ns1 = new LoggerNs(); // LoggerNs Initiated
    cout << "Non logger is " << ns1 << endl; // Non logger is Called << log * 0x5d2d980492c0
    delete ns1; // LoggerNs Destroyed

    LoggerNs * ns3 = new LoggerNs(); // LoggerNs Initiated
    cout << "Non logger is " << ns3 << endl; //Non logger is Called << log * 0x5d2d980492c0

    LoggerNs * ns4 = new LoggerNs(); // LoggerNs Initiated
    cout << "Non logger is " << ns4 << endl; // Non logger is Called << log * 0x5d2d980492e0
    // ns3 = nullptr;
    delete ns3; // LoggerNs Destroyed

    // ns4 = nullptr;
    delete ns4; // LoggerNs Destroyed
}