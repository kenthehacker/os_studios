#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stddef.h>
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

int main(void){
    int server_sock, incoming_socket;
    int port_num = 30303;
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed to make\n");
        exit(1);
    }
    struct sockaddr_in server_addr, cli_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_sock, (struct sockaddr *) & server_addr, sizeof(server_addr)) == -1){
        perror("bind");exit(1);
    }
    if (listen(server_sock, BACKLOG) == -1){
        perror("listen");exit(1);
    }
    int counter = 0;
    while(1==1){
        uint32_t msg;
        socklen_t cli_addr_len = sizeof(cli_addr);
        incoming_socket = accept(server_sock, (struct sockaddr *) &cli_addr, &cli_addr_len);
        int num_bytes = read(incoming_socket, &msg, sizeof(msg));
        if (num_bytes > 0){
            printf("received %u \n",ntohl(msg));
        }
        if (counter>=5){
            break;
        }
    }
    char server_msg[] = "Server to client message";
    int write_byte_len = write(incoming_socket, server_msg, sizeof(server_msg)); //strlen?
    printf("Wrote %d bytes to cli\n",write_byte_len);
    close(incoming_socket);
    return 0;
}


