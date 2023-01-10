//
// Created by Mattia on 03/12/2022.
//

#include "../include/ThreadPrompt.h"

#define MAXIPBYTES 16
#define MAXPORTBYTES 6

void* thrPrompt(void* arg) {
    int main_socket, signal_num, prompt_mode, out_len;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], tempbuffer[MAXCOMMBUFFER];

    struct sockaddr_un localaddr;
    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, CLIENTLOCALSOCKET);

    struct current_line *prompt_line;
    prompt_line = (struct current_line*)arg;

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
        usleep(5000);

        memset(incoming, '\0', sizeof(incoming));
        memset(outgoing, '\0', sizeof(incoming));

        out_len = strlen(outgoing);

        switch (prompt_mode) {
            case -1:
                // EWOULDBLOCK ERROR
                sleep(3);
                break;
            case -2:
                break;
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                break;
            case 0:
                break;
            case C_CONNECTION:
                promptConnection(prompt_line, outgoing);
                writeToServer(main_socket, C_CONNECTION, outgoing);
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

pthread_t createPrompt(int localsocket, int *prompt_socket, struct current_line *prompt_line) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrPrompt, prompt_line)) {
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

void promptConnection(struct current_line *prompt_line, char outgoing[]) {
    char temp_buffer[MAXCOMMBUFFER];

    renderConnection();

    memset(prompt_line->input, '\0', sizeof(prompt_line->input));

    strcpy(prompt_line->input, "Attivare modalità di debug? (Y/y) ");
    printf("%s", prompt_line->input);
    fflush(stdout);
    promptChar(prompt_line, temp_buffer, 2);

    if((temp_buffer[0] == 'Y') || (temp_buffer[0] == 'y')) {
        printf("!ATTENZIONE! Connessione automatica avviata. Connessione a "
               "25.72.233.6:5200\n");
        strcpy(outgoing, "25.72.233.6-5200");
        return;
    }
    else {
        printf("!ATTENZIONE! Connessione automatica disattivata.\n\n");
    }


    strcpy(prompt_line->input, "Inserire l'indirizzo IPv4: ");
    printf("%s", prompt_line->input);
    fflush(stdout);
    promptChar(prompt_line, temp_buffer, MAXIPBYTES);

    strcat(outgoing, temp_buffer);
    strcat(outgoing, "-");

    strcpy(prompt_line->input, "Inserire la porta: ");
    printf("%s", prompt_line->input);
    fflush(stdout);
    promptChar(prompt_line, temp_buffer, MAXPORTBYTES);

    strcat(outgoing, temp_buffer);
    outgoing[strcspn(outgoing, ";")+1] = '\0';
}

// Riceve la struttura temporanea per il salvataggio della riga di comando, rigorosamente vuota, e restituisce per
// riferimento il messaggio finale in uscita dal prompt.
void promptChar(struct current_line *prompt_line, char outgoing[], int max_len) {
    int line_len, c;

    if(max_len <= 0) {
        fprintf(stderr, "\t\tPROMPTCHAR: max_len cannot be less or equal to zero.\n");
        return;
    }

    memset(prompt_line->line, '\0', sizeof(prompt_line->line));
    line_len = 0;

    do {
        if(pthread_mutex_trylock(&prompt_line->mutex) == 0) {
            c = getchar();
            if (c == '\n') {
                prompt_line->line[line_len++] = '\0';
                strcpy(outgoing, prompt_line->line);
                memset(prompt_line->input, '\0', sizeof(prompt_line->input));
                memset(prompt_line->line, '\0', sizeof(prompt_line->line));
                pthread_mutex_unlock(&prompt_line->mutex);
                break;
            }
            if (line_len < max_len - 1) {
                prompt_line->line[line_len++] = c;
            }
            else {
                prompt_line->line[max_len - 1] = '\0';
            }
            pthread_mutex_unlock(&prompt_line->mutex);
        }
        else {
            usleep(5000);
        }
    } while(1);
}