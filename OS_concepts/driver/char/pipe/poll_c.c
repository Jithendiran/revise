/**
 * Author: E.K.Jithendiran
 * Date: 12 Oct 24
 *
 * gcc poll_c.c
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

char buffer[4096];

int main(int argc, char **argv)
{
    int delay = 1, n, m = 0;
    int fptr =  open("/dev/sc_poll", O_RDWR);

    if (argc > 1)
        delay = atoi(argv[1]);
    

    fcntl(fptr, F_SETFL, fcntl(fptr, F_GETFL) | O_NONBLOCK); /* stdin */
    fcntl(fptr, F_SETFL, fcntl(fptr, F_GETFL) | O_NONBLOCK); /* stdout */

    while (1)
    {
        n = read(0, buffer, 4096);
        if (n >= 0)
            m = write(fptr, buffer, n);
        if ((n < 0 || m < 0) && (errno != EAGAIN))
            break;
        sleep(delay);
    }
    perror(n < 0 ? "stdin" : "stdout");
    close(fptr);
    exit(1);
}
