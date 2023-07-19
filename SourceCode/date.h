void currentDate(struct Date* d){
	struct tm dt;
	
	time_t t = time(NULL);
	dt = *localtime(&t);
	
	d->annee = dt.tm_year+1900;
	d->mois = dt.tm_mon+1;
	d->jours = dt.tm_mday;
}

void lireDate(struct Date* d){
	int t[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; //tableau contenant le maximum des jours pour chaque mois
	struct Date now;
	
	currentDate(&now);
	
	do{
		while(!printf("\nDonner la date sous la forme jj/mm/aaaa: ") || scanf("%d/%d/%d%c",&d->jours,&d->mois,&d->annee,&term) != 4 || term != '\n'){
			printf("\nFormat de la date est invalide (contient des lettre ou n'est pas de la forme j/m/a)\n");
			while ((getchar()) != '\n');
		}
		
		if((d->annee%400 == 0) || ((d->annee%4 == 0) && (d->annee%100 != 0)))	
			t[1] = 29;	//si l'année est bissextile le mois de février aura 29 comme max
	}while((d->mois<0) || (d->mois>12) || (d->jours<0) || (d->jours>t[d->mois-1]) || (d->annee<now.annee-150) || (d->annee>now.annee+10));
	//(d->annee<now.annee-150) || (d->annee>now.annee+10): n'accepte pas une date trés ancienne(-150 ans) ou trés lointe (+10 ans)
}

void afficherDate(struct Date d, int mode){
	if(mode == 1){
		char t[12][10] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};
		printf("%d %s %d",d.jours, t[d.mois-1], d.annee);
	}else{
		printf("%02d/%02d/%d",d.jours, d.mois, d.annee);
	}
}

long date(struct Date d){
	int i, t[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	long n = d.annee*365 + d.jours;
	
	for(i=0; i<d.mois-1; i++)
		n += t[i];
	return n;
	//retourne la date en jours
}

int anneeBissextile(struct Date d){
	int annee = d.annee;
	if(d.mois <= 2)
		annee--;
	return(annee/4 - annee/100 + annee/400);
}

int diffDate(struct Date d1, struct Date d2){
	return ((date(d1)+anneeBissextile(d1))-(date(d2)+anneeBissextile(d2)));
	//retourne le nombre de jours entre 2 dates
}

void lireHeure(struct Heure* h){
	h->mm = -1; //Pour que la valeur des minutes soit fausse au cas ou l'utilisateur n'a pas saisie les minutes
	do{
		while(!printf("\nDonner l'heure de la forme hh:mm : ") || scanf("%d:%d%c",&h->hh,&h->mm,&term) != 3 || term != '\n'){
			printf("\nFormat heure invalide (contient des caracters ou n'est pas de la forme h:m)\n");
			while ((getchar()) != '\n');
		}
	}while(h->hh>23 || h->hh<0 || h->mm>59 || h->mm<0);
}

int diffHeure(struct Heure h1, struct Heure h2){
	int n1 = (h1.hh*60)+(h1.mm);
	int n2 = (h2.hh*60)+(h2.mm);
	return (n1-n2);
	// Retourne le nombre de minutes entre h1 et h2: h1-h2
}

