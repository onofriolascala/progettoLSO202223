
#ifndef PROGETTOLSO202223_PARSERCLIENT_H
#define PROGETTOLSO202223_PARSERCLIENT_H

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

char* parserIp(char* incoming[]);

char* parserPort(char incoming[]);

char* parserPassword(char incoming[]);

char* parserUsername(char incoming[]);

#endif //PROGETTOLSO202223_PARSERCLIENT_H
