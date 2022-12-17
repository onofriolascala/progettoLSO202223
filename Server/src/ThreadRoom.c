/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#include "../include/ThreadRoom.h"

// Funzione principale.
void* thrRoom(void* arg) {
    //printf("DEBUG: Creazione stanza.\n");
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];
    int ID, localsocket, temp_sd, out_len, signal_num;
    struct room_node** room_list;
    struct room_node* this_room;
    struct room_node* currenthead;

    signal_num = 1;

    struct sockaddr_un localsocket_addr;
    socklen_t localsocket_len;

    struct room_arg temp;
    temp = *(struct room_arg*)arg;
    //printf("DEBUG:1.\n");

    // Copia i valori della struttura originale.
    //printf("DEBUG:1,5\n");
    room_list = temp.room_list;

    /*if (room_list == NULL) {
        currenthead = NULL;
        printf("DEBUG:2.1\n");
    }
    else {
        currenthead = *room_list;
        printf("DEBUG:2.2\n");
    }*/

    this_room = createAndAddNewRoom(room_list);
    ID = this_room->id;

    //printf("DEBUG:3.\n");
    // Apertura della localsocket
    localsocket = localSocketInit(ID, this_room->localsocket, &localsocket_addr, &localsocket_len);
    //printf("DEBUG:4.\n");

    // La flag, condivisa da service e room appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct room_arg*)arg).room_ID = this_room->id;
    (*(struct room_arg*)arg).flag = 1;
    printf("\t\t\t\tROOM_ID%d: initialized with ID value of \"%d\", and local socket of \"%s:%d\".\n", ID, ID, localsocket_addr.sun_path, localsocket);
    fflush(stdout);

    memset(incoming, '\0', sizeof(incoming));
    memset(outgoing, '\0', sizeof(outgoing));

    // Routine di accettazione sulla localsocket
    if((temp_sd = accept(localsocket, NULL, NULL)) < 0) {
        perror(":ACCEPT ERROR");
        //close(localsocket);
    }
    else {
        writeToClient(temp_sd, S_OK, S_OK_MSG);
        close(temp_sd);
    }

    temp_sd = this_room->player_list->player_socket;
    // Switch per la gestione della game logic
    /*while (this_room->player_num > 0) {
        //printf("\t\tDEBUG_SD%d: secondo while.\n", sd);

        memset(incoming, '\0', sizeof(incoming));
        memset(outgoing, '\0', sizeof(outgoing));
        signal_num = readFromClient(temp_sd, incoming, MAXCOMMBUFFER);

        printf("\t\t\t\tROOM_ID%d: <client> %d:%s\n", ID, signal_num, incoming);
        switch (signal_num) {
            case -1:
                break;
            case -2:
                break;
            case -3:
                break;
            case S_DISCONNECT:
                printf("\t\t\t\tDEBUG_STANZAID%d: <Disconnessione> %d:%s\n", ID, signal_num, incoming);
                //removePlayerNode();
                //destroyPlayerNode();
                //updatePlayerNum()
                this_room->player_num -= 1;
                writeToClient(temp_sd, S_DISCONNECT, S_DISCONNECT_MSG);
                break;
            case C_LOGIN:
                writeToClient(temp_sd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo login.");
                break;
            case C_SIGNIN:
                writeToClient(temp_sd, S_NOPERMISSION, "Uscire dalla stanza prima di un nuovo sign in.");
                break;
            case C_CREATEROOM:
                writeToClient(temp_sd, S_NOPERMISSION, "Uscire dalla stanza prima di crearne una nuova.");
                break;
            case C_JOINROOM:
                writeToClient(temp_sd, S_NOPERMISSION, "Uscire dalla stanza prima di unirsi ad un'altra.");
                break;
            case C_LISTROOM:
                writeToClient(temp_sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                break;
            case C_LOGOUT:
                writeToClient(temp_sd, S_NOPERMISSION, "Uscire dalla stanza prima di effettuare il logout.");
                break;
            case C_SELECTWORD:
                //printf("\t\t\t\tDEBUG_STANZAID%d: <Seleziona Parola> %d:%s\n", ID, signal_num, incoming);
                writeToClient(temp_sd, S_OK, "Parola.");
                break;
            case C_GUESSSKIP:
                //printf("\t\t\t\tDEBUG_STANZAID%d: <Guess> %d:%s\n", ID, signal_num, incoming);
                writeToClient(temp_sd, signal_num, "Mancato");
                break;
            case C_EXITROOM:
                //printf("\t\t\t\tDEBUG_STANZAID%d: <Lascia Stanza> %d:%s\n", ID, signal_num, incoming);
                //removePlayerNode();
                //destroyPlayerNode();
                //updatePlayerNum()
                //this_room->player_num -= 1;
                writeToClient(temp_sd, S_HOMEPAGEOK, S_HOMEPAGEOK_MSG);
                break;
            default:
                printf("\t\t\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                writeToClient(temp_sd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
        }
        fflush(stdout);
    }*/

    // Chiusura della stanza.
    //*room_list = removeAndDestroyRoomNode(*room_list, ID);

    sleep(20);
    deleteLocalSocket(localsocket, localsocket_addr.sun_path);

    printf("\t\t\t\tROOM_ID%d: room thread has ended.\n", ID);
    return 0;
}

// Creazione del thread della stanza.
int createNewRoom(int sd, struct room_node** room_list) {
    /*printf("DEBUG: Input for createNewRoom.\n");
    fflush(stdout);*/
    int flag;
    pthread_t tid;

    struct room_arg args;
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

// AGGIUNGERE MUTEX
int joinRoom(int sd, int ID, struct room_node** room_list, struct player_node* player, char outgoing[]) {
    printf("\t\tSERVICE_SD%d: join started\n", sd);
    fflush(stdout);
    int signal_num, room_sd;
    char temp_buf[MAXCOMMBUFFER];
    struct room_node* joined_room;

    struct sockaddr_un indirizzo;
    indirizzo.sun_family = PF_LOCAL;

    strncpy(temp_buf, "\0", sizeof());

    signal_num = S_UNKNOWNSIGNAL;

    if (room_list == NULL){
        printf("DEBUG:0\n");
    }
    else {
        printf("DEBUG:1\n");
    }
    fflush(stdout);

    joined_room = getRoom(*room_list, ID);

    if (joined_room == NULL) {
        printf("DEBUG:2\n");
        strcpy(outgoing, S_ROOMNOTFOUND_MSG);
        signal_num = S_ROOMNOTFOUND;
    }
    else if (joined_room->player_num > 8){
        printf("DEBUG:3\n");
        strcpy(outgoing, S_FULLROOM_MSG);
        signal_num = S_FULLROOM;
    }
    else {
        printf("DEBUG:4\n");
        if (getPlayer(joined_room->player_list, player->player_socket) == NULL) {
            printf("DEBUG:5\n");

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
            printf("DEBUG:7\n");
            fflush(stdout);
            printf("DEBUG:room_sd %d\n", room_sd);
            fflush(stdout);
            printf("DEBUG:string %s\n", indirizzo.sun_path);
            fflush(stdout);
            printf("\t\tDEBUG_joinSD%d: connection established with room_sd \"%d\" on addr \"%s\" signal buf \"%s\".\n", sd, room_sd, indirizzo.sun_path, temp_buf);
            fflush(stdout);
            signal_num = readFromClient(room_sd, temp_buf, MAXCOMMBUFFER);
            printf("\t\tDEBUG_joinSD%d: signal %d:%s.\n", sd, signal_num, temp_buf);
            fflush(stdout);

            if(signal_num == 50) {
                strcpy(outgoing, S_ROOMOK_MSG);
                joined_room->player_list = addPlayerToPlayerList(joined_room->player_list, player);
                //updatePlayerNum()
                joined_room->player_num += 1;
                signal_num = S_ROOMOK;
            }
            else {
                signal_num = S_UNKNOWNSIGNAL;
                strcpy(outgoing, "Lettura scorretta da localSocket.");
            }

            close(room_sd);
        }
        else {
            printf("DEBUG:6\n");
            strcpy(outgoing, S_USERINROOM_MSG);
            signal_num = S_USERINROOM;
        }
    }

    /* Comunicazioni attese:
     * 70-Stanza piena, 71-Stanza inesistente, 72-Utente già connesso, 54-Stanza
     * */
    if(signal_num == 54) {
        printf("\t\tSERVICE_SD%d: player \"%s\" joined room with ID:%d and %d players.\n", sd,
               player->username, ID, joined_room->player_num);
    }
    else {
        printf("\t\tSERVICE_SD%d: player \"%s\" unable to join room with ID:%d on basis:\"%s\".\n", sd,
               player->username, ID, outgoing);
    }
    return signal_num;
}

