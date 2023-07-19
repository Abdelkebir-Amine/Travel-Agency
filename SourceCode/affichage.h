void afficherCelluleReservation(struct Cellule_reservation *P){
	printf("\n");
	//printf("ID client: %d\n",P->client_id);
	printf("ID reservation: %d\n",P->reservation_id);
			
	if(P->type_reservation == 0)
		printf("Type reservation: Economique\n");
	else if(P->type_reservation == 1)
		printf("Type reservation: Affaire\n");
			
	if(P->etat_billet == 0)
		printf("Billet: Non Imprimee\n");
	else if(P->etat_billet == 1)
		printf("Billet: Imprimee\n");
				
	if(P->assurance == 0)
		printf("Assurance: Non\n");
	else if(P->assurance == 1)
		printf("Assurance: Oui\n");
				
	printf("Date Reservation: ");
	afficherDate(P->date_reservation,1);
	printf("\n");	
}

void afficherListeReservation(struct Liste_reservation *LR){
	struct Cellule_reservation *P=LR->tete;
	int  num_vol,n=0;
	
	printf("\n---------------------Affichage Information De Reservation Sur Un Vol---------------------\n");
	
	while(!printf("\nDonner le numero du vol que vous souhaitez afficher sa liste de reservation: ") ||scanf("\n%d%c",&num_vol,&term) != 2 || term != '\n'){
		printf("\nNumero invalide\n");
		while(getchar() != '\n');
	}
	
	while(P != NULL){
		if(P->num_vol == num_vol){
			afficherCelluleReservation(P);
			printf("\n\t\t-----------------------\n");
			n++;
		}
		P = P->suivant;
	}
	if(n == 0)
		printf("\nAucune reservation concernant ce vol\n");
}

void afficherReservationClient(struct Liste_reservation *LR, int client_id){
	struct Cellule_reservation *P=LR->tete;
	
	while(P != NULL){
		if(P->client_id == client_id){
			afficherCelluleReservation(P);
			printf("\n-----------------------\n");
		}
		P = P->suivant;
	}
}

void afficherReservationToutClient(struct Liste_reservation *LR, struct Liste_client *LC){
	int i,n = nombreClients(LC);
	printf("\n---------------------Affichage Reservations Pour Chaque Client---------------------\n");
	printf("\n%d clinets\n",n);
	for(i=1; i<=n; i++){
		printf("\n---------------------Client %d---------------------\n",i);
		afficherReservationClient(LR,i);
	}

//Affiche tout les reservation du client i. i de 1 jusqu'a nombre des clients 
//(id du client ne peut pas etre supperieur au nombre des client.)
//(l'id du client est affecté automatiquement lors de l'ajout d'un client)
}

void afficherBenifices(){
	printf("\nBenifices Agence: ### %lf #### Dinars\n",benifices_agence);
}

void affichernbBillet(struct Liste_reservation *LR){
	struct Date d;
	
	printf("\n--------------------------Nombre billets emis pour une journee--------------------------\n");
	lireDate(&d);
	printf("\nNombre billets emis au ");afficherDate(d,1);printf(": %d\n",nbBillets(LR,d));
}

