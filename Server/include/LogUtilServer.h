//
// Created by onofrio on 04/08/23.
//

#ifndef PROGETTOLSO202223_LOGUTILSERVER_H
#define PROGETTOLSO202223_LOGUTILSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#define MAXERRORBUFFER 1024
#define defaultFormat()     printf("\033[0m")
#define bold()              printf("\033[1m")
#define red()               printf("\033[31m")
#define green()             printf("\033[32m")
#define yellow()            printf("\033[33m")

#include "../include/Def.h"



int createLog(int room_id);
int writeToLog(int fd, const char input_buf[]);

#endif //PROGETTOLSO202223_LOGUTILSERVER_H
