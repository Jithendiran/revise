// g++ main.cpp -o Student.out
#include "./concrete/Student.cpp"
#ifndef MY_HEADER
#include "header.h"
#endif
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