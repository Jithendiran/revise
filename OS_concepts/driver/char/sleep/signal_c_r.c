/**
 * Author: E.K.Jithendiran
 * Date: 13 Oct 24
 * 
 * gcc signal_c_r.c -o r.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

int gotdata=0;
void sighandler(int signo)
{
    printf("In Handler\n");
    if (signo==SIGIO)
        gotdata++;
    return;
}

char buffer[4096];

int main(int argc, char **argv)
{
    int count;
    struct sigaction action;
    int fd = open("/dev/sc_signal", O_RDONLY | O_NONBLOCK);
    printf("fd : %d\n",fd);
    memset(&action, 0, sizeof(action));
    action.sa_handler = sighandler;
    action.sa_flags = 0;

    sigaction(SIGIO, &action, NULL);

    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);

    while(1) {
        
        sleep(86400); /* one day */
        if (!gotdata)
            continue;
        count=read(fd, buffer, 4096);
        printf("Got Data : %s\n",buffer);
        gotdata=0;
    }

    close(fd);
}