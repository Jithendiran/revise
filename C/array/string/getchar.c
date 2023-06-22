/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void input(char *p)
{
    char ch;
    while ((ch = getchar()) != '\n')
    {
        *p = ch;
        ++p;
    }
    *p = '\0';
}

void main()
{
    char a[100];
    // char *a; // or// char a[100];
    /**
     * getchar() is used to read one character
     */
    // char b;
    // printf("Enter Char : ");
    // b = getchar(); // a
    // printf("Entered char : ");
    // putchar(b); // a
    // printf("\n");
    // putchar(65); // A
    // printf("\n");

    // custom input function
    printf("Enter string (enter (\\n) will end string) ");
    input(a);
    printf("Entered value : %s\n", a);
}