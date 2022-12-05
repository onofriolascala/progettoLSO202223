/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/ThreadService.h"

// Funzione principale.
void* thrService(void* arg) {
    void* out;

    return out;
}

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2) {
    pthread_t tid;
    //if (pthread_create(&tid, NULL, thrService, &sd2)) {
    //    printf(":THREAD CREATION ERROR:\n");
    //    close(sd2);
    //}
    return tid;
}