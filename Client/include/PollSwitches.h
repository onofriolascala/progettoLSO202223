//
// Created by osboxes on 12/23/22.
//

#ifndef POLLSWITCHES_H
#define POLLSWITCHES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <poll.h>

#include "../include/CommUtilClient.h"
#include "../include/ThreadRender.h"
#include "../../Server/include/Def.h"

int switchServer(struct pollfd fds[], int i, char incoming[]);
int switchPrompt(struct pollfd fds[], int i, int client_mode, int signal_num, char incoming[]);

#endif // POLLSWITCHES_H
