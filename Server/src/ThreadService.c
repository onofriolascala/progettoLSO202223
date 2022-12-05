/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#include "../include/ThreadService.h"

// Funzione principale.
void* thrService(void* arg) {
    void* out;

    return out;
}

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2) {
    pthread_t tid;
    if (pthread_create(&tid, NULL, thrService, &sd2)) {
        printf(":THREAD CREATION ERROR:\n");
        close(sd2);
    }
    return tid;
}