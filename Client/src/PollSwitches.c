//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]) {
    int end_loop, same_signal;

    if(server->last_signal != signal_num) {
        emptyConsole();
        server->last_signal == signal_num;
        same_signal = 0;
    }
    else {
        same_signal = 1;
    }

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
        case S_DISCONNECT:
            // Il server comunica di aver recepito l'intento di disconnessione. Il client provvederà a chiudere
            // la connessione, e riportare la console alla schermata di connessione.
            pthread_mutex_lock(&prompt->mutex);
            printf("\n\t\tSERVER_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            close(*server->sd);
            *server->sd = -1;

            pthread_cancel(prompt->id);
            close(*prompt->sd);
            prompt->id = createPrompt(*server->localsocket, prompt);

            sleep(5);
            if( !same_signal ) renderConnection();
            pthread_mutex_unlock(&prompt->mutex);
            writeToServer(*prompt->sd, C_CONNECTION, "C_CONNECTION");
            break;
        case S_LOGINOK:
            // Il server comunica il via libera alla schermata di login. Il client provvederà ad attivare
            // il prompt associato.
            printf("\t\tSERVER_SWITCH: <Connection Successful> %d:%s\n", signal_num, incoming);
            //emptyConsole();
            if( !same_signal ) renderLogin(server);
            if(strcmp(incoming, S_LOGINOK_MSG) != 0) printf("\t%s\n", incoming);
            writeToServer(*prompt->sd, C_LOGIN, "C_LOGIN");
            // check login
            break;
        case S_HOMEPAGEOK:
            // Il server comunica il via libera alla schermata di homepage. Il client provvederà ad attivare
            // il prompt associato.
            printf("\t\tSERVER_SWITCH: <Login Successful> %d:%s\n", signal_num, incoming);
            if( !same_signal ) renderHomepage(server);
            if(strcmp(incoming, S_HOMEPAGEOK_MSG) != 0) printf("\t%s\n", incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        case S_ROOMLISTOK:
            // Il server comunica il via libera della stampa delle stanze esistenti sul server. Il client procede
            // ad estrarle, formattarle ed infine riattivare il prompt.
            printf("\t\tSERVER_SWITCH: <Join Successful> %d:%s\n", signal_num, incoming);
            if( !same_signal ) renderRoom(server, room);
            //if(strcmp(incoming, S_HOMEPAGEOK_MSG)) printf("\t%s\n", incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        case S_ROOMOK:
            // Il server comunica l'avvenuto accesso alla stanza. Verrà eseguita la prima stampa della stanza.
            printf("\t\tSERVER_SWITCH: <Join Successful> %d:%s\n", signal_num, incoming);
            emptyConsole();
            renderRoom(server, room);
            //if(strcmp(incoming, S_HOMEPAGEOK_MSG)) printf("\t%s\n", incoming);
            writeToServer(*prompt->sd, C_GUESSSKIP, incoming);
            break;
        case S_MISSEDGUESS:
            //
            printf("\t\tSERVER_SWITCH: <Missed Guess> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        case S_VICTORY:
            //
            printf("\t\tSERVER_SWITCH: <Victoryl> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        case S_DEFEAT:
            //
            printf("\t\tSERVER_SWITCH: <Defeat> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        case S_ENDOFTURN:
            //
            printf("\t\tSERVER_SWITCH: <End of Turn> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_CREATEROOM, "C_CREATEROOM");
            break;
        //          CASI DI ERRORE          //
        case S_FULLROOM:
            // La stanza è piena. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_ROOMNOTFOUND:
            // La stanza non esiste. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_USERINROOM:
            // L'utente è già connesso da un altro client. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_TURNTIMEOUT:
            // L'utente non ha inviato per tempo il segnale.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_NOTYOURTURN:
            // L'utente prova a comunicare col server quando non è il suo turno.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_SERVERERROR:
            // Il server non è attualmente disponibile. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_LOGINERROR:
            // Credenziali errate. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_SIGNINERROR:
            // Username già esistente. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_NOPERMISSION:
            // Il client non ha i permessi adeguati. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_UNKNOWNSIGNAL:
            // Il server non ha riconosciuto il messaggio in entrata. Nuovo tentativo.
            printf("\t\tSERVER_SWITCH: <ERROR> %d:%s\n", signal_num, incoming);
            writeToServer(*prompt->sd, C_RETRY, incoming);
            break;
        case S_COMMTIMOUT:
        default:
            // Il server ha effettuato una comunicazione non riconosciuta. Per tanto, il client terminerà la connessione
            // per prevenire problemi di gestione del segnale sconosciuto.
            fprintf(stderr, "SERVER_SWITCH: <ERROR> signal \"%d\" with message \"%s\" not recognized. Disconnecting in 3 seconds.\n", signal_num, incoming);
            writeToServer(*(server->sd), S_DISCONNECT, S_UNKNOWNSIGNAL_MSG);
            close(*(server->sd));
            *(server->sd) = -1;

            sleep(3);
            writeToServer(*prompt->sd, C_CONNECTION, "C_CONNECTION");
    }
    return end_loop;
}

int switchPrompt(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]) {
    int end_loop, parser_return;

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
        case C_RETRY:
            writeToServer(*server->sd, C_RETRY, "C_RETRY");
            break;
        case S_DISCONNECT:
            // Il prompt ha inviato un segnale di disconnessione dal server a cui il client è connesso in quel
            // momento. Il client provvederà a trasferirlo al server.
            printf("\t\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_CONNECTION:
            // Il prompt desidera connettersi al processo server identificato da indirizzo IPv4 e porta forniti.
            // I valori saranno esaminati perché rispettino i requisiti non funzionali. In caso di successo saranno
            // inviati al server, altrimenti saranno richiesti nuovamente.
            printf("\t\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);

            parser_return = parserIp(incoming, server);
            if(parser_return == -1) {
                red();
                printf("L'indirizzo IPv4 fornito non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("L'indirizzo IPv4 può contenere solo caratteri numerici. Riprovare.\n");
                defaultFormat();
            }
            parser_return = parserPort(incoming, server);
            if(parser_return == -1) {
                red();
                printf("La porta fornita non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("La porta può contenere solo caratteri numerici. Riprovare.\n");
                defaultFormat();
            }

            if (parser_return < 0) {
                writeToServer(*prompt->sd, C_RETRY, "C_RETRY");
                break;
            }
            else {
                if ((*(server->sd) = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                    fprintf(stderr,":SWITCH PROMPT ERROR: Apertura della server socket non riuscita correttamente. Riprovare.\n");
                    *(server->sd) = -1;
                    writeToServer(*prompt->sd, C_CONNECTION, "C_CONNECTION");
                    break;
                }
            }
            break;
        case C_LOGIN:
            printf("\t\tPROMPT_SWITCH: <Login> %d:%s\n", signal_num, incoming);
            parser_return = parserUsername(incoming, server);
            if(parser_return == -1) {
                red();
                printf("Il nome fornito non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("Il nome può contenere solo caratteri ASCII. Riprovare.\n");
                defaultFormat();
            }
            parser_return = parserPassword(incoming);
            if(parser_return == -1) {
                red();
                printf("La password fornita non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("La password può contenere solo caratteri ASCII. Riprovare.\n");
                defaultFormat();
            }

            if (parser_return < 0) {
                writeToServer(*prompt->sd, C_RETRY, "C_RETRY");
                break;
            }
            else {
                writeToServer(*server->sd, C_LOGIN, incoming);
            }
            break;
        case C_SIGNIN:
            printf("\t\tPROMPT_SWITCH: <Signin> %d:%s\n", signal_num, incoming);
            parser_return = parserUsername(incoming, server);
            if(parser_return == -1) {
                red();
                printf("Il nome fornito non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("Il nome può contenere solo caratteri ASCII. Riprovare.\n");
                defaultFormat();
            }
            parser_return = parserPassword(incoming);
            if(parser_return == -1) {
                red();
                printf("La password fornita non è della lunghezza corretta. Riprovare.\n");
                defaultFormat();
            }
            else if(parser_return == -2) {
                red();
                printf("La password può contenere solo caratteri ASCII. Riprovare.\n");
                defaultFormat();
            }

            if (parser_return < 0) {
                writeToServer(*prompt->sd, C_RETRY, "C_RETRY");
                break;
            }
            else {
                writeToServer(*server->sd, C_SIGNIN, incoming);
            }
            break;
        case C_LOGOUT:
            writeToServer(*server->sd, signal_num, incoming);
            memset(server->connected_user, '\0', sizeof(server->connected_user));
            break;
        case C_CREATEROOM:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_JOINROOM:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_LISTROOM:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_EXITROOM:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_GUESSSKIP:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        case C_CLIENTERROR:
            writeToServer(*server->sd, signal_num, incoming);
            break;
        default:
            printf("\t\tPROMPT_SWITCH: <Default> %d:%s\n", signal_num, incoming);
            writeToServer(*server->sd, signal_num, incoming);
    }
    return end_loop;
}