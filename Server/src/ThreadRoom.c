/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/ThreadRoom.h"

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

// Funzione principale.
void* thrRoom(void* arg) {
    void* out;

    return out;
}

// Creazione del thread. Richiama <foo_name> dal thread costruttore.
pthread_t createNewRoom() {
    pthread_t tid;

    return tid;
}