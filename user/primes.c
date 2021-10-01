#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

void prime(int fd){
    int a = 0;
    if (read(fd, &a, sizeof(int)) <= 0) {
        close(fd);
        exit(0);
    }
    printf("prime %d\n", a);

    int child_fd[2];
    pipe(child_fd);

    int ret = fork();
    if (ret == 0) {
        close(child_fd[1]);
        prime(child_fd[0]);
    } else {
        close(child_fd[0]);
        int b = 0;
        while (read(fd, &b, sizeof(int)) > 0) {
            if (b % a) {
                write(child_fd[1], &b, sizeof(int));
            }
        }
        close(fd);
        close(child_fd[1]);
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);

    int status;
    int ret = fork();
    if (ret == 0) {
        close(fd[1]);
        prime(fd[0]);
    } else {
        close(fd[0]);
        for (int i=2; i<36; i++) {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(&status);
    }
    exit(0);
}
