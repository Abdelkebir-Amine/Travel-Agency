int bielletsAchetes(struct Liste_client *LC, int client_id){
	struct Cellule_client *P;
	P = LC->tete;
	
	while(P != NULL){
		if(P->client_id == client_id)
			return P->biellets_achetes;
		P = P->suivant;
	}
	return 0;
	//retourne le nombre des billets qu'un client a acheté
}

int nombreReservation(struct Liste_client *LC, int client_id){
	struct Cellule_client *P;
	P = LC->tete;
	
	while(P != NULL){
		if(P->client_id == client_id)
			return P->nb_reservation;
		P = P->suivant;
	}
	return 0;
	//retourne le nombre des reservations qu'un client a fait
}

int placesEcoDisponibles(struct Listes *L, int num_vol){
	struct Cellule_vol *PV;
	struct Cellule_avion *PA;
	struct Cellule_reservation *PR;
	int avion_id=0;
	int n=0; // n: nombre de places disponibles,
	
	/*Récuperer l'id de l'avion du vol*/
	PV = getVol(L->LV,num_vol);
	if(PV != NULL)
		avion_id = PV->avion_id;

	
	/*Récuperer le nombre totale des places économiques*/
	PA = L->LA->tete;
	while(PA != NULL){
		if(PA->avion_id == avion_id){
			n = PA->capacite_eco;
			break;
		}
		PA = PA->suivant;
	}
	
	/*Déterminer le nombre restant des places économiques*/
	PR = L->LR->tete;
	while(PR != NULL){
		if(PR->num_vol == num_vol){
			if(n == 0)
				return 0;
			n--;//a chaque reservation correspondante au vol, on diminue le nombres des places eco
		}
		PR = PR->suivant;
	}

	return n; //retourne le nombres des place économiques restantes
}

int placesAffDisponibles(struct Listes *L, int num_vol){
	struct Cellule_vol *PV;
	struct Cellule_avion *PA;
	struct Cellule_reservation *PR;
	int avion_id=0; // =-1 si aucune avion n'est associer à une vol
	int n=0; // n: nombre de places disponibles, =-1 si on n'a pas trouver l'avion
	
	/*Récuperer l'id de l'avion du vol*/
	PV = getVol(L->LV,num_vol);
	if(PV != NULL)
		avion_id = PV->avion_id;
	
	/*Récuperer le nombre totale des places affaires*/
	PA = L->LA->tete;
	while(PA != NULL){
		if(PA->avion_id == avion_id){
			n = PA->capacite_aff;
			break;
		}
		PA = PA->suivant;
	}		
	
	/*Déterminer le nombre restant des places affaires*/
	PR = L->LR->tete;
	while(PR != NULL){
		if(PR->num_vol == num_vol){
			if(n == 0)
				return 0;
			n--;//a chaque reservation correspondante au vol, on diminue le nombres des places aff
		}
		PR = PR->suivant;
	}

	return n; //retourne le nombres des place affaires restantes
}

void traitementArgent(struct Listes *L, int client_id, int num_vol, int type_reservation, int assurance){
	struct Cellule_client *PC=getClient(L->LC,client_id);
	struct Cellule_vol *PV=getVol(L->LV,num_vol);
	float remise,prix,frais_assurance;
	
	if(type_reservation == 0)
		prix = PV->montant_totale_eco;
	else
		prix = PV->montant_totale_aff;
	if(bielletsAchetes(L->LC,client_id)%10 == 0 && bielletsAchetes(L->LC,client_id) != 0)
		remise = 10;
	else
		remise = 0;
	if(assurance == 0)
		frais_assurance = 0;
	else
		frais_assurance = PV->frais_assurance;
		
	PC->montant_payee += prix + frais_assurance + prix*(PV->taxe/100) - prix*(PV->remise/100) - prix*remise/100;
	benifices_agence += ( prix - (prix*PV->remise/100)- (prix*remise/100) )*0.1 + frais_assurance; //0.1 = 10% du prix billet après le remises
	solde_agence += PC->montant_payee;
}

void ajouterReservationEco(struct Listes *L, int client_id, int num_vol, int type_reservation){
	struct Cellule_reservation *P;
	struct Cellule_client *PC;
	struct Cellule_vol *PV=getVol(L->LV,num_vol);
	struct Date current_date;
	int reservation_id;
	float remise=0;
	
	P = (struct Cellule_reservation *) malloc(sizeof(struct Cellule_reservation));
	
	if(L->LR->tete == NULL){			//tester si la liste est vide
		reservation_id = 0;
		L->LR->tete = P;				//pointer la tete de la liste sur la nouvelle cellule
		L->LR->queue = P;				//pointer le queue de la liste sur la nouvelle cellule
		P->suivant = NULL;
	}else{								
		reservation_id = L->LR->queue->reservation_id;	//Récuperer l'id de la derniere reservation
		L->LR->queue->suivant = L->LR->queue = P;	//pointer le queue de la liste sur la nouvelle cellule
		P->suivant = NULL;
	}
	
	P->reservation_id = reservation_id + 1;
	P->client_id = client_id;
	P->num_vol = num_vol;
	P->type_reservation = type_reservation;
	currentDate(&P->date_reservation);
	//currentDate(&P->date_impression);
	P->date_impression.annee = 0000;
	P->date_impression.mois = 00;
	P->date_impression.jours = 00;
	P->etat_billet = 0;
	
	printf("\nVoulez vous s'inscrire a une assurence? (%f dinars)\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n",PV->frais_assurance);
	do{
		while(!printf("->") || scanf("\n%d%c",&P->assurance,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while((getchar()) != '\n');
		}
		if(P->assurance != 0 && P->assurance != 1)
			printf("\nLe numero doit etre soit 0 soit 1\n");
	}while(P->assurance != 0 && P->assurance != 1);
		
	PC = getClient(L->LC,client_id);
	PC->nb_reservation++;	//incrementer nombre de reservation dans la cellule du client
	if(nombreReservation(L->LC,client_id) == 0){	//si le client n'a jamais fait une reservation
		currentDate(&PC->date_reservation);	//Date de la 1ere reservation
	}
	traitementArgent(L,client_id,num_vol,type_reservation,P->assurance);

	printf("\nReservation num %d ajoutee avec succee\n",P->reservation_id);
}

void ajouterReservationAff(struct Listes *L, int client_id, int num_vol, int type_reservation){
	struct Cellule_reservation *P;
	struct Cellule_client *PC;
	struct Cellule_vol *PV=getVol(L->LV,num_vol);
	struct Date current_date;
	int reservation_id;
	float remise=0;
	
	P = (struct Cellule_reservation *) malloc(sizeof(struct Cellule_reservation));
	
	if(L->LR->tete == NULL){			//tester si la liste est vide
		reservation_id = 0;
		L->LR->tete = P;				//pointer la tete de la liste sur la nouvelle cellule
		L->LR->queue = P;				//pointer le queue de la liste sur la nouvelle cellule
		P->suivant = NULL;
	}else{								
		reservation_id = L->LR->queue->reservation_id;	//Récuperer l'id de la derniere reservation
		L->LR->queue->suivant = L->LR->queue = P;	//pointer le queue de la liste sur la nouvelle cellule
		P->suivant = NULL;
	}
	
	P->reservation_id = reservation_id + 1;
	P->client_id = client_id;
	P->num_vol = num_vol;
	P->type_reservation = type_reservation;
	currentDate(&P->date_reservation);
	currentDate(&P->date_impression);
	P->etat_billet = 0;
	
	printf("\nVoulez vous s'inscrire a une assurence? (%f dinars)\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n",PV->frais_assurance);
	do{
		while(!printf("->") || scanf("\n%d%c",&P->assurance,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while((getchar()) != '\n');
		}
		if(P->assurance != 0 && P->assurance != 1)
			printf("\nLe numero doit etre soit 0 soit 1\n");
	}while(P->assurance != 0 && P->assurance != 1);
		
	PC = getClient(L->LC,client_id);
	PC->nb_reservation++;	//incrementer nombre de reservation dans la cellule du client
	if(nombreReservation(L->LC,client_id) == 0){	//si le client n'a jamais fait une reservation
		currentDate(&PC->date_reservation);	//Date de la 1ere reservation
	}
	traitementArgent(L,client_id,num_vol,type_reservation,P->assurance);

	printf("\nReservation num %d ajoutee avec succee\n",P->reservation_id);
}

void ajouterReservation(struct Listes *L, int client_id, int num_vol, int type_reservation){
	if(type_reservation == 0 && placesEcoDisponibles(L,num_vol) > 0){	//si le type est eco et il existe des places=>reserver
		ajouterReservationEco(L,client_id,num_vol,0);
	}else if(type_reservation == 1 && placesAffDisponibles(L,num_vol) > 0){  //si le type est aff et il existe des places=>reserver
		ajouterReservationAff(L,client_id,num_vol,1);
	}else{
		printf("\nAucune place n'est disponible\n");
	}
}

void reserver(struct Listes *L){
	struct Cellule_client *PC;
	struct Cellule_vol *PV;
	struct Date current_date;
	struct Heure h_depart;
	int tel,num_vol,type_reservation,reponse=0;
	float prix_aff,prix_eco,remise=0;
	
	currentDate(&current_date);
	
	printf("\n--------------------------Ajout Reservation--------------------------\n");
	
	if(L->LC->tete == NULL){	//Si la liste des clients est vide
		printf("\n--------------------------Aucun Client n'est inscrit a l'agence--------------------------\n");
		ajouterClient(L->LC);
		printf("\n--------------------------Continuer Ajout Reservation--------------------------\n");

		tel = L->LC->tete->tel;	
	}else{
		do{
			while(!printf("\nDonner le numero de telephone du client: ") || scanf("%d%c",&tel,&term) != 2 || term != '\n'){
				printf("\nNumero invalide\n");
				while((getchar()) != '\n');
			}
			if(!verifierTelephone(tel))
				printf("\nNumero invalide (doit contenir 8 chiffres et doit commencer par: 2 ou 4 ou 5 ou 9)\n");
		}while(!verifierTelephone(tel));
	}
	
	PC = chercherClient(L->LC,tel);	//Récuperer la cellule de client correspondant
	if(PC != NULL){	//Si le client est inscrit à l'agence
		if(bielletsAchetes(L->LC,PC->client_id) % 10 == 0 && bielletsAchetes(L->LC,PC->client_id) != 0){
			remise = 10;
			printf("\nLe client d'ID %d a un remise de 10% (%d reservation)\n",PC->client_id,bielletsAchetes(L->LC,PC->client_id));
		}
		printf("\nDonner le numero du vol\n");
		do{
			while(!printf("->") || scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
				printf("\nNumero invalide\n");
				while((getchar()) != '\n');
			}
			if(getVol(L->LV,num_vol) == NULL)
				printf("\nCe vol n'existe pas\n");
		}while(getVol(L->LV,num_vol) == NULL);	//Vérifier l'existance du vol demandé
		
		PV = getVol(L->LV,num_vol);
		if(diffDate(PV->d_depart, current_date) < 2){
			printf("\nVous ne pouvez pas faire une reservation que pour un vol apres 2 jours au moins d'aujourd'hui\n");
		}else if(diffDate(PV->d_depart, current_date) < 0){
			printf("\nCe vol a expiree\n");
		}else{	//Si la date est respectée (aprés 2 jours de la date courante au moin)
			prix_eco = PV->montant_totale_eco + (PV->montant_totale_eco*PV->taxe/100) - (PV->montant_totale_eco*PV->remise/100) - (PV->montant_totale_eco*remise/100);
			prix_aff = PV->montant_totale_aff + (PV->montant_totale_aff*PV->taxe/100) - (PV->montant_totale_aff*PV->remise/100) - (PV->montant_totale_aff*remise/100);
			printf("\nDonner le type de reservation:\n\tTaper 0 pour ECONOMIQUE(%f dinars)\n\tTaper 1 pour AFFAIRE(%f dinars)\n",prix_eco,prix_aff);
			do{
				while(!printf("->") || scanf("\n%d%c",&type_reservation,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(type_reservation < 0 || type_reservation > 1)
					printf("\nLe numero doit etre soit 0 soit 1\n");
			}while(type_reservation < 0 || type_reservation > 1);
			
			ajouterReservation(L,PC->client_id,num_vol,type_reservation);
		}
	}else{
		printf("\nAucun client n'est inscrit avec le telephone: %d, Voulez-vous ajouter un nouveau client?\n\tTpaer 0 pour NON\n\tTaper 1 pour OUI\n",tel);
		do{
			while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
				printf("\nNumero invalide\n");
				while((getchar()) != '\n');
			}
			if(reponse != 0 && reponse != 1)
				printf("\nLe numero doit etre soit 0 soit 1\n");
		}while(reponse != 0 && reponse != 1);
		
		if(reponse == 1){
			ajouterClient(L->LC);
			printf("\n--------------------------Continuer Ajout Reservation--------------------------\n");
			
			printf("\nDonner le numero du vol\n");
			do{
				while(!printf("->") || scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(getVol(L->LV,num_vol) == NULL)
					printf("\nCe vol n'existe pas\n");
			}while(getVol(L->LV,num_vol) == NULL);
			
			PV = getVol(L->LV,num_vol);
			if(diffDate(PV->d_depart, current_date) < 2){
				printf("\nVous ne pouvez pas faire une reservation que pour un vol apres 2 jours au moins d'aujourd'hui\n");
			}else if(diffDate(PV->d_depart, current_date) < 0){
				printf("\nCet vol a expiree\n");
			}else{	//Si la date est respectée (aprés 2 jours de la date courante au moin)
				prix_eco = PV->montant_totale_eco + (PV->montant_totale_eco*PV->taxe/100) - (PV->montant_totale_eco*PV->remise/100);
				prix_aff = PV->montant_totale_aff + (PV->montant_totale_aff*PV->taxe/100) - (PV->montant_totale_aff*PV->remise/100);
				printf("\nDonner le type de reservation:\n\tTaper 0 pour ECONOMIQUE(%f dinars)\n\tTaper 1 pour AFFAIRE(%f dinars)\n",prix_eco,prix_aff);
				do{
					while(!printf("->") || scanf("\n%d%c",&type_reservation,&term) != 2 || term != '\n'){
						printf("\nNumero invalide\n");
						while((getchar()) != '\n');
					}
					if(type_reservation < 0 || type_reservation > 1)
						printf("\nLe numero doit etre soit 0 soit 1\n");	
				}while(type_reservation < 0 || type_reservation > 1);
				
				ajouterReservation(L,L->LC->queue->client_id,num_vol,type_reservation); //On reserve un vol pour le dernier client ajoutee
			}
		}
		else{
			printf("\nAjout reservation annulee\n");
		}
	}
	
}

void imprimerBillet(struct Liste_reservation *LR, struct Liste_client *LC){
	struct Cellule_reservation *P;
	struct Cellule_client *PC;
	int reservation_id;
	P = LR->tete;
	
	printf("\n--------------------------Imprimer Billet--------------------------\n");
	
	while(!printf("\nDonner l'ID de la reservation: ") || scanf("\n%d%c",&reservation_id,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while((getchar()) != '\n');
	}
	
	while(P != NULL){
		if(P->reservation_id == reservation_id)
			break;
		P = P->suivant;
	}
	
	if(P != NULL){
		if(P->etat_billet == 1){
			printf("\nCette billet est deja imprimee\n");
		}else{
			P->etat_billet = 1;
			currentDate(&P->date_impression);
			PC = getClient(LC,P->client_id);
			PC->biellets_achetes++;
			printf("\nLe Biellet est imprime.\n");
		}
	}else{
		printf("\nAucune reservation n'est trouvee\n");
	}
}

struct Cellule_reservation *getReservation(struct Liste_reservation *LR, int reservation_id){
	struct Cellule_reservation *P=LR->tete;
	while(P != NULL){
		if(P->reservation_id == reservation_id)
			return P;
		P = P->suivant;
	}
	return NULL;
}

struct Cellule_reservation *getReservationPrecedente(struct Liste_reservation *LR, int reservation_id){
	struct Cellule_reservation *PP,*P=LR->tete;
	
	PP = P;
	while(P != NULL){
		if(P->reservation_id == reservation_id)
			return PP;
		PP = P;
		P = P->suivant;
	}
	return NULL;
}

int tailleListeReservation(struct Liste_reservation *LR){
	struct Cellule_reservation *P;
	int n = 0;
	
	P = LR->tete;
	while(P != NULL){
		n++;
		P = P->suivant;
	}
	return n;
}

void annulerReservation(struct Listes *L){
	struct Cellule_reservation *P,*PP;
	struct Cellule_client *PC;
	struct Cellule_vol *PV;
	struct Date current_date;
	int reservation_id,reponse;
	float remise=0;
	
	currentDate(&current_date);
	printf("\n--------------------------Annuler Reservation--------------------------\n");
	
	do{
		while(!printf("\nDonner l'id de la reservation a supprimee: ") || scanf("\n%d%c",&reservation_id,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while(getchar() != '\n');
		}
		if(reservation_id <= 0)
			printf("\nL'id de reservation ne doit pas etre negative\n");
	}while(reservation_id <= 0);
	
	P = getReservation(L->LR,reservation_id);
	
	if(P != NULL){
		PC = getClient(L->LC,P->client_id); //Récuperer l'id du client à partir de la reservation (P->client_id)
		PV = getVol(L->LV,P->num_vol);	//Récuperer le numero de vol à partir de la reservation (P->num_vol)
		
		if(diffDate(PV->d_depart,current_date) < 2){
			printf("\nVous ne pouvez pas annuler cette reservation! Une reservation doit etre annuler au moins avant 2 jours du vol\n");
		}else{
			printf("\nVous etes en train d'annuler la reservation %d. Voulez vous continuer?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n",P->reservation_id);
			do{
				while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
					printf("\nNumero invalide\n");
					while((getchar()) != '\n');
				}
				if(reponse != 0 && reponse != 1)
					printf("\nLe numero doit etre soit 0 soit 1\n");
			}while(reponse != 0 && reponse != 1);
			if(reponse == 0)
				return;
			//Récuperer le remise pour ce client (a chaque 10 billet il a un solde de 10%)
			if(bielletsAchetes(L->LC,P->client_id) % 10 == 0 && bielletsAchetes(L->LC,P->client_id) != 0) //10 ou 20 ou 30 ou ....
				remise = 10;
				
			if(P->etat_billet == 1)	//Si billet imprimée
				PC->biellets_achetes--;
				
			PC->nb_reservation--;	//diminuer le nombre de reservation du client
		
			if(P->type_reservation == 0){	//si la reservation est economique
				PC->montant_payee -= (PV->montant_totale_eco + (PV->montant_totale_eco*PV->taxe/100) - (PV->montant_totale_eco*PV->remise/100) - (PV->montant_totale_eco*remise/100));
				benifices_agence -= (PV->montant_totale_eco - (PV->montant_totale_eco*PV->remise/100)- (PV->montant_totale_eco*remise/100))*0.1;
				solde_agence -= PV->montant_totale_eco + (PV->montant_totale_eco*PV->taxe/100) - (PV->montant_totale_eco*PV->remise/100) - (PV->montant_totale_eco*remise/100);
			}else{	//si la reservation est affaire
				PC->montant_payee -= (PV->montant_totale_aff + (PV->montant_totale_aff*PV->taxe/100) - (PV->montant_totale_aff*PV->remise/100) - (PV->montant_totale_aff*remise/100));
				benifices_agence -= (PV->montant_totale_aff - (PV->montant_totale_aff*PV->remise/100)- (PV->montant_totale_aff*remise/100))*0.1;	
				solde_agence -= PV->montant_totale_aff + (PV->montant_totale_aff*PV->taxe/100) - (PV->montant_totale_aff*PV->remise/100) - (PV->montant_totale_eco*remise/100);
			}
			
			if(P->etat_billet == 1){	//si la billet est imprimée
				if(P->assurance == 0){
					printf("\nVous allez payer 50 dinars (frais annulation). Voulez vous continuer l'annulation de la reservation ?\n\tTaper 0 pour NON\n\tTaper 1 pour OUI\n");
					do{
						while(!printf("->") || scanf("\n%d%c",&reponse,&term) != 2 || term != '\n'){
							printf("\nNumero invalide\n");
							while((getchar()) != '\n');
						}
						if(reponse != 0 && reponse != 1)
							printf("\nLe numero doit etre soit 0 soit 1\n");
					}while(reponse != 0 && reponse != 1);
					
					if(reponse == 1){
						PC->montant_payee += 50;
						benifices_agence += 50;
						solde_agence += 50;
					}else{
						printf("\nReservation n'est pas annulee\n");
						return;
					}
				}
			}
			
			/*supression de la reservation*/
			PP = getReservationPrecedente(L->LR,reservation_id);
			PP->suivant = P->suivant;
			free(P);
			if(tailleListeReservation(L->LR) == 1)
				L->LR->tete = L->LR->queue = NULL;	//Si la liste contient une seule reservation, la liste pointe sur NULL
			/*fin supression de la reservation*/
			
			printf("\nReservation numero %d supprimee avec succee\n",reservation_id);
		}	
	}else{
		printf("\nReservation numero %d n'existe pas! Aucune reservation ne sera supprimee\n",reservation_id);
	}
}

int nbBillets(struct Liste_reservation *LR, struct Date d){
	struct Cellule_reservation *P=LR->tete;
	int n=0;
	
	while(P != NULL){
		if(diffDate(d,P->date_impression) == 0 && P->etat_billet == 1)
			n++;
		P = P->suivant;
	}
	return n;
}
