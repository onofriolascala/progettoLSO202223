//
// Created by Mattia on 02/12/2022.
//

#ifndef SOCKETUTILCLIENT_H
#define SOCKETUTILCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/Def.h"

// Funzione di inizializzazione della connessione al server. Riceve l'indirizzo e la porta a cui si desidera
// connettersi e restituisce il valore del socket descriptor su cui è stata stabilita la connessione.
int socketInit(struct sockaddr_in *server_addr, socklen_t *len, char ip[], int port);

// Funzione di inizializzazione della connessione locale con il thread del PROMPT. Riceve un indirizzo del
// file system (una costante), e restituisce la listening socket della connessione.
int localSocketInit(struct sockaddr_un *localsocket_addr, socklen_t *len, struct prompt_thread *prompt);

// Elimina in sicurezza la socket locale al momento della chiusura del main thread. Riceve una socket ed un
// indirizzo di memoria su cui effettuare il CLOSE() e l'UNLINK().
void deleteLocalSocket(struct prompt_thread *prompt);

#endif /* SOCKETUTILCLIENT_H */
