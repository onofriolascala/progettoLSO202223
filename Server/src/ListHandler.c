//
// Created by Mattia on 15/12/2022.
//


#include "../include/ListHandler.h"

pthread_mutex_t room_creation_mutex = PTHREAD_MUTEX_INITIALIZER;

// Cosa da fare: bisogna gestire gli errori delle malloc

// FUNZIONI DI GESTIONE GIOCATORI //
/* La funzione alloca dinamicamente lo spazio per un nuovo nodo giocatore, restituendone il riferimento.
 * Non presenta possibili criticità che richiedano un mutex. */
struct player_node* createNewPlayerNode( int player_socket, char username[] ){
    struct player_node* new;
    if( (new = (struct player_node*)malloc(sizeof(struct player_node))) == NULL){
        //gestire errore malloc
    }
    else{
        new->player_socket = player_socket;
        strcpy( new->username, username);
        new->next = NULL;
    }
    return new;
}

/* La funzione riceve una lista circolare di giocatori ed un nodo da aggiungervi. Restituisce la testa della lista
 * qualora essa fosse stata vuota e quindi riempita dal nodo aggiunto.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
struct player_node* addPlayerToPlayerList( struct player_node* player_list, struct player_node* newPlayer){
    //printf("DEBUG_addPtoPlist:started\n");
    struct player_node* tmp;
    if( player_list != NULL){
        //printf("DEBUG_addPtoPlist:1\n");
        if( newPlayer != NULL ){
            //printf("\tDEBUG_addPtoPlist:1.3\n");
            tmp = player_list;
            //scorre tutta la lista fino a quando non arriva all'elemento prima della testa
            while( tmp->next->player_socket != player_list->player_socket ){
                //printf("\tDEBUG_addPtoPlist:1.4\n");
                tmp = tmp->next;
            }
            newPlayer->next = tmp->next;
            tmp->next = newPlayer;
        }
    }
    else {
        //printf("\tDEBUG_addPtoPlist:2\n");
        newPlayer->next = newPlayer;

        player_list = newPlayer;
    }
    //printf("DEBUG_addPtoPlist:completed\n");
    return player_list;
}

/* La funzione riceve una lista circolare di giocatori ed il socket descriptor del nodo da rimuovere.
 * Restituisce la testa della lista qualora essa coincida col nodo da rimuovere.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
struct player_node* removePlayerNode( struct player_node* player_list, int target_socket ) {
    struct player_node *tmp, *target;
    if (player_list != NULL) {
        if (player_list->player_socket == target_socket) {
            target = player_list;
            player_list = player_list->next;
        } else {
            tmp = player_list;
            while (tmp->next->player_socket != target_socket && tmp != player_list) {
                tmp = tmp->next;
            }
            // target trovato
            if (tmp != player_list) {
                target = tmp->next;
                tmp->next = tmp->next->next;
            }
        }
    }
    return player_list;
}

/* La funzione riceve un nodo giocatore e procede a deallocarlo in maniera sicura.
 * Non presenta possibili criticità che richiedano un mutex. */
int destroyPlayerNode( struct player_node* player ) {
    int distrutto = -1;
    if( player != NULL ){
        free(player);
        distrutto = 1;
    }
    return distrutto;
}

/* La funzione riceve una lista circolare di giocatori ed il socket descriptor del nodo da cercare.
 * Restituisce il nodo giocatore la cui socket corrisponde con quella in input..
 * Non presenta criticità che richiedano un mutex. */
struct player_node* getPlayer( struct player_node* player_list, int target_socket ){
    //printf("DEBUG_getplayernode:started\n");
    struct player_node* target, *tmp;
    if( player_list != NULL){
        if( player_list->player_socket == target_socket) {
            //printf("DEBUG_getplayernode:1\n");
            target = player_list;
        }
        else {
            tmp = player_list->next;
            while( tmp->player_socket != target_socket && tmp != player_list ){
                tmp = tmp->next;
            }
            if( tmp != player_list ){
                //hit
                target = tmp;
            }
            else{
                target = NULL;
            }
        }
    }
    else {
        target = NULL;
    }
    //printf("DEBUG_getplayernode:completed\n");
    return target;
}

// FUNZIONI DI GESTIONE STANZE //
/* La funzione riceve la testa di una lista di stanze da cui calcolare l'ID del nuovo nodo. Restituisce
 * il nuovo nodo creato con tutte le variabili settate o a zero o a null.
 * Non presenta criticità che richiedano un mutex. */
struct room_node* createNewRoomNode( struct room_node* list_head ) {
    //printf("DEBUG_createroomnode:started\n");
    fflush(stdout);
    struct room_node* new;
    new = (struct room_node*)malloc(sizeof(struct room_node));
    if( new != NULL){
        if(list_head != NULL)
            new->id = list_head->id + 1;
        else {
            new->id = 1;
        }
        //sprintf(new->localsocket, "/tmp/LSO202223/thrRoom_socket_local_%d", new->id);
        sprintf(new->localsocket, "/tmp/thrRoom_socket_local_%d", new->id);
        new->player_list = NULL;
        new->player_num = 0;
        new->next = NULL;
        pthread_mutex_init(&new->roomnode_mutex, NULL);
    }
    else {
        //gestire errore malloc
    }
    //printf("DEBUG_createroomnode:completed\n");
    fflush(stdout);
    return new;
}

/* La funzione riceve il puntatore alla testa della lista delle stanze ed un nodo da aggiungervi.
 * Siccome la modifica avviene sul puntatore, non è necessario modificare la testa della lista.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
void addRoomToRoomList (struct room_node** head_pointer, struct room_node* new_room ){
    //printf("DEBUG_addroomnode:started\n");
    fflush(stdout);
    if(head_pointer != NULL){
        if( new_room != NULL ){
            new_room->next = *head_pointer;
            *head_pointer = new_room;
        }
    }
    //printf("DEBUG_addroomnode:completed\n");
    fflush(stdout);
}

/* La funzione riceve il puntatore alla testa della lista delle stanze. Crea ed aggiunge un nuovo nodo
 * associato alla stanza appena generata.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
struct room_node* createAndAddNewRoom( struct room_node** head_pointer){
    struct room_node* new_room;

    //printf("DEBUG_C&Aroomnode:started\n");
    fflush(stdout);

    pthread_mutex_lock(&room_creation_mutex);
    if ((*head_pointer) != NULL) {
        pthread_mutex_lock(&(*head_pointer)->roomnode_mutex);

        new_room = createNewRoomNode(*head_pointer);
        addRoomToRoomList(head_pointer, new_room);

        pthread_mutex_unlock(&(*head_pointer)->roomnode_mutex);
    }
    else {
        new_room = createNewRoomNode(*head_pointer);
        addRoomToRoomList(head_pointer, new_room);
    }
    pthread_mutex_unlock(&room_creation_mutex);

    //printf("DEBUG_C&Aroomnode:completed\n");
    fflush(stdout);
    return new_room;
}

/* La funzione riceve la testa di una lista di stanze ed l'ID di un nodo da rimuovere. Restituisce la testa
 * della lista appena modificata.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
void removeAndDestroyRoomNode(struct room_node** head_pointer, int target_id ){
    //printf("DEBUG_R&Droomnode:started\n");
    /*                      ** RICORSIVA**
    struct room_node* tmp;
    if( list_head != NULL){
        if( list_head->id == target_id ){
            tmp = list_head;
            list_head = list_head->next;
            free(tmp);
        }
        else{
            list_head->next = removeAndDestroyRoomNode( list_head->next, target_id );
        }
    }
    //printf("DEBUG_R&Droomnode:completed\n");
    return list_head;
     */

    struct room_node *list_head;
    struct room_node *tmp, *target;

    pthread_mutex_lock(&room_creation_mutex);
    if((*head_pointer) != NULL){

        pthread_mutex_lock(&(*head_pointer)->roomnode_mutex);
        list_head = *head_pointer;
        pthread_mutex_unlock(&room_creation_mutex);

        if(list_head->id == target_id ){
            target = list_head;
            (*head_pointer) = list_head->next;
            pthread_mutex_unlock(&target->roomnode_mutex);
        }
        else {
            tmp = list_head;
            // !isNULL(tmp->next) { lock tmp->next }
            if(tmp->next != NULL) {
                pthread_mutex_unlock(&tmp->next->roomnode_mutex);
            }
            while( tmp->next != NULL && tmp->next->id != target_id ) {
                if(tmp->next->next != NULL) {
                    pthread_mutex_unlock(&tmp->next->next->roomnode_mutex);
                }
                pthread_mutex_unlock(&tmp->roomnode_mutex);
                tmp = tmp->next;
            }
            if(tmp->next != NULL) {
                target = tmp->next;
                tmp->next = tmp->next->next;
                pthread_mutex_unlock(&tmp->roomnode_mutex);
                pthread_mutex_unlock(&target->roomnode_mutex);
            }
            else {
                target = NULL;
                pthread_mutex_unlock(&tmp->roomnode_mutex);
            }
        }
        if(target != NULL){
            free(target);
        }
    }
    else {
        pthread_mutex_unlock(&room_creation_mutex);
    }
}

/* La funzione riceve la testa della lista di stanze ed ID da ricercare. Restituisce il nodo il cui ID
 * combaci con quello dato in input.
 * Non presenta criticità che richiedano un mutex. */
struct room_node* getRoom( struct room_node* room_list, int target_id ){
    //printf("DEBUG_getroomnode:started\n");
    struct room_node* target;
    if( room_list != NULL){
        if( room_list->id == target_id) {
            //printf("DEBUG_getroomnode:1\n");
            target = room_list;
        }
        else {
            //printf("\tDEBUG_getroomnode:2\n");
            target = getRoom(room_list->next, target_id);
        }
    }
    else {
        target = NULL;
    }
    //printf("DEBUG_getroomnode:completed\n");
    return target;
}

