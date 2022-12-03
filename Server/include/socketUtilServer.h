//
// Funzioni per la gestione dei socket
//

#ifndef PROGETTOLSO202223_SOCKETUTIL_H
#define PROGETTOLSO202223_SOCKETUTIL_H

int socketInit(struct sockaddr_in* server_addr, int len);
void acceptLoop(int sd1);



#endif //PROGETTOLSO202223_SOCKETUTIL_H
