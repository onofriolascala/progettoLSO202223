//
// Created by onofrio on 15/12/22.
//

#ifndef PROGETTOLSO202223_LISTHANDLER_H
#define PROGETTOLSO202223_LISTHANDLER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "../include/ListHandler.h"
#include "../include/Def.h"

// Funzioni player_node //
struct player_node* createNewPlayerNode( int player_socket, char username[] );
struct player_node* addPlayerToPlayerList(struct player_node* playerlist_head, struct player_node* newPlayer);

struct player_node* removePlayerNode( struct player_node** player_list, int target_socket );
int destroyPlayerNode( struct player_node* player_node );

struct player_node* getPlayer( struct player_node* player_list, int target_socket );
struct player_node* getPlayerByUsername( struct player_node* playerlist_head, char usr_target[]);

// Funzioni room_node //
struct room_node* createNewRoomNode( struct room_node* list_head );
void addRoomToRoomList (struct room_node** head_pointer, struct room_node* new_room );
struct room_node* createAndAddNewRoom( struct room_node** head_pointer );

void removeAndDestroyRoomNode(struct room_node** list_head, int target_id );

struct room_node* getRoom( struct room_node* room_list, int target_id );
int lockRoomNode(struct room_node* room);
struct room_node* getRoomList( struct room_node* room_list, char outgoing[], int max_out, int* control_flag );

#endif //PROGETTOLSO202223_LISTHANDLER_H
