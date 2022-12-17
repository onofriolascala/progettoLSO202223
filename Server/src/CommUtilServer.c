//
// Created by osboxes on 12/5/22.
//

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

#include "../include/CommUtilServer.h"
#include "../include/Def.h"



int writeToServer_old(int sock, char msg[], int msgLenght){
    int n;
    if((n = write(sock, msg, msgLenght)) < msgLenght){
        if(n < 0){
            perror(":WRITE ERROR");
            return -1;
        }
    }
    return n;
}

int readFromServer_old(int sock, char msg[], int msgLenght){
    int n;
    if((n = read(sock, msg, msgLenght ) < msgLenght)){
        if(n < 0){
            perror(":READ ERROR");
            return -1;
        }
    }
    return n;
}

// Scrittura al client con anesso parser per l'impacchettamento dei segnali.
int writeToClient(int sd, int signal_num, char outgoing[]) {
    char finalmessage[MAXCOMMBUFFER];
    int len;
    if(sprintf(finalmessage, "%d:%s", signal_num, outgoing) < 0) {
        perror(":COMPOSITION ERROR");
        return -1;
    }
    len = strlen(finalmessage);
    if(write(sd, finalmessage, len) < 0) {
        perror(":WRITE ERROR");
        return -1;
    }
    return 0;
}

// Lettura dal server con annesso parser per spacchettamento del signal code. Restituisce il segnale inviato dal client
int readFromClient(int sd, char incoming[], int max_len){
    int signal_num;
    char signal_code[MAXSIGNALBUF+1], tmp[MAXSIGNALBUF];

    //printf("DEBUG %s\n", incoming);

    // Lettura del segnale inviato dal client
    if((read(sd, signal_code, MAXSIGNALBUF)) < 0)
    {
        perror(":SIGNAL CODE READ ERROR");
        return -1;
    }
    //printf("DEBUG %s\n", signal_code);
    // Rimozione del separatore
    if((read(sd, tmp, 1)) < 0)
    {
        perror(":SEPARATOR READ ERROR");
        return -2;
    }
    // Lettura del messaggio associato al segnale.
    if((read(sd, incoming, max_len)) < 0)
    {
        perror(":MESSAGE READ ERROR");
        return -3;
    }

    signal_code[MAXSIGNALBUF] = '\0';
    signal_num = atoi(signal_code);

    return signal_num;
}