//
// Created by osboxes on 12/23/22.
//

#include "../include/PollSwitches.h"

int switchServer(struct server_connection *server, struct room_struct *room, struct prompt_thread *prompt, int signal_num, char incoming[]) {
    int end_loop, same_signal, contacted_sd, counter;
    char temp_buffer[MAXCOMMBUFFER*2];

    memset(temp_buffer, '\0', sizeof(temp_buffer));

    if(server->last_signal != signal_num && signal_num != S_DISCONNECT && signal_num <= S_ROOMOK) {
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
            printErrorNoNumber(prompt, "!Attenzione! Il server ha terminato prematuramente la connessione. Riavvio.\n", incoming);
            switchServer(server, room, prompt, S_DISCONNECT, "S_DISCONNECT");
            return 0;
        case S_DISCONNECT:
            // Il server comunica di aver recepito l'intento di disconnessione. Il client provvederà a chiudere
            // la connessione, e riportare la console alla schermata di connessione.
            do {
                if(pthread_mutex_trylock(&prompt->mutex) == 0) {

                    if(writeToServer(*prompt->sd, S_DISCONNECT_ABRUPT, "S_DISCONNECT_ABRUPT") < 0) printErrorNoNumber(prompt, "\tErrore in scrittura rilevato.\n", incoming);
                    usleep(REFRESHCONSTANT);

                    pthread_cancel(prompt->id);

                    bold();
                    yellow();
                    printf("\nRiavvio in corso...\n");
                    defaultFormat();
                    fflush(stdout);

                    sprintf( prompt->log_str, "\tSERVER_SWITCH: <Disconnection> %d:%s\n", signal_num, incoming);
                    writeToLog(*prompt->log, prompt->log_str);

                    close(*server->sd);
                    *server->sd = -1;


                    //deleteLocalSocket(prompt);
                    close(*prompt->sd);

                    prompt->id = createPrompt(*server->localsocket, prompt, NULL);

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
            if( !same_signal ) {
                renderHomepage(server);
                printf("Lista delle stanze:\n");
            }
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <List> %d:%s\n", signal_num, incoming);

            signal_num = parserList(incoming, temp_buffer);

            if (signal_num == 0) {
                printf("%s\n", temp_buffer);
                contacted_sd = *prompt->sd;
                signal_num = C_RETRY;
                strcpy(incoming, "C_RETRY");
                break;
            } else if (signal_num > 0) {
                printf("%s\n", temp_buffer);
                printf("In attesa della pagina successiva ...");
                signal_num = C_PAUSE;
            } else {
                sprintf(prompt->log_str, "\ttSERVER_SWITCH: <List_Incomplete> %d:%s\n", signal_num, incoming);
                printWarning(prompt, "Il client ha ricevuto una lista vuota. Riprovare.\n");
                contacted_sd = *prompt->sd;
                signal_num = C_RETRY;
                strcpy(incoming, "C_RETRY");
                break;
            }
            printf("\n");
            break;
        case S_ROOMOK:
            // Il server comunica l'avvenuto accesso alla stanza. Verrà eseguita la prima stampa della stanza.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Join> %d:%s\n", signal_num, incoming);

            parserRoomJoin(room, incoming);

            renderRoom(server, room);

            updateSuzerain(room);
            updatePlayerNumber(room);
            updateWord(room);
            for(int i = 0; i < MAXPLAYERS; i++)
            {
                updatePlayer(room, i);
            }

            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            break;
        //          GAME LOGIC          //
        case S_MISSEDGUESS:
            // Il server fa eco del tentativo di un giocatore
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Missed Guess> %d:%s\n", signal_num, incoming);

            sprintf(temp_buffer, " > " BLD WHT "%s" DFT "  ->  " RED "SBAGLIATO" DFT, incoming);
            addMessage(room, temp_buffer);
            slideMessages(room);

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            break;
        case S_VICTORY:
            // Il server annuncia la vittoria di questo client sugli altri. Si prepara alla ricezione delle
            // parole tra cui il nuovo Suzerain dovrà scegliere.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Victory> %d:%s\n", signal_num, incoming);

            sprintf(temp_buffer, " > " BLD WHT "%s" DFT "  ->  " GRN "CORRETTO" DFT, incoming);
            addMessage(room, temp_buffer);
            slideMessages(room);
            updateVictory();

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            break;
        case S_DEFEAT:
            // Il server annuncia la sconfitta di questo client. Si prepara alla ricezione della nuova
            // parola e all'eventuale aggiornamento della schermata.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Defeat> %d:%s\n", signal_num, incoming);

            sprintf(temp_buffer, " > " BLD WHT "%s" DFT "  ->  " GRN "CORRETTO" DFT, incoming);
            addMessage(room, temp_buffer);
            slideMessages(room);
            updateDefeat();

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            break;
        case S_ENDOFTURN:
            // Il server ribadisce al client che il suo turno è terminato.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <End of Turn> %d:%s\n", signal_num, incoming);

            prePromptExit();

            room->turn_flag = 0;

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            break;
        case S_YOURTURN:
            // Il server comunica di star aspettando un tentativo da parte del client. Il prompt verrà
            // aggiornato di conseguenza.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Your Turn> %d:%s\n", signal_num, incoming);

            room->turn_flag = 1;

            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_CHOOSEWORD:
            // Il server chiede al client di restituirgli il numero di una delle parole indicate.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <Choose Word> %d:%s\n", signal_num, incoming);

            strncpy(room->secret_word, incoming, MAXWORDLENGTH*3);

            prePromptChooseWord();
            updateWord(room);

            room->turn_flag = 2;

            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_NEWGAME:
            // Il server comunica l'avvio di una nuova partita.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <NewGame> %d:%s\n", signal_num, incoming);

            parserRoomJoin(room, incoming);

            saveCursor();
            renderRoom(server, room);
            loadCursor();

            sprintf(temp_buffer, " > Inizio Nuova Partita.");
            addMessage(room, temp_buffer);
            slideMessages(room);

            prePromptExit();

            contacted_sd = *prompt->sd;
            signal_num = C_GUESSSKIP;
            strcpy(incoming, "C_GUESSSKIP");
            break;
        case S_PLAYERUPDATE:
            // Il server comunica un aggiornamento relativo ai giocatori in partita
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <PlayerUpdate> %d:%s\n", signal_num, incoming);

            counter = parserRoomJoin(room, incoming);

            updatePlayerNumber(room);
            for(int i = 0; i < MAXPLAYERS; i++)
            {
                updatePlayer(room, i);
            }

            if (counter < 0) {
                sprintf(temp_buffer, " > Un giocatore ha lasciato la partita.");
            }
            else if (counter > 0) {
                sprintf(temp_buffer, " > Un giocatore si è unito alla partita.");
            }
            else {
                sprintf(temp_buffer, " > I giocatori sono stati aggiornati.");
            }
            addMessage(room, temp_buffer);
            slideMessages(room);

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;

            break;
        case S_NEWHINT:
            // Il server chiede al client di restituirgli il numero di una delle parole indicate.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <NewHint> %d:%s\n", signal_num, incoming);

            if(incoming != NULL) {
                strncpy(room->secret_word, incoming, sizeof(room->secret_word));
                room->secret_word[(MAXWORDLENGTH*3)-1] = '\0';
                updateWord(room);
            }
            sprintf(temp_buffer, GRN " > Giro concluso. Nuova lettera rivelata." DFT);
            addMessage(room, temp_buffer);
            slideMessages(room);

            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            break;
        //          CASI DI ERRORE          //
        case S_FULLROOM:
            // La stanza è piena. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_FULLROOM> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "La stanza è attualmente piena.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_ROOMNOTFOUND:
            // La stanza non esiste. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_ROOMNOTFOUND> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "La stanza con l'ID inserito non esiste.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_USERINROOM:
            // L'utente è già connesso da un altro client. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_USERINROOM> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "L'utente è già connesso a questa stanza. Riprovare con un utente diverso.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_TURNTIMEOUT:
            // L'utente non ha inviato per tempo il segnale.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_TURNTIMEOUT> %d:%s\n", signal_num, incoming);
            saveCursor();
            gotoxyCursor(V_OFFSET_PROMPT+1, 0);
            printWarning(prompt, " Tempo scaduto. Prossimo turno.\n");
            loadCursor();
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_NOTYOURTURN:
            // L'utente prova a comunicare col server quando non è il suo turno.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_NOTYOURTURN> %d:%s\n", signal_num, incoming);
            saveCursor();
            gotoxyCursor(V_OFFSET_PROMPT+1, 0);
            printWarning(prompt, "Attendere il proprio turno.\n");
            loadCursor();
            contacted_sd = *prompt->sd;
            signal_num = C_PAUSE;
            strcpy(incoming, "C_PAUSE");
            break;
        case S_SERVERERROR:
            // Il server non è attualmente disponibile. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_SERVERERROR> %d:%s\n", signal_num, incoming);
            printErrorNoNumber(prompt, "Il server non è attualmente disponibile. Riprovare a breve.\n", incoming);
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_LOGINERROR:
            // Credenziali errate. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_LOGINERROR> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "Crendenziali inserite errate. Riprovare.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_SIGNINERROR:
            // Username già esistente. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_SIGNINERROR> %d:%s\n", signal_num, incoming);
            printWarning(prompt, "!Esiste già un utente con l'username inserito. Riprovare.\n");
            contacted_sd = *prompt->sd;
            signal_num = C_RETRY;
            strcpy(incoming, "C_RETRY");
            break;
        case S_NOPERMISSION:
            // Il client non ha i permessi adeguati. Nuovo tentativo.
            sprintf( prompt->log_str, "\tSERVER_SWITCH: <S_NOPERMISSION> %d:%s\n", signal_num, incoming);
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
    if (signal_num > 0 && signal_num != C_PAUSE) {
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
            // Il prompt comunica l'intenzione di voler uscire dalla stanza
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Exit Room> %d:%s.\n", signal_num, incoming);
            room->turn_flag = -1;
            emptyMessageList(room);
            contacted_sd = *server->sd;
            break;
        case C_GUESSSKIP:
            // Il prompt comunica il tentativo dell'utente
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Guess> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            room->turn_flag = 0;
            break;
        case C_ENDOFTURN:
            // Il prompt comunica il tentativo dell'utente
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Guess> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_YOURTURN:
            // Il prompt comunica il tentativo dell'utente
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Guess> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            break;
        case C_SELECTWORD:
            // Il prompt comunica il tentativo dell'utente
            sprintf( prompt->log_str, "\tPROMPT_SWITCH: <Chose Word> %d:%s.\n", signal_num, incoming);
            contacted_sd = *server->sd;
            room->turn_flag = 0;
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

void addMessage(struct room_struct *room, char *incoming) {
    if (incoming == NULL) {
        return;
    }
    char temp_buff[MAXCOMMBUFFER*2];

    memset(temp_buff, '\0', sizeof(temp_buff));
    memset(room->saved_messages[MAXSAVEDMESSAGES-1], '\0', sizeof(room->saved_messages[MAXSAVEDMESSAGES-1]));

    for(int i = MAXSAVEDMESSAGES-1; i > 0; i--) {
        strcpy(room->saved_messages[i], room->saved_messages[i - 1]);
    }
    strcpy(room->saved_messages[0], incoming);
    room->saved_messages[0][MAXCOMMBUFFER] = '\0';
}
void emptyMessageList(struct room_struct *room) {
    int string_size = sizeof(room->saved_messages[0]);
    for(int i = 0; i < MAXSAVEDMESSAGES; i++) {
        memset(room->saved_messages[i], '\0', string_size);
    }
    clearMessages(NULL);
}
void changePlayers(struct room_struct *room) {
    if (room == NULL) {
        return;
    }
    for(int i = 0; i < MAXPLAYERS; i++)
    {
        updatePlayer(room, i);
    }
}
