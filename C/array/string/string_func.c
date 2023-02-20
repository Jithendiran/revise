/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
#include <string.h>
void main()
{
    char a[10] = "hi jith", b[10];
    // string copy
    strcpy(b, a);          // strcpy(destination, source)
    printf("b : %s\n", b); // hi jith
    char a1[6] = "hello", b1[4];
    strncpy(b1, a1, sizeof(b1)); // strncpy(destination, source, size of destination) // only copy until available space in destination it will not add '\0'
    b1[4] = '\0';
    printf("b1 : %s\n", b1); // hell

    // string length
    printf("Length :%d\n", strlen("Jith")); // 4

    // string concat
    char c[10] = "abc", c1[10] = "defg";
    strcat(c, c1);
    printf("Concated :%s\n", c); // Concated : abcdefg
    // strncat is like strncpy it only concat limited number of character to destination specified by 3rd argument
    // strncat automatically add '\0' null character

    char c2[5] = "He", c3[10] = "llo!";
    strncat(c2, c3, sizeof(c2) - strlen(c2) - 1); // 3rd argument size of c2 is 5
                                                  // already occupied value is get by strlen(c2) is 2
                                                  // -1 for null char
    printf("%s\n", c2);                           // Hell

    // string compare
    printf("%d\n", strcmp("A", "a"));     // -1 (s1 < s2)
    printf("%d\n", strcmp("Aart", "aA")); // -1

    printf("%d\n", strcmp("A", "A")); // 0 (s1 == s2)

    printf("%d\n", strcmp("a", "A")); // 1 (s1 > s2)

    printf("%d\n", strcmp("abcd", "abce"));  // -1
    printf("%d\n", strcmp("d", "e"));        // -1
    printf("%d\n", strcmp("abcd", "abcde")); // -1
}