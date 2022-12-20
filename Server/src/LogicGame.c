//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicGame.h"



char* randomWords(char outgoing[]){
    int i=3;
    int array[3]={0};
    randomSCT(array,3);
    while(i--){

    }
    return outgoing;
}

int* randomSCT(int array[], int j){
    char buf[3]="";
    int fd,k;
    j=3;
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
            if((j==0)&&(array[j]==array[j+1])&&(array[j]==array[j+2]))
                j++;
        }
    }
    return array;
}