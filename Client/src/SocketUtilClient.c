//
// Funzioni per la gestione dei socket da parte dell'applicazione lato server.
//

#include "../include/SocketUtilClient.h"

// Funzione di inizializzazione della connessione al server. Riceve l'indirizzo e la porta a cui si desidera
// connettersi e restituisce il valore del socket descriptor su cui è stata stabilita la connessione.
int socketInit(struct sockaddr_in *addr, socklen_t *len, char ip[], int port){
    int sd;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_aton(ip, &addr->sin_addr);
    *len = sizeof(*addr);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        //perror(":SOCKET ERROR");
        //exit(1);
        sd = -errno;
        return sd;
    }
    if(connect(sd, (struct sockaddr*)addr, *len) < 0) {
        //perror(":CONNECT ERROR");
        close(sd);
        sd = -errno;
    }
    //printf("DEBUG: socketInit completed.\n");
    //fflush(stdout);
    return sd;
}

// Funzione di inizializzazione della connessione locale con il thread del PROMPT. Riceve un indirizzo del
// file system (una costante), e restituisce la listening socket della connessione.
int localSocketInit(struct sockaddr_un *localsocket_addr, socklen_t *len, struct prompt_thread *prompt) {
    //printf("DEBUG: localSocketInit start.\n");
    int sd;

    pid_t pid = getpid();

    sprintf(prompt->localsocket_path, "%s%d", CLIENTLOCALSOCKET, pid);

    (*localsocket_addr).sun_family = PF_LOCAL;
    strcpy((*localsocket_addr).sun_path, prompt->localsocket_path);
    *len = sizeof(*localsocket_addr);

    if(unlink(prompt->localsocket_path) < 0) {
        if (errno != ENOENT)
        {
            //perror(":UNLINK ERROR");
        }
    }
    // Apertura del socket lato threadRoom.
    if ((sd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0){
        //perror(":SOCKET ERROR");
        sd = -errno;
        return sd;
        //exit(1);
    }
    // Binding dell'indirizzo al socket.
    if(bind(sd, (struct sockaddr*)localsocket_addr, *len) < 0){
        //perror(":BIND ERROR");
        close(sd);
        sd = -errno;
        return sd;
        //exit(1);
    }
    // Messa in ascolto del socket.
    if(listen(sd, 1) < 0) {
        //perror(":LISTEN ERROR");
        close(sd);
        unlink(prompt->localsocket_path);
        sd = -errno;
        return sd;
        //exit(1);
    }
    //printf("MAIN: localSocketInit completed with value \"%d\".\n", sd);
    //fflush(stdout);
    return sd;
}

// Elimina in sicurezza la socket locale al momento della chiusura del main thread. Riceve una socket ed un
// indirizzo di memoria su cui effettuare il CLOSE() e l'UNLINK().
void deleteLocalSocket(struct prompt_thread *prompt) {
    //if(!(strcmp(localsocket_path, ""))) {
    close(prompt->sd);
    unlink(prompt->localsocket_path);
    //}
    //else {
    //    fprintf(stderr, ":DELETE LOCALSOCKET: input string must be not be empty.\n");
    //}
}