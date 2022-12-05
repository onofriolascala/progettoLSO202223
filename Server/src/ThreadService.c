/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/
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

#include "../include/ThreadService.h"

#define MAXCOMMBUFFER 1024

// Funzione principale.
void* thrService(void* arg) {
    void* out;
    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];

    return out;
}

// Comunicazioni tramite stringa tra thread e client associato.
void sendToClient(char input[], char output[]) {

}
void receiveFromClient(char input[], char output[]) {

}

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2) {
    pthread_t tid;
    if (pthread_create(&tid, NULL, thrService, &sd2)) {
        printf(":THREAD CREATION ERROR:\n");
        close(sd2);
    }
    printf("DEBUG: new service thread successful.\n");
    return tid;
}