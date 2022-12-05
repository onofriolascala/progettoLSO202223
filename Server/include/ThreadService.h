/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#ifndef THREADSERVICE_H
#define THREADSERVICE_H

#include <pthread.h>


#define MAXCOMMBUFFER 1024

/* Funzione principale. Una volta richiesto e ricevuto un login da parte dell'utente, procede a gestire le sue
 * richieste e a comunicare al client di renderizzare quanto serve. */
// !!RICHIESTO UN PARSER ED UN DIZIONARIO DELLE COMUNICAZIONI!!
void* thrService(void* arg);

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2);

// Comunicazioni tramite stringa tra thread e client associato.
void sendToClient(char input[], char output[]);
void receiveFromClient(char input[], char output[]);

#endif /* THREADSERVICE_H */
