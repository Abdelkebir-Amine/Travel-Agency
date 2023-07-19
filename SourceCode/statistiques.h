int max5(int n1, int n2, int n3, int n4, int n5){
	int i,max,t[5] = {n1,n2,n3,n4,n5};
	max = t[0];
	for(i=1; i<5; i++)
		if(t[i] > max)
			max = t[i];
	return max;
}

void destinationPlusVendu(struct Listes *L){
	struct Cellule_reservation *P=L->LR->tete;
	struct Cellule_vol *PV;
	struct Date d;
	int dest,n1=0,n2=0,n3=0,n4=0,n5=0;
	printf("\n--------------------------Destination plus vendu--------------------------\n");
	
	lireDate(&d);
	while(P != NULL){
		if(diffDate(d,P->date_impression) == 0 && P->etat_billet == 1){
			PV = getVol(L->LV,P->num_vol);
			switch(PV->destination){
				case 1: n1++;
					break;
				case 2: n2++;
					break;
				case 3: n3++;
					break;
				case 4: n4++;
					break;
				case 5: n5++;
					break;
			}
		}		
		P = P->suivant;
	}
	printf("\nDestinations plus vendu le ");afficherDate(d,1);printf(":\n\n");

	if(max5(n1,n2,n3,n4,n5) == n1 && max5(n1,n2,n3,n4,n5) != 0)
		printf("\tParis");	
	
	if(max5(n1,n2,n3,n4,n5) == n2 && max5(n1,n2,n3,n4,n5) != 0)
		printf("\tMarseille");	
	
	if(max5(n1,n2,n3,n4,n5) == n3 && max5(n1,n2,n3,n4,n5) != 0)
		printf("\tMadrid");	
	
	if(max5(n1,n2,n3,n4,n5) == n4 && max5(n1,n2,n3,n4,n5) != 0)
		printf("\tBarcelone");
	
	if(max5(n1,n2,n3,n4,n5) == n5 && max5(n1,n2,n3,n4,n5) != 0)
		printf("\tBerlin");		
	
	if(max5(n1,n2,n3,n4,n5) == 0){
		printf("\nAcune destination n'est vendu le ");afficherDate(d,1);printf("\n");
	}
	printf("\n");
	
}

void classerClients(struct Listes *L){
	struct Cellule_reservation *P=L->LR->tete;
	struct Cellule_vol *PV;
	struct Cellule_client *PC;
	struct Date d1, d2;
	int i,j,indice_max=0,client_id,num_vol,type_reservation,assurance,nb_clients=nombreClients(L->LC);
	float tmp_indice, tmp_montant,remise=0,montant_vol=0,T[2][100];
	
	printf("\n---------------------------Classer clients---------------------------\n");
	/*Précision de l'intervalle de la recherche*/
	printf("\n-------Donner l'intervalle de la recherche-------\n");
	printf("-------Date 1-------");
	lireDate(&d1);
	printf("-------Date 2-------");
	lireDate(&d2);
	/*Fin précision de l'intervalle de la recherche*/
	
	/*Initialiser la matrice*/
	for(i=0; i<nb_clients; i++)
		T[0][i] = i+1;
	
	for(i=0; i<nb_clients; i++)
		T[1][i] = 0;
	/*Fin initialisation de la matrice*/
	
	while(P != NULL){
		if(diffDate(P->date_reservation,d1) >= 0 && diffDate(P->date_reservation,d2) <= 0){
			montant_vol = 0;
			client_id = P->client_id;
			num_vol = P->num_vol;
			type_reservation = P->type_reservation;
			assurance = P->assurance;
			
			PV = getVol(L->LV,num_vol);
			/*Calcul du montant payee par un client dans un vol précis*/
			if(bielletsAchetes(L->LC,client_id) % 10 == 0 && bielletsAchetes(L->LC,client_id) != 0) //10 ou 20 ou 30 ou ....
				remise = 10;
			if(type_reservation == 0)
				montant_vol += (PV->montant_totale_eco) + (PV->montant_totale_eco*PV->taxe/100) - (PV->montant_totale_eco*PV->remise/100) - (PV->montant_totale_eco*remise/100);
			else if(type_reservation == 1)
				montant_vol += (PV->montant_totale_aff) + (PV->montant_totale_aff*PV->taxe/100) - (PV->montant_totale_aff*PV->remise/100) - (PV->montant_totale_aff*remise/100);
			if(assurance == 1)
				montant_vol += PV->frais_assurance;
			/*Fin calcul du montant payee par un client dans un vol précis*/
			
			T[1][client_id-1] += montant_vol; //ajouter le montant du vol au case qui correspond au client
		}
		P = P->suivant;
	}
	
	/*Trier le tableau T selon le chiffre d'affaire apporté à l'agence*/
    for(i=0; i<nb_clients-1; i++){
    	indice_max = i;
	    for(j=i+1; j<nb_clients; j++){
	    	if(T[1][j] > T[1][indice_max])
	    		indice_max = j;
	    }
		
		tmp_indice = T[0][i];
		tmp_montant = T[1][i];
			
    	T[0][i] = T[0][indice_max];
		T[1][i] = T[1][indice_max];
		
		T[0][indice_max] = tmp_indice;
		T[1][indice_max] =  tmp_montant;	
    }	
	/*Fin trie*/
	
	/*Afficher T*/
	for(i=0; i<nb_clients; i++){
		PC = getClient(L->LC,(int)T[0][i]);
		if(PC != NULL)
			printf("\nID client: %d\nNom: %s\nPrenom: %s\nMontant payee: %f\n",(int)T[0][i],PC->nom,PC->prenom,T[1][i]);
	}
}
