//
// Created by Mattia on 15/12/2022.
//


#include "../include/ListHandler.h"



// Cosa da fare: bisogna gestire gli errori della malloc

// FUNZIONI DI GESTIONE GIOCATORI //

struct player_node* createNewPlayerNode( int player_socket, char username[] ){
    struct player_node* new;
    if( (new = (struct player_node*)malloc(sizeof(struct player_node))) == NULL){
        //gestire errore malloc
    }
    else{
        new->player_socket = player_socket;
        strcpy( new->username, username);
        //sprintf( new->service_addr, "/tmp/LSO202223/thrService_socket_local_%d", player_socket);
        new->next = NULL;
    }
    return new;
}

// ADD MUTEX
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

struct player_node* removePlayerNode( struct player_node* player_list, int target_socket ){
    if( player_list != NULL ){
        if( player_list->player_socket == target_socket ) {
            player_list = player_list->next;
        }
        else
            player_list->next = removePlayerNode(player_list -> next, target_socket);
    }
    return player_list;
}

int destroyPlayerNode( struct player_node* player ) {
    int distrutto = -1;
    if( player != NULL ){
        free(player);
        distrutto = 1;
    }
    return distrutto;
}

struct player_node* getPlayer( struct player_node* player_list, int target_socket ){
    //printf("DEBUG_getplayernode:started\n");
    struct player_node* target;
    if( player_list != NULL){
        if( player_list->player_socket == target_socket) {
            //printf("DEBUG_getplayernode:1\n");
            target = player_list;
        }
        else {
            //printf("\tDEBUG_getplayernode:2\n");
            target = getPlayer(player_list->next, target_socket);
        }
    }
    else {
        target = NULL;
    }
    //printf("DEBUG_getplayernode:completed\n");
    return target;
}

// FUNZIONI DI GESTIONE STANZE //
struct room_node* createNewRoomNode( struct room_node* room_list ) {
    //printf("DEBUG_createroomnode:started\n");
    fflush(stdout);
    struct room_node* new;
    new = (struct room_node*)malloc(sizeof(struct room_node));
    if( new != NULL){
        if( room_list != NULL)
            new->id = room_list->id+1;
        else {
            new->id = 1;
        }
        //sprintf(new->localsocket, "/tmp/LSO202223/thrRoom_socket_local_%d", new->id);
        sprintf(new->localsocket, "/tmp/thrRoom_socket_local_%d", new->id);
        new->player_list = NULL;
        new->player_num = 0;
        new->next = NULL;
    }
    else {
        //gestire errore malloc
    }
    //printf("DEBUG_createroomnode:completed\n");
    fflush(stdout);
    return new;
}

void addRoomToRoomList ( struct room_node** room_list, struct room_node* new_room ){
    //printf("DEBUG_addroomnode:started\n");
    fflush(stdout);
    if( room_list != NULL){
        if( new_room != NULL ){
            new_room->next = *room_list;
            *room_list = new_room;
        }
    }
    //printf("DEBUG_addroomnode:completed\n");
    fflush(stdout);
}

struct room_node* createAndAddNewRoom( struct room_node** room_list){
    struct room_node* new_room;

    //printf("DEBUG_C&Aroomnode:started\n");
    fflush(stdout);

    new_room = createNewRoomNode(*room_list);
    addRoomToRoomList(room_list, new_room);

    //printf("DEBUG_C&Aroomnode:completed\n");
    fflush(stdout);
    return new_room;
}

struct room_node* removeAndDestroyRoomNode( struct room_node* room_list, int target_id ){
    //printf("DEBUG_R&Droomnode:started\n");
    struct room_node* tmp;
    if( room_list != NULL){
        if( room_list->id == target_id ){
            tmp = room_list;
            room_list = room_list->next;
            free(tmp);
        }
        else{
            room_list->next = removeAndDestroyRoomNode( room_list->next, target_id );
        }
    }
    //printf("DEBUG_R&Droomnode:completed\n");
    return room_list;
}

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

