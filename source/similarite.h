#ifndef SIMILARITE
#define SIMILARITE


#include "librairies.h"
#include "jeu.h"
#include "resolution.h"

struct coordonnee_s {
    int dimension;
    int* c;
};
typedef struct coordonnee_s coordonnee_t;

/*
    * Libère la mémoire allouée pour une coordonnée.
    *
    * @param coordonnee: la coordonnée à libérer.
*/
void free_coordonnee(coordonnee_t);

typedef coordonnee_t(*fct_coordonnee)(jeu);

/*
    * Calcule la distance euclidienne entre deux coordonnées.
    *
    * @param coordonnee1: la première coordonnée.
    * @param coordonnee2: la deuxième coordonnée.
    *
    * @return: la distance euclidienne au carré entre les deux coordonnées.
*/
int distance_euclidienne(coordonnee_t, coordonnee_t);

/*
    * Retourne les coordonnées d'un jeu.
    *
    * @param j: le jeu dont on veut les coordonnées.
    *
    * @return: les coordonnées du jeu.
*/
coordonnee_t coordonnee1(jeu);

#endif