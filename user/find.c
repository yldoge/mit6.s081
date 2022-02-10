#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


const char *cur = ".", *prev = "..";

char* get_name(char *path) {
    char *p;
    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--) 
        ;
    p++;
    return p;
}

void find(char *path, char *filename) {
    char buf[128], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, 0)) < 0) {
        printf("find: can't open: %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_FILE:
            if (strcmp(filename, get_name(path)) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf); // p point to the first '\0'
            *p++ = '/'; // set '\0' to '/', and increment p
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                if (strcmp(de.name, cur) == 0 || strcmp(de.name, prev) == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                // p[DIRSIZ] = '\0';
                find(buf, filename);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: find [directory] [filename]");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}