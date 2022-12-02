int createAndBindClientSocket(){
    int sd1;
    char buf[50]="Messaggio da parte del client\n";
    int len=strlen(buf);
    struct sockaddr_in indirizzo;
    indirizzo.sin_family=AF_INET;
    indirizzo.sin_port=htons(5200);

    inet_aton("127.0.1.1", &indirizzo.sin_addr);

    if((sd1=socket(PF_INET, SOCK_STREAM,0))<0){
        perror(":SOCKET ERROR:");
        exit(1);
    }

    if(connect(sd1, (struct sockaddr*)&indirizzo, sizeof(indirizzo))<0) {
        perror(":CONNECT ERROR:");
        close(sd1);
        exit(1);
    }

}