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

int login(char incoming[], struct player_node* player, char outgoing[]){
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
