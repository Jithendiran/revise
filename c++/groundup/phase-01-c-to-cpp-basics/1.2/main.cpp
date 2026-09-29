// main.cpp
#include "utility.hpp"

/*
int calculate_total(int items) {
    return items * 10;
}
*/

// Forward declaration of the function inside fileA.cpp
int process_batch();

int main() {
    // Calls the function directly from main.cpp
    int direct_result = calculate_total(10);
    
    // Calls the function indirectly through fileA.cpp
    int batch_result = process_batch();
    
    return 0;
}