#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

/* Sanity test for task 1*/
void
task2_test(void){
    int fd;
    char data[1024];

    // fills the data array with 7s
    memset(data, 7, 1024);
    fd = open("sanity_check_file", O_CREATE | O_RDWR);
    for(int i = 0; i < 1024*10; i++){
        write(fd, data, sizeof(data));
        if (i == 11)
            printf("Finished writing 12KB (direct)\n");
        if (i == 267)
            printf("Finished writing 268KB (single direct)\n");
    }
    printf("Finished writing 10MB\ntest finished\n");
}

int main(int argc, char* argv[]){
    printf("Sanity Test - Task 2\n");
    task2_test();

    exit(1);
}