/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
#include <string.h>


void construire_message(int number, int length, char *output) {
    // Génération du numéro du message formaté sur 5 caractères
    char message_number[6];
    snprintf(message_number, sizeof(message_number), "%5d", number);
    
    // Détermination du caractère répétitif basé sur l ordre alphabétique cyclique
    char ch = ((number - 1) % 26) + 'a';
    
    // Création du message formaté
    snprintf(output, length + 6, "%s%.*s", message_number, length, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    for (int i = 5; i < length + 5; i++) {
        output[i] = ch;
    }
    output[length + 5] = '\0';
}

void afficher_message(char *message, int lg) {
    int i;
    printf("message construit : \n");
    for (i=0;i<lg;i++) printf("%c", message[i]); printf("\n");
}


/*
////////////////////////
Creation du socket local
////////////////////////
*/
int creationSocketLocal(int PROTOCOL){
    int sock;
    if ((sock = socket(AF_INET, PROTOCOL, 0)) == -1){
        perror("Erreur : creation du socket\n");
    };
    return sock;
}

/*
////////////////////////////////////
Adressage sockets
////////////////////////////////////
*/

// socket local
void creationAdresseLocal(struct sockaddr_in *adr_local, int lg_adr_local, int PORT){
    memset((char *)adr_local, 0, lg_adr_local);
    adr_local->sin_family = AF_INET; /*Domaine Internet*/
    adr_local->sin_port = PORT;
    adr_local->sin_addr.s_addr = INADDR_ANY;
}

// Socket distant
void creationAdresseDistant(struct sockaddr_in *adr_distant, int lg_adr_distant, const char* nom_station,int PORT){
    struct hostent *hp;

    memset((char *)adr_distant, 0, lg_adr_distant);
    adr_distant->sin_family = AF_INET ; /*Domaine Internet*/
    adr_distant->sin_port = PORT;
    if ((hp = gethostbyname(nom_station)) == NULL){
        perror("Erreur : Creation de l'adresse socket distant\n");
    }
    memcpy((char*) &adr_distant->sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
}

void adresserSocket(int sock, struct sockaddr *adr_local, int lg_adr_local){
    // association du socket à son adresse 
    if(bind(sock, adr_local, lg_adr_local)){
        close(sock);
        perror("Erreur : bind\n");
        exit(0);
    }
}

/*
///////////////////
Emission et recetion d'un message
///////////////////
*/
int envoyerMessage(int sock_emetteur, char *message, int lg_message,struct sockaddr *adr_destinataire, int lg_adr_destinataire){
    int lg_emis;
    if ((lg_emis = sendto(sock_emetteur, message, lg_message, 0, adr_destinataire, lg_adr_destinataire)) == -1){
        close(sock_emetteur);
        perror("Erreur : envoie du message\n");
        exit(-1);
    }; 
    return lg_emis;
}

int attenteReceptionMessage(int sock, char *buf_message, int lg_buffer, int flag, struct sockaddr *adr_emetteur, int *lg_adr_emetteur){
    int lg_recu;
    if ((lg_recu = recvfrom(sock, buf_message, lg_buffer, flag, adr_emetteur, (socklen_t *)lg_adr_emetteur)) == -1){
            close(sock);
            perror("Erreur : reception message\n");
            exit(-1);
    }
    return lg_recu;
}
