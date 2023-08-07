
/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#include "../include/ThreadRoom.h"

// Funzione principale.
void* thrRoom(void* arg) {
    //printf("DEBUG: Creazione stanza.\n");
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER], parser_buf[USERNAMELENGTH];
    int ID, localsocket, temp_sd, parser_sd, signal_num;
    struct room_node** room_list;
    struct room_node* this_room;
    struct player_node *joining_player, *temp_player;

    struct sockaddr_un localsocket_addr;
    socklen_t localsocket_len;
    struct room_arg temp;

    signal_num = 0;

    // Copia i valori della struttura originale.
    room_list = (*(struct room_arg*)arg).room_list;
    struct mySQLConnection* db_connection = (*(struct room_arg*)arg).db_connection;
    this_room = createAndAddNewRoom(room_list);
    ID = this_room->id;
    (*(struct room_arg*)arg).room_ID = this_room->id;

    // Apertura della localsocket
    localsocket = localSocketInit(ID, this_room->localsocket, &localsocket_addr, &localsocket_len);

    // La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct room_arg*)arg).flag = 1;
    printf("\t\t\t\tROOM_ID%d: initialized with ID value of \"%d\", and local socket of \"%s:%d\".\n", ID, ID, localsocket_addr.sun_path, localsocket);
    fflush(stdout);

    memset(incoming, '\0', sizeof(incoming));
    memset(outgoing, '\0', sizeof(outgoing));
    //                  log                                 //
    //int log_fd;
    //log_fd = createLog(this_room->id);

    //                 mainRoomLoop init                   //

    struct pollfd fds[MAXPLAYERS+1];
    char buffer[MAXCOMMBUFFER];
    char users[500];
    int timeout, rc = 0;
    //int localsocket;
    int nfds, new_local_sd, current_size = 0, i, j, signal_code;
    int next_turn = 0, close_conn;
    char *saveptr = NULL;
    char *username_p, *fd_p;
    struct player_node* new_player, *player, *tmp;
    int new_player_fd;


    /*                gameLogic                             */

    struct player_node* current_player = NULL;
    struct player_node* suzerain = NULL;
    int close_room = 0, word_is_selected = 0;
    clock_t start_t, end_t;
    double total_t;
    char words[3][MAXWORDLENGTH];
    char hidden_word[MAXWORDLENGTH];
    char guess[MAXWORDLENGTH];
    int unveiling_sequence[MAXWORDLENGTH];
    int current_unveil;
    int selected_word,selected_word_len, z;
    int word_is_sent, add_hint_flag;
    //initializing the listening socket into the polling array
    memset(fds, 0 ,sizeof(fds));
    fds[0].fd = localsocket;
    fds[0].events = POLLIN;
    nfds = 1;

    timeout = TURNTIMEOUT;

    //writeToLog(log_fd,"ROOM %d: SETUP COMPLETED!");

    //              MAIN GAME LOOP                //

    while( !close_room ){
        /* inizio di un nuovo turno*/
        memset(hidden_word,'\0',sizeof(hidden_word));
        start_t = clock();
        total_t = 0;
        next_turn = 0;
        word_is_selected = 0;
        selected_word = 0;
        add_hint_flag = 0;
        word_is_sent = 0;
        generateRandomWords(words);

        do{
            printf("\t\t\t\tDEBUG_STANZAID_%d: Waiting on poll function...\n", ID);
            fflush(stdout);
            memset(outgoing,'\0',sizeof(outgoing));
            memset(incoming,'\0',sizeof (incoming));

            /* game logic */

            if(!word_is_selected && suzerain != NULL && !word_is_sent && this_room->player_num > 1){

                sprintf(outgoing, "1) %s - 2) %s - 3) %s ", words[0], words[1], words[2]);
                sleep(1);
                writeToClient(suzerain->player_socket, S_CHOOSEWORD,  outgoing);
                printf("\t\t\t\tDEBUG_STANZAID%d: Sending words [%s - %s - %s] to Suzerain %s with socket %d.\n", this_room->id, words[0], words[1] , words[2], suzerain->username, suzerain->player_socket);
                word_is_sent = 1;
                memset(outgoing,'\0',sizeof(outgoing));
            }

            rc = poll(fds, nfds, timeout);

            if( rc < 0 ){
                //poll fallita, errore critico quindi chiudo stanza
                perror("poll failed");
                close_room = 1;
                break;
            }
            if( rc == 0 ){
                // timeout raggiunto
                if(!word_is_selected){
                    if(suzerain != NULL)
                        moveSuzerainTurn(&suzerain,suzerain->next);
                    break;
                }
                writeToClient(current_player->player_socket, S_ENDOFTURN, S_ENDOFTURN_MSG);

                movePlayerTurn(&current_player,suzerain,&add_hint_flag);
                if(add_hint_flag){
                    addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);
                    add_hint_flag = 0;
                }

                writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);

                continue;
            }
            current_size = nfds;
            for(i = 0; i < current_size; i++){
                if(fds[i].revents == 0){
                    continue;
                }
                if(fds[i].revents != POLLIN){
                    //unexpected behaviour
                    //probabile socket chiusa, rimuovere il giocatore dalla partita
                    printf("\t\t\t\tDEBUG_STANZAID_%d: Error! fds[%d].revents = %d\n", ID, i, fds[i].revents);
                    next_turn = 1;
                    close_room = 1;
                    break;
                }
                if(fds[i].fd == localsocket){

                    //the listening socket is readable, so we manage the new incoming connections
                    printf("\t\t\t\tDEBUG_STANZAID_%d: new incoming connection to the room\n", ID);
                    new_local_sd = accept(localsocket, NULL, NULL);
                    if (new_local_sd < 0){
                        if (errno != EWOULDBLOCK){
                            perror("  accept() failed");
                            next_turn = 1;
                            close_room = 1;
                        }
                        break;
                    }
                    /*  new join routine   */

                    signal_num = readFromClient(new_local_sd, incoming, MAXCOMMBUFFER);

                    username_p = strtok_r(incoming, "-", &saveptr);
                    fd_p = strtok_r(NULL, "\0", &saveptr);
                    new_player_fd = atoi(fd_p);
                    //check if the usr is already in the room
                    new_player = getPlayerByUsername(this_room->player_list, username_p);

                    if( new_player == NULL){
                        new_player = createNewPlayerNode(new_player_fd, username_p);
                        this_room->player_list = addPlayerToPlayerList(this_room->player_list, new_player);



                        fds[nfds].fd = new_player_fd;
                        fds[nfds].events = POLLIN;
                        nfds++;
                        this_room->player_num++;

                        printf("\t\t\t\tDEBUG_STANZAID_%d: New connection to the room was accepted\n", ID);
                        fflush(stdout);

                        /* game logic */
                        /* if no suzerain was chosen set new usr as the suzerain*/
                        if( suzerain == NULL){
                            //printf("\t\t\t\tDEBUG_STANZAID%d: Selecting new suzerain...\n", ID);
                            suzerain = new_player;
                            printf("\t\t\t\tDEBUG_STANZAID_%d: current Suzerain %s with socket %d.\n", ID, new_player->username, new_player_fd);
                            word_is_selected = 0;
                        }
                        /* game logic */
                        /* if no usr was set to play and the new usr isn't the only one in the room set him as the current player*/
                        if( current_player == NULL && new_player != suzerain){
                            //printf("				DEBUG_STANZAID%d: Selecting new gamer...\n", ID);
                            current_player = new_player;
                            printf("\t\t\t\tDEBUG_STANZAID_%d: current turn %s with socket %d.\n", ID, new_player->username, new_player_fd);
                        }

                        getRoomInfo(suzerain, this_room->id, this_room->player_num, hidden_word, outgoing);

                        writeToClient(new_local_sd, S_OK, S_OK_MSG);
                        writeToClient(new_player_fd, S_ROOMOK, outgoing);

                        for(i = 1; i < nfds; i++){
                            if(fds[i].fd != new_player_fd)
                                writeToClient(fds[i].fd, S_PLAYERUPDATE, outgoing);
                        }
                    }
                    else{
                        printf("\t\t\t\tDEBUG_STANZAID_%d: New connection to the room was refused\t-- Player %s with socket %d already in room --\n", ID, new_player->username, new_local_sd);
                        fflush(stdout);
                        writeToClient(new_local_sd, S_USERINROOM, S_USERINROOM_MSG);
                    }
                }
                else {
                    //not the listening socket, managing the players socket
                    close_conn = 0;
                    signal_num = readFromClient(fds[i].fd, incoming, MAXCOMMBUFFER);
                    player = getPlayer(this_room->player_list, fds[i].fd);
                    switch (signal_num) {
                        case -1:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <SIGNAL_NUM A -1> \n", ID);
                            //EWOULDBLOCK ERROR
                            break;
                        case -2:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <SIGNAL_NUM A -2> \n", ID);
                            close_conn = 1;
                            break;
                        case -3:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <SIGNAL_NUM A -3> \n", ID);
                            //ERROR DIFFERENT FROM EWOULDBLOCK
                            close_conn = 1;
                            break;
                        case S_DISCONNECT_ABRUPT:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <Disconnessione> %d:%s\n", ID, signal_num, incoming);
                            /* game logic */
                            //if the player was the current player and disconnects we need to assign a new player
                            if( suzerain == player ){
                                next_turn = 1;
                                moveSuzerainTurn(&suzerain,suzerain->next);
                                //for(i = 0; i< nfds; i++) writeToClient(fds[i].fd, S_NEW_GAME, S_NEW_GAME_MESSAGE);
                                //gameOver?
                            }
                            if( current_player == player ){
                                movePlayerTurn(&current_player,suzerain,&add_hint_flag);
                                if(add_hint_flag){
                                    addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);
                                    add_hint_flag = 0;
                                }
                                if(current_player == player){   //caso in cui ci sono 2 giocatori nella stanza
                                    current_player = NULL;
                                }
                                else {
                                    usleep(REFRESHCONSTANT);
                                    writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);
                                }
                            }
                            destroyPlayerNode(removePlayerNode(&this_room->player_list, player->player_socket));
                            this_room->player_num--;
                            close_conn = 1;
                            break;
                        case S_DISCONNECT:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima della disconnessione.");
                            break;
                        case C_LOGIN:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo login.");
                            break;
                        case C_SIGNIN:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo sign in.");
                            break;
                        case C_CREATEROOM:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di crearne una nuova.");
                            break;
                        case C_JOINROOM:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di unirsi ad un'altra.");
                            break;
                        case C_LISTROOM:
                            writeToClient(fds[i].fd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                            break;
                        case C_LOGOUT:
                            writeToClient(fds[i].fd, S_NOPERMISSION, "Uscire dalla stanza prima di effettuare il logout.");
                            break;
                        case C_SELECTWORD:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <Seleziona Parola> %d:%s\n", ID, signal_num, incoming);
                            if(suzerain == player){

                                word_is_selected = 1;
                                selected_word = parserChosenWord(incoming);

                                // selected_word < 0 implica un errore
                                if(selected_word < 0) {
                                    word_is_selected = 0;
                                    word_is_sent = 0;
                                    continue;
                                }
                                selected_word--;
                                current_unveil = 0;

                                selected_word_len = strlen(words[selected_word]);

                                memset(hidden_word,'\0',sizeof(hidden_word));
                                strncpy(hidden_word, "______________________________", selected_word_len);
                                generateUnveilingSequence(unveiling_sequence,selected_word_len);
                                addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);

                                getRoomInfo(suzerain, this_room->id, this_room->player_num, hidden_word, outgoing);

                                for(z = 1; z < nfds; z++) {
                                    writeToClient(fds[z].fd, S_NEWGAME, outgoing );
                                }

                                current_player = suzerain->next;
                                usleep(REFRESHCONSTANT);
                                writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);
                                printf("\t\t\t\tDEBUG_STANZAID_%d: current turn %s with %d socket.\n", ID, current_player->username, current_player->player_socket);

                            }
                            break;
                        case C_GUESSSKIP:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <Guess> %d:%s\n", ID, signal_num, incoming);

                            signal_num = guessParser(incoming, outgoing, guess);

                            sleep(1);

                            if(signal_num == S_OK){
                                sprintf(outgoing, "%s prova \"%s\" ", player->username, guess);
                                if (strcmp(guess, words[selected_word]) == 0) {
                                    for (i = 1; i < nfds; i++) {
                                        if (fds[i].fd != player->player_socket) {
                                            writeToClient(fds[i].fd, S_DEFEAT, outgoing);
                                        }
                                        else {
                                            writeToClient(fds[i].fd, S_VICTORY, outgoing);
                                            printf("\t\t\t\tDEBUG_STANZAID_%d: %s with %d socket has won the round.\n", ID, current_player->username, current_player->player_socket);

                                        }
                                    }
                                    sleep(5);
                                    suzerain = player;
                                    next_turn = 1;
                                }
                                else {
                                    for (i = 1; i < nfds; i++) {
                                        writeToClient(fds[i].fd, S_MISSEDGUESS, outgoing);
                                    }
                                    sleep(1);
                                    movePlayerTurn(&current_player,suzerain,&add_hint_flag);
                                    if(add_hint_flag){
                                        addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);
                                        add_hint_flag = 0;
                                    }
                                    writeToClient(current_player->player_socket,S_YOURTURN,S_YOURTURN_MSG);
                                    printf("\t\t\t\tDEBUG_STANZAID_%d: current turn %s with %d socket.\n", ID, current_player->username, current_player->player_socket);
                                }
                            }
                            else {
                                sprintf(outgoing, "%s ha scritto una parola troppo lunga. ", player->username);
                                for (i = 1; i < nfds; i++) {
                                    writeToClient(fds[i].fd, S_MISSEDGUESS, outgoing);
                                }
                                sleep(1);
                                movePlayerTurn(&current_player, suzerain, &add_hint_flag);
                                if(add_hint_flag){
                                    addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);
                                    add_hint_flag = 0;
                                }
                                writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);
                            }
                            break;
                        case C_EXITROOM:
                            printf("\t\t\t\tDEBUG_STANZAID%d: <Lascia Stanza> %d:%s\n", ID, signal_num, incoming);

                            // Riavvio del threadService
                            rebuildService(player, room_list, db_connection);

                            if( current_player == player ){
                                movePlayerTurn(&current_player,suzerain,&add_hint_flag);
                                if(add_hint_flag){
                                    addHint(unveiling_sequence,&current_unveil,hidden_word,words[selected_word],selected_word_len);
                                    add_hint_flag = 0;
                                }
                                if(current_player == player){   //caso in cui ci sono 2 giocatori nella stanza
                                    current_player = NULL;
                                }
                                else {
                                    usleep(REFRESHCONSTANT);
                                    writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);
                                }
                            }
                            if( suzerain == player ){
                                next_turn = 1;
                                moveSuzerainTurn(&suzerain,suzerain->next);
                                //for(i = 0; i< nfds; i++) writeToClient(fds[i].fd, S_NEW_GAME, S_NEW_GAME_MESSAGE);
                                //gameOver?
                            }

                            writeToClient(player->player_socket, S_HOMEPAGEOK, S_HOMEPAGEOK_MSG);

                            destroyPlayerNode(removePlayerNode(&this_room->player_list, player->player_socket));
                            this_room->player_num--;
                            close_conn = 1;

                            break;
                        default:
                            printf("\t\t\t\tDEBUG_STANZAID_%d: <ERRORE> %d: Codice di comunicazione non riconosciuto.\n", ID, signal_num);
                            writeToClient(fds[i].fd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
                    }
                    if (close_conn) {
                        //if the close connection flag is true we need to remove the socket descriptor from the polling array
                        for (j = i; j < nfds; j++) {
                            fds[j].fd = fds[j + 1].fd;
                        }
                        nfds--;

                        /* check to see if the room is empty*/
                        if ( nfds == 1 ){
                            //room is empty
                            //closing room routine
                            next_turn = 1;
                            close_room = 1;
                            break;
                        }
                        if( nfds == 2){
                            next_turn = 1;
                        }

                        getRoomInfo(suzerain, this_room->id, this_room->player_num, hidden_word, outgoing);


                        for(i = 1; i < nfds; i++){
                                writeToClient(fds[i].fd, S_PLAYERUPDATE, outgoing);
                        }

                    }
                }
            }
            // restart polling unless loop ended

            end_t = clock();
            total_t += (double) (end_t - start_t) / CLOCKS_PER_SEC;

            //::IMPORTANTE:: Da dividere turntimeout per 10000 per riportarlo a secondi
            if(total_t > (TURNTIMEOUT)){ //1 minute round timeout + 30 seconds for server-side delays
                //round timeout routine
                if(!word_is_selected){
                    if(suzerain != NULL){
                        suzerain = suzerain->next;
                        break;
                    }
                }
                current_player = current_player->next;

                writeToClient(current_player->player_socket, S_ENDOFTURN, S_ENDOFTURN_MSG);

                if(current_player == suzerain){
                    //addHint()
                    current_player = current_player->next;
                }

                writeToClient(current_player->player_socket, S_YOURTURN, S_YOURTURN_MSG);

            }

        }while(!next_turn);
    }

    // Chiusura della stanza.

    //Se sono presenti ancora dei giocatori durante la chiusura della stanza mi occupo di fare il rebuild del service
    for(i=0;i<nfds;i++){
        if(fds[i].fd != localsocket){
            player = getPlayer(this_room->player_list, fds[i].fd);
            rebuildService(player, room_list, db_connection);
        }
    }

    removeAndDestroyRoomNode(room_list, ID);

    //sleep(10);
    deleteLocalSocket(localsocket, localsocket_addr.sun_path);

    printf("\t\t\t\tROOM_ID%d: room thread has ended.\n", ID);
    return 0;
}

// Creazione del thread della stanza.
int createNewRoom(int sd, struct room_node** room_list, struct mySQLConnection* db_connection) {
    printf("DEBUG: Input for createNewRoom.\n");
    fflush(stdout);
    int flag;
    pthread_t tid;

    struct room_arg args;
    args.db_connection = db_connection;
    args.room_list = room_list;
    args.room_ID = 0;
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrRoom, &args)) {
        printf(":THREAD CREATION ERROR:\n");
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il service terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewRoom. */
    while(args.flag == 0) {
        usleep(REFRESHCONSTANT);
    }

    printf("\t\tSERVICE_SD%d: room thread created with ID:%d.\n", sd, args.room_ID);
    fflush(stdout);
    return args.room_ID;
}

// Da sincronizzare con la distruzione delle stanze? Ma come?
int joinRoom(int sd, int ID, struct room_node** head_pointer, char username[], char outgoing[]) {
    printf("\t\tSERVICE_SD%d: join started\n", sd);

    int signal_num, room_sd;
    char temp_in[MAXCOMMBUFFER], temp_out[MAXCOMMBUFFER];
    struct room_node* joined_room;
    struct sockaddr_un indirizzo;

    indirizzo.sun_family = PF_LOCAL;
    signal_num = S_UNKNOWNSIGNAL;

    joined_room = getRoom(*head_pointer, ID);

    if (joined_room == NULL) {
        strcpy(outgoing, S_ROOMNOTFOUND_MSG);
        signal_num = S_ROOMNOTFOUND;
    }
    else if (joined_room->player_num >= MAXPLAYERS){
        strcpy(outgoing, S_FULLROOM_MSG);
        signal_num = S_FULLROOM;
    }
    else {
        strcpy(indirizzo.sun_path, joined_room->localsocket);
        // Creazione del socket
        if((room_sd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
            perror(":SOCKET CREATION");
            strcpy(outgoing, "Impossibile creare socket locale del service.");
            return S_UNKNOWNSIGNAL;
        }
        // Connessione...
        if(connect(room_sd, (struct sockaddr*)&indirizzo, sizeof(indirizzo)) < 0) {
            perror(":SOCKET CONNECT");
            strcpy(outgoing, "Impossibile connettersi a localsocket.");
            return S_UNKNOWNSIGNAL;
        }
        // Invio delle informazioni utente
        sprintf(temp_out, "%s-%d", username, sd);
        writeToClient(room_sd, C_JOINROOM, temp_out);
        // Attesa di conferma dalla stanza
        signal_num = readFromClient(room_sd, temp_in, MAXCOMMBUFFER);

        if (signal_num == S_OK) {
            strcpy(outgoing, S_ROOMOK_MSG);
            signal_num = S_ROOMOK;
        }
        else if (signal_num == S_USERINROOM) {
            strcpy(outgoing, S_USERINROOM_MSG);
            signal_num = S_USERINROOM;
        }
        else {
            strcpy(outgoing, "Lettura scorretta da localSocket.");
            signal_num = S_UNKNOWNSIGNAL;
        }
        close(room_sd);
    }

    /* Comunicazioni attese:
     * 70-Stanza piena, 71-Stanza inesistente, 72-Utente già connesso, 54-Stanza
     * 97-Segnale Sconosciuto, 95-Stanza non risponde
     * */
    if(signal_num == 54) {
        printf("\t\tSERVICE_SD%d: player \"%s\" joined room with ID:%d and %d players.\n", sd,
               username, ID, joined_room->player_num);
    }
    else {
        printf("\t\tSERVICE_SD%d: player \"%s\" unable to join room with ID:%d on basis:\"%s\".\n", sd,
               username, ID, outgoing);
    }
    return signal_num;
}


int guessParser(char incoming[], char outgoing[], char guess[]){
    char *saveptr = NULL;
    char *word_p;
    int word_len, return_value = S_COMMERROR;

    word_p = strtok_r(incoming, "\0", &saveptr);

    if(word_p != NULL) {
        word_len = strlen(word_p);
        if (word_len >= WORDLENGTH)
        {
            //fprintf(stderr, ":GUESS ERROR: guess is too long\n");
            strcpy(outgoing, "Guess is too long.");
            return_value = S_MISSEDGUESS;
        }
        else {
            strncpy(guess, word_p, word_len);
            guess[word_len] = '\0';
            return_value = S_OK;
        }
    }

    return return_value;
}


void getRoomInfo(struct player_node* suzerain,int room_num, int player_num, char selected_word[], char outgoing[MAXCOMMBUFFER]){
    char users[MAXCOMMBUFFER];
    struct player_node *player_tmp;
    int i = 0;
    if(strcmp(selected_word,"") == 0)
        strcpy(selected_word,"In attesa della parola...");
    if(suzerain != NULL){
        memset(users,'\0',sizeof(users));
        player_tmp = suzerain;
        do{
            strcat(users, player_tmp->username);
            strcat(users,",");
            player_tmp = player_tmp->next;
            i++;
        }
        while(player_tmp != suzerain);
        for(;i<=MAXPLAYERS;i++){
            strcat(users, ",");
        }
        sprintf(outgoing, "%d-%d-%s-%s-%s", room_num, player_num, suzerain->username, users, selected_word);
    }
}

void moveSuzerainTurn(struct player_node** curr_suzerain, struct player_node* new_suzerain){
    if(*curr_suzerain != NULL){
        if((*curr_suzerain) == (*curr_suzerain)->next)
            *curr_suzerain = NULL;
        else
            *curr_suzerain = new_suzerain;
    }
}

void movePlayerTurn(struct player_node** curr_player,struct player_node* suzerain, int* addHintFlag){
    if(*curr_player != NULL){
        *curr_player = (*curr_player)->next;
        if((*curr_player) == suzerain){
            *addHintFlag = 1;
            *curr_player = (*curr_player)->next;
        }
    }
}