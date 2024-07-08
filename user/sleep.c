#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: %s [seconds]\n", argv[0]);
        exit(-1);
    }
    int n = atoi(argv[1]);
    if (n < 0) {
        printf("usage: %s seconds must greater than 0", argv[0]);
        exit(-1);
    }
    sleep(n);
    exit(0);
}