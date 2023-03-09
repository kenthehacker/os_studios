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
    fd = shm_open("/shared_memory_region", O_RDWR, S_IRWXU);
    my_struct = mmap(NULL, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Shared data values:\n");
    while(my_struct->write_guard == 0){
        
    }
    for (int i = 0; i < shared_mem_size; i++) {
        printf("%d\n", my_struct->data[i]);
    }
    my_struct->read_guard = 1;
    printf("Data pointer value: %p\n", (void*)my_struct);
}