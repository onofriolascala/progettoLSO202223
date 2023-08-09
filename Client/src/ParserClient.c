
#include "../include/ParserClient.h"

int parserIp(char incoming[], struct server_connection *server){
    //Prende incoming come input, restituisce una stringa con il valore dell'ip e 0 in caso di successo, un -1 in caso di dimensioni errate, -2 in caso di caratteri alfanumerici nell'ip
    //Data la tipologia di controlli fatti non è possibile accettare degli alias come ip
    char input[MAXCOMMBUFFER] = "";
    strcpy(input, incoming);

    char temp[MAXIP+1]="";
    temp[MAXIP] = '\0';
    char* ip;
    char* saveptr;
    int count=2;
    int len;
    ip = strtok_r(input, ".", &saveptr);
    if(ip == NULL || ip[0] == 0) return -1;
    len=strlen(ip);
    if ((len > 3) || (len < 1)) {
        return -1;
    }
    strncpy(temp, ip, len);
    while(count){
        ip = strtok_r(NULL, ".", &saveptr);
        if(ip == NULL || ip[0] == 0) return -1;
        len=strlen(ip);
        if ((len > 3) || (len < 1)) {
            return -1;
        }
        else{
            strncat(temp, ".", 2);
            strncat(temp, ip, len);
        }
        count--;
    }
    ip = strtok_r(NULL, "-", &saveptr);
    if(ip == NULL || ip[0] == 0) return -1;
    len=strlen(ip);
    if ((len > 3) || (len < 1)) {
        return -1;
    }
    else{
        strncat(temp, ".", 2);
        strncat(temp, ip, len);
    }
    len=strlen(temp);
    for (int i=0; i<len; i++) {
        if((temp[i]<'0' || temp[i]>'9') && (temp[i]!='.')){
            return -2;
        }
    }
    //strncat(ip, temp, len);
    //printf("%s", temp);
    strcpy(server->ip, temp);
    return 0;
}

int parserPort(char incoming[],struct server_connection *server){
    //Prende incoming come input, restituisce una stringa con il valore della porta e 0 in caso di successo, un -1 in caso di dimensioni errate, -2 in caso di caratteri alfanumerici nella porta
    char input[MAXCOMMBUFFER];
    strcpy(input, incoming);

    char temp[MAXPORT+1];
    temp[MAXPORT] = '\0';
    char* pr;
    char* saveptr;
    int len;
    strtok_r(incoming, "-", &saveptr);
    pr = strtok_r(NULL, ";", &saveptr);
    if(pr == NULL || pr[0] == 0) return -1;
    len=strlen(pr);
    if ((len > MAXPORT) || (len < MINPORT)) {
        return -1;
    }
    strcpy(temp, pr);
    for (int i=0; i<len; i++) {
        if(temp[i]<'0' || temp[i]>'9'){
            return -2;
        }
    }
    //printf("%s", temp);
    server->port = atoi(temp);
    return 0;
}

int parserPassword(char incoming[]){
    //Prende incoming come input, restituisce uno 0 in caso di password valida, un -1 in caso di dimensioni errate, -2 in caso di caratteri non accettati
    char input[MAXCOMMBUFFER];
    strcpy(input, incoming);

    char temp[PASSWORDLENGTH+1];
    temp[PASSWORDLENGTH] = '\0';
    char* psw;
    char* saveptr;
    int len;
    strtok_r(input, "-", &saveptr);
    psw = strtok_r(NULL, "\0", &saveptr);
    if(psw == NULL || psw[0] == 0) return -1;
    len=strlen(psw);
    if ((len < PASSWORDMINLENGTH) || (len > PASSWORDLENGTH)) {
        return -1;
    }
    strcpy(temp, psw);
    for (int i=0; i<len; i++) {
        if(temp[i]<'!' || temp[i]>'z'){
            return -2;
        }
    }
    //printf("%s", temp);
    return 0;
}

int parserUsername(char incoming[], struct server_connection *server) {
    //Prende incoming come input, restituisce uno 0 in caso di username valido, un -1 in caso di dimensioni errate, -2 in caso di caratteri non accettati
    char input[MAXCOMMBUFFER];
    strcpy(input, incoming);

    char temp[USERNAMELENGTH+1];
    temp[USERNAMELENGTH] = '\0';
    char *user;
    char *saveptr;
    int len;
    user = strtok_r(input, "-", &saveptr);
    if(user == NULL || user[0] == 0) return -1;
    len = strlen(user);
    if ((len < USERNAMEMINLENGTH) || (len > USERNAMELENGTH)) {
        return -1;
    }
    strcpy(temp, user);
    for (int i = 0; i < len; i++) {
        if (temp[i] < '!' || temp[i] > 'z') {
            return -2;
        }
    }
    //printf("%s", temp);
    strncpy(server->connected_user, temp, USERNAMELENGTH);
    server->connected_user[USERNAMELENGTH] = '\0';
    return 0;
}

// Parser della lista delle stanze. Opera in modo da rimanere in attesa delle stanze da scrivere qualora eccedano il buffer
// Riceve in ingresso il messaggio in entrata, il contatore delle colonne scritte ed il puntatore del buffer di stampa
// Restituisce 0 in caso di successo, -1 in caso di arrey mal inizializzati, -2 in caso di messaggio in input vuoto,
// 1 in caso di attesa di una nuova comunicazione
// per indicare l'attuale posizione del cursore delle colonne
int parserList(char incoming[], char print_buffer[]) {
    int return_value, column_position;
    char *saveptr;
    char *room_p;
    char temp_buf[MAXCOMMBUFFER];

    saveptr = NULL;
    return_value = 0;
    column_position = 0;

    if (incoming == NULL || print_buffer == NULL) {
        return_value = -1;
    }
    else {
        strcpy(temp_buf, incoming);
        for (room_p = strtok_r(temp_buf, "-", &saveptr);
             room_p != NULL;
             room_p = strtok_r(NULL, "-", &saveptr))
        {
            if(room_p[0] == '#') {
                return_value = 1;
                break;
            }
            else {
                if(isdigit(room_p[0])) strcat(print_buffer, "Stanza_");
                strcat(print_buffer, room_p);
                if(column_position <= MAXLISTCOLUMNS) {
                    strcat(print_buffer, "\t\t");
                    column_position++;
                }
                else {
                    strcat(print_buffer, "\n");
                    column_position = 1;
                }
            }
        }
    }

    return return_value;
}

int parserRoomJoin(struct room_struct *room, char incoming[]) {
    if(incoming == NULL) {
        return -1;
    }

    int return_value, count;
    char *saveptr;
    char *roomid_p;
    char *playernum_p;
    char *suzerain_p;
    char *players_p;
    char *player_p;
    char *word_p;
    char temp_buf[MAXCOMMBUFFER];

    saveptr = NULL;
    return_value = 0;
    count = 0;

    roomid_p = strtok_r(incoming, "-", &saveptr);
    playernum_p = strtok_r(NULL, "-", &saveptr);
    suzerain_p = strtok_r(NULL, "-", &saveptr);
    players_p = strtok_r(NULL, "-", &saveptr);
    word_p = strtok_r(NULL, "-", &saveptr);

    count = atoi(playernum_p);
    if (count < room->player_num) {
        return_value = -1;
    }
    else if (count > room->player_num) {
        return_value = 1;
    }
    else {
        return_value = 0;
    }
    room->ID = atoi(roomid_p);
    room->player_num = count;
    strcpy(room->suzerain, suzerain_p);
    strcpy(room->secret_word, word_p);

    strcpy(temp_buf, players_p);

    count = 0;
    player_p = strtok_r(temp_buf, ",", &saveptr);
    for (count = 0; count < MAXPLAYERS; count++)
    {
        if(player_p == NULL) {
            sprintf(room->players[count], BLK"%s"DFT, "Vuoto");
            room->players[count][USERNAMELENGTH] = '\0';
        }
        else {
            sprintf(room->players[count], "%s", player_p);
            room->players[count][USERNAMELENGTH] = '\0';
        }
        player_p = strtok_r(NULL, ",", &saveptr);
    }

    strcpy(incoming, players_p);

    return return_value;
}

// Parser dell'array dei giocatori. Opera in modo da rimanere in attesa delle stanze da scrivere qualora eccedano il buffer
// Riceve in ingresso il messaggio in entrata, il contatore delle colonne scritte ed il puntatore del buffer di stampa
// Restituisce 0 in caso di successo, -1 in caso di arrey mal inizializzati, -2 in caso di messaggio in input vuoto,
// 1 in caso di attesa di una nuova comunicazione
// per indicare l'attuale posizione del cursore delle colonne
int parserPlayers(struct room_struct *room, char incoming[]) {
    int return_value, count;
    char *saveptr;
    char *player_p;
    char temp_buf[MAXCOMMBUFFER];

    saveptr = NULL;
    return_value = 0;
    count = 0;

    if (incoming == NULL) {
        return_value = -1;
    }
    else {
        strcpy(temp_buf, incoming);
        for (player_p = strtok_r(temp_buf, ",", &saveptr);
             player_p != NULL;
             player_p = strtok_r(NULL, ",", &saveptr))
        {
            if(count < MAXPLAYERS) {
                strcpy(room->players[count], player_p);
                room->players[count][USERNAMELENGTH] = '\0';
                count++;
            }
        }
    }

    return return_value;
}