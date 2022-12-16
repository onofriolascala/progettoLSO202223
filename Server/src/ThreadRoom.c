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
#include "../include/ListHandler.h"
#include "../include/Def.h"


// Funzione principale.
void* thrRoom(void* arg) {
    printf("DEBUG: Creazione stanza.\n");
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];
    int ID, localsocket, room_sd, out_len, signal_num;
    struct room_node* room_list;
    struct room_node* this_room;

    // Copia i valori della struttura originale.
    room_list = *(*(struct room_arg*)arg).room_list;
    this_room = createNewRoomNode(room_list);
    ID = this_room->id;
    (*(struct room_arg*)arg).room_ID = this_room->id;

    // Apertura della localsocket
    localsocket = 15;
    printf("DEBUG:4.\n");

    // La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct room_arg*)arg).flag = 1;
    printf("\t\tROOM_ID%d: initialized with ID value of \"%d\", and local socket of \"%s:%d\".\n", ID, ID, this_room->localsocket, localsocket);
    fflush(stdout);

    printf("\t\tROOM_ID%d: service thread has ended.\n", ID);
    return 0;
}

// Creazione del thread della stanza.
int createNewRoom(int sd, struct room_node** room_list) {
    printf("DEBUG: Input for createNewRoom.\n");
    fflush(stdout);
    int flag;
    pthread_t tid;

    struct room_arg args;
    args.room_list = room_list;
    args.room_ID = 0;
    args.flag = 0;

    printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrRoom, &args)) {
        printf(":THREAD CREATION ERROR:\n");
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il service terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewRoom. */
    while(args.flag == 0) {
        usleep(REFRESHCONSTANT);
    }

    printf("\t\t\tSERVICE_SD%d: room thread created with ID:%d.\n", sd, args.room_ID);
    fflush(stdout);
    return args.room_ID;
}

//
int joinRoom(int ID, struct room_node** room_list, struct player_node* player) {
    int signal_num;

    //signal_num =

    /* Comunicazioni attese:
     * 70-Stanza piena, 71-Stanza inesistente, 72-Utente già connesso, 54-Stanza
     * */
    return signal_num;
}