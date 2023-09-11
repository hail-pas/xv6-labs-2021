/*
 *
 * Filename         :primes.c
 * Description      :
 * Time             :2023/09/08 11:36:56
 * Author           :Phoenix(hypofiasco@gmail.com)
 * Version          :1.0
*/


#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define THREAD_NUMBER 35
#define USED '0'
#define NOT_USED '1'


void prime(int in_pip, int out_pipe);

int main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);

    char message[THREAD_NUMBER];

    //  0，1 不需要处理
    message[0] = USED;
    message[1] = USED;

    for (int i = 2; i < THREAD_NUMBER; i++)
    {
        message[i] = NOT_USED;
    }

    int pid = fork();

    if (pid > 0)
    {
        // parent send the messages
        write(fd[1], message, sizeof(char) * THREAD_NUMBER);
        wait(0);
    }else if (pid == 0)
    {
        // child deal
        prime(fd[0], fd[1]);
    }

    exit(0);
}

void prime(int in_pip, int out_pipe) {
    char message[THREAD_NUMBER];
    read(in_pip, message, sizeof(char) * THREAD_NUMBER);
    int val = 0;
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        if (message[i] == NOT_USED)
        {
            val = i;
            break;
        }
    }

    if (val == 0)
    {
        return;
    }

    printf("prime: %d\n", val);

    message[val] = USED;

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        if (i % val == 0) {
            message[i] = USED;
        }
    }

    int pid = fork();
    if (pid > 0) {
        write(out_pipe, message, sizeof(char) * THREAD_NUMBER);
        wait(0);
    } else {
        prime(in_pip, out_pipe);
    }
}
