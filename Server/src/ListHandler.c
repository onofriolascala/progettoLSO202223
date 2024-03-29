//
// Created by Mattia on 15/12/2022.
//


#include "../include/ListHandler.h"

pthread_mutex_t roomhead_pointer_mutex = PTHREAD_MUTEX_INITIALIZER;

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
        pthread_mutex_init(&new->playernode_mutex, NULL);
    }
    return new;
}

/* La funzione riceve una lista circolare di giocatori ed un nodo da aggiungervi. Restituisce la testa della lista
 * qualora essa fosse stata vuota e quindi riempita dal nodo aggiunto.
 * */
struct player_node* addPlayerToPlayerList(struct player_node* playerlist_head, struct player_node* newPlayer){
    //printf("DEBUG_addPtoPlist:started\n");
    struct player_node* tmp;
    if(playerlist_head != NULL){
        //printf("DEBUG_addPtoPlist:1\n");
        if( newPlayer != NULL ){
            //printf("\tDEBUG_addPtoPlist:1.3\n");
            tmp = playerlist_head;
            //scorre tutta la lista fino a quando non arriva all'elemento prima della testa
            while(tmp->next != playerlist_head ){
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

        playerlist_head = newPlayer;
    }
    //printf("DEBUG_addPtoPlist:completed\n");
    return playerlist_head;
}

/* La funzione riceve una lista circolare di giocatori ed il socket descriptor del nodo da rimuovere.
 * Restituisce la testa della lista qualora essa coincida col nodo da rimuovere.
 * Non presenta criticità che richiedano un mutex. */
struct player_node* removePlayerNode( struct player_node** playerhead_pointer, int target_socket ) {
    struct player_node *tmp,*prev, *target, *playerlist_head;

    target = NULL;
    playerlist_head = (*playerhead_pointer);

    if (playerlist_head != NULL) {
        if (playerlist_head->player_socket == target_socket) {
            target = playerlist_head;
            prev = playerlist_head;
            //cerco il nodo che punta al target e cambio il suo next con il next del target
            while(prev->next != target){
                prev = prev->next;
            }
            prev->next = target->next;

            //modifico la testa della lista
            (*playerhead_pointer) = target->next;
            // caso di una lista di un singolo elemento
            if(target == *playerhead_pointer)
                *playerhead_pointer = NULL;
            // distacco il nodo dalla lista
            target->next = NULL;
        } else {
            tmp = playerlist_head->next;
            prev = playerlist_head;
            while (tmp->player_socket != target_socket && tmp != playerlist_head) {
                tmp = tmp->next;
                prev = prev->next;
            }
            // target trovato
            if (tmp != playerlist_head) {
                target = tmp;
                prev->next = target->next;
                target->next = NULL;
            }
        }
    }
    return target;
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
struct player_node* getPlayer( struct player_node* playerlist_head, int target_socket ){
    //printf("DEBUG_getplayernode:started\n");
    struct player_node *target, *tmp;
    if( playerlist_head != NULL){
        if( playerlist_head->player_socket == target_socket) {
            //printf("DEBUG_getplayernode:1\n");
            target = playerlist_head;
        }
        else {
            tmp = playerlist_head->next;
            while( tmp->player_socket != target_socket && tmp != playerlist_head ){
                tmp = tmp->next;
            }
            if( tmp != playerlist_head ){
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

struct player_node* getPlayerByUsername( struct player_node* playerlist_head, char usr_target[]){
    //printf("DEBUG_getplayernode:started\n");
    struct player_node *target, *tmp;
    if( playerlist_head != NULL){
        if( strcmp(playerlist_head->username,usr_target) == 0) {
            //printf("DEBUG_getplayernode:1\n");
            target = playerlist_head;
        }
        else {
            tmp = playerlist_head->next;
            while( strcmp(tmp->username, usr_target) != 0 && tmp != playerlist_head ){
                tmp = tmp->next;
            }
            if( tmp != playerlist_head ){
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

    pthread_mutex_lock(&roomhead_pointer_mutex);
    if ((*head_pointer) != NULL) {
        pthread_mutex_lock(&(*head_pointer)->roomnode_mutex);

        new_room = createNewRoomNode(*head_pointer);
        addRoomToRoomList(head_pointer, new_room);

        pthread_mutex_unlock(&(*head_pointer)->next->roomnode_mutex);
    }
    else {
        new_room = createNewRoomNode(*head_pointer);
        addRoomToRoomList(head_pointer, new_room);
    }
    pthread_mutex_unlock(&roomhead_pointer_mutex);

    //printf("DEBUG_C&Aroomnode:completed\n");
    fflush(stdout);
    return new_room;
}

/* La funzione riceve la testa di una lista di stanze ed l'ID di un nodo da rimuovere. Restituisce la testa
 * della lista appena modificata.
 * La funzione può essere soggetta a race-condition e per tanto va sincronizzata con un mutex. */
void removeAndDestroyRoomNode(struct room_node** head_pointer, int target_id ){
    //printf("DEBUG_R&Droomnode:started\n");

    struct room_node *list_head;
    struct room_node *tmp, *target;

    pthread_mutex_lock(&roomhead_pointer_mutex);
    if((*head_pointer) != NULL){

        pthread_mutex_lock(&(*head_pointer)->roomnode_mutex);
        list_head = *head_pointer;

        if(list_head->id == target_id ){
            target = list_head;
            (*head_pointer) = list_head->next;
            pthread_mutex_unlock(&target->roomnode_mutex);
            pthread_mutex_unlock(&roomhead_pointer_mutex);
        }
        else {
            pthread_mutex_unlock(&roomhead_pointer_mutex);
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
        pthread_mutex_unlock(&roomhead_pointer_mutex);
    }
}

/* La funzione riceve la testa della lista di stanze ed ID da ricercare. Restituisce il nodo il cui ID
 * combacia con quello dato in input.
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
/* La funzione riceve la testa della lista di stanze, una stringa di output e la lunghezza massima della stringa di output.
 * Scrive nella stringa di output la lista di stanze con formattazione [id_stanza]: [num_giocatori_stanza]/[max_num_giocatori][- || ;]
 * valori control flag:
 * 0: tutte le stanze lette e aggiunte al risultato
 * -1: non ci sono stanze da leggere
 * -2: trylock error
 * 1: buffer pieno e mancano stanze da leggere
 * */

struct room_node* getRoomList( struct room_node* room_list, char outgoing[], int max_out, int* control_flag ){
    struct room_node* tmp;
    int curr_lenght = 0;
    char room[20];
    int record_len = 12;

    *control_flag = 0;


    if(room_list == NULL){
        strcpy(outgoing, "Non esistono stanze.-;");
        tmp = room_list;
        *control_flag = -1;
    }
    else{
        tmp = room_list;

        if(!lockRoomNode(tmp)){
            *control_flag = -2;
            return tmp;
        }

        sprintf(outgoing,"%d: %d/8", tmp->id, tmp->player_num);
        curr_lenght += strlen(outgoing);
        while( tmp->next != NULL && curr_lenght < (max_out - record_len)){
            pthread_mutex_unlock(&tmp->roomnode_mutex);
            tmp = tmp->next;

            if(!lockRoomNode(tmp)){
                *control_flag = -2;
                return tmp;
            }

            strcat(outgoing,"-");
            curr_lenght++;
            sprintf(room,"%d: %d/8", tmp->id, tmp->player_num);
            strcat(outgoing, room);
            curr_lenght += strlen(room);
        }

        strcat(outgoing, "-");
        if( curr_lenght < (max_out - record_len)){
            strcat(outgoing, ";");
        }
        else{
            if(tmp->next != NULL){
                *control_flag = 1;
                strcat(outgoing, "#;");
            }
            else{
                strcat(outgoing, ";");
            }
        }
        pthread_mutex_unlock(&tmp->roomnode_mutex);
        tmp = tmp->next;
    }
    return tmp;
}

int lockRoomNode(struct room_node* room){
    int result = 0;
    int try = 0;
    while(pthread_mutex_trylock(&room->roomnode_mutex) != 0){
        if(++try == (MAXREFRESHCONSTANT + 20)){
            return result;
        }
        usleep(REFRESHCONSTANT);
    }
    result = 1;
    return result;
}