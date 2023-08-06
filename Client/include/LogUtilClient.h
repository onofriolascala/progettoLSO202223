//
// Created by osboxes on 1/13/23.
//

#ifndef PROGETTOLSO202223_LOGUTILCLIENT_H
#define PROGETTOLSO202223_LOGUTILCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>


#include "../include/ConsoleLayouts.h"
#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/Prompt.h"
#include "../include/Def.h"

int createLog(void);
int writeToLog(int fd, const char input_buf[]);
int printError(struct prompt_thread *prompt, char error_out[], char error_str[], int error_no);
int printErrorNoNumber(struct prompt_thread *prompt, char error_out[], char error_str[]);
int printWarning(struct prompt_thread *prompt, char input_buf[]);

#endif //PROGETTOLSO202223_LOGUTILCLIENT_H
