//
// Main del Client. Responsabile di inizializzare il processo client e di gestire le comunicazioni con il server.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <poll.h>

#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/ThreadPrompt.h"
#include "../include/ThreadRender.h"
#include "../include/PollSwitches.h"
#include "../../Server/include/Def.h"

int main() {
    int sd1, sd2, localsocket, prompt_socket, render_socket, server_port, signal_num;
    struct sockaddr_in server_addr;
    struct sockaddr_un localsocket_addr;
    socklen_t server_len;
    socklen_t local_len;
    char *saveptr, *server_p, *port_p;
    char server_ip[LOCALSOCKETADDRLENGTH], incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];

    // Inizializzazione altri thread
    signal_num = 2;
    localsocket = localSocketInit(&localsocket_addr, &local_len);

    createPrompt(localsocket, &prompt_socket);
    //createRender(localsocket, &render_socket);

    //              MAIN CLIENT LOOP                //

    struct pollfd fds[CLIENTPOLLINGCONST];
    char buffer[MAXCOMMBUFFER];
    int timeout, rc = 0;
    int num_fds, new_local_sd, usr_sd, current_size, i, j, signal_code;
    int end_loop = 0, close_conn;

    // Inizializzazioni
    current_size = 0;

    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = prompt_socket;                     // Socket riservata alla connessione col server
    fds[0].events = POLLIN;             // Inizialmente settata a -1 per essere ignorata.
    //fds[1].fd = prompt_socket;
    //fds[1].events = POLLIN;
    //fds[2].fd = render_socket;
    //fds[2].events = POLLIN;
    num_fds = 1;

    timeout = ( 3 * 60 * 1000 );

    emptyConsole();
    renderConnection();
    writeToServer(prompt_socket, 1, "Enabled");

    fcntl(prompt_socket, F_SETFL, fcntl(prompt_socket, F_GETFL, 0) | O_NONBLOCK);

    do {
        printf("MAIN: Waiting on poll function...\n");
        fflush(stdout);

        memset(incoming, '\0', sizeof(incoming));
        memset(outgoing, '\0', sizeof(outgoing));

        rc = poll(fds, num_fds, timeout);

        // Errore del poll
        if( rc < 0 ) {
            perror(":POLL ERROR");
            break;
        }
        // Timeout
        if( rc == 0 ) {
            printf("MAIN: poll timed out. Closing the logic loop\n");
            break;
        }
        for(i = 0; i < num_fds; i++){
            if(fds[i].revents == 0){
                printf("DEBUG CONTINUE \n");
                continue;
            }
            if(fds[i].revents != POLLIN){
                //unexpected behaviour
                fprintf(stderr, ":REVENTS ERROR: fds[%d].revents = %d\n", i, fds[i].revents);
                end_loop = 1;
                break;
            }
            // Socket del prompt
            if(fds[i].fd == prompt_socket) {
                signal_num = readFromServer(prompt_socket, incoming, MAXCOMMBUFFER);

                if (signal_num == 42) {
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
            }
            // Socket del giocatore
            else {
                signal_num = readFromServer(fds[1].fd, incoming, MAXCOMMBUFFER);
                printf("DEBUG SERVER %d %s\n", fds[i].fd, incoming);
            }
        }
        // restart polling unless loop ended
    } while( !end_loop );

    // rendi le socket non blocking

    //printf("Inserire l'indirizzo ip: ");
    //fgets(server_ip, MAXCOMMBUFFER, stdin);
    //server_ip[strcspn(server_ip, "\n")] = 0;

    /*printf("Inserire la porta: ");
    fgets(tempbuffer, MAXCOMMBUFFER, stdin);
    server_ip[strcspn(tempbuffer, "\n")] = 0;*/
    // Inizializzazione connessione
    /*strcpy(server_ip, "25.72.233.6");
    server_port = 5200;

    sd1 = socketInit(server_addr, server_len, server_ip, server_port);

    while(signal_num > 1 && signal_num != 50) {
        printf("\e[2J");
        printf("Opzioni disponibili:\n"
               "1  - Disconnessione\t\t10 - Login\t\t11 - Registrazione\n"
               "20 - Crea Stanza   \t\t21 - Join \t\t22 - Lista\n"
               "23 - Logout        \t\t30 - Word \t\t31 - Guess\t\t32 - ExitRoom\n");
        printf("Inserire un numero tra i precedenti: ");
        fflush(stdout);
        fgets(server_ip, MAXCOMMBUFFER, stdin);
        server_ip[strcspn(server_ip, "\n")] = 0;
        strcat(server_ip, ":test;");
        printf("%s\n", server_ip);
        write(sd1, server_ip, strlen(server_ip)+1);

        memset(server_ip,0,sizeof(server_ip));
        read(sd1, server_ip, 2);
        signal_num = atoi(server_ip);
        read(sd1, server_ip, 1);
        read(sd1, server_ip, MAXCOMMBUFFER);
        printf("MAIN: server says %d:%s\n", signal_num, server_ip);
        fflush(stdout);
    }*/

    //close(sd1);
    printf("Terminazione processo client.\n");
    return 0;
}