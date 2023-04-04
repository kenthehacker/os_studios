#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 1024

int main(int argc, char * argv[]){
    struct sockaddr_un sock_address;
    int cli_socket;
    unsigned int i;
    char buf[BUF_SIZE];
    char user_input[100];
    unsigned int TERMINATION_VALUE = 418;
	const char *SOCKNAME = "PLANET_EXPRESS";

    
    cli_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (cli_socket == -1){
       //errExit("CLIENT Socket Fail");
		perror("Client socket fail\n");
		exit(1);
    }
    memset(&sock_address, 0, sizeof(sock_address));
    sock_address.sun_family = AF_LOCAL;
    strncpy(sock_address.sun_path, SOCKNAME, sizeof(sock_address.sun_path) - 1);
    if (connect(cli_socket, (struct sockaddr *) &sock_address, sizeof(struct sockaddr_un)) == -1){
        perror("CLIENT connect fail");
		exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "quit") == 0) {
        printf("quitting program...\n");
        sprintf(buf, "%u", TERMINATION_VALUE);
        if (write(cli_socket, buf, sizeof(unsigned int)) < 0 ){
            perror("CLIENT write Fail");
			exit(1);
        }
        close(cli_socket);
        return 0;
    }


    printf("sending values\n");
    for (i = 1; i<=10; i++){
        sprintf(buf, "%u", i);
        if (write(cli_socket, buf, sizeof(unsigned int)) < 0 ){
            perror("CLIENT write Fail");
			exit(1);
        }
        //utoa(i, buf, )
    }
    /*
    while(1==1){
        printf("Enter some an int or 'quit' to exit: ");
        scanf("%s", &user_input);
        if (strcmp(user_input, "quit") == 0){
            sprintf(buf,"%u",418);
            if (write(cli_socket, buf, sizeof(unsigned int)) < 0){
                perror("Client failed to kill server");
                exit(1);
            }
            break;
        }else{
            i = (unsigned int) strtoul(user_input, NULL, 10);
            sprintf(buf, "%u", i);
            if (write(cli_socket, buf, sizeof(unsigned int)) < 0 ){
                perror("CLIENT write Fail");
			    exit(1);
            }
        }
    }
    */

    printf("this client is ded\n");
    close(cli_socket);
    return 0;
}

//https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
