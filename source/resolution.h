#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "librairies.h"
#include "egalite.h"
#include "jeu.h"
#include "liste_chaine.h"
#include "file.h"
#include "abr.h"
#include "tas.h"
#include "recherche.h"

#include <time.h>

/**
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
*/
liste v0(jeu, int*);

/**
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations
    
    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
        - on stocke les grilles déjà vues dans une liste pour ne pas les revoir
*/
liste v1(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise une brute force en essayant toutes les combinaisons de pièces
          à bouger
        - on stocke les grilles déjà vues dans un arbre binaire (améliore la complexité) de recherche pour
          ne pas les revoir
*/
liste v2(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on choisi aléatoirement la pièce suivante à bouger
*/
liste v3(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu sur lequel on va appliquer l'heuristique
    Retournes:
        - true si l'ajout a réussi, false sinon
    
    Retourne la distance entre la pièce à sortir et la sortie
*/
int heuristique1(jeu);

/*
    Paramètres:
        - jeu_: le jeu sur lequel on va appliquer l'heuristique

    Retourne le nombre de pièces bloquant la pièce à sortir
    Si la fonction renvoie -1, cela signifie que l'ajout des voisins a échoué
*/
int heuristique2(jeu);

/*
    Paramètres:
        - jeu_: le jeu sur lequel on va appliquer l'heuristique

    Retourne le nombre de pièces sur le chemin de la pièce à sortir dans la dernière grille de path
*/
int heuristique3(jeu);

/*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise une file de priorité pour stocker les états du jeu 
            (priorité = distance pièce à sortir jusqu'à la sortie)
*/
liste v4(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise un tas min pour stocker les états du jeu 
            (priorité = nombre de pièces bloquant la pièce à sortir)
*/
liste v5(jeu, int*);

/*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations

    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
        - 1 si une allocation a échoué

    Méthode:
        - on utilise un tas min pour stocker les états du jeu 
            (priorité = nombre de pieces plus proche de la sortie que la pièce à sortir)
*/
liste v6(jeu, int*);
#endif