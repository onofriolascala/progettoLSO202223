//
// Created by Mattia on 03/12/2022.
//

#ifndef PROGETTOLSO202223_CONSOLELAYOUTS_H
#define PROGETTOLSO202223_CONSOLELAYOUTS_H

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

#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/Prompt.h"
#include "../include/ConsoleLayouts.h"
#include "../include/Def.h"

// Macro di manipolazione del cursore
#define clearScreen()       printf("\033[2J")
#define clearLine()         printf("\033[2K")
#define carriageReturn()    printf("\r")
#define gotoxy(x,y)         printf("\033[%d;%dH", (y), (x))
#define up(x)               printf("\033[%dA", (x))
#define down(x)             printf("\033[%dB", (x))
#define right(x)            printf("\033[%dC", (x))
#define left(x)             printf("\033[%dD", (x))
#define saveCursor()        printf("\033[s")
#define loadCursor()        printf("\033[u")

// Macro di formattazione
#define defaultFormat()     printf("\033[0m")
#define bold()              printf("\033[1m")
#define red()               printf("\033[31m")
#define green()             printf("\033[32m")
#define yellow()             printf("\033[33m")
#define blue()              printf("\033[34m")
#define white()             printf("\033[37m")

// Funzione di pulizia della console. Da usare solo al passaggio da una schermata all'altra.
void emptyConsole(void);

// Funzioni di stampa dei vari input.
void inputGeneric(int input);
void inputAddress(void);
void inputPort(void);
void inputUsername(void);
void inputPassword(void);
void inputRoom(void);
int inputComfirmation(void);

// Funzioni di stampa delle varie schermate del client.
void renderConnection(void);
void renderLogin(struct server_connection *server);
void renderHomepage(struct server_connection *server);
void renderRoom(struct server_connection *server, struct room_struct *room);

#endif //PROGETTOLSO202223_CONSOLELAYOUTS_H
