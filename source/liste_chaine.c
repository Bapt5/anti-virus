#include "liste_chaine.h"

liste creer_liste(void){
	liste l = NULL;
	return l;
}

liste ajouter_tete_liste(void* c, liste l){
	liste new_l = malloc(sizeof(maillon));

	if (new_l == NULL) {  // Si l'allocation a échoué on retourne NULL
		return NULL;
	}

    new_l->valeur = c;
    new_l->suivant = l;
    
    return new_l;
}

bool est_vide_liste(liste l){
	return l == NULL;
}

void* tete_liste(liste l){
	assert (!est_vide_liste(l));
	return l->valeur;
}

liste queue_liste(liste l){
	assert (!est_vide_liste(l));
	return l->suivant;
}


void free_liste(liste l, fct_free free_valeur){
	if (!est_vide_liste(l)){
		free_liste(l->suivant, free_valeur);

		if (free_valeur != NULL && l->valeur != NULL) {
			free_valeur(l->valeur);
		}

		free(l);
	}
}

int longueur_liste(liste l){
	if (est_vide_liste(l)){
		return 0;
	}
	else{
		return 1 + longueur_liste(l->suivant);
	}
}

void affiche_liste_aux(liste l) {
	int* valeur = tete_liste(l);
	printf("%p", valeur);

    if (!est_vide_liste(l->suivant)) {
        printf(", ");
        affiche_liste_aux(l->suivant);
    }
}

void affiche_liste(liste l) {
    printf("[");
    if (!est_vide_liste(l)) {
        affiche_liste_aux(l);
    }
    printf("]\n");
}

liste concatener_liste(liste l1, liste l2){
	if (est_vide_liste(l1)){
		return l2;
	}
	else if (l1->suivant == NULL){
		l1->suivant = l2;
	}
	else{
		l1->suivant = concatener_liste(l1->suivant,l2);
	}
	return l1;
}

bool appartient_liste(void* c, liste l, bool (*egales)(void*, void*)){
	if (est_vide_liste(l)){
		return false;
	}
	else if (egales(c, l->valeur)){
		return true;
	}
	else{
		return appartient_liste(c, l->suivant, egales);
	}
}

liste inverser_liste(liste l, void* (*copie_valeur)(void*), bool* succes){
	*succes = true;

	liste rev_l = creer_liste();
	while (!est_vide_liste(l)) {
		if (copie_valeur != NULL) {
			rev_l = ajouter_tete_liste(copie_valeur(tete_liste(l)), rev_l);
			if (rev_l == NULL) {
				free_liste(rev_l, NULL);
				*succes = false;
				return NULL;
			}
		} else {
			rev_l = ajouter_tete_liste(tete_liste(l), rev_l);
			if (rev_l == NULL) {
				free_liste(rev_l, NULL);
				*succes = false;
				return NULL;
			}
		}

		l = queue_liste(l);
	}

	return rev_l;
}

liste copie_liste (liste l, void* (*copie_valeur)(void*), bool* succes) {
	*succes = true;

	liste l_inv = inverser_liste(l, copie_valeur, succes);
	if (!*succes) {
		return NULL;
	}

	liste l_copie = inverser_liste(l_inv, NULL, succes);
	if (!*succes) {
		return NULL;
	}

	free_liste(l_inv, NULL);
	return l_copie;
}

void* i_ieme_element_liste(liste l, int i){
	assert(i >= 0);

	if (i == 0){
		return tete_liste(l);
	}
	else{
		return i_ieme_element_liste(queue_liste(l), i-1);
	}
}