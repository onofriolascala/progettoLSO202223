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
#include <sys/poll.h>


#include "../include/ListHandler.h"
#include "../include/CommUtilServer.h"
#include "../include/SocketUtilServer.h"
#include "../include/Def.h"



struct room_node* addNewRoom(struct room_node* room_list);

/* Funzione principale. Una volta inizializzata, comunica direttamente con tutti gli utenti connessi e gestisce la
 * logica di gioco.*/
void* thrRoom(void* arg);
// Creazione del thread. Richiama <foo_name> dal thread costruttore.
int createNewRoom(int sd, struct room_node** room_list, struct mySQLConnection* db_connection);
int joinRoom(int sd, int ID, struct room_node** head_pointer, char username[], char outgoing[]);
/* Funzione che prende una comunicazione in ingresso che contiene una parola e la estrae */
int guessParser(char incoming[], char outgoing[], char guess[] );
/* Funzione che impacchetta le informazioni della stanza nella stringa outgoing*/
void getRoomInfo(struct player_node* suzerain, int player_num, char selected_word[], char outgoing[MAXCOMMBUFFER]);
void movePlayerTurn(struct player_node** curr_player,struct player_node* suzerain, int* addHintFlag);
void moveSuzerainTurn(struct player_node** curr_suzerain, struct player_node* new_suzerain);
#endif //PROGETTOLSO202223_THREADROOM_H
