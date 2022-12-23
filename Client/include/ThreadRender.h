//
// Created by Mattia on 03/12/2022.
//

#ifndef PROGETTOLSO202223_THREADRENDER_H
#define PROGETTOLSO202223_THREADRENDER_H

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

#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/ThreadPrompt.h"
#include "../include/ThreadRender.h"
#include "../../Server/include/Def.h"

/*void* thrRender(void* arg);

pthread_t createRender(int localsocket, int *render_socket);*/

void emptyConsole(void);
void renderConnection(void);
void renderLogin(void);
void renderHomepage(void);

#endif //PROGETTOLSO202223_THREADRENDER_H
