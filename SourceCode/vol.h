int chercherVol(struct Liste_vol *LV, struct Cellule_vol *PV){
	struct Cellule_vol *P=LV->tete;
	while(P != NULL){
		if((diffDate(P->d_depart,PV->d_depart)) && (diffDate(P->d_arrivee,PV->d_arrivee)) && (diffHeure(P->h_arrivee,PV->h_arrivee)) && (diffHeure(P->h_depart,PV->h_depart))){
			if((P->avion_id == PV->avion_id) && (P->destination == PV->destination) && (P->montant_totale_eco == PV->montant_totale_eco) && (P->montant_totale_aff == PV->montant_totale_aff) && (P->remise == PV->remise) && (P->taxe == PV->taxe)){
				return P->num_vol;
			}	
		}
		P = P->suivant;
	}
	return 0;
	//Retourne le numero de vol d'une cellule vol (pour verifier si une vol existe deja ou non)
}

struct Cellule_vol *getVol(struct Liste_vol *LV, int num_vol){
	struct Cellule_vol *P;
	P = LV->tete;
	while(P != NULL){
		if(P->num_vol == num_vol)
			return P;
		P = P->suivant;
	}
	/*if(P == NULL)
		printf("\nLe vol numero %d n'existe pas.\n", num_vol);*/
	return NULL;
	// Retourne un pointeur vers une cellule vol dont le numero de vol = num_vol
}

int volReservee(struct Liste_reservation *LR, int num_vol){
	struct Cellule_reservation *P=LR->tete;
	while(P != NULL){
		if(P->num_vol == num_vol)
			return 1;
		P = P->suivant;
	}
	return 0;
	//Retourne si num_vol est reservée au moins une fois ou non (pour ne pas modifier un vol aprés qu'un client la reservée)
}

/* --- suppression de vol --- */ 

void supprimerVol(struct Liste_vol *LV, struct Liste_reservation *LR, int num_vol){
	struct Cellule_vol *P, *PP;
	int reponse;
	P = PP = LV->tete;
	printf("\n--------------------------Supression Vol--------------------------\n");
	
	while(LV->tete != NULL){
		if(LV->tete->num_vol == num_vol)
			break;
		PP = LV->tete;
		LV->tete = LV->tete->suivant;
	}
	if(LV->tete == NULL){
		printf("\nCette vol n'existe pas! Aucune vol ne sera supprimee\n");
	}else if(volReservee(LR,num_vol)){
		printf("\nIl y a des reservation concernant cet vol.\n\tTaper 0 pour annuler la suppression du vol\n\tTaper 1 pour continuer la supression du vol\n");
			do{
				while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(reponse != 0 && reponse != 1)
					printf("\nLe numero doit etre soit 0 soit 1\n");
			}while(reponse != 0 && reponse != 1);		
		
		if(reponse == 1){
			printf("\nVol numero \"%d\" supprimee avec succee\n",num_vol);
			PP->suivant = LV->tete->suivant;
			free(LV->tete);	
		}else{
			printf("\nAucun vol n'est supprimee\n");
		}
	}/*else{
		printf("\nVol numero \"%d\" supprimee avec succee\n",num_vol);
		PP->suivant = LV->tete->suivant;
		free(LV->tete);
	}*/
	LV->tete = P;
}

/* --- ajout de vol --- */ 

void ajouterVol(struct Listes *L){
	struct Cellule_vol *P;
	struct Date current_date;
	struct Heure h_arrivee;
	int num_vol,reponse;
	
	currentDate(&current_date);
	
	printf("\n--------------------------Ajout Vol--------------------------\n");
	
	P = (struct Cellule_vol *) malloc(sizeof(struct Cellule_vol));
	
	if(L->LV->tete == NULL){			//tester si la liste est vide
		num_vol = 0;
		L->LV->tete = P;
		L->LV->queue = P;
		P->suivant = NULL;
	}else{
		num_vol = L->LV->queue->num_vol;	//Récuperer le numero de le dernier vol
		L->LV->queue->suivant = P;
		L->LV->queue = P;
		P->suivant = NULL;
	}
	
	printf("\nDonner la destination:\n\tTaper 1 pour PARIS\n\tTaper 2 pour MARSEILLE\n\tTaper 3 pour MADRID\n\tTaper 4 pour BARCELONE\n\tTaper 5 pour BERLIN\n");
	do{
		while(!printf("->") || scanf("\n%d%c",&P->destination,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while((getchar()) != '\n');
		}
	}while(P->destination<0 || P->destination>5);
	
	printf("\n-------DEPART-------\n");
	do{
		lireDate(&P->d_depart);
		if(diffDate(P->d_depart,current_date) < 2 || diffDate(P->d_depart,current_date) > 356)
			printf("\nDate invalide (doit etre au moins 2 jours d'aujourd'hui et au max 356 jours d'aujourd'hui)\n");
	}while(diffDate(P->d_depart,current_date) < 2 || diffDate(P->d_depart,current_date) > 356); //Le vol ajoutee doit au maximum apres 1 an et au minimum apres 2 jours
	lireHeure(&P->h_depart);
	
	printf("\n-------ARRIVEE-------\n");
	do{
		lireDate(&P->d_arrivee);
		lireHeure(&P->h_arrivee);
		
		h_arrivee.hh = P->h_arrivee.hh;
		h_arrivee.mm = P->h_arrivee.mm;
		
		if(diffDate(P->d_arrivee,P->d_depart) == 1)
			h_arrivee.hh = P->h_arrivee.hh + 24;	// Pour comparer correctement la différence entre heures (ne pas confendre entre jour et nuit)
		if(diffHeure(h_arrivee,P->h_depart)>7*60 || diffHeure(h_arrivee,P->h_depart)<60 || diffDate(P->d_arrivee,P->d_depart)<0 || diffDate(P->d_arrivee,P->d_depart)>1)
			printf("\nDate invalide (un vol doit arriver au moins apres une heure de depart et au max apres 7 heures de depart)\n");
	}while((diffHeure(h_arrivee,P->h_depart)>7*60) || diffHeure(h_arrivee,P->h_depart)<60 || diffDate(P->d_arrivee,P->d_depart)<0 || diffDate(P->d_arrivee,P->d_depart)>1);
	//Un vol vers nos destinations ne doit pas dépasser 7 heures et ne doit pas etre négative ou inférieur à une heure
	
	do{
		while(!printf("\nDonner l'id de l'avion: ") || scanf("%d%c",&P->avion_id,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while((getchar()) != '\n');
		}
		if(!existeAvion(L->LA,P->avion_id)){
			printf("\nCette avion n'existe pas. Voulez vous ajouter une nouvelle avion?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n");
			do{
				while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(reponse != 0 && reponse != 1)
					printf("\nLe numero doit etre soit 0 soit 1\n");
			}while(reponse != 0 && reponse != 1);
			
			if(reponse == 1){
				ajouterAvion(L->LA);
				printf("\n--------------------------Continuer Ajout Vol--------------------------\n");
				/*while(!printf("\nDonner l'id de l'avion: ") || scanf("%d%c",&P->avion_id,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}*/
			}
		}else if(occupeeAvion(L->LV,P->d_depart,P->avion_id) && occupeeAvion(L->LV,P->d_depart,P->avion_id) != P->num_vol){
			printf("\nCette avion est occupee par le vol numero %d Voulez vous annuler ce vol?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n",occupeeAvion(L->LV,P->d_depart,P->avion_id));
			do{
				while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(reponse != 0 && reponse != 1)
					printf("\nLe numero doit etre soit 0 soit 1\n");
			}while(reponse != 0 && reponse != 1);	
				
			if(reponse == 1){
				P->num_vol = num_vol+1;
				supprimerVol(L->LV,L->LR,P->num_vol);
				return; // pour quitter la fonction d'ajout du vol
				//break;
			}
		}
	}while(!existeAvion(L->LA,P->avion_id) || (occupeeAvion(L->LV,P->d_depart,P->avion_id) && occupeeAvion(L->LV,P->d_depart,P->avion_id) != P->num_vol));
	//n'accepte l'id de l'avion que si elle existe et si elle est disponible (aprés 2 jours de l'arrivée de dernier vol) 
	
	do{
		printf("\nDonner le montant totale du place economique hors taxe du vol: ");
		scanf("%f",&P->montant_totale_eco);
		while ((getchar()) != '\n'); //pour éviter le boucle infini (clears the input buffer) 
	}while(P->montant_totale_eco <= 0); //le montant d'une vol ne doit pas etre negative ou gratuit
	
	do{
		printf("\nDonner le montant totale du place affaire hors taxe du vol: ");
		scanf("%f",&P->montant_totale_aff);
		while ((getchar()) != '\n'); //pour éviter le boucle infini (clears the input buffer) 
	}while(P->montant_totale_aff <= P->montant_totale_eco); //une places affaire doit etre obligatoirement plus chère qu'une place economique 
	
	
	do{
		while(!printf("\nDonner le frais de l'assurance: ") || scanf("%f%c",&P->frais_assurance,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while ((getchar()) != '\n');
		}
		if(P->frais_assurance < 0)
			printf("\nCe montant ne doit pas etre negative\n");
	}while(P->frais_assurance < 0); // frais assurence ne peut pas etre négative
	
	do{
		while(!printf("\nDonenr le taxe: ") || scanf("%f%c",&P->taxe,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while ((getchar()) != '\n');
		}
		if(P->taxe < 0)
			printf("\nCe montant ne doit pas etre negative\n");
	}while(P->taxe < 0);
	//P->taxe = 19;
	
	do{
		while(!printf("\nDonner la valeur du remise: ") || scanf("%f%c",&P->remise,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while ((getchar()) != '\n');
		}
		if(P->remise < 0)
			printf("\nCe montant ne doit pas etre negative\n");		
	}while(P->remise < 0 || P->remise > 100);	
	
	
	P->num_vol = num_vol+1;
	if(chercherVol(L->LV,P)){
		printf("\nCe vol existe deja sous le numero %d, le nouveau va etre supprimee\n",chercherVol(L->LV,P));
		supprimerVol(L->LV,L->LR,P->num_vol);
	}else{
		printf("\nVol num %d ajoutee avec succee\n",P->num_vol);	
	}
}

/* --- Modification de vol --- */ 

void modifierVol(struct Listes *L, int num_vol){
	struct Cellule_vol *P;
	struct Date current_date;
	struct Heure h_arrivee;
	int reponse;
	P=getVol(L->LV,num_vol);
	
	printf("\n--------------------------Modification Vol N: %d--------------------------\n",num_vol);
	currentDate(&current_date);

	if(P == NULL){		/*tester si la vol existe ou non*/
		printf("\nCe vol n'existe pas! Aucun vol ne sera modifiee\n");
	}else if(volReservee(L->LR,num_vol)){
		printf("\nIl y a des reservation concernant ce vol. Voulez vous le modifier quand-meme ?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n");
		do{
			while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
				printf("\nNumero invalide\n");
				while((getchar()) != '\n');
			}
			if(reponse != 0 && reponse != 1)
				printf("\nLe numero doit etre soit 0 soit 1\n");
		}while(reponse != 0 && reponse != 1);
		
		if(reponse == 1){
			printf("\nDonner la nouvelle destination:\n\tTaper 1 pour PARIS\n\tTaper 2 pour MARSEILLE\n\tTaper 3 pour MADRID\n\tTaper 4 pour BARCELONE\n\tTaper 5 pour BERLIN\n");
				do{
					while(!printf("->") || scanf("\n%d%c",&P->destination,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while((getchar()) != '\n');
					}
				}while(P->destination<0 || P->destination>5);
				
				printf("\n-------DEPART-------\n");
				do{
					lireDate(&P->d_depart);
					if(diffDate(P->d_depart,current_date) < 2 || diffDate(P->d_depart,current_date) > 356)
						printf("\nDate invalide (doit etre au moins 2 jours d'aujourd'hui et au max 356 jours d'aujourd'hui)\n");
				}while(diffDate(P->d_depart,current_date) < 2 || diffDate(P->d_depart,current_date) > 356); //Le vol ajoutee doit au maximum apres 1 an et au minimum apres 2 jours
				lireHeure(&P->h_depart);
				
				printf("\n-------ARRIVEE-------\n");
				do{
					lireDate(&P->d_arrivee);
					lireHeure(&P->h_arrivee);
					
					h_arrivee.hh = P->h_arrivee.hh;
					h_arrivee.mm = P->h_arrivee.mm;
					
					if(diffDate(P->d_arrivee,P->d_depart) == 1)
						h_arrivee.hh = P->h_arrivee.hh + 24;	// Pour comparer correctement la différence entre heures (ne pas confendre entre jour et nuit)
					if(diffHeure(h_arrivee,P->h_depart)>7*60 || diffHeure(h_arrivee,P->h_depart)<60 || diffDate(P->d_arrivee,P->d_depart)<0 || diffDate(P->d_arrivee,P->d_depart)>1)
						printf("\nDate invalide (un vol doit arriver au moins apres une heure de depart et au max apres 7 heures de depart)\n");
				}while((diffHeure(h_arrivee,P->h_depart)>7*60) || diffHeure(h_arrivee,P->h_depart)<60 || diffDate(P->d_arrivee,P->d_depart)<0 || diffDate(P->d_arrivee,P->d_depart)>1);
				//Un vol vers nos destinations ne doit pas dépasser 7 heures et ne doit pas etre négative ou inférieur à une heure
				
				do{
					while(!printf("\nDonner l'id de l'avion: ") || scanf("%d%c",&P->avion_id,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while((getchar()) != '\n');
					}
					if(!existeAvion(L->LA,P->avion_id)){
						printf("\nCette avion n'existe pas. Voulez vous ajouter une nouvelle avion?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n");
						do{
							while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
								printf("\nNumero invalide\n");
								while((getchar()) != '\n');
							}
							if(reponse != 0 && reponse != 1)
								printf("\nLe numero doit etre soit 0 soit 1\n");
						}while(reponse != 0 && reponse != 1);
						
						if(reponse == 1){
							ajouterAvion(L->LA);
							printf("\n--------------------------Continuer Ajout Vol--------------------------\n");
							/*while(!printf("\nDonner l'id de l'avion: ") || scanf("%d%c",&P->avion_id,&term) != 2 || term != '\n'){
								printf("\nNumero invalide\n");
								while((getchar()) != '\n');
							}*/
						}
					}else if(occupeeAvion(L->LV,P->d_depart,P->avion_id) && occupeeAvion(L->LV,P->d_depart,P->avion_id) != P->num_vol){
						printf("\nCette avion est occupee par le vol numero %d Voulez vous annuler ce vol?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n",occupeeAvion(L->LV,P->d_depart,P->avion_id));
						do{
							while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
								printf("\nNumero invalide\n");
								while((getchar()) != '\n');
							}
							if(reponse != 0 && reponse != 1)
								printf("\nLe numero doit etre soit 0 soit 1\n");
						}while(reponse != 0 && reponse != 1);	
							
						if(reponse == 1){
							P->num_vol = num_vol+1;
							supprimerVol(L->LV,L->LR,P->num_vol);
							return; // pour quitter la fonction d'ajout du vol
							//break;
						}
					}
				}while(!existeAvion(L->LA,P->avion_id) || (occupeeAvion(L->LV,P->d_depart,P->avion_id) && occupeeAvion(L->LV,P->d_depart,P->avion_id) != P->num_vol));
				//n'accepte l'id de l'avion que si elle existe et si elle est disponible (aprés 2 jours de l'arrivée de dernier vol) 
				
				do{
					while(!printf("\nDonner le montant totale du place economique hors taxe du vol: ") || scanf("%f%c",&P->montant_totale_eco,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while ((getchar()) != '\n');					
					}
				}while(P->montant_totale_eco <= 0); //le montant d'une vol ne doit pas etre negative ou gratuit
				
				do{
					while(!printf("\nDonner le montant totale du place affaire hors taxe du vol: ") || scanf("%f%c",&P->montant_totale_aff,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while ((getchar()) != '\n');					
					}
				}while(P->montant_totale_aff <= P->montant_totale_eco); //une places affaire doit etre obligatoirement plus chère qu'une place economique 
				
				
				do{
					while(!printf("\nDonner le frais de l'assurance: ") || scanf("%f%c",&P->frais_assurance,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while ((getchar()) != '\n');
					}
					if(P->frais_assurance < 0)
						printf("\nCe montant ne doit pas etre negative\n");
				}while(P->frais_assurance < 0); // frais assurence ne peut pas etre négative
				
				do{
					while(!printf("\nDonner le taxe: ") || scanf("%f%c",&P->taxe,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while ((getchar()) != '\n');
					}
					if(P->taxe < 0)
						printf("\nCe montant ne doit pas etre negative\n");
				}while(P->taxe < 0);
				//P->taxe = 19;
				
				do{
					while(!printf("\nDonner la valeur du remise: ") || scanf("%f%c",&P->remise,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while ((getchar()) != '\n');
					}
					if(P->remise < 0)
						printf("\nCe montant ne doit pas etre negative\n");		
				}while(P->remise < 0 || P->remise > 100);	
				printf("\nVol num %d modifee avec succee\n",P->num_vol);
		}else{
			printf("\nAucun vol n'est modifie\n");
		}
	}
}

void afficherDestination(struct Liste_vol *LV, int num_vol){
	char t[5][10] = {"Paris", "Marseille", "Madrid", "Barcelone", "Berlin"};
	struct Cellule_vol *P=LV->tete;
	
	while(P != NULL){
		if(P->num_vol == num_vol)
			break;
		P = P->suivant;
	}
	if(P == NULL)
		printf("\nAucune vol n'est trouvee\n");
	else
		printf("Destination: %s",t[P->destination-1]);
}
