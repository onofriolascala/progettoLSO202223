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
    int sd1, sd2, server_port, signal_num;
    struct sockaddr_in server_addr;
    socklen_t len;
    char server_ip[MAXCOMMBUFFER];
    char tempbuffer[MAXCOMMBUFFER];

    // Inizializzazioni
    signal_num = 1;

    // createPrompt();
    // createRender();

    printf("Inserire l'indirizzo ip: ");
    fgets(server_ip, MAXCOMMBUFFER, stdin);
    server_ip[strcspn(server_ip, "\n")] = 0;
    /*printf("Inserire la porta: ");
    fgets(tempbuffer, MAXCOMMBUFFER, stdin);
    server_ip[strcspn(tempbuffer, "\n")] = 0;*/
    // Inizializzazione connessione
    strcpy(server_ip, "25.72.233.6");
    server_port = 5200;

    sd1 = socketInit(server_addr, len, server_ip, server_port);

    while(signal_num > 0) {
        printf("Opzioni disponibili:\n"
               "0 - Disconnessione\t\t10 - Login\t\t11 - Registrazione\n");
        printf("Inserire un numero tra i precedenti: ");
        fflush(stdout);
        fgets(server_ip, MAXCOMMBUFFER, stdin);
        server_ip[strcspn(server_ip, "\n")] = 0;
        strcat(server_ip, ":test");
        printf("%s\n", server_ip);
        write(sd1, server_ip, strlen(server_ip)+1);

        memset(server_ip,0,sizeof(server_ip));
        read(sd1, server_ip, 2);
        signal_num = atoi(server_ip);
        read(sd1, server_ip, 1);
        read(sd1, server_ip, MAXCOMMBUFFER);
        printf("MAIN: server says %d:%s\n", signal_num, server_ip);
        fflush(stdout);
    }

    close(sd1);
    printf("Terminazione processo client.\n");
    return 0;
}