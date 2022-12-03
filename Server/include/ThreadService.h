/*
* Funzioni del thread responsabile della gestione di comunicazioni tra server e client.
*/

#ifndef PROGETTOLSO202223_THREADROOM_H
#define PROGETTOLSO202223_THREADROOM_H

/* Funzione principale. Una volta richiesto e ricevuto un login da parte dell'utente, procede a gestire le sue
 * richieste e a comunicare al client di renderizzare quanto serve. */
// !!RICHIESTO UN PARSER ED UN DIZIONARIO DELLE COMUNICAZIONI!!
void* thrService(void* arg);

// Creazione del thread. Richiama pthread_create.
pthread_t createNewService(int sd2);

#endif //PROGETTOLSO202223_THREADROOM_H
