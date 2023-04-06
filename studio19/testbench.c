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
int main(void){

    printf("hi this is bender and i like jiggley puff");
    for(int i = 0 ; i < 500; i++){
        printf("%d",i);
    }
    return 0;
}