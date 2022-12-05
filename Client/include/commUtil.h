//
// Created by onofrio on 05/12/22.
//

#ifndef PROGETTOLSO202223_COMMUTIL_H
#define PROGETTOLSO202223_COMMUTIL_H

int sendToServer(int sock, char msg[], int msgLenght);
int receiveFromServer(int sock, char msg[],int msgLenght);
ssize_t readn(int fd,void* buf,size_t n);
ssize_t writen(int fd,const void* buff,size_t n);

#endif //PROGETTOLSO202223_COMMUTIL_H
