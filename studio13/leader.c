#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "shared_mem.h"
#include <stdlib.h>

int main(void){
    int file_descriptor;
    file_descriptor = shm_open("/shared_memory_region", O_CREAT | O_RDWR, S_IRWXU);
    if (file_descriptor == -1){
        printf("shm_open failed to create\n");
        return -1;
    }
    struct shared_data * dat;

    if (ftruncate(file_descriptor,sizeof(dat)) == -1){
        printf("failed to ftruncate\n");
        return -1;
    }

    dat = mmap(NULL, sizeof(dat),PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (dat == MAP_FAILED){
        printf("map failed \n");
        return -1;
    }

    srand(21);
    printf("PRINTING LEADER VALUES \n");
    for(int i = 0; i < shared_mem_size; i++){
        dat->data[i] = rand();
        printf("%d\n",dat->data[i]);
    }

    return 0;
}
