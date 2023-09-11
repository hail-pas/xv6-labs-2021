/*
 *
 * Filename         :xargs.c
 * Description      :
 * Time             :2023/09/08 11:36:36
 * Author           :Phoenix(hypofiasco@gmail.com)
 * Version          :1.0
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char*
strcpy_single_line(char *s, const char *t)
{
  char *os;

  os = s;
  while(*t != '\n' && *t != 0) {
    *os++ = *t++;
  }
  *os = 0;
  return s;
}

void xargs(int argc, char *argv[]) {
    // for (int i = 0; i < argc ; i ++)
        // fprintf(1, "%s\n", argv[i]);

    char buffer[1024];
    char *finalargv[argc];
    int n = 0;

    for (int i = 1; i < argc; i ++) {
      finalargv[i - 1] = argv[i];
    }

    while(read(0, buffer+n, 1)){
      if (n == 1023)
      {
        fprintf(2, "argument is too long\n");
        exit(1);
      }
      n++;
    }

    char temp[1024];

    char *p = buffer;

    while (*p)
    {
        strcpy_single_line(temp, p);
        finalargv[argc -1] = temp;

        fprintf(1, "%s\n", temp);

        int pid = fork();
        if (pid == 0){
          exec(finalargv[0], finalargv);
          fprintf(2, "exec failed\n");
			    exit(1);
        }
        wait(0);
        if (*(p + strlen(temp)) == '\0') break;
        p += (strlen(temp) + 1);
    }

}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "xargs: Params must at least one.");
    exit(1);
  }
    xargs(argc, argv);
    exit(0);
}
