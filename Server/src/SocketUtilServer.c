//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#define MAXCONNECTIONS 5

// Funzione adibita all'inizializzazione della socket primaria d'ascolto.
int socketInit(struct sockaddr_in* server_addr,int len){
    int sd1;
    // Apertura del socket lato server.
    if ((sd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        //exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd1, (struct sockaddr*)&ip_addr, sizeof(ip_addr)) < 0){
        perror(":BIND ERROR");
        close(sd1);
        //exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd1, MAXCONNECTIONS) < 0) {
        perror(":LISTEN ERROR");
        close(sd1);
        //exit(1);
    }
    return sd1;
}

// Funzione contenente il while infinito con annesso ascolto passivo.
void acceptLoop(int sd1) {
    return NULL;
}