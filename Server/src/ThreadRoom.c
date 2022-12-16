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
#include "../include/SocketUtilServer.h"
#include "../include/Def.h"


// Funzione principale.
void* thrRoom(void* arg) {
    printf("DEBUG: Creazione stanza.\n");
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];
    int ID, localsocket, room_sd, out_len, signal_num;
    struct room_node** room_list;
    struct room_node* this_room;
    struct room_node* currenthead;

    struct sockaddr_un localsocket_addr;
    socklen_t localsocket_len;

    struct room_arg temp;
    temp = *(struct room_arg*)arg;
    //printf("DEBUG:1.\n");

    // Copia i valori della struttura originale.
    //printf("DEBUG:1,5\n");
    room_list = temp.room_list;
    if (room_list == NULL) {
        currenthead = NULL;
        //printf("DEBUG:2.1\n");
    }
    else {
        currenthead = *room_list;
        //printf("DEBUG:2.2\n");
    }
    this_room = createNewRoomNode(currenthead);
    ID = this_room->id;

    printf("DEBUG:3.\n");
    // Apertura della localsocket
    localsocket = localSocketInit(ID, this_room->localsocket, &localsocket_addr, &localsocket_len);
    printf("DEBUG:4.\n");

    // La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct room_arg*)arg).room_ID = this_room->id;
    (*(struct room_arg*)arg).flag = 1;
    printf("\t\t\t\tROOM_ID%d: initialized with ID value of \"%d\", and local socket of \"%s:%d\".\n", ID, ID, localsocket_addr.sun_path, localsocket);
    fflush(stdout);

    printf("\t\t\t\tROOM_ID%d: service thread has ended.\n", ID);
    return 0;
}

// Creazione del thread della stanza.
int createNewRoom(int sd, struct room_node** room_list) {
    /*printf("DEBUG: Input for createNewRoom.\n");
    fflush(stdout);*/
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

