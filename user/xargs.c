#include "kernel/syscall.h"
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void main(int argc, char *argv[]) {

    char *child_argv[MAXARG];

    int i;
    for (i = 1; i < argc; i++) {
        child_argv[i - 1] = argv[i];
    }
    
    char c;
    char buf[128];
    char *buf_p = buf;
    int arg_idx = argc - 1, char_idx = 0;

    while (read(0, &c, 1) > 0) {
        if (c == ' ') {
            buf[char_idx++] = 0;
            child_argv[arg_idx++] = buf_p;
            buf_p = buf + char_idx;
        } else if (c == '\n') {
            buf[char_idx++] = 0;
            child_argv[arg_idx++] = buf_p;
            child_argv[arg_idx] = 0;
            char_idx = 0;
            if (fork() == 0) {
                exec(argv[1], child_argv);
            } else {
                wait(0);
                arg_idx = argc - 1;
                char_idx = 0;
                buf_p = buf;
            }
        } else {
            buf[char_idx++] = c;
        }
    }
    exit(0);
}