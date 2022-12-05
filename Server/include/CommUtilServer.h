//
// Created by osboxes on 12/5/22.
//



#ifndef COMMUTILSERVER_H
#define COMMUTILSERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int sendToServer(int sock, char msg[], int msgLenght);
int receiveFromServer(int sock, char msg[],int msgLenght);

#endif /* COMMUTILSERVER_H */
