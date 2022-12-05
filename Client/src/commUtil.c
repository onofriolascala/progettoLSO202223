#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "../include/commUtil.h"



int sendToServer(int sock, char msg[], int msgLenght){
    int n;
    if((n=writen(sock,msg,msgLenght)) < msgLenght){
        if(n < 0){
            perror(":WRITE ERROR:");
            return -1;
        }
    }
    return n;
}

int receiveFromServer(int sock, char msg[],int msgLenght){
    int n;
    if((n=readn(sock, msg, msgLenght ) < msgLenght)){
        if(n < 0){
            perror(":READ ERROR:");
            return -1;
        }
    }
    return n;
}
