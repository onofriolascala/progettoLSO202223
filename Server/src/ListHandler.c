//
// Created by Mattia on 15/12/2022.
//


#include "../include/ListHandler.h"


struct player_node* createNewPlayerNode( int player_socket, char username[] ){
    struct player_node* new;
    if( (new = (struct player_node*)malloc(sizeof(struct player_node))) == NULL){
        //gestire errore malloc
    }
    else{
        new->player_socket = player_socket;
        strcpy( new->username, username);
        sprintf( new->service_addr, "/tmp/LSO202223/thrService_socket_local_%d", player_socket);
        new->next = NULL;
    }
    return new;
}

struct player_node* removePlayerNode( struct player_node* player_list, int target_socket ){
    if( player_list != NULL ){
        if( player_list->player_socket == target_socket ){
            player_list = player_list->next;
        }
        else
            player_list->next = removePlayerNode(player_list -> next, target_socket);
    }
    return player_list;
}

int destroyPlayerNode( struct player_node* player_node ){
    int distrutto = -1;
    if( player_node != NULL ){
        free(player_node);
        distrutto = 1;
    }
    return distrutto;
}

struct room_node* createNewRoomNode( struct room_node* room_list ) {
    struct room_node* new;
    new = (struct room_node*)malloc(sizeof(struct room_node));
    if( new != NULL){
        if( room_list != NULL)
            new->id = room_list->id+1;
        else
            new->id = 1;
        sprintf(new->localsocket,"/tmp/LSO202223/thrRoom_socket_local_%d", new->id);
        new->player_list = NULL;
        new->player_num = 0;
        new->next = NULL;
    }
    else;
        //gestire errore malloc
    return new;
}


