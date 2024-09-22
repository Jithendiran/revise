// Internally, scull represents each device with a structure of type struct scull_dev (our purpose)
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

int scull_quantum = 20;  // Number of Quantum
int scull_qset = 5;     // Quantum Set