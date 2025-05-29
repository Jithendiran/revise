#include <iostream>
#include <thread>
#include <unistd.h> // for getpid() and sleep
using namespace std;

// Thread function without parameter
void threadFunc1()
{
    cout << "Thread 1 (no param), PID: " << getpid() << endl;
    cout << "Thread 1 id: " << std::this_thread::get_id() << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << "--";
        usleep(100000);
    }

    cout << "Thread 1 finished\n";
}

// Thread function with parameter
void threadFunc2(int x)
{
    cout << "Thread 2 (param=" << x << "), PID: " << getpid() << endl;
    cout << "Thread 2 id: " << std::this_thread::get_id() << endl;
    for (int i = 0; i < x; i++)
    {
        cout << ">>";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "Thread 2 finished\n";
}

void detach_th()
{
    cout << "Detach Thread  , PID: " << getpid() << endl;
    cout << "Detach Thread  id: " << std::this_thread::get_id() << endl;
    for (int i = 0; i < 50; i++)
    {
        cout << "<<";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "Detach Thread  finished\n";
}

int main()
{
    cout << "Main thread, PID: " << getpid() << endl;

    // Start threads
    thread t1(threadFunc1);
    thread t2(threadFunc2, 42);
    thread t3(detach_th);

    t3.detach();
    /*
    Note:
    - Detached threads run independently; main may finish before they do.
    - If main exits before threads finish, their output may not appear.
    - Resources are released automatically when finished.
    */

    // Wait for threads to finish
    t1.join();
    t2.join();

    cout << "Main thread finished\n";
    return 0;
}
/*
Main thread, PID: 6536
Thread 1 (no param), PID: 6536
Thread 1 id: 140737345746496
--Thread 2 (param=42), PID: 6536
Thread 2 id: 140737337353792
>>Detach Thread  , PID: 6536
Detach Thread  id: 140737328961088
<<-->><<-->><<-->><<>>--<<>>--<<>>--<<-->><<-->><<>>--<<>>Thread 1 finished
<<<<>><<>>>><<>><<<<>>>><<<<>>>><<>><<<<>>>><<<<>><<>>>><<<<>>>><<>><<>><<>><<<<>><<>><<>><<>>>><<>><<<<>>>><<>><<>><<>><<>><<<<Thread 2 finished
Main thread finished
*/