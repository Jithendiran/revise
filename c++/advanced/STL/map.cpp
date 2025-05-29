// Properties of std::map:
// - Stores key-value pairs (associative container)
// - Keys are unique and sorted (by default, in ascending order)
// - Underlying data structure is usually a Red-Black Tree (self-balancing BST)
// - All operations (insert, erase, find) are O(log n)
// - Provides bidirectional iterators
// - Keys and values can be of any type (as long as keys are comparable)
// - No random access by index (no operator[] for position)
// - Elements are always sorted by key
// - Access or insert value by key using operator[]
// - Automatically creates a value if you access a non-existent key with operator[]

#include <iostream>
#include <map>
using namespace std;

int main() {
    // Create a map with int keys and string values
    map<int, string> m;

    // Insert elements
    m[1] = "one";
    m[2] = "two";
    m.insert({3, "three"});
    m.insert({0, "ZERO"}); // always store in ascending

    // Access elements
    cout << "Key 2: " << m[2] << endl;

    // Iterate using iterator
    cout << "All key-value pairs:" << endl;
    for (map<int, string>::iterator it = m.begin(); it != m.end(); ++it) {
        cout << it->first << " => " << it->second << endl;
    }
    /*
    0 => ZERO
    1 => one
    2 => two
    3 => three
    */

    // Check size
    cout << "Size: " << m.size() << endl; // Size: 4

    // Find and erase
    auto it = m.find(2);
    if (it != m.end()) {
        m.erase(it);
    }

    // Iterate using range-based for loop
    cout << "After erase:" << endl;
    for (const auto& p : m) {
        cout << p.first << " => " << p.second << endl;
    }
    /*
    0 => ZERO
    1 => one
    3 => three
    */

    // there is an another map which will not do the sorting "unordered_map"

    return 0;
}