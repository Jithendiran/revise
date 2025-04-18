/**
 * Author: E.K.Jithendiran
 * Date  : 18 Apr 2025
 */
// g++ templates.cpp -o templates.o
#include <memory>
#include <iostream>
using namespace std;

// Tamplates let us write generic code that works with any type
// template <> must be declared above the usage

// function template
template <typename T>
T add(T a, T b)
{
    return a + b;
}
//-----------------------------------------------------------------------------------------------------
// class template
template <typename T> // without this line compiler throw error for bleow class
struct MyClass
{
    T value;
    MyClass(T a) : value(a) {}
};
//-----------------------------------------------------------------------------------------------------
// with multiple types
template <typename T1, typename T2>
struct MyClass2
{
    T1 a;
    T2 b;
    MyClass2(T1 a, T2 b = 1) : a(a), b(b) {}
};
//-----------------------------------------------------------------------------------------------------
// default template arguments
template <typename T1 = int>
struct MyDefaultType0
{
    T1 a;

    MyDefaultType0(T1 a = 15) : a(a) {}
};

template <typename T1, typename T2 = int>
struct MyDefaultType
{
    T1 a;
    T2 b;
    MyDefaultType(T1 a, T2 b = 10) : a(a), b(b) {}
};
//-----------------------------------------------------------------------------------------------------
// Template Specialization
template <typename T>
struct Printer
{ // this class for all types
public:
    void print(T val) { cout << "General printer : " << val << endl; }
};

// Specialization for bool
template <>
struct Printer<bool>
{ // this class only for bool
public:
    void print(bool val)
    {
        cout << "Bool printer : " << (val ? "True" : "False") << endl;
    }
};
//-----------------------------------------------------------------------------------------------------
// classs function template
struct Math
{
public:
    template <typename T>
    T square(T x)
    {
        return x * x;
    }
};
//-----------------------------------------------------------------------------------------------------
// Template Inheritance
template<typename T>
class Container {
protected:
    T* arr;
    int size;
public:
    Container(int s) : size(s) {
        cout << "Container cons" << endl;
        arr = new T[size];
    }
    virtual ~Container() {
        cout << "Container des" << endl;
        delete[] arr; 
    }

    void set(int index, T value) {
        arr[index] = value;
    }

    T get(int index) const {
        return arr[index];
    }
};

// A stack using the container
template <typename T>
class Stack : public Container<T> {
    int top = -1;
public:
    Stack(int s) : Container<T>(s) {
        cout << "Stack cons" << endl;
    }

    void push(T val) {
        if (top < this->size - 1)
            // set(top, val);
            // top++;
            this->arr[++top] = val;
    }

    T pop() {
        return (top >= 0) ? this->arr[top--] : T();
    }
    bool isEmpty() const {
        return top == -1;
    }
    ~Stack(){
        cout << "Stack des" << endl;
    }
};

class Student {
    string name;
    int id;
public:
    Student(string n = "NoName", int i = 0) : name(n), id(i) {}

    void display() const {
        cout << "Student(Name: " << name << ", ID: " << id << ")" << endl;
    }
};
//-----------------------------------------------------------------------------------------------------
// Non-Type Template
template <int N> 
struct FixedArray {
    int arr[N];
public:
    void printSize() {
        cout << "Size = " << N << endl;
    }
};
//-----------------------------------------------------------------------------------------------------
// Variadic templates
template<typename T>
void printv(T val) {
    cout << val << " ";
}

template<typename T, typename... Args>
void printv(T first, Args... rest) {
    cout << first << " ";
    printv(rest...);
    cout << endl;
}
//-----------------------------------------------------------------------------------------------------
// SFINAE (Enable_if, Used to conditionally compile functions or overloads:) 
template<typename T>
typename enable_if<is_integral<T>::value>::type
onlyIntegers(T t) {
    cout << "Integer: " << t << endl;
}
//-----------------------------------------------------------------------------------------------------

int main()
{

    cout << add(2, 3) << endl; // int
    // op: 5
    cout << add(2.5, 3.1) << endl; // double
    // op: 5.6

    // template class
    MyClass o1(1);            // or MyClass o1(1) ;
    cout << o1.value << endl; // 1

    MyClass<string> o2("aa"); // or MyClass o2("aa");
    cout << o2.value << endl; // aa

    MyClass2 o21(2, "bb");                   // or MyClass2<int, string> o21(2, "bb");
    cout << o21.a << " , " << o21.b << endl; // 2 , bb

    // MyClass2 o22("BB"); // error: class template argument deduction failed
    MyClass2<const char*, int> o22("BB"); // OK
    std::cout << o22.a << ", " << o22.b << std::endl; // BB, 1

    MyClass2<int, float> obj1(10, 2.5);
    std::cout << obj1.a << ", " << obj1.b << std::endl; // 10, 2.5
    
    MyClass2<int, int> obj2(5); // b = 1 by default
    std::cout << obj2.a << ", " << obj2.b << std::endl; // 5, 1
    
    // MyClass2<int, std::string> obj3(10); // '1' can't convert to std::string
    // MyClass2<int> obj3(2); //  error: wrong number of template arguments (1, should be 2)

    // auto deducation will resolve the above issue

    cout << "---------------------------" << endl;
    
    MyDefaultType0 m00;
    cout << m00.a << endl ; // 15

    MyDefaultType0<> m11;
    cout << m11.a << endl ; // 15

    MyDefaultType0 m22("BB"); 
    cout << m22.a << endl ; // BB

    // MyDefaultType0<int> m33("BB"); // error: invalid conversion from ‘const char*’ to ‘int’ [-fpermissive]

    cout << "---------------------------" << endl;

    MyDefaultType m0(1);
    cout << m0.a << ", " << m0.b << endl; // 1, 10

    MyDefaultType<int> m1(20);
    cout << m1.a << ", " << m1.b << endl; //20,  10

    MyDefaultType<double> m2(11.12);
    cout << m2.a << ", " << m2.b << endl; // 11.12, 10

    // MyDefaultType<> m3(11.12); // error: wrong number of template arguments (0, should be at least 1)

    Printer<int> p0;
    p0.print(1);    // General printer : 1
    p0.print(true); // General printer : 1

    Printer<bool> p1;
    p1.print(true); // Bool printer : True

    Math m;
    cout << m.square(5) << endl;     // 25
    cout << m.square(2.5) << endl;   // 6.25

    FixedArray<5> f; // creates an array of 5
    f.printSize();    // Size = 5

    printv(1, 2.5, "hello", 'c'); // 1 2.5 hello c

    onlyIntegers(10); // OK
    // onlyIntegers(2.5); // error: no matching function for call to ‘onlyIntegers(double)’

    Stack<Student> studentStack(3);

    studentStack.push(Student("Alice", 101));
    studentStack.push(Student("Bob", 102));
    studentStack.push(Student("Charlie", 103));
    studentStack.push(Student("Overflow", 999)); // triggers overflow

    while (!studentStack.isEmpty()) {
        Student s = studentStack.pop();
        s.display();
    }

    studentStack.pop(); // triggers underflow
    /*
    Container cons
    Stack cons
    Student(Name: Charlie, ID: 103)
    Student(Name: Bob, ID: 102)
    Student(Name: Alice, ID: 101)
    Stack des
    Container des
    */
}