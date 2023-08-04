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
    char tempstring[MAXLOGBUFFER] = "";

    bold();
    renderLogo();
    defaultFormat();
    printf("\n");

    encaseHeaderLine(" GUESS THE WORD - Schermata di Login ");
    encaseSideLine("");
    sprintf(tempstring, "Server IPv4: %-*s Porta: %-*d", MAXIP, server->ip, MAXPORT, server->port);
    encaseSideLine(tempstring);
    encaseSideLine("Connessione al server riuscita. Effettuare l'accesso per iniziare a giocare.");
    encaseSideLine("In assenza di un account, registrarsi prima e poi provare ad accedere.");
    encaseSideLine("");
    encaseSideLine("Opzioni disponibili:");
    encaseSideLine("      0) Disconnessione");
    encaseSideLine("      1) Accesso");
    encaseSideLine("      2) Registrazione");
    encaseSideLine("");
    encaseHeaderLine("");
    printf("\n");
    fflush(stdout);
}

void renderHomepage(struct server_connection *server) {
    char tempstring[MAXLOGBUFFER] = "";

    bold();
    renderLogo();
    defaultFormat();
    printf("\n");

    encaseHeaderLine(" GUESS THE WORD - Schermata di Homepage ");
    encaseSideLine("");
    sprintf(tempstring, "Server IPv4: %-*s  Porta: %-*d", MAXIP, server->ip, MAXPORT, server->port);
    encaseSideLine(tempstring);
    sprintf(tempstring, "Username Giocatore: %-*s", USERNAMELENGTH, server->connected_user);
    encaseSideLine(tempstring);
    encaseSideLine("");
    encaseSideLine("Benvenuto nella homepage del gioco. Da qui e' possibile creare una stanza ed");
    encaseSideLine("unirvisi come giocatore, entrarne in una gia' esistente o vedere quali stanze");
    encaseSideLine("sono state create sul server. E' infine possibile effettuare il logout, e");
    encaseSideLine("tornare alla schermata di login.");
    encaseSideLine("");
    encaseSideLine("Opzioni disponibili:");
    encaseSideLine("      0) Logout");
    encaseSideLine("      1) Crea una stanza");
    encaseSideLine("      2) Entra in una stanza");
    encaseSideLine("      3) Lista delle stanze");
    encaseSideLine("");
    encaseHeaderLine("");
    printf("\n");
    fflush(stdout);
}

void renderRoom(struct server_connection *server, struct room_struct *room) {
    char tempstring[MAXLOGBUFFER] = "";
    int i, j;


    bold();
    renderLogo();
    defaultFormat();
    printf("\n");

    sprintf(tempstring, " GUESS THE WORD - STANZA ID.%d ", room->ID);
    encaseHeaderLine(tempstring);
    encaseSideLine("");
    sprintf(tempstring, "Server IPv4: %-*s  Porta: %-*d", MAXIP, server->ip, MAXPORT, server->port);
    encaseSideLine(tempstring);
    sprintf(tempstring, "Username Giocatore: %-*s", USERNAMELENGTH, server->connected_user);
    encaseSideLine(tempstring);
    encaseSideLine("");
    sprintf(tempstring, "Suzerain: %s", room->surezain);
    encaseSideLine(tempstring);
    sprintf(tempstring, "Giocatori(%d/8): %-*s %-*s", room->player_num, USERNAMELENGTH, room->players[0], USERNAMELENGTH, room->players[1]);
    encaseSideLine(tempstring);

    i = 2;
    while( i < MAXPLAYERS ) {
        j = i+1;
        i = i+2;
        sprintf(tempstring, "                %-*s %-*s", USERNAMELENGTH, room->players[i], USERNAMELENGTH, room->players[j]);
        encaseSideLine(tempstring);
    }

    encaseSideLine("");
    encaseSideLine("Parola da indovinare:");
    encaseSideLine(room->secret_word);
    encaseSideLine("");
    encaseHeaderLine("");
    printf("\n");
    fflush(stdout);
}

