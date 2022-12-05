//
// Created by Mattia on 02/12/2022.
//

#ifndef SOCKETUTILCLIENT_H
#define SOCKETUTILCLIENT_H

int socketInit(struct sockaddr_in* server_addr, socklen_t* len, char ip[], int port);

#endif /* SOCKETUTILCLIENT_H */
