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
            printf("\t\tSERVER_SWITCH: <Server Disconnected> %d:%s\n", signal_num, incoming);
            close(*(server->sd));
            *(server->sd) = -1;

            sleep(5);
            emptyConsole();
            renderConnection();
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            break;
        case S_DISCONNECT:
            // Il server comunica di aver recepito l'intento di disconnessione. Il client provvederà a chiudere
            // la connessione, e riportare la console alla schermata di connessione.
            printf("\t\tSERVER_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);

            close(*(server->sd));
            *(server->sd) = -1;
            memset(server->ip, '\0', sizeof(server->ip));

            sleep(5);
            emptyConsole();
            renderConnection();
            writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
            break;
        case S_LOGINOK:
            // Il server comunica il via libera alla schermata di login. Il client provvederà ad attivare
            // il prompt associato.
            renderLogin();
            writeToServer(prompt_socket, C_LOGIN, "C_LOGIN");
            // check login
            break;
        case S_HOMEPAGEOK:
            // Il server comunica il via libera alla schermata di homepage. Il client provvederà ad attivare
            // il prompt associato.
            printf("\t\tSERVER_SWITCH: <Login Successful> %d:%s\n", signal_num, incoming);
            renderHomepage();
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
    int end_loop, parser_return;

    char *ip, *port, *saveptr;

    end_loop = 0;

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
            // momento. Il client provvederà a trasferirlo al server.
            printf("\t\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
            break;
        case C_CONNECTION:
            // Il prompt desidera connettersi al processo server identificato da indirizzo IPv4 e porta forniti.
            // I valori saranno esaminati perché rispettino i requisiti non funzionali. In caso di successo saranno
            // inviati al server, altrimenti saranno richiesti nuovamente.
            printf("\t\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);

            parser_return = parserIp(incoming, server);
            if(parser_return == -1) {
                printf("L'indirizzo IPv4 fornito non è della lunghezza corretta. Riprovare.\n");

            }
            else if(parser_return == -2) {
                printf("L'indirizzo IPv4 può contenere solo caratteri numerici. Riprovare.\n");

            }
            parser_return = parserPort(incoming, server);
            if(parser_return == -1) {
                printf("La porta fornita non è della lunghezza corretta. Riprovare.\n");

            }
            else if(parser_return == -2) {
                printf("La porta può contenere solo caratteri numerici. Riprovare.\n");

            }

            if ((*(server->sd) = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                fprintf(stderr,":SWITCH PROMPT ERROR: Apertura della server socket non riuscita correttamente. Riprovare.\n");
                *(server->sd) = -1;
                writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
                break;
            }
            break;
        case C_LOGIN:
            printf("\t\tPROMPT_SWITCH: <Login> %d:%s\n", signal_num, incoming);
            writeToServer(prompt_socket, 88, "C_CONNECTION");
            break;
        case C_SIGNIN:
            printf("\t\tPROMPT_SWITCH: <Signin> %d:%s\n", signal_num, incoming);
            writeToServer(prompt_socket, 88, "C_CONNECTION");
            break;
        default:
            printf("\t\tPROMPT_SWITCH: <Default> %d:%s\n", signal_num, incoming);
            writeToServer(*(server->sd), signal_num, incoming);
    }
    return end_loop;
}