//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicGame.h"



int parserChosenWord(char incoming[]){
    char *num_p;
    int return_value = -1;
    num_p=strtok(incoming, "\0");
    if(num_p != NULL) {
        return_value = atoi(num_p);
        if(return_value < 0 || return_value > 3) {
            return_value = -1;
        }
    }
    return return_value;
}
/*
 * Questa funzione prende in ingresso la sequenza di suggerimenti della parola nascosta, lo stato di smascheramento attuale della parola
 * codificato con un intero nella variabile current_unveil, la parola nascosta in gioco al momento, la parola da indovinare e la sua lunghezza.
 * La funzione dunque aggiunge il prossimo suggerimento alla parola nascosta, modificandola e modificando anche il suo stato di smascheramento
 * attuale, questo avviene solo nel caso in cui *current_unveil < word_len-1 negli altri casi non farà nulla.
 * NB: la lunghezza dalla parola nascosta e quello della sequenza di suggerimenti deve essere la stessa
 * */
void addHint(int unveiling_sequence[],int* current_unveil, char hidden_word[], char chosen_word[], int word_len){
    if(*current_unveil < (word_len-1)){
        hidden_word[unveiling_sequence[*current_unveil]] = chosen_word[unveiling_sequence[*current_unveil]];
        *current_unveil = *current_unveil+1;
    }
}
/*
Funzione che genera una sequenza di interi che codificano l'ordine in cui i suggerimenti di una parola
 verranno mostrati, la sequenza in output verrà inserità nell'array sequence, l'array passato
 non può essere nullo e dovrà essere di dimensione >= wordLenght+1
*/
void generateUnveilingSequence(int sequence[], int wordLength){
    randomSCT(sequence,wordLength,wordLength);
}

//::DEBUG::
void playedWord(char word[]){
    int len;
    char blank[]="___________________________________________-";
    len=strlen(word);
    blank[len]='\0';
    printf("%s \n", blank);
    addLetter(word, blank, len);
    return;
}
//::DEBUG::
void addLetter(char word[],char blank[], int len){
    int letter[len];
    randomSCT(letter, len, len);
    for(int i=0; i<len; i++) {
        blank[letter[i]] = word[letter[i]];
        printf("%s \n", blank);
    }
    return;
}
/*
Funzione che riempie l'array (array[]) di j interi con valori unici random compresi nel range passato in input
*/
void randomSCT(int array[], int j, int range){
    char buf[30]="";
    int fd, seed, count, i;
    if((fd=open("/dev/urandom",O_RDONLY))<0){
        perror(":OPEN ERROR:");
    }
    else {
        for(i=0; i<j; i++){
            read(fd, buf, 1);
            seed = *buf;
            srand(seed + i);
            array[i] =  rand() % range;
            count=i;
            while(count>0){
                if(array[i]==array[--count]){
                    i--;
                    count=0;
                }
            }
        }
    }
    close(fd);
    return;
}

/*
Questa funzione si occupa della generazione di tre parole selezionate in modo randomico dal file words.txt, genera tre valori random nel range del numero delle parole presenti nel file words.txt tramite la funzione randomSCT e li estrae dal file tramite la funzione pick.
Infine li inserisce nelle due variabili di output words e outgoing
*/
void generateRandomWords(char words[3][MAXWORDLENGTH]){
    int i=0;
    int array[3]={0};
    randomSCT(array, 3, MAXWORDSINFILE);
    char word[MAXWORDLENGTH];
    for(i=0;i < 3;i++){
        memset(word,'\0',sizeof(word));
        pick(word, array[i]);
        strcpy(words[i],word);
    }
}
/*
Questa funzione si occupa di estrarre la parola n-esima dal file delle parole words.txt, per farlo la funzione prende in input la parola
n-esima da estrarre, legge dal file le prime n parole fino ad arrivare al target e poi lo copia nella variabile di output buff
*/
void pick(char buff[],int n){
    int fd;
    char bufChar[2];
    char* newline="\n";
    fflush(stdout);
    //char buff[BUFFSIZE]="";
    if((fd = open(WORDSFILEPATH, O_RDONLY, NULL))<0) {
        perror(":OPEN ERROR:");
    }
    while(n>=0){
        if(n==0){
            read(fd, bufChar, 1);
            bufChar[1]='\0';
            if (bufChar[0] == *newline){
                //strcat(buff, "-");
                n--;
            }
            else {
                strcat(buff, bufChar);
            }
        }
        else {
            read(fd, bufChar, 1);
            bufChar[1]='\0';
            if (bufChar[0] == *newline){
                n--;
            }
        }
    }
    close(fd);
    return;
}