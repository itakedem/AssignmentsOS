#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

void env_large(){

    int pid;
    int status;

    for (int i=0; i<4; i++) {
        if ((pid = fork()) == 0) {
            for (int i = 0; i <= 10000000; i++){
                if (i % 10000 == 0){
                    printf("%d", i);
                }
            }
            exit(0);
        }
    }

    while (wait(&status) > 0);
}

int
main(int argc, char *argv[])
{
    env_large();
    print_stats();
    exit(0);
}
