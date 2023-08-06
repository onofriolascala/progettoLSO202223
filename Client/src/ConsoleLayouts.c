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
    char tempstring[MAXLOGBUFFER] = "";
    printf(SCR );
    gotoxyCursor(0,0);

    bold();
    slowblink();
    renderLogo();
    defaultFormat();
    printf("\n");

    sprintf(tempstring, " GUESS THE WORD - Gioco per %d persone ", MAXPLAYERS);
    encaseHeaderLine(tempstring);
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

    printf(SCR );
    gotoxyCursor(0,0);

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

    printf(SCR );
    gotoxyCursor(0,0);

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
    sprintf(tempstring, "Suzerain:       %s", room->suzerain);
    encaseSideLine(tempstring);
    sprintf(tempstring, "Giocatori(%d/%d): %-*s %-*s", room->player_num, MAXPLAYERS, USERNAMELENGTH, room->players[0], USERNAMELENGTH, room->players[1]);
    encaseSideLine(tempstring);

    i = 2;
    while( i < MAXPLAYERS ) {
        j = i+1;
        if (j >= MAXPLAYERS) {
            sprintf(tempstring, "                %-*s", USERNAMELENGTH, room->players[i]);
        }
        else {
            sprintf(tempstring, "                %-*s %-*s", USERNAMELENGTH, room->players[i], USERNAMELENGTH,
                    room->players[j]);
        }
        i = i+2;
        encaseSideLine(tempstring);
    }

    encaseSideLine("");
    encaseSideLine("Parola da indovinare:");
    encaseSideLine(room->secret_word);
    encaseSideLine("");
    encaseHeaderLine("");
    printf("\n");
    fflush(stdout);

    updateWord(room);
    updateSuzerain(room);
}

// Le seguenti funzioni manipolano il cursore della console per modificare lo stato della schermata.
// Essendo funzioni di UI non fanno error-checking in relazione ai valori passati, assumendoli corretti.
/*   N.B. Il numero di giocatori ha offset orizzontale 17, la parola 7, i giocatori pari ed il suzerain 23, quelli dispari 56
 *   La composizione della schermata è la seguente
 *   righe  0-11 : logo + ""
 *   righe  12-13: Header + ""
 *   righe  14-16: ServerIP, Porta + Username + ""
 *   riga   17   : Suzerain
 *   righe  18-??: Giocatori, va da 1 a MAXPLAYERS
 *   righe ~19-20: "" + Pre-parola
 *   riga  ~21   : riga : Parola nascosta
 *   righe ~22-24: "" + Closing Header + ""
 *   righe ~25-26: User prompt
 *   right ~27-MAXSAVEDMESSAGES: Log ascendente di comunicazioni
 */
void slideMessages(struct room_struct *room) {
    int vertical_offset, i;
    saveCursor();

    if ((MAXPLAYERS % 2)==0) {
        vertical_offset = 27 + (MAXPLAYERS/2) - 1;
    }
    else {
        vertical_offset = 27 + (MAXPLAYERS/2);
    }

    gotoxyCursor(vertical_offset, 0);

    for(i = 0; i < MAXSAVEDMESSAGES-1; i++)  {
        clearLine();
        printf( "%s\n", room->saved_messages[i]);
    }
    clearLine();
    printf( BLK"%s"DFT, room->saved_messages[++i]);

    loadCursor();
    fflush(stdout);
}
void clearMessages(void) {
    int vertical_offset;
    saveCursor();

    if ((MAXPLAYERS % 2)==0) {
        vertical_offset = 29 + (MAXPLAYERS/2) - 1;
    }
    else {
        vertical_offset = 29 + (MAXPLAYERS/2);
    }

    gotoxyCursor(vertical_offset, 0);

    for(int i = 0; i < MAXSAVEDMESSAGES; i++)  {
        clearLine();
    }
    loadCursor();
    fflush(stdout);
}
void updatePlayer(struct room_struct *room, int position) {
    int vertical_offset = 0, horizontal_offset = 0;
    saveCursor();

    if (position == 0 || (position % 2) == 0) {
        vertical_offset = 18 + (position/2);
        horizontal_offset = 23;
    }
    else {
        vertical_offset = 18 + (position/2);
        horizontal_offset = 56;
    }

    gotoxyCursor(vertical_offset, horizontal_offset);
    if((strcmp(room->players[position], "Vuoto") == 0) || (strcmp(room->players[position], BLK"Vuoto"DFT) == 0)) {
        black();
    }
    printf("%s"DFT, room->players[position]);
    loadCursor();
    fflush(stdout);
}
void updatePlayerNumber(struct room_struct *room) {
    int vertical_offset, horizontal_offset;
    saveCursor();

    vertical_offset = 18;
    horizontal_offset = 17;

    gotoxyCursor(vertical_offset, horizontal_offset);
    printf("%-1d", room->player_num);
    loadCursor();
    fflush(stdout);
}
void updateWord(struct room_struct *room) {
    int vertical_offset;
    saveCursor();

    if ((MAXPLAYERS % 2)==0) {
        vertical_offset = 21 + (MAXPLAYERS/2) - 1;
    }
    else {
        vertical_offset = 21 + (MAXPLAYERS/2);
    }

    gotoxyCursor(vertical_offset, 0);
    clearLine();
    encaseSideLine("");
    gotoxyCursor(vertical_offset, 7);

    printf( BLD WHT "%-32s" DFT, room->secret_word);
    loadCursor();
    fflush(stdout);
}
void updateSuzerain(struct room_struct *room) {
    saveCursor();
    gotoxyCursor(17, 23);
    printf(BLD GRN "%-32s" DFT, room->suzerain);
    loadCursor();
    fflush(stdout);
}
void updateVictory(void) {
    saveCursor();
    clearLine();
    up(1);
    clearLine();
    printf(BLD YLW "****** HAI INDOVINATO! VITTORIA! ******\n"
                   "Resta in attesa per diventare il nuovo Suzerain e scegliere la parola.\n\n" DFT);
    loadCursor();
    fflush(stdout);
}
void updateDefeat(void) {
    saveCursor();
    clearLine();
    up(1);
    clearLine();
    printf(BLD RED "------ QUALCUN ALTRO HA INDOVINATO. SCONFITTA! ------\n"
           "Resta in attesa della parola scelta dal nuovo Surain.\n\n" DFT);
    loadCursor();
    fflush(stdout);
}
void failedGuess(char *incoming) {
    printf(" > %s  ->  SBAGLIATO", incoming);
    fflush(stdout);
}
void selectWord(char *incoming) {
    //saveCursor();
    //clearLine();
    //up(1);
    //up(1);
    //clearLine();
    printf("Sei il nuovo Surezain. Scegli una parola tra le seguenti:\n"
           " > %s\n", incoming);
    //loadCursor();
    fflush(stdout);
}

