/**
 * Author: E.K.Jithendiran
 * Date  : 4 Apr 2025
 */
// g++ singleton.cpp -o singleton.o

#include <iostream>
using namespace std;

struct singleton{

    singleton(const singleton&) = delete;            // prevent copy
    singleton& operator=(const singleton&) = delete; // prevent assignment
    // no need to prevent move

    static singleton* getInstance(){
        if(obj == NULL) {
            obj = new singleton();
        }
        return obj;
    }

    private:
        inline static singleton* obj = nullptr;
        singleton(){
            cout << "Singleton cons" << endl;
        }
};

// singleton* singleton::obj = nullptr; // or inline is mandatory// without this cause linker error

/*
/usr/bin/ld: /tmp/ccIVXQsK.o: warning: relocation against `_ZN9singleton3objE' in read-only section `.text._ZN9singleton11getInstanceEv[_ZN9singleton11getInstanceEv]'
/usr/bin/ld: /tmp/ccIVXQsK.o: in function `singleton::getInstance()':
singleton.cpp:(.text._ZN9singleton11getInstanceEv[_ZN9singleton11getInstanceEv]+0x10): undefined reference to `singleton::obj'
/usr/bin/ld: singleton.cpp:(.text._ZN9singleton11getInstanceEv[_ZN9singleton11getInstanceEv]+0x37): undefined reference to `singleton::obj'
/usr/bin/ld: singleton.cpp:(.text._ZN9singleton11getInstanceEv[_ZN9singleton11getInstanceEv]+0x3e): undefined reference to `singleton::obj'
/usr/bin/ld: warning: creating DT_TEXTREL in a PIE
collect2: error: ld returned 1 exit status
*/

int main() {

    // singleton s; // ‘singleton::singleton()’ is private within this context
    singleton *s, *s1;
    s = singleton::getInstance();
    s1 = singleton::getInstance();
    cout << "Addr " << s << "\t" << s1 << endl;
}