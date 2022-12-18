//
// Created by onofrio on 15/12/22.
//

#ifndef PROGETTOLSO202223_DEF_H
#define PROGETTOLSO202223_DEF_H

#define MAXCONNECTIONS 5

#define MAXCOMMBUFFER 1024

#define MAXSIGNALBUF 2

#define USERNAMELENGHT 32

#define PASSWORDLENGHT 16

#define LOCALSOCKETADDRLENGHT 100

// Numero di microsecondi degli usleep()
#define REFRESHCONSTANT 5000

// Segnali di comunicazione lato Server
#define S_DISCONNECT 0      // Disconnessione del client
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

#define S_NOPERMISSION 97   // Funzionalità non disponibile.
#define S_NOPERMISSION_MSG "Funzionalità non disponibile."

#define S_UNKNOWNSIGNAL 98  // Segnale non riconosciuto
#define S_UNKNOWNSIGNAL_MSG "Segnale non riconosciuto."

#define S_COMMTIMOUT 99     // Timout della comunicazione
#define S_COMMTIMOUT_MSG "Timout della comunicazione."

// Segnali di comunicazione lato Client
#define C_LOGIN 10          // Login
#define C_SIGNIN 11         // Registrazione
#define C_CREATEROOM 20     // Creazione di una stanza
#define C_JOINROOM 21       // Entrata in una stanza
#define C_LISTROOM 22       // Mostra lista delle stanze/refresh
#define C_LOGOUT 23         // Logout
#define C_SELECTWORD 30     // Selezione della parola
#define C_GUESSSKIP 31      // Guess o passo
#define C_EXITROOM 32       // Uscita dalla stanza

struct player_node{
    char username[USERNAMELENGHT+1];
    int player_socket;
    struct player_node* next;
    pthread_mutex_t playernode_mutex;
    pthread_attr_t playernode_mutex_attr;
    pthread_cond_t playernode_mutex_cond;
};

struct room_node{
    int id;
    char localsocket[LOCALSOCKETADDRLENGHT];
    struct player_node* player_list;
    int player_num;
    struct room_node* next;
    pthread_mutex_t roomnode_mutex;
    pthread_attr_t roomnode_mutex_attr;
    pthread_cond_t roomnode_mutex_cond;
};

struct service_arg {
    int sd;
    struct room_node** room_list;
    struct player_node* player;
    int flag;
};

struct room_arg {
    struct room_node** room_list;
    int room_ID;
    int flag;
};

#endif //PROGETTOLSO202223_DEF_H
