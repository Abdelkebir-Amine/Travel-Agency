#include <stdio.h>
#include <string.h>
#include <stdlib.h> //Pour la fonction malloc
#include <time.h>

#include "structures.h"
#include "date.h"
#include "client.h"
#include "avion.h"
#include "vol.h"
#include "reservation.h"
#include "affichage.h" 
#include "statistiques.h"
#include "extra.h"
#include "init.h"

void delay(int number_of_seconds) {
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds); 
    
    //Copyright Geeks4Geeks
}

void main(){
	/*-----------INITIALISATION DES LISTES-----------*/
	struct Liste_vol *LV;
	LV = (struct Liste_vol *) malloc(sizeof(struct Liste_vol));
	LV->tete = NULL;
	LV->queue = NULL;
	
	struct Liste_client *LC;
	LC = (struct Liste_client *) malloc(sizeof(struct Liste_client));
	LC->tete = NULL;
	LC->queue = NULL;
	
	struct Liste_reservation *LR;
	LR = (struct Liste_reservation *) malloc(sizeof(struct Liste_reservation));
	LR->tete = NULL;
	LR->queue = NULL;
	
	struct Liste_avion *LA;
	LA = (struct Liste_avion *) malloc(sizeof(struct Liste_avion));
	LA->tete = NULL;
	LA->queue = NULL;
	
	struct Listes *L;
	L = (struct Listes *) malloc(sizeof(struct Listes));
	L->LV = LV;
	L->LC = LC;
	L->LR = LR;
	L->LA = LA;
	
	struct Date current_date;
	int reponse,num_vol,quitter=0;
	
	/*-----------FIN INITIALISATION DES LISTES-----------*/
	
	
	/*-----------TEST DE FONCTIONNEMENT-----------*/
	currentDate(&current_date);
	printf("--------------------------------TRAVEL TOUR--------------------------------\n");
	printf("\n--------------------------------");afficherDate(current_date,1);printf("--------------------------------\n");
	
	init_listes(L);
	
	while(quitter == 0){
		printf("\n--------------------------------MENU--------------------------------\n");
		printf("\nChoisir la tache a faire: \n");
		printf("\t[0] pour quitter le programme\n\n");
		
		printf("\t[1] pour ajouter un client\n");
		printf("\t[2] pour ajouter une avion\n");
		printf("\t[3] pour ajouter un vol\n");
		printf("\t[4] pour ajouter une reservation\n\n");
		
		printf("\t[5] pour afficher les informations d'un client\n");
		printf("\t[6] pour afficher les informations d'une avion\n");
		printf("\t[7] pour afficher les informations d'un vol\n");
		printf("\t[8] pour afficher les informations d'une reservation\n\n");
		
		printf("\t[9] pour modifier un vol\n");
		printf("\t[10] pour supprimer un vol\n");
		printf("\t[11] pour annuler une reservation\n");
		printf("\t[12] pour imprimer une billet\n\n");
		
		printf("\t[13] pour afficher la liste des reservations d'un vol\n");
		printf("\t[14] pour afficher pour chaque client les information sur leurs reservations\n");
		printf("\t[15] pour afficher le solde de l'agence\n");
		printf("\t[16] pour afficher les benifices de l'agence\n");
		printf("\t[17] pour afficher le nombre des billets vendu pour une journee\n");
		printf("\t[18] pour afficher les client classee selon leur chiffre d'affaire apporte a l'agence\n");
		printf("\t[19] pour afficher la destination la plus vendu pour une journee\n");
		printf("\t[20] pour afficher les vol disponibles\n");
		
		printf("\n--------------------------------------------------------------------\n");
		
		do{
			printf("\n->");
			scanf("\n%d",&reponse);
			while ((getchar()) != '\n'); //pour éviter le boucle infini (clears the input buffer) 
		}while(reponse < 0 || reponse > 20);
		
		switch(reponse){
			case 0: quitter = 1;
					printf("\n--------------------Fermeture Programme--------------------\n");
				break;
			case 1: ajouterClient(LC);
					delay(1);
				break;
			case 2: ajouterAvion(LA);
					delay(1);
				break;
			case 3: ajouterVol(L);
					delay(1);
				break;
			case 4: reserver(L);
					delay(1);
				break;
				
			case 5: afficherClient(LC);
					delay(1);
				break;
			case 6: afficherAvion(LA);
					delay(1);
				break;
			case 7: afficherVol(LV);
					delay(1);
				break;
			case 8: afficherReservation(L);
					delay(1);
				break;
				
				
			case 9: while(!printf("\nDonner le numero de vol a modifiee: ") || scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while(getchar() != '\n');	
					}
					modifierVol(L,num_vol);
					delay(1);		
				break;
				
			case 10: while(!printf("\nDonner le numero de vol a supprimee: ") || scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while(getchar() != '\n');	
					}
					supprimerVol(LV,LR,num_vol);
					delay(1);
				break;
			case 11: annulerReservation(L);
					delay(1);
				break;
			case 12: imprimerBillet(LR,LC);
					delay(1);
				break;
			case 13: afficherListeReservation(LR);
					delay(1);
				break;
			case 14: afficherReservationToutClient(LR,LC);
					delay(1);
				break;
			case 15: printf("\nSolde Agence: ####%lf#### Dinars\n",solde_agence);
					delay(1);
				break;
			case 16: afficherBenifices();
					delay(1);
				break;
			case 17: affichernbBillet(LR);
					delay(1);
				break;
			case 18: classerClients(L);
					delay(1);
				break;
			case 19: destinationPlusVendu(L);
					delay(1);
				break;
			case 20: afficherVolDisponible(LV);
					delay(1);
				break;
				
			default : delay(1);
				continue;
		}
	}
	/*-----------FIN TEST DE FONCTIONNEMENT-----------*/	
}
