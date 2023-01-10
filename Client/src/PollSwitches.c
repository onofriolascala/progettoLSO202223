//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, int prompt_socket, int signal_num, char incoming[]) {
    int end_loop;

    end_loop = 0;

    switch (signal_num) {
        case -1:
            // EWOULDBLOCK ERROR
            end_loop = 0;
            break;
        case -2: case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            end_loop = 1;
            break;
        case S_DISCONNECT_ABRUPT:
            // La lettura della socket ha restituito 0, indicando una chiusura inaspettata da parte del server.
            // Il client ritornerà alla schermata di connessione.
            break;
        case S_DISCONNECT:
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
    int visited;

    char *ip, *port, *saveptr;

    end_loop = 0;

    do {
        visited = 0;
        switch (signal_num) {
            case -1:
                // EWOULDBLOCK ERROR
                end_loop = 0;
                break;
            case -2:
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                end_loop = 1;
                break;
            case S_DISCONNECT_ABRUPT:
                // La lettura della socket locale ha restituito 0, indicando una chiusura inattesa.
                // Un simile comportamento non è previsto dal programma, che per tanto procederà a chiudersi.
                fprintf(stderr, ":SWITCH PROMPT ERROR: prompt socket read ha returned 0. Closing program.\n");
                end_loop = 1;
                break;
            case S_DISCONNECT:
                // Il prompt ha inviato un segnale di disconnessione dal server a cui il client è connesso in quel
                // momento. Il client si disconnetterà dal server, ripulirà la struttura della connessione ed infine
                // mostrerà la schermata di connessione.

                //printf("\t\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
                writeToServer(*(server->sd), signal_num, incoming);
                close(*(server->sd));
                *(server->sd) = -1;
                memset(server->ip, '\0', sizeof(server->ip));
                writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
                break;
            case C_CONNECTION:
                // Il prompt desidera connettersi al processo server identificato da indirizzo IPv4 e porta forniti.
                // I valori saranno esaminati perché rispettino i requisiti non funzionali. In caso di successo saranno
                // inviati al server, altrimenti saranno richiesti nuovamente.
                printf("\t\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);

                if (visited == 0) {
                    emptyConsole();
                    renderConnection();
                    visited = 1;
                }

                ip = parserIp(&incoming);
                port = parserPort(&incoming);

                /*ip = strtok_r(incoming, "-", &saveptr);
                port = strtok_r(NULL, "\0", &saveptr);

                strcpy(server->ip, ip);
                server->port = atoi(port);*/

                if ((*(server->sd) = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                    fprintf(stderr,":SWITCH PROMPT ERROR: Apertura della server socket non riuscita correttamente. Riprovare.\n");
                    *(server->sd) = -1;
                    //writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
                    break;
                }
                visited = 0;
                break;
            default:
                printf("\t\tPROMPT_SWITCH: <Default> %d:%s\n", signal_num, incoming);
                writeToServer(*(server->sd), signal_num, incoming);
        }
    } while(visited != 0);
    return end_loop;
}