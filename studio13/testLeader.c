#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "shared_mem.h"
int main(){
    int fd;
    struct shared_data *my_struct;
    fd = shm_open("/shared_memory_region", O_CREAT | O_RDWR, S_IRWXU);
    ftruncate(fd, sizeof(struct shared_data));
    my_struct = mmap(NULL, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    my_struct->read_guard =0;
    for (int i = 0; i < shared_mem_size; i++) {
        my_struct->data[i] = rand() % 100;
    }
    printf("my_struct pointer value: %p\n", (void*)my_struct);
    while(my_struct->read_guard==0){

    }
    return 0;
}