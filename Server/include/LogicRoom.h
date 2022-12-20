//
// Created by osboxes on 12/18/22.
//

#ifndef LOGICROOM_H
#define LOGICROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>


#include "../include/CommUtilServer.h"
#include "../include/Def.h"

void mainRoomLoop(int listen_local_sd);

#endif //LOGICROOM_H
