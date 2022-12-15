//
// Created by Mattia on 15/12/2022.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/Def.h"


void newHead(struct player_node* head){
    struct player_node temp;
    temp=*head;
    *head=malloc()
}




struct room_node* addNewRoom(struct room_node* room_list) {
    struct room_node* new;
    if(room_list == NULL){
        new = (struct room_node*)malloc(sizeof(struct room_node));
        new->id = 1;
        strcpy(new->localsocket, "");
        new->player_list = NULL;
        new->player_num = 0;
        new->next = NULL;
        room_list = new; //aggiorno la testa della lista delle stanze
    }
    else{
        new = (struct room_node*)malloc(sizeof(struct room_node));
        new->id = room_list->id + 1;
        strcpy(new->localsocket, "");
        new->player_list = NULL;
        new->player_num = 0;
        new->next = room_list;
        room_list = new; //aggiorno la testa della lista delle stanze
    }
    return room_list;
}