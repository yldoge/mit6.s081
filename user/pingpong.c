#include "kernel/syscall.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv) {

    // check usage
    if (argc > 1) {
        write(1, "usage: pingpong", sizeof("usage: pingpong"));
    }

    char buf[1];
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);
    if (fork() == 0) { // child
        close(p2c[1]); // close write end of parent-to-child pipe
        close(c2p[0]); // close read end of child-to-parent pipe
        if (read(p2c[0], buf, 1) != 1) {
            printf("child read error\n");
            exit(1);
        }
        close(p2c[0]); // close write read end of parent-to-child pipe
        printf("%d: received ping\n", getpid());
        
        if (write(c2p[1], "1", 1) != 1) {
            printf("child write error\n");
            exit(1);
        }
        close(c2p[1]); // close write end of child-to-parent pipe
        exit(0);
    } else { // parent
        close(c2p[1]);
        close(p2c[0]);

        if (write(p2c[1], "1", 1) != 1) {
            printf("parent write error\n");
            exit(1);
        }
        close(p2c[1]);

        if (read(c2p[0], buf, 1) != 1) {
            printf("parent read error\n");
            exit(1);
        }
        close(p2c[1]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}