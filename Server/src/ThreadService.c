/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#include "../include/ThreadService.h"

/* Funzione del service thread. Gestisce le comunicazioni con un singolo client fino alla sua disconnessione
 * o eventuale entrata in una stanza di gioco. In quest'ultimo caso il thread viene chiuso per salvare risorse,
 * perchè sia "riavviato" all'uscita dall'utente dalla stanza.
 * Una volta ricevuto un login da parte dell'utente, procede a gestire le sue richieste e a comunicare al client
 * di renderizzare quanto serve alla sua elaborazione.
 * Riceve in ingresso la struttura service_args. Essendo un detatched thread non restituisce nulla. */
void* thrService(void* arg) {
    /*printf("DEBUG: Thread started...\n");
    fflush(stdout);*/
    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];
    char username[USERNAMELENGTH + 1];
    int sd, service_sd, out_len, signal_num, room_ID;
    struct room_node** room_list;
    struct mySQLConnection* db_connection;
    /*debug*/ char deb[USERNAMELENGTH + 1];

    /* get room list utility*/
    struct room_node* tmp;
    int control_flag;


    //signal_num = 2;

    // Copia i valori della struttura originale.
    sd = (*(struct service_arg*)arg).sd;
    db_connection = (*(struct service_arg*)arg).db_connection;
    room_list = (*(struct service_arg*)arg).room_list;
    strcpy(username, (*(struct service_arg *)arg).username);

    // La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
    (*(struct service_arg*)arg).flag = 1;
    printf("\t\tSERVICE_SD%d: initialized with socket value of \"%d\", and player value of \"%s\".\n", sd, sd, ":Sostituire con stringa giocatore:");
    fflush(stdout);

    if (username[0] == '\0') {
        signal_num = 2;
        writeToClient(sd, S_LOGINOK, S_LOGINOK_MSG);
        //printf("\t\tDEBUG_SD%d: mode new thread.\n", sd);
    }
    else {
        signal_num = S_HOMEPAGEOK;
        //printf("\t\tDEBUG_SD%d: mode rebuilt thread.\n", sd);
    }

    while(signal_num > 1) {
        while (signal_num > 1 && signal_num != 52) {
            //printf("\t\tDEBUG_SD%d: primo while.\n", sd);

            memset(incoming, '\0', sizeof(incoming));
            memset(outgoing, '\0', sizeof(outgoing));
            signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);

            printf("\t\tSERVICE_SD%d: <client> \"%d:%s\".\n", sd, signal_num, incoming);
            switch (signal_num) {
                case -1:
                    writeToClient(sd, S_COMMERROR, "Failed parsing.");
                    break;
                case -2:
                    writeToClient(sd, S_COMMERROR, "Failed write.");
                    break;
                case -3:
                    writeToClient(sd, S_COMMERROR, "Failed read.");
                    break;
                case S_DISCONNECT_ABRUPT:
                    break;
                case S_DISCONNECT:
                    writeToClient(sd, S_DISCONNECT, S_DISCONNECT_MSG);
                    break;
                case C_LOGIN:
                    //printf("\t\tDEBUG_SD%d: <Login>.\n", sd);
                    /*strcpy(incoming, "debug1debug2debug3debug4debug5de-"
                                     "debug1debug2debu;");
                                     */

                    //sprintf(deb,"debug%d-debug", sd);
                    //strcpy(incoming, deb);

                    signal_num = login(sd, incoming, username, outgoing, db_connection);
                    //printf("\t\tDEBUG_SD%d: <Login as \"%s\">.\n", sd, username);
                    writeToClient(sd, signal_num, outgoing);
                    break;
                case C_SIGNIN:
                    //printf("\t\tDEBUG_SD%d: <Signin>\n", sd);
                    //strcpy(incoming, "debug1debug2debug3debug4debug5de-debug1debug2debu;");
                    signal_num = signin(incoming, username, outgoing, db_connection);
                    //printf("\t\tDEBUG_SD%d: <Signin as \"%s\">.\n", sd, username);
                    writeToClient(sd, signal_num, outgoing);
                    break;
                case 14:
                    writeToClient(sd, 42, "Hai trovato il messaggio di DEBUG.");
                    break;
                case C_CREATEROOM: case C_JOINROOM: case C_LISTROOM: case C_LOGOUT:
                case C_SELECTWORD: case C_GUESSSKIP: case C_EXITROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                    writeToClient(sd, S_UNKNOWNSIGNAL, S_UNKNOWNSIGNAL_MSG);
            }
            fflush(stdout);
        }

        while (signal_num > 1 && signal_num != 51) {
            //printf("\t\tDEBUG_SD%d: secondo while.\n", sd);

            memset(incoming, '\0', sizeof(incoming));
            memset(outgoing, '\0', sizeof(outgoing));
            signal_num = readFromClient(sd, incoming, MAXCOMMBUFFER);

            printf("\t\tSERVICE_SD%d: <client> \"%d:%s\", outgoing \"%s\"\n", sd, signal_num, incoming, outgoing);
            switch (signal_num) {
                case -1:
                    writeToClient(sd, S_COMMERROR, "Failed parsing.");
                    break;
                case -2:
                    writeToClient(sd, S_COMMERROR, "Failed write.");
                    break;
                case -3:
                    writeToClient(sd, S_COMMERROR, "Failed read.");
                    break;
                case S_DISCONNECT_ABRUPT:
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
                    printf("\t\tDEBUG_SD%d: <Crea stanza> %d:%s\n", sd, signal_num, incoming);
                    room_ID = createNewRoom(sd, room_list, db_connection);
                    sprintf(outgoing, "Stanza creata con ID %d", room_ID);
                    signal_num = joinRoom(sd, room_ID, room_list, username, outgoing);
                    writeToClient(sd, signal_num, outgoing);
                    printf("\t\tDEBUG_SD%d: <Crea stanza> %d:%s\n", sd, signal_num, outgoing);
                    // Chiusura del threadService in caso di successo
                    if(signal_num == S_ROOMOK){
                        pthread_exit(NULL);
                    }
                    break;
                case C_JOINROOM:
                    printf("\t\tDEBUG_SD%d: <Entra stanza> %d:%s\n", sd, signal_num, incoming);
                    room_ID = parserInteger(incoming, outgoing);
                    if(room_ID == S_NOPERMISSION) {
                        writeToClient(sd, room_ID, outgoing);
                    }
                    else {
                        signal_num = joinRoom(sd, room_ID, room_list, username, outgoing);
                        writeToClient(sd, signal_num, outgoing);
                        if(signal_num == S_ROOMOK) {
                            pthread_exit(NULL);
                        }
                    }
                    break;
                case C_LISTROOM:
                    //printf("\t\tDEBUG_SD%d: <Lista stanze> %d:%s\n", sd, signal_num, incoming);
                    control_flag = 1;
                    tmp = *room_list;
                    while(control_flag == 1){
                        tmp = getRoomList(tmp, outgoing, 16, &control_flag);
                        if(control_flag != -2){
                            printf("\t\tDEBUG_SD%d: <Lista stanze> %d:%s\n", sd, S_ROOMLISTOK, outgoing);
                            writeToClient(sd, S_ROOMLISTOK, outgoing);
                        }
                        else{
                            printf("\t\tDEBUG_SD%d: <Lista stanze> %d:%s\n", sd, S_SERVERERROR, S_SERVERERROR_MSG);
                            writeToClient(sd, S_SERVERERROR, S_SERVERERROR_MSG);
                        }
                        usleep(REFRESHCONSTANT);
                    }
                    break;
                case C_LOGOUT:
                    printf("\t\tDEBUG_SD%d: <Logout>\n", sd);
                    strcpy(username, "\0");
                    signal_num = S_LOGINOK;
                    printf("\t\tDEBUG_SD%d: <Logout successful. New player username is \"%s\">\n", sd, username);
                    writeToClient(sd, signal_num, "Nuovo Login");
                    break;
                case C_SELECTWORD: case C_GUESSSKIP: case C_EXITROOM:
                    writeToClient(sd, S_NOPERMISSION, S_NOPERMISSION_MSG);
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
            }
            fflush(stdout);
            /*if (signal_num == S_ROOMOK) {
                printf("\t\tSERVICE_SD%d: closed thread in favor of ROOM_ID%d.\n", sd, room_ID);
                return 0;
            }*/
        }
    }

    /* La funzione inizia la fase di terminazione a seguito di una disconnessione. E' per tanto necessario deallocare
     * il nodo giocatore e chiudere la socket. */

    close(sd);
    printf("\t\tSERVICE_SD%d: service thread has been closed.\n", sd);
    fflush(stdout);
    return 0;
}

/* Pseudo-costruttore del thread di handling delle richieste lato client, in stato detatched.
 * Riceve in input il puntatore alla testa della lista delle stanze ed il nuovo socket descriptor accettato
 * dal main thread. Restituisce il thread id appena generato. */
pthread_t createNewService(int sd2, struct room_node** room_list, struct mySQLConnection* db_connection) {
    /*printf("DEBUG: Input for createNewService sd:%d.\n",sd2);
    fflush(stdout);*/
    int flag;
    pthread_t tid;

    struct service_arg args;
    args.sd = sd2;
    args.db_connection = db_connection;
    args.room_list = room_list;
    strcpy(args.username, "\0");
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrService, &args)) {
        fprintf(stderr, ":THREAD CREATION ERROR: unable to create new service thread. Closing socket.\n");
        close(sd2);
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il main terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti. */
    while(args.flag == 0) {
        usleep(REFRESHCONSTANT);
    }

    printf("MAIN: Service thread created with sd:%d.\n", sd2);
    fflush(stdout);
    return tid;
}

/* Pseudo-costruttore del thread di handling delle richieste lato client, in stato detatched.
 * Simula un recupero dell'elaborazione di un service thread precedentemente chiuso, ma in realtà è diverso.
 * Riceve in input il puntatore alla testa della lista delle stanze ed il giocatore in uscita dalla stanza
 * di gioco. Restituisce il thread id appena generato. */
pthread_t rebuildService(struct player_node* player, struct room_node** room_list, struct mySQLConnection* db_connection) {
    pthread_t tid;

    struct service_arg args;
    args.sd = player->player_socket;
    args.db_connection = db_connection;
    args.room_list = room_list;
    strcpy(args.username, player->username);
    args.flag = 0;

    //printf("DEBUG: Creation of detatched thread...\n");
    if (pthread_create(&tid, NULL, thrService, &args)) {
        fprintf(stderr, ":THREAD REBUILD ERROR: unable to re-create new service thread. Closing player socket.\n");
        close(player->player_socket);
        return -1;
    }

    // Detatch necessario per far sì che le risorse di ciascun thread siano liberate senza un join.
    pthread_detach(tid);

    /* La flag, condivisa da main e service appena creato, opera come un single-use mutex legato alla risorsa.
     * Senza di essa il room thread terminerebbe l'esecuzione della funzione prima che il service abbia copiato i valori
     * degli argomenti. */
    while(args.flag == 0);

    printf("SERVICE_SD%d: Service thread created with sd:%d.\n", args.sd, args.sd);
    fflush(stdout);
    return tid;
}