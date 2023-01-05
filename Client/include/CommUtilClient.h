//
// Created by onofrio on 05/12/22.
//

#ifndef PROGETTOLSO202223_COMMUTILCLIENT_H
#define PROGETTOLSO202223_COMMUTILCLIENT_H

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

#include "../../Server/include/Def.h"

int relayStatus(int sd, int signal_num);
int receiveStatus(int sd);

int writeToServer(int sd, int signal_num, char outgoing[]);
int readFromServer(int sd, char incoming[],int max_len);

#endif //PROGETTOLSO202223_COMMUTILCLIENT_H
