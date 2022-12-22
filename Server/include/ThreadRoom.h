/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#ifndef PROGETTOLSO202223_THREADROOM_H
#define PROGETTOLSO202223_THREADROOM_H

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

#include "../include/ListHandler.h"
#include "../include/CommUtilServer.h"
#include "../include/SocketUtilServer.h"
#include "../include/Def.h"



struct room_node* addNewRoom(struct room_node* room_list);

/* Funzione principale. Una volta inizializzata, comunica direttamente con tutti gli utenti connessi e gestisce la
 * logica di gioco.*/
void* thrRoom(void* arg);

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
int createNewRoom(int sd, struct room_node** room_list);
int joinRoom(int sd, int ID, struct room_node** head_pointer, char username[], char outgoing[]);
int joinParser(char incoming[], char outgoing[], char username[], int *sd);

#endif //PROGETTOLSO202223_THREADROOM_H
