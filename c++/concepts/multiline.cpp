/**
 * Author: E.K.Jithendiran
 * Date  : 22 Apr 2025
 */
// g++ multiline.cpp -o multiline.o
#include <iostream>
#include <string>
using namespace std;

int main() {
  std::string line;
  std::cout << "Enter multiple lines of text :\n";
  while (std::getline(std::cin, line)) {
    std::cout << "You entered: " << line << std::endl;
  }
  return 0;
}