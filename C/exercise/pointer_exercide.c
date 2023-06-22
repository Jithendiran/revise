/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
int f(int x, int *py, int **ppz)
{
    int y, z;
    **ppz += 1;                                                                    // 5
    z = **ppz;                                                                     // 5
    *py += 2;                                                                      // 7
    y = *py;                                                                       // 7
    x += 3;                                                                        // 7
    printf("x : %d, *py : %d, **ppz : %d, y : %d, z : %d\n", x, *py, **ppz, y, z); // x : 7, *py : 7, **ppz : 7, y : 7, z : 5
    return x + y + z;                                                              // 7 + 7 + 5 => 19
}
void main()
{
    int c, *b, **a;
    c = 4, b = &c, a = &b;
    printf("%d\n", f(c, b, a)); // 19
}