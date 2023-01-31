//
// Created by osboxes on 12/5/22.
//

#include "../include/LogicSigninLogin.h"

/* Funzione di signin. Riceve due stringhe non vuote ed esegue una query di inserimento all'interno del database MySQL.
 * Restituisce un segnale a seconda dell'esito. S_LOGINOK in caso di successo, S_LOGINERROR per input scorretto,
 * S_SIGNINERROR in caso di uno username già esistente. */
int signin(char incoming[], char username[], char outgoing[]){
    char password[PASSWORDLENGTH+1], temp_buf[MAXSIGNALBUF+1];
    char *sub_string;
    int signal_num, username_len, pswd_len, cursor, query_result;

    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    // insertQuery((struct mySQLConnection* LSO2223, char[] username, char[] password):)
    if (1 == 1062) {
        strcpy(outgoing, "Utente già esistente.");
        strcpy(username, "\0");
        signal_num = 91;
    } else {
        strcpy(outgoing, "Utente registrato.");
        signal_num = 51;
    }

    /*strncpy(temp_buf, incoming, MAXSIGNALBUF);
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
    }*/

    return signal_num;
}

/* Funzione di login. Interroga il database MySQL alla ricerca di un utente con stesso username e password.
 * Riceve un socket descriptor, due stringhe non nulle ed una puntatore ad un giocatore. In caso di successo
 * della query assegnerà i valori username e socket alla struttura giocatore.
 * Restituisce un segnale, S_HOMEPAGEOK in caso di successo, S_LOGINERROR in tutti gli altri casi. */
int login(int sd, char incoming[], char username[], char outgoing[]) {
    //printf("\t\tDEBUG_loginSD%d: strated with values %s %s.\n", sd, incoming, outgoing);
    char password[PASSWORDLENGTH+1];
    char *sub_string;
    int signal_num, query_result;

    // Il parser verificherà che il contenuto della stringa in input sia corretto
    if((signal_num = loginParser(incoming, outgoing, username, password)) != 0) {
        return signal_num;
    }

    // Accesso alla memoria secondaria/database //
    //if (selectQuery(LSO2223, username, password);) {
    if(1){
        //printf("\t\tDEBUG_loginSD%d: 1\n", sd);
        strcpy(outgoing, S_HOMEPAGEOK_MSG);
        //printf("\t\tDEBUG_login: nome utente \"%s\" con password \"%s\".\n", username, password);
        signal_num = S_HOMEPAGEOK;
    }
    else {
        //printf("\t\tDEBUG_loginSD%d: 2\n", sd);
        strcpy(username, "\0");
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
            strcpy(outgoing, "Credenziali errate.");
            return_value = S_LOGINERROR;
        }
    }
    else {
        fprintf(stderr, ":LOGIN ERROR: null username or password arguments detected.\n");
        strcpy(outgoing, "Credenziali errate.");
        return_value = S_LOGINERROR;
    }
    return return_value;
}