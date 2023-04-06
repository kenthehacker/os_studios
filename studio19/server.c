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
#define TERMINATION_VALUE 418

int main(void){
    int server_socket, comm_socket;
    unsigned int current_value;
    struct sockaddr_in server_addr, cli_addr;
    int num_bytes;
    int byte_count;
    uint32_t msg;
    int cont = 1;
    int port_num = 30303;
    

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Socket failed to make\n");
        perror("Socket Fail");exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *) & server_addr, sizeof(server_addr)) == -1){
        perror("bind");exit(1);
    }

    if (listen(server_socket, BACKLOG) == -1){
        perror("listen");exit(1);
    }
    
    int nfds;
	fd_set readfds;
	int ret;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    FD_SET(server_socket, &readfds);
    nfds = server_socket>STDIN_FILENO? server_socket : STDIN_FILENO;
    while(1==1 && cont == 1){
        ret = select(nfds+1, &readfds, NULL, NULL, NULL);
		if (ret == -1){
			perror("select failed:");
			return -1;
		}
		if (FD_ISSET(server_socket, &readfds)){
            socklen_t cli_addr_len = sizeof(cli_addr);
            comm_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &cli_addr_len);
            num_bytes = read(comm_socket, &msg, sizeof(msg));
            while(num_bytes>0){
                printf("received %u \n",ntohl(msg));
                num_bytes = read(comm_socket, &msg, sizeof(msg));
            }
            char server_msg[] = "Server to client message";
            write(comm_socket, server_msg, sizeof(server_msg)); //strlen?
        }
        if (FD_ISSET(STDIN_FILENO, &readfds)){
			char words[1024];
			fgets(words, 1024, stdin);
            printf("user input: %s\n",words);
			if(strcmp(words,"quit\n") == 0){
				printf("killed program \n");
				return 0;
			}
		}
        

        

    }

    printf("client(s) disconnect TERMINAL VALUE %d received\n",TERMINATION_VALUE);
    close(comm_socket);
    return 0;
}
