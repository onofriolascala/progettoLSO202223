/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#include "../include/ThreadService.h"
#include "../include/ThreadRoom.h"
#include "../include/CommUtilServer.h"
#include "../include/ListHandler.h"
#include "../include/LogicSigninLogin.h"
#include "../include/Def.h"

// Funzione principale.
void* thrService(void* arg) {
    /*printf("DEBUG: Thread started...\n");
    fflush(stdout);*/

    //struct player_node player;
    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];
    int sd, service_sd, out_len, signal_num, room_ID;
    struct player_node* player;
    struct room_node** room_list;

    // Copia i valori della struttura originale.
    sd = (*(struct service_arg*)arg).sd;
    player = (*(struct service_arg*)arg).player;
    room_list = (*(struct service_arg*)arg).room_list;

    // La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct service_arg*)arg).flag = 1;
    printf("\t\tSERVICE_SD%d: initialized with socket value of \"%d\", and player value of \"%s\".\n", sd, sd, player->username);
    fflush(stdout);

    if (player == NULL) {
        signal_num = 1;
    }
    else {
        signal_num = 52;
    }

    while(signal_num > 0) {
        while (signal_num > 0 && signal_num != 52) {
            //printf("\t\tDEBUG_SD%d: primo while.\n", sd);

            memset(incoming, '\0', sizeof(incoming));
            memset(outgoing, '\0', sizeof(outgoing));
            signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);

            printf("\t\tSERVICE_SD%d: <client> %d:%s outgoing %s\n", sd, signal_num, incoming, outgoing);
            switch (signal_num) {
                case -1:
                    break;
                case -2:
                    break;
                case -3:
                    break;
                case S_DISCONNECT:
                    writeToClient(sd, S_DISCONNECT, S_DISCONNECT_MSG);
                    break;
                case C_LOGIN:
                    //printf("\t\tDEBUG_SD%d: <Login>.\n", sd);
                    strcpy(incoming, "32"
                                     "debug1debug2debug3debug4debug5de"
                                     "16"
                                     "debug1debug2debu");
                    player = createNewPlayerNode(sd, "Guest");
                    signal_num = login(sd, incoming, player, outgoing);
                    writeToClient(sd, signal_num, outgoing);
                    break;
                case C_SIGNIN:
                    //printf("\t\tDEBUG_SD%d: <Signin> %d:%s\n", sd, signal_num, incoming);
                    //signin()
                    writeToClient(sd, S_LOGINOK, S_LOGINOK_MSG);
                    break;
                case 14:
                    writeToClient(sd, 42, "Hai trovato il messaggio di DEBUG.");
                    break;
                case C_CREATEROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_JOINROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_LISTROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_LOGOUT:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_SELECTWORD:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_GUESSSKIP:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_EXITROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                    writeToClient(sd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
            }
            fflush(stdout);
        }
        while (signal_num > 0 && signal_num != 51) {
            //printf("\t\tDEBUG_SD%d: secondo while.\n", sd);

            memset(incoming, '\0', sizeof(incoming));
            memset(outgoing, '\0', sizeof(outgoing));
            signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);

            printf("\t\tSERVICE_SD%d: <client> %d:%s outgoing %s\n", sd, signal_num, incoming, outgoing);
            switch (signal_num) {
                case -1:
                    break;
                case -2:
                    break;
                case -3:
                    break;
                case S_DISCONNECT:
                    writeToClient(sd, S_DISCONNECT, S_DISCONNECT_MSG);
                    break;
                case C_LOGIN:
                    writeToClient(sd, S_NOPERMISSION, "Effettuare il logout prima di un nuovo login.");
                    break;
                case C_SIGNIN:
                    writeToClient(sd, S_NOPERMISSION, "Effettuare il logout prima di un nuovo sign in.");
                    break;
                case 14:
                    writeToClient(sd, 42, "Hai trovato il messaggio di DEBUG.");
                    break;
                case C_CREATEROOM:
                    //printf("\t\tDEBUG_SD%d: <Crea stanza> %d:%s\n", sd, signal_num, incoming);
                    room_ID = createNewRoom(sd, room_list);
                    //sprintf(outgoing, "Stanza creata con ID %d", room_ID);
                    signal_num = joinRoom(sd, room_ID, room_list, player, outgoing);
                    writeToClient(sd, signal_num, outgoing);
                    printf("\t\tDEBUG_SD%d: <Crea stanza> %d:%s\n", sd, signal_num, incoming);
                    // Chiusura del threadService in caso di successo
                    if (signal_num == 50) {
                        return 0;
                    }
                    break;
                case C_JOINROOM:
                    //printf("\t\tDEBUG_SD%d: <Entra stanza> %d:%s\n", sd, signal_num, incoming);
                    room_ID = 1; //DEBUG
                    signal_num = joinRoom(sd, room_ID, room_list, player, outgoing);
                    writeToClient(sd, signal_num, outgoing);
                    // Chiusura del threadService in caso di successo
                    if (signal_num == 50) {
                        return 0;
                    }
                    break;
                case C_LISTROOM:
                    //printf("\t\tDEBUG_SD%d: <Lista stanze> %d:%s\n", sd, signal_num, incoming);
                    //getRoomList();
                    writeToClient(sd, 53, "Lista delle stanze");
                    break;
                case C_LOGOUT:
                    //printf("\t\tDEBUG_SD%d: <Logout> %d:%s\n", sd, signal_num, incoming);
                    //signal_num = logout();
                    destroyPlayerNode(player);
                    player = NULL;
                    signal_num = 51;
                    writeToClient(sd, signal_num, "Nuovo Login");
                    break;
                case C_SELECTWORD:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_GUESSSKIP:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                case C_EXITROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                    writeToClient(sd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
            }
            fflush(stdout);
        }
    }

    /* La funzione inizia la terminazione finale. Dopo di essa dovra essere stato deallocato il nodo del giocatore e
     * dovrà essere chiusa la socket associata. */
    /*if(player != NULL) {
        destroyPlayerNode(player);
    }*/

    close(sd);
    printf("\t\tSERVICE_SD%d: service thread has ended.\n", sd);
    fflush(stdout);
    return 0;
}

// Creazione del thread in stato detached.
pthread_t createNewService(int sd2, struct room_node** room_list) {
    /*printf("DEBUG: Input for createNewService sd:%d.\n",sd2);
    fflush(stdout);*/
    int flag;
    pthread_t tid;

    struct service_arg args;
    args.sd = sd2;
    args.room_list = room_list;
    args.player = NULL;
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrService, &args)) {
        printf(":THREAD CREATION ERROR:\n");
        close(sd2);
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il main terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewService. */
    while(args.flag == 0) {
        usleep(REFRESHCONSTANT);
    }

    printf("MAIN: Service thread created with sd:%d.\n", sd2);
    fflush(stdout);
    return tid;
}

//  Creazione di un nuovo thread service in stato detached, che recupera le informazioni del precedente thread.
pthread_t rebuildService(struct player_node* player, struct room_node** room_list) {
    pthread_t tid;

    struct service_arg args;
    args.sd = player->player_socket;
    args.room_list = room_list;
    args.player = player;
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrService, &args)) {
        printf(":THREAD CREATION ERROR:\n");
        //close(sd2);
        //pthread_cancel(pthread_self());
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il main terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti, che verrebbero persi con la chiusura del record dello stack di attivazione di createNewService. */
    while(args.flag == 0);

    //printf("SERVICE_SD%d: Service thread created with sd:%d.\n", sd2);
    //fflush(stdout);
    return tid;
}



/*int createServiceSocket(char addr_str[], struct sockaddr_un* service_addr) {
    int sd;

    service_addr->sun_family = PF_LOCAL;
    strcpy(service_addr->sun_path, addr_str);

    // Apertura del Socket.
    if ((sd = socket(PF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        perror(":SOCKET CREATION");
        exit(1);
    }
    // Bind dell'address.
    if(bind(sd, (struct sockaddr*)service_addr, sizeof(*service_addr)) < 0) {
        perror(":SOCKET BINDING");
        close(sd1);
        exit(1);
    }

    return sd;
}

int destroyServiceSocket(int service_sd);*/