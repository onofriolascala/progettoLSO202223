//
// Funzioni adibite alla standardizzazione delle comunicazioni tra client e server, ma anche tra room thread e service threads.
//

#include "../include/CommUtilServer.h"

/* Scrittura al client con anesso parser per l'impacchettamento dei segnali.
 * Riceve in input il socket descriptor della comunicazione ed il segnale da restituire al client con annessa
 * una stringa non nulla.
 * Restituisce l'esito della comunicazione. 0 per successo, -1 per errore di parsing, -2 per write error. */
int writeToClient(int sd, int signal_num, char outgoing[]) {
    int len, return_value;
    char finalmessage[MAXCOMMBUFFER];

    return_value = 0;
    finalmessage[0] = '\0';

    if (signal_num < 10) {
        if (snprintf(finalmessage, MAXCOMMBUFFER, "0%d:%s;", signal_num, outgoing) < 0) {
            perror(":COMPOSITION ERROR");
            return -1;
        }
    }
    else {
        if (snprintf(finalmessage, MAXCOMMBUFFER, "%d:%s;", signal_num, outgoing) < 0) {
            perror(":COMPOSITION ERROR");
            return -1;
        }
    }

    len = strlen(finalmessage);

    if(len >= (MAXCOMMBUFFER - 2)) {
        finalmessage[MAXCOMMBUFFER - 2] = ';';
        finalmessage[MAXCOMMBUFFER - 1] = '\0';
        if(write(sd, finalmessage, MAXCOMMBUFFER) < 0) {
            perror(":WRITE ERROR");
            return_value = -2;
        }
    }
    else {
        if(write(sd, finalmessage, len) < 0) {
            perror(":WRITE ERROR");
            return_value = -2;
        }
    }
    printf("\t\tDEBUG_SD%d: writing end %s\n", sd, finalmessage);
    fflush(stdout);
    return return_value;
}

/* Lettura dal client con anesso parser per lo spacchettamento dei segnali.
 * Riceve in input il socket descriptor della comunicazione ed la stringa non nulla
 * su cui ricevere i dati, con annessa una lunghezza massima.
 * Restituisce l'esito della comunicazione. Il segnale in ingresso per successo, -3 per errore di lettura. */
int readFromClient(int sd, char incoming[], int max_len){
    int signal_num, signal_len, message_len, n_tmp;
    char *saveptr;
    char *signal_p, *message_p;
    char incoming_buf[MAXCOMMBUFFER], signal_code[MAXSIGNALBUF+1];

    saveptr = NULL;
    signal_num = S_COMMERROR;

    if((n_tmp = read(sd, incoming_buf, MAXCOMMBUFFER)) < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror(":READ ERROR");
            return -3;
        }
        return -1;
    }
    else if(n_tmp == 0) {
        fprintf(stderr, ":READ ERROR: Client socket has closed unexpectedly. Closing Service Thread.\n");
        return 0;
    }

    signal_p = strtok_r(incoming_buf, ":", &saveptr);
    message_p = strtok_r(NULL, ";", &saveptr);

    if(signal_p != NULL && message_p != NULL) {
        signal_len = strlen(signal_p);
        message_len = strlen(message_p);

        if (signal_len == MAXSIGNALBUF && message_len < (MAXCOMMBUFFER - 4))
        {
            strncpy(signal_code, signal_p, signal_len);
            signal_code[signal_len] = '\0';
            strncpy(incoming, message_p, message_len);
            incoming[message_len] = '\0';

            signal_num = atoi(signal_code);
        }
        else {
            fprintf(stderr, ":READ ERROR: wrong length for signal or message arguments detected.\n");
        }
    }
    else {
        if(signal_p != NULL) {
            strncpy(signal_code, signal_p, signal_len);
            signal_code[signal_len] = '\0';
            signal_num = atoi(signal_code);
        }
        fprintf(stderr, ":READ ERROR: null signal or message arguments detected.\n");
    }

    printf("\t\tDEBUG_SD%d: reading endvalue %d %s\n", sd, signal_num, incoming);
    fflush(stdout);
    return signal_num;
}