/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#ifndef PROGETTOLSO202223_THREADROOM_H
#define PROGETTOLSO202223_THREADROOM_H
#include <pthread.h>

#define MAXCOMMBUFFER 1024
#define USERNAMELENGHT 32
#define LOCALSOCKETADDRLENGHT 100


struct room_node* addNewRoom(struct room_node* room_list);

/* Funzione principale. Una volta inizializzata, comunica direttamente con tutti gli utenti connessi e gestisce la
 * logica di gioco.*/
void* thrRoom(void* arg);

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom(int sd, struct room_node** room_list);

#endif //PROGETTOLSO202223_THREADROOM_H
