//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#include "../include/SocketUtilServer.h"
#include "../include/Def.h"

// Funzione adibita all'inizializzazione della socket primaria d'ascolto.
int socketInit(struct sockaddr_in* server_addr, socklen_t* len) {
    int sd1;

    (*server_addr).sin_family = AF_INET;
    (*server_addr).sin_port = htons(5200);
    (*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);
    *len = sizeof(*server_addr);

    // Apertura del socket lato server.
    if ((sd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        //exit(1);
    }
    // Setup delle impostazioni del socket
    if(setsockopt(sd1, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror(":SOCKET OPTION ERROR");
        exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd1, (struct sockaddr*)server_addr, *len) < 0){
        perror(":BIND ERROR");
        close(sd1);
        exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd1, MAXCONNECTIONS) < 0) {
        perror(":LISTEN ERROR");
        close(sd1);
        exit(1);
    }
    printf("MAIN: SocketInit completed.\n");
    fflush(stdout);
    return sd1;
}

int localSocketInit(int ID, char local_path[], struct sockaddr_un* localsocket_addr, socklen_t* len) {
    int sd;

    (*localsocket_addr).sun_family = PF_LOCAL;
    strcpy((*localsocket_addr).sun_path, local_path);
    *len = sizeof(*localsocket_addr);

    // Apertura del socket lato server.
    if ((sd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        //exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd, (struct sockaddr*)localsocket_addr, *len) < 0){
        perror(":BIND ERROR");
        close(sd);
        //exit(1);
    }
    // Setup delle impostazioni del socket
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror(":SOCKET OPTION ERROR");
        //exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd, MAXCONNECTIONS) < 0) {
        perror(":LISTEN ERROR");
        close(sd);
        //exit(1);
    }
    printf("\t\t\t\tROOM_ID%d: localSocketInit completed.\n", ID);
    fflush(stdout);
    return sd;
}

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1, struct room_node** room_list) {
    int sd2;
    int count = 5;
    while(count--){
        // Loop di Accettazione
        if((sd2 = accept(sd1, NULL, NULL)) < 0) {
            perror(":ACCEPT ERROR");
            //close(sd1);
        }
        else {
            createNewService(sd2, room_list);
        }
        printf("MAIN: Accept loop restarting...\n");
        fflush(stdout);
    }
}