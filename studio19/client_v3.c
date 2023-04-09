#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#define BUF_SIZE 1024

int connect_to_server(){
    int cli_socket;
    int port_num = 30303;
    struct sockaddr_in address;
    cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket == -1){
		perror("Client socket fail\n");
		exit(1);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port_num);
    address.sin_addr.s_addr = inet_addr("128.252.167.161");
    if (connect(cli_socket, (struct sockaddr *) &address, sizeof(address)) == -1){
        perror("CLIENT connect fail");
		exit(1);
    }
    return cli_socket;
}

int main(int argc, char *argv[]){
    int fd = connect_to_server();
    for (int i = 1; i<=10; i++){
        char msg[64];
        sprintf(msg,"client says: %d\n",i);
        int bytes_written = write(fd, msg, sizeof(msg));
        if (bytes_written<0){
            perror("write failure");
            exit(1);
        }
    }
    while(1==1){
        char message[BUF_SIZE];
        printf("Say something to server\n");
        fgets(message, sizeof(message), stdin);
        strcat(message,"\n");
        write(fd,message,strlen(message),0);
        if (strcmp(message,"quit\n") == 0){
            break;
        }
    }

    // printf("End sending values\n");
    // char buffer[1024];
    // if (read(fd, buffer, 1024) < 0) {
    //     perror("read failed");
    //     exit(EXIT_FAILURE);
    // }
    // printf("Server response: %s\n", buffer);

    return 0;
}

