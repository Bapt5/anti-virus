#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "librairies.h"
#include "egalite.h"
#include "jeu.h"
#include "liste_chaine.h"
#include "file.h"
#include "abr.h"

/**
    Paramètres:
        - jeu_: le jeu

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
*/
liste v1(jeu, int*);

/**
    Paramètres:
        - jeu_: le jeu
    
    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
        - on stocke les grilles déjà vues dans une liste pour ne pas les revoir
*/
liste v2(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
        - on stocke les grilles déjà vues dans un arbre binaire (améliore la complexité) de recherche pour
          ne pas les revoir
*/
liste v3(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution

    Méthode:
        - on choisi aléatoirement la pièce suivante à bouger
*/
liste v4(jeu, int*);

#endif