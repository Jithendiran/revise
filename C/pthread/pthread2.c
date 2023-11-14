/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int rvalue = 0;
int rvalueLock = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *raceCondition(void *arg)
{
    // rvalue = 0;
    for (int i = 0; i < 100000; i++)
    {
        rvalue++;
    }

    return NULL;
}
void *raceConditionWithLock(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&lock);
        rvalueLock++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}
int main()
{
    pthread_t t1;
    if (pthread_create(&t1, NULL, raceCondition, NULL) != 0)
    {
        return 1; // error
    }
    raceCondition(NULL); // calling in main thread
    if (pthread_join(t1, NULL))
    {
        return 1;
    }
    printf("EXPECTED VALUE 200000 GOT VALUE = %d\n", rvalue); // EXPECTED VALUE 200000 GOT VALUE = 123780

    /**
     * The above result is wrong but the both thread executed 100000 times each
     */

    if (pthread_create(&t1, NULL, raceConditionWithLock, NULL) != 0)
    {
        return 1; // error
    }
    raceConditionWithLock(NULL); // calling in main thread
    if (pthread_join(t1, NULL))
    {
        return 1;
    }
    printf("EXPECTED VALUE 200000 GOT VALUE = %d\n", rvalueLock); // EXPECTED VALUE 200000 GOT VALUE = 200000
}