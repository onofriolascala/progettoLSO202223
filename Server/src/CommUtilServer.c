//
// Funzioni adibite alla standardizzazione delle comunicazioni tra client e server, ma anche tra room thread e service threads.
//

#include "../include/CommUtilServer.h"

int writeToServer_old(int sock, char msg[], int msgLenght){
    int n;
    if((n = write(sock, msg, msgLenght)) < msgLenght){
        if(n < 0){
            perror(":WRITE ERROR");
            return -1;
        }
    }
    return n;
}

int readFromServer_old(int sock, char msg[], int msgLenght){
    int n;
    if((n = read(sock, msg, msgLenght ) < msgLenght)){
        if(n < 0){
            perror(":READ ERROR");
            return -1;
        }
    }
    return n;
}

/* Scrittura al client con anesso parser per l'impacchettamento dei segnali.
 * Riceve in input il socket descriptor della comunicazione ed il segnale da restituire al client con annessa
 * una stringa non nulla.
 * Restituisce l'esito della comunicazione. 0 per successo, -1 per errore di parsing, -2 per write error. */
int writeToClient(int sd, int signal_num, char outgoing[]) {
    //printf("\t\tDEBUG_SD%d: writing input %s\n", sd, outgoing);
    char finalmessage[MAXCOMMBUFFER];
    int len;

    finalmessage[0] = '\0';

    if(sprintf(finalmessage, "%d:%s", signal_num, outgoing) < 0) {
        perror(":COMPOSITION ERROR");
        return -1;
    }

    len = strlen(finalmessage);
    finalmessage[len] = '\0';

    if(write(sd, finalmessage, len) < 0) {
        perror(":WRITE ERROR");
        return -2;
    }
    //printf("\t\tDEBUG_SD%d: writing end %s\n", sd, finalmessage);
    return 0;
}

/* Lettura dal client con anesso parser per lo spacchettamento dei segnali.
 * Riceve in input il socket descriptor della comunicazione ed la stringa non nulla
 * su cui ricevere i dati, con annessa una lunghezza massima.
 * Restituisce l'esito della comunicazione. Il segnale in ingresso per successo, -3 per errore di lettura. */
int readFromClient(int sd, char incoming[], int max_len){
    //printf("\t\tDEBUG_SD%d: reading input %s\n", sd, incoming);
    int signal_num;
    char signal_code[MAXSIGNALBUF + 1], tmp[MAXSIGNALBUF];

    // Lettura del segnale inviato dal client
    if((read(sd, signal_code, MAXSIGNALBUF)) < 0)
    {
        perror(":SIGNAL CODE READ ERROR");
        return -3;
    }
    signal_code[MAXSIGNALBUF] = '\0';
    //printf("\t\tDEBUG_SD%d: reading code %s\n", sd, signal_code);

    // Rimozione del separatore
    if((read(sd, tmp, 1)) < 0)
    {
        perror(":SEPARATOR READ ERROR");
        return -3;
    }
    tmp[0] = '\0';

    // Lettura del messaggio associato al segnale.
    if((read(sd, incoming, max_len)) < 0)
    {
        perror(":MESSAGE READ ERROR");
        return -3;
    }
    incoming[strlen(incoming)] = '\0';

    signal_num = atoi(signal_code);

    //printf("\t\tDEBUG_SD%d: reading endvalue %d %s\n", sd, signal_num, incoming);
    return signal_num;
}