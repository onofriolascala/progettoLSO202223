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
#include "../include/ThreadPrompt.h"
#include "../include/ConsoleLayouts.h"
#include "../include/Def.h"

// Funzione di pulizia della console. Da usare solo al passaggio da una schermata all'altra.
void emptyConsole(void);

// Funzioni di stampa dei vari input.
void inputGeneric(void);
void inputAddress(void);
void inputPort(void);
void inputUsername(void);
void inputPassword(void);
void inputRoom(void);
void inputConfirmation(void);

// Funzioni di stampa delle varie schermate del client.
void renderConnection(void);
void renderLogin(void);
void renderHomepage(void);

#endif //PROGETTOLSO202223_CONSOLELAYOUTS_H
