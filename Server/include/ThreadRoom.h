/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#ifndef PROGETTOLSO202223_THREADROOM_H
#define PROGETTOLSO202223_THREADROOM_H
#include <pthread.h>

#define MAXCOMMBUFFER 1024
#define USERNAMELENGHT 20
#define LOCALSOCKETADDRLENGHT 100
#define SERVICEADDRLENGHT 100


struct player_node{
    char username[USERNAMELENGHT];
    int player_socket;
    char service_addr[SERVICEADDRLENGHT];
    struct player_node* next;
};

struct room_node{
    int id;
    char localsocket[LOCALSOCKETADDRLENGHT];
    struct player_node* player_list;
    int player_num;
    struct room_node* next;
};




/* Funzione principale. Una volta inizializzata, comunica direttamente con tutti gli utenti connessi e gestisce la
 * logica di gioco.*/
void* thrRoom(void* arg);

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom();

#endif //PROGETTOLSO202223_THREADROOM_H
