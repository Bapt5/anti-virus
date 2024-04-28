#include "file.h"

file creer_file(void){
	file f = {.dernier = NULL, .premier = NULL};
	return f;
}

bool est_vide_file(file f){
	return f.premier == NULL && f.dernier == NULL;
}

bool enfiler(void* c, file* f){
	maillon* m = malloc(sizeof(maillon));
    
    if (m == NULL) {
		return false;
	}

    m->valeur = c;
    m->suivant = f->dernier;

	f->dernier = m;
    
    return true;
}

void transfere (file* f) {
	assert (!est_vide_file(*f));

	while (f->dernier != NULL){
		maillon* m = f->dernier;
		f->dernier = m->suivant;
		m->suivant = f->premier;
		f->premier = m;
	}
}

void* defiler(file* f){
	assert (!est_vide_file(*f));
	
	if (f->premier == NULL) {
		transfere(f);
	}

	maillon* m = f->premier;
	f->premier = m->suivant;

	void* c = m->valeur;
	free(m);

	return c;
}


void free_file(file* f, fct_free free_valeur){
	while (!est_vide_file(*f)){
		if (free_valeur != NULL) {
			free_valeur(defiler(f));
		}
		else {
			defiler(f);
		}	
	}
}