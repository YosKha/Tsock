#include "protocole.c"

int PORT;
char *nom_station;


int main (int argc, char **argv){
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int lg_message = 30;
	int source = -1 ; /* 0=puits, 1=source */
    int udp = 0;
	while ((c = getopt(argc, argv, "pn:sun:l:")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;
        case 'u':
            printf("On utilise le protocole udp\n");
            udp = 1;
            break;
		case 'l':
			lg_message = atoi(optarg);
			break;
		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1)
		printf("on est dans le source\n");
	else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}

	PORT = atoi(argv[argc-1]);
    if(source == 1){
        nom_station = (char *)argv[argc -2];
		if(udp){
			printf("SOURCE : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=UDP, dest=%s\n", lg_message, PORT, nb_message, nom_station);
			udpSource(nom_station, PORT, nb_message, lg_message);
		}else{
			printf("SOURCE : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=TCP, dest=%s\n", lg_message, PORT, nb_message, nom_station);
			tcpSource(nom_station, PORT, nb_message, lg_message);
		}
		printf("SOURCE : fin\n");
    }else{
		if(udp){
			if(nb_message == -1){
				printf("PUITS : lg_mesg_lu=%d, port=%d, nb_receptions=infini, TP=UDP\n", lg_message, PORT);
			}else{
				printf("PUITS : lg_mesg_lu=%d, port=%d, nb_receptions=%d, TP=UDP\n", lg_message, PORT, nb_message);
			}
			udpPuit(PORT, nb_message, lg_message);	
		}else{	
			if(nb_message == -1){
				printf("PUITS : lg_mesg_lu=%d, port=%d, nb_receptions=infini, TP=TCP\n", lg_message, PORT);
			}else{
				printf("PUITS : lg_mesg_lu=%d, port=%d, nb_receptions=%d, TP=TCP\n", lg_message, PORT, nb_message);
			}	
			tcpPuit(PORT, nb_message, lg_message);
		}
        printf("PUITS : fin\n");
    }
    return 0;
}