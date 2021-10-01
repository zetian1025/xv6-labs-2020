#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#include <stddef.h>

#define ARGSIZE (16)

char *my_gets(char *buf, int max)
{
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;)
    {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        if (c == '\n' || c == '\r')
        {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

int main(int argc, char *argv[])
{
    char buf[ARGSIZE];
    char *new_argv[MAXARG];
    int curr = argc - 1;

    for (int i = 0; i < argc - 1; i++)
    {
        new_argv[i] = (char *)malloc(ARGSIZE);
        memmove(new_argv[i], argv[i + 1], strlen(argv[i + 1]));
    }

    while (my_gets(buf, sizeof(buf)))
    {
        if (buf[0] == '\0')
        {
            break;
        }
        new_argv[curr] = (char *)malloc(ARGSIZE);
        memmove(new_argv[curr], buf, strlen(buf));
        new_argv[++curr] = NULL;
        if (curr == MAXARG + 1)
        {
            printf("max args.\n");
            break;
        }
    }

    int ret = fork();
    if (ret == 0)
    {
        exec(new_argv[0], new_argv);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    for (int i = 0; i < curr - 1; i++)
    {
        free(new_argv[i]);
    }

    exit(0);
}