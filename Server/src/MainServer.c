#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sd1, sd2;
    char buf[100];

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5200);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len=sizeof(ip_addr);

    sd1=initializeSocket(&server_addr, len);

    while(){
        // Loop di accettazione
        // Accettazione
        if((sd2 = accept(sd1, NULL, NULL)) < 0) {
            perror(":ACCEPT ERROR");
            close(sd1);
            exit(1);
        }
        else {
            //dosomething
        }
    }

    close(sd1);
    printf("Terminazione processo server!\n");
    return 0;
}