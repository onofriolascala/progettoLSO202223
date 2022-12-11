//
// Main del Client. Responsabile di inizializzare il processo client e di gestire le comunicazioni con il server.
//

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

#include "../include/SocketUtilClient.h"

#define MAXCOMMBUFFER 1024

int main() {
    int sd1, sd2, server_port;
    struct sockaddr_in server_addr;
    socklen_t len;
    char server_ip[MAXCOMMBUFFER];

    // Inizializzazioni
    // createPrompt();
    // createRender();


    // Inizializzazione connessione
    strcpy(server_ip, "127.0.0.1");
    server_port = 5200;
    sd1 = socketInit(&server_addr, &len, server_ip, server_port);

    // Main loop di ascolto

    write(sd1, "14:tentativo", 13);

    sleep(20);
    close(sd1);
    printf("Terminazione processo client.\n");
    return 0;
}