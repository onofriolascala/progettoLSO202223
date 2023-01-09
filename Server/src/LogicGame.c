//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicGame.h"

int* randomSCT(int [], int);
void pick(char*, int);
void randomWords(char*);

int* randomSCT(int array[], int j){
    char buf[MAXCOMMBUFFER]="";
    int fd,k;
    if((fd=open("/dev/urandom",O_RDONLY))<0){
        perror(":OPEN ERROR:");
    }
    else {
        while (j--) {
            read(fd, buf, 1);
            k = *buf;
            srand(k + j);
            array[j] = rand() % 100;
            if((j==1)&&(array[j]==array[j+1]))
                j++;
            if((j==0)&&((array[j]==array[j+1])||(array[j]==array[j+2])))
                j++;
        }
    }
    return array;
}

void randomWords(char outgoing[]){
    int i=3;
    //char outgoing[BUFFSIZE]="";
    int array[3]={0};
    randomSCT(array,i);
    while (i>0){
        pick(outgoing, array[--i]);
    }
    return;
}

void pick(char buff[],int n){
    int fd;
    char bufChar[2];
    char* newline="\n";
    fflush(stdout);
    //char buff[BUFFSIZE]="";
    if((fd = open("user.txt", O_RDONLY, NULL))<0) {
        perror(":OPEN ERROR:");
    }
    while(n>=0){
        if(n==0){
            read(fd, bufChar, 1);
            bufChar[1]='\0';
            if (bufChar[0] == *newline){
                strcat(buff, bufChar);
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