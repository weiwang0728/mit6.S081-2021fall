#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#define BUFFER_SIZE 512

char files[MAXARG][BUFFER_SIZE];
void execute_command(int argc, char* argv[], int filecount) {
    char* args[MAXARG];
    // printf("filecount is %d\n", filecount);
    int i;
    args[0] = argv[1];
    for (i = 2; i < argc; i ++) {
        args[i - 1] = argv[i];
    }

    for (int j = 0; j < filecount; j ++, i ++) {
        args[i - 1] = files[j];
    }

    args[i - 1] = 0;
    // for (int i = 0; i < filecount + argc - 3; i ++) {
    //     printf("files[%d] is %s\n",i, files[i]);
    // }

    args[filecount + argc] = 0;

    if (fork() == 0) {
        exec(args[0], args);
        fprintf(2, "exec failed\n");
        exit(-1);
    } else {
        wait(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: %s <patter>\n", argv[0]);
        exit(-1);
    }

    char buffer[BUFFER_SIZE];

    int file_count = 0;
    while (gets(buffer, BUFFER_SIZE)) {
        if (strlen(buffer) == 0) break;
        buffer[strlen(buffer) - 1] = '\0';
        strcpy(files[file_count], buffer);
        file_count ++;
        if (file_count >= MAXARG - argc) {
            execute_command(argc, argv, file_count);
            file_count = 0;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    if (file_count > 0) {
        execute_command(argc, argv, file_count);
    }
    exit(0);
}