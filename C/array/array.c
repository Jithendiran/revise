#include <stdio.h>
void main()
{
    // 2 -> row, 3 -> column
    int a[2][3] = {1, 2, 3, 4, 5, 6};
    int a1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int a2[][2] = {1, 2, 3, 4}; // here row is optional

    // 3d
    int b[2][2][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int b1[2][2][3] = {
        {{1, 2, 3}, {4, 5, 6}},
        {{7, 8, 9}, {10, 11, 12}}};
    int b2[][2][2] = {1, 2, 3, 4, 5, 6}; // here also 1st position is optional
}