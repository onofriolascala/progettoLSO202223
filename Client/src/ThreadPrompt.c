//
// Created by Mattia on 03/12/2022.
//

#include "../include/ThreadPrompt.h"

void* thrPrompt(void* arg) {
    int main_socket, signal_num, temp_int, prompt_status, mode;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], tempbuffer[MAXCOMMBUFFER];
    struct sockaddr_un localaddr;
    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, CLIENTLOCALSOCKET);

    if((main_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        perror(":PROMPT SOCKET CREATION");
        exit(1);
    }
    // Connessione...
    if(connect(main_socket, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0) {
        perror(":PROMPT SOCKET CONNECT");
        exit(1);
    }
    //printf("\tRENDER: Prompt thread created with main_socket sd:%d.\n", main_socket);

    prompt_status = readFromServer(main_socket, incoming, MAXCOMMBUFFER);

    fcntl(main_socket, F_SETFL, fcntl(main_socket, F_GETFL, 0) | O_NONBLOCK);

    while (prompt_status) {
        prompt_status = readFromServer(main_socket, incoming, MAXCOMMBUFFER);
        memset(incoming, '\0', sizeof(incoming));

        sleep(1);

        printf("Input: ");
        fgets(outgoing, MAXCOMMBUFFER, stdin);
        outgoing[strcspn(outgoing, "\n")] = '\0';

        //sleep(10);

        //writeToServer(main_socket, 0, "ciao.");

        //mode = readFromServer(main_socket, incoming, MAXCOMMBUFFER);

        mode = 0;

        signal_num = atoi(outgoing);

        switch (signal_num) {
            case -1:
                // EWOULDBLOCK ERROR
                break;
            case -2:
                break;
            case -3:
                // ERROR DIFFERENT FROM EWOULDBLOCK
                break;
            case 0:
                //printf("\t\tPROMPT: <Connessione>\n");
                /*if ((temp_int = promptConnection(outgoing)) == 0) {
                    writeToServer(main_socket, 0, outgoing);
                } else {
                    writeToServer(main_socket, C_CLIENTERROR, outgoing);
                }*/
                break;
            case 10:
                writeToServer(main_socket, C_LOGIN, "pippo-pippo");
                break;
            case 42:
                writeToServer(main_socket, C_CONNECTION, "25.72.233.6-5200");
                break;
            default:
                //printf("\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                printf("\t\t<DEBUG>: sending directly this signal \"%d\".\n", signal_num);
                printf("Input string: ");
                fgets(outgoing, MAXCOMMBUFFER, stdin);
                outgoing[strcspn(outgoing, "\n")] = '\0';
                writeToServer(main_socket, signal_num, outgoing);
        }
    }

    printf("\tRENDER: Prompt thread closed.\n");
    return 0;
}

pthread_t createPrompt(int localsocket, int *prompt_socket) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrPrompt, NULL)) {
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

int promptConnection(char outgoing[]) {
    int return_value;
    char tempbuffer[MAXCOMMBUFFER];

    return_value = 0;

    printf("Inserire l'indirizzo ip: ");
    fgets(outgoing, MAXCOMMBUFFER, stdin);
    outgoing[strcspn(outgoing, "\n")] = '-';

    printf("Inserire la porta: ");
    fgets(tempbuffer, MAXCOMMBUFFER, stdin);
    tempbuffer[strcspn(tempbuffer, "\n")] = ';';

    strcat(outgoing, tempbuffer);
    outgoing[strcspn(outgoing, ";")+1] = '\0';

    return return_value;
}