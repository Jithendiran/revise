#include <iostream>
#include <vector>
#include <initializer_list>

// Standard container initialization uses initializer_list
std::vector<int> numbers = {10, 20, 30, 40}; // The compiler creates an initializer_list here

// Direct use in a function
void print_list(std::initializer_list<std::string> words) {
    for (const auto& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";
}

class MySet
{
public:
    std::vector<int> elements;

    MySet(){
        std::cout << "Default constructor.\n";
    }

    MySet(std::vector<int> ele){
        std::cout << "constructor.\n";
    }

    // This is the initializer_list constructor
    MySet(std::initializer_list<int> list) {
        std::cout << "Using MySet initializer_list constructor.\n";
        for (int value : list) {
            elements.push_back(value);
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Calling the function using the braced-list syntax
    print_list({"Apple", "Banana", "Cherry"});
    MySet s = {5, 10, 15}; // Calls the initializer_list constructor
    return 0;
}

// g++ initializer_list.cpp -o /tmp/initializer_list