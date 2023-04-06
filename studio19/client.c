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

int main(int argc, char * argv[]){
    struct sockaddr_in sock_address;
    int cli_socket;
    uint32_t i;
    char user_input[100];
    char hostname[BUF_SIZE];
    unsigned int TERMINATION_VALUE = 418;
	const char *SOCKNAME = "PLANET_EXPRESS";
    uint32_t payload;
    uint32_t kill_server = 418;
    int port_num = 30303;
    hostname[1023] = '\0';
    
    cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket == -1){
		perror("Client socket fail\n");
		exit(1);
    }
    memset(&sock_address, 0, sizeof(sock_address));
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(port_num);
    sock_address.sin_addr.s_addr = inet_addr("128.252.167.161");

    if (connect(cli_socket, (struct sockaddr *) &sock_address, sizeof(sock_address)) == -1){
        perror("CLIENT connect fail");
		exit(1);
    }
    
    gethostname(hostname, 1023);
    printf("CLIENT hostname: %s\n", hostname);
    struct sockaddr *temp_addy = (struct sockaddr *)&sock_address;
    socklen_t len = sizeof(sock_address);
    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    int result = getnameinfo(temp_addy, len, host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    if (result == 0) {
        printf("IP: %s, port: %s\n", host, port);
    }

    if (argc == 2 && strcmp(argv[1], "quit") == 0) {
        payload = htonl(kill_server);
        if (write(cli_socket, &payload, sizeof(payload))<0){
            perror("CLIENT TO KILL SERVER Fail");
            exit(1);
        }
        close(cli_socket);
        return 0;
    }


    printf("sending values\n");
    for (i = 1; i<=50; i++){
        payload = htonl(i);
        int byte_write = write(cli_socket, &payload, sizeof(payload));
        if (byte_write<0){
            perror("CLIENT write Fail");
            exit(1);
        }else if (byte_write < sizeof(payload)){
            printf("Short write detected... killing cli\n");
            break;        
        }
        
    }
    struct sockaddr_in server_addr;
    char server_msg[BUF_SIZE];
    socklen_t server_addr_size = sizeof(server_addr);
    int comm_socket = accept(cli_socket, (struct sockaddr *) &server_addr, &server_addr_size);
    int num_bytes = read(comm_socket, server_msg, sizeof(server_msg)-1);
    server_msg[1023]='\0';
    printf("Server said: %s\n",server_msg);
    
    


    printf("this client is ded\n");
    close(cli_socket);
    return 0;
}

