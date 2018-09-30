#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void decrypte(char * message, char * reponse, int key){
/* Décrypte le message dans réponse (César)*/

    int p_key = key;

    if (key < 0){
    p_key = -key;
    }

    int i=0;
    while (message[i]!='\0'){

        reponse[i]=message[i];
        int n=0;
        while (n<p_key){

            if ((reponse[i]>='a' && reponse[i]<='z') || (reponse[i]>='A' && reponse[i]<='Z')){

                if (key > 0) {

                    if (reponse[i]=='z'){
                        reponse[i]='a';
                    }else{

                        if (reponse[i]=='Z'){
                            reponse[i]='A';
                        }else{
                            ++reponse[i];
                        }
                    }

                }else{

                    if (reponse[i]=='a'){
                        reponse[i]='z';
                    }else{

                        if (reponse[i]=='A'){
                            reponse[i]='Z';
                        }else{
                            --reponse[i];
                        }
                    }
                }

            }else{
                break;
            }
            n++;
        }
        i++;
    }
    reponse[i]='\0';
}

void crypte_opp(char * message, char * reponse, int key){
/* Décrypte le message dans reponse avec un écart de -key */

    key=-key;

	int i=0;
	while (message[i]!='\0'){

	        reponse[i]=message[i];

	        for (int n=0;n<key;n++){

				if ((reponse[i]>='a' && reponse[i]<='z') || (reponse[i]>='A' && reponse[i]<='Z')){

						if (reponse[i]=='a'){
							reponse[i]='z';
						}else{

							if (reponse[i]=='A'){
								reponse[i]='Z';
							}else{
								--reponse[i];
							}
						}

				}else{
					break;
				}
			}
			i++;
		}
		reponse[i]='\0';
}

int deja_rencontre (char lettre,char*seq){
/* Renvoie l'indice de la lettre si dans la séquence, sinon -1 */

	for (int i=0;seq[i]!='\0';i++){

		if (seq[i]==lettre){
			return i;
		}
	}
	return -1;
}

int dernier_char(char*seq){
/* Renvoie l'indice du dernier char dans la séquence */

	int i=0;
	while(seq[i]!='\0'){
		i++;
	}
	return i-1;
}

char * nouv_seq(char*seq,int i_lettre){
/* Place la lettre à la fin de la séquence et retourne la nouvelle séquence */

	char lettre=seq[i_lettre];

	int fin=dernier_char(seq);
	int n=i_lettre+1;

	while(n<=fin){
		seq[n-1]=seq[n];
		n++;
	}
	seq[n-1]=lettre;
	seq[n+1]='\0';

	return seq;
}

void crypteSeq(char*message,char*crypte){
/* Applique l'algorithme d'Alice sur le message qui sera stocké dans crypte */

	char seq[250]="\0";

	int i=0;
	int n=0;

	while (message[n]!='\0'){

		char write='\0';

		if (deja_rencontre(message[n],seq)==-1){

			write=message[n];
			seq[i]=message[n];
			i++;
		}else{

			int lettre=deja_rencontre(message[n],seq);

			if (lettre==0){
				write=seq[dernier_char(seq)];
			}else{
                write=seq[lettre-1];
			}
			strcpy(seq,nouv_seq(seq,lettre));
		}
		crypte[n]=write;
		n++;
	}
	crypte[n-1]='\0';

}

void decrypteSeq(char*message,char*decrypte){
/* Decrypte le message selon l'algorithme d'Alice */

    char seq[250]="\0";

    int i=0;
    int n=0;

    while (message[n]!='\0'){

        char write='\0';

		if (deja_rencontre(message[n],seq)==-1){
            write=message[n];
			seq[i]=message[n];
			i++;
        }else{
            int lettre=deja_rencontre(message[n],seq);

			if (lettre==dernier_char(seq)){
                write=seq[0];
                strcpy(seq,nouv_seq(seq,0));
            }else{
                write=seq[lettre+1];
                if (lettre+1!=dernier_char(seq)){
                    strcpy(seq,nouv_seq(seq,lettre+1));
                }
            }
        }
        decrypte[n]=write;
        n++;
    }
    decrypte[n]='\0';

}


int main() {
    char reponse[100000];
    char test[100000];
    char message[1916];
    char mdp[100000];
    char * a_traiter;

    // Affiche les échanges avec le serveur (false pour désactiver)
    mode_debug(true);

    // Attend après chaque échange l'appui sur entrée (false pour désactiver)
    attente_automatique(false);

    connexion(); // Connexion au serveur AppoLab
    envoyer_recevoir("login <nom> <num_etudiant>", reponse);
    printf("%s", reponse);  // pour afficher la réponse du serveur, même sans debug

	/* PROJETX */

    envoyer_recevoir("load projetX", reponse);
    envoyer_recevoir("help", reponse);

    strcpy(message,reponse);
    decrypte(message,reponse,5);

    envoyer_recevoir(reponse, reponse);

    envoyer_recevoir("depart", reponse);

    /* PLANB */

    connexion();
    envoyer_recevoir("login <nom> <num_etudiant>", reponse);
    printf("%s", reponse);

    int g_key = 0;

 	//envoyer_recevoir("unlock planB 'qui vivra verra'",reponse);

    envoyer_recevoir("load planB", reponse);
    envoyer_recevoir("help", reponse);

    strcpy(message,reponse);
    g_key = -(message[0]  - 'C');

    decrypte(message,reponse, g_key);

    envoyer_recevoir("depart", reponse);

    strcpy(mdp,"hasta la revolucion");
    crypte_opp(mdp,message,g_key);

    envoyer_recevoir(message, reponse);


	/* CRYPTESEQ */

	strcpy(test,reponse);
	a_traiter=&test[18];

	g_key = -(a_traiter[0]  - 'C');
	decrypte(a_traiter,test,g_key);
	crypteSeq(test,message);

	connexion();
    envoyer_recevoir("login <nom> <num_etudiant>", reponse);
    printf("%s", reponse);

	//envoyer_recevoir("unlock crypteSeq 'hasta la victoria siempre'",reponse);
    envoyer_recevoir("load crypteSeq", reponse);
    envoyer_recevoir("depart", reponse);
    envoyer_recevoir(message, reponse);

    decrypteSeq(reponse,message);
    envoyer_recevoir(message, reponse);

    return 0;
}
