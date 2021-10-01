#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[1024];
    int p1[2], p2[2];
    int ret;

    pipe(p1);
    pipe(p2);

    if ((ret = fork()) == 0)
    {
        read(p1[0], buf, 4);
        printf("%d: received %s\n", p1[0], buf);
        write(p2[1], "pong", 4);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else
    {
        write(p1[1], "ping", 4);
        read(p2[0], buf, 4);
        printf("%d: received %s\n", p2[0], buf);
        close(p2[0]);
        close(p1[1]);
        exit(0);
    }
}