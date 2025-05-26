#include<iostream>
#include<list>
using namespace std;
int main(){
    list<int> l;
    /*
    // Properties of std::list:
    // - Doubly linked list (each node has prev and next pointers)
    // - efficient for insert and remove, not for searching
    // - Efficient insert/erase at any position (O(1) with iterator)
    // - No random access (no operator[] or at())
    // - No pre-allocated capacity, grows node-by-node
    // - Stable iterators (except for erased elements)
    // - Bidirectional traversal (begin(), end(), rbegin(), rend())
    // - Not cache friendly (nodes are scattered in memory)
    */
   l.push_back(0);
   l.push_front(1); // push as 1st element
   l.push_back(2); // push at last

    for(int i:l){
        cout << i << endl;
    }
    /*
    1
    0
    2
    */

    l.push_back(3);

    // iterator are like pointers
    // begin will take the 1st element address
    for(list<int>::iterator it = l.begin(); it != l.end(); it++){
        cout << *it << endl;
    }
    /*
    1
    0
    2
    3
    */
    cout<<endl;
    // to delete
    l.erase(l.begin()); // it will remove the first element from the list and also deallocate the memory used for that int node.
    // internal pointer will update to next element as 1st

    for(list<int>::iterator it = l.begin(); it != l.end(); it++){
        *it = *it+1;
        cout << *it << endl;
    } // 1 3 4
    cout<<endl;
    list<int> l1 = {2,4,2};

    // const_iterator is more secure
    for(list<int>::const_iterator it = l1.begin(); it != l1.end(); it++){
        // *it = *it+1; NOT ALLOWED due to const_iterator
        cout << *it << endl;
    } // 2 4 2

}