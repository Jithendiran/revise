/**
 * Author: E.K.Jithendiran
 * Date: 12 Oct 24
 *
 * gcc poll_c_w.c -o w.out
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <string.h>
#include <sys/select.h>

char buffer[4096] = "Hi Jidesh";

int main(int argc, char **argv)
{
    int fd = open("/dev/sc_poll", O_WRONLY | O_NONBLOCK);

    // // select
    // fd_set writefds;
    // FD_ZERO(&writefds);
    // FD_SET(fd, &writefds);

    // // process is blocked
    // int ready = select(fd+1, NULL, &writefds, NULL, NULL);

    // if (ready == -1)
    // {
    //     printf("Error Occurred\n");
    // }

    // if (FD_ISSET(fd, &writefds))
    // {
    //     printf("Writting......\n");
    //     int c = write(fd, buffer, strlen(buffer));
    //     printf("Set : %s\n");
    // }

    // ---------------------------------------------------------

    // // poll
    struct pollfd pollFd;

    pollFd.fd = fd;
    pollFd.events = POLLOUT | POLLWRNORM;

    // // process is blocked
    int ready = poll(&pollFd, 1, -1);

    if( ready < 0 ) 
    {
      perror("poll");
    }

    if( ( pollFd.revents & POLLOUT )  == POLLOUT )
    {
     write(fd, buffer, strlen(buffer));
     printf("set : %s\n",buffer);
    }

    close(fd);
}
