//
// Created by osboxes on 1/13/23.
//

#include "../include/LogUtilClient.h"

// Restituisce il file descriptor del Log in caso di successo, altrimenti chiude il processo in quanto
// l'inizializzazione di questa risorsa è critica
int createLog(void) {
    char temp_buffer[MAXERRORBUFFER];
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
            perror(":LOG WRITE ERROR");
            defaultFormat();
        }
        else {
            result = 0;
        }
    }
    return result;
}

int printError(struct prompt_thread *prompt, char input_buf[], int error) {
    char temp_buf[MAXERRORBUFFER];
    int result;

    result = 0;

    sprintf(temp_buf, "%s%s\n", input_buf, strerror(error));

    if(pthread_mutex_trylock(&prompt->mutex) == 0) {
        red();
        printf("%s", temp_buf);
        defaultFormat();
        pthread_mutex_unlock(&prompt->mutex);
    }
    else {
        saveCursor();
        printf("\n");
        red();
        printf("%s", temp_buf);
        defaultFormat();
        loadCursor();
    }
    if(writeToLog(*prompt->log, temp_buf) < 0) result = -1;
    return result;
}

int printWarning(struct prompt_thread *prompt, char input_buf[]) {
    int result;

    result = 0;

    bold();
    yellow();
    if(pthread_mutex_trylock(&prompt->mutex) == 0) {
        printf("\n%s", input_buf);
        pthread_mutex_unlock(&prompt->mutex);
    }
    else {
        printf("\n");
        printf("\n%s(Premi invio per continuare)", input_buf);
        fflush(stdout);
    }
    defaultFormat();
    if(writeToLog(*prompt->log, input_buf) < 0) result = -1;
    return result;
}

/*int printServerError(struct prompt_thread *prompt, char input_buf[]) {
    int len, result;
    result  = -1;
    if(input_buf != NULL && prompt->log > 2) {
        len = strlen(input_buf);
        if(write(prompt->log, input_buf, len) != len) {
            red();
            perror(":WRITE ERROR");
            defaultFormat();
        }
        else {
            result = 0;
        }
    }
    return result;
}*/