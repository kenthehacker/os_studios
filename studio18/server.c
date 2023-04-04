#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#define BACKLOG 5
#define BUF_SIZE 1024
#define TERMINATION_VALUE 418
int main(void){
    int server_socket, comm_socket;
    unsigned int current_value;
    struct sockaddr_in server_addr;
    const char *SOCKNAME = "PLANET_EXPRESS";
    char buf[BUF_SIZE];
    int num_bytes;
    int byte_count;
    uint32_t msg;
    int cont = 1;
    int port_num = 30050;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        printf("Socket failed to make\n");
        perror("Socket Fail");exit(1);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_un)); 
    server_addr.sin_family = AF_INET;
    //strncpy(server_addr.sun_path, SOCKNAME, sizeof(server_addr.sun_path) - 1);
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;

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
        while(read(comm_socket, &msg, sizeof(msg))>0){
            if (TERMINATION_VALUE == ntohl(msg)){
                cont = 0;
                printf("client killed server\n");
            }
            printf("received %u \n",ntohl(msg));
        }

    }

    printf("client(s) disconnect TERMINAL VALUE %d received\n",TERMINATION_VALUE);
    close(comm_socket);
    //unlink(SOCKNAME);
    
    return 0;
}


