/**
 * Author: E.K. Jithendiran
 * Date  : 31 Mar 24
 */

// gcc usr_app.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define IOCTL_MAGIC  'k'

#define IOCTL_ _IO(IOCTL_MAGIC, 0)

#define IOCTL_WP _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_RP _IOR(IOCTL_MAGIC, 1, int)

#define IOCTL_WPG _IOW(IOCTL_MAGIC, 2, int)
#define IOCTL_RPS _IOR(IOCTL_MAGIC, 2, int)

#define IOCTL_DUAL _IOWR(IOCTL_MAGIC, 1, int)

int main() {
    int a = 27;
    int c;
    int* b = &c;
    int fd = open("/dev/ioctl_dr",O_RDWR);
    if(fd < 0) {
        printf("No device file found \n");
        return -1;
    }

    // write 

    // write by value
    a = ioctl(fd, IOCTL_, 112);
    printf("Device value is : %d\n", a); // Device value is : 4

    // write by reference
    *b = 12;
    ioctl(fd, IOCTL_WP, b); 
    printf("B value is : %d\n",*b);

    // // Write by GET_USER
    // *b = 14;
    // ioctl(fd, IOCTL_WPG, b); 

    
    // // read

    ioctl(fd, IOCTL_RP, b);
    printf("Device value is : %d\n", *b); // Device value is : 521

    ioctl(fd, IOCTL_RPS, b);
    printf("Device value is : %d\n", *b); // Device value is : 123


    // read & write
    *b = 96;
    ioctl(fd,IOCTL_DUAL, b);
    printf("Device value is : %d\n", *b); // Device value is : 69
    close(fd);
    return 0;
}