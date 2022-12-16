//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#ifndef SOCKETUTILSERVER_H
#define SOCKETUTILSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "../include/Def.h"
#include "../include/ThreadService.h"



/* Funzione adibita all'inizializzazione del socket primaria d'ascolto. Crea il socket, gli associa l'indirizzo
 * del server e la mette in ascolto di connessioni in entrata. */
int socketInit(struct sockaddr_in* server_addr, socklen_t* len);

int localSocketInit(int ID, char local_path[], struct sockaddr_un* localsocket_addr, socklen_t* len);
void deleteLocalSocket(int localsocket, char localsocket_path[]);

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1, struct room_node** room_list);



#endif /* SOCKETUTILSERVER_H */
