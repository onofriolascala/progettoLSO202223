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

#include "../include/LogUtilClient.h"
#include "../include/SocketUtilClient.h"
#include "../include/CommUtilClient.h"
#include "../include/Prompt.h"
#include "../include/PollSwitches.h"
#include "../../Server/include/Def.h"

int main() {
    // Dichiarazioni per la connessione
    int localsocket;
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

    // Dichiarazione delle strutture che conterranno le informazioni relative alla connessione ed alla stanza.
    struct server_connection server;
    struct room_struct room;

    // Inizializzazione della struttura per il prompt e del suo mutex
    struct prompt_thread *prompt;
    if((prompt = (struct prompt_thread*)malloc(sizeof(struct prompt_thread))) == NULL){
        //gestire errore malloc
        fprintf(stderr, ":PROMPT STRUCT MALLOC: prompt was initialized to NULL.");
        exit(1);
    }
    pthread_mutex_init(&prompt->mutex, NULL);

    // Inizializzazione del file descriptor del log e del file stesso.
    if((prompt->log = (int*)malloc(sizeof(int))) == NULL){
        //gestire errore malloc
        fprintf(stderr, ":PROMPT STRUCT MALLOC: prompt was initialized to NULL.");
        exit(1);
    }
    *prompt->log = createLog();

    // Inizializzazioni poll
    end_loop = 0;
    current_size = 0;
    signal_num = 2;

    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[1].fd = -1;                                 // Socket riservata alla connessione col server
    fds[1].events = POLLIN;                         // Inizialmente settata a -1 per essere ignorata.
    num_fds = 2;

    timeout = ( 10 * 60 * 1000 );

    // Inizializzazione della socket locale e del thread per il PROMPT
    if((localsocket = localSocketInit(&localsocket_addr, &local_len)) < 0) {
        printError(prompt, "ERRORE: Prima creazione della socket locale fallita. Chiusura processo.\n",
                   ":LOCAL SOCKET INIT", localsocket);
        exit(1);
    }
    else {
        sprintf(prompt->log, "MAIN: localSocketInit completed with value \"%d\".\n", localsocket);
        writeToLog(*prompt->log, prompt->log);
        memset(prompt->log_str, '\0', sizeof(prompt->log_str));
    }
    prompt->sd = &fds[0].fd;
    prompt->id = createPrompt(localsocket, prompt);

    // Rendering Iniziale
    emptyConsole();
    renderConnection();
    server.last_signal = C_CONNECTION;

    // Via libera al prompt. La socket viene impostata su non bloccante e salvata  all'interno della
    // struttura per la connessione.

    writeToServer(*prompt->sd, C_CONNECTION, "C_CONNECTION");
    fcntl(*prompt->sd, F_SETFL, fcntl(*prompt->sd, F_GETFL, 0) | O_NONBLOCK);
    server.sd = &fds[1].fd;
    server.localsocket = &localsocket;
    memset(server.connected_user, '\0', sizeof(server.connected_user));

    //              MAIN CLIENT LOOP                //

    do {
        writeToLog(*prompt->log, "MAIN: Waiting on poll function...\n");
        fflush(stdout);

        memset(incoming, '\0', sizeof(incoming));
        memset(outgoing, '\0', sizeof(outgoing));
        memset(prompt->log_str, '\0', sizeof(prompt->log_str));

        rc = poll(fds, num_fds, timeout);

        // Errore del poll
        if( rc < 0 ) {
            printError(prompt, ECRITICALCLIENT, ":POLL ERROR", errno);
            break;
        }
        // Timeout
        if( rc == 0 ) {
            printWarning(prompt, "!ATTENZIONE! Nessuna risposta dal server o dall'utente. Riavvio.\n");
            // RIAVVIO CONNESSIONE
            switchServer(&server, &room, prompt, S_DISCONNECT, "C_DISCONNECT");
            continue;
        }
        for(i = 0; i < num_fds; i++){
            // Socket dormiente
            if(fds[i].revents == 0){
                writeToLog(*prompt->log, "MAIN: poll continued.\n");
                continue;
            }
            // Valore non atteso.
            if(fds[i].revents != POLLIN){
                sprintf(prompt->log_str, ":REVENTS ERROR: fds[%d].revents = %d\n", i, fds[i].revents);
                printError(prompt, ECRITICALCLIENT, prompt->log_str, errno);
                end_loop = 1;
                break;
            }
            // Socket del prompt
            if(fds[i].fd == *prompt->sd) {
                signal_num = readFromServer(*prompt->sd, incoming, MAXCOMMBUFFER);
                sprintf(prompt->log_str, "MAIN: <Prompt> \"%d\" \"%s\".\n", signal_num, incoming);
                writeToLog(*prompt->log, prompt->log_str);
                end_loop = switchPrompt(&server, &room, prompt, signal_num, incoming);
            }
            // Socket del server
            else {
                signal_num = readFromServer(fds[1].fd, incoming, MAXCOMMBUFFER);
                sprintf(prompt->log_str, "MAIN: <Server> \"%d\" \"%s\".\n", signal_num, incoming);
                writeToLog(*prompt->log, prompt->log_str);
                end_loop = switchServer(&server, &room, prompt, signal_num, incoming);
            }
        }
    } while( !end_loop );

    writeToLog(*prompt->log, "Terminazione processo client.\n\n\t\t\t END CLIENT.\n");
    return 0;
}