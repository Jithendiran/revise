/**
 * Author: E.K. Jithendiran
 * Date  : 25 Sep 24
 */

// This is user side application program for sc_1.c
// once the module of sc_1.ko is loaded execute this program
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
FILE *fptr;
char str[20];
int res;
char in[40] = "Hi hello aaaaaaaaaaaaaaaaaaaa";

int write_fprintf() {
 fptr = fopen("/dev/sc_1", "w");

    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    res = fprintf(fptr, "%s", in);
    printf("Result is : %d\n", res); // Result is : 29
    fclose(fptr);
    return res;
}

int read_fscnf() {
    // fsacnf reads one word at a time from the file, where words are separated by spaces or newlines.
    printf("fscanf s\n");
    fptr = fopen("/dev/sc_1", "r");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }


    res = fscanf(fptr, "%s", &str);
    printf("Result is : %d\n", res); // Result is : 1
    printf("Value of n=%s\n", str); // Value of n=Hi

    res = fscanf(fptr, "%s", &str);
    printf("Result is : %d\n", res); // Result is : 1
    printf("Value of n=%s\n", str);  // Value of n=hello

    res = fscanf(fptr, "%s", &str);
    printf("Result is : %d\n", res); // Result is : 1
    printf("Value of n=%s\n", str);  // Value of n=aaaaaaaaaaaaaaaaaaaa

    res = fscanf(fptr, "%s", &str);
    printf("Result is : %d\n", res); // Result is : -1
    fclose(fptr);
    printf("fscanf c\n");
}

int read_f(){
    printf("readf s\n");
    fptr = fopen("/dev/sc_1", "r");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }


    res = fread(str, 1, 3, fptr);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str); // Value of n=Hi aaaaaaaaaaaaaaaaa
    // Here it read only 3 bytes , remaining aaa.. is previous fscanf's read buff stored in str variable 

    res = fread(str, 1, 3, fptr);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str);  // Value of n=helaaaaaaaaaaaaaaaaa

    res = fread(str, 1, 3, fptr);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str);  // Value of n=lo aaaaaaaaaaaaaaaaa

    res = fread(str, 1, 3, fptr);
    printf("Result is : %d\n", res); // Result is : 3
    fclose(fptr);
    printf("readf c\n");
}

int read_vanilla(){
    char str[20];
    printf("readvan s\n");
    int fptr =  open("/dev/sc_1", O_RDONLY);
    if (fptr == -1)
    {
        printf("Error!");
        exit(1);
    }


    res = read(fptr, str, 20);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str); // Value of n=Hi 

    res = read(fptr, str, 20);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str);  // Value of n=hel

    res = read(fptr, str, 20);
    printf("Result is : %d\n", res); // Result is : 3
    printf("Value of n=%s\n", str);  // Value of n=lo 

    res = read(fptr, str, 20);
    printf("Result is : %d\n", res); // Result is : 3
    close(fptr);
    printf("readvan c\n");
}

int write_vanilla(){
    char str[20];
    printf("writevan s\n");
    int fptr =  open("/dev/sc_1", O_WRONLY);
    if (fptr == -1)
    {
        printf("Error!");
        exit(1);
    }


    res = write(fptr, in, 20);
    printf("Result is : %d\n", res); // Result is : 3

    res = write(fptr, in, 20);
    printf("Result is : %d\n", res);  // Result is : 3

    res = write(fptr, in, 20);
    printf("Result is : %d\n", res);  // Result is : 3

    res = write(fptr, in, 20);
    printf("Result is : %d\n", res);  // Result is : 3
    close(fptr);
    printf("writevan c\n");
    // $ cat /dev/sc_1 #(in terminal)
    // op is : Hi Hi Hi Hi 
}



int main()
{

    write_fprintf();
    read_fscnf();
    read_f();
    read_vanilla();
    write_vanilla();
    return 0;
}

/**
 * Execute the pgm with read comment or write comment
 * $ gcc sc_1_c.c
 * $ ./a.out
 * $ cat /dev/sc_1
 */