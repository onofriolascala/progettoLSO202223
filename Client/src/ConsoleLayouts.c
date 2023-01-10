//
// Created by Mattia on 03/12/2022.
//

#include "../include/ConsoleLayouts.h"

// Macro di manipolazione del cursore
#define clearScreen()       printf("\033[2J")
#define clearLine()         printf("\033[2K")
#define carriageReturn()    printf("\r")
#define gotoxy(x,y)         printf("\033[%d;%dH", (y), (x))
#define up(x)               printf("\033[%dA", (x))
#define down(x)             printf("\033[%dB", (x))
#define right(x)            printf("\033[%dC", (x))
#define left(x)             printf("\033[%dD", (x))

// Macro di formattazione
#define default()           printf("\033[0m")
#define bold()              printf("\033[1m")
#define red()               printf("\033[32m")
#define green()             printf("\033[32m")
#define blue()              printf("\033[34m")
#define white()             printf("\033[37m")


void emptyConsole(void) {
    clearScreen();
}

void inputGeneric(void) {
    green();
    printf("Input: ");
    default();
}
void inputAddress(void) {
    green();
    printf("Indirizzo IPv4: ");
    default();
}
void inputPort(void) {
    green();
    printf("Porta: ");
    default();
}
void inputUsername(void) {
    green();
    printf("Nome Utente: ");
    default();
}
void inputPassword(void) {
    green();
    printf("Password: ");
    default();
}
void inputRoom(void) {
    green();
    printf("ID della stanza: ");
    default();
}
void inputComfirmation(void) {
    green();
    printf("Confermare per procedere (Y/y): ");
    default();
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

void renderLogin(void) {
    printf("\n"
           "\033[32m\033[1m+-----------------------------------------\033[34m L'IMPICCATO - LOGIN \033[32m-----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tConnessione al server riuscita. Effettuare l'accesso per iniziare a giocare. In assenza di\t\033[1m\033[32m|\n"
           "|\033[0m\tun account, registrarsi prima e poi provare ad accedere.\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\tOpzioni disponibili:\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t0) Disconnessione\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t1) Accesso\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\033[0m\t\t2) Registrazione\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n\n");
    fflush(stdout);
}

void renderHomepage(void) {
    printf("\n"
           "\033[32m\033[1m+---------------------------------------\033[34m L'IMPICCATO - HOMEPAGE \033[32m----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
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
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n\n");
    fflush(stdout);
}

