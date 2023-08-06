//
// Created by onofrio on 04/08/23.
//

#include "../include/LogUtilServer.h"

int createLog(int room_id) {
    char temp_buffer[MAXERRORBUFFER];
    char log_path[MAXERRORBUFFER-179];
    int fd;
    pid_t pid = getpid();

    sprintf(log_path, "./roomlog_%d_id%d.txt", pid, room_id);

    if( unlink(log_path) < 0) {
        if (errno != ENOENT)
        {
            yellow();
            perror(":UNLINK ERROR");
            defaultFormat();
        }
    }
    if (( fd = open(log_path, O_RDWR|O_CREAT, S_IRWXU)) < 0) {
        red();
        perror(":OPEN ERR");
        defaultFormat();
        exit(1);
    }

    sprintf(temp_buffer, "\t\t\tCLIENT OPERATIONS LOG\n\nLog successfully created at \"%s\" with FD \"%d\" for process ID \"%d\".\n", log_path, fd, pid);
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