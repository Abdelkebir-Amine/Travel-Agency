void afficherClient(struct Liste_client *LC){
	struct Cellule_client *P;
	int tel;
	printf("\n----------------------Affichage Client----------------------\n");
	
	while(!printf("\nDonner le numero telephone du client a afficher: ") || scanf("\n%d%c",&tel,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while(getchar() != '\n');
	}
	P = chercherClient(LC,tel);
	
	if(P != NULL){
		printf("\tID client: %d\n\tNom: %s\n\tPrenom: %s\n\tNumero telephone: %d\n",P->client_id,P->nom,P->prenom,P->tel);
		if(P->date_reservation.annee == 0 || P->date_reservation.mois ==0 || P->date_reservation.jours == 0)
			printf("\tDate 1ere reservation: Aucune reservation\n");
		else
			printf("\tDate 1ere reservation: %d/%d/%d\n",P->date_reservation.jours,P->date_reservation.mois,P->date_reservation.annee);
		printf("\tNombre billet achetee: %d\n\tMontant totale payee: %lf\n",P->nb_reservation,P->montant_payee);
	}else{
		printf("\nAucun client n'est inscrit avec ce numero de telephone\n");
	}
}

void afficherCelluleVol(struct Liste_vol *LV, struct Cellule_vol *P, int num_vol){
	if(P != NULL){
		printf("\n\tNumero de vol: %d\n\t",num_vol);afficherDestination(LV,num_vol);printf(" (%d)",P->destination);
		printf("\n\tID avion: %d\n\tPrix economique: %.3f dinars\n\tPrix affaire: %.3f dinars",P->avion_id,P->montant_totale_eco,P->montant_totale_aff);
		printf("\n\tDepart: ");afficherDate(P->d_depart,1);printf(", %02d:%02d",P->h_depart.hh,P->h_depart.mm);
		printf("\n\tArrivee: ");afficherDate(P->d_arrivee,1);printf(", %02d:%02d",P->h_arrivee.hh,P->h_arrivee.mm);
		printf("\n\tAssurance: %.3f dinars\n\tRemise: %.2f%%\n\tTaxe: %.2f%%\n",P->frais_assurance,P->remise,P->taxe);
	}else{
		printf("\nAucun vol sous le numero %d n'est trouvee\n",num_vol);
	}
}

void afficherVol(struct Liste_vol *LV){
	struct Cellule_vol *P;
	int num_vol;
	
	printf("\n----------------------Affichage Vol----------------------\n");

	while(!printf("\nDonner le numero de vol a afficher: ") || scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while(getchar() != '\n');
	}
	P = getVol(LV,num_vol);
	
	afficherCelluleVol(LV,P,num_vol);
}

void afficherAvion(struct Liste_avion *LA){
	struct Cellule_avion *P=LA->tete;
	int avion_id;
	
	printf("\n----------------------Affichage Avion----------------------\n");
	
	while(!printf("\nDonner l'ID de l'avion a afficher: ") || scanf("\n%d%c",&avion_id,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while(getchar() != '\n');
	}
	
	while(P != NULL){
		if(P->avion_id == avion_id)
			break;
		P = P->suivant;
	}
	
	if(P != NULL){
		printf("\n\tID avion: %d",avion_id);
		printf("\n\tNombre des places economiques: %d",P->capacite_eco);
		printf("\n\tNombre des places affaires: %d\n",P->capacite_aff);
	}else{
		printf("\nAucune Avion n'est trouvee\n");		
	}
}

void afficherReservation(struct Listes *L){
	struct Cellule_reservation *P;
	struct Cellule_client *PC;
	struct Cellule_vol *PV;
	int reservation_id;
	
	printf("\n----------------------Affichage Reservation----------------------\n");
	
	while(!printf("\nDonner l'ID de la reservation a afficher: ") || scanf("\n%d%c",&reservation_id,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while(getchar() != '\n');
	}
	
	P = getReservation(L->LR,reservation_id);
	
	if(P != NULL){
		PC = getClient(L->LC,P->client_id);
		PV = getVol(L->LV,P->num_vol);
		printf("\n\tID de reservation: %d\n\tNumero de vol: %d\n\t",reservation_id,P->num_vol);afficherDestination(L->LV,P->num_vol);
		printf("\n\tID du client: %d\n\tNumero telephone du client: %d",P->client_id,PC->tel);
		printf("\n\tDate de reservation: ");afficherDate(P->date_reservation,1);
		if(P->type_reservation == 0)
			printf("\n\tType reservation: Economique");
		else if(P->type_reservation == 1)
			printf("\n\tType reservation: Affaire");
		if(P->etat_billet == 1){
			printf("\n\tEtat billet: Imprimee\n\tDate impression billet: ");afficherDate(P->date_impression,1);
		}else{
			printf("\n\tEtat billet: Non Imprimee");
		}
		if(P->assurance == 1)
			printf("\n\tAssurance: Oui\n");
		else
			printf("\n\tAssurance: Non\n");	
	}else{
		printf("\nAucune Reservation n'est trouvee\n");	
	}
}


void afficherVolDisponible(struct Liste_vol *LV){
	struct Cellule_vol *P=LV->tete;
	int dest,n=0;
	struct Date current_date;
	
	currentDate(&current_date);
	printf("\n--------------------------Les Vols Disponibles--------------------------\n");
	printf("\nDonner la destination a chercher:\n\tTaper 1 pour PARIS\n\tTaper 2 pour MARSEILLE\n\tTaper 3 pour MADRID\n\tTaper 4 pour BARCELONE\n\tTaper 5 pour BERLIN\n");
	do{
		while(!printf("->") || scanf("\n%d%c",&dest,&term) != 2 || term != '\n'){
			printf("\nNumero invalide\n");
			while((getchar()) != '\n');
		}
	}while(dest<0 || dest>5);	
	
	while(P != NULL){
		if(P->destination == dest && diffDate(P->d_depart,current_date) >= 2){
			printf("\n\t\t-----------------------\n");
			afficherCelluleVol(LV,P,P->num_vol);
			n++;
		}
		P = P->suivant;
	}
	if(n == 0)
		printf("\nAucun vol disponible vers cette destination\n");
}
