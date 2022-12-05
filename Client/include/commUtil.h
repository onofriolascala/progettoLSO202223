//
// Created by onofrio on 05/12/22.
//

#ifndef PROGETTOLSO202223_COMMUTIL_H
#define PROGETTOLSO202223_COMMUTIL_H

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>


int sendToServer(int sock, char msg[], int msgLenght);
int receiveFromServer(int sock, char msg[],int msgLenght);

#endif //PROGETTOLSO202223_COMMUTIL_H
