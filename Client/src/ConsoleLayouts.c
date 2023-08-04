//
// Created by Mattia on 03/12/2022.
//

#include "../include/ConsoleLayouts.h"

void emptyConsole(void) {
    //system("clear");
    clearScreen();
    gotoxyCursor(0,0);
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
void inputComfirmation(void) {
    green();
    printf("Confermare per procedere (Y/y): ");
    defaultFormat();
    fflush(stdout);
}

void centerText(char *text, int fieldwidth) {
    int padlen = (fieldwidth / 2) - (strlen(text) / 2);
    printf("%*s%s%*s", padlen, "", text, padlen, "");
}
void offsetText(char *text, int fieldwidth) {
    int wordlen = strlen(text);
    int padlen = fieldwidth - wordlen - (SIDEOFFSET*2);
    printf("%*s%s%*s%*s", SIDEOFFSET, "", text, padlen, "", SIDEOFFSET, "");
}
void encaseHeaderLine(char *text) {
    printf(GRN "+");
    int wordlen = strlen(text);
    int padlen = ((MAXLINEWIDTH-2) / 2) - (wordlen/ 2);
    for( int i = 0; i < padlen; i++ ) {
        printf("-");
    }
    printf( DFT BLD BLU "%s" DFT GRN , text);

    if ((wordlen%2)!=0) {
        padlen--;
    }
    for( int i = 0; i < padlen; i++ ) {
        printf("-");
    }
    printf("+" DFT "\n");
}
void encaseCenterLine(char *text) {
    printf(GRN "|" DFT);
    centerText(text, MAXLINEWIDTH-2);
    printf(GRN "|" DFT "\n");
}
void encaseSideLine(char *text) {
    printf(GRN "|" DFT);
    offsetText(text, MAXLINEWIDTH-2);
    printf(GRN "|" DFT "\n");
}

void renderLogo(void) {
    centerText("    ___          ___          ___          ___          ___     ", MAXLINEWIDTH);
    printf("\n");
    centerText("   /  /\\        /__/\\        /  /\\        /  /\\        /  /\\    ", MAXLINEWIDTH);
    printf("\n");
    centerText("  /  / /_       \\  \\ \\      /  / /       /  / /_      /  / /_   ", MAXLINEWIDTH);
    printf("\n");
    centerText(" /  / / /\\    ___\\  \\ \\    /  / /_      /  / / /\\    /  / / /\\  ", MAXLINEWIDTH);
    printf("\n");
    centerText("/__/ /_/  \\  /__/\\\\  \\ \\  /__/ / /\\__  /__/ / /  \\  /__/ / /  \\ ", MAXLINEWIDTH);
    printf("\n");
    centerText("\\  \\ \\_\\/\\ \\ \\  \\ \\\\__\\ \\ \\  \\ \\/ / /\\ \\  \\ \\/ /\\ \\ \\  \\ \\/ /\\ \\", MAXLINEWIDTH);
    printf("\n");
    centerText(" \\  \\ \\  7 /  \\  \\ \\  / /  \\  \\  / / /  \\  \\  / 7 /  \\  \\  / 7 /", MAXLINEWIDTH);
    printf("\n");
    centerText("  \\  \\ \\/ /    \\  \\ \\/ /    \\  \\ \\/ /    \\__\\/ / /    \\__\\/ / / ", MAXLINEWIDTH);
    printf("\n");
    centerText("   \\  \\  /      \\  \\  /      \\  \\  /       /__/ /       /__/ /  ", MAXLINEWIDTH);
    printf("\n");
    centerText("    \\__\\/        \\__\\/        \\__\\/        \\__\\/        \\__\\/   ", MAXLINEWIDTH);
    printf("\n");
    fflush(stdout);
}

void renderConnection(void) {
    printf(SCR );
    gotoxyCursor(0,0);

    bold();
    slowblink();
    renderLogo();
    defaultFormat();
    printf("\n");

    encaseHeaderLine(" GUESS THE WORD - Gioco per 8 persone ");
    encaseSideLine("");
    encaseSideLine("Benvenuto nel gioco de \"Guess the Word\". Indovina la parola nascosta prima dei");
    encaseSideLine("tuoi avversari, e selezionane una a tua volta perche' gli altri la scoprano!");
    encaseSideLine("Per iniziare, inserisci l'indirizzo ip e la porta del server a cui si desidera");
    encaseSideLine("connettersi.");
    encaseSideLine("");
    encaseHeaderLine("");
    printf("\n");
    fflush(stdout);
}

void renderLogin(struct server_connection *server) {
    char tempstring[100] = "";

    bold();
    renderLogo();
    defaultFormat();
    printf("\n");

    encaseHeaderLine(" GUESS THE WORD - Schermata di Login ");
    encaseSideLine("");
    sprintf(tempstring, "Server IPv4: %s    Porta: %d", server->ip, server->port);
    encaseSideLine("Benvenuto nel gioco de \"Guess the Word\". Indovina la parola nascosta prima dei");
    encaseSideLine("Connessione al server riuscita. Effettuare l'accesso per iniziare a giocare.");
    encaseSideLine("In assenza di un account, registrarsi prima e poi provare ad accedere.");
    encaseSideLine("");
    encaseSideLine("Opzioni disponibili:");
    encaseSideLine("      0) Disconnessione");
    encaseSideLine("      1) Accesso");
    encaseSideLine("      2) Registrazione");
    encaseSideLine("");
    encaseHeaderLine("");

    /*printf("\n"
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
    //printf("\tServer: %s \t Port: %d\n\n", server->ip, server->port);*/
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
           "\033[32m\033[1m+---------------------------------------\033[34m L'IMPICCATO - ROOM ID \033[32m-----------------------------------------+\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\t\t\t\t\t\t\t\t\t\t\t\t\t\033[1m\033[32m|\n");
    saveCursor();
    gotoxyCursor(2, 64);
    printf("\033[34m\033[1m%d \033[0m", room->ID);
    loadCursor();

    saveCursor();
    up(4);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Server IPv4: %s    Porta: %d    Username: %s", server->ip, server->port, server->connected_user);
    loadCursor();

    saveCursor();
    up(2);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Suzerain: %s", server->connected_user);
    loadCursor();

    saveCursor();
    up(1);
    carriageReturn();
    printf("\033[32m\033[1|\033[0m\t");
    printf("Giocatori(%d/8): %s", room->player_num, server->connected_user);
    loadCursor();

    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "|\033[0m\t\033[1m\033[37mParola: In attesa di una parola...\t\t\t\t\t\t\t\t\033[32m|\n"
           "|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n"
           "+-------------------------------------------------------------------------------------------------------+\033[0m\n");
    //printf("\tServer IPv4: %s      Porta: %d      Username: %s\n\n", server->ip, server->port, server->connected_user);
    fflush(stdout);
}

