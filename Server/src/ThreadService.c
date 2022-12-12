/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/ThreadService.h"
#include "../include/CommUtilServer.h"

#define MAXCOMMBUFFER 1024

struct service_arg {
    int sd;
    char* string;
    int flag;
};

// Funzione principale.
void* thrService(void* arg) {
    /*printf("DEBUG: Thread started...\n");
    fflush(stdout);*/

    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];
    int signal_num, out_len, sd;

    // Copia i valori della struttura originale.
    struct service_arg temparg = *((struct service_arg*)arg);
    sd = temparg.sd;
    strncpy(incoming, temparg.string, MAXCOMMBUFFER);
    // La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct service_arg*)arg).flag = 1;
    printf("\t\tSERVICE_SD%d: initialized with string value \"%s\".\n", sd, incoming);
    fflush(stdout);

    //strcpy(incoming, "");

    int count = 5;
    while(count--) {
        signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);
        printf("\t\tSERVICE_SD%d: %d comunicazioni prima della chiusura.\n", sd, count);
        switch(signal_num)
        {
            case -1:
                break;
            case -2:
                break;
            case -3:
                break;
            case 0:
                printf("\t\t\t<Disconnesione> %d:%s\n", signal_num, incoming);
                break;
            case 10:
                printf("\t\t\t<Login> %d:%s\n", signal_num, incoming);
                break;
            case 11:
                printf("\t\t\t<Registrazione> %d:%s\n", signal_num, incoming);
                break;
            case 14:
                printf("\t\t\t<DEBUG> %d:%s\n", signal_num, incoming);
                break;
            case 20:
                printf("\t\t\t<Crea stanza> %d:%s\n", signal_num, incoming);
                break;
            case 21:
                printf("\t\t\t<Entra stanza> %d:%s\n", signal_num, incoming);
                break;
            case 22:
                printf("\t\t\t<Lista stanze> %d:%s\n", signal_num, incoming);
                break;
            case 23:
                printf("\t\t\t<Logout> %d:%s\n", signal_num, incoming);
                break;
            default:
                printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
        }
        fflush(stdout);
    }

    sleep(2);
    close(sd);
    printf("\t\tSERVICE_SD%d: service thread has ended.\n", sd);
    fflush(stdout);
    return 0;
}

// Creazione del thread in stato detached.
pthread_t createNewService(int sd2) {
    /*printf("DEBUG: Input for createNewService sd:%d.\n",sd2);
    fflush(stdout);*/
    int flag;
    pthread_t tid;
    char buf[100] = "Argomento";

    struct service_arg args;
    args.sd = sd2;
    args.string = buf;
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrService, &args)) {
        printf(":THREAD CREATION ERROR:\n");
        close(sd2);
        pthread_cancel(pthread_self());
        return 0;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il main terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewService. */
    while(args.flag == 0);

    printf("MAIN: Service thread created with sd:%d.\n", sd2);
    fflush(stdout);
    return tid;
}