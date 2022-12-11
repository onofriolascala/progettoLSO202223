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
    char temp[100];
};

// Funzione principale.
void* thrService(void* arg) {
    void* out;
    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];
    int signal_num, out_len, sd;

    struct service_arg lmao = *(struct service_arg*)arg;
    sd = lmao.sd;
    printf("%d:%s\n", lmao.sd, lmao.temp);

    signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);
    printf("%d:%s\n", signal_num, incoming);
    //strcpy(incoming, "");
    /*int count = 5;
    while(count--) {

        switch
    }*/

    sleep(2);
    close(sd);
    printf("DEBUG: thread ended.\n");
    fflush(stdout);
    return out;
}

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2) {
    pthread_t tid;
    struct service_arg args;
    args.sd = sd2;
    printf("%d\n", sd2);
    strcpy(args.temp, "ciao\n");
    if (pthread_create(&tid, NULL, thrService, &args))
    {
        printf(":THREAD CREATION ERROR:\n");
        close(sd2);
        pthread_cancel(pthread_self());
    }
    printf("DEBUG: thread created.\n");
    fflush(stdout);
    return tid;
}