//
// Created by Mattia on 03/12/2022.
//

#include "../include/Prompt.h"

void* thrPrompt(void* arg) {
    int main_socket, signal_num, prompt_mode, last_mode, result;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], tempbuffer[MAXCOMMBUFFER];

    struct sockaddr_un localaddr;
    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, CLIENTLOCALSOCKET);

    struct prompt_thread *prompt;
    prompt = (struct prompt_thread*)arg;

    // Apertura socket su un indirizzo costante.
    if((main_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        printError(prompt, ECRITICALCLIENT, "PROMPT SOCKET CREATION", result);
        exit(1);
    }
    // Connessione...
    if((result = connect(main_socket, (struct sockaddr*)&localaddr, sizeof(localaddr))) < 0) {
        printError(prompt, ECRITICALCLIENT, "PROMPT SOCKET CONNECT", result);
        exit(1);
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
            case 0:
                break;
            case C_RETRY:
                prompt_mode = last_mode;
                continue;
            case C_CONNECTION:
                last_mode = prompt_mode;
                promptConnection(prompt, outgoing);
                writeToServer(main_socket, C_CONNECTION, outgoing);
                break;
            case C_LOGIN:
                last_mode = prompt_mode;
                result = promptLogin(prompt, outgoing);
                // Disconnessione
                if(result == 0) {
                    writeToServer(main_socket, S_DISCONNECT, S_DISCONNECT_MSG);
                }
                    // Login
                else if(result == 1) {
                    writeToServer(main_socket, C_LOGIN, outgoing);
                }
                    // Signin
                else if(result == 2) {
                    writeToServer(main_socket, C_SIGNIN, outgoing);
                }
                    // Errore
                else {
                    writeToServer(main_socket, C_RETRY, "C_RETRY");
                }
                break;
            case C_CREATEROOM:
                last_mode = prompt_mode;
                result = promptHomepage(prompt, outgoing);
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
            default:
                last_mode = prompt_mode;
                printf("\t\t\t\tPROMPT_THREAD: <DEBUG> signal number not recognized. Manual response enabled.\n");
                printf("\t\t\t\tSignal Number: ");
                fgets(outgoing, MAXCOMMBUFFER, stdin);
                outgoing[strcspn(outgoing, "\n")] = '\0';
                signal_num = atoi(outgoing);
                printf("\t\t\t\tMessage: ");
                fgets(outgoing, MAXCOMMBUFFER, stdin);
                outgoing[strcspn(outgoing, "\n")] = '\0';
                writeToServer(main_socket, signal_num, outgoing);
        }
        prompt_mode = readFromServer(main_socket, incoming, MAXCOMMBUFFER);
    } while (prompt_mode != 0);

    printf("\tRENDER: Prompt thread closed.\n");
    return 0;
}

pthread_t createPrompt(int localsocket, struct prompt_thread *prompt) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrPrompt, prompt)) {
        printErrorNoNumber(prompt, ECRITICALCLIENT, ":THREAD CREATION ERROR: unable to create new prompt thread. Closing socket.\n");
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    // Detatch necessario per far sì che le risorse del thread siano liberate senza un join.
    pthread_detach(tid);

    if((*prompt->sd = accept(localsocket, NULL, NULL)) < 0) {
        //perror(":ACCEPT ERROR");
        printError(prompt, ECRITICALCLIENT, ":PROMPT CREATION SOCKET ACCEPT", errno);
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    sprintf(prompt->log_str, "MAIN: Prompt thread created with prompt_socket sd:%d.\n", *prompt->sd);
    writeToLog(*prompt->log, prompt->log_str);
    memset(prompt->log_str, '\0', sizeof(prompt->log_str));
    return tid;
}

int promptConnection(struct prompt_thread *prompt, char outgoing[]) {
    char temp_buffer[MAXCOMMBUFFER] = "";
    int result;

    green();
    printf("Attivare la connessione di debug? (25.72.233.6:5200)\n");
    defaultFormat();
    inputComfirmation();
    if(promptConfirmation(prompt)) {
        printf("Connessione a "
               "25.72.233.6:5200\n");
        strcpy(outgoing, "25.72.233.6-5200;");
        return 0;
    }
    else {
        up(1);
        clearLine();
        carriageReturn();
        up(1);
        clearLine();
        carriageReturn();
        fflush(stdout);
    }

    inputAddress();
    if ((result = promptString(prompt, temp_buffer, MAXIP)) < 0) return result;

    strncat(outgoing, temp_buffer, MAXIP+1);
    strncat(outgoing, "-", 2);

    memset(temp_buffer, '\0', sizeof(temp_buffer));

    inputPort();
    if ((result = promptString(prompt, temp_buffer, MAXPORT)) < 0) return result;

    strncat(outgoing, temp_buffer, MAXPORT+1);
    strncat(outgoing, ";", 2);
    return 0;
}

int promptLogin(struct prompt_thread *prompt, char outgoing[]) {
    char temp_buffer[USERNAMELENGTH] = "";
    int result, end_loop;

    do {
        end_loop = 1;
        //
        result = promptSelection(prompt, '2');

        switch (result) {
            // Disconnessione
            case 0:
                inputComfirmation();
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
                inputComfirmation();
                if (promptConfirmation(prompt)) {
                    printf("Accesso come "
                           "\"pippopippopippo\" con password \"pippopippo\".\n");
                    strcpy(outgoing, "pippopippo-pippopippo;");
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

                inputUsername();
                if ((result = promptString(prompt, temp_buffer, USERNAMELENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
                strncat(outgoing, "-", 2);

                memset(temp_buffer, '\0', sizeof(temp_buffer));

                inputPassword();
                if ((result = promptString(prompt, temp_buffer, PASSWORDLENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
                strncat(outgoing, ";", 2);

                result = 1;
                break;
                // Signin
            case 2:
                printf("\n");
                inputUsername();
                if ((result = promptString(prompt, temp_buffer, USERNAMELENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
                strncat(outgoing, "-", 2);

                memset(temp_buffer, '\0', sizeof(temp_buffer));

                inputPassword();
                if ((result = promptString(prompt, temp_buffer, PASSWORDLENGTH)) < 0) return result;

                strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
                strncat(outgoing, ";", 2);

                result = 2;
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

        //
        result = promptSelection(prompt, '3');

        switch (result) {
            // Logout
            case 0:
                inputComfirmation();
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

                inputRoom();
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
        inputGeneric((max_select - '0') + 1);
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