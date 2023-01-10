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
#include "../include/PollSwitches.h"
#include "../../Server/include/Def.h"

int main() {
    // Dichiarazioni per la connessione
    int localsocket, prompt_socket;
    struct sockaddr_un localsocket_addr;
    socklen_t local_len;

    // Dichiarazioni per le comunicazioni.
    int signal_num;
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];

    // Dichiarazioni per la poll
    struct pollfd fds[CLIENTPOLLINGCONST];
    char buffer[MAXCOMMBUFFER];
    int timeout, rc = 0;
    int num_fds, new_local_sd, usr_sd, current_size, i, j, signal_code;
    int end_loop, close_conn;

    // Dichiarazione della struttura che conterrà le informazioni relative alla connessione.
    struct server_connection server;

    // Inizializzazione della current line e del suo mutex
    struct current_line *prompt_line;
    if((prompt_line = (struct current_line*)malloc(sizeof(struct current_line))) == NULL){
        //gestire errore malloc
        fprintf(stderr, ":CURRENT LINE STRUCT MALLOC: prompt_line was initialized to NULL.");
        exit(1);
    }
    pthread_mutex_init(&prompt_line->mutex, NULL);

    // Inizializzazione della socket locale e del thread per il PROMPT
    localsocket = localSocketInit(&localsocket_addr, &local_len);
    createPrompt(localsocket, &prompt_socket, prompt_line);

    // Inizializzazioni poll
    end_loop = 0;
    current_size = 0;
    signal_num = 2;

    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = prompt_socket;
    fds[0].events = POLLIN;
    fds[1].fd = -1;                                 // Socket riservata alla connessione col server
    fds[1].events = POLLIN;                         // Inizialmente settata a -1 per essere ignorata.
    num_fds = 2;

    timeout = ( 10 * 60 * 1000 );

    // Via libera al prompt. La socket viene impostata su non bloccante e salvata  all'interno della
    // struttura per la connessione.
    writeToServer(prompt_socket, C_CONNECTION, "C_CONNECTION");
    fcntl(prompt_socket, F_SETFL, fcntl(prompt_socket, F_GETFL, 0) | O_NONBLOCK);
    server.sd = &fds[1].fd;

    //              MAIN CLIENT LOOP                //

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
            // Socket dormiente
            if(fds[i].revents == 0){
                printf("MAIN: poll continued.\n");
                continue;
            }
            // Valore non atteso.
            if(fds[i].revents != POLLIN){
                fprintf(stderr, ":REVENTS ERROR: fds[%d].revents = %d\n", i, fds[i].revents);
                end_loop = 1;
                break;
            }
            // Socket del prompt
            if(fds[i].fd == prompt_socket) {
                signal_num = readFromServer(prompt_socket, incoming, MAXCOMMBUFFER);
                printf("MAIN: <Prompt> \"%d\" \"%s\".\n", signal_num, incoming);
                end_loop = switchPrompt(&server, prompt_socket, signal_num, incoming);
            }
            // Socket del server
            else {
                signal_num = readFromServer(fds[1].fd, incoming, MAXCOMMBUFFER);
                printf("MAIN: <Server> \"%d\" \"%s\".\n", signal_num, incoming);
                end_loop = switchServer(&server, prompt_socket, signal_num, incoming);
            }
        }
        // restart polling unless loop ended
    } while( !end_loop );

    printf("Terminazione processo client.\n");
    return 0;
}