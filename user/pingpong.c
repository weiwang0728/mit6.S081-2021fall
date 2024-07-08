#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int pingpongPipe[2];
    
    pipe(pingpongPipe);
    int pid = fork();
    if (pid == 0) {
        char buf[10];
        read(pingpongPipe[0], buf, 10);
        printf("%d: received %s\n", getpid(), buf);
        close(pingpongPipe[0]);
        write(pingpongPipe[1], "pong", strlen("pong"));
        close(pingpongPipe[1]);
        exit(0);
    } else if (pid > 0) {
        write(pingpongPipe[1], "ping", strlen("ping"));
        close(pingpongPipe[1]);
        char buf[10];
        read(pingpongPipe[0], buf, 10);
        printf("%d: received %s\n",getpid(), buf);
        close(pingpongPipe[0]);
        wait(0);
        exit(0);
    } else {
        printf("ping-pong called fork failed\n");
        exit(-1);
    }
    exit(0);
}