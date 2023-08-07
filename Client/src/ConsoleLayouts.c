//
// Created by Mattia on 03/12/2022.
//

#include "../include/ConsoleLayouts.h"

void emptyConsole(void) {
    //system("clear");
    clearScreen();
    gotoxyCursor(0,0);
}

void promptGenericChoiceMSG(int input) {
    green();
    if(input > 0)
        printf("Scegliere una delle %d opzioni: ", input);
    else
        printf("Input: ");
    defaultFormat();
    fflush(stdout);
}
void promptIPAddressMSG(void) {
    green();
    printf("Indirizzo IPv4: ");
    defaultFormat();
    fflush(stdout);
}
void promptPortMSG(void) {
    green();
    printf("Porta: ");
    defaultFormat();
    fflush(stdout);
}
void promptUsernameMSG(void) {
    green();
    printf("Nome Utente (6-32 caratteri ASCII): ");
    defaultFormat();
    fflush(stdout);
}
void promptPasswordMSG(void) {
    green();
    printf("Password (10-16 caratteri ASCII): ");
    defaultFormat();
    fflush(stdout);
}
void promptRoomIDMSG(void) {
    green();
    printf("ID della stanza: ");
    defaultFormat();
    fflush(stdout);
}
// Ritorna la conferma dell'operazione per cui è stata richiesta.
void promptConfirmationMSG(void) {
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

    printf("\n");
    for(int j = 0; j <= MAXCOLUMNHEIGHT; j++) {
        printf("\n");
    }

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
    //sprintf(tempstring, "Giocatori(%d/%d): %-*s %-*s", room->player_num, MAXPLAYERS, USERNAMELENGTH, room->players[0], USERNAMELENGTH, room->players[1]);
    sprintf(tempstring, "Giocatori(%d/%d):", room->player_num, MAXPLAYERS);
    encaseSideLine(tempstring);

    i = 2;
    while( i < MAXPLAYERS ) {
        j = i+1;
        if (j >= MAXPLAYERS) {
            //sprintf(tempstring, "                %-*s", USERNAMELENGTH, room->players[i]);
        }
        else {
            //sprintf(tempstring, "                %-*s %-*s", USERNAMELENGTH, room->players[i], USERNAMELENGTH,
            //        room->players[j]);
        }
        i = i+2;
        encaseSideLine("");
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
    updatePlayerNumber(room);
    for(int i = 0; i < MAXPLAYERS; i++)
    {
        updatePlayer(room, i);
    }

    gotoxyCursor(V_OFFSET_PROMPT, 2);
    printf("> ");
    slideMessages(room);
}

// Le seguenti funzioni manipolano il cursore della console per modificare lo stato della schermata.
// Essendo funzioni di UI non fanno error-checking in relazione ai valori passati, assumendoli corretti.
/*   N.B. Il numero di giocatori ha offset orizzontale 17, la parola 7, i giocatori pari ed il suzerain 23, quelli dispari 56
 *   La composizione della schermata è la seguente
 *   righe  0-11 : logo + ""
 *   righe  12-13: Header + ""
 *   righe  14-16: ServerIP, Porta + Username + ""
 *   riga   17   : Suzerain
 *   righe  18-??: Giocatori + "", 18+(MAXPLAYERS/2)
 *   righe ~19   : Pre-parola
 *   riga  ~20   : riga : Parola nascosta
 *   righe ~21-23: "" + Closing Header + ""
 *   righe ~24-26: Pre-prompt, User prompt, Post-prompt
 *   right ~27-MAXSAVEDMESSAGES: Log ascendente di comunicazioni
 */
// Stampa video sulle linee dalla posizione V_OFFSET_SAVEDMSG a V_OFFSET_SAVEDMSG + MAXSAVEDMESSAGE tutti
// i messaggi salvati sulla struct room.
void slideMessages(struct room_struct *room) {
    int i;
    saveCursor();
    for(i = 0; i < MAXSAVEDMESSAGES; i++)  {
        gotoxyCursor(V_OFFSET_SAVEDMSG+i, 0);
        clearLine();
        if(room != NULL) {
            printf("%s", room->saved_messages[i]);
        }
    }
    i++;
    gotoxyCursor(V_OFFSET_SAVEDMSG + i, 0);
    encaseHeaderLine("");
    loadCursor();
    fflush(stdout);
}
// Pulisce tutte le linee dalla posizione V_OFFSET_SAVEDMSG a V_OFFSET_SAVEDMSG + MAXSAVEDMESSAGE
void clearMessages(struct room_struct *room) {
    saveCursor();
    gotoxyCursor(V_OFFSET_SAVEDMSG, 0);
    for(int i = 0; i < MAXSAVEDMESSAGES; i++)  {
        clearLine();
        gotoxyCursor(V_OFFSET_SAVEDMSG+i, 0);
        if(room != NULL) {
            memset(room->saved_messages[i], '\0', sizeof(room->saved_messages[i]));
        }
    }
    loadCursor();
    fflush(stdout);
}
// Aggiorna le posizioni da V_OFFSET_PLAYERS a V_OFFSET_PLAYERS+((MAXPLAYERS/2)-1), H_OFFSET_PLAYERSEVN per le posizioni pari,
// H_OFFSET_PLAYERSODD per le posizioni dispari
void updatePlayer(struct room_struct *room, int position) {
    int vertical_offset = 0, horizontal_offset = 0;
    saveCursor();

    if (position == 0 || (position % 2) == 0) {
        vertical_offset = V_OFFSET_PLAYERS + (position/2);
        horizontal_offset = H_OFFSET_PLAYEREVN;
    }
    else {
        vertical_offset = V_OFFSET_PLAYERS + (position/2);
        horizontal_offset = H_OFFSET_PLAYERODD;
    }

    gotoxyCursor(vertical_offset, horizontal_offset);
    printf("%*s", USERNAMELENGTH, "");
    gotoxyCursor(vertical_offset, horizontal_offset);
    if(room != NULL) {
        if((strcmp(room->players[position], "Vuoto") == 0) || (strcmp(room->players[position], BLK"Vuoto"DFT) == 0)) {
            black();
        }
        printf("%s"DFT, room->players[position]);
    }
    else {
        printf(BLK"Vuoto"DFT);
    }
    loadCursor();
    fflush(stdout);
}
// Aggiorna la posizione V_OFFSET_PLAYERS, H_OFFSET_PLAYERNUM con l'attuale numero di giocatori salvati nella struct room
void updatePlayerNumber(struct room_struct *room) {
    saveCursor();
    gotoxyCursor(V_OFFSET_PLAYERS, H_OFFSET_PLAYERNUM);
    if (room != NULL) {
        printf("%-1d", room->player_num);
    }
    else {
        printf("-");
    }
    loadCursor();
    fflush(stdout);
}
// Aggiorna la posizione V_OFFSET_WORD, H_OFFSET_WORD con l'attuale parola salvata nella struct room
void updateWord(struct room_struct *room) {
    gotoxyCursor(V_OFFSET_WORD, 0);
    clearLine();
    encaseSideLine("");
    gotoxyCursor(V_OFFSET_WORD, H_OFFSET_WORD);
    if(room != NULL) {
        printf( BLD WHT "%s" DFT, room->secret_word);
    }
    else {
        printf( BLD WHT "Placeholder" DFT);
    }
    loadCursor();
    fflush(stdout);
}
// Aggiorna la posizione V_OFFSET_SUZERAIN, H_OFFSET_SUZERAIN con l'attuale Suzerain salvato nella struct room
void updateSuzerain(struct room_struct *room) {
    saveCursor();
    gotoxyCursor(V_OFFSET_SUZERAIN, H_OFFSET_SUZERAIN);
    if(room != NULL) {
        printf(BLD "%-*s" DFT, USERNAMELENGTH, room->suzerain);
    }
    else {
        printf( BLD WHT "Placeholder" DFT);
    }
    loadCursor();
    fflush(stdout);
}
// Scrive a video l'opzione omonima sulla linea V_OFFSET_PROMPT-1
void updateVictory(void) {
    gotoxyCursor(V_OFFSET_PROMPT-1, 0);
    clearLine();
    carriageReturn();
    printf(BLD YLW " HAI INDOVINATO! VITTORIA! Resta in attesa per diventare il nuovo Suzerain e scegliere la parola." DFT);
    loadCursor();
    fflush(stdout);
}
// Scrive a video l'opzione omonima sulla linea V_OFFSET_PROMPT-1
void updateDefeat(void) {
    gotoxyCursor(V_OFFSET_PROMPT-1, 0);
    clearLine();
    carriageReturn();
    printf(RED " QUALCUN ALTRO HA INDOVINATO. SCONFITTA! Resta in attesa della parola scelta dal nuovo Suzerain." DFT);
    loadCursor();
    fflush(stdout);
}

// Scrive a video l'opzione omonima sulla linea V_OFFSET_PROMPT-1
void prePromptTryGuess(void) {
    saveCursor();
    gotoxyCursor(V_OFFSET_PROMPT-1, 0);
    clearLine();
    carriageReturn();
    printf(" E' il tuo turno, prova a indovinare:");
    loadCursor();
    fflush(stdout);
}
// Scrive a video l'opzione omonima sulla linea V_OFFSET_PROMPT-1
void prePromptExit(void) {
    saveCursor();
    gotoxyCursor(V_OFFSET_PROMPT-1, 0);
    clearLine();
    carriageReturn();
    printf(" Premere Esc + Invio per uscire dalla stanza:");
    loadCursor();
    fflush(stdout);
}
// Scrive a video l'opzione omonima sulla linea V_OFFSET_PROMPT-1
void prePromptChooseWord(void) {
    saveCursor();
    gotoxyCursor(V_OFFSET_PROMPT-1, 0);
    clearLine();
    carriageReturn();
    printf(" Sei il Suzerain. Scegli un'opzione tra quelle elencate sopra:");
    loadCursor();
    fflush(stdout);
}
// Riporta il cursore alla posizione V_OFFSET_PROMPT
void resetCursor(void) {
    gotoxyCursor(V_OFFSET_PROMPT, 0);
    clearLine();
    carriageReturn();
    printf(" > ");
    fflush(stdout);
}

