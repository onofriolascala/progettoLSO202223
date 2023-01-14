//
// Created by osboxes on 12/18/22.
//

#ifndef LOGICGAME_H
#define LOGICGAME_H

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
#include <fcntl.h>

#include "../include/Def.h"

void randomSCT(int [], int, int);
void randomWords(char [3][20], char []);
void pick(char[], int);
void addLetter(char [],char [],int );

void playedWord(char []);
void parserChosenWord(char [3][20], char []);

#endif //LOGICGAME_H
