
#include "../include/CommUtilClient.h"



int sendToServer(int sock, char msg[], int msgLenght){
    int n;
    if((n=write(sock,msg,msgLenght)) < msgLenght){
        if(n < 0){
            perror(":WRITE ERROR:");
            return -1;
        }
    }
    return n;
}

int receiveFromServer(int sock, char msg[],int msgLenght){
    int n;
    if((n=read(sock, msg, msgLenght ) < msgLenght)){
        if(n < 0){
            perror(":READ ERROR:");
            return -1;
        }
    }
    return n;
}
