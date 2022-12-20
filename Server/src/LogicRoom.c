//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicRoom.h"

// listen_sd deve essere non blocking

void mainRoomLoop(int listen_local_sd){

    struct pollfd fds[9];
    char buffer[MAXCOMMBUFFER];
    int timeout, rc = 0;
    int nfds, new_local_sd, current_size = 0, i, j, signal_code;
    int end_loop = 0, close_conn, compress_array = 0;

    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = listen_local_sd;
    fds[0].events = POLLIN;
    nfds = 1;

    timeout = ( 3 * 60 * 1000);

    do{
        printf("DEBUG: Waiting on poll function...\n");
        fflush(stdout);

        rc = poll(fds, nfds, timeout);

        if( rc < 0 ){
            //poll failed
            perror("poll failed");
            break;
        }
        if( rc == 0 ){
            // timeout raggiunto
            printf("DEBUG: poll timed out. Chiusura del logic loop\n");
            break;
        }
        current_size = nfds;
        for(i = 0; i < current_size; i++){
            if(fds[i].revents == 0){
                continue;
            }
            if(fds[i].revents != POLLIN){
                //unexpected behaviour
                printf("DEBUG: Error! fds[%d].revents = %d\n",i, fds[i].revents);
                end_loop = 1;
                break;
            }
            if(fds[i].fd == listen_local_sd){
                do{
                    //the listening socket is readable, so we manage the new incoming connections
                    printf("DEBUG: new incoming connetion to the room\n");/*

                    **GESTIONE DELLE NUOVE CONNESSIONI**


                    new_local_sd = accept(listen_local_sd, NULL, NULL);
                    if (new_local_sd < 0){
                        if (errno != EWOULDBLOCK){
                            perror("  accept() failed");
                            end_loop = 1;
                        }
                        break;
                    }

                    fds[nfds].fd = new_local_sd;
                    fds[nfds].events = POLLIN;
                    nfds++;

                */
                }while( listen_local_sd != -1 );
            }
            else {
                //not the listening socket gestione dei socket giocatori
                close_conn = 0;
                signal_code = readFromClient(fds[i].fd, buffer, MAXCOMMBUFFER);
                if (signal_code < 0) {
                    //error situation
                    if (signal_code != -1) {
                        //error with the read of the socket different from EWOULDBLOCK
                        close_conn = 1; //set the close connection flag to true
                    }
                }
                else if (signal_code == 0) {
                    //in this case the client has disconnected
                    printf("DEBUG: client with sd %d, disconnected\n", fds[i].fd);
                    close_conn = 1;
                } else {
                    //data was recived
                    //gamelogic
                }
                if (close_conn) {
                    //if the close connection flag is true we need to remove the socket descriptor from the polling array
                    for (j = i; j < nfds; j++) {
                        fds[j].fd = fds[j + 1].fd;
                    }
                    nfds--;
                }
            }
        }
        // restart polling unless loop ended
    }while(!end_loop);

}