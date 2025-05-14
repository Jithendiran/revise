#include <iostream>
#include <ctime>  // For system time
using namespace std;

class Time {
private:
    int totalSeconds;

public:
    // Constructor to initialize totalSeconds
    Time(int seconds) : totalSeconds(seconds) {}

    // Function to convert seconds to HH:MM:SS format
    void convertToHHMMSS() {
        // 60 sec * 60 min = 3600 sec
        int hours = totalSeconds / 3600; // 1 hour = 3600 seconds
        totalSeconds = totalSeconds % 3600;
        int minutes = totalSeconds / 60; // Remaining minutes
        int seconds = totalSeconds % 60; // Remaining seconds

        cout << "Time: " << hours << " hours " << minutes << " minutes " << seconds << " seconds" << endl;
    }
};

int main() {
    // Get the current time in seconds since the Unix epoch (January 1, 1970)
    time_t currentTime = time(0);

    cout << "Time in seconds : " << currentTime << endl;


    // Create an object of Time class with the current time in seconds
    Time time(currentTime);

    // Convert and display the time in HH:MM:SS format
    time.convertToHHMMSS();

    int time0;
    cout << "Eter time in sec : ";
    cin >> time0;
    Time time1(time0);
    time1.convertToHHMMSS();
    return 0;
}
/*
Time in seconds : 1747196737
Time: 485332 hours 25 minutes 37 seconds

Eter time in sec : 3800
Time: 1 hours 3 minutes 20 seconds
*/