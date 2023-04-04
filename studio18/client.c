#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE = 1024;

int main(void){
    struct sockaddr_un sock_address;
    int cli_socket;
    unsigned int i;
    char buf[BUF_SIZE];

    cli_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (cli_socket == -1){
        errExit("CLIENT Socket Fail");
    }
    memset(&sock_address, 0, sizeof(sock_address));
    sock_address.sun_family = AF_LOCAL;
    strncpy(sock_address.sun_path, SV_SOCK_PATH, sizeof(sock_address.sun_path) - 1);
    if (connect(cli_socket, (struct sockaddr *) &sock_address, sizeof(struct sockaddr_un)) == -1){
        errExit("CLIENT connect fail");
    }
    for (i = 1; i<=15; i++){
        sprintf(buf, "%u", i);
        if (write(cli_socket, buf, sizeof(unsigned int)) < 0 ){
            errExit("CLIENT write Fail");
        }
        //utoa(i, buf, )
    }
    close(cli_socket);
    return 0;
}

//https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm