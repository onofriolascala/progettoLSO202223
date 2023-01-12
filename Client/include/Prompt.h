//
// Created by Mattia on 03/12/2022.
//

#ifndef PROGETTOLSO202223_PROMPT_H
#define PROGETTOLSO202223_PROMPT_H

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
#include <ctype.h>

#include "../include/ConsoleLayouts.h"
#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/Prompt.h"
#include "../include/Def.h"

void* thrPrompt(void* arg);

pthread_t createPrompt(int localsocket, struct prompt_thread *prompt);

int promptConnection(struct prompt_thread *prompt_line, char outgoing[]);
int promptLogin(struct prompt_thread *prompt, char outgoing[]);
int promptHomepage(struct prompt_thread *prompt, char outgoing[]);
int promptString(struct prompt_thread *prompt_line, char buffer[], int max_len);

int promptSelection(struct prompt_thread *prompt_line, char max_selection);

#endif //PROGETTOLSO202223_PROMPT_H
