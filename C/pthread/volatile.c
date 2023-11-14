/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// bool done = false;
volatile bool done = false;

void *fun()
{
    sleep(1);
    done = true;
    return NULL;
}

void main()
{
    pthread_t t;
    pthread_create(&t, NULL, fun, NULL);
    printf("Waiting.......\n");
    while (!done)
    {
    }
    printf("Completed......\n");
}

/**
 * Compile: gcc volatile.c
 *
 * Op
    Waiting.......
    Completed......
*/

/**
 * Without volatile
 * Compile: gcc -O2 volatile.c
 *
 * Op
    Waiting.......
program will not end

to fix this `volatile bool done = false;` instead of `bool done = false;`
*/

/**
 * with volatile
 * Compile: gcc -O2 volatile.c
 *
 * Op
    Waiting.......
    Completed......
*/

/**
 * Volatile can be
 *
 * volatile int var; // var is a volatile integer
 * voilatile int* var; // var is a pointer to volatile integer
 * int *volatile var; // var is a volatile pointer to integer
 * volatile int *volatile var; // var is a volatile pointer to volatile integer
 *
 * also...
 * volatile struct var { volatile int a;}
 */