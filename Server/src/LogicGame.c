//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicGame.h"



void parserChosenWord(char words[3][20], char incoming[]){
    char* num;
    int con;
    num=strtok(incoming, "-");
    con=atoi(num);
    playedWord(words[con]);
    return;
}

void playedWord(char word[]){
    int len;
    char blank[]="___________________";
    len=strlen(word);
    blank[len]='\0';
    printf("%s \n", blank);
    addLetter(word, blank, len);
    return;
}

void addLetter(char word[],char blank[],int len){
    int letter[len];
    randomSCT(letter, len, len);
    for(int i=0; i<len; i++) {
        blank[letter[i]] = word[letter[i]];
        printf("%s \n", blank);
    }
    return;
}

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

void randomWords(char words[3][20], char outgoing[]){
    int i=0;
    int array[3]={0};
    randomSCT(array, 3, 100);
    char word[300];
    while (i<3){
        strcpy(word, "");
        pick(word, array[i]);
        strcpy(words[i],word);
        strcat(outgoing, word);
        i++;
        if(i<3){
            strcat(outgoing, "-");
        }
        else{
            strcat(outgoing, "\0");
        }
    }
    return;
}

void pick(char buff[],int n){
    int fd;
    char bufChar[2];
    char* newline="\n";
    fflush(stdout);
    //char buff[BUFFSIZE]="";
    if((fd = open("words.txt", O_RDONLY, NULL))<0) {
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
