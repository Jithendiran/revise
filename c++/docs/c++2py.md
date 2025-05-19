# extern "C"
Call c++ class data members and member function from c or other programming languages like python 

> [!TIP]   
> Why is this needed?  
> C++ compilers mangle function names to support: Function overloading/Namespaces/Type safety  
> C++ will convert function names to some thing like below while compiling  
> `int add(int a, int b);` -> `_Z3addii`   
> `extern "C"` disables mangling  
> `extern "C" int add(int a, int b);` Now the compiled symbol name will be just `add`, it can linked with c/python/..

## Usage  

### Wrapping a single functio
```c++
extern "C" int add(int a, int b);
```

### Wrapping multiple functions:
```c++
extern "C" {
    int add(int a, int b);
    void greet(const char* name);
}
```

Generate shared lib file and use them in other programming languages

>[!CAUTION]  Can't use extern "C" with:  
> Overloaded functions  
> Classes directly (but you can wrap class methods)  

eg:
```c++
// lib.cpp
#include <iostream>

class MyClass {
public:
    void greet() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

// Wrap with extern "C"
extern "C" {
    void* MyClass_create() {
        return new MyClass();
    }

    void MyClass_greet(void* obj) {
        static_cast<MyClass*>(obj)->greet();
    }

    void MyClass_destroy(void* obj) {
        delete static_cast<MyClass*>(obj);
    }
}

```

```c++
// lib.h
#ifndef MYLIB_H
#define MYLIB_H

#ifdef __cplusplus
extern "C" {
#endif

void* MyClass_create();
void  MyClass_greet(void*);
void  MyClass_destroy(void*);

#ifdef __cplusplus
}
#endif

#endif // MYLIB_H
```

### not a shared lib
`g++ -c lib.cpp -o lib.o` 

### shared lib
`g++ -fPIC -shared lib.cpp -o libmyclass.so`

C File  
```c
// main.c
#include "lib.h"

int main() {
    void* obj = MyClass_create();
    MyClass_greet(obj);
    MyClass_destroy(obj);
    return 0;
}

```
### non shared lib  
> `gcc main.c lib.o -lstdc++ -o main.out`  


### shared lib
> `gcc main.c -L. -lmyclass -o main.out`  
> `LD_LIBRARY_PATH=. ./main.out`  

> `./main.out`

py file [For python c++ file must be as shared lib]

```py
# main.py
import ctypes
import os

# Load the shared library
lib = ctypes.CDLL('./libmyclass.so')

# Define opaque pointer type
class MyClass_C(ctypes.Structure):
    pass

# Set argument and return types
lib.MyClass_new.restype = ctypes.POINTER(MyClass_C)
lib.MyClass_greet.argtypes = [ctypes.POINTER(MyClass_C)]
lib.MyClass_delete.argtypes = [ctypes.POINTER(MyClass_C)]

# Use the library
obj = lib.MyClass_new()
lib.MyClass_greet(obj)
lib.MyClass_delete(obj)

```
