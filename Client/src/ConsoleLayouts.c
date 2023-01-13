//
// Created by Mattia on 03/12/2022.
//

#include "../include/ConsoleLayouts.h"

void emptyConsole(void) {
    system("clear");
}

void inputGeneric(int input) {
    green();
    if(input > 0)
        printf("Scegliere una delle %d opzioni: ", input);
    else
        printf("Input: ");
    defaultFormat();
    fflush(stdout);
}
void inputAddress(void) {
    green();
    printf("Indirizzo IPv4: ");
    defaultFormat();
    fflush(stdout);
}
void inputPort(void) {
    green();
    printf("Porta: ");
    defaultFormat();
    fflush(stdout);
}
void inputUsername(void) {
    green();
    printf("Nome Utente (6-32 caratteri ASCII): ");
    defaultFormat();
    fflush(stdout);
}
void inputPassword(void) {
    green();
    printf("Password (10-16 caratteri ASCII): ");
    defaultFormat();
    fflush(stdout);
}
void inputRoom(void) {
    green();
    printf("ID della stanza: ");
    defaultFormat();
    fflush(stdout);
}
// Ritorna la conferma dell'operazione per cui è stata richiesta.
int inputComfirmation(void) {
    int confirm;
    green();
    printf("Confermare per procedere (Y/y): ");
    defaultFormat();
    fflush(stdout);

    confirm = getchar();
    if(confirm == 'Y' || confirm == 'y') confirm = 1;
    else confirm = 0;
    getchar();
    return confirm;
}

void renderConnection(void) {
    printf("\n"
           "\033[32m\033[1m+--------------------------------\033[34m GIOCO DELL'IMPICCATO PER 8 GIOCATORI \033[32m---------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tBenvenuto nel gioco de \"L'Impiccato\". Indovina la parola nascosta prima dei tuoi\t\t\033[1m\033[32m|\n"
           "|\033[0m\tavversari, e selezionane una a tua volta perchè gli altri la scoprano!\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\tPer iniziare, inserisci l'indirizzo ip e la porta del server a cui si desidera connettersi.\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n\n");
    fflush(stdout);
}

void renderLogin(struct server_connection *server) {
    printf("\n"
           "\033[32m\033[1m+-----------------------------------------\033[34m L'IMPICCATO - LOGIN \033[32m-----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\t\t\t\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n");
    saveCursor();
    up(1);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Server IPv4: %s    Porta: %d", server->ip, server->port);
    loadCursor();
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tConnessione al server riuscita. Effettuare l'accesso per iniziare a giocare. In assenza di\t\033[1m\033[32m|\n"
           "|\033[0m\tun account, registrarsi prima e poi provare ad accedere.\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tOpzioni disponibili:\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t0) Disconnessione\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t1) Accesso\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t2) Registrazione\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n");
    //printf("\tServer: %s \t Port: %d\n\n", server->ip, server->port);
    fflush(stdout);
}

void renderHomepage(struct server_connection *server) {
    printf("\n"
           "\033[32m\033[1m+---------------------------------------\033[34m L'IMPICCATO - HOMEPAGE \033[32m----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\t\t\t\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n");
    saveCursor();
    up(1);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Server IPv4: %s    Porta: %d    Username: %s", server->ip, server->port, server->connected_user);
    loadCursor();
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tBenvenuto nella homepage del gioco. Da qui è possibile creare una stanza ed unirvisi come\t\033[1m\033[32m|\n"
           "|\033[0m\tgiocatore, entrarne in una già esistente o vedere quali stanze sono state create sul\t\t\033[1m\033[32m|\n"
           "|\033[0m\tserver. E' infine possibile effettuare il logout, e tornare alla schermata di login.\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tOpzioni disponibili:\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t0) Logout\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t1) Crea una stanza\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t2) Entra in una stanza\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t3) Lista delle stanze\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n");
    //printf("\tServer IPv4: %s      Porta: %d      Username: %s\n\n", server->ip, server->port, server->connected_user);
    fflush(stdout);
}

void renderRoom(struct server_connection *server, struct room_struct *room) {
    printf("\n"
           "\033[32m\033[1m+---------------------------------------\033[34m L'IMPICCATO - HOMEPAGE \033[32m----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\t\t\t\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n");
    saveCursor();
    up(1);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Server IPv4: %s    Porta: %d    Username: %s", server->ip, server->port, server->connected_user);
    loadCursor();
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tBenvenuto nella homepage del gioco. Da qui è possibile creare una stanza ed unirvisi come\t\033[1m\033[32m|\n"
           "|\033[0m\tgiocatore, entrarne in una già esistente o vedere quali stanze sono state create sul\t\t\033[1m\033[32m|\n"
           "|\033[0m\tserver. E' infine possibile effettuare il logout, e tornare alla schermata di login.\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tOpzioni disponibili:\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t0) Logout\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t1) Crea una stanza\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t2) Entra in una stanza\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t3) Lista delle stanze\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n");
    //printf("\tServer IPv4: %s      Porta: %d      Username: %s\n\n", server->ip, server->port, server->connected_user);
    fflush(stdout);
}

