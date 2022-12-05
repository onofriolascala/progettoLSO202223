//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#include "../include/ThreadService.h"

#ifndef SOCKETUTILSERVER_H
#define SOCKETUTILSERVER_H

/* Funzione adibita all'inizializzazione del socket primaria d'ascolto. Crea il socket, gli associa l'indirizzo
 * del server e la mette in ascolto di connessioni in entrata. */
int socketInit(struct sockaddr_in* server_addr, socklen_t* len);

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1);



#endif /* SOCKETUTILSERVER_H */
