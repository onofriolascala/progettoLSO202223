//
// Created by osboxes on 12/5/22.
//

#include "../include/LogicSigninLogin.h"

int signin(char incoming[], struct player_node* player, char outgoing[]){
    int signal_num;
    char username[USERNAMELENGHT+1], password[PASSWORDLENGHT+1], temp_buf[MAXSIGNALBUF+1];
    char* sub_string;
    int username_len, pswd_len, cursor;
    strncpy(temp_buf, incoming, MAXSIGNALBUF);
    temp_buf[MAXSIGNALBUF] = '\0';
    username_len = atoi(temp_buf);
    //controlli di correttezza username, implementate richieste di dimensione
    if((username_len<6)||(username_len>32)){
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
            if (1/*insertQuery((struct mySQLConnection* LSO2223, char[] username, char[] password):)*/== 1062) {
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

struct player_node* logout(struct player_node* player){
    int signal_num;
    signal_num=51;
    destroyPlayerNode(player);
    player=NULL;
    return player;
}

int login(int sd, char incoming[], struct player_node* player, char outgoing[]) {
    //printf("\t\tDEBUG_loginSD%d: strated with values %s %s.\n", sd, incoming, outgoing);
    int signal_num;
    char username[USERNAMELENGHT+1], password[PASSWORDLENGHT+1], temp_buf[MAXSIGNALBUF+1];
    char* sub_string;
    int username_len, pswd_len, cursor;

    // Estrazione dello username //
    // printf("\t\tDEBUG_loginSD%d: username\n", sd);
    // Lunghezza dello username.
    strncpy(temp_buf, incoming, MAXSIGNALBUF);
    temp_buf[MAXSIGNALBUF] = '\0';
    username_len = atoi(temp_buf);

    // Assegnazione cursore e puntatore.
    cursor = MAXSIGNALBUF;
    sub_string = &incoming[cursor];

    // Copia username
    strncpy(username, sub_string, username_len);

    // Aggiornamento cursore e svuotamento della string temporanea.
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
    pswd_len = atoi(temp_buf);

    // Assegnazione cursore e puntatore.
    cursor += MAXSIGNALBUF;
    sub_string = &incoming[cursor];

    // Copia password
    strncpy(password, sub_string, pswd_len);
    password[pswd_len] = '\0';

    //printf("\t\t\tDEBUG_login: pswd_len %d %d %s %s\n", pswd_len, cursor, temp_buf, password);

    // Accesso alla memoria secondaria/database //
    if(1){
        //printf("\t\tDEBUG_loginSD%d: 1\n", sd);
        strcpy(outgoing, "Homepage");
        strcpy(player->username, username);
        player->player_socket = sd;
        //printf("\t\tDEBUG_login: nome utente \"%s\" con password \"%s\".\n", username, password);
        signal_num = 52;
    }
    else {
        //printf("\t\tDEBUG_loginSD%d: 2\n", sd);
        strcpy(outgoing, "Credenziali errate.");
        signal_num = 91;
    }

    //printf("\t\tDEBUG_loginSD%d: ended\n", sd);
    return signal_num;
}