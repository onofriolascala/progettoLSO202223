/*
 * Main del Server. Responsabile di inizializzare il processo server e di gestire l'inizializzazione dei socket
 * e dei thread adibiti alla gestione delle comunicazioni con i singoli client.
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
//#include <mysql/mysql.h>

#include "../include/SocketUtilServer.h"
#include "../include/ThreadService.h"
#include "../include/Def.h"
//#include "../include/ThreadRoom.h"
#include "../include/ListHandler.h"
#include <signal.h>

static void sigHandler(int signum);

int main() {
    int sd1, sd2;
    struct room_node** room_list;
    struct room_node* room_head;
    struct sockaddr_in server_addr;
    //struct mySQLConncection* LSO2223;
    socklen_t len;

    // Loop di inizializzazione della connessione al DB_server.
    //LSO2223 = establishDBConnection();

    room_head = NULL;
    room_list = &room_head;

    // Inizializzazione del signal handler
    if(signal(SIGINT, sigHandler) == SIG_ERR){
        printf("Errore nella creazione del sigHandler, impossibile intercettare SIGINT\n");
    }
    if(signal(SIGTERM, sigHandler) == SIG_ERR){
        printf("Errore nella creazione del sigHandler, impossibile intercettare SIGTERM\n");
    }
    if(signal(SIGKILL, sigHandler) == SIG_ERR){
        printf("Errore nella creazione del sigHandler, impossibile intercettare SIGKILL\n");
    }

    // Inizializzazione della socket primaria d'ascolto: socket, bind e listen.
    sd1 = socketInit(&server_addr, &len);

    // Loop di accettazione di nuove chiamate in entrata.
    acceptLoop(sd1, room_list);

    close(sd1);
    printf("Terminazione processo server.\n");
    return 0;
}

static void sigHandler(int signum){
    if(signum == SIGINT || signum == SIGTERM || signum == SIGKILL){
        //closing routine
        system("rm /tmp/thrRoom_socket_local_*");
        exit(1);
    }
}