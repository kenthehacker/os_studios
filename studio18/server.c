#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netdb.h>
#define BACKLOG 5
#define BUF_SIZE 1024
#define TERMINATION_VALUE 418

int main(void){
    int server_socket, comm_socket;
    unsigned int current_value;
    struct sockaddr_in server_addr, cli_addr;
    const char *SOCKNAME = "PLANET_EXPRESS";
    char hostname[BUF_SIZE];
    int num_bytes;
    int byte_count;
    uint32_t msg;
    int cont = 1;
    int port_num = 30303;
    hostname[1023] = '\0';

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Socket failed to make\n");
        perror("Socket Fail");exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //strncpy(server_addr.sin_path, SOCKNAME, sizeof(server_addr.sin_path) - 1);
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    gethostname(hostname, 1023);
    printf("SERVER hostname: %s\n", hostname);


    if (bind(server_socket, (struct sockaddr *) & server_addr, sizeof(server_addr)) == -1){
        perror("bind");exit(1);
    }

    if (listen(server_socket, BACKLOG) == -1){
        perror("listen");exit(1);
    }

    while(1==1 && cont == 1){
        //comm_socket = accept(server_socket, NULL, NULL);
        socklen_t cli_addr_len = sizeof(cli_addr);
        comm_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &cli_addr_len);
        if (comm_socket == -1){
            perror("Accept failed");
            exit(1);
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("cli ip: %s\n", client_ip);
        num_bytes = read(comm_socket, &msg, sizeof(msg));
        while(num_bytes>0){
            if (TERMINATION_VALUE == ntohl(msg)){
                cont = 0;
                printf("client killed server\n");
            }else if (num_bytes < sizeof(msg)){
                printf("short read received\n continuing read...\n");
            }
            printf("received %u \n",ntohl(msg));
            num_bytes = read(comm_socket, &msg, sizeof(msg));
        }

    }

    printf("client(s) disconnect TERMINAL VALUE %d received\n",TERMINATION_VALUE);
    close(comm_socket);
    //unlink(SOCKNAME);
    
    return 0;
}

/*
https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
https://man7.org/linux/man-pages/man3/inet_ntop.3.html
*/
