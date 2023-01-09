#include "../include/ParserClient.h"

char* parserIp(char* incoming[]){
    //Prende incoming come input, restituisce una stringa con il valore dell'ip in caso di successo, un -1 in caso di dimensioni errate, -2 in casi di caratteri alfanumerici nell'ip
    //Data la tipologia di controlli fatti non è possibile accettare degli alias come ip
    char temp[MAXIP]="";
    char* ip;
    char* saveptr;
    int count=2;
    int len;
    ip = strtok_r(incoming, ".", &saveptr);
    len=strlen(ip);
    if ((len > 3) || (len < 1)) {
        return -1;
    }
    strncat(temp, ip, len);
    while(count){
        ip = strtok_r(NULL, ".", &saveptr);
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
    len=strlen(ip);
    if (len > 3) {
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
    strncat(ip, temp, len);
    //printf("%s", temp);
    return ip;
}

char* parserPort(char incoming[]){
    //Prende incoming come input, restituisce una stringa con il valore della porta in caso di successo, un -1 in caso di dimensioni errate, -2 in casi di caratteri alfanumerici nella porta
    char temp[MAXPORT];
    char* pr;
    char* saveptr;
    int len;
    strtok_r(incoming, "-", &saveptr);
    pr = strtok_r(NULL, "\0", &saveptr);
    len=strlen(pr);
    if ((len > 4)&&(len < 1)) {
        return -1;
    }
    strcpy(temp, pr);
    for (int i=0; i<len; i++) {
        if(temp[i]<'0' || temp[i]>'9'){
            return -2;
        }
    }
    //printf("%s", temp);
    return pr;
}

char* parserPassword(char incoming[]){
    char temp[PASSWORDLENGTH];
    char* psw;
    char* saveptr;
    int len;
    int uppercaseCount=0;
    int specialCharCount=0;
    strtok_r(incoming, "-", &saveptr);
    psw = strtok_r(NULL, "\0", &saveptr);
    len=strlen(psw);
    if ((len < PASSWORDMINLENGTH) && (len > PASSWORDLENGTH)) {
        return -1;
    }
    strcpy(temp, psw);
    for (int i=0; i<len; i++) {
        if(temp[i]<'!' || temp[i]>'z'){
            return -2;
        }
    }
    printf("%s", temp);
    return 0;
}

char* parserUsername(char incoming[]){
    char temp[USERNAMELENGTH];
    char* user;
    char* saveptr;
    int len;
    int uppercaseCount=0;
    int specialCharCount=0;
    strtok_r(incoming, "-", &saveptr);
    user = strtok_r(NULL, "\0", &saveptr);
    len=strlen(user);
    if ((len < USERNAMEMINLENGTH) && (len > USERNAMELENGTH)) {
        return -1;
    }
    strcpy(temp, user);
    for (int i=0; i<len; i++) {
        if(temp[i]<'!' || temp[i]>'z'){
            return -2;
        }
    }
    printf("%s", temp);
    return 0;
}