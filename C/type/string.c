#include <stdio.h>
int main()
{
    printf("%s\n", "hello");                              // hello
    printf("%10s\n", "hello");                            //      hello
    printf("%10s\n", "hello do it Today when u r ready"); // hello do it Today when u r ready      hello
    printf("%s\n", "Hi\
                  ji");                                   // Hi                  ji
    // \ is used to break the current line and continue in second line
    printf("%s\n", "hi", "ji"); // hi
    printf("%s\n", "hi"         // no comma here
                   "ji");       // OP: hiji
}