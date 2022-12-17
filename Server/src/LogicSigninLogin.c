//
// Created by osboxes on 12/5/22.
//

#include "../include/ListHandler.h"
#include "../include/LogicSigninLogin.h"
#include "../include/Def.h"

void sigin(){

}

void logout(){

}

int findUsername(char incoming[])
{
    char string[50];
    int num=0;
    FILE *in_file = fopen("users.txt", "r");
    if (in_file == NULL)
    {
        printf("Error file missing\n");
        return 0;
    }
    while ( (fscanf(in_file,"%s", string) == 1)&&(num==0))
    {
        if(strstr(string, incoming)!=0) {
            num++;
            printf("Login successful");
        }
    }
    fclose(in_file);
    return 1;
}

int login_old(char incoming[], struct player_node* player, char outgoing[]){
    int signal_num;
    //DEPRECATO //controllo coerenza dati trasmessi dal client, username e password che non utilizzano caratteri non accettati ed appartengono al range accettato 6-32
    //Ritengo più efficiente fare un cotrollo di coerenza dati all'inserimento, piuttosto che fare un parsing della concatenzaione di username e password
    //In questa funzione verrà considerato che nel file, le credenziali di ogni utente, sono conservate come una stringa concatenata di username e password
    if(findUsername(&incoming[1])){
        int i;
        char* username;
        char* userNum;
        memcpy(userNum, incoming, 1);
        i=atoi(userNum);
        memcpy(username, incoming, i);
        username[i] = '\0';
        printf("nome utente %s ", username);
        player=createNewPlayerNode(12, username);
        strcpy(outgoing,"Login successful");
        signal_num=52;
    }
    else{
        strcpy(outgoing,"Login error");
        signal_num=91;
    }
    return signal_num;
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