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
#define IOCTL_R _IOR(IOCTL_MAGIC, 1, int32_t*)
#define IOCTL_W _IOW(IOCTL_MAGIC, 2, int32_t*)

int main() {
    int a = 27;
    int b;
    printf("Executing user space app : a =  %d, %p, b = %d, % p\n", &a, &a, &b, &b);
    int fd = open("/dev/ioctl_dr",O_RDWR);
    if(fd < 0) {
        printf("No device file found \n");
        return -1;
    }

    // read
    ioctl(fd, IOCTL_R, &b);
    printf("Device value is : %d\n", b); // Device value is : -1

    // write
    printf("Going to write : %d\n", a); // Going to write : 27
    ioctl(fd, IOCTL_W, &a);

    // read
    ioctl(fd, IOCTL_R, &b);
    printf("Device value is : %d\n", b); // Device value is : 27

    close(fd);
    return 0;
}