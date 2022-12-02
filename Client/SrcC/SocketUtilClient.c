int createAndBindClientSocket(struct sockaddr_in* client_addr, int porta, char indirizzo[]){
    int sd1;
    struct sockaddr_in client_addr;
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=htons(porta);

    inet_aton(indirizzo, &client_addr.sin_addr);

    if((sd1=socket(PF_INET, SOCK_STREAM,0))<0){
        perror(":SOCKET ERROR:");
        exit(1);
    }

    if(connect(sd1, (struct sockaddr*)&client_addr, sizeof(client_addr))<0) {
        perror(":CONNECT ERROR:");
        close(sd1);
        exit(1);
    }

    return sd1;
}