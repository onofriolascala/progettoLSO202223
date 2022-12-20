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

int login(int sd, char incoming[], struct player_node* player, char outgoing[]);

#endif /* LOGICSIGNINLOGIN_H */
