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


struct room_node* addNewRoom(struct room_node* room_list){
    struct room_node* new;
    if(room_list == NULL){
        new=(struct room_node*)malloc(sizeof(struct room_node));
        new->id=1;
        strcpy(new->localsocket,"");
        new->player_list=NULL;
        new->player_num=0;
        new->next=NULL;
        room_list=new; //aggiorno la testa della lista delle stanze
    }
    else{
        new=(struct room_node*)malloc(sizeof(struct room_node));
        new->id=room_list->id+1;
        strcpy(new->localsocket,"");
        new->player_list=NULL;
        new->player_num=0;
        new->next=room_list;
        room_list=new; //aggiorno la testa della lista delle stanze
    }
    return room_list;
}

// Funzione principale.
void* thrRoom(void* arg) {
    void* out;
    struct room_node* room_list;
    char localsocket[LOCALSOCKETADDRLENGHT];
    room_list = (struct room_node*)arg;

    struct room_node* this_room;

    this_room=addNewRoom(room_list);                                //crea nuovo nodo room e aggiungilo alla lista
    sprintf(localsocket,"/tmp/thrRoom_socket_local_%d",this_room->id); //crea socket locale e aggiungilo al nodo
    //crea socket
    strcpy(this_room->localsocket,localsocket);

    return out;
}

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom(struct room_node* room_list) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrRoom, &room_list)) {
        printf(":THREAD CREATION ERROR:\n");
        pthread_cancel(pthread_self());
        return 0;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il main terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewService. */

    printf("MAIN: Service thread created.\n");
    fflush(stdout);
    return tid;
}