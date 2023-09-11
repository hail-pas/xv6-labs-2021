/*
 *
 * Filename         :pingpong.c
 * Description      :
 * Time             :2023/09/08 11:36:49
 * Author           :Phoenix(hypofiasco@gmail.com)
 * Version          :1.0
*/


#include "kernel/types.h"
#include "user/user.h"



const int BUFFSIZE = 16;
const char *ping = "ping";
const char *pong = "pong";

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int main(int argc, char *argv[])
{
    int pid;
    char buff[BUFFSIZE];

    int pipe_fd[2], nbytes;

    if (pipe(pipe_fd) < 0) panic("Acquire pipe failed!");

    pid = fork();

    if (pid == 0) {
        nbytes = read(pipe_fd[0], buff, BUFFSIZE);
        if (nbytes > 0) printf("%d: received %s\n", pid, buff);
        write(pipe_fd[1], pong, sizeof(pong));
    }else {
        write(pipe_fd[1], ping, sizeof(ping));
        wait(0);
        nbytes = read(pipe_fd[0], buff, BUFFSIZE);
        if (nbytes > 0) printf("%d: received %s\n", pid, buff);
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    exit(0);
}

