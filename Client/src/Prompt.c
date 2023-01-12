//
// Created by Mattia on 03/12/2022.
//

#include "../include/Prompt.h"

void* thrPrompt(void* arg) {
    int main_socket, signal_num, prompt_mode, out_len, result;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], tempbuffer[MAXCOMMBUFFER];

    struct sockaddr_un localaddr;
    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, CLIENTLOCALSOCKET);

    struct prompt_thread *prompt;
    prompt = (struct prompt_thread*)arg;

    // Apertura socket su un indirizzo costante.
    if((main_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        perror(":PROMPT SOCKET CREATION");
        exit(1);
    }
    // Connessione...
    if(connect(main_socket, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0) {
        perror(":PROMPT SOCKET CONNECT");
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

        out_len = strlen(outgoing);

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
            case C_CONNECTION:
                promptConnection(prompt, outgoing);
                writeToServer(main_socket, C_CONNECTION, outgoing);
                break;
            case C_LOGIN:
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
                    writeToServer(main_socket, C_RETRY, outgoing);
                }
                break;
            default:
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

pthread_t createPrompt(int localsocket, int *prompt_socket, struct prompt_thread *prompt) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrPrompt, prompt)) {
        fprintf(stderr, ":THREAD CREATION ERROR: unable to create new prompt thread. Closing socket.\n");
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    // Detatch necessario per far sì che le risorse del thread siano liberate senza un join.
    pthread_detach(tid);

    if((*prompt_socket = accept(localsocket, NULL, NULL)) < 0) {
        perror(":ACCEPT ERROR");
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    printf("MAIN: Prompt thread created with prompt_socket sd:%d.\n", *prompt_socket);
    fflush(stdout);
    return tid;
}

int promptConnection(struct prompt_thread *prompt_line, char outgoing[]) {
    char temp_buffer[MAXCOMMBUFFER] = "";
    int result;

    green();
    printf("Attivare la connessione di debug? (25.72.233.6:5200)\n");
    defaultFormat();
    if(inputComfirmation()) {
        printf("Connessione a "
               "25.72.233.6:5200\n");
        strcpy(outgoing, "25.72.233.6-5200;");
        return 0;
    }
    else {
        printf("\n");
    }

    inputAddress();
    if ((result = promptString(prompt_line, temp_buffer, MAXIP)) < 0) return result;

    strncat(outgoing, temp_buffer, MAXIP+1);
    strncat(outgoing, "-", 2);

    memset(temp_buffer, '\0', sizeof(temp_buffer));

    inputPort();
    if ((result = promptString(prompt_line, temp_buffer, MAXPORT)) < 0) return result;

    strncat(outgoing, temp_buffer, MAXPORT+1);
    strncat(outgoing, ";", 2);
    return 0;
}

int promptLogin(struct prompt_thread *prompt_line, char outgoing[]) {
    char temp_buffer[USERNAMELENGTH];
    int result;



    //
    result = promptSelection(prompt_line, '2');

    switch(result) {
        case 0:
            result = 0;
            break;
        case 1:
            green();
            printf("Attivare l'accesso di debug?\n");
            defaultFormat();
            if (inputComfirmation()) {
                printf("Accesso come "
                       "\"pippo\" con password \"pippo\".\n");
                strcpy(outgoing, "pippo-pippo;");
                return 1;
            } else {
                printf("\n");
            }

            inputUsername();
            if ((result = promptString(prompt_line, temp_buffer, USERNAMELENGTH)) < 0) return result;

            strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
            strncat(outgoing, "-", 2);

            inputPassword();
            if ((result = promptString(prompt_line, temp_buffer, PASSWORDLENGTH)) < 0) return result;

            strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
            strncat(outgoing, ";", 2);

            result = 1;
            break;
        case 2:
            inputUsername();
            if ((result = promptString(prompt_line, temp_buffer, USERNAMELENGTH)) < 0) return result;

            strncat(outgoing, temp_buffer, USERNAMELENGTH + 1);
            strncat(outgoing, "-", 2);

            inputPassword();
            if ((result = promptString(prompt_line, temp_buffer, PASSWORDLENGTH)) < 0) return result;

            strncat(outgoing, temp_buffer, PASSWORDLENGTH + 1);
            strncat(outgoing, ";", 2);

            result = 2;
            break;
        default:
            result = -1;
    }
    return result;
}

// Riceve la struttura temporanea per il salvataggio della riga di comando, rigorosamente vuota, e restituisce per
// riferimento il messaggio finale in uscita dal prompt.
int promptString(struct prompt_thread *prompt_line, char buffer[], int max_len) {
    char temp_buffer[MAXCOMMBUFFER];

    do {
        if(pthread_mutex_trylock(&prompt_line->mutex) == 0) {
            if(!fgets(temp_buffer, sizeof temp_buffer, stdin)) {
                fprintf(stderr, ":INPUT READING ERROR: prompt has failed reading from input.\n");
                return -1;
            }
            temp_buffer[strcspn(temp_buffer, "\r\n")] = '\0';
            pthread_mutex_unlock(&prompt_line->mutex);
            break;
        }
        else {
            usleep(REFRESHCONSTANT);
        }
    } while(1);
    strncat(buffer, temp_buffer, max_len);
    return 0;
}

int promptSelection(struct prompt_thread *prompt_line, char max_select) {
    char temp_buffer[MAXCOMMBUFFER];
    char c = 3;
    do {
        memset(temp_buffer, '\0', sizeof(temp_buffer));
        inputGeneric();
        if(promptString(prompt_line, temp_buffer, USERNAMELENGTH) < 0) return -1;
        if(temp_buffer[0] == 0) {
            red();
            printf("\tInserire un valore.\n");
            defaultFormat();
            continue;
        }
        sscanf(temp_buffer, "%c", &c);
        if(!isdigit(c)) {
            red();
            printf("\tInserire un numero.\n");
            defaultFormat();
            continue;
        }
        if(c < '0' || c > max_select){
            red();
            printf("\tInserire una delle opzioni previste.\n");
            defaultFormat();
            continue;
        }
        break;
    } while(1);

    return c - '0';
}