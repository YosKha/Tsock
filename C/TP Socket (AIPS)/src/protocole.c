#include "interface.c"
#define MAX_WAITING_THREAD 1000
#define MAX_LENGTH_BUFFER_MESSAGE 1000
#include <limits.h>

int counter_messsage = 0;


void udpSource(const char *nom_station, const int PORT, int nb_messages, int lg_messages){
    int sock; 
    struct sockaddr_in adr_distant;
    int lg_adr_distant = sizeof (adr_distant);

    char *message = malloc(10*sizeof(char));
    int lg_tot_messages = lg_messages+5;

    sock = creationSocketLocal(SOCK_DGRAM);
    creationAdresseDistant(&adr_distant, lg_adr_distant, nom_station, PORT);

    // envoi de tous les messagee
    for(int i = 0; i<nb_messages; i++) {
        int num_message = counter_messsage + i + 1;
        construire_message(num_message, lg_messages, message);

        // envoie du message
        int lg_emis = envoyerMessage(sock, message, lg_tot_messages, (struct sockaddr *)&adr_distant, lg_adr_distant);

        //vérification envoi du correct du messaga
        if(lg_emis == lg_tot_messages){
            printf("SOURCE : Envoie n°%d (%d) [", num_message, lg_messages);
            printf("%s", message);
            printf("]\n");
        }else{
            printf("Erreur : le message n'a pas été envoyé en intégralité\n");
            exit(-1);
        }  
    }

    close(sock);
}

void udpPuit(const int PORT, int nb_messages, int lg_messages){

    // déclaration socket et adresse
    int sock; 
    struct sockaddr_in adr_local;
    int lg_adr_local = sizeof(adr_local);
    struct sockaddr* adr_emetteur = malloc(sizeof(struct sockaddr));
    int lg_adr_emetteur = sizeof(adr_emetteur);

    // déclaration message à recevoir
    int lg_max_message = lg_messages+5;
    char *buf_message = malloc(lg_max_message*sizeof(char));
    
    sock = creationSocketLocal(SOCK_DGRAM);
    creationAdresseLocal(&adr_local, lg_adr_local, PORT);
    adresserSocket(sock, (struct sockaddr *) &adr_local, lg_adr_local); // bind de sock et adr_local
   
    
    int condition;
    int i = 0;
    if(nb_messages == -1){
        condition = 1; // Condition pour une boucle infini => TOUJOURS à attente 
    }else{
        condition = i<nb_messages;
    }
    // Reception des messages
    while(condition){
        int lg_recu = attenteReceptionMessage(sock, buf_message, lg_max_message, 0, adr_emetteur, &lg_adr_emetteur);
        printf("PUITS : Reception n°%d (%d) [", i+1, lg_recu - 5);
        for(int i = 0; i<lg_recu; i++){
            printf("%c", buf_message[i]);
        }
        printf("]\n");
        i++;
        if(nb_messages != -1){
            condition = i<nb_messages;
        }
    }
    close(sock);
}


void tcpSource(const char* nom_station, const int PORT, int nb_messages, int lg_messages){
    // déclaration adresse et socket
    int sock;
    struct sockaddr_in adr_serv;
    int lg_adr_serv = sizeof(adr_serv);

    // déclaration message
    int lg_tot_message = lg_messages + 5;
    char *message = malloc(lg_tot_message*sizeof(char));
    int lg_envoyee;

    sock = creationSocketLocal(SOCK_STREAM);
    creationAdresseDistant(&adr_serv, lg_adr_serv, nom_station, PORT);

    int counterError = 0;
    while(connect(sock, (struct sockaddr *)&adr_serv, lg_adr_serv) == -1 && counterError<100){
        counterError++;
    }
    if (counterError == 1){
        close(sock);
        perror("Erreur : nombre de tentatives maximales de connection atteinte");
        exit(-1); 
    }
    // envoi des messages
    for(int i = 0; i<nb_messages; i++){
        int num_message = counter_messsage + i + 1;
        construire_message(num_message, lg_messages, message);
        if ((lg_envoyee = send(sock, message, lg_tot_message, 0)) == -1){
            close(sock);
            perror("---> Erreur : envoie de message avec TCP");
            exit(-1); 
        }

        //affichage messages envoyées
        if(lg_envoyee == lg_tot_message){
            printf("SOURCE : Envoie n°%d (%d) [", i+1, lg_messages);
            for(int i = 0; i<lg_tot_message; i++){
                printf("%c", message[i]);
            }
            printf("]\n");
        }else{
            printf("Erreur : le message n'a pas été envoyé en intégralité\n");
            exit(-1);
        } 
        sleep(1);
    }

    shutdown(sock, 2);
    close(sock);
}

void tcpPuit(const int PORT, int nb_messages, int lg_messages){
    int sock;
    int connection_socket; 
    // adresse du server
    struct sockaddr_in adr_local;
    int lg_adr_local = sizeof(adr_local);
    // adresse du futur client
    struct sockaddr_in adr_client;
    int lg_adr_client = sizeof(adr_client);

    // déclaration message à recevoir
    int lg_max_message = lg_messages + 5;
    char *buf_message = malloc(lg_max_message *sizeof(char));
    int lg_recu = -1;

    int max;
    if (nb_messages < 0){
        max = INT_MAX;
    }else {
        max = nb_messages;
    }

    sock = creationSocketLocal(SOCK_STREAM);
    creationAdresseLocal(&adr_local, lg_adr_local, PORT);
    adresserSocket(sock, (struct sockaddr *)&adr_local, lg_adr_local);

    if(listen(sock, MAX_WAITING_THREAD) == -1){
        perror("Erreur : création de la file d'attende du serveur");  
        exit(-1); 
    }

    while (1) {
        if ((connection_socket = accept( sock, (struct sockaddr *)&adr_client, (socklen_t *)&lg_adr_client)) == -1) {
            printf("échec du accept\n"); 
            exit(1) ;
        }
        switch (fork()) {
            case - 1 : /* il y a une erreur */
                printf("erreur fork\n") ; 
                exit(1) ;
                break;
            case 0 : /* on est dans le proc. fils */
                close(sock) ; /* fermeture socket du proc. père */
                for (int i=0 ; i < max ; i ++) {
                    if ((lg_recu = read(connection_socket, buf_message, lg_max_message)) < 0){
                        printf("échec du read\n") ; exit(1);
                    }
                    if(lg_recu > 0){
                        printf("PUITS : Reception n°%d (%d) [", i+1, lg_messages);
                        for(int i = 0; i<lg_recu; i++){
                            printf("%c", buf_message[i]);
                        }
                        printf("]\n");
                    }

                    
                }
                exit(0) ;
                break;
            default : /* on est dans le proc. père */
                close(connection_socket) ; /* fermeture socket du proc. fils */
            }
        }
}   