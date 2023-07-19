int existeAvion(struct Liste_avion *LA, int avion_id){
	struct Cellule_avion *P;
	P=LA->tete;
	while(P != NULL){
		if(P->avion_id == avion_id)
			return 1;
		P = P->suivant;
	}
	return 0;
	//Retourne si une avion existe dans la liste d'avion ou pas
}

int occupeeAvion(struct Liste_vol *LV, struct Date d, int avion_id){
	struct Cellule_vol *P=LV->tete;
	
	while(P != NULL){
		if(P->avion_id == avion_id){
			if(diffDate(d,P->d_arrivee) < 2){	//apres 2 jours de l'arrivee de l'avion au destination 
				return P->num_vol;
			}
		}
		P = P->suivant;
	}
	return 0;
	//Retourne le numero de vol qui utilise l'avion à une date donnée, si l'avion est libre retourne 0
}

/* --- Ajout Avion --- */ 

void ajouterAvion(struct Liste_avion *LA){
	struct Cellule_avion *P;
	int avion_id;
	
	printf("\n--------------------------Ajout Avion--------------------------\n");
	
	P = (struct Cellule_avion *) malloc(sizeof(struct Cellule_avion));
	
	if(LA->tete == NULL){			//tester si la liste est vide
		avion_id = 0;
		LA->tete = P;
		LA->queue = P;
		P->suivant = NULL;
	}else{
		avion_id = LA->queue->avion_id;
		LA->queue->suivant = LA->queue = P;
		P->suivant = NULL;
	}
	
	P->avion_id = avion_id + 1;
	
	do{
		do{
			while(!printf("\nDonner le nombre des places economiques: ") || scanf("%d%c",&P->capacite_eco,&term) != 2 || term != '\n'){
				printf("\nLe numero entree est invalide\n");
				while((getchar()) != '\n');
			}	
		}while(P->capacite_eco < 0 || P->capacite_eco > 853);
		
		do{
			while(!printf("\nDonner le nombre des places affaires: ") || scanf("%d%c",&P->capacite_aff,&term) != 2 || term != '\n'){
				printf("\nLe numero entree est invalide\n");
				while((getchar()) != '\n');
			}
		}while(P->capacite_aff < 0 || P->capacite_aff > 853);
		
		if(P->capacite_eco+P->capacite_aff == 0){
			printf("\nUne avion doit avoir au moins une place soit economique soit affaire!\n");
		}
		
	}while(P->capacite_eco+P->capacite_aff == 0 || P->capacite_eco+P->capacite_aff > 853);
	//ne pas accepter un nombre négative des places et une avion doit avoir une place soit eco soit aff
	
	printf("\nAvion num %d ajoutee avec succee\n",P->avion_id);
 
 	//Ajouter une nouvelle avion a une Liste Avion
}
