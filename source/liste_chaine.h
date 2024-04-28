#ifndef _LISTE_CHAINE_
#define _LISTE_CHAINE_

#include "librairies.h"


struct maillon_s {
    void* valeur;
    struct maillon_s* suivant;
};
typedef struct maillon_s maillon;

/** type liste
	- représente une liste implémentée par maillons simplement chaînés
	- mutable, homogène
*/
typedef maillon* liste;

/*----------------------------------------------
----------------- LES FONCTIONS ----------------
------------------------------------------------*/

/** fonction : creer_liste
	- entrée : rien
	- précondition : rien
	- sortie : une liste vide
	- postcondition : rien
*/
liste creer_liste(void);

/** fonction : ajouter_tete_liste
	- entrée : une valeur (type int) "c" et une liste (type liste) "l"
	- précondition : la valeur doit être un entier
	- sortie : une nouvelle liste avec la valeur ajoutée en tête (NULL si échec d'allocation mémoire)
	- postcondition : la liste "l" en paramètre n'a pas été modifiée
*/
liste ajouter_tete_liste(void*, liste);

/** fonction : est_vide_liste
	- entrée : une liste "l" (type liste)
	- précondition : "l" est une liste bon dieu
	- sortie : booléen (true si vide, false sinon)
	- postcondition : "l" non modifiée
	
*/
bool est_vide_liste(liste);

/** fonction : tete_liste
	- entrée : une liste "l" (type liste)
	- précondition :  "l" EST NON VIDE
	- sortie : la valeur de la tête (type void*)
	- postcondition : "l" est non modifiée
	- arrête le programme si les préconditions ne sont pas respectées
*/
void* tete_liste(liste);

/** fonction : queue_liste
	- entrée : une liste "l" (type liste)
	- précondition :  "l" EST NON VIDE
	- sortie : la queue de la liste (type liste), i.e tout sauf la tête
	- postcondition : "l" non modifiée
	- arrête le programme si les préconditions ne sont pas respectées
*/
liste queue_liste(liste);

/** fonction : detruire_liste
	- entrée : une liste "l" (type liste)
	- précondition :  rien
	- sortie : rien
	- postcondition : tout l'espace autrefois occupé par "l" est libéré
*/
void free_liste(liste, fct_free);

/** fonction : acceder_indice_liste
	- entrée : une liste "l" (type liste), un "indice" (type int)
	- précondition :  "l" NON VIDE && indice valable
	- sortie : valeur de l'elt d'indice dans "l"
	- postcondition : "l" non modifiée
    - arrête le programme si les préconditions ne sont pas respectées
*/
void* acceder_indice_liste(liste,int);


/** fonction : modifier_indice_liste
	- entrée : une liste "l" (type liste), un "indice" et une valeur "v" (type int)
	- précondition :  "l" NON VIDE && indice valable
	- sortie : rien
	- postcondition : "l" modifiée avec l'elt d'indice de valeur "v"
    - arrête le programme si les préconditions ne sont pas respectées
*/
void modifier_indice_liste(liste,int,void*);

/** fonction : insere_indice_liste
	- entrée : une liste "l" (type liste), un "indice" et une valeur "v" (type int)
	- précondition :  indice valable && ("l" non-vide if indice!=0)
	- sortie : nouvelle liste avec le nouveau maillon ajouté, de valeur "v"
	- postcondition : "l" non-modifiée
    - arrête le programme si les préconditions ne sont pas respectées
*/
liste insere_indice_liste(liste,int,void*);

/** fonction : supprimer_indice_liste
	- entrée : une liste "l" (type liste), un "indice" (type int)
	- précondition :  indice valable && "l" non-vide 
	- sortie : nouvelle liste avec le maillon supprimé
	- postcondition : "l" non-modifiée
    - arrête le programme si les préconditions ne sont pas respectées
*/
liste supprimer_indice_liste(liste,int);

/** fonction : longueur_liste
	- entrée : une liste "l"
	- précondition :  rien
	- sortie : taille de la liste (type int)
	- postcondition : "l" non-modifiée
*/
int longueur_liste(liste);

/** fonction : affiche_liste
	- entrée : une liste "l" (type liste)
	- précondition :  rien
	- sortie : rien, affiche juste la liste comme voulu, genre : [1;2]
	- postcondition : "l" non-modifiée
*/
void affiche_liste(liste);

/** fonction : concatener_liste
	- entrée : une liste "l1" et une liste "l2"(type liste)
	- précondition :  rien
	- sortie : nouvelle liste contenant "l1" et "l2" concaténées, i.e queue de "l1" connectée à tête "l2
	- postcondition : "l1" et "l2" non-modifiées
*/
liste concatener_liste(liste,liste);

/** fonction : appartient_liste
	- entrée : une liste "l" (type liste), un pointeur "elt" et une fonction de comparaison "comp" (type bool (*)(void*, void*))
	- précondition :  rien
	- sortie : booléen (true si elt est dans la liste, false sinon)
	- postcondition : "l" non-modifiée
*/
bool appartient_liste(void*, liste, bool (*)(void*, void*));

/** fonction : inverser_liste
	- entrée : une liste "l" (type liste) et une fonction de copie "copie_valeur" (type void* (*)(void*))
	(copie_valeur doit renvoyer NULL si l'allocation a échoué)
	un pointeur vers un booléen qui sera mis à true si tout s'est bien passé
	- précondition :  rien
	- sortie : nouvelle liste contenant les éléments de "l" dans l'ordre inverse
	- postcondition : "l" non-modifiée
*/
liste inverser_liste(liste l, void* (*copie_valeur)(void*), bool* succes);

/** fonction : copie_liste
	- entrée : une liste "l" (type liste) et une fonction de copie "copie_valeur" (type void* (*)(void*)) 
	(copie_valeur doit renvoyer NULL si l'allocation a échoué)
	un pointeur vers un booléen qui sera mis à true si tout s'est bien passé
	- précondition :  rien
	- sortie : nouvelle liste contenant les éléments de "l" copiés
	- postcondition : "l" non-modifiée
*/
liste copie_liste(liste l, void* (*copie_valeur)(void*), bool* succes);

/** fonction : i_ieme_element_liste
	- entrée : une liste "l" (type liste) et un entier "i" positif
	- précondition :  rien
	- sortie : l'élément d'indice "i" dans "l"
	- postcondition : "l" non-modifiée
*/
void* i_ieme_element_liste(liste l, int i);

#endif
