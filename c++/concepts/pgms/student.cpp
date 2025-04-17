#include <iostream>
#include <cstring>
using namespace std;

class Student;
class Logger;

class AbsLog{
    public:
    AbsLog(){
        cout << "Default AbsLog cons" << endl;
    }
    virtual void infoNonFriend(Student & s) = 0;
    virtual ~AbsLog(){
        cout << "AbsLog destrutor" << endl;
    }
};

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

// These functions should declar under Student class, if declared above student it will throw error

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



int main()
{
    int n;
    cout << "Enter no of students : ";
    cin >> n;
    Student *s[n + 1];
    s[n] = nullptr;
    char name[25];
    int stdno = -1;
    for (int i = 0; i < n; i++)
    {

        cout << "Enter name of the student (limit 25 char, no space):";
        cin >> name;
        cout << "Enter class no (Only number):";
        cin >> stdno;
        s[i] = new Student(name, stdno);
    }

    cout << "Details : " << endl;

    // Loop
    // calling copy const when printing
    for (int i = 0; i < n; i++)
    {
        cout << "Student : " << i + 1 << " : " << *s[i] << endl;
    }

    // recersive
    // *s is 1st element reference
    printStudent(s);
    
    // print without Friend, logger
    
    // Log method
    AbsLog & log = Logger::getInstance();

    log.infoNonFriend(*(s[0])); // non friend

    Logger & log1 = dynamic_cast<Logger&>(log); 

    log1.info(*(s[1])); // friend


    // delete objects
    for (int i = 0; i < n; i++)
    {
        delete s[i];
    }

    Logger::deleteInstance();
}
/*
OP
Enter no of students : 2
Enter name of the student (limit 25 char, no space):AA
Enter class no (Only number):1
Enter name of the student (limit 25 char, no space):BB
Enter class no (Only number):2
Details : 
Student : 1 : (AA,1)
Student : 2 : (BB,2)
Student : (AA,1)
Student : (BB,2)
No OBJ 
Default AbsLog cons
Logger Initiated
Logger Non friend : [AA,1]
Logger friend : [BB,2]
Deleting object (AA,1)
Deleting object (BB,2)
Logger Destroying
Logger Destroyed
AbsLog destrutor
Deleted Logger
*/

// TODO seperate in each files