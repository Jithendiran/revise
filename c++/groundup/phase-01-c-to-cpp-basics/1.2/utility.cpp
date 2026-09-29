// utility.hpp
#ifndef UTILITY_HPP
#define UTILITY_HPP

// ERROR STATE: If compiled like this, it causes a linker error.
int calculate_total(int items) {
    return items * 10;
}

// FIX STATE: To resolve the error, uncomment the line below and comment out the line above.
// inline int calculate_total(int items) { return items * 10; }

#endif