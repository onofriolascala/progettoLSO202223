//
// Created by osboxes on 4/11/23.
//

#ifndef PROGETTOLSO202223_MYSQLUTIL_H
#define PROGETTOLSO202223_MYSQLUTIL_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define USERNAMELENGHT 100
#define QUERYLENGHT 1024

struct mySQLConnection {
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;

    char hostname[USERNAMELENGHT];
    char port[USERNAMELENGHT];
    char username[USERNAMELENGHT];
    char password[USERNAMELENGHT];
    char database[USERNAMELENGHT];
};

int selectQuery(struct mySQLConnection* LSO2223, char username[], char password[]);
int insertQuery(struct mySQLConnection* LSO2223, char username[], char password[]);
struct mySQLConnection* establishDBConnection(void);

#endif //PROGETTOLSO202223_MYSQLUTIL_H
