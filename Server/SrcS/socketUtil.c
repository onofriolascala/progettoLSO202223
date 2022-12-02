//
// Funzioni per la gestione dei socket
//

int createAndBindServerSocket(struct sockaddr_in* server_addr,int len){
    int sd1;
    if((sd1 = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror(":SOCKET ERROR");
    }
    if(bind(sd1, (struct sockaddr*)&server_addr, len) < 0){
        perror(":BIND ERROR");
        close(sd1);
        //exit(1);
    }
    //listen
    return sd1;
}

