//
// Created by osboxes on 1/13/23.
//

#include "../include/LogUtilClient.h"

// Restituisce il file descriptor del Log in caso di successo, altrimenti chiude il processo in quanto
// l'inizializzazione di questa risorsa è critica
int createLog(void) {
    char temp_buffer[MAXCOMMBUFFER];
    int fd;

    if(unlink(CLIENTLOG) < 0) {
        if (errno != ENOENT)
        {
            yellow();
            perror(":UNLINK ERROR");
            defaultFormat();
        }
    }

    if (( fd = open(CLIENTLOG, O_RDWR|O_CREAT, S_IRWXU)) < 0) {
        red();
        perror(":OPEN ERR");
        defaultFormat();
        exit(1);
    }

    sprintf(temp_buffer, "\t\t\tCLIENT OPERATIONS LOG\n\nLog successfully created at \"%s\" with FD \"%d\".\n", CLIENTLOG, fd);

    if(writeToLog(fd, temp_buffer) < 0) exit(1);

    return fd;
}

int writeToLog(int fd, const char input_buf[]) {
    int len, result;
    result  = -1;
    if(input_buf != NULL && fd > 2) {
        len = strlen(input_buf);
        if(write(fd, input_buf, len) != len) {
            red();
            perror(":WRITE ERROR");
            defaultFormat();
        }
        else {
            result = 0;
        }
    }
    return result;
}