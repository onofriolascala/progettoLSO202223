//
// Created by Mattia on 03/12/2022.
//

#ifndef PROGETTOLSO202223_THREADPROMPT_H
#define PROGETTOLSO202223_THREADPROMPT_H

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
#include <fcntl.h>

#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/ThreadPrompt.h"
#include "../include/ConsoleLayouts.h"
#include "../include/Def.h"

void* thrPrompt(void* arg);

pthread_t createPrompt(int localsocket, int *prompt_socket, struct current_line *prompt_line);

void promptConnection(struct current_line *prompt_line, char outgoing[]);
void promptChar(struct current_line *prompt_line, char outgoing[], int max_len);

#endif //PROGETTOLSO202223_THREADPROMPT_H
