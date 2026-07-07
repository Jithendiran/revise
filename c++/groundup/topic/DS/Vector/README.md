## Vector
A `std::vector` is a sequence container in the C++ Standard Template Library (STL) that encapsulates a dynamic array. Unlike standard arrays, which have a fixed size determined at compile-time, a `std::vector` can grow or shrink in size dynamically at runtime as elements are added or removed.

### Core Mechanics: Size, Capacity, and Growth Logic
To manage memory efficiently, a `std::vector` distinguishes between two distinct concepts: Size and Capacity.

```
[ Element 1 ] [ Element 2 ] [ Element 3 ] [ Empty Slot ] [ Empty Slot ]
|<------------------- Size (3) ----------------->|
|<--------------------------- Capacity (5) ----------------------------->|
```
* Size: The exact number of elements currently stored in the vector.
* Capacity: The total amount of space currently allocated in memory, measured in terms of how many elements it can hold without allocating new memory.

### Default Size and Capacity
When a vector is declared without any initial parameters (e.g., `std::vector<int> vec;`), its default size is `0` and its default capacity is `0`. No memory is allocated from the system heap until the first element is inserted.

### The Growth Mechanism (Reallocation)
When an element is added to a vector that has reached its maximum capacity (`size == capacity`), the vector must expand. Because memory must remain contiguous (side-by-side in a single unbroken block), the vector cannot simply append new memory to its current location, as adjacent memory addresses may already be occupied by other data.

The vector executes the following growth logic:
1. Allocation: It allocates a completely new, larger block of memory elsewhere on the heap.
2. Growth Factor: The new capacity is typically 1.5 times or 2 times the old capacity (depending on the specific compiler implementation, such as GCC or MSVC). Geometric doubling ensures that reallocation happens infrequently.
3. Element Transfer: It moves or copies the existing elements from the old memory location to the new memory location.
4. Deallocation: It releases the old memory block back to the system.
5. Insertion: It inserts the new element into the next available slot.

### Element Access Methods
Elements in a vector are stored contiguously, allowing direct access via memory offsets. C++ provides multiple ways to access these elements, balancing performance and safety.

**Subscript Operator ([])**
* Syntax: `vec[index]`
* Mechanism: Computes the memory address directly using the index offset.
* Safety: Does not perform bounds checking. If the index is out of range, it causes undefined behavior (potential memory corruption or crashes).
* Time Complexity: $O(1)$ (Constant Time)

**At Method (.at())**
* Syntax: vec.at(index)
* Mechanism: Computes the address but includes an internal check against the current size of the vector.
* Safety: If the index is out of bounds, it throws a `std::out_of_range` exception, preventing invalid memory access.
* Time Complexity: $O(1)$ (Constant Time, with a slight overhead for the check)

**Front and Back Methods (.front(), .back())**
* Syntax: `vec.front()` and `vec.back()`
* Mechanism:`front()` references the first element (index 0), while `back()` references the final element (index size - 1).
* Time Complexity: $O(1)$

### Element Modification and Insertion Methods
Adding elements to a vector varies in performance depending on where the insertion occurs.

**Insertion at the End**
* `push_back(value)`: Copies or moves the value into the slot at the end of the vector.
* `emplace_back(arguments)`: Constructs the element directly in place at the end of the vector using the provided arguments. This eliminates the temporary copy step required by `push_back`.
* Performance: $O(1)$ Amortized time. It is instantaneous unless a reallocation (growth mechanism) is triggered.

**Insertion in the Middle or Beginning**
* `insert(iterator, value)`: Places an element at a specific position defined by an iterator.
* Performance: $O(n)$ (Linear Time). Because elements must remain contiguous, all elements situated after the insertion point must be shifted one slot to the right to clear space.