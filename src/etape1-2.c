/*******************************************************************************
		module : 	etapes1-2.c
		role   : 	permet validation des étapes 1 et 2
	 	remarque :	syntaxe IMPORT/EXPORT/LOCAL non utilisé pour distinguer l'applicatif
*******************************************************************************/

extern void affiche(const char *);

int P0() {

	affiche("debut de P0");
	// while (1);
	affiche("fin de P0, doit se detruire implicitement");
	return 0;
}

