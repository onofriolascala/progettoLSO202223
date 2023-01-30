//
// Created by osboxes on 1/30/23.
//

#include "../include/ParserServer.h"
#include "../include/Def.h"

// Parser di interi in entrata dai client. Utile nello specifico per determinare l'ID richiesto per un join.
// Resituisce il numero in caso di successo, S_NOPERMISSION(96) in caso di errore, di modo da notificare il client.
int parserInteger(char incoming[], char outgoing[]) {
    char *endptr;
    int result;

    if(incoming == NULL || incoming[0] == 0) {
        fprintf(stderr, ":PARSERINT ERROR: Incoming value is null.\n");
        strcpy(outgoing, "Nessun valore inserito. Prego inserire un valore.\n");
        return S_NOPERMISSION;
    }
    result = strtol(incoming, &endptr, 10);
    if(incoming == endptr || *endptr != '\0') {
        fprintf(stderr, ":PARSERINT ERROR: Incoming value is not a number.\n");
        strcpy(outgoing, "Valore inserito errato. Inserire un valore numerico.\n");
        return S_NOPERMISSION;
    }
    return result;
}