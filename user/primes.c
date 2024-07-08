#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter_prime_num(int fd) {
    int pipefd[2];
    int p;

    if (read(fd, &p, sizeof(int)) == 0) {
        close(fd);
        exit(0);
    }

    printf("prime %d\n", p);

    pipe(pipefd);
    int pid = fork();
    if (pid > 0) {
        close(pipefd[0]);
        int n;
        while (read(fd, &n, sizeof(int)) != 0) {
            if (n % p != 0) {
                write(pipefd[1], &n , sizeof(int));
            }
        }
        close(pipefd[1]);
        close(fd);
        wait(0);
    } else {
        close(pipefd[1]);
        close(fd);
        filter_prime_num(pipefd[0]);
        close(pipefd[0]);
    }
}
int main(int argc, char* argv[]) {
    int pipefd[2];
    pipe(pipefd);

    int pid = fork();

    if (pid < 0) {
        printf("fork error\n");
        exit(-1);
    }
    if (pid == 0) {
        close(pipefd[1]);
        filter_prime_num(pipefd[0]);
        close(pipefd[0]);
        exit(0);
    } 
    // 父进程
    close(pipefd[0]);
    for (int i = 2; i < 35; i ++) {
        write(pipefd[1], &i, sizeof(int));
    }
    close(pipefd[1]);
    wait(0);
    exit(0);
}