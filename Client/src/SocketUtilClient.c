//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#include "../include/SocketUtilClient.h"

// Funzione di inizializzazione della connessione al server. Riceve l'indirizzo e la porta a cui si desidera
// connettersi e restituisce il valore del socket descriptor su cui è stata stabilita la connessione.
int socketInit(struct sockaddr_in *addr, socklen_t *len, char ip[], int port){
    int sd1;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_aton(ip, &addr->sin_addr);
    *len = sizeof(*addr);

    if((sd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        exit(1);
    }
    if(connect(sd1, (struct sockaddr*)addr, *len) < 0) {
        perror(":CONNECT ERROR");
        close(sd1);
        return 0;
    }

    printf("DEBUG: socketInit completed.\n");
    fflush(stdout);
    return sd1;
}

// Funzione di inizializzazione della connessione locale con il thread del PROMPT. Riceve un indirizzo del
// file system (una costante), e restituisce la listening socket della connessione.
int localSocketInit(struct sockaddr_un *localsocket_addr, socklen_t *len) {
    //printf("DEBUG: localSocketInit start.\n");
    int sd;

    (*localsocket_addr).sun_family = PF_LOCAL;
    strcpy((*localsocket_addr).sun_path, CLIENTLOCALSOCKET);
    *len = sizeof(*localsocket_addr);

    if(unlink(CLIENTLOCALSOCKET) < 0) {
        if (errno != ENOENT)
        {
            perror(":UNLINK ERROR");
        }
    }
    // Apertura del socket lato threadRoom.
    if ((sd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror(":SOCKET ERROR");
        exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd, (struct sockaddr*)localsocket_addr, *len) < 0){
        perror(":BIND ERROR");
        close(sd);
        exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd, 1) < 0) {
        perror(":LISTEN ERROR");
        close(sd);
        unlink(CLIENTLOCALSOCKET);
        exit(1);
    }
    printf("MAIN: localSocketInit completed with value \"%d\".\n", sd);
    fflush(stdout);
    return sd;
}

// Elimina in sicurezza la socket locale al momento della chiusura del main thread. Riceve una socket ed un
// indirizzo di memoria su cui effettuare il CLOSE() e l'UNLINK().
void deleteLocalSocket(int localsocket, char localsocket_path[]) {
    //if(!(strcmp(localsocket_path, ""))) {
    close(localsocket);
    unlink(localsocket_path);
    //}
    //else {
    //    fprintf(stderr, ":DELETE LOCALSOCKET: input string must be not be empty.\n");
    //}
}