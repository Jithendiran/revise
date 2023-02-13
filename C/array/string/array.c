#include <stdio.h>
void main()
{
    char a[][5] = {'abcd', 'abc', 'a'};
    /**
     * how a stores
     * index -> 0   1   2   3   4
     *  0    -> a   b   c   d   \0
     *  1    -> a   b   c   \0  \0
     *  2    -> a   \0  \0  \0  \0
     */

    char *b[] = {'abcd', 'abc', 'a'};

    /**
     * how b stores (b is an array of pointer)
     *
     *  0    -> abcd\0
     *  1    -> abc\0
     *  2    -> a\0
     */
}