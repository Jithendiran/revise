
#include <iostream>
#include <cstdlib>  // For system() function
#include <cstring>  // For strerror()
#include <unistd.h> // For getuid()

using namespace std;

bool isRoot() {
    return getuid() == 0;  // Check if the program is running as root
}

int main() {
    // Check if the program is running with root privileges
    if (!isRoot()) {
        cout << "You must run this program as root to change the system time." << endl;
        return 1;
    }

    // // Define the new date and time (format: "YYYY-MM-DD HH:MM:SS")
    // string newDateTime = "2025-05-14 14:30:00"; 

    // // Build the command to set the date and time
    // string command = "date --set=\"" + newDateTime + "\"";

    // // Execute the command
    // int ret = system(command.c_str());

    // if (ret == -1) {
    //     cerr << "Error executing date command: " << strerror(errno) << endl;
    //     return 1;
    // }

    // cout << "Date and time set to: " << newDateTime << endl;

    string cmd = "ls -la";
    int ret = system(cmd.c_str());
    /*
    The c_str() method is used to get a C-style string (null-terminated character array) from a std::string object in C++. 
    Since functions like system() expect a C-style string as input, we use c_str() to convert a std::string into that format.
    */
   if (ret == -1) {
        cerr << "Error executing date command: " << strerror(errno) << endl;
        return 1;
    }

    return 0;
}

/*
Enter the number of strings: 5
Enter 5 strings:
zzsds
zasddf
abc
aaa
bfe
Sorted Array: aaa abc bfe zasddf zzsds 
Enter the string to search for: zsasddf
String "zsasddf" not found in the array.

Enter the number of strings: 5
Enter 5 strings:
zzsds
zasddf
abc
aaa
bfe
Sorted Array: aaa abc bfe zasddf zzsds 
Enter the string to search for: zsasddf
String "zsasddf" not found in the array.
*/