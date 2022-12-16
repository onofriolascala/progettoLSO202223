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

#include "../include/SocketUtilServer.h"
#include "../include/ThreadService.h"
#include "../include/Def.h"
//#include "../include/ThreadRoom.h"



int main() {
    int sd1, sd2;
    struct room_node** room_list_head;
    struct sockaddr_in server_addr;
    socklen_t len;

    room_list_head = NULL;

    sd1 = socketInit(&server_addr, &len);
    acceptLoop(sd1, room_list_head);

    close(sd1);
    printf("Terminazione processo server.\n");
    return 0;
}