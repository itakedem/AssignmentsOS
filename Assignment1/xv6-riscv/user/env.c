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
    int loop_size = 1000000000;
    for (int i = 0; i < loop_size; i++) {
        for (int j = 0; j < loop_size; j++) {
            if (j % interval == 0) {
                result = result * size * 10e6 *10e6 ;
            }
        }
    }
}

void env_large() {
    env(100000000, 1000000000, "env_large");
}

void env_freq() {
    env(100, 100, "env_freq");
}

int
main(int argc, char *argv[])
{
    int n_forks = 4;
    int pid = getpid();
    for (int i = 0; i < n_forks; i++) {
        fork();
    }
    int n_experiments = 10;
    for (int i = 0; i < n_experiments; i++) {
        env_large();
        if (pid == getpid()) {
            printf("experiment %d\n", i);
            printf("env large\n");
            wait(0);
            print_stats();
        }
        sleep(10);
        env_freq();
        if (pid == getpid()) {
            printf("\nenv freq\n");
            wait(0);
            print_stats();
        }
    }
    exit(0);
}