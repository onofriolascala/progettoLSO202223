//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#ifndef PROGETTOLSO202223_SOCKETUTIL_H
#define PROGETTOLSO202223_SOCKETUTIL_H

/* Funzione adibita all'inizializzazione del socket primaria d'ascolto. Crea il socket, gli associa l'indirizzo
 * del server e la mette in ascolto di connessioni in entrata. */
int socketInit(struct sockaddr_in* server_addr, int len);

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1);



#endif //PROGETTOLSO202223_SOCKETUTIL_H
