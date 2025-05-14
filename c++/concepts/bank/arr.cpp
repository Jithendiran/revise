#include <iostream>
#include <cstring>  // For strcpy and strlen

using namespace std;

// Function to perform Bubble Sort on strings
void bubbleSort(char** arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                // Swap strings using temporary pointer
                char* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform Linear Search on strings
int linearSearch(char** arr, int size, const char* target) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(arr[i], target) == 0) {
            return i;  // Return the index if found
        }
    }
    return -1;  // Return -1 if not found
}

int main() {
    int size;

    // Input the size of the array
    cout << "Enter the number of strings: ";
    cin >> size;

    // Dynamically allocate memory for the array of pointers to strings
    char** arr = new char*[size]; // or char* arr[]

    // Input the strings and dynamically allocate memory for each string
    cout << "Enter " << size << " strings:" << endl;
    for (int i = 0; i < size; ++i) {
        string input;
        cin >> input;

        // Dynamically allocate memory for each string
        arr[i] = new char[input.length() + 1];  // +1 for null terminator
        strcpy(arr[i], input.c_str());  // Copy the C++ string into the C-style string
    }

    // Perform Bubble Sort on the array of strings
    bubbleSort(arr, size);

    // Output the sorted array
    cout << "Sorted Array: ";
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Input the string to search for
    string target;
    cout << "Enter the string to search for: ";
    cin >> target;

    // Perform Linear Search for the target string
    int index = linearSearch(arr, size, target.c_str());

    if (index != -1) {
        cout << "String \"" << target << "\" found at index " << index << endl;
    } else {
        cout << "String \"" << target << "\" not found in the array." << endl;
    }

    // Deallocate memory for each string and the array of pointers
    for (int i = 0; i < size; ++i) {
        delete[] arr[i];  // Deallocate memory for each string
    }
    delete[] arr;  // Deallocate memory for the array of pointers

    return 0;
}
