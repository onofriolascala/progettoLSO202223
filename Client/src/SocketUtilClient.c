//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/SocketUtilClient.h"

int socketInit(struct sockaddr_in addr, socklen_t len, char ip[], int port){
    int sd1;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    len = sizeof(addr);
    inet_aton("localhost", &addr.sin_addr);

    if((sd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        exit(1);
    }
    if(connect(sd1, (struct sockaddr*)&addr, len) < 0) {
        perror(":CONNECT ERROR");
        exit(1);
    }

    printf("DEBUG: socketInit completed.\n");
    fflush(stdout);
    return sd1;
}