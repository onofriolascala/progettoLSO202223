//
// Created by osboxes on 12/5/22.
//

#ifndef COMMUTILSERVER_H
#define COMMUTILSERVER_H

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

#include "../include/Def.h"

int readFromClient(int sd, char incoming[],int max_len);
int writeToClient(int sd, int signal_num, char outgoing[]);

#endif /* COMMUTILSERVER_H */
