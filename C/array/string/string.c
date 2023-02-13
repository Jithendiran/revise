#include <stdio.h>
int main()
{
    printf("%s\n", "hello");                              // hello
    printf("%10s\n", "hello");                            //      hello
    printf("%10s\n", "hello do it Today when u r ready"); // hello do it Today when u r ready
    printf("%s\n", "Hi\
                  ji");                                   // Hi                  ji
    // \ is used to break the current line and continue in second line
    printf("%s\n", "hi", "ji"); // hi
    printf("%s\n", "hi"         // no comma here
                   "ji");       // OP: hiji

    /**
     * string are array of character
     * String are end with \0 null character
     * Strings must be enclosed inside double quotes ""
     * Eg: let see how "ji" is stored
     * character is each 1 byte assume starting address 1000
     * j -> 1000, i -> 1001, \0 -> 1002
     * total 3 bytes required to store ji
     * These 3 bytes are read only, But each character is changeable
     * String returns starting address and it evaluate or process until it encounter \0 null character
     * \0 null character is added by compiler
     * */
    char a[] = "ji";
    char a1[3] = {'j', 'i', '\0'};
    // a = "hi";  error: assignment to expression with array type
    a[0] = 'h';
    printf("a : %s, a1: %s\n", a, a1); // a : hi, a1: ji
    char *b = "jith";
    printf("%s\n", b);               // jith
    printf("%d\n", "n" == 'n');      // 0 (because string will return start address but character will return ASCII value)
    printf("%.5s\n", "HelloWorld");  // Hello (want to print 5)
    printf("%6.5s\n", "HelloWorld"); //  Hello (with 1 gap from start)
    /**
     * puts
     * It is used to display string in OP
     * After printing string puts automatically moves to new line
     * It's declaration in stdio.h
     */
    puts(a); // hi
    puts(a); // hi
    char a2[10];
    printf("Enter string : ");
    scanf("%s", a2); // jidesh ji
    // scanf("%2s", a2);           // (2 indicates how much character need to store)
    printf("\na2 : %s\n", a2); // jidesh

    // printf("Enter string : ");
    // char *b1;
    // scanf("%s", b1);           // jith
    // printf("\nb1 : %s\n", b1); // null

    /**
     * scanf dosen't store space it will terminate when it encounter space
     * gets is decleared in stdio.h
     * gets will store data beyond allocated sized if the allocated size is exceed and some part of string need to store means gets will overwrite next block of memory which is unsafe
     *
     */
    char c[10];
    printf("Enter c \n");
    gets(c);
    puts(c);
}