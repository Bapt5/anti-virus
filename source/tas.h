#ifndef _TAS_
#define _TAS_

#include "librairies.h"

struct tas_content {
    void* valeur;
    int priorite;
};
typedef struct tas_content tas_content;

typedef bool (*fct_comparaison)(int, int);

struct tas_s {
    tas_content* tab;
    int taille;
    int capacite;
    fct_comparaison comparaison;
};
typedef struct tas_s tas;

/*
    * Crée un tas vide.
    *
    * @param capacite: la capacité initiale du tas.
    * @param min: true si le tas est un tas minimum, false si c'est un tas maximum.
    * 
    * @return: le tas créé.
*/
tas creer_tas(int, bool);

/*
    * Vérifie si un tas est vide.
    *
    * @param tas: le tas à vérifier.
    *
    * @return: true si le tas est vide, false sinon.
*/
bool est_vide_tas(tas);

/*
    * Insère une valeur dans un tas.
    *
    * @param valeur: la valeur à insérer.
    * @param priorite: la priorité de la valeur.
    * @param tas: le tas dans lequel insérer la valeur.
    *
    * @return: true si l'insertion a réussi, false sinon.
*/
bool inserer_tas(void*, int, tas*);

/*
    * Extrait la valeur de priorité minimale d'un tas.
    *
    * @param tas: le tas duquel extraire la valeur.
    *
    * @return: la valeur extraite.
*/
void* extraire_tas(tas*);

/*
    * Libère la mémoire allouée pour un tas.
    *
    * @param tas: le tas à libérer.
    * @param f: la fonction de libération de la valeur (si NULL la valeur n'est pas libérée).
*/
void free_tas(tas*, fct_free);


#endif