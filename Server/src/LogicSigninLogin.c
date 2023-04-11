//
// Created by osboxes on 12/5/22.
//

#include "../include/LogicSigninLogin.h"

/* Funzione di signin. Riceve due stringhe non vuote ed esegue una query di inserimento all'interno del database MySQL.
 * Restituisce un segnale a seconda dell'esito. S_LOGINOK in caso di successo, S_LOGINERROR per input scorretto,
 * S_SIGNINERROR in caso di uno username già esistente. */
int signin(char incoming[], char username[], char outgoing[], struct mySQLConnection* db_connection){
    char password[PASSWORDLENGTH+1],;
    int signal_num, query_result;

    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    query_result = insertQuery(db_connection, username, password);
    if (query_result == 1) {
        strcpy(outgoing, "Utente registrato con successo.");
        signal_num = S_LOGINOK;
    }
    else if (query_result == 0) {
        strcpy(username, "\0");
        strcpy(outgoing, S_SIGNINERROR_MSG);
        signal_num = S_SIGNINERROR;
    }
    else {
        strcpy(username, "\0");
        strcpy(outgoing, "Si è verificato un errore a livello database. Contattare un amministratore.");
        signal_num = S_SIGNINERROR;
    }

    return signal_num;
}

/* Funzione di login. Interroga il database MySQL alla ricerca di un utente con stesso username e password.
 * Riceve un socket descriptor, due stringhe non nulle ed una puntatore ad un giocatore. In caso di successo
 * della query assegnerà i valori username e socket alla struttura giocatore.
 * Restituisce un segnale, S_HOMEPAGEOK in caso di successo, S_LOGINERROR in tutti gli altri casi. */
int login(int sd, char incoming[], char username[], char outgoing[], struct mySQLConnection* db_connection) {
    char password[PASSWORDLENGTH+1];
    int signal_num, query_result;

    // Il parser verificherà che il contenuto della stringa in input sia corretto
    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    // Accesso alla memoria secondaria/database //
    query_result = selectQuery(db_connection, username, password);
    if (query_result == 1) {
        strcpy(outgoing, S_HOMEPAGEOK_MSG);
        signal_num = S_HOMEPAGEOK;
    }
    else if (query_result == 0) {
        strcpy(username, "\0");
        strcpy(outgoing, S_LOGINERROR_MSG);
        signal_num = S_LOGINERROR;
    }
    else {
        strcpy(username, "\0");
        strcpy(outgoing, "Si è verificato un errore a livello database. Contattare un amministratore.");
        signal_num = S_LOGINERROR;
    }

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
    password_p = strtok_r(NULL, "\0", &saveptr);

    if(username_p != NULL && password_p != NULL) {
        username_len = strlen(username_p);
        pswd_len = strlen(password_p);

        if (username_len >= USERNAMEMINLENGTH && username_len < USERNAMELENGTH
            && pswd_len >= PASSWORDMINLENGTH && pswd_len <= PASSWORDLENGTH)
        {
            strncpy(username, username_p, username_len);
            username[username_len] = '\0';
            strncpy(password, password_p, pswd_len);
            password[pswd_len] = '\0';
            return_value = 0;
        }
        else {
            fprintf(stderr, ":LOGIN ERROR: wrong length username or password arguments detected.\n");
            strcpy(outgoing, S_LOGINERROR_MSG);
            return_value = S_LOGINERROR;
        }
    }
    else {
        fprintf(stderr, ":LOGIN ERROR: null username or password arguments detected.\n");
        strcpy(outgoing, S_LOGINERROR_MSG);
        return_value = S_LOGINERROR;
    }
    return return_value;
}