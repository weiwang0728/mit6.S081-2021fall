#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char * path, char* name){
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        exit(-1);
    }

    if (st.type != T_DIR) {
        fprintf(2, "find must input with a directory\n");
        exit(-1);
    }

    // printf("path is %s\n", path);
    if (strcmp(path, ".") != 0 && strcmp(path, "..") != 0) {}
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de))== sizeof(de)) {
        // printf("de.name is %s\n", de.name);
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat%s\n", buf);
        }
        if (st.type == T_FILE && strcmp(de.name, name) == 0) {
            printf("%s\n", buf);
        }
        if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
            find(buf, name);
        }
    }
}
int main(int argc, char* argv[]) {
    find(argv[1], argv[2]);
    exit(0);
}