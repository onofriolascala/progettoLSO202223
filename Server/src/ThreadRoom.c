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
#include "../include/Def.h"


// Funzione principale.
void* thrRoom(void* arg) {
    struct room_node* room_list;
    char localsocket[LOCALSOCKETADDRLENGHT];
    room_list = (struct room_node*)arg;

    struct room_node* this_room;

    //Crea nuovo nodo room e aggiungilo alla lista
    this_room = addNewRoom(room_list);
    //Crea socket locale e aggiungilo al nodo
    sprintf(localsocket,"/tmp/thrRoom_socket_local_%d",this_room->id);
    //Crea socket a
    strcpy(this_room->localsocket,localsocket);

    return 0;
}

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom(int sd, struct room_node** room_list) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrRoom, room_list)) {
        printf(":THREAD CREATION ERROR:\n");
        //pthread_cancel(pthread_self());
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    printf("\t\tSERVICE_SD%d: Room thread created.\n", sd);
    fflush(stdout);
    return tid;
}