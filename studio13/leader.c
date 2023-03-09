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
        printf("map failed in leader \n");
        return -1;
    }

    dat->write_guard = 0;
    dat->read_guard = 0;
    dat->delete_guard = 0;
    
    while(dat->write_guard == 0){
        //wait
    }
    
    srand(420);
    printf("PRINTING LEADER VALUES \n");
    for(int i = 0; i < shared_mem_size; i++){
        int val = rand();
        dat->data[i] = val;
        printf("%d\n",dat->data[i]);
    }

    dat->read_guard = 1;


    while(dat->delete_guard == 0){
        //do nothing
    }


    munmap(dat,0);
    //close(file_descriptor);
    return 0;
}
