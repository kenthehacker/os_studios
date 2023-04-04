#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BACKLOG 5
#define BUF_SIZE 1024
#define TERMINATION_VALUE 418
int main(void){
    int server_socket, comm_socket;
    unsigned int current_value;
    struct sockaddr_un server_addr;
    const char *SOCKNAME = "PLANET_EXPRESS";
    char buf[BUF_SIZE];
    int num_bytes;
    int byte_count;
    int cont = 1;

    server_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (server_socket == -1){
        printf("Socket failed to make\n");
        perror("Socket Fail");exit(1);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_un)); 
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SOCKNAME, sizeof(server_addr.sun_path) - 1);

    if (bind(server_socket, (struct sockaddr *) & server_addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");exit(1);
    }

    if (listen(server_socket, BACKLOG) == -1){
        perror("listen");exit(1);
    }

    
    
    while(1==1 && cont == 1){
        comm_socket = accept(server_socket, NULL, NULL);
        if (comm_socket == -1){
            perror("Accept failed");
            exit(1);
        }    
        num_bytes = read(comm_socket, buf, sizeof(buf));
        if (num_bytes<=0){
            printf("done reading \n");
            break;
        }
        byte_count = 0;
        while(byte_count < num_bytes){
            sscanf(&buf[byte_count], "%u ", &current_value);
            printf("Received value %u \n", current_value);
            if (current_value == 418){
                cont = 0;
                break;
            }
            byte_count += sizeof(unsigned int);
        }
    }

    printf("client(s) disconnect TERMINAL VALUE %d received\n",TERMINATION_VALUE);
    close(comm_socket);
    unlink(SOCKNAME);
    
    return 0;
}


