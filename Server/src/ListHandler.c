//
// Created by Mattia on 15/12/2022.
//


#include "../include/ListHandler.h"


struct player_node* createNewPlayerNode( int player_socket, char username[], char service_addr[]){
    struct player_node* player_node;
    if( (player_node = (struct player_node*)malloc(sizeof(player_node))) == NULL){
        //gestire errore malloc
    }
    else{
        player_node->player_socket = player_socket;
        strcpy( player_node->username, username);
        strcpy( player_node->service_addr, service_addr);
        player_node->next = NULL;
    }
    return player_node;
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




struct room_node* addNewRoom(struct room_node* room_list) {
    struct room_node* new;
    if(room_list == NULL){
        new = (struct room_node*)malloc(sizeof(struct room_node));
        if( new != NULL){
            new->id = 1;
            strcpy(new->localsocket, "");
            new->player_list = NULL;
            new->player_num = 0;
            new->next = NULL;
            room_list = new;
        }
        else;
            //gestisci errore malloc
    }
    else{
        new = (struct room_node*)malloc(sizeof(struct room_node));
        if( new != NULL){
            new->id = room_list->id + 1;
            strcpy(new->localsocket, "");
            new->player_list = NULL;
            new->player_num = 0;
            new->next = room_list;
            room_list = new;
        }
        else;
            //gestisci errore malloc
    }
    return room_list;
}


