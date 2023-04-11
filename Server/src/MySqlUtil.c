//
// Created by osboxes on 4/11/23.
//

#include "../include/MySqlUtil.h"

/* Query per vedere se username e password sono presenti nel database.
 * Restituisce 0 in caso di successo, 1 in caso di fallimento, o un intero
 * negativo per indicare una situazione d'errore. */
int selectQuery(struct mySQLConnection* LSO2223, char username[], char password[]) {
    char query[QUERYLENGTH];
    int ret_value;
    unsigned int num_fields;
    unsigned long *field_lengths;
    MYSQL_RES *result;
    MYSQL_ROW row;

    ret_value = 0;

    if(LSO2223 == NULL) {
        fprintf(stderr, ":DB_STRUCT ERROR: struct is not initialized");
        return -1;
    }

    // Composizione query.
    sprintf(query, "SELECT * "
                   "FROM Profile "
                   "WHERE username = '%s';", username);
    //printf("DEBUG: query \"%s\".\n", query);

    // Esecuzione query.
    pthread_mutex_lock(&LSO2223->db_mutex);
    if (mysql_query(LSO2223->connection, query)) {
        fprintf(stderr, ":DB_STRUCT ERROR: error while querying server \"%s\"\n", mysql_error(LSO2223->connection));
        return -1;
    }

    // Ricezione dei record.
    result = mysql_use_result(LSO2223->connection);
    if (result == NULL) {
        return 0;
    }

    // Controlla tutti i record.
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        field_lengths = mysql_fetch_lengths(result);
        if (field_lengths == NULL) {
            fprintf(stderr, ":DB_STRUCT ERROR: failed to get field lengths \"%s\"\n", mysql_error(LSO2223->connection));
            return -1;
        }
        /*if(strcmp(row[0], username) != 0) {
            printf("DEBUG: Wrong username \"%s\" - \"%s\".\n", row[0], username);
        }*/
        if (strcmp(row[1], password) != 0) {
            //printf("DEBUG: Wrong password \"%s\" - \"%s\".\n", row[1], password);
        }
        else {
            //printf("DEBUG: correct.\n");
            ret_value = 1;
        }
        /*for (int i = 0; i < num_fields; i++) {
            printf("[%.*s] ", (int) field_lengths[i], LSO2223->row[i] ? LSO2223->row[i] : "NULL");
        }*/
    }
    pthread_mutex_unlock(&LSO2223->db_mutex);

    mysql_free_result(result);

    return ret_value;
}

/* Query per inserire una nuova riga al database. Controlla che la riga
 * non sia già presente con una chiamata a selectQuery.
 * Restituisce 0 in caso di successo, 1 in caso di fallimento, o un intero
 * negativo per indicare una situazione d'errore. */
int insertQuery(struct mySQLConnection* LSO2223, char username[], char password[]) {
    char query[QUERYLENGTH];
    int ret_value;
    unsigned int num_fields;
    unsigned long *field_lengths;
    MYSQL_RES *result;
    MYSQL_ROW row;

    ret_value = 0;

    if(LSO2223 == NULL) {
        fprintf(stderr, ":DB_STRUCT ERROR: struct is not initialized");
        return -1;
    }

    // Esecuzione query di controllo per verificare che il campo non esista.

    // Composizione query.
    sprintf(query, "INSERT INTO Profile "
                   "VALUES ('%s', '%s');", username, password);
    //printf("DEBUG: query \"%s\".\n", query);

    // Esecuzione query.
    pthread_mutex_lock(&LSO2223->db_mutex);
    if (mysql_query(LSO2223->connection, query)) {
        fprintf(stderr, ":DB_STRUCT ERROR: error while querying server \"%s\"\n", mysql_error(LSO2223->connection));
        if((ret_value = mysql_errno(LSO2223->connection)) == 1062) {
            //printf("Duplicate Key\n");
            ret_value = 0;
        }
        else {
            //printf("%d\n", ret_value);
            ret_value = -1;
        }
    }
    else {
        ret_value = 1;
    }
    pthread_mutex_unlock(&LSO2223->db_mutex);

    return ret_value;
}

/* Inizializzatore della connessione col database.
 * Alloca dinamicamente lo spazio per la struttura della connessione.
 * Restituisce il puntatore alla struttura appena creata */
struct mySQLConnection* establishDBConnection(void) {
    struct mySQLConnection* LSO2223 = NULL;
    while(LSO2223 == NULL) {
        if ((LSO2223 = (struct mySQLConnection *) malloc(sizeof(struct mySQLConnection))) == NULL) {
            perror(":MALLOC DBCONNECTION");
            exit(1);
        } else {
            /**/printf("MAIN: Inizializzazione automatica connessione al database.");
            strcpy(LSO2223->hostname, "localhost");
            strcpy(LSO2223->port, "3306");
            strcpy(LSO2223->username, "osboxes");
            strcpy(LSO2223->password, "password");
            strcpy(LSO2223->database, "LSO2223_Database");/**/

            /*printf("MAIN: Inizializzazione connessione al database. Prego inserire i dati richiesti...\n");

            printf("\t\tIndirizzo del server: ");
            fgets(LSO2223->hostname, sizeof(LSO2223->hostname), stdin);
            LSO2223->hostname[strlen(LSO2223->hostname) - 1] = '\0';

            printf("\t\tPorta del server: ");
            fgets(LSO2223->port, sizeof(LSO2223->port), stdin);
            LSO2223->port[strlen(LSO2223->port) - 1] = '\0';

            printf("\t\tUsername: ");
            fgets(LSO2223->username, sizeof(LSO2223->username), stdin);
            LSO2223->username[strlen(LSO2223->username) - 1] = '\0';

            printf("\t\tPassword: ");
            fgets(LSO2223->password, sizeof(LSO2223->password), stdin);
            LSO2223->password[strlen(LSO2223->password) - 1] = '\0';

            printf("\t\tDatabase: ");
            fgets(LSO2223->database, sizeof(LSO2223->database), stdin);
            LSO2223->database[strlen(LSO2223->database) - 1] = '\0';*/

            /* Inizializzazione della connessione */
            LSO2223->connection = mysql_init(NULL);
            if (LSO2223->connection == NULL) {
                fprintf(stderr, ":DB_CONN_OBJ FAILED TO INITIALIZE: %s\n", mysql_error(LSO2223->connection));
                printf("MAIN: Tentativo di connessione fallito. Riprovare.\n");
                free(LSO2223);
                LSO2223 = NULL;
                sleep(10);
                continue;
            }

            /**/printf("\nTentativo di connessione con le seguenti credenziali:\n"
                   "\t\tHostname: %s\n"
                   "\t\tPort: %s\n"
                   "\t\tUsername: %s\n"
                   "\t\tPassword: %s\n"
                   "\t\tDatabase: %s\n",
                   LSO2223->hostname, LSO2223->port, LSO2223->username, LSO2223->password, LSO2223->database); /**/

            /* Tentativo di connessione */
            if (!mysql_real_connect(LSO2223->connection, LSO2223->hostname, LSO2223->username, LSO2223->password,
                                    LSO2223->database,
                                    atoi(LSO2223->port), NULL, 0)) {
                fprintf(stderr, ":DB_SERVER CONNECT ERROR: %s\n", mysql_error(LSO2223->connection));
                printf("MAIN: Tentativo di connessione fallito. Riprovare.\n");
                free(LSO2223);
                LSO2223 = NULL;
                sleep(10);
                continue;
            }

            printf("\nMAIN: Connessione riuscita.\n");
        }
    }
    pthread_mutex_init(&LSO2223->db_mutex, NULL);
    return LSO2223;
}

/*
#include "MySqlUtil.c"

int main(int argc, char *argv[])
{
    char temp_buf[USERNAMELENGHT];
    char username[USERNAMELENGHT];
    char password[USERNAMELENGHT];
    int count, temp;

    struct mySQLConnection* LSO2223 = NULL;

    LSO2223 = establishDBConnection();

    count = 5;
    while(count--) {
        printf("\t\tUsername: ");
        fgets(username, sizeof(username), stdin);
        username[strlen(username) - 1] = '\0';

        printf("\t\tPassword: ");
        fgets(password, sizeof(password), stdin);
        password[strlen(password) - 1] = '\0';

        printf("\t\tMAIN: Select 1 for SelectQuery, 0 for InsertQuery\n"
               "\t\tMode: ");
        fgets(temp_buf, sizeof(temp_buf), stdin);
        temp_buf[strlen(temp_buf) - 1] = '\0';
        temp = atoi(temp_buf);
        if(temp) {
            if((temp = selectQuery(LSO2223, username, password)) == 1)
                printf("MAIN: Utente presente.\n");
            else if(temp == 0)
                printf("MAIN: Utente non presente.\n");
            else
                printf("MAIN: Errore del select.\n");
        }
        else if(temp == 0) {
            if((temp = insertQuery(LSO2223, username, password)) == 1)
                printf("MAIN: Inserimento riuscito.\n");
            else if(temp == 0)
                printf("MAIN: Inserimento non riuscito.\n");
            else if(temp == 1062)
                printf("MAIN: Valore duplicato.\n");
            else
                printf("MAIN: Errore del insert.\n");
        }
        else {
            printf("MAIN: Comando non riconosciuto\n.");
        }
        printf("\n");
    }

    free(LSO2223);
    mysql_close(LSO2223->connection);

    return 0;
}
*/