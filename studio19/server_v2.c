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
    int max_fd = server_sock;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_sock, &read_fds);
    while(1==1){
        printf("maxfd: %d\n",max_fd);
        printf("server_sock: %d\n",server_sock);
        fd_set tmp_fds = read_fds;
        int num_fds = select(max_fd+1, &tmp_fds, NULL, NULL, NULL);
        if (num_fds == -1){
            perror("select");
            exit(1);
        }
        for (int fd = 0; fd <= max_fd; fd++){
            if (FD_ISSET(fd, &tmp_fds)){
                if (fd == server_sock){ 
                    socklen_t cli_addr_len = sizeof(cli_addr);
                    incoming_socket = accept(server_sock, (struct sockaddr *) &cli_addr, &cli_addr_len);
                    if (incoming_socket == -1){
                        perror("accept failed");
                    }
                    else {
                        FD_SET(incoming_socket, &read_fds);
                        max_fd = incoming_socket > max_fd? incoming_socket : max_fd;
                        printf("new connection established\n");
                    }
                }
                else {
                    printf("fd: %d\n",fd);
                    uint32_t msg;
                    int num_bytes = read(fd, &msg, sizeof(msg));
                    if (num_bytes == -1){
                        perror("read");
                    }
                    else if (num_bytes == 0){ 
                        printf("Connection closed.\n");
                        close(fd);
                        FD_CLR(fd, &read_fds);
                    }
                    else { 
                        printf("received %u \n",ntohl(msg));
                        if (ntohl(msg) == 10) {
                            char server_msg[] = "Server to client message";
                            int write_byte_len = write(fd, server_msg, sizeof(server_msg)); //strlen?
                            if (write_byte_len == -1){
                                perror("write");
                            }
                            else {
                                printf("Wrote %d bytes to cli\n",write_byte_len);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}



