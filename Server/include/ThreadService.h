/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#ifndef THREADSERVICE_H
#define THREADSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../include/ThreadRoom.h"
#include "../include/CommUtilServer.h"
#include "../include/ListHandler.h"
#include "../include/LogicSigninLogin.h"
#include "../include/Def.h"

void* thrService(void* arg);

pthread_t createNewService(int sd2, struct room_node** room_list);
pthread_t rebuildService(struct player_node* player, struct room_node** room_list);

#endif /* THREADSERVICE_H */
