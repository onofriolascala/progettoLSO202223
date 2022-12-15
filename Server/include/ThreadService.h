/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#ifndef THREADSERVICE_H
#define THREADSERVICE_H

#include <pthread.h>


/* Funzione principale. Una volta richiesto e ricevuto un login da parte dell'utente, procede a gestire le sue
 * richieste e a comunicare al client di renderizzare quanto serve. */
// !!RICHIESTO UN PARSER ED UN DIZIONARIO DELLE COMUNICAZIONI!!
void* thrService(void* arg);

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2);

pthread_t rebuildService(struct player_node* player;

#endif /* THREADSERVICE_H */
