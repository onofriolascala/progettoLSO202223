//
// Created by onofrio on 15/12/22.
//

#ifndef PROGETTOLSO202223_DEF_H
#define PROGETTOLSO202223_DEF_H

#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <mysql/mysql.h>

// Costanti per la gestione delle socket.
#define MAXCONNECTIONS 5
#define MAXLOCALCONNECTIONS 5
#define LOCALSOCKETADDRLENGTH 100
#define SERVERPORT 5200

// Costanti per le comunicazioni.
#define MAXCOMMBUFFER 1024
#define MAXSIGNALBUF 2

// Costanti per le strutture.
#define MAXPLAYERS 8
#define MAXWORDLENGTH 32

// Costanti per le dimensioni dell'ip e delle porte
#define MAXPORT 4
#define MINPORT 1
#define MAXIP 15

// Costanti per il Login/Signin/Logout.
#define USERNAMEMINLENGTH 6
#define USERNAMELENGTH 32
#define PASSWORDMINLENGTH 10
#define PASSWORDLENGTH 16
#define QUERYLENGTH 1024

// Costanti per la Game Logic
#define WORDLENGTH 30
#define TURNTIMEOUT 1*60*1000

// Costanti per il file delle parole
#define MAXWORDSINFILE 100
#define WORDSFILEPATH "../Server/media/words.txt"

// Numero di refresh massimi consentiti
#define MAXREFRESHCONSTANT 100

// Numero di microsecondi degli usleep().
#define REFRESHCONSTANT 5000

// Segnali di comunicazione lato Server.
#define S_DISCONNECT_ABRUPT 0      // Disconnessione improvvisa

#define S_DISCONNECT 1      // Disconnessione
#define S_DISCONNECT_MSG "Disconnessione."

#define S_OK 50             // Generico messaggio di OK
#define S_OK_MSG "OK."

#define S_LOGINOK 51        // Entrata nella pagina di login consentita
#define S_LOGINOK_MSG "Entrata nella pagina di login consentita."

#define S_HOMEPAGEOK 52     // Entrata nella homepage consentita
#define S_HOMEPAGEOK_MSG "Entrata nella homepage consentita."

#define S_ROOMLISTOK 53     // Stampa delle stanze in arrivo
#define S_ROOMLISTOK_MSG "Stampa delle stanze in arrivo."

#define S_ROOMOK 54         // Entrata nella stanza consentita
#define S_ROOMOK_MSG "Entrata nella stanza consentita."

#define S_MISSEDGUESS 60    // Miss e guess degli utenti
#define S_MISSEDGUESS_MSG "Miss e guess degli utenti."

#define S_VICTORY 61        // Vittoria
#define S_VICTORY_MSG "Vittoria."

#define S_DEFEAT 62         // Sconfitta
#define S_DEFEAT_MSG "Sconfitta."

#define S_ENDOFTURN 63      // Fine del turno
#define S_ENDOFTURN_MSG "Fine del turno."

#define S_YOURTURN 64      // Inizio del turno
#define S_YOURTURN_MSG  "Inizio del turno."

#define S_CHOOSEWORD 65      // Fase di selezione della parola
#define S_CHOOSEWORD_MSG  "Sei il suzerain! Scegli la parola per iniziare."

#define S_NEWGAME 66
#define S_NEWGAME_MSG "La parola e' stata selezionata inizia una nuova partita!"

#define S_PLAYERUPDATE 67
#define S_PLAYERUPDATE_MSG

#define S_NEWHINT 68
#define S_NEWHINT_MSG "Viene dispensato un indizio sulla parola"

#define S_FULLROOM 70       // La stanza è piena
#define S_FULLROOM_MSG "La stanza è piena."

#define S_ROOMNOTFOUND 71   // La stanza non esiste
#define S_ROOMNOTFOUND_MSG "La stanza non esiste."

#define S_USERINROOM 72     // L'utente è già nella stanza
#define S_USERINROOM_MSG "Esiste già un utente con lo stesso nome all'interno della stanza."

#define S_TURNTIMEOUT 80    // Turno scaduto
#define S_TURNTIMEOUT_MSG "Turno scaduto."

#define S_NOTYOURTURN 81    // Non il turno dell'utente
#define S_NOTYOURTURN_MSG "Non il turno dell'utente."

#define S_SERVERERROR 90    // Server non disponibile
#define S_SERVERERROR_MSG "Server non disponibile."

#define S_LOGINERROR 91     // Credenziali errate
#define S_LOGINERROR_MSG "Credenziali errate."

#define S_SIGNINERROR 92    // Username già in uso
#define S_SIGNINERROR_MSG "Username già in uso."

#define S_NOPERMISSION 96   // Funzionalità non disponibile.
#define S_NOPERMISSION_MSG "Funzionalità non disponibile."

#define S_UNKNOWNSIGNAL 97  // Segnale non riconosciuto
#define S_UNKNOWNSIGNAL_MSG "Segnale non riconosciuto."

#define S_COMMTIMOUT 98     // Timout della comunicazione
#define S_COMMTIMOUT_MSG "Timout della comunicazione."

#define S_COMMERROR 99     // Generico errore di comunicazione lato server

// Segnali di comunicazione lato Client.
#define C_CONNECTION 02     // Client si connette a server
#define C_LOGIN 10          // Login
#define C_SIGNIN 11         // Registrazione
#define C_CREATEROOM 20     // Creazione di una stanza
#define C_JOINROOM 21       // Entrata in una stanza
#define C_LISTROOM 22       // Mostra lista delle stanze/refresh
#define C_LOGOUT 23         // Logout
#define C_SELECTWORD 30     // Selezione della parola
#define C_GUESSSKIP 31      // Guess o passo
#define C_EXITROOM 32       // Uscita dalla stanza
#define C_CLIENTERROR 49    // Generico errore lato client

struct mySQLConnection {
    MYSQL *connection;

    char hostname[MAXCOMMBUFFER];
    char port[MAXCOMMBUFFER];
    char username[MAXCOMMBUFFER];
    char password[MAXCOMMBUFFER];
    char database[MAXCOMMBUFFER];

    pthread_mutex_t db_mutex;
};

struct player_node{
    char username[USERNAMELENGTH+1];
    int player_socket;
    struct player_node* next;
    pthread_mutex_t playernode_mutex;
};

struct room_node{
    int id;
    char localsocket[LOCALSOCKETADDRLENGTH];
    struct player_node* player_list;
    struct player_node** playerhead_pointer;
    struct player_node** turn_pointer;
    int player_num;
    struct room_node* next;
    pthread_mutex_t roomnode_mutex;
};

struct service_arg {
    int sd;
    struct mySQLConnection* db_connection;
    struct room_node** room_list;
    char username[USERNAMELENGTH+1];
    int flag;
};

struct room_arg {
    struct mySQLConnection* db_connection;
    struct room_node** room_list;
    int room_ID;
    int flag;
};

#endif //PROGETTOLSO202223_DEF_H
