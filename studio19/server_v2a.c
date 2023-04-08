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

int main(int argc, char *argv[]){
    struct sockaddr_in addr, cli_addr;
    int sfd, cfd;
    int port_num = 30303;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("server socket failed");
        exit(1);
    }
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_num);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1){
        perror("bind fail");
        exit(1);
    } 
    if (listen(sfd, BACKLOG) == -1){
        perror("listen fail");
        exit(1);
    }
    int nfd = sfd;
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    for(;;){
        int ready = select(nfd+1, &readfds, &writefds, NULL, NULL);
        if (ready == -1){
            perror("Select fail");
            exit(1);
        }
        for(int i_fd = 0; i_fd<nfd+1; i_fd++){
            if (FD_ISSET(i_fd, &readfds)){
                if (i_fd == sfd){
                    cfd = accept(sfd, (struct sockaddr *)&cli_addr, &(sizeof(cli_addr)));
                    if (cfd == -1){
                        perror("failed to accept client");
                        exit(1);
                    }
                    FD_SET(cfd, &readfds);
                    nfd = cfd > nfd? cfd : nfd;
                }else{
                    uint32_t cli_msg;
                    int byte_count = read(i_fd, &cli_msg, sizeof(uint32_t));
                    if (byte_count == 0){
                        close(i_fd);
                        FD_CLR(i_fd, &readfds);
                    }else{
                        while(byte_count > 0){
                            printf("read from cli: %u\n", ntohl(cli_msg));  
                            byte_count = read(i_fd, &cli_msg, sizeof(uint32_t));
                        }
                        char server_to_cli_msg[BUF_SIZE];
                        char hostname[BUF_SIZE];
                        gethostname(hostname,BUF_SIZE);
                        sprintf(server_to_cli_msg, "%s: end of msg", hostname);
                        

                    }
                }
            }
        }
    }
    
}