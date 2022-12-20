//
// Created by osboxes on 12/5/22.
//

#include "../include/LogicSigninLogin.h"

/* Funzione di signin. Riceve due stringhe non vuote ed esegue una query di inserimento all'interno del database MySQL.
 * Restituisce un segnale a seconda dell'esito. S_LOGINOK in caso di successo, S_LOGINERROR per input scorretto,
 * S_SIGNINERROR in caso di uno username già esistente. */
int signin(char incoming[], struct player_node* player, char outgoing[]){
    char username[USERNAMELENGHT+1], password[PASSWORDLENGHT+1], temp_buf[MAXSIGNALBUF+1];
    char *sub_string;
    int signal_num, username_len, pswd_len, cursor, query_result;

    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    strncpy(temp_buf, incoming, MAXSIGNALBUF);
    temp_buf[MAXSIGNALBUF] = '\0';
    username_len = atoi(temp_buf);
    //controlli di correttezza username, implementate richieste di dimensione
    if((username_len < 6) || (username_len > 32)){
        strcpy(outgoing, "L'username non soddisfa le richieste");
        signal_num = 91;
    }
    else {
        cursor = MAXSIGNALBUF;
        sub_string = &incoming[cursor];
        strncpy(username, sub_string, username_len);
        cursor += username_len;
        username[username_len] = '\0';
        memset(temp_buf, 0, sizeof(temp_buf));
        sub_string = &incoming[cursor];
        strncpy(temp_buf, sub_string, MAXSIGNALBUF);
        temp_buf[MAXSIGNALBUF] = '\0';
        pswd_len = atoi(temp_buf);
        //controlli di correttezza password, implementate richieste di dimensione, necessarie regole per la scelta di caratteri?
        if((pswd_len<6)||(pswd_len>32)){
            strcpy(outgoing, "La password non soddisfa le richieste");
            signal_num = 91;
        }
        else {
            cursor += MAXSIGNALBUF;
            sub_string = &incoming[cursor];
            strncpy(password, sub_string, pswd_len);
            password[pswd_len] = '\0';
            // insertQuery((struct mySQLConnection* LSO2223, char[] username, char[] password):)
            if (1 == 1062) {
                strcpy(outgoing, "Utente già esistente.");
                signal_num = 91;
            } else {
                strcpy(outgoing, "Utente registrato.");
                signal_num = 51;
            }
        }
    }
    return signal_num;
}

/* Funzione di logout. Elimina in maniera sicura il nodo del giocatore, e ne restituisce un puntatore a NULL.*/
struct player_node* logout(struct player_node* player){
    destroyPlayerNode(player);
    player = NULL;
    return player;
}

/* Funzione di login. Interroga il database MySQL alla ricerca di un utente con stesso username e password.
 * Riceve un socket descriptor, due stringhe non nulle ed una puntatore ad un giocatore. In caso di successo
 * della query assegnerà i valori username e socket alla struttura giocatore.
 * Restituisce un segnale, S_HOMEPAGEOK in caso di successo, S_LOGINERROR in tutti gli altri casi. */
int login(int sd, char incoming[], struct player_node* player, char outgoing[]) {
    //printf("\t\tDEBUG_loginSD%d: strated with values %s %s.\n", sd, incoming, outgoing);
    char username[USERNAMELENGHT+1], password[PASSWORDLENGHT+1];
    char *sub_string;
    int signal_num, query_result;

    if(player == NULL) {
        fprintf(stderr, ":LOGIN ERROR: null player pointer passed as argument.\n");
        strcpy(outgoing, "Giocatore non inizializzato correttamente dal server.");
        return S_SERVERERROR;
    }

    // Il parser verificherà che il contenuto della stringa in input sia corretto
    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    /*
    //int username_len, pswd_len, cursor;
    //char *sub_string;
    //char temp_buf[MAXSIGNALBUF+1];
    // Estrazione dello username //
    // printf("\t\tDEBUG_loginSD%d: username\n", sd);
    // Lunghezza dello username.
    strncpy(temp_buf, incoming, MAXSIGNALBUF);
    temp_buf[MAXSIGNALBUF] = '\0';
    if((username_len = atoi(temp_buf)) > USERNAMELENGHT) {
        username_len = USERNAMELENGHT;
    }
    else if (username_len < USERNAMEMINLENGTH) {
        strcpy(outgoing, "Credenziali errate.");
        signal_num = S_LOGINOK;
    }

    // Assegnazione cursore e puntatore.
    cursor = MAXSIGNALBUF;
    sub_string = &incoming[cursor];

    // Copia username
    strncpy(username, sub_string, username_len);

    // Aggiornamento cursore e svuotamento della stringa temporanea.
    cursor += username_len;
    username[username_len] = '\0';
    //printf("\t\t\tDEBUG_login: username_len %d %d %s %s\n", username_len, cursor, temp_buf, username);
    memset(temp_buf, 0, sizeof(temp_buf));

    // Estrazione della password //
    // printf("\t\tDEBUG_loginSD%d: password\n", sd);
    // Lunghezza della password.
    sub_string = &incoming[cursor];
    strncpy(temp_buf, sub_string, MAXSIGNALBUF);
    temp_buf[MAXSIGNALBUF] = '\0';
    if((pswd_len = atoi(temp_buf)) > PASSWORDLENGHT) {
        pswd_len = PASSWORDLENGHT;
    }

    // Assegnazione cursore e puntatore.
    cursor += MAXSIGNALBUF;
    sub_string = &incoming[cursor];

    // Copia password
    strncpy(password, sub_string, pswd_len);
    password[pswd_len] = '\0';
    //printf("\t\t\tDEBUG_login: pswd_len %d %d %s %s\n", pswd_len, cursor, temp_buf, password);
    */

    // Accesso alla memoria secondaria/database //
    //if (selectQuery(LSO2223, username, password);) {
    if(1){
        //printf("\t\tDEBUG_loginSD%d: 1\n", sd);
        strcpy(outgoing, "Homepage");
        strcpy(player->username, username);
        player->player_socket = sd;
        //printf("\t\tDEBUG_login: nome utente \"%s\" con password \"%s\".\n", username, password);
        signal_num = S_HOMEPAGEOK;
    }
    else {
        //printf("\t\tDEBUG_loginSD%d: 2\n", sd);
        strcpy(outgoing, "Credenziali errate.");
        signal_num = S_LOGINOK;
    }

    //printf("\t\tDEBUG_loginSD%d: ended\n", sd);
    return signal_num;
}

/* Funzione di parsing di username e password dalla stringa d'ingresso. Analizza in maniera sicura la stringa.
 * Restituisce un intero, 0 in caso di successo, S_LOGINERROR in tutti gli altri casi. */
int loginParser(char incoming[], char outgoing[], char username[], char password[]) {
    char *saveptr = NULL;
    char *username_p, *password_p;
    int username_len, pswd_len, return_value;

    return_value = 1;

    username_p = strtok_r(incoming, "-", &saveptr);
    password_p = strtok_r(NULL, ";", &saveptr);

    if(username_p == NULL || password_p == NULL) {
        fprintf(stderr, ":LOGIN ERROR: null username or password arguments detected.\n");
        strcpy(outgoing, "Credenziali errate.");
        return_value = S_LOGINERROR;
    }
    else {
        username_len = strlen(username_p);
        pswd_len = strlen(password_p);

        if (username_len < USERNAMEMINLENGTH || username_len > USERNAMELENGHT
            || pswd_len < PASSWORDMINLENGTH || pswd_len > PASSWORDLENGHT)
        {
            fprintf(stderr, ":LOGIN ERROR: wrong lenght username or password arguments detected.\n");
            strcpy(outgoing, "Credenziali errate.");
            return_value = S_LOGINERROR;
        }
        else {
            strncpy(username, username_p, username_len);
            username[username_len] = '\0';
            strncpy(password, password_p, pswd_len);
            password[pswd_len] = '\0';
            return_value = 0;
        }
    }
    return return_value;
}