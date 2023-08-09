
#include "../include/Prompt.h"

void* thrPrompt(void* arg) {
    int main_socket, signal_num, prompt_mode, last_mode, result, sock_status;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], tempbuffer[MAXCOMMBUFFER];

    struct prompt_thread *prompt;
    struct room_struct *room;

    prompt = (*(struct prompt_args*)arg).prompt;
    room = (*(struct prompt_args*)arg).room;

    struct sockaddr_un localaddr;
    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, prompt->localsocket_path);


    // Apertura socket su un indirizzo costante.
    if((main_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        printError(prompt, ECRITICALCLIENT, "PROMPT SOCKET CREATION", result);
        raise(SIGTERM);
    }
    // Connessione...
    if((result = connect(main_socket, (struct sockaddr*)&localaddr, sizeof(localaddr))) < 0) {
        printError(prompt, ECRITICALCLIENT, "PROMPT SOCKET CONNECT", result);
        raise(SIGTERM);
    }

    // Completamento inizializzazione lato MainThread.
    prompt_mode = readFromServer(main_socket, incoming, MAXCOMMBUFFER);
    // La socket viene impostata su non bloccante.
    fcntl(main_socket, F_SETFL, fcntl(main_socket, F_GETFL, 0) | O_NONBLOCK);

    //              MAIN PROMPT LOOP                //

    do {
        usleep(REFRESHCONSTANT);

        memset(incoming, '\0', sizeof(incoming));
        memset(outgoing, '\0', sizeof(outgoing));

        switch (prompt_mode) {
            case -1:
                // EWOULDBLOCK ERROR
                usleep(REFRESHCONSTANT);
                break;
            case -2:
                break;
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                break;
            case S_DISCONNECT_ABRUPT: case S_DISCONNECT:
                // Il MainThread ha richiesto la chiusura del prompt.
                prompt_mode = 0;
                break;
            case C_RETRY:
                prompt_mode = last_mode;
                continue;
            case C_CONNECTION:
                last_mode = prompt_mode;
                //result = promptConnection(prompt, outgoing);
                usleep(REFRESHCONSTANT);
                strcpy(outgoing, "192.168.1.139-5200;");
                writeToServer(main_socket, C_CONNECTION, outgoing);
                // Chiusura
                /*if(result == 0) {
                    signal_num = C_CLOSEAPP;
                    prompt_mode = 0;
                    continue;
                }
                    // Connessione
                else if(result == 1){
                    writeToServer(main_socket, C_CONNECTION, outgoing);
                }
                    // Errore
                else {
                    writeToServer(main_socket, C_RETRY, "C_RETRY");
                }*/
                break;
            case C_LOGIN:
                last_mode = prompt_mode;
                //result = promptLogin(prompt, outgoing);
                usleep(REFRESHCONSTANT);
                result = 1;
                strcpy(outgoing, "Utente03-password03;");
                // Disconnessione
                if(result == 0) {
                    signal_num = S_DISCONNECT;
                    prompt_mode = 0;
                    continue;
                }
                    // Login
                else if(result == 1) {
                    writeToServer(main_socket, C_LOGIN, outgoing);
                }
                    // Signin
                else if(result == 2) {
                    writeToServer(main_socket, C_SIGNIN, outgoing);
                }
                    // Exit from login
                else if(result == 3){
                    signal_num = C_CLOSEAPP;
                    prompt_mode = 0;
                    continue;
                }
                    // Errore
                else {
                    writeToServer(main_socket, C_RETRY, "C_RETRY");
                }
                break;
            case C_CREATEROOM:
                last_mode = prompt_mode;
                //result = promptHomepage(prompt, outgoing);
                usleep(REFRESHCONSTANT);
                result = 1;
                sprintf(outgoing, "%d;", 1);
                // Logout
                if(result == 0) {
                    writeToServer(main_socket, C_LOGOUT, "C_LOGOUT");
                }
                // Create Room
                else if(result == 1) {
                    writeToServer(main_socket, C_CREATEROOM, "C_CREATEROOM");
                }
                // Join Room
                else if(result == 2) {
                    writeToServer(main_socket, C_JOINROOM, outgoing);
                }
                // List Rooms
                else if(result == 3) {
                    writeToServer(main_socket, C_LISTROOM, "C_LISTROOM");
                }
                // Errore
                else {
                    writeToServer(main_socket, C_RETRY, "C_RETRY");
                }
                break;
            case C_GUESSSKIP: case C_SELECTWORD:
                // E' necessario rendere la socket non bloccante per poterla svuotare al momento dell'uscita in caso
                // di messaggi svuggiti al controllo.
                last_mode = prompt_mode;
                //result = promptRoom(prompt, room, outgoing);
                // EXIT
                /*if(result == 0) {
                    sock_status = O_NONBLOCK;
                    fcntl(main_socket, F_SETFL, sock_status);
                    do {
                        result = readFromServer(main_socket, incoming, MAXCOMMBUFFER);
                    } while ( result > 0 && result != 99);
                    sock_status &= ~O_NONBLOCK;
                    fcntl(main_socket, F_SETFL, sock_status);
                    writeToServer(main_socket, C_EXITROOM, "C_EXITROOM");
                }
                    // GUESSSKIP
                else if(result == 1) {
                    writeToServer(main_socket, C_GUESSSKIP, outgoing);
                    continue;
                }
                    // SELECTWORD
                else if(result == 2) {
                    writeToServer(main_socket, C_SELECTWORD, outgoing);
                    continue;
                }
                    // Errore
                else {
                    writeToServer(main_socket, C_RETRY, "C_RETRY");
                }*/
                break;
            default:
                last_mode = prompt_mode;
                printf("<DEBUG> segnale di comunicazione non riconosciuto. Formulazione manuale del messaggio attivata.\n");
                printf("Signal Number: ");
                fgets(outgoing, MAXCOMMBUFFER, stdin);
                outgoing[strcspn(outgoing, "\n")] = '\0';
                signal_num = atoi(outgoing);
                printf("Message: ");
                fgets(outgoing, MAXCOMMBUFFER, stdin);
                outgoing[strcspn(outgoing, "\n")] = '\0';
                writeToServer(main_socket, signal_num, outgoing);
        }
        prompt_mode = readFromServer(main_socket, incoming, MAXCOMMBUFFER);
    } while (prompt_mode != 0);

    writeToServer(main_socket, signal_num, "Closing prompt_thread for app restart or closure.");

    usleep(REFRESHCONSTANT*6);

    close(main_socket);

    sprintf( prompt->log_str, "\tPROMPT: Prompt thread has closed.\n");
    writeToLog(*prompt->log, prompt->log_str);
    //printf("\tRENDER: Prompt thread closed.\n");
    return 0;
}

pthread_t createPrompt(int localsocket, struct prompt_thread *prompt, struct room_struct *room) {
    pthread_t tid;

    struct prompt_args arg;
    arg.prompt = prompt;
    arg.room = room;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrPrompt, &arg)) {
        printErrorNoNumber(prompt, ECRITICALCLIENT, ":THREAD CREATION ERROR: unable to create new prompt thread. Closing socket.\n");
        deleteLocalSocket(prompt);
        raise(SIGTERM);
    }

    // Detatch necessario per far sì che le risorse del thread siano liberate senza un join.
    pthread_detach(tid);

    if((*prompt->sd = accept(localsocket, NULL, NULL)) < 0) {
        //perror(":ACCEPT ERROR");
        printError(prompt, ECRITICALCLIENT, ":PROMPT CREATION SOCKET ACCEPT", errno);
        deleteLocalSocket(prompt);
        raise(SIGTERM);
    }

    sprintf(prompt->log_str, "MAIN: Prompt thread created with prompt_socket sd:%d.\n", *prompt->sd);
    writeToLog(*prompt->log, prompt->log_str);
    memset(prompt->log_str, '\0', sizeof(prompt->log_str));
    return tid;
}

int promptConnection(struct prompt_thread *prompt, char outgoing[]) {
    char temp_buffer[MAXCOMMBUFFER] = "";
    int result, end_loop;
    do{
        end_loop = 1;

        usleep(REFRESHCONSTANT);

        result = promptSelection(prompt, '2');

        usleep(REFRESHCONSTANT);
        switch (result) {
            // App closure
            case 0:
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    result = 0;
                } else {
                    end_loop = 0;
                    up(1);
                    clearLine();
                    carriageReturn();
                    up(1);
                    clearLine();
                    carriageReturn();
                    fflush(stdout);
                }
                break;
                // Autoconnection
            case 1:
                //green();
                //printf("Attivare la connessione di debug? (79.19.140.52:5200)\n");
                //defaultFormat();
                //promptConfirmationMSG();
                //if (promptConfirmation(prompt)) {
                printf("Connessione a "
                       "79.19.140.52:5200\n");
                strcpy(outgoing, "79.19.140.52-5200;");
                result = 1;
                break;
                //}
                // Manual Connection
            case 2:
                up(1);
                clearLine();
                carriageReturn();
                fflush(stdout);

                usleep(REFRESHCONSTANT);

                promptIPAddressMSG();
                if ((result = promptString(prompt, temp_buffer, MAXIP)) < 0) return result;

                usleep(REFRESHCONSTANT);

                strncat(outgoing, temp_buffer, MAXIP+1);
                strncat(outgoing, "-", 2);

                memset(temp_buffer, '\0', sizeof(temp_buffer));

                promptPortMSG();
                if ((result = promptString(prompt, temp_buffer, MAXPORT)) < 0) return result;

                usleep(REFRESHCONSTANT);

                strncat(outgoing, temp_buffer, MAXPORT+1);
                strncat(outgoing, ";", 2);

                result = 1;
                break;
            default:
                result = -1;
        }
    } while( !end_loop );

    return result;
}

int promptLogin(struct prompt_thread *prompt, char outgoing[]) {
    char temp_buffer[USERNAMELENGTH] = "";
    int result, end_loop;

    do {
        end_loop = 1;

        usleep(REFRESHCONSTANT);

        result = promptSelection(prompt, '3');

        usleep(REFRESHCONSTANT);

        switch (result) {
            // Disconnessione
            case 0:
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    result = 0;
                } else {
                    end_loop = 0;
                    up(1);
                    clearLine();
                    carriageReturn();
                    up(1);
                    clearLine();
                    carriageReturn();
                    fflush(stdout);
                }
                break;
                // Login
            case 1:
                green();
                printf("Attivare l'accesso di debug?\n");
                defaultFormat();
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    printf("Accesso come "
                           "\"Utente01\" con password \"password01\".\n");
                    strcpy(outgoing, "Utente01-password01;");
                    return 1;
                } else {
                    up(1);
                    clearLine();
                    carriageReturn();
                    up(1);
                    clearLine();
                    carriageReturn();
                    fflush(stdout);
                }

                promptUsernameMSG();
                if ((result = promptString(prompt, temp_buffer, USERNAMELENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
                strncat(outgoing, "-", 2);

                memset(temp_buffer, '\0', sizeof(temp_buffer));

                promptPasswordMSG();
                if ((result = promptString(prompt, temp_buffer, PASSWORDLENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
                strncat(outgoing, ";", 2);

                result = 1;
                break;
                // Signin
            case 2:
                printf("\n");
                promptUsernameMSG();
                if ((result = promptString(prompt, temp_buffer, USERNAMELENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
                strncat(outgoing, "-", 2);

                memset(temp_buffer, '\0', sizeof(temp_buffer));

                promptPasswordMSG();
                if ((result = promptString(prompt, temp_buffer, PASSWORDLENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
                strncat(outgoing, ";", 2);

                result = 2;
                break;
                // Exit from login
            case 3:
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    result = 3;
                } else {
                    up(1);
                    clearLine();
                    carriageReturn();
                    up(1);
                    clearLine();
                    carriageReturn();
                    fflush(stdout);
                }
                break;
            default:
                result = -1;
        }
    } while( !end_loop );
    return result;
}

int promptHomepage(struct prompt_thread *prompt, char outgoing[]) {
    char temp_buffer[USERNAMELENGTH];
    char *endp;
    int result, end_loop, n;

    do {
        end_loop = 1;

        usleep(REFRESHCONSTANT);

        result = promptSelection(prompt, '3');

        usleep(REFRESHCONSTANT);

        switch (result) {
            // Logout
            case 0:
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    result = 0;
                } else {
                    end_loop = 0;
                    up(1);
                    clearLine();
                    carriageReturn();
                }
                break;
                // Create Room
            case 1:
                printf("\n");

                result = 1;
                break;
                // Join room
            case 2:
                printf("\n");

                promptRoomIDMSG();
                result = promptInteger(prompt);
                if (result == 0) {
                    end_loop = 0;
                }
                else if (result > 0) {
                    sprintf(outgoing, "%d;", result);
                    result = 2;
                }
                break;
                // Room List
            case 3:
                printf("\n");

                result = 3;
                break;
            default:
                result = -1;
        }

    } while ( !end_loop );
    return result;
}

// Restituisce 0 in caso di C_EXIT, 1 in caso di C_TRYGUESS, 2 in caso di C_CHOOSEWORD
int promptRoom(struct prompt_thread *prompt, struct room_struct *room, char outgoing[]) {
    char temp_buffer[USERNAMELENGTH];
    char *endp;
    int result, end_loop;

    do {
        end_loop = 1;

        if(room->turn_flag == 2) {
            prePromptChooseWord();
        }
        else if (room->turn_flag == 1){
            prePromptTryGuess();
        }
        else {
            prePromptExit();
        }

        result = promptExitKey(prompt, room, temp_buffer);

        if(room->turn_flag == 2) {
            prePromptChooseWord();
        }
        else if (room->turn_flag == 1){
            prePromptTryGuess();
        }
        else {
            prePromptExit();
        }

        gotoxyCursor(V_OFFSET_PROMPT+1, 0);
        clearLine();
        resetCursor();

        switch (result) {
            case -1:
                // Wrong key
                end_loop = 0;
                break;
            case 0:
                // Exit Room
                gotoxyCursor(V_OFFSET_PROMPT, 0);
                clearLine();
                carriageReturn();
                printf(" ");
                promptConfirmationMSG();
                if (promptConfirmation(prompt)) {
                    result = 0;
                    end_loop = 1;
                    continue;
                } else {
                    end_loop = 0;
                    resetCursor();
                }
                break;
            case 1:
                // TryGuess mode
                strncpy(outgoing, temp_buffer, MAXWORDLENGTH + 1);
                prePromptExit();
                result = 1;
                break;
            case 2:
                // Choose Word Mode
                if(temp_buffer[0] == 0) {
                    gotoxyCursor(V_OFFSET_PROMPT+1, 0);
                    clearLine();
                    carriageReturn();
                    printWarning(prompt, " Inserire un valore.");
                    end_loop = 0;
                    resetCursor();
                }
                else {
                    result = strtol(temp_buffer, &endp, 10);
                    if (temp_buffer == endp || *endp != '\0') {
                        gotoxyCursor(V_OFFSET_PROMPT+1, 0);
                        clearLine();
                        carriageReturn();
                        printWarning(prompt, " Inserire un valore numerico.");
                        end_loop = 0;
                    } else {
                        strncpy(outgoing, temp_buffer, MAXWORDLENGTH + 1);
                        result = 2;
                        prePromptExit();
                    }
                    resetCursor();
                }
                break;
            default:
                end_loop = 0;
                break;
        }
        memset(temp_buffer, '\0', sizeof(temp_buffer));
    } while ( !end_loop );
    return result;
}

// Principale funzione di ricezione della stringa. Tramite un mutex si assicura che non venga attivata la ricezione di
// una stringa durante le altre operazioni di stampa. Riceve la struttura del prompt, un buffer in uscita ed una
// lunghezza massima per la stringa da recepire (con massimo comunque MAXCOMMBUFFER).
// Restituisce 0 in caso di successo, -1 in caso di errore di ricezione dell'input.
int promptString(struct prompt_thread *prompt, char buffer[], int max_len) {
    char temp_buffer[MAXCOMMBUFFER];

    do {
        if(pthread_mutex_trylock(&prompt->mutex) == 0) {
            //pthread_mutex_lock(&prompt->mutex);
            if(!fgets(temp_buffer, sizeof temp_buffer, stdin)) {
                printErrorNoNumber(prompt, "Errore durante la ricezione dell'input. Riprovare.", ":INPUT READING ERROR: prompt has failed reading from input.\n");
                pthread_mutex_unlock(&prompt->mutex);
                return -1;
            }
            temp_buffer[strcspn(temp_buffer, "\r\n")] = '\0';
            pthread_mutex_unlock(&prompt->mutex);
            break;
        }
        else {
            usleep(REFRESHCONSTANT);
        }
    } while(1);
    strncat(buffer, temp_buffer, max_len);
    usleep(REFRESHCONSTANT);
    return 0;
}

// Funzione di richiesta ed analisi di un intero tra 0 ed un massimo in entrata. Restituisce l'intero dopo
// avergli rimosso il char offset o -1 in caso di errore di ricezione dell'input.
int promptSelection(struct prompt_thread *prompt, char max_select) {
    char temp_buffer[MAXCOMMBUFFER];
    char c = 3;
    do {
        memset(temp_buffer, '\0', sizeof(temp_buffer));
        promptGenericChoiceMSG((max_select - '0') + 1);
        if(promptString(prompt, temp_buffer, USERNAMELENGTH) < 0) return -1;
        if(temp_buffer[0] == 0) {
            printWarning(prompt, "Inserire un valore.\n");
            continue;
        }
        sscanf(temp_buffer, "%c", &c);
        if(!isdigit(c)) {
            printWarning(prompt, "Inserire un valore numerico.\n");
            continue;
        }
        if(c < '0' || c > max_select){

            printWarning(prompt, "Inserire una delle opzioni previste.\n");
            continue;
        }
        break;
    } while(1);

    return c - '0';
}

// Funzione di richiesta ed analisi di un intero. Restituisce l'intero in caso di successo, 0 in caso di input
// non corretto, -1 in caso di errore di ricezione dell'input.
int promptInteger(struct prompt_thread *prompt) {
    char temp_buffer[MAXCOMMBUFFER];
    char *endp;
    int result;

    memset(temp_buffer, '\0', sizeof(temp_buffer));
    if(promptString(prompt, temp_buffer, MAXCOMMBUFFER) < 0) return -1;
    if(temp_buffer[0] == 0) {
        printWarning(prompt, "Inserire un valore.\n");
        return 0;
    }
    result = strtol(temp_buffer, &endp, 10);
    if(temp_buffer == endp || *endp != '\0') {
        printWarning(prompt, "Inserire un valore numerico.\n");
        return 0;
    }
    return result;
}

// La funzione attende una conferma Y/y da parte dell'utente. Restituisce -1 in caso di errore,
// 1 in caso di conferma, 0 altrimenti.
int promptConfirmation(struct prompt_thread *prompt) {
    char temp_buffer[MAXCOMMBUFFER];
    int result;

    result = 0;

    memset(temp_buffer, '\0', sizeof(temp_buffer));
    if(promptString(prompt, temp_buffer, MAXCOMMBUFFER) < 0) return result;
    if(temp_buffer[0] == 'Y' || temp_buffer[0] == 'y') {
        result = 1;
    }

    return result;
}

// La funzione resta in attesa o della hotkey d'uscita o, se la flag del turno è attiva, la
// conferma richiesta all'attivazione del prompt.
// Restituisce -1 se la stringa non corrisponde alla chiave d'uscita, 0 se corrisponde,
// 1 se non corrisponde ma è il turno dell'utente, se non corrisponde ma l'utente deve scegliere una parola
int promptExitKey(struct prompt_thread *prompt, struct room_struct *room, char *buffer) {
    char temp_buffer[MAXCOMMBUFFER];
    int result;

    result = -1;

    memset(temp_buffer, '\0', sizeof(temp_buffer));
    if(promptString(prompt, temp_buffer, MAXCOMMBUFFER) < 0) return result;
    if (temp_buffer[0] == EXITKEY && temp_buffer[1] == '\0') {
        result = 0;
    }
    else if (room->turn_flag == 1) {
        strncpy(buffer, temp_buffer, MAXWORDLENGTH-1);
        result = 1;
        if((strcmp(buffer, "")) == 0) {
            strcpy(buffer, "User has passed.");
        }
    }
    else if (room->turn_flag == 2) {
        strncpy(buffer, temp_buffer, MAXWORDLENGTH-1);
        result = 2;
    }

    return result;
}

