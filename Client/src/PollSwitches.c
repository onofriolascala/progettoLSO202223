//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct pollfd fds[], int i, char incoming[]) {
    int close_conn, signal_num;

    close_conn = 0;
    signal_num = readFromServer(fds[i].fd, incoming, MAXCOMMBUFFER);

    switch (signal_num) {
        case -1:
            // EWOULDBLOCK ERROR
            break;
        case -2:
            close_conn = 1;
            break;
        case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            close_conn = 1;
            break;
        case S_DISCONNECT_ABRUPT:
            printf("\t\t\t\tMAIN: <Disconnessione> %d:%s\n", signal_num, incoming);
            close_conn = 1;
            break;
        case S_DISCONNECT:
            printf("\t\t\t\tMAIN: <Disconnessione> %d:%s\n", signal_num, incoming);
            close_conn = 1;
            writeToServer(fds[i].fd, S_DISCONNECT, S_DISCONNECT_MSG);
            break;
        case C_LOGIN:
            printf("\t\t\t\tMAIN: <Login> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo login.");
            break;
        case C_SIGNIN:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo sign in.");
            break;
        case C_CREATEROOM:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di crearne una nuova.");
            break;
        case C_JOINROOM:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di unirsi ad un'altra.");
            break;
        case C_LISTROOM:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, S_NOPERMISSION_MSG);
            break;
        case C_LOGOUT:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di effettuare il logout.");
            break;
        case C_SELECTWORD:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_OK, "Parola.");
            break;
        case C_GUESSSKIP:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, signal_num, "Mancato");
            break;
        case C_EXITROOM:
            printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
            //writeToServer(fds[i].fd, S_HOMEPAGEOK, S_HOMEPAGEOK_MSG);
            break;
        default:
            printf("\t\t\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
            //writeToServer(fds[i].fd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
    }
    return close_conn;
}

int switchPrompt(struct pollfd fds[], int i, int client_mode, int signal_num, char incoming[]) {
    char *server_p, *port_p;

    //signal_num = readFromServer(fds[i].fd, "Enable", MAXCOMMBUFFER);

    if(client_mode == 0) {
        switch (signal_num) {
            case -1:
                client_mode = 0;
                // EWOULDBLOCK ERROR
                break;
            case -2:
                client_mode = 0;
                break;
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                client_mode = 0;
                break;
            case C_LOGIN:
                printf("\t\t\t\tMAIN: <Login> %d:%s\n", signal_num, incoming);

                if (signal_num == 42) {

                    // attempt Connection
                    if(signal_num == 0) {
                        server_p = strtok_r(incoming, "-", &saveptr);
                        port_p = strtok_r(NULL, "\0", &saveptr);

                        if((fds[0].fd = socketInit(&server_addr, &server_len, server_p, atoi(port_p))) < 4) {
                            printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                            continue;
                        }
                    }
                    else {
                        printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                        continue;
                    }

                    server_p = strtok_r(incoming, "-", &saveptr);
                    port_p = strtok_r(NULL, "\0", &saveptr);

                    fds[1].fd = socketInit(&server_addr, &server_len, server_p, atoi(port_p));
                    fds[1].events = POLLIN;
                    num_fds++;

                    printf("DEBUG PROMPT %d %s\n", fds[i].fd, incoming);

                    emptyConsole();
                    renderLogin();
                }
                if (signal_num == 51) {
                    writeToServer(fds[1].fd, 10, incoming);
                }

                /*if (num_fds == 1) {
                    printf("Type anything for standard procedure. Type 1 for automated connection & Login.\n"
                           "Input: ");
                    // Connection Routine //
                    fgets(incoming, MAXCOMMBUFFER, stdin);
                    if(atoi(incoming) == 1) {
                        strcpy(server_ip, "25.72.233.6");
                        server_port = 5200;
                        sprintf(incoming, "%s-%d;", server_ip, server_port);
                    }
                    else
                    {
                        emptyConsole();
                        renderConnection();
                        writeToServer(prompt_socket, 0, "Connessione;");
                        signal_num = readFromServer(prompt_socket, incoming, MAXCOMMBUFFER);
                    }
                    printf("%s\n", incoming);
                    // attempt Connection
                    if(signal_num == 0) {
                        server_p = strtok_r(incoming, "-", &saveptr);

                        port_p = strtok_r(NULL, "\0", &saveptr);

                        if((fds[0].fd = socketInit(&server_addr, &server_len, server_p, atoi(port_p))) < 4) {
                            printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                            continue;
                        }
                        num_fds++;
                    }
                    else {
                        printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                        continue;
                    }*/

                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo login.");
                break;
            default:
                printf("\t\t\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
        }
    }
    else if(client_mode == 1) {
        switch (signal_num) {
            case -1:
                client_mode = 0;
                // EWOULDBLOCK ERROR
                break;
            case -2:
                client_mode = 0;
                break;
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                client_mode = 0;
                break;
            case S_DISCONNECT_ABRUPT:
                printf("\t\t\t\tMAIN: <Disconnessione> %d:%s\n", signal_num, incoming);
                client_mode = 0;
                break;
            case S_DISCONNECT:
                printf("\t\t\t\tMAIN: <Disconnessione> %d:%s\n", signal_num, incoming);
                client_mode = 0;
                writeToServer(sd, S_DISCONNECT, S_DISCONNECT_MSG);
                break;
            case 2:
                printf("\t\t\t\tMAIN: <Login> %d:%s\n", signal_num, incoming);

                /*if (num_fds == 1) {
                    printf("Type anything for standard procedure. Type 1 for automated connection & Login.\n"
                           "Input: ");
                    // Connection Routine //
                    fgets(incoming, MAXCOMMBUFFER, stdin);
                    if(atoi(incoming) == 1) {
                        strcpy(server_ip, "25.72.233.6");
                        server_port = 5200;
                        sprintf(incoming, "%s-%d;", server_ip, server_port);
                    }
                    else
                    {
                        emptyConsole();
                        renderConnection();
                        writeToServer(prompt_socket, 0, "Connessione;");
                        signal_num = readFromServer(prompt_socket, incoming, MAXCOMMBUFFER);
                    }
                    printf("%s\n", incoming);
                    // attempt Connection
                    if(signal_num == 0) {
                        server_p = strtok_r(incoming, "-", &saveptr);

                        port_p = strtok_r(NULL, "\0", &saveptr);

                        if((fds[0].fd = socketInit(&server_addr, &server_len, server_p, atoi(port_p))) < 4) {
                            printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                            continue;
                        }
                        num_fds++;
                    }
                    else {
                        printf("Connessione non riuscita col seguente messaggio \"%s\". Nuovo tentativo.\n\n", outgoing);
                        continue;
                    }*/

                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo login.");
                break;
            case C_SIGNIN:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo sign in.");
                break;
            case C_CREATEROOM:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di crearne una nuova.");
                break;
            case C_JOINROOM:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di unirsi ad un'altra.");
                break;
            case C_LISTROOM:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                break;
            case C_LOGOUT:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di effettuare il logout.");
                break;
            case C_SELECTWORD:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_OK, "Parola.");
                break;
            case C_GUESSSKIP:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, signal_num, "Mancato");
                break;
            case C_EXITROOM:
                printf("\t\t\t\tMAIN: <Placeholder> %d:%s\n", signal_num, incoming);
                //writeToServer(fds[i].fd, S_HOMEPAGEOK, S_HOMEPAGEOK_MSG);
                break;
            default:
                printf("\t\t\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                //writeToServer(fds[i].fd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
        }
    }
    return client_mode;
}