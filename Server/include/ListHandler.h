//
// Created by onofrio on 15/12/22.
//

#ifndef PROGETTOLSO202223_LISTHANDLER_H
#define PROGETTOLSO202223_LISTHANDLER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/Def.h"

struct player_node* createNewPlayerNode( int player_socket, char username[] );

struct player_node* removePlayerNode( struct player_node* player_list, int target_socket );

int destroyPlayerNode( struct player_node* player_node );

struct room_node* createNewRoomNode( struct room_node* room_list );

#endif //PROGETTOLSO202223_LISTHANDLER_H
