//
// Created by osboxes on 12/5/22.
//

#ifndef LOGICSIGNINLOGIN_H
#define LOGICSIGNINLOGIN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../include/ListHandler.h"
#include "../include/Def.h"

int signin(char incoming[], struct player_node* player, char outgoing[]);

struct player_node* logout(struct player_node* player);

int login(int sd, char incoming[], struct player_node* player, char outgoing[]);

int loginParser(char incoming[], char outgoing[], char username[], char password[]);

#endif /* LOGICSIGNINLOGIN_H */
