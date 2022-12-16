/*
* Funzioni del thread responsabile della gestione della logica di gioco.
*/

#include "../include/ThreadRoom.h"

// Funzione principale.
void* thrRoom(void* arg) {
    //printf("DEBUG: Creazione stanza.\n");
    char incoming[MAXCOMMBUFFER], outgoing[MAXCOMMBUFFER];
    int ID, localsocket, room_sd, out_len, signal_num;
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

    // Switch per la gestione della game logic
    /*while(signal_num > 0) {
        while (signal_num > 0) {
            printf("\t\t\t\t\tDEBUG_ROOMID%d: inizio while.\n", ID);
            memset(incoming, 0, sizeof(incoming));
            memset(outgoing, 0, sizeof(outgoing));
            signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);
            switch (signal_num) {
                case -1:
                    break;
                case -2:
                    break;
                case -3:
                    break;
                case 0:
                    printf("\t\t\t<Disconnesione> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 50, "OK.");
                    break;
                case 10:
                    strcpy(incoming, "32"
                                     "debug1debug2debug3debug4debug5de"
                                     "16"
                                     "debug1debug2debu");
                    printf("\t\t\t<Login> %d:%s\n", signal_num, incoming);
                    signal_num = login(sd, incoming, player, outgoing);
                    writeToClient(sd, signal_num, outgoing);
                    break;
                case 11:
                    printf("\t\t\t<Registrazione> %d:%s\n", signal_num, incoming);
                    //signin()
                    writeToClient(sd, 51, "Registrazione");
                    break;
                case 14:
                    printf("\t\t\t<DEBUG> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 42, "Hai trovato il messaggio di DEBUG.");
                    break;
                case 20:
                    printf("\t\t\t<Crea stanza> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Funzionalità non permessa.");
                    break;
                case 21:
                    printf("\t\t\t<Entra stanza> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Funzionalità non permessa.");
                    break;
                case 22:
                    printf("\t\t\t<Lista stanze> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Funzionalità non permessa.");
                    break;
                case 23:
                    printf("\t\t\t<Logout> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Funzionalità non permessa.");
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                    writeToClient(sd, 98, "Comunicazione non riconosciuta.");
            }
            fflush(stdout);
        }
    }*/

    // Chiusura della stanza.
    //*room_list = removeAndDestroyRoomNode(*room_list, ID);
    deleteLocalSocket(localsocket, localsocket_addr.sun_path);

    printf("\t\t\t\tROOM_ID%d: service thread has ended.\n", ID);
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

    printf("\t\t\tSERVICE_SD%d: room thread created with ID:%d.\n", sd, args.room_ID);
    fflush(stdout);
    return args.room_ID;
}

//
int joinRoom(int ID, struct room_node** room_list, struct player_node* player) {
    int signal_num;

    //signal_num =

    /* Comunicazioni attese:
     * 70-Stanza piena, 71-Stanza inesistente, 72-Utente già connesso, 54-Stanza
     * */
    return signal_num;
}

