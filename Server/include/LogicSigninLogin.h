//
// Created by osboxes on 12/5/22.
//

#ifndef LOGICSIGNINLOGIN_H
#define LOGICSIGNINLOGIN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/ListHandler.h"
#include "../include/MySqlUtil.h"
#include "../include/Def.h"

int signin(char incoming[], char username[], char outgoing[], struct mySQLConnection* db_connection);

int login(int sd, char incoming[], char username[], char outgoing[], struct mySQLConnection* db_connection);

int loginParser(char incoming[], char outgoing[], char username[], char password[]);

#endif /* LOGICSIGNINLOGIN_H */
