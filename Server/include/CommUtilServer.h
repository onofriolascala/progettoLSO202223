//
// Created by osboxes on 12/5/22.
//

#ifndef COMMUTILSERVER_H
#define COMMUTILSERVER_H

int writeToServer_old(int sock, char msg[], int msgLenght);
int readFromServer_old(int sock, char msg[],int msgLenght);

int readFromClient(int sd, char incoming[],int max_len);
int writeToClient(int sd, int signal_num, char outgoing[]);

#endif /* COMMUTILSERVER_H */
