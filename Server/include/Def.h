//
// Created by onofrio on 15/12/22.
//

#ifndef PROGETTOLSO202223_DEF_H
#define PROGETTOLSO202223_DEF_H

#define MAXCONNECTIONS 5

#define MAXCOMMBUFFER 1024

#define MAXSIGNALBUF 2

#define USERNAMELENGHT 32

#define PASSWORDLENGHT 16

#define LOCALSOCKETADDRLENGHT 100

// Numero di microsecondi degli usleep()
#define REFRESHCONSTANT 5000


struct player_node{
    char username[USERNAMELENGHT];
    int player_socket;
    //char service_addr[LOCALSOCKETADDRLENGHT];
    struct player_node* next;
};

struct room_node{
    int id;
    char localsocket[LOCALSOCKETADDRLENGHT];
    struct player_node* player_list;
    int player_num;
    struct room_node* next;
};

struct service_arg {
    int sd;
    struct room_node** room_list;
    struct player_node* player;
    int flag;
};

struct room_arg {
    struct room_node** room_list;
    int room_ID;
    int flag;
};

#endif //PROGETTOLSO202223_DEF_H