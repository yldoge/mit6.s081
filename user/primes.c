#include "kernel/syscall.h"
#include "kernel/types.h"
#include "user/user.h"

void recursion(int left_pipe_read) {
    int prime;
    if (read(left_pipe_read, &prime, sizeof(prime)) != 0) {
        printf("prime %d\n", prime);
        int right_pipe[2];
        pipe(right_pipe);
        if (fork() == 0) {
            close(right_pipe[1]);
            recursion(right_pipe[0]);
        } else {
            int n;
            close(right_pipe[0]);
            while (read(left_pipe_read, &n, sizeof(n)) != 0) {
                if (n % prime != 0) {
                    write(right_pipe[1], &n, sizeof(n));
                }
            }
            close(right_pipe[1]);
            close(left_pipe_read);
            wait(0);
        }
    } else {
        close(left_pipe_read);
    }
}

int main() {
    int i, p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        recursion(p[0]);
    } else {
        close(p[0]);
        for (i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}