/**
 * Author: E.K.Jithendiran
 */
// typedef is used to define user data type
#include <stdio.h>
typedef int INTEGER; // typedef(keyword) old name(anything for change name) INTEGER(typedef new name)

// typedef for structure
/**
 * typedef struct strcture_tagname{
 * properties
 * } typedef_name;
 */

typedef struct vechicle
{
    char *seat;
    float price;
} car;

void main()
{
    INTEGER a = 3, b = 3.14;
    printf("%d %d\n", a, b); // 3 3

    car c1, c2;
    c1.seat = "4";
    c2.seat = "5";
    printf("c1 %s %f\n", c1.seat, c1.price); // c1 4 -0.000000
}