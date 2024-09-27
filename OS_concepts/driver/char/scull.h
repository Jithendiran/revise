/**
 * Author: E.K. Jithendiran
 * Date  : 22 Sep 24
 */
// Internally, scull represents each device with a structure of type struct scull_dev (our purpose)
// #include <sys/types.h>

#include <linux/fs.h>
#include <linux/cdev.h>

struct scull_qset {
    void **data;
    struct scull_qset *next;
};

struct scull_dev
{
    struct scull_qset *data;    /* Pointer to first quantum set */
    int quantum;                /* the current quantum size */
    int qset;                   /* the current array size */
    unsigned long size;         /* amount of data stored here */
    unsigned int access_key;    /* used by sculluid and scullpriv */
    struct semaphore sem;       /* mutual exclusion semaphore */
    struct cdev cdev;           /* Char device structure*/
};

int scull_quantum = 3;  // Number of Quantum, Each quantum size is 3 byte
int scull_qset = 2;     // Quantum Set, Each quantum has 3 quantum
// So each each quantum set can store up to 6 bytes of data