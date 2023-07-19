double benifices_agence=0,solde_agence=0; // Variable globale pour stocker le solde et les bénifices de l'agence au fure et à mesure de reservation et annulation
char term;

struct Date{
	int jours, mois, annee;
};

struct Heure{
	int hh, mm;
};

struct Cellule_client{
	int client_id, biellets_achetes, nb_reservation, tel; //nb_reservation pour determiner le nombre de réservations par un client
	double montant_payee; // Le montant que le client a payée (prix billets + frais annulation)
	char nom[50], prenom[50];
	struct Date date_reservation; // date de la 1ere réservation
	struct Cellule_client *suivant;
};

struct Liste_client{
	struct Cellule_client *tete, *queue;
};

struct Cellule_vol{
	int num_vol, avion_id, destination;
	float montant_totale_eco, montant_totale_aff, taxe, remise, frais_assurance;
	struct Date d_depart, d_arrivee;
	struct Heure h_depart, h_arrivee;
	struct Cellule_vol *suivant;
};

struct Liste_vol{
	struct Cellule_vol *tete, *queue;
};

struct Cellule_reservation{
	int reservation_id, client_id, num_vol, type_reservation, etat_billet, assurance;
	struct Date date_reservation, date_impression;
	struct Cellule_reservation *suivant;
};

struct Liste_reservation{
	struct Cellule_reservation *tete, *queue;
};

struct Cellule_avion{
	int avion_id, capacite_eco, capacite_aff;
	struct Cellule_avion *suivant;
};

struct Liste_avion{
	struct Cellule_avion *tete, *queue;
};

struct Listes{
	struct Liste_client *LC;
	struct Liste_vol *LV;
	struct Liste_reservation *LR;
	struct Liste_avion *LA;
};
