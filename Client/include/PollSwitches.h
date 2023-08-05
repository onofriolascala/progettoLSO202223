//
// Created by osboxes on 12/23/22.
//

#ifndef POLLSWITCHES_H
#define POLLSWITCHES_H

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

#include <poll.h>

#include "../include/LogUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/ConsoleLayouts.h"
#include "../include/SocketUtilClient.h"
#include "../include/ParserClient.h"
#include "../include/Def.h"

int switchServer(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]);
int switchPrompt(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]);

void addMessage(struct room_struct *room, char *incoming);
void emptyMessageList(struct room_struct *room);
void changePlayers(struct room_struct *room, char *incoming);
void changeSuzerain(struct room_struct *room, char *incoming);

#endif // POLLSWITCHES_H
