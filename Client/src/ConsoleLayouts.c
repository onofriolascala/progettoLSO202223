//
// Created by Mattia on 03/12/2022.
//

#include "../include/ConsoleLayouts.h"

/*void* thrRender(void* arg) {
    int main_socket, signal_num;
    char incoming[MAXCOMMBUFFER];
    struct sockaddr_un localaddr;

    localaddr.sun_family = PF_LOCAL;
    strcpy(localaddr.sun_path, CLIENTLOCALSOCKET);

    if((main_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        perror(":RENDER SOCKET CREATION");
        exit(1);
    }
    // Connessione...
    if(connect(main_socket, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0) {
        perror(":RENDER SOCKET CONNECT");
        exit(1);
    }
    printf("\tRENDER: Render thread created with main_socket sd:%d.\n", main_socket);

    while (1) {

        memset(incoming, '\0', sizeof(incoming));
        signal_num = receiveStatus(main_socket);

        printf("\t\tRENDER: received signal %d.\n", signal_num);
        fflush(stdout);
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
                printf("\t\tRENDER: <Connessione>\n");

                emptyConsole();
                renderHomepage();

                relayStatus(main_socket, 0);
                break;
            default:
                printf("\t\t\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                //writeToServer(fds[i].fd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
        }
    }

    printf("\tRENDER: Render thread closed.\n");
    return 0;
}

pthread_t createRender(int localsocket, int *render_socket) {
    pthread_t tid;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrRender, NULL)) {
        fprintf(stderr, ":THREAD CREATION ERROR: unable to create new render thread. Closing socket.\n");
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    // Detatch necessario per far sì che le risorse del thread siano liberate senza un join.
    pthread_detach(tid);

    if((*render_socket = accept(localsocket, NULL, NULL)) < 0) {
        perror(":ACCEPT ERROR");
        deleteLocalSocket(localsocket, CLIENTLOCALSOCKET);
        exit(1);
    }

    printf("MAIN: Render thread created with render_socket sd:%d.\n", *render_socket);
    fflush(stdout);
    return tid;
}*/

void emptyConsole(void) {
    system("clear");
}

void renderConnection(void) {
    printf("\n*****\t\t\tSCHERMATA TEMPORANEA DI CONNESSIONE\t\t\t*****\n"
           "\n"
           "Inserire indirizzo ip e porta del server a cui si desidera connettersi.\n\n");
    fflush(stdout);
}

void renderLogin(void) {
    printf("\n*****\t\t\tSCHERMATA TEMPORANEA DI LOGIN\t\t\t*****\n"
           "\n"
           "Scelte disponibili:\n"
           "\t\t0 - Disconnessione dal server\n"
           "\t\t1 - Login\n"
           "\t\t2 - Signin\n\n");
    fflush(stdout);
}

void renderHomepage(void) {
    printf("\n*****\t\t\tSCHERMATA TEMPORANEA DI HOMEPAGE\t\t\t*****\n"
           "\n"
           "Scelte disponibili:\n"
           "\t\t0 - Logout\n"
           "\t\t1 - Creazione stanza\n"
           "\t\t2 - Entrata stanza\n"
           "\t\t3 - Lista stanze\n\n");
    fflush(stdout);
}