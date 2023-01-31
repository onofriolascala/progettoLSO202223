//
// Created by osboxes on 1/30/23.
//

#ifndef PARSERSERVER_H
#define PARSERSERVER_H

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

#include "../include/ParserServer.h"
#include "../include/Def.h"

int parserInteger(char incoming[], char outgoing[]);

#endif //PARSERSERVER_H
