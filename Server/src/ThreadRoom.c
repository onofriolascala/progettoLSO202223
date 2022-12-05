/*
* Funzioni del thread responsabile della gestione della logica di gioco.
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

#include "../include/ThreadRoom.h"

#define MAXCOMMBUFFER 1024

// Funzione principale.
void* thrRoom(void* arg) {
    void* out;

    return out;
}

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom() {
    pthread_t tid;

    return tid;
}