//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, int prompt_socket, int signal_num, char incoming[]) {
    int end_loop;

    end_loop = 0;

    switch (signal_num) {
        case -1: case -2: case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            end_loop = 1;
            break;
        case S_DISCONNECT: S_DISCONNECT_ABRUPT:
            printf("\t\tSERVER_SWITCH: <Server Disconnected> %d:%s\n", signal_num, incoming);
            close(*(server->sd));
            *(server->sd) = -1;

            sleep(3);
            emptyConsole();
            renderConnection();
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            break;
        case S_LOGINOK:
            renderLogin();
            writeToServer(prompt_socket, C_LOGIN, "C_LOGIN");
            // check login
            break;
        case S_HOMEPAGEOK:
            printf("\t\tSERVER_SWITCH: <Login Successful> %d:%s\n", signal_num, incoming);
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            break;
        default:
            fprintf(stderr, "SERVER_SWITCH: <ERROR> signal \"%d\" with message \"%s\" not recognized. Disconnecting in 3 seconds.\n", signal_num, incoming);
            writeToServer(*(server->sd), S_DISCONNECT, S_UNKNOWNSIGNAL_MSG);
            close(*(server->sd));
            *(server->sd) = -1;

            sleep(3);
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
    }
    return end_loop;
}

int switchPrompt(struct server_connection *server, int prompt_socket, int signal_num, char incoming[]) {
    int end_loop;

    end_loop = 0;

    switch (signal_num) {
        case -1: case -2: case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            end_loop = 1;
            break;
        case S_DISCONNECT_ABRUPT:
            //lock mutex
            //printf("\t\tPROMPT_SWITCH: <Exit> %d:%s\n", signal_num, incoming);
            end_loop = 1;
            break;
        case S_DISCONNECT:
            //printf("\t\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
            close(*(server->sd));
            *(server->sd) = -1;
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            break;
        case C_CONNECTION:
            printf("\t\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);

            char *ip, *port, *saveptr;

            ip = strtok_r(incoming, "-", &saveptr);
            port = strtok_r(NULL, "\0", &saveptr);

            strcpy(server->ip, ip);
            server->port = atoi(port);

            if((*(server->sd) = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                //fprintf(stderr,"Apertura della socket non riuscita correttamente. Chiusura applicazione.\n\n");
                *(server->sd) = -1;
                writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            }
            break;
        default:
            printf("\t\tPROMPT_SWITCH: <Default> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
    }
    return end_loop;
}