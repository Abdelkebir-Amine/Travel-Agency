int verifierTelephone(int tel){
	int n=0,operateur = tel/10000000;
	while(tel != 0){
		tel /= 10;
		n++;
	}
	if(n == 8)
		if(operateur == 2 || operateur == 4 || operateur == 5 || operateur == 9)
			return 1;
	return 0;
}

struct Cellule_client *chercherClient(struct Liste_client *LC, int tel){
	struct Cellule_client *P=LC->tete;
	while(P != NULL){
		if(P->tel == tel)
			break;
		P = P->suivant;
	}
	return P;
}

struct Cellule_client *getClient(struct Liste_client *LC, int client_id){
	struct Cellule_client *P;
	P = LC->tete;
	while(P != NULL){
		if(P->client_id == client_id)
			return P;
		P = P->suivant;
	}
	return NULL;
	// Retourne un pointeur vers une cellule client dont le numero de client = client_id
}

void ajouterClient(struct Liste_client *LC){
	struct Cellule_client *P;
	int client_id,tel;
	char nom[50],prenom[50];
	
	printf("\n--------------------------Ajout Client--------------------------\n");
	
	printf("\nDonner le nom du client: ");
	scanf("%s",&nom);
	printf("\nDonner le prenom du client: ");
	scanf("%s",&prenom);
	do{
		while(!printf("\nDonner le numero telephone du client: ") || scanf("%d%c",&tel,&term) != 2 || term != '\n'){
			printf("\nNumero invalide (contient des caracteres)\n");
			while ((getchar()) != '\n');
		}
		if(!verifierTelephone(tel))
			printf("\nNumero invalide (doit contenir 8 chiffres et doit commencer par: 2 ou 4 ou 5 ou 9)\n");
	}while(!verifierTelephone(tel));
	
	if(chercherClient(LC,tel) != NULL){
		printf("\nLe client avec le numero telephone %d existe deja sous l'ID: %d. Aucun client ne sera ajoutee.\n",tel,chercherClient(LC,tel)->client_id);
	}else{
		P = (struct Cellule_client *) malloc(sizeof(struct Cellule_client));
	
		if(LC->tete == NULL){			//tester si la liste est vide
			client_id = 0;
			LC->tete = P;
			LC->queue = P;
			P->suivant = NULL;
		}else{
			client_id = LC->queue->client_id;
			LC->queue->suivant = LC->queue = P;
			P->suivant = NULL;
		}
		
		P->client_id = client_id + 1;
		P->biellets_achetes = 0;
		P->nb_reservation = 0;
		P->montant_payee = 0;
		//currentDate(&P->date_reservation);
		P->date_reservation.annee = 0000;
		P->date_reservation.jours = 00;
		P->date_reservation.mois = 00;
		P->tel = tel;
		strcpy(P->nom,nom);
		strcpy(P->prenom,prenom);
		
		printf("\nClient num %d ajoutee avec succee\n",P->client_id);	
	}
 	//Créer un nouveau client
}

int nombreClients(struct Liste_client *LC){
	struct Cellule_client *P=LC->tete;
	int n=0;
	
	while(P != NULL){
		n++;
		P = P->suivant;
	}
	return n;
}
