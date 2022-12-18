//
// Created by osboxes on 12/18/22.
//

#include "../include/LogicRoom.h"

// listen_sd deve essere non blocking

void mainRoomLoop(int listen_sd){

    struct pollfd fds[9];
    int timeout, rc = 0;
    int nfds, current_size = 0, i;
    int end_loop = 0;

    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = listen_sd;
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
            if(fds[i].fd == listen_sd){

            }
        }


    }while(!end_loop);

}