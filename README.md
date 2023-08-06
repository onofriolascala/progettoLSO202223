# Progetto "Guess The Word" di Laboratorio di Sistemi Operativi, UniNA a.a. 2022-2023
- **Nome del Gruppo**: LSO2223_VEC_LAS_SCO
- **Autori del progetto**: Vecce Alfonso Maria, Scotti Mattia, La Scala Onofrio
- **Ordinamento**: Vecchio Ordinamento
- **Referente del progetto**: Professoressa Rossi Alessandra

## Descrizione del Progetto
A partire dalle specifiche formulate dalla traccia, il gioco "Guess the Word" sviluppato prevede che al più 8 giocatori registrati provino ad indovinare una parola selezionata da uno di loro, detto "Suzerain", tra quelle suggerite. Ogni giocatore ha a disposizione un tentativo per turno di gioco, e se nessuno ha indovinato la parola per il turno del Suzerain, viene rivelata una lettera casuale fra quelle della parola nascosta. Se un giocatore indovina la parola diventa il nuovo Suzerain, e sceglierà a sua volta una nuova parola.

L'applicazione è stata sviluppata per Ubuntu 22.04.1 LTS in linguaggio C attraverso l'IDE CLion, e compilata tramite CMake v3.24. Il programma è distribuito secondo un'architettura client-server, dove gli utenti, tramite dei client, possono connettersi ad un singolo server alla volta. L'applicazione:
- gestisce le comunicazioni tra client e server tramite comunicazioni socket su pila TCP-IP e multi-threading.
- registra e controlla gli accessi al server di gioco tramite l'integrazione di un database MySQL.
- permette agli utenti di creare stanze di gioco gestite tramite multi-threading, vedere le stanze già create ed eventualmente provare ad unirvisi.
- gestisce i turni di gioco e l'interazione asincrona fino a 8 giocatori per stanza tramite un sistema di Polling.

## Guida all'installazione
La repository di GitHub contiene sia il source code che gli eseguibili. Scaricare i file eseguibili e collocarli in una directory appositamente creata. Scaricare inoltre il file '.SQL' da importare per creare e popolare il database MySQL necessario al funzionamento dell'applicativo.

## Guida all'utilizzo
### Client
Per avviare il Client, eseguire il file "Client.out" da linea di comando. L'applicazione si articola in una serie di schermate su console con delle opzioni numerate tra cui scegliere. La selezione, così come l'inserimento delle parole, avverà in riga di comando. Per uscire dall'applicativo è sufficente premere 'CTRL-C'.
All'avvio, sarà richiesta come prima cosa di connettersi ad un server: inserire l'indirizzo IP e la porta a cui si desidera connettersi.[^1][^2]

Una volta connessi al server, sarà necessario identificarsi per poter procedere al gioco vero e proprio. Per farlo è possibile selezionare la voce "Accesso" qualora si possieda già un account, altrimenti sarà necessario selezionare prima la voce "Registrazione". Alternativamente sarà possibile disconnettersi dal server, riportanto l'utente alla schermata precedente.

Effettuato l'accesso alla homepage, sarà possibile creare una nuova stanza, visionare l'elenco delle stanze aperte da altri utenti assieme al numero di giocatori connessi, o unirsi ad una delle stanze già create attraverso il suo ID univoco. Alternativamente sarà possibile cambiare account, riportanto l'utente alla schermata precedente.

Quando un numero sufficente di utenti si unisce alla stessa stanza inizia il gioco vero e proprio:
- Se il giocatore è selezionato come Suzerain, dovrà digitare un numero corrispondente ad una delle parole proposte dal server.
- Se il giocatore non è il Surezain, rimarrà in attesa che il server gli comunichi che sia arrivato il suo turno. Qualsiasi input diverso dalla seguenza di uscita sarà ignorato.
- Arrivato il turno del giocatore, l'input da lui confermato sarà inviato al server, il quale comunicherà a tutti i partecipanti della stanza se il giocatore ha sbagliato oppure no.
- In caso di tentativo corretto, verrà comunicata al giocatore la sua vittoria e a tutti gli altri la loro sconfitta. Il vincitore diventerà il nuovo Suzerain.
- Durante il turno di altri giocatori, ciascun giocatore riceverà aggiornamenti sui tentativi degli avversari, l'entrata e l'uscita di partecipanti, e nuove lettere date come indizio alla fine di ciasun giro.
- Tutti i giocatori possono uscire dalla stanza di gioco in qualsiasi momento digitando la seguenza d'uscita "ESC + ENTER".

### Server
Per avviare il Server, eseguire il file "Server.out" da linea di comando. L'applicazione è pensata per funzionare automaticamente e richiedere quanti meno input da parte di un operatore, necessari solo all'avvio del programma. All'avvio sarà infatti richiesta come prima cosa di connettersi al database MySQL necessario alle operazioni di identificazione degli utenti. Inserire l'indirizzo IP e la porta a cui si desidera connettersi.[^1][^3]

## Testing


[^1]: Né il Client né il Server accettano ALIAS. E' sempre necessario inserire un indirizzo IP completo.
[^2]: La porta che viene aperta di default per il server è la '5200'.
[^3]: La porta che viene aperta di default per il database è la '3306'.
