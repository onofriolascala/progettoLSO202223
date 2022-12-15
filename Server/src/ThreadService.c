/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#include "../include/ThreadService.h"
#include "../include/ThreadRoom.h"
#include "../include/CommUtilServer.h"
#include "../include/ListHandler.h"
#include "../include/Def.h"

// Funzione principale.
void* thrService(void* arg) {
    /*printf("DEBUG: Thread started...\n");
    fflush(stdout);*/

    //struct player_node player;
    char incoming[MAXCOMMBUFFER];
    char outgoing[MAXCOMMBUFFER];
    int out_len, sd, signal_num;

    struct player_node* player;

    // Copia i valori della struttura originale.
    sd = (*(struct service_arg*)arg).sd;
    player = (*(struct service_arg*)arg).player;

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
        while (signal_num > 0 || signal_num != 52) {
            printf("\t\t\tSERVICE_SD%d: primo while.\n", sd);
            memset(incoming, 0, sizeof(incoming));
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
                    printf("\t\t\t<Login> %d:%s\n", signal_num, incoming);
                    //signal_num = login(incoming, player, outgoing);
                    player = createNewPlayerNode(sd, "Pippo");
                    //initServiceSocket()
                    strcpy(outgoing, player->username);
                    strcat(outgoing, " - ");
                    strcat(outgoing, player->service_addr);
                    signal_num = 52;
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
        while (signal_num > 0) {
            printf("\t\t\tSERVICE_SD%d: secondo while.\n", sd);
            memset(incoming, 0, sizeof(incoming));
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
                    //closeServiceSocket()
                    writeToClient(sd, 50, "OK.");
                    break;
                case 10:
                    printf("\t\t\t<Login> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Effettuare il logout prima di un nuovo login.");
                    break;
                case 11:
                    printf("\t\t\t<Registrazione> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 98, "Effettuare il logout prima di un nuovo sign in.");
                    break;
                case 14:
                    printf("\t\t\t<DEBUG> %d:%s\n", signal_num, incoming);
                    writeToClient(sd, 42, "Hai trovato il messaggio di DEBUG.");
                    break;
                case 20:
                    printf("\t\t\t<Crea stanza> %d:%s\n", signal_num, incoming);
                    //createNewRoom()
                    //joinRoom()
                    writeToClient(sd, 54, "Messaggio contenente la stanza.");
                    break;
                case 21:
                    printf("\t\t\t<Entra stanza> %d:%s\n", signal_num, incoming);
                    //joinRoom()
                    //waitRoom()
                    //fetchRoom()
                    writeToClient(sd, 54, "Messaggio contenente la stanza.");
                    //closeServiceSocket()
                    break;
                case 22:
                    printf("\t\t\t<Lista stanze> %d:%s\n", signal_num, incoming);
                    //getRoomList()
                    writeToClient(sd, 53, "Lista delle stanze");
                    break;
                case 23:
                    printf("\t\t\t<Logout> %d:%s\n", signal_num, incoming);
                    //closeServiceSocket()
                    //destroyPlayerNode()
                    signal_num = 51;
                    writeToClient(sd, signal_num, "Nuovo Login");
                    break;
                default:
                    printf("\t\t\t<ERRORE> %d: Codice di comunicazione non riconosciuto.\n", signal_num);
                    writeToClient(sd, 98, "Comunicazione non riconosciuta.");
            }
            if (signal_num == 51) {
                break;
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
pthread_t createNewService(int sd2) {
    /*printf("DEBUG: Input for createNewService sd:%d.\n",sd2);
    fflush(stdout);*/
    int flag;
    pthread_t tid;

    struct service_arg args;
    args.sd = sd2;
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
    while(args.flag == 0);

    printf("MAIN: Service thread created with sd:%d.\n", sd2);
    fflush(stdout);
    return tid;
}

//  Creazione di un nuovo thread service in stato detached, che recupera le informazioni del precedente thread.
pthread_t rebuildService(struct player_node* player) {
    pthread_t tid;

    struct service_arg args;
    args.sd = player->player_socket;
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
