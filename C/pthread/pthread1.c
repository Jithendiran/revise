#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

// to compile gcc -g -pthread pthread1.c -o pthread.o 
void* routine() {
    printf("Inside routine\n");
    sleep(2);
    printf("End of routine\n");
}
void main() {
    pthread_t t;
    if(pthread_create(&t, NULL, &routine, NULL) !=0 ) {
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n",i);
    }
    
    printf("in main\n");
    // wait until thread execution complete
    if (pthread_join(t,NULL) != 0){
        return;
    } 
    printf("Completed");  
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
*/