/*
 * Main del Server. Responsabile di inizializzare il processo server e di gestire l'inizializzazione dei socket
 * e dei thread adibiti alla gestione delle comunicazioni con i singoli client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sd1, sd2;
    //struct room_node room_head;

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5200);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(ip_addr);

    sd1 = initializeSocket(&server_addr, len);
    acceptLoop(sd1);

    close(sd1);
    printf("Terminazione processo server.\n");
    return 0;
}