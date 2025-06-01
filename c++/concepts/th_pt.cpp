#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <chrono>
#include <atomic>

using namespace std;

queue<string> messageQueue;
atomic<bool> running(true);

// Background thread: simulates saving messages to a file
void loggerThread() {
    while (running || !messageQueue.empty()) {
        this_thread::sleep_for(chrono::seconds(1));  // simulate delay

        while (!messageQueue.empty()) {
            cout << "[Logger] Saved message: " << messageQueue.front() << "\n";
            messageQueue.pop();
        }
    }
}

// Frontend thread: simulates user input
void inputThread() {
    string input;
    while (true) {
        cout << "Enter message (type 'exit' to quit): ";
        getline(cin, input);

        if (input == "exit") {
            running = false;
            break;
        }

        messageQueue.push(input);
    }
}

int main() {
    thread t1(inputThread);
    thread t2(loggerThread);

    t1.join();
    t2.join();

    cout << "Application exited.\n";
    return 0;
}
