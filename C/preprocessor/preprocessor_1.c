/**
 * Author: E.K. Jithendiran
 *
 * to see only preprocessor compile: gcc -E preprocessor_1.c will produce  .i file
 * to see only object code compile: gcc -c preprocessor_1.c will produce .o file
 * execute to see full stage output  compile: gcc -save-temps -c -o preprocessor_1.o preprocessor_1.c
 */
#include <stdio.h>
// Include will copy the content of stdio.h and paste in this file
// preprocessors will not end with ';'
#define PI 3.14

#define add(x, y) x + y
#define mul(x, y) x *y
#define greater(x, y)                 \
    if (x > y)                        \
        printf("%d is greater\n", x); \
    else                              \
        printf("%d is greater\n", y);

// \ is used to break the current line and continue in second line

void main()
{
    // float PI = PI; error
    float pi = PI;
    printf("%f, %f\n", PI, pi); // 3.140000, 3.140000

    printf("Add : %d\n", add(3, 4)); // Add : 7
    /**
     * Preprocessor will replace  add(3, 4) to 3 + 4
     */
    greater(4, 5); // 5 is greater
    greater(6, 5); // 6 is greater
                   /**
                    * Preprocessor will replace greater(6,5) to
                    *
                    *  if (6 > 5)                        \
                       printf("%d is greater\n", 6);     \
                        else                              \
                       printf("%d is greater\n", 5);
                   */
    // this type of function will faster than normal function, because no need of new stack is required

    printf("%d\n", mul(2 + 2, 3 + 3)); // 11 not 24
/**
 *  mul(2+2,3+3); will be replace to
 *
 * 2 + 2 * 3 + 3
 * so based on the precedence * will eveluate 1st
 *  result will be 6 + 3 + 2 = 11
 */
#undef add
    //    add(3, 4); // error:  undefined reference to `add'

    // some pre defined macros are there like __DATE__, __TIME__
    printf("%s\n", __TIME__); // 14:42:46
    // this time is not current time, This time is the time when preprocess encounter __TIME__
    sleep(1);
    printf("%s\n", __TIME__); // 14:42:46

#if TRUE
    printf("1\n"); // this will not print
#endif

#define TRUE 1

    // the above perprocessor will execute compile

#ifdef TRUE
    printf("2\n");
#endif

// printf("Sub = %d\n",sub(4,3)); // error
#ifndef sub
    printf("3\n");
#define AA "aa"
#define sub(a, b) a - b
#else
    printf("undefined");
#endif
    printf("Sub = %d\n", sub(4, 3)); // Sub = 1

    printf("AA = %s\n", AA); // AA = aa

/**
 *Op
    2
    3
*/
#ifndef AA
    printf("Not defined AA\n");
#else
    printf("defined AA\n"); // defined AA
#endif

#define FALSE 0
#ifndef AA
    printf("Not defined AA\n");
#elif FALSE
    printf("FALSE AA\n");
#elif !FALSE
    printf("! FALSE will execute\n"); // ! FALSE will execute
#else
    printf("nothing\n");
#endif

#define AA 1            // it redeclare the AA
    printf("%d\n", AA); // 1

// # in macro is string conversion
#define msg(a, b) printf(#a " and " #b ", Hello\n");
                        //   printf(##AA); // error

    // # will replace the value
    msg("aa", 'b'); // "aa" and 'b', Hello
    msg(aa, b);     // aa and b, Hello

    // ## in macro is looking for variable
    // if n is 1 then looking for variable st1
    // if n is 2 then looking for variable st2

#define parser(n) printf("value of st{num} = %d n = " #n " \n", st##n);
    // parser(1); // error
    int st1 = 2;
    parser(1); // value of st{num} = 2 n = 1
    // parser(2); // error: 'st2' undeclared
}