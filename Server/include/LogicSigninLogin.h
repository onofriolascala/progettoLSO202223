//
// Created by osboxes on 12/5/22.
//

#ifndef LOGICSIGNINLOGIN_H
#define LOGICSIGNINLOGIN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../include/ListHandler.h"
#include "../include/Def.h"

int signin(char incoming[], char username[], char outgoing[]);

int login(int sd, char incoming[], char username[], char outgoing[]);

int loginParser(char incoming[], char outgoing[], char username[], char password[]);

#endif /* LOGICSIGNINLOGIN_H */
