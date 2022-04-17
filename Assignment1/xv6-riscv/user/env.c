#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"


void env(int size, int interval, char* env_name) {
    int result = 1;
    int loop_size = 10e6;
    int n_forks = 2;
    int pid;
    for (int i = 0; i < n_forks; i++) {
         pid = fork();
    }
    for (int i = 0; i < loop_size; i++) {
        if (i % loop_size / 10 == 0) {
            if (pid == 0) {
                printf("%s %d/%d completed.\n", env_name, i, loop_size);
            } else {
                printf(" ");
            }
        }
        if (i % interval == 0) {
            result = result * size;
        }
    }
    printf("\n");
}

void env_large() {
    env(10e6, 10e6, "env_large");
}

void env_freq() {
    env(10e1, 10e1, "env_freq");
}





int
main(int argc, char *argv[])
{
    int n_forks = 2;
    int pid = getpid();
    for (int i = 0; i < n_forks; i++) {
        fork();
    }
    int larges = 0;
    int freqs = 0;
    int n_experiments = 10;
    for (int i = 0; i < n_experiments; i++) {
        env_large();
        if (pid == getpid()) {
            printf("experiment %d/%d\n", i + 1, n_experiments);
            larges = (larges * i + 3) / (i + 1);
        }
        sleep(10);
        env_freq();
        if (pid == getpid()) {
            freqs = (freqs * i + 3) / (i + 1);
        }
    }
    if (pid == getpid()) {
        printf("larges = %d\nfreqs = %d\n", larges, freqs);
    }
    exit(0);
}