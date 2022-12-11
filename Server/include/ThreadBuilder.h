/*
* Funzioni del thread responsabile della creazione ed indicizzazione delle stanze da gioco.
*/

#ifndef PROGETTOLSO202223_THREADBUILDER_H
#define PROGETTOLSO202223_THREADBUILDER_H

/* Funzione principale. Si occupa della creazione dei thread associati stanze e della loro indicizzazione.
 * Rimane in ascolto attivo di segnalazioni di chiusura di una stanza. */
void* thrBuilder(void* arg);

// Creazione del thread una tantum dal main-thread. Richiama pthread_create.
pthread_t createBuilder();

#endif //PROGETTOLSO202223_THREADBUILDER_H
