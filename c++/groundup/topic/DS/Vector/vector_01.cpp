#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm> // Required for std::find
#include <ranges>

int main() {
    // 1. Initialization and Capacity Management
    // Declares an empty vector. Size = 0, Capacity = 0
    std::vector<int> numbers;
    
    // Explicitly allocates memory for 5 elements to prevent overhead from automatic growth reallocations
    numbers.reserve(5);
    
    // 2. Modifiers: Adding Elements
    // Appends elements to the end of the continuous memory block
    numbers.push_back(10);
    numbers.push_back(20);
    
    // Constructs the element directly in place inside the vector memory, avoiding a copy operation
    numbers.emplace_back(30); 
    numbers.emplace_back(40);
    numbers.emplace_back(50);

    // 3. Size and Capacity Inspection
    std::cout << "Current Vector Size: " << numbers.size() << "\n";          // Outputs: 5
    std::cout << "Current Vector Capacity: " << numbers.capacity() << "\n";  // Outputs: 5

    numbers.emplace_back(60);
    std::cout << "Current Vector Size: " << numbers.size() << "\n";          // Outputs: 6
    std::cout << "Current Vector Capacity: " << numbers.capacity() << "\n";  // Outputs: 10

    // 4. Element Access Methods
    // Fast, direct access via subscript operator (No bounds checking)
    std::cout << "Element at index 0 (operator[]): " << numbers[0] << "\n"; 
    
    // Bound-checked access via .at() method
    try {
        std::cout << "Element at index 2 (.at()): " << numbers.at(2) << "\n";
        
        // This line will deliberately trigger an exception
        std::cout << "Out of bounds access: " << numbers.at(10) << "\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception caught successfully: " << e.what() << "\n";
    }

    // Direct access to first and last elements
    std::cout << "First Element (.front()): " << numbers.front() << "\n";
    std::cout << "Last Element (.back()): " << numbers.back() << "\n";

    // 5. Advanced Modifiers: Insertion and Erasure
    // Inserts the value 15 at index 1. Requires shifting elements (20, 30, 40, 50, 60) to the right.
    numbers.insert(numbers.begin() + 1, 15); 
    
    // Removes the element at index 3. Requires shifting subsequent elements to the left.
    numbers.erase(numbers.begin() + 3); 

    // Removes the final element (60) without shifting overhead
    numbers.pop_back(); 

    // 6. Iterating over the Vector
    std::cout << "Final Vector Elements: ";
    for (int value : numbers) {
        std::cout << value << " ";
    }
    std::cout << "\n";

    // 7. Memory Clearing
    numbers.clear(); // Resets size to 0, but retains the allocated capacity
    std::cout << "Size after clear(): " << numbers.size() << "\n";         // Outputs: 0
    std::cout << "Capacity after clear(): " << numbers.capacity() << "\n"; // Outputs: 10

    //-----------------------------------------------------------------------------------------------------
    std::vector<int> vec_find = {10, 20, 30, 40};
    
    // Searches for the value 30
    auto it_find = std::find(vec_find.begin(), vec_find.end(), 30);
    
    if (it_find != vec_find.end()) {
        std::cout << "Element found at index: " << std::distance(vec_find.begin(), it_find) << "\n";
    }

    //------------------------------------------------------------------

    std::vector<int> vec_rm = {1, 2, 3, 4, 5, 6};
    
    // Filter out all odd numbers in-place
    auto new_end = std::remove_if(vec_rm.begin(), vec_rm.end(), [](int n) { return n % 2 != 0; });
    vec_rm.erase(new_end, vec_rm.end());

    //-------------------------------------------------------------------
    std::vector<int> vec_filter = {1, 2, 3, 4, 5, 6};
    
    // Filters even numbers dynamically without copying the vector
    auto even_view = vec_filter | std::views::filter([](int n) { return n % 2 == 0; });
    
    for (int n : even_view) {
        std::cout << n << " "; // Outputs: 2 4 6
    }
    std::cout << "\n";
    return 0;
}