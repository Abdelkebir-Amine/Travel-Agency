void ajouter_client(struct Liste_client *LC, int id, int nb_billet, int nb_res, int tel, int jours, int mois, int annee, char nom[], char prenom[] ){
	struct Cellule_client *P;
	P = (struct Cellule_client *)malloc(sizeof(struct Cellule_client));
	if(LC->tete == NULL){
		LC->tete = LC->queue = P;
	}else{
		LC->queue = LC->queue->suivant = P;
	}
	P->suivant = NULL;
	P->montant_payee = 0;
	P->client_id = id;
	P->biellets_achetes = nb_billet;
	P->nb_reservation = nb_res;
	P->tel = tel;
	P->date_reservation.jours = jours;
	P->date_reservation.mois = mois;
	P->date_reservation.annee = annee;
	strcpy(P->nom,nom);
	strcpy(P->prenom,prenom);

}

void ajouter_avion(struct Liste_avion *LA, int id, int eco, int aff){
	struct Cellule_avion *P;
	P = (struct Cellule_avion *)malloc(sizeof(struct Cellule_avion));
	if(LA->tete == NULL){
		LA->tete = LA->queue = P;
	}else{
		LA->queue = LA->queue->suivant = P;
	}
	P->suivant = NULL;
	P->avion_id = id;
	P->capacite_eco = eco;
	P->capacite_aff = aff;

}

void ajouter_vol(struct Liste_vol *LV, int num, int dest, int id, float eco, float aff, float taxe, float remise, float assurance){
	struct Cellule_vol *P;
	P = (struct Cellule_vol *)malloc(sizeof(struct Cellule_vol));
	if(LV->tete == NULL){
		LV->tete = LV->queue = P;
	}else{
		LV->queue = LV->queue->suivant = P;
	}
	P->suivant = NULL;
	
	P->num_vol = num;
	P->avion_id = id;
	P->destination = dest;
	P->montant_totale_eco = eco;
	P->montant_totale_aff = aff;
	P->taxe = taxe;
	P->remise = remise;
	P->frais_assurance = assurance;
	P->d_depart.jours = 15;
	P->d_depart.mois = 2;
	P->d_depart.annee = 2019;
	P->d_arrivee.jours = 15;
	P->d_arrivee.mois = 2;
	P->d_arrivee.annee = 2019;
	P->h_depart.hh = 19;
	P->h_depart.mm = 40;
	P->h_arrivee.hh = 22;
	P->h_arrivee.mm = 20;

}

void ajouter_reservation(struct Listes *L, int id, int client, int vol, int type, int billet, int assurance){
	struct Cellule_reservation *P;
	P = (struct Cellule_reservation *)malloc(sizeof(struct Cellule_reservation));
	if(L->LR->tete == NULL){
		L->LR->tete = L->LR->queue = P;
	}else{
		L->LR->queue = L->LR->queue->suivant = P;
	}
	P->suivant = NULL;
	
	P->reservation_id = id;
	P->client_id = client;
	P->num_vol = vol;
	P->type_reservation = type;
	P->etat_billet = billet;
	P->assurance = assurance;
	P->date_reservation.jours = 8;
	P->date_reservation.mois = 2;
	P->date_reservation.annee = 2019;
	traitementArgent(L,client,vol,type,assurance);

}

void init_listes(struct Listes *L){
	ajouter_client(L->LC,1,1,3,22222222,8,2,2019,"name1","surname1");//L->LC->queue->montant_payee += 50;
	ajouter_client(L->LC,2,1,3,55555555,8,2,2019,"name2","surname2");//L->LC->queue->montant_payee += 150;
	ajouter_client(L->LC,3,0,1,99999999,8,2,2019,"name3","surname3");//L->LC->queue->montant_payee += 50;
	ajouter_client(L->LC,4,0,1,44444444,8,2,2019,"name4","surname4");//L->LC->queue->montant_payee += 0;
	
	ajouter_avion(L->LA,1,15,5);
	ajouter_avion(L->LA,2,20,0);
	ajouter_avion(L->LA,3,10,10);
	ajouter_avion(L->LA,4,40,0);
	ajouter_avion(L->LA,5,80,20);
	
	ajouter_vol(L->LV,1,1,1,590,2835,19,0,80);
	ajouter_vol(L->LV,2,2,2,216,0,19,0,80);L->LV->queue->h_depart.hh = 8;L->LV->queue->h_depart.mm = 15;L->LV->queue->h_arrivee.hh = 9;L->LV->queue->h_arrivee.mm = 50;
	ajouter_vol(L->LV,3,3,3,928,3080,19,0,80);L->LV->queue->h_depart.hh = 17;L->LV->queue->h_depart.mm = 15;L->LV->queue->h_arrivee.hh = 23;L->LV->queue->h_arrivee.mm = 5;
	ajouter_vol(L->LV,4,4,4,685,0,19,0,80);L->LV->queue->h_depart.hh = 14;L->LV->queue->h_depart.mm = 45;L->LV->queue->h_arrivee.hh = 16;L->LV->queue->h_arrivee.mm = 30;
	ajouter_vol(L->LV,5,5,5,915,4575,19,0,80);L->LV->queue->h_depart.hh = 8;L->LV->queue->h_depart.mm = 0;L->LV->queue->h_arrivee.hh = 10;L->LV->queue->h_arrivee.mm = 30;
	
	ajouter_reservation(L,1,1,1,0,1,0);L->LR->tete->date_impression.jours = 8;L->LR->tete->date_impression.mois = 2;L->LR->tete->date_impression.annee = 2019;
	ajouter_reservation(L,2,1,2,0,0,1);
	ajouter_reservation(L,3,3,2,0,0,1);
	ajouter_reservation(L,4,2,2,0,0,1);
	ajouter_reservation(L,5,4,2,0,0,0);
	ajouter_reservation(L,6,2,3,1,0,1);
	ajouter_reservation(L,7,1,4,1,0,0);
	ajouter_reservation(L,8,2,5,0,1,1);L->LR->queue->date_impression.jours = 8;L->LR->queue->date_impression.mois = 2;L->LR->queue->date_impression.annee = 2019;
}
