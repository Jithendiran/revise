/*
 Properties of std::vector:
 - Dynamic array: stores elements in contiguous memory
 - Supports random access (operator[] and at())
 - Fast access by index (O(1))
 - Efficient insertion/removal at the end (push_back/pop_back) (amortized O(1))
 - Insertion/removal at positions other than end is slow (O(n))
 - Automatically resizes (capacity grows as needed)
 - May reallocate and move elements when resizing (invalidates pointers/iterators)
 - Provides size() and capacity() functions
 - Cache friendly due to contiguous storage
*/

/*
size(): Returns the number of elements currently stored in the vector.
capacity(): Returns the number of elements the vector can store without reallocating memory.
*/

#include <iostream>
#include <vector>
using namespace std;

int main() {
    // Create an empty vector of integers
    vector<int> v;

    cout << "Size: " << v.size() << endl; // Size: 0
    cout << "Capacity: " << v.capacity() << endl; // Capacity: 0

    // Add elements
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // Access elements
    cout << "First element: " << v[0] << endl; // First element: 10
    cout << "Second element: " << v.at(1) << endl; // Second element: 20

    // Iterate using range-based for loop
    cout << "All elements: "; // All elements: 10 20 30
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;

    // Size and capacity
    cout << "Size: " << v.size() << endl; // Size: 3
    cout << "Capacity: " << v.capacity() << endl; // Capacity: 4

    // Remove last element
    v.pop_back();

    // Iterate using iterator
    cout << "After pop_back: ";
    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    // After pop_back: 10 20 
    cout << endl;

    cout << "Size: " << v.size() << endl; // Size: 2
    cout << "Capacity: " << v.capacity() << endl; // Capacity: 4

    // resize()             // Changes the number of elements in the vector.
    // back()               // Returns a reference to the last element.
    // front()              // Returns a reference to the first element.
    // clear()              // Removes all elements from the vector.
    // insert(pos, num)     // Inserts elements at the specified position.
    // erase(v.begin())     // Erase element at the specified position.
    // pop_back()           // Erase element at the last position.
    return 0;
}