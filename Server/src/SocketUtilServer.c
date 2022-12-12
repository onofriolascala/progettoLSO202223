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

#include "../include/SocketUtilServer.h"

#define MAXCONNECTIONS 5

// Funzione adibita all'inizializzazione della socket primaria d'ascolto.
int socketInit(struct sockaddr_in* server_addr, socklen_t* len) {
    int sd1;

    (*server_addr).sin_family = AF_INET;
    (*server_addr).sin_port = htons(5200);
    (*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);
    *len = sizeof(*server_addr);

    // Apertura del socket lato server.
    if ((sd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        //exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd1, (struct sockaddr*)server_addr, *len) < 0){
        perror(":BIND ERROR");
        close(sd1);
        //exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd1, MAXCONNECTIONS) < 0) {
        perror(":LISTEN ERROR");
        close(sd1);
        //exit(1);
    }
    printf("MAIN: SocketInit completed.\n");
    fflush(stdout);
    return sd1;
}

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1) {
    int sd2;
    int count = 5;
    while(count--){
        // Loop di Accettazione
        if((sd2 = accept(sd1, NULL, NULL)) < 0) {
            perror(":ACCEPT ERROR");
            //close(sd1);
        }
        else {
            createNewService(sd2);
        }
        printf("MAIN: Accept loop restarting...\n");
        fflush(stdout);
    }
}