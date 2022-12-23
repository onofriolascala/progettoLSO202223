//
// Created by Mattia on 02/12/2022.
//

#ifndef SOCKETUTILCLIENT_H
#define SOCKETUTILCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../Server/include/Def.h"

int socketInit(struct sockaddr_in *server_addr, socklen_t *len, char ip[], int port);

int localSocketInit(struct sockaddr_un *localsocket_addr, socklen_t *len);
void deleteLocalSocket(int localsocket, char localsocket_path[]);

#endif /* SOCKETUTILCLIENT_H */
