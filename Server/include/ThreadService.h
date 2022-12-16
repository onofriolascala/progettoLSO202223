/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#ifndef THREADSERVICE_H
#define THREADSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include "../include/Def.h"


/* Funzione principale. Una volta richiesto e ricevuto un login da parte dell'utente, procede a gestire le sue
 * richieste e a comunicare al client di renderizzare quanto serve. */
// !!RICHIESTO UN PARSER ED UN DIZIONARIO DELLE COMUNICAZIONI!!
void* thrService(void* arg);

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2);
// Crea un thread simulando un riavvio.
pthread_t rebuildService(struct player_node* player);

// Gestione della Socket di servizio per la comunicazione di avvenuto join.
/*int createServiceSocket(char service_addr[]);
int destroyServiceSocket(int service_sd);*/

#endif /* THREADSERVICE_H */
