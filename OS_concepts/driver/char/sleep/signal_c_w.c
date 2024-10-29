/**
 * Author: E.K.Jithendiran
 * Date: 13 Oct 24
 * 
 * gcc signal_c_w.c -o w.out
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



char buffer[4096] = "Hi Ji Hello";

int main(int argc, char **argv)
{
    struct sigaction action;
    int fd = open("/dev/sc_signal", O_WRONLY | O_NONBLOCK);
    printf("fd : %d\n",fd);
    memset(&action, 0, sizeof(action));
    action.sa_handler = sighandler;
    action.sa_flags = 0;

    sigaction(SIGIO, &action, NULL);

    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);

    int res = write(fd,buffer,strlen(buffer));
    printf("Result : %d\n",res);
    if(res == strlen(buffer)) return 0;

    while(1) {
        
        sleep(86400); /* one day */
        if (!gotdata)
            continue;
        if(res > 0 && res < strlen(buffer)){
            printf("Written part : res = %d, buffer = %d \n", res, strlen(buffer));
            strncpy(buffer,buffer+(res-1),strlen(buffer));
            printf("Remainig buff : %s\n",buffer);
        }
        int res = write(fd,buffer,strlen(buffer));
        printf("Set Data : %s\n",buffer);
        if(res == strlen(buffer)) return 0;
        gotdata=0;
    }

    close(fd);
}