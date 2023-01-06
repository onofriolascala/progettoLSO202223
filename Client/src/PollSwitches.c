//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, int signal_num, char incoming[]) {

}

int switchPrompt(struct server_connection *server, int signal_num, char incoming[]) {
    int end_loop;

    switch (signal_num) {
        case -1:
            end_loop = 1;
            // EWOULDBLOCK ERROR
            break;
        case -2:
            end_loop = 1;
            break;
        case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            end_loop = 1;
            break;
        case S_DISCONNECT_ABRUPT:
            printf("\t\tPROMPT_SWITCH: <Exit> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
            close(*(server->sd));
            end_loop = 1;
            break;
        case S_DISCONNECT:
            printf("\t\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
            close(*(server->sd));
            end_loop = 0;
            break;
        case C_CONNECTION:
            printf("\t\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);

            char *ip, *port, *saveptr;

            ip = strtok_r(incoming, "-", &saveptr);
            port = strtok_r(NULL, "\0", &saveptr);

            strcpy(server->ip, ip);
            server->port = atoi(port);

            if((*(server->sd) = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                fprintf(stderr,"Apertura della socket non riuscita correttamente. Chiusura applicazione.\n\n");
                exit(1);
            }

            end_loop = 0;
            break;
        default:
            printf("\t\tPROMPT_SWITCH: <Default> %d:%s\n", signal_num, incoming);

            writeToServer(*(server->sd), signal_num, incoming);

            end_loop = 0;
            break;
    }
    return end_loop;
}