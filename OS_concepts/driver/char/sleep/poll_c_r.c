/**
 * Author: E.K.Jithendiran
 * Date: 12 Oct 24
 *
 * gcc poll_c_r.c -o r.out
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>

char buffer[4096];

int main(int argc, char **argv)
{
    int fd = open("/dev/sc_poll", O_RDONLY | O_NONBLOCK);

    // select

    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(fd, &readfds);

    // /**
    //  * fd+1 -> This is the number of file descriptors that select() needs to monitor,
    //  *  which is one more than the largest file descriptor in any of the file descriptor sets 
    //  * (readfds, writefds, exceptfds).
    //  * 
    //  * The reason for fd + 1 is that select() checks file descriptors in a range from 0 to nfds - 1. 
    //  * So if the highest file descriptor you're interested in is fd, you pass fd + 1.
    //  * 
    //  * If your file descriptor is fd = 5, then the first argument for select() will be 5 + 1 = 6. 
    //  * This tells select() to check file descriptors from 0 through 5.
    //  */
    // // process is blocked
    // int ready = select(fd+1, &readfds, NULL, NULL, NULL);

    // if (ready == -1)
    // {
    //     printf("Error Occurred\n");
    // }

    // if (FD_ISSET(fd, &readfds))
    // {
    //     printf("Reading......\n");
    //     int c = read(fd, buffer, 4096);
    //     printf("Got : %s\n", buffer);
    // }

    // ------------------------------------------------------

    // poll
    struct pollfd pollFd;

    pollFd.fd = fd;
    pollFd.events = POLLIN | POLLRDNORM ;

    // // process is blocked
    // here 1 is count
    // -1 for no time out
    int ready = poll(&pollFd, 1, -1);

    if( ready < 0 ) 
    {
      perror("poll");
    }

    if( ( pollFd.revents & POLLIN )  == POLLIN )
    {
     read(fd, buffer, 4096);
     printf("Get %s\n",buffer);
    }


    close(fd);
}
