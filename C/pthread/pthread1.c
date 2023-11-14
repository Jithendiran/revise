#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// to compile gcc -g -pthread pthread1.c -o pthread.o
void *routine()
{
    printf("Inside routine\n");
    sleep(2);
    printf("End of routine\n");
}

// passing arguments and returning results
// it accepts only one arguments that one as void pointer
void *routine1(void *arg)
{
    printf("Inside routine 1 with arg %d\n", *((int *)arg));
    int *ret = (int *)malloc(sizeof(int));
    *ret = 12;
    return ret;
}
void main()
{
    pthread_t t;
    if (pthread_create(&t, NULL, &routine, NULL) != 0)
    {
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", i);
    }

    printf("in main\n");
    // wait until thread execution complete
    if (pthread_join(t, NULL) != 0)
    {
        return;
    }
    printf("Completed\n");
    if (pthread_detach(t) != 0)
    {
        // free resource
        printf("Resource freeed\n");
    }

    // passing arguments and returning results
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = 27;
    if (pthread_create(&t, NULL, &routine1, ptr) != 0)
    {
        return;
    }
    if (pthread_join(t, (void *)&ptr) != 0) // 2nd argument is double pointer so pass '&' address of ptr
    {
        return;
    }
    printf("return thread value = %d\n", *ptr);
    if (pthread_detach(t) != 0)
    {
        // free resource
        printf("Resource freeed\n");
    }
}

/**
 *Op
 0
1
Inside routine
2
3
4
5
6
7
8
9
in main
End of routine
Completed
Resource freeed
Inside routine 1 with arg 27
return thread value = 12
Resource freeed
*/