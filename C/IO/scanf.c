/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
    int a;
    printf("Enter a value : ");
    scanf("%d", &a);             // 27        // address of a
    printf("A value : %d\n", a); // content of a

    float f;
    char c;
    char s[20];
    scanf("%f, %c, %s", &f, &c, s);             // 3.4, a, hello
    printf("f : %f, c : %c, s: %s\n", f, c, s); // f : 3.400000, c : a, s: hello
}