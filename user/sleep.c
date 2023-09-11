/*
 *
 * Filename         :sleep.c
 * Description      :
 * Time             :2023/09/08 11:37:01
 * Author           :Phoenix(hypofiasco@gmail.com)
 * Version          :1.0
*/


#include "kernel/types.h"
#include "user/user.h"

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        panic("Parameter should be only one");
    }

    char * perid = argv[1];

    sleep(*((int *)perid) - 48);
    exit(0);
}
