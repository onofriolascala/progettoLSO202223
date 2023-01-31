//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]) {
    int end_loop, same_signal, contacted_sd;
    char temp_buffer[MAXCOMMBUFFER] = "";

    if(server->last_signal != signal_num && signal_num != S_DISCONNECT && signal_num <= S_FULLROOM) {
        emptyConsole();
        server->last_signal = signal_num;
        same_signal = 0;
    }
    else {
        server->last_signal = signal_num;
        same_signal = 1;
    }

    end_loop = 0;

    switch (signal_num) {
        case -1:
            //sprintf( prompt->log_str, "\tSERVER_SWITCH: <EWOULDBLOCK> %d:%s.\n", signal_num, incoming);
            // EWOULDBLOCK ERROR
            end_loop = 0;
            break;
        case -2: case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <ERROR CASE 3>.\n");
            printErrorNoNumber(prompt, ECRITICALCLIENT, incoming);
            end_loop = 1;
            break;
        case S_DISCONNECT_ABRUPT:
            writeToLog( *prompt->log, "\tSERVER_SWITCH: <Abrupt Disconnection>.\n");
            printErrorNoNumber(prompt, "!Attenzione! Il server ha prematuramente terminato la connessione. Riavvio.\n", incoming);
            switchServer(server, room, prompt, S_DISCONNECT, "S_DISCONNECT");
            return 0;
        case S_DISCONNECT:
            // Il server comunica di aver recepito l'intento di disconnessione. Il client provvederà a chiudere
            // la connessione, e riportare la console alla schermata di connessione.
            do {
                if(pthread_mutex_trylock(&prompt->mutex) == 0) {

                    bold();
                    yellow();
                    printf("\nRiavvio in corso...\n");
                    defaultFormat();
                    fflush(stdout);

                    sprintf( prompt->log_str, "\tSERVER_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
                    writeToLog(*prompt->log, prompt->log_str);

                    close(*server->sd);
                    *server->sd = -1;

                    pthread_cancel(prompt->id);
                    close(*prompt->sd);
                    prompt->id = createPrompt(*server->localsocket, prompt);

                    sleep(3);
                    emptyConsole();
                    renderConnection();
                    pthread_mutex_unlock(&prompt->mutex);

                    if(writeToServer(*prompt->sd, C_CONNECTION, "C_CONNECTION.") < 0) printErrorNoNumber(prompt, "\tErrore in scrittura rilevato.\n", incoming);
                    return 0;
                }
                else {
                    usleep(REFRESHCONSTANT);
                }
            } while(1);
            break;
        case S_LOGINOK:
            // Il server comunica il via libera alla schermata di login. Il client provvederà ad attivare
            // il prompt associato.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Connection> %d:%s\n", signal_num, incoming);
            if( !same_signal ) renderLogin(server);
            if(strcmp(incoming, S_LOGINOK_MSG) != 0) printf("\n\t%s\n\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_LOGIN;
            strcpy(incoming, "C_LOGIN");
            // check login
            break;
        case S_HOMEPAGEOK:
            // Il server comunica il via libera alla schermata di homepage. Il client provvederà ad attivare
            // il prompt associato.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Login> %d:%s\n", signal_num, incoming);
            if( !same_signal ) renderHomepage(server);
            if(strcmp(incoming, S_HOMEPAGEOK_MSG) != 0) printf("\n\t%s\n\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_CREATEROOM;
            strcpy(incoming, "C_CREATEROOM");
            break;
        case S_ROOMLISTOK:
            // Il server comunica il via libera della stampa delle stanze esistenti sul server. Il client procede
            // ad estrarle, formattarle ed infine riattivare il prompt.
            if( !same_signal ) renderHomepage(server);
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <List> %d:%s\n", signal_num, incoming);

            do {
                signal_num = parserList(incoming, 0, temp_buffer);
                if (signal_num == 0) {
                    printf("Lista delle stanze:\n%s", temp_buffer);
                    contacted_sd = *prompt->sd;
                    signal_num = C_RETRY;
                    strcpy(incoming, "C_RETRY");
                    break;
                } else if (signal_num > 0) {
                    signal_num = parserList(incoming, signal_num, temp_buffer);
                    printf("%s", temp_buffer);
                } else {
                    sprintf(prompt->log_str, "\ttSERVER_SWITCH: <S_ROOMNOTFOUND> %d:%s\n", signal_num, incoming);
                    printWarning(prompt, "Il client ha ricevuto una lista vuota. Riprovare.\n");
                    contacted_sd = *prompt->sd;
                    signal_num = C_RETRY;
                    strcpy(incoming, "C_RETRY");
                    break;
                }
            } while(1);
            printf("\n");
            break;
        case S_ROOMOK:
            // Il server comunica l'avvenuto accesso alla stanza. Verrà eseguita la prima stampa della stanza.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Join> %d:%s\n", signal_num, incoming);
            if( !same_signal ) renderRoom(server, room);
            if(strcmp(incoming, S_ROOMOK_MSG) != 0) printf("\n\t%s\n\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            break;
        //          GAME LOGIC          //
        case S_MISSEDGUESS:
            //
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <Missed Guess> %d:%s\n", signal_num, incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_VICTORY:
            //
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <Victory> %d:%s\n", signal_num, incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_DEFEAT:
            //
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <Defeat> %d:%s\n", signal_num, incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_ENDOFTURN:
            //
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <End of Turn> %d:%s\n", signal_num, incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        //          CASI DI ERRORE          //
        case S_FULLROOM:
            // La stanza è piena. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_FULLROOM> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "La stanza è attualmente piena.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_ROOMNOTFOUND:
            // La stanza non esiste. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_ROOMNOTFOUND> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "La stanza con l'ID inserito non esiste.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_USERINROOM:
            // L'utente è già connesso da un altro client. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_USERINROOM> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "L'utente è già connesso a questa stanza. Riprovare con un utente diverso.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_TURNTIMEOUT:
            // L'utente non ha inviato per tempo il segnale.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_TURNTIMEOUT> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "Tempo scaduto. Prossimo turno.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_NOTYOURTURN:
            // L'utente prova a comunicare col server quando non è il suo turno.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_NOTYOURTURN> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "Attendere il proprio turno.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_SERVERERROR:
            // Il server non è attualmente disponibile. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_SERVERERROR> %d:%s\n", signal_num, incoming);
            printErrorNoNumber(prompt, "!Attenzione! Il server non è attualmente disponibile. Riprovare.\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_LOGINERROR:
            // Credenziali errate. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_LOGINERROR> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "Crendenziali inserite errate. Riprovare.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_SIGNINERROR:
            // Username già esistente. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_SIGNINERROR> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "!Esiste già un utente con l'username inserito. Riprovare.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_NOPERMISSION:
            // Il client non ha i permessi adeguati. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_NOPERMISSION> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "Il client non possiede i permessi adeguati. Riprovare.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_UNKNOWNSIGNAL:
            // Il server non ha riconosciuto il messaggio in entrata. Nuovo tentativo.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <S_UNKNOWNSIGNAL> %d:%s\n", signal_num, incoming);
            printErrorNoNumber(prompt, "!Attenzione! Il server non ha riconosciuto il messaggio inviato. Riprovare.\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_COMMERROR:
        case S_COMMTIMOUT:
        default:
            // Il server ha effettuato una comunicazione non riconosciuta. Per tanto, il client terminerà la connessione
            // per prevenire problemi di gestione del segnale sconosciuto.
            sprintf( prompt->log_str, "\ttSERVER_SWITCH: <Unknown Signal> %d:%s\n", signal_num, incoming);
            printErrorNoNumber(prompt, "!Attenzione! Il server ha inviato un codice di comunicazione non riconosciuto. Riavvio.\n", incoming);
            switchServer(server, room, prompt, S_DISCONNECT, "S_DISCONNECT");
            return 0;
    }
    writeToLog(*prompt->log, prompt->log_str);
    if (signal_num > 0) {
        if(writeToServer(contacted_sd, signal_num, incoming) < 0) printErrorNoNumber(prompt, "\tErrore in scrittura rilevato.\n", incoming);
    }
    return end_loop;
}

int switchPrompt(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]) {
    int end_loop, parser_return, contacted_sd;

    end_loop = 0;

    switch (signal_num) {
        case -1:
            // EWOULDBLOCK ERROR
            end_loop = 0;
            break;
        case -2:
        case -3:
            // ERROR DIFFERENT FROM EWOULDBLOCK
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <ERROR CASE 3>.\n");
            printErrorNoNumber(prompt, ECRITICALCLIENT, incoming);
            end_loop = 1;
            break;
        case C_RETRY:
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_DISCONNECT_ABRUPT:
            // La socket del prompt si è chiusa inaspettatamente. Comportamento non definito, si procede a chiudere il
            // processo.
            printErrorNoNumber(prompt, ECRITICALCLIENT, incoming);
            end_loop = 1;
            break;
        case S_DISCONNECT:
            // Il prompt ha inviato un segnale di disconnessione dal server a cui il client è connesso in quel
            // momento. Il client provvederà a trasferirlo al server.
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_CONNECTION:
            // Il prompt desidera connettersi al processo server identificato da indirizzo IPv4 e porta forniti.
            // I valori saranno esaminati perché rispettino i requisiti non funzionali. In caso di successo saranno
            // inviati al server, altrimenti saranno richiesti nuovamente.
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Connection> %d:%s\n", signal_num, incoming);
            parser_return = parserIp(incoming, server);
            if(parser_return == -1) {
                printWarning(prompt, "L'indirizzo IPv4 fornito non è della lunghezza corretta. Riprovare.\n");
            }
            else if(parser_return == -2) {
                printWarning(prompt, "L'indirizzo IPv4 può contenere solo caratteri numerici. Riprovare.\n");
            }

            if(parser_return >= 0) {
                parser_return = parserPort(incoming, server);
                if(parser_return == -1) {
                    printWarning(prompt, "La porta fornita non è della lunghezza corretta. Riprovare.\n");
                }
                else if(parser_return == -2) {
                    printWarning(prompt, "La porta può contenere solo caratteri numerici. Riprovare.\n");
                }
            }

            if (parser_return < 0) {
                contacted_sd = *prompt->sd;
                signal_num = C_RETRY;
                strcpy(incoming, "C_RETRY");
                break;
            }
            else {
                if ((signal_num = socketInit(&server->addr, &server->len, server->ip, server->port)) < 4) {
                    printError(prompt, "Errore nell'apertura della connessione con il server. Riprovare.\n",
                                       ":SWITCH PROMPT ERROR", -signal_num);
                    contacted_sd = *prompt->sd;
                    signal_num = C_CONNECTION;
                    strcpy(incoming, "C_CONNECTION");
                    break;
                }
                else {
                    *(server->sd) = signal_num;
                    signal_num = 0;
                }
            }
            break;
        case C_LOGIN:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Login> %d:%s\n", signal_num, incoming);
            parser_return = parserUsername(incoming, server);
            if(parser_return == -1) {
                printWarning(prompt, "Il nome fornito non è della lunghezza corretta. Riprovare.\n");
            }
            else if(parser_return == -2) {
                printWarning(prompt, "Il nome può contenere solo caratteri alfabetici. Riprovare.\n");
            }
            if(parser_return >= 0) {
                parser_return = parserPassword(incoming);
                if(parser_return == -1) {
                    printWarning(prompt, "La password fornita non è della lunghezza corretta. Riprovare.\n");
                }
                else if(parser_return == -2) {
                    printWarning(prompt, "La password può contenere solo caratteri ASCII. Riprovare.\n");
                }
            }

            if (parser_return < 0) {
                contacted_sd = *prompt->sd;
                signal_num = C_RETRY;
                strcpy(incoming, "C_RETRY");
                //writeToServer(*prompt->sd, C_RETRY, "C_RETRY");
                break;
            }
            else {
                contacted_sd = *server->sd;
                signal_num = C_LOGIN;
                //writeToServer(*server->sd, C_LOGIN, incoming);
            }
            break;
        case C_SIGNIN:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Signin> %d:%s\n", signal_num, incoming);
            parser_return = parserUsername(incoming, server);
            if(parser_return == -1) {
                printWarning(prompt, "Il nome fornito non è della lunghezza corretta. Riprovare.\n");
            }
            else if(parser_return == -2) {
                printWarning(prompt, "Il nome può contenere solo caratteri alfabetici. Riprovare.\n");
            }
            if(parser_return >= 0) {
                parser_return = parserPassword(incoming);
                if(parser_return == -1) {
                    printWarning(prompt, "La password fornita non è della lunghezza corretta. Riprovare.\n");
                }
                else if(parser_return == -2) {
                    printWarning(prompt, "La password può contenere solo caratteri ASCII. Riprovare.\n");
                }
            }

            if (parser_return < 0) {
                contacted_sd = *prompt->sd;
                signal_num = C_RETRY;
                strcpy(incoming, "C_RETRY");
                //writeToServer(*prompt->sd, C_RETRY, "C_RETRY");
                break;
            }
            else {
                contacted_sd = *server->sd;
                signal_num = C_SIGNIN;
                //writeToServer(*server->sd, C_SIGNIN, incoming);
            }
            break;
        case C_LOGOUT:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Logout> %d:%s\n", signal_num, incoming);
            contacted_sd = *server->sd;
            memset(server->connected_user, '\0', sizeof(server->connected_user));
            break;
        case C_CREATEROOM:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Create Room> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_JOINROOM:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Join Room> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_LISTROOM:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <List Room> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_EXITROOM:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Exit Room> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_GUESSSKIP:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Guess> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_CLIENTERROR:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Client Error> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        default:
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Default> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
    }
    writeToLog(*prompt->log, prompt->log_str);
    if (signal_num > 0) {
        if(writeToServer(contacted_sd, signal_num, incoming) < 0) printErrorNoNumber(prompt, "\tErrore in scrittura rilevato.\n", incoming);
    }
    return end_loop;
}